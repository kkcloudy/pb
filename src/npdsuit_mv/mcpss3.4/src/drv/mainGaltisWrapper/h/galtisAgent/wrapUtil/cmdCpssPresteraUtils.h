/*******************************************************************************
*                Copyright 2001, Marvell International Ltd.
* This code contains confidential information of Marvell semiconductor, inc.
* no rights are granted herein under any patent, mask work right or copyright
* of Marvell or any third party.
* Marvell reserves the right at its sole discretion to request that this code
* be immediately returned to Marvell. This code is provided "as is".
* Marvell makes no warranties, express, implied or otherwise, regarding its
* accuracy, completeness or performance.
********************************************************************************
* cmdPresteraUtils.h
*
* DESCRIPTION:
*       galtis agent interface to prestera specific functionality
*
* DEPENDENCIES:
*
* FILE REVISION NUMBER:
*       $Revision: 1.1.1.1 $
*******************************************************************************/

#ifndef __cmdPresteraUtils_h__
#define __cmdPresteraUtils_h__

/***** Include files ***************************************************/
#include <cpssCommon/cpssPresteraDefs.h>
#include <cpss/generic/cpssTypes.h>
#include <cmdShell/common/cmdCommon.h>
#include <cmdShell/common/cmdWrapUtils.h>
#include <cpss/generic/config/private/prvCpssConfigTypes.h>

#ifdef  __cplusplus
    extern "C" {
#endif

/*
 * Typedef: struct GT_BYTE_ARRY
 *
 * Description:
 *    This structure contains byte array data and length.
 *
 * Fields:
 *    data   - pointer to allocated data buffer
 *    length - buffer length
 */
typedef struct
{
    GT_U8      *data;
    GT_U32      length;
} GT_BYTE_ARRY;

/* Wrapper related types for union fields*/
typedef GT_U8 GT_U8_UNION;

typedef GT_U16 GT_U16_UNION;

typedef GT_U32 GT_U32_UNION;

typedef GT_BOOL GT_BOOL_UNION;

/* Get the portGroupsBmp for multi port groups device.
   when 'enabled' --> wrappers will use the APIs with portGroupsBmp parameter*/
#define UTIL_MULTI_PORT_GROUPS_BMP_GET(device,enable,portGroupBmp)  \
    utilMultiPortGroupsBmpGet(device,&enable,&portGroupBmp);

/*
 * Typedef: struct GT_UTIL_STC_ELEMENT_PATH_DSC_STC
 *
 * Description:
 *    This record is a description of the structure element.
 *    The structure represented by array of such records -
 *    record for each structure member
 *    An array terminated by record with the namePtr==NULL.
 *    An application can use the larger record (the structure containing
 *    this as a first member) as derived class. The search function has
 *    the special "description record size" parameter to follow the array.
 *
 * Fields:
 *    namePtr    - pointer to the expose name of a structure element
 *    subtreePtr - pointer to araay of substructure elements descriptors
 *                 NULL if the element is not substructure
 *    offset     - offset of the element from the structure origin
 */
typedef struct _stc_GT_UTIL_STC_ELEMENT_DSC_STC
{
    GT_U8                                        *namePtr;
    struct _stc_GT_UTIL_STC_ELEMENT_DSC_STC      *subtreePtr;
    GT_U32                                       offset;
} GT_UTIL_STC_ELEMENT_DSC_STC;

/*
*    The regular descriptor record
*  parameters:
*  _struct_type    - the typedef defined strurcture name
*  _element_name   - the name of structure element used
*                    in "C" language definition
*  _element_expose - the name of element used in "path"
*  _subtree_ptr    - pointer to array that describes the substructure elements
*                    for scalar element NULL
*
*/
#define GT_UTIL_STC_ELEMENT_DSC_MAC(                               \
    _struct_type, _element_name, _element_expose, _subtree_ptr)    \
    {                                                              \
        _element_expose,                                           \
        (struct _stc_GT_UTIL_STC_ELEMENT_DSC_STC*)_subtree_ptr,    \
        ((char*)&(((_struct_type*)0)->_element_name) - (char*)0)   \
    }

/* the end of structure element descriptors list */
#define GT_UTIL_STC_ELEMENT_DSC_END_LIST_CNS                       \
    { (GT_U8*)0, (struct _stc_GT_UTIL_STC_ELEMENT_DSC_STC*)0,  0}

