/*******************************************************************************
* cmdFileTransfer.c
*
* DESCRIPTION:
*       File transfer to/from cmdFS
*
* DEPENDENCIES:
*
* FILE REVISION NUMBER:
*       $Revision: 1.1.1.1 $
*******************************************************************************/

/***** Include files ***************************************************/

#include <cmdShell/common/cmdCommon.h>
#include <cmdShell/os/cmdStream.h>
#include <cmdShell/FS/cmdFS.h>

/***** Defines  ********************************************************/
#define     FT_CMD_BUFFER       300
#define     FT_INPUT_BUFFER     1024

#define     FT_MSG_M_HELLO              "200"
#define     FT_MSG_M_UPLOAD             "201"
#define     FT_MSG_M_GOTFILE            "202"
#define     FT_MSG_M_FILELIST           "210"
#define     FT_MSG_M_DELETED            "203"
#define     FT_MSG_M_FILE_FOLLOW        "204"
#define     FT_MSG_M_FILE_DONE          "205"
#define     FT_MSG_E_FCREATE            "501"
#define     FT_MSG_E_INPUT              "502"
#define     FT_MSG_E_DIRERR             "503"
#define     FT_MSG_E_DELETE             "504"
#define     FT_MSG_E_NOTEXISTS          "505"
#define     FT_MSG_E_FOPEN              "506"
#define     FT_MSG_E_NOTFILE            "507"


#define IS_CMD_LS(s) ( cmdOsMemCmp((s), "ls", 2) == 0 && ((s)[2] == 0 || (s)[2] == ' '))
#define CMD_LS_DATA(s) ( (s)[2] == ' ' ? (s) + 3 : NULL )
#define IS_CMD_PUSHFILE(s) ( cmdOsMemCmp((s), "push ", 5) == 0 )
#define CMD_PUSHFILE_DATA(s) ( (s) + 5 )
#define IS_CMD_DELETE(s) ( cmdOsMemCmp((s), "del ", 4) == 0 )
#define CMD_DELETE_DATA(s) ( (s) + 4 )
#define IS_CMD_GETFILE(s) ( cmdOsMemCmp((s), "get ", 4) == 0 )
#define CMD_GETFILE_DATA(s) ( (s) + 4 )

/***** Global macros ***************************************************/

/***** Private Types ***************************************************/

/***** Private Data ****************************************************/

/***** Private Functions ***********************************************/

