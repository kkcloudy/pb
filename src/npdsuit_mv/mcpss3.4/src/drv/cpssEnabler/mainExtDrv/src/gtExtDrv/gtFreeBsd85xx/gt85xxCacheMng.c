/*******************************************************************************
*              Copyright 2001, GALILEO TECHNOLOGY, LTD.
*
* THIS CODE CONTAINS CONFIDENTIAL INFORMATION OF MARVELL. NO RIGHTS ARE GRANTED
* HEREIN UNDER ANY PATENT, MASK WORK RIGHT OR COPYRIGHT OF MARVELL OR ANY THIRD
* PARTY. MARVELL RESERVES THE RIGHT AT ITS SOLE DISCRETION TO REQUEST THAT THIS
* CODE BE IMMEDIATELY RETURNED TO MARVELL. THIS CODE IS PROVIDED "AS IS".
* MARVELL MAKES NO WARRANTIES, EXPRESS, IMPLIED OR OTHERWISE, REGARDING ITS
* ACCURACY, COMPLETENESS OR PERFORMANCE. MARVELL COMPRISES MARVELL TECHNOLOGY
* GROUP LTD. (MTGL) AND ITS SUBSIDIARIES, MARVELL INTERNATIONAL LTD. (MIL),
* MARVELL TECHNOLOGY, INC. (MTI), MARVELL SEMICONDUCTOR, INC. (MSI), MARVELL
* ASIA PTE LTD. (MAPL), MARVELL JAPAN K.K. (MJKK), GALILEO TECHNOLOGY LTD. (GTL)
* AND GALILEO TECHNOLOGY, INC. (GTI).
********************************************************************************
* gt85xxCacheMng.c
*
* DESCRIPTION:
*       Includes cache management functions wrappers implementation for the 
*       PQ3-8548 system controller under FreeBSD
*
* DEPENDENCIES:
*       None.
*
* FILE REVISION NUMBER:
*       $Revision: 1.1.1.1 $
*
*******************************************************************************/
#include <gtExtDrv/drivers/gtCacheMng.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>

#include "kerneldrv/include/prestera_glob.h"
/* ?? */
#define PAGESIZE 4096

extern GT_32 gtPpFd;
/*******************************************************************************
* extDrvMgmtCacheFlush
*
* DESCRIPTION:
*       Flush to RAM content of cache
*
* INPUTS:
*       type        - type of cache memory data/intraction
*       address_PTR - starting address of memory block to flush
*       size        - size of memory block
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       GT_OK   - on success,
*       GT_FAIL - otherwise.
*
* COMMENTS:
*
*******************************************************************************/

GT_STATUS extDrvMgmtCacheFlush
(
    IN GT_MGMT_CACHE_TYPE_ENT   type, 
    IN void                     *address_PTR, 
    IN size_t                   size
)
{
    unsigned long address;
    GT_RANGE_STC range;

    /* page size is a power of 2 */
    /* align address to page boundery */
    address = ((unsigned long) address_PTR) & ~(PAGESIZE - 1);
    /* increase size if needed */
    size += ((unsigned long) address_PTR) - address;

    range.address = address;
    range.length = size;
    
    /* call driver function */
    if (ioctl(gtPpFd, PRESTERA_IOC_FLUSH, &range) < 0)
    {
        return GT_FAIL;
    }

    return GT_OK;
}


/*******************************************************************************
* extDrvMgmtCacheInvalidate
*
* DESCRIPTION:
*       Invalidate current content of cache
*
* INPUTS:
*       type        - type of cache memory data/intraction
*       address_PTR - starting address of memory block to flush
*       size        - size of memory block
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       GT_OK   - on success,
*       GT_FAIL - otherwise.
*
* COMMENTS:
*
*******************************************************************************/

GT_STATUS extDrvMgmtCacheInvalidate 
(
    IN GT_MGMT_CACHE_TYPE_ENT   type, 
    IN void                     *address_PTR, 
    IN size_t                   size
)
{
    unsigned long address;
    GT_RANGE_STC range;
    
    /* page size is a power of 2 */
    /* align address to page boundery */
    address = ((unsigned long) address_PTR) & ~(PAGESIZE - 1);
    /* increase size if needed */
    size += ((unsigned long) address_PTR) - address;
    
    range.address = address;
    range.length = size;
    
    /* call driver function */
    if (ioctl(gtPpFd, PRESTERA_IOC_INVALIDATE, &range) < 0)
    {
        return GT_FAIL;
    }
    return GT_OK;
}

/*******************************************************************************
* extDrvGetDmaBase
*
* DESCRIPTION:
*       Get the base address of the DMA area need for Virt2Phys and Phys2Virt
*           translaton
*
* INPUTS:
*       None.
*
* OUTPUTS:
*       dmsBase     - he base address of the DMA area.
*
* RETURNS:
*       GT_OK   - on success,
*       GT_FAIL - otherwise.
*
* COMMENTS:
*
*******************************************************************************/
GT_STATUS extDrvGetDmaBase
(
	OUT GT_U32 * dmaBase
)
{
    if (ioctl(gtPpFd, PRESTERA_IOC_GETBASEADDR, dmaBase) < 0)
    {
        return GT_FAIL;
    }
    return GT_OK;
}

/*******************************************************************************
* extDrvGetDmaSize
*
* DESCRIPTION:
*       Get the size of the DMA area
*
* INPUTS:
*       None.
*
* OUTPUTS:
*       dmaSize     - The size of the DMA area.
*
* RETURNS:
*       GT_OK   - on success,
*       GT_FAIL - otherwise.
*
* COMMENTS:
*
*******************************************************************************/
extern GT_STATUS extDrvGetDmaSize
(
	OUT GT_U32 * dmaSize
)
{
    /* Kernel driver always allocates 2M for DMA */
    *dmaSize = 2 * (1024 * 1024);
    return GT_OK;
}