/*
 * Typedef: enum GT_UTIL_HEX_DATA_STORE_FORMAT_ENT
 *
 * Description: Hex data store formats
 *
 * Enumerations:
 *    GT_UTIL_HEX_DATA_STORE_NETWORK_ORDERED_E  - in network order
 *    GT_UTIL_HEX_DATA_STORE_REVERSE_ORDERED_E  - in order reverse to network.
 *    GT_UTIL_HEX_DATA_STORE_CPU_INTEGER_E      - each byte added by rule
 *                                                num = (num << 8) * next
 */
typedef enum
{
    GT_UTIL_HEX_DATA_STORE_NETWORK_ORDERED_E,
    GT_UTIL_HEX_DATA_STORE_REVERSE_ORDERED_E,
    GT_UTIL_HEX_DATA_STORE_CPU_INTEGER_E
} GT_UTIL_HEX_DATA_STORE_FORMAT_ENT;

/*******************************************************************************
* galtisMacAddr
*
* DESCRIPTION:
*       Create Ethernet MAC Address from hexadecimal coded string
*       6 elements, string size = 12 bytes
*
* INPUTS:
*       source - hexadecimal coded string reference
*
* OUTPUTS:
*       dest - pointer to GT_ETHERADDR structure
*
* RETURNS:
*       None
*
* COMMENTS:
*       no assertion is performed on validity of coded string
*
* Toolkit:
*
*******************************************************************************/
GT_VOID galtisMacAddr
(
    OUT GT_ETHERADDR *dest,
    IN  GT_U8        *source
);

/*******************************************************************************
* galtisIpAddr
*
* DESCRIPTION:
*       Create IPv4 Address from hexadecimal coded string
*       4 elements, string size = 8 bytes
*
* INPUTS:
*       source - hexadecimal coded string reference
*
* OUTPUTS:
*       dest - pointer to GT_IPADDR structure
*
* RETURNS:
*       None
*
* COMMENTS:
*       no assertion is performed on validity of coded string
*
* Toolkit:
*
*******************************************************************************/
GT_VOID galtisIpAddr
(
    OUT GT_IPADDR *dest,
    IN  GT_U8     *source
);

/*******************************************************************************
* galtisIpv6Addr
*
* DESCRIPTION:
*       Create IPv6 Address from hexadecimal coded string of
*       16 elements, string size = 32 bytes
*
* INPUTS:
*       source - hexadecimal coded string reference
*
* OUTPUTS:
*       dest   - pointer to GT_IPV6ADDR structure
*
* RETURNS:
*       None
*
* COMMENTS:
*       no assertion is performed on validity of coded string
*
*******************************************************************************/
GT_VOID galtisIpv6Addr
(
    OUT GT_IPV6ADDR *dest,
    IN  GT_U8     *source
);

/*******************************************************************************
* galtisU64
*
* DESCRIPTION:
*       Create GT_U64 structure from two elements
*
* INPUTS:
*       lo, hi - GT_U64 elements
*
* OUTPUTS:
*       big    - initialized GT_U64 structure
*
* RETURNS:
*       None
*
* COMMENTS:
*
* Toolkit:
*
*******************************************************************************/
GT_VOID galtisU64
(
    OUT GT_U64 *big,
    IN  GT_U32 lo,
    IN  GT_U32 hi
);

/*******************************************************************************
* galtisU64COut
*
* DESCRIPTION:
*       Create Galtis compatible string output from GT_U64 for OUTPUT
*
* INPUTS:
*       big - 64-bit integer (GT_U64)
*
* OUTPUTS:
*       None
*
* RETURNS:
*       galtis output string that represent the GT_U64 value
*
* COMMENTS:
*
* Toolkit:
*
*******************************************************************************/
GT_U8* galtisU64COut
(
    IN  GT_U64 *big
);

/*******************************************************************************
* galtisU64FOut
*
* DESCRIPTION:
*       Create Galtis compatible string output from GT_U64 for FIELDS
*
* INPUTS:
*       big - 64-bit integer (GT_U64)
*
* OUTPUTS:
*       None
*
* RETURNS:
*       galtis output string that represent the GT_U64 value as table fileds
*
* COMMENTS:
*
* Toolkit:
*
*******************************************************************************/
GT_U8* galtisU64FOut
(
    IN GT_U64 *big
);