/*******************************************************************************
* cmdFileTransferEventLoop
*
* DESCRIPTION:
*       command line shell pipeline; should be spawned as an independend thread
*
* INPUTS:
*       stream  - pointer to stream descriptor
*
* OUTPUTS:
*       None
*
* RETURNS:
*       GT_OK   - on success
*       GT_FAIL - on error
*
* COMMENTS:
*       None
*
*******************************************************************************/
static GT_STATUS cmdFileTransferEventLoop
(
    cmdStreamPTR IOStream
)
{
    char cmdBuf[FT_CMD_BUFFER];
    char buf[FT_INPUT_BUFFER];

    IOStream->flags &= ~STREAM_FLAG_I_ECHO;
    IOStream->isConsole = GT_FALSE;

    IOStream->writeLine(IOStream, FT_MSG_M_HELLO " cmdFileTransfer ready\n");

    /* accept and handle input as long as connected */
    while (IOStream->connected(IOStream))
    {
        if (IOStream->readLine(IOStream, cmdBuf, sizeof(cmdBuf)) < 0)
        {
            /* input faled, possible connection failed */
            break;
        }
        if (!cmdBuf[0])
        {
            continue;
        }
        if (IS_CMD_PUSHFILE(cmdBuf))
        {
            char *p;
            size_t filesize;
            int fd;
            /* parse line: size then filename */
            p = CMD_PUSHFILE_DATA(cmdBuf);
            while (*p == ' ' || *p == '\t')
                p++;
            /* size */
            for (filesize = 0; *p && *p != ' ' && *p != '\t'; p++)
            {
                if (*p >= '0' && *p <= '9')
                    filesize = filesize * 10 + (*p - '0');
            }
            /* skip spaces before file name */
            while (*p == ' ' || *p == '\t')
                p++;
            /*TODO: remove trailing spaces */
            fd = cmdFSopen(p, CMD_FS_O_WRONLY | CMD_FS_O_CREAT | CMD_FS_O_TRUNC);

            if (fd < 0)
            {
                IOStream->writeLine(IOStream, FT_MSG_E_FCREATE " failed to open/create file\n");
                continue;
            }
            /* read \n from stream if one */
            IOStream->readChar(IOStream, buf, 0);
            IOStream->writeLine(IOStream, FT_MSG_M_UPLOAD " Upload file now\n");
            while (filesize)
            {
                int k;
                k = sizeof(buf);
                if (k > (int)filesize)
                    k = filesize;
                k = IOStream->read(IOStream, buf, k);
                if (k < 0)
                {
                    IOStream->writeLine(IOStream, FT_MSG_E_INPUT " failed to read input\n");
                    break;
                }
                cmdFSwrite(fd, buf, k);
                filesize -= k;
            }
            cmdFSclose(fd);
            if (filesize == 0)
            {
                /* file completely uploaded */
                IOStream->writeLine(IOStream, FT_MSG_M_GOTFILE " File upload successful\n");
            }
            else
            {
                /* unlink file */
                cmdFSunlink(p);
            }
            continue;
        }
        if (IS_CMD_LS(cmdBuf))
        {
            CMD_FS_DIRENT_STC   dirent;
            CMD_FS_STAT_STC     stat;
            int                 nfiles = 0;
            int                 nbytes = 0;
            int                 dirFd;
            int                 rc;

            dirFd = cmdFSopendir(CMD_LS_DATA(cmdBuf));
            if (dirFd < 0)
            {
                IOStream->writeLine(IOStream, FT_MSG_E_DIRERR " failed to open directory\n");
                continue;
            }
            IOStream->writeLine(IOStream, FT_MSG_M_FILELIST "- file listing follows\n");
            rc = 1;
            while (rc == 1)
            {
                 rc = cmdFSreaddir(dirFd, &dirent);
                 if (rc == 1)
                 {
                     if (dirent.d_name[0] == '.')
                     {
                         /* hidden */
                         continue;
                     }
                     if (CMD_LS_DATA(cmdBuf))
                     {
                         cmdOsStrCpy(buf, CMD_LS_DATA(cmdBuf));
                         cmdOsStrCat(buf, "/");
                         cmdOsStrCat(buf, dirent.d_name);
                     }
                     else
                     {
                         cmdOsStrCpy(buf, dirent.d_name);
                     }
                     if (cmdFSstat(buf, &stat) < 0)
                         continue;
                     if (CMD_FS_S_ISDIR(stat.st_mode))
                     {
                         nfiles++;
                         cmdOsSprintf(buf, " %-30s <DIR>\n",dirent.d_name);
                         IOStream->writeLine(IOStream, buf);
                         continue;
                     }
                     /* print file name and size */
                     nfiles++;
                     nbytes += stat.st_size;
                     cmdOsSprintf(buf, " %-30s %d\n",dirent.d_name, stat.st_size);
                     IOStream->writeLine(IOStream, buf);
                 }
            }
            cmdFSclosedir(dirFd);
            cmdOsSprintf(buf, FT_MSG_M_FILELIST " %d files, %d bytes\n", nfiles, nbytes);
            IOStream->writeLine(IOStream, buf);
            continue;
        }
        if (IS_CMD_DELETE(cmdBuf))
        {
            char *p;
            /* skip spaces before file name */
            p = CMD_DELETE_DATA(cmdBuf);
            while (*p == ' ' || *p == '\t')
                p++;
            /*TODO: remove trailing spaces */
            if (cmdFSunlink(p) == 0)
            {
                IOStream->writeLine(IOStream, FT_MSG_M_DELETED " file detele successful\n");
            }
            else
            {
                IOStream->writeLine(IOStream, FT_MSG_E_DELETE " failed to delete: ");
                IOStream->writeLine(IOStream, cmdFSlastError());
                IOStream->writeLine(IOStream, "\n");
            }
            continue;
        }
        if (IS_CMD_GETFILE(cmdBuf))
        {
            char                *p;
            int                 fd;
            CMD_FS_STAT_STC     stat;
            /* skip spaces before file name */
            p = CMD_DELETE_DATA(cmdBuf);
            while (*p == ' ' || *p == '\t')
                p++;
            /*TODO: remove trailing spaces */
            if (cmdFSstat(p, &stat) != 0)
            {
                IOStream->writeLine(IOStream, FT_MSG_E_NOTEXISTS " file not exists\n");
                continue;
            }
            if (!CMD_FS_S_ISREG(stat.st_mode))
            {
                IOStream->writeLine(IOStream, FT_MSG_E_NOTFILE " file not exists\n");
                continue;
            }
            fd = cmdFSopen(p, CMD_FS_O_RDONLY);
            if (fd < 0)
            {
                IOStream->writeLine(IOStream, FT_MSG_E_FOPEN " failed to open file: ");
                IOStream->writeLine(IOStream, cmdFSlastError());
                IOStream->writeLine(IOStream, "\n");
                continue;
            }
            /* transter file */
            cmdOsSprintf(buf, FT_MSG_M_FILE_FOLLOW " %d bytes follows\n", stat.st_size);
            IOStream->writeLine(IOStream, buf);
            while (IOStream->connected(IOStream))
            {
                int k;
                k = cmdFSread(fd, buf, sizeof(buf));
                if (k <= 0)
                    break;
                IOStream->write(IOStream, buf, k);
            }
            IOStream->writeLine(IOStream, FT_MSG_M_FILE_DONE " file transfer done\n");
            continue;
        }
        IOStream->writeLine(IOStream, "500 wrong command: ");
        IOStream->writeLine(IOStream, cmdBuf);
        IOStream->writeLine(IOStream, "\n");
    }

    IOStream->destroy(IOStream);
    return GT_OK;
}


/***** Public Functions ************************************************/

/*******************************************************************************
* cmdFileTransferServiceStart
*
* DESCRIPTION:
*       Start file transfer service
*
* INPUTS:
*       stream  - pointer to stream descriptor
*
* OUTPUTS:
*       None
*
* RETURNS:
*       GT_OK   - on success
*       GT_FAIL - on error
*
* COMMENTS:
*       None
*
*******************************************************************************/
GT_STATUS cmdFileTransferServiceStart(void)
{
    GT_STATUS rc;
    GT_TASK   tid;

    rc = cmdStreamSocketCreateListenerTask(
                "FTSERVER",
                NULL, 12346/*port*/,
                "FTSERVER_",
                cmdFileTransferEventLoop,
                GT_TRUE/*multipleInstances*/,
                GT_FALSE/*isConsole*/,
                &tid);
    if (rc != GT_OK)
    {
        cmdOsPrintf("commander: failed to create LUA CLI listening task\n");
    }
    return rc;
}