/*******************************************************************************
* galtisBArrayOut
*
* DESCRIPTION:
*       Create Galtis compatible string output from char buffer
*
* INPUTS:
*       bArray - the buffer data and length in a struct
*
* OUTPUTS:
*       None
*
* RETURNS:
*       galtis output string that represent the buffer data
*
* COMMENTS:
*
* Toolkit:
*
*******************************************************************************/
GT_U8* galtisBArrayOut
(
    IN GT_BYTE_ARRY* bArray
);

/*******************************************************************************
* galtisBArray
*
* DESCRIPTION:
*       build GT_BYTE_ARRY from string
*
* INPUTS:
*       source - byte array buffer
*
* OUTPUTS:
*       byteArray - byte array data and size
*
* RETURNS:
*       None
*
* COMMENTS:
*
* Toolkit:
*
*******************************************************************************/
GT_VOID galtisBArray
(
    OUT GT_BYTE_ARRY *byteArray,
    IN  GT_U8* source
);

/*******************************************************************************
* galtisBArraySized
*
* DESCRIPTION:
*       build GT_BYTE_ARRY from string expanding or trunkating to size
*
* INPUTS:
*       source      - byte array buffer
*       size        - exact size in bytes
*       defDataSize - size of default data
*       defData     - default data for bytes, not specified in source
*
* OUTPUTS:
*       byteArray   - byte array data and size
*
* RETURNS:
*       None
*
* COMMENTS:
*
*******************************************************************************/
GT_VOID galtisBArraySized
(
    OUT GT_BYTE_ARRY *byteArray ,
    IN GT_U8* source,
    IN GT_U32 size,
    IN GT_U32 defDataSize,
    IN GT_U8* defData
) ;

/*******************************************************************************
* galtisBArrayFree
*
* DESCRIPTION:
*       Free GT_BYTE_ARRY from string
*
* INPUTS:
*       source - byte array buffer
*
* OUTPUTS:
*       none
*
* RETURNS:
*       None
*
* COMMENTS:
*
* Toolkit:
*
*******************************************************************************/
GT_VOID galtisBArrayFree
(
    IN GT_BYTE_ARRY *byteArray
);

/*******************************************************************************
* galtisIsMacGreater
*
* DESCRIPTION:
*      return true if one mac address is greater than the other.
*
* INPUTS:
*      firstMac  - first MAC address.
*      secondMac - second MAC address.
*
* OUTPUTS:
*      None.
*
* RETURNS:
*      true if greater.
*
* COMMENTS:
*      None.
*
*******************************************************************************/
GT_BOOL galtisIsMacGreater
(
    IN GT_ETHERADDR firstMac,
    IN GT_ETHERADDR secondMac
);

/*******************************************************************************
* gtPortsMapDbEnable
*
* DESCRIPTION:
*       Enables ports mapping DB lookup
*
* INPUTS:
*       none
*
* OUTPUTS:
*       none
*
* RETURNS:
*       none
*
* COMMENTS:
*
*
*******************************************************************************/
GT_VOID gtPortsMapDbEnable(GT_VOID);


/*******************************************************************************
* gtPortsMapDbDisable
*
* DESCRIPTION:
*       Disables ports mapping DB lookup
*
* INPUTS:
*       none
*
* OUTPUTS:
*       none
*
* RETURNS:
*       none
*
* COMMENTS:
*
*
*******************************************************************************/
GT_VOID gtPortsMapDbDisable(GT_VOID);

/*******************************************************************************
* gtIsPortsMapDbEnable
*
* DESCRIPTION:
*       Enables ports mapping DB lookup
*
* INPUTS:
*       none
*
* OUTPUTS:
*       none
*
* RETURNS:
*       none
*
* COMMENTS:
*
*
*******************************************************************************/
GT_BOOL gtIsPortsMapDbEnable(GT_VOID);



/*******************************************************************************
* gtIsPortsMapConvert
*
* DESCRIPTION:
*       Do ports mapping.
*
* INPUTS:
*       none
*
* OUTPUTS:
*       none
*
* RETURNS:
*       none
*
* COMMENTS:
*
*
*******************************************************************************/
void gtPortsDevConvert
(
   INOUT GT_U8 *devNum,
   INOUT GT_U8 *portNum
);

void gtPortsDevConvertDataBack
(
   INOUT GT_U8 *devNum,
   INOUT GT_U8 *portNum
);

void gtPortsBitmapDevConvert
(
   INOUT GT_U8              *devNum,
   INOUT CPSS_PORTS_BMP_STC *portBitMap
);

void gtPortsBitmapDevConvertDataBack
(
   INOUT GT_U8              *devNum,
   INOUT CPSS_PORTS_BMP_STC *portBitMap
);

/* flag for debug to remove the hwDevNum convert*/
extern GT_BOOL gtHwDevNumConvert;
/* enable/disable the hwDevNum convert .
function sets the flag of gtHwDevNumConvert */
GT_STATUS gtHwDevNumConvertSet
(
   IN GT_BOOL enableConvert
);

/* Don't do casting to GT_U8 for devNum/portNum. They must be of this type already*/
/* CONVERT_DEV_PORT_MAC  :
    convert dev,port when 'dev' is the first input parameter for the CPSS API's.
    so dev is not used as 'Field in HW' and not as 'index in HW table'

    meaning when calling
     cpssYyyFunc(devNum,PortNum,enable); --> need to use CONVERT_DEV_PORT_MAC

     but for :
     fdbEntry.interface.type = 'port';
     the pair of :
     fdbEntry.interface.devPort.devNum,fdbEntry.interface.devPort.portNum -->
     need convert by CONVERT_DEV_PORT_DATA_MAC

     cpssZzzFunc(devNum,&fdbEntry); --> need to use CONVERT_DEV_PORT_KEY_MAC

     and also for function that uses 'interface' as index into HW like

     cpssMmmFunc(devNum , &interface , data) and for :
     fdbEntry.interface.type = 'port';
     fdbEntry.interface.devPort.devNum,fdbEntry.interface.devPort.portNum -->
     used as INDEX in HW --> need convert by CONVERT_DEV_PORT_DATA_MAC

     NOTE: this macro NOT need 'Convert back' because this is 'key' that always
           given from caller and never returned from wrapper

*/
#define CONVERT_DEV_PORT_MAC(devNum, portNum) \
{                                    \
   gtPortsDevConvert(&devNum, &portNum);      \
}


/* CONVERT_DEV_PORT_DATA_MAC :
    convert dev,port when those are used as 'Field in HW' or as 'index in HW table'
    and dev is not the first input parameter for the CPSS API's

    meaning when calling
     fdbEntry.interface.type = 'port';
     the pair of :
     fdbEntry.interface.devPort.devNum,fdbEntry.interface.devPort.portNum -->
     need convert by CONVERT_DEV_PORT_DATA_MAC

     cpssZzzFunc(devNum,&fdbEntry); --> need to use CONVERT_DEV_PORT_KEY_MAC

     and also for function that uses 'interface' as index into HW like

     cpssMmmFunc(devNum , &interface , data) and for :
     fdbEntry.interface.type = 'port';
     fdbEntry.interface.devPort.devNum,fdbEntry.interface.devPort.portNum -->
     used as INDEX in HW --> need convert by CONVERT_DEV_PORT_DATA_MAC


     but for :
     cpssYyyFunc(devNum,PortNum,enable); --> need to use CONVERT_DEV_PORT_MAC

*/
#define CONVERT_DEV_PORT_DATA_MAC(devNum, portNum) \
{                                               \
    gtPortsDevConvert(&devNum, &portNum);       \
    /* do convert to HW id only after SW convert */ \
    if(gtHwDevNumConvert == GT_TRUE && PRV_CPSS_IS_DEV_EXISTS_MAC(devNum))      \
       devNum = PRV_CPSS_HW_DEV_NUM_MAC(devNum);\
}

/* CONVERT_BACK_DEV_PORT_DATA_MAC :
    convert back from dev,port of CPSS to this of RDE test.
    the convert back needed ONLY for the cases that used the 'CONVERT_DEV_PORT_DATA_MAC'
*/
#define CONVERT_BACK_DEV_PORT_DATA_MAC(devNum, portNum) \
{                                                  \
   gtPortsDevConvertDataBack(&devNum, &portNum);       \
}

/* CONVERT_DEV_PORTS_BMP_MAC :
    convert to dev,portBmp (CPSS_PORTS_BMP_STC) of CPSS from that of RDE test.
    the convert back is done by 'CONVERT_BACK_DEV_PORTS_BMP_MAC'
*/
#define CONVERT_DEV_PORTS_BMP_MAC(devNum, portsBmp) \
{                                                   \
   gtPortsBitmapDevConvert(&devNum, &portsBmp);     \
}

/* CONVERT_BACK_DEV_PORTS_BMP_MAC :
    convert back from dev,portBmp (CPSS_PORTS_BMP_STC) of CPSS to that of RDE test.
    the convert back needed ONLY for the cases that used the 'CONVERT_DEV_PORTS_BMP_MAC'
*/
#define CONVERT_BACK_DEV_PORTS_BMP_MAC(devNum, portsBmp)    \
{                                                           \
   gtPortsBitmapDevConvertDataBack(&devNum, &portsBmp);      \
}


/*******************************************************************************
* gtTrunkConvertFromTestToCpss
*
* DESCRIPTION:
*       convert trunkId from test value (caller of gatlis wrapper to SET value)
*       to value that the CPSS expect
*
* INPUTS:
*       trunkIdPtr - (pointer to) the trunkId of the test
*
* OUTPUTS:
*       trunkIdPtr - (pointer to) the trunkId of the CPSS
*
* RETURNS:
*       none
*
* COMMENTS:
*
*
*******************************************************************************/
void gtTrunkConvertFromTestToCpss
(
   INOUT GT_TRUNK_ID *trunkIdPtr
);

/*******************************************************************************
* gtTrunkConvertFromTestToCpss
*
* DESCRIPTION:
*       convert trunkId from CPSS value to value that the TEST expect (refresh)
*
* INPUTS:
*       trunkIdPtr - (pointer to) the trunkId of the CPSS
*
* OUTPUTS:
*       trunkIdPtr - (pointer to) the trunkId of the test
*
* RETURNS:
*       none
*
* COMMENTS:
*
*
*******************************************************************************/
void gtTrunkConvertFromCpssToTest
(
   INOUT GT_TRUNK_ID *trunkIdPtr
);

/* Don't do casting to GT_TRUNK_ID for trunkId. it must be of this type already */
#define CONVERT_TRUNK_ID_TEST_TO_CPSS_MAC(_trunkId) \
   gtTrunkConvertFromTestToCpss(&_trunkId)

#define CONVERT_TRUNK_ID_CPSS_TO_TEST_MAC(_trunkId) \
   gtTrunkConvertFromCpssToTest(&_trunkId)

/* flag to state that the DXCH (xcat) devices need the WA of trunk-Id conversions */
extern GT_BOOL dxChTrunkWaNeeded;

/*******************************************************************************
* gtPclMaskTrunkConvertFromTestToCpss
*
* DESCRIPTION:
*       for PCL MASK trunkId field -- when <isTrunk> has 'exact match' mask and pattern is 1
*
*       convert trunkId from test value (caller of gatlis wrapper to SET value)
*       to value that the CPSS expect
*
* INPUTS:
*       isTrunkMask - <is trunk> in the mask
*       isTrunkPattern - <is trunk> in the pattern
*       trunkIdPtr - (pointer to) the trunkId of the test
*
* OUTPUTS:
*       trunkIdPtr - (pointer to) the trunkId of the CPSS
*
* RETURNS:
*       none
*
* COMMENTS:
*
*
*******************************************************************************/
void gtPclMaskTrunkConvertFromTestToCpss
(
    IN    GT_U32        isTrunkMask,
    IN    GT_U32        isTrunkPattern,
    INOUT GT_TRUNK_ID   *trunkIdPtr
);

/*******************************************************************************
* gtPclMaskTrunkConvertFromCpssToTest
*
* DESCRIPTION:
*       for PCL MASK trunkId field -- when <isTrunk> has 'exact match' mask and pattern is 1
*
*       convert trunkId from CPSS value to value that the TEST expect (refresh)
*
* INPUTS:
*       isTrunkMask - <is trunk> in the mask
*       isTrunkPattern - <is trunk> in the pattern
*       trunkIdPtr - (pointer to) the trunkId of the CPSS
*
* OUTPUTS:
*       trunkIdPtr - (pointer to) the trunkId of the test
*
* RETURNS:
*       none
*
* COMMENTS:
*
*
*******************************************************************************/
void gtPclMaskTrunkConvertFromCpssToTest
(
    IN    GT_U32        isTrunkMask,
    IN    GT_U32        isTrunkPattern,
    INOUT GT_TRUNK_ID *trunkIdPtr
);

/*******************************************************************************
* gtPclPatternTrunkConvertFromTestToCpss
*
* DESCRIPTION:
*       for PCL Pattern trunkId field -- when <isTrunk> has 'exact match' mask and pattern is 1
*
*       convert trunkId from test value (caller of gatlis wrapper to SET value)
*       to value that the CPSS expect
*
* INPUTS:
*       isTrunkMask - <is trunk> in the mask
*       isTrunkPattern - <is trunk> in the pattern
*       trunkIdPtr - (pointer to) the trunkId of the test
*
* OUTPUTS:
*       trunkIdPtr - (pointer to) the trunkId of the CPSS
*
* RETURNS:
*       none
*
* COMMENTS:
*
*
*******************************************************************************/
void gtPclPatternTrunkConvertFromTestToCpss
(
    IN    GT_U32        isTrunkMask,
    IN    GT_U32        isTrunkPattern,
    INOUT GT_TRUNK_ID   *trunkIdPtr
);

/*******************************************************************************
* gtPclPatternTrunkConvertFromCpssToTest
*
* DESCRIPTION:
*       for PCL Pattern trunkId field -- when <isTrunk> has 'exact match' mask and pattern is 1
*
*       convert trunkId from CPSS value to value that the TEST expect (refresh)
*
* INPUTS:
*       isTrunkMask - <is trunk> in the mask
*       isTrunkPattern - <is trunk> in the pattern
*       trunkIdPtr - (pointer to) the trunkId of the CPSS
*
* OUTPUTS:
*       trunkIdPtr - (pointer to) the trunkId of the test
*
* RETURNS:
*       none
*
* COMMENTS:
*
*
*******************************************************************************/
void gtPclPatternTrunkConvertFromCpssToTest
(
    IN    GT_U32        isTrunkMask,
    IN    GT_U32        isTrunkPattern,
    INOUT GT_TRUNK_ID *trunkIdPtr
);


/*******************************************************************************
* gtPclMaskPclIdConvertFromTestToCpss
*
* DESCRIPTION:
*       convert pclId of the pcl mask from test value (caller of gatlis wrapper to SET value)
*       to value that the CPSS expect
*
*       NOTE: call this only for short keys (24B),not needed for larger keys.
*
* INPUTS:
*       pclIdPtr - (pointer to) the pclId of the test
*
* OUTPUTS:
*       pclIdPtr - (pointer to) the pclId of the CPSS
*
* RETURNS:
*       none
*
* COMMENTS:
*
*
*******************************************************************************/
void gtPclMaskPclIdConvertFromTestToCpss
(
   INOUT GT_U16 *pclIdPtr
);

/* Don't do casting to GT_U16 for pclId. it must be of this type already */
#define CONVERT_PCL_MASK_PCL_ID_TEST_TO_CPSS_MAC(_pclId) \
   gtPclMaskPclIdConvertFromTestToCpss(&_pclId)

/*******************************************************************************
* gtTrunkWaForceDownSet
*
* DESCRIPTION:
*       set if the trunk WA need to be forced down (disabled)
*
* INPUTS:
*       forceDown - do we want to force down the WA
*               GT_TRUE - force down (disable) the WA
*               GT_FALSE - don't force down (allow to enable) the WA
*
* OUTPUTS:
*
*
* RETURNS:
*       GT_OK - success
*
* COMMENTS:
*       this function is to be called from the terminal -->
*       AFTER cpssInitSystem(...)
*
*******************************************************************************/
GT_STATUS gtTrunkWaForceDownSet
(
    IN GT_U32    forceDown
);


/*******************************************************************************
* utilFindStructureElenentDescriptorByPath
*
* DESCRIPTION:
*       searches the structure element descriptor by path
*
* INPUTS:
*       startDscArr   - (pointer to) array of descriptors for root structure
*       dscEntrySize  - size of descriptor entry in bytes
*                       dedicated to application definded descrioprs that
*                       contain the GT_UTIL_STC_ELEMENT_DSC_STC as first member
*                       0 means sizeof(GT_UTIL_STC_ELEMENT_DSC_STC)
*       nameSeparator - character used as separator in the path
*       pathStr          - string that contains the path of the searched field
*
* OUTPUTS:
*       foundDscPtrPtr   - (pointer to) pointer to found descriptor entry
*       summaryOffsetPtr - (pointer to) summary offset of the root structure origin
*
* RETURNS:
*       GT_OK      - found
*       GT_NO_SUCH - not found
*
* COMMENTS:
*
*******************************************************************************/
GT_STATUS utilFindStructureElenentDescriptorByPath
(
    IN  GT_UTIL_STC_ELEMENT_DSC_STC   startDscArr[],
    IN  GT_U32                        dscEntrySize,
    IN  GT_U8                         nameSeparator,
    IN  GT_U8                         *pathStr,
    OUT GT_UTIL_STC_ELEMENT_DSC_STC   **foundDscPtrPtr,
    OUT GT_U32                        *summaryOffsetPtr
);

/*******************************************************************************
* utilHexStringToStcField
*
* DESCRIPTION:
*       Convers the string that contains pairs of hex numbers to the
*       contents of the "C" language defined data
*
* INPUTS:
*       dataStr        - (pointer to) string with pairs of hex numbers
*       dataBytesSize  - amount of bytes to store
*       storeFormat    - the format conversion rule used during the strore
*
* OUTPUTS:
*       fieldPtr   - (pointer to) the field tat the data stored to
*
* RETURNS:
*       GT_OK        - OK
*       GT_BAD_PARAM - wrong hex data in the string
*
* COMMENTS:
*
*******************************************************************************/
GT_STATUS utilHexStringToStcField
(
    IN  GT_U8                              *dataStr,
    IN  GT_U32                             dataBytesSize,
    IN  GT_UTIL_HEX_DATA_STORE_FORMAT_ENT  storeFormat,
    OUT GT_VOID                            *fieldPtr
);

/*******************************************************************************
* utilMultiPortGroupsBmpSet
*
* DESCRIPTION:
*       set the portGroupsBmp for multi port groups device.
*       when 'enabled' --> wrappers will use the APIs with portGroupsBmp parameter
*
* INPUTS:
*       devNum - device number
*       enable - enable / disable the use of APIs with portGroupsBmp parameter.
*       portGroupsBmp - port groups bmp , relevant only when enable = GT_TRUE
*
* OUTPUTS:
*       none
*
* RETURNS:
*       GT_OK        - OK
*       GT_BAD_PARAM - on bad device number
*
* COMMENTS:
*
*******************************************************************************/
GT_STATUS utilMultiPortGroupsBmpSet
(
    IN  GT_U8               devNum,
    IN  GT_BOOL             enable,
    IN  GT_PORT_GROUPS_BMP  portGroupsBmp
);

/*******************************************************************************
* utilMultiPortGroupsBmpGet
*
* DESCRIPTION:
*       Get the portGroupsBmp for multi port groups device.
*       when 'enabled' --> wrappers will use the APIs with portGroupsBmp parameter
*
* INPUTS:
*       devNum - device number
*
* OUTPUTS:
*       enablePtr - (pointer to)enable / disable the use of APIs with portGroupsBmp parameter.
*       portGroupsBmpPtr - (pointer to)port groups bmp , relevant only when enable = GT_TRUE
*
* RETURNS:
*       GT_OK        - OK
*       GT_BAD_PARAM - on bad device number
*       GT_BAD_PTR   - on NULL pointer
*
* COMMENTS:
*
*******************************************************************************/
GT_STATUS utilMultiPortGroupsBmpGet
(
    IN  GT_U8               devNum,
    OUT  GT_BOOL             *enablePtr,
    OUT  GT_PORT_GROUPS_BMP  *portGroupsBmpPtr
);


/* convert from
    devNum,portGroupsBmp    of test values  (received from Galtis/RDE test)

    to

    devNum,portGroupsBmp    of CPSS values
*/
void gtPortGroupDevConvert
(
   INOUT GT_U8 *devNum,
   INOUT GT_PORT_GROUPS_BMP *portGroupsBmpPtr
);

/* convert from
    devNum,portGroupsBmp    of CPSS values

    to

    devNum,portGroupsBmp    of test values (to send to Galtis/RDE test)
*/
void gtPortGroupDevConvertBack
(
   INOUT GT_U8 *devNum,
   INOUT GT_PORT_GROUPS_BMP *portGroupsBmpPtr
);

/* Don't do casting to GT_U8 for devNum. They must be of this type already*/
#define CONVERT_DEV_PORT_GROUPS_BMP_MAC(devNum, portGroupsBmp) \
   gtPortGroupDevConvert(&devNum, &portGroupsBmp)

#define CONVERT_BACK_DEV_PORT_GROUPS_BMP_MAC(devNum, portGroupsBmp) \
   gtPortGroupDevConvertBack(&devNum, &portGroupsBmp)




#ifdef  __cplusplus
}
#endif  /* __cplusplus      */
#endif /* __cmdPresteraUtils_h__ */

