/******************************************************************************
*
* Copyright (C) 2015 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* XILINX CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*****************************************************************************/
/**
*
* @file xfsbl_main.h
*
* This is the main header file which contains definitions for the FSBL.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date        Changes
* ----- ---- -------- -------------------------------------------------------
* 1.00  kc   10/21/13 Initial release
*
* </pre>
*
* @note
*
******************************************************************************/

#ifndef XFSBL_MAIN_H
#define XFSBL_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "xfsbl_image_header.h"
#include "xfsbl_misc_drivers.h"
#include "xfsbl_hw.h"
/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/**
 * This stores the handoff Address of the different cpu's
 */
typedef struct {
	u32 CpuSettings;
	u64 HandoffAddress;
} XFsblPs_HandoffValues;

/**
 * This is FSBL instance pointer. This stores all the information
 * required for FSBL
 */
typedef struct {
	u32 Version; /**< FSBL Version */
	u32 PresentStage; /**< Stage */
	u32 ProcessorID; /**< One of R5-0, R5-LS, A53-0 */
	u32 BootHdrAttributes; /**< Boot Header attributes */
	u32 ImageOffsetAddress; /**< Flash offset address */
	XFsblPs_ImageHeader ImageHeader; /** Image header */
	u32 ErrorCode; /**< Error code during FSBL failure */
	u32 PrimaryBootDevice; /**< Primary boot device used for booting  */
	u32 SecondaryBootDevice; /**< Secondary boot device in image header*/
	XFsblPs_DeviceOps DeviceOps; /**< Device operations for bootmodes */
	u32 HandoffCpuNo; /**< Number of CPU's FSBL will handoff to */
	XFsblPs_HandoffValues HandoffValues[10];
		/**< Handoff address for different CPU's  */
} XFsblPs;


/***************** Macros (Inline Functions) Definitions *********************/

/* SDK release version */
#define SDK_RELEASE_YEAR	2015
#define SDK_RELEASE_QUARTER	1

#define XFSBL_RUNNING			(0xFFFFU)
#define XFSBL_COMPLETED			(0x0U)
#define XFSBL_IMAGE_SEARCH_OFFSET	(0x8000U) /**< 32KB offset */

/**
 * Fsbl exit definitions
 */
#define XFSBL_NO_HANDOFFEXIT	(0x0U)
#define XFSBL_HANDOFFEXIT	(0x1U)
#define XFSBL_HANDOFFEXIT_32	(0x2U)

/**
 * Boot Modes definition
 */
#define XFSBL_JTAG_BOOT_MODE		(0x0U)
#define XFSBL_QSPI24_BOOT_MODE		(0x1U)
#define XFSBL_QSPI32_BOOT_MODE		(0x2U)
#define XFSBL_SD_BOOT_MODE	        (0x3U)
#define XFSBL_NAND_BOOT_MODE		(0x4U)
#define XFSBL_EMMC_BOOT_MODE		(0x6U)
#define XFSBL_USB_BOOT_MODE		(0x7U)

/**
 * FSBL stages definition
 */
#define XFSBL_STAGE1		(0x1U)
#define XFSBL_STAGE2		(0x2U)
#define XFSBL_STAGE3		(0x3U)
#define XFSBL_STAGE4		(0x4U)
#define XFSBL_STAGE_ERR		(0x5U)
#define XFSBL_STAGE_DEFAULT	(0x6U)

/************************** Function Prototypes ******************************/
/**
 * Functions defined in xfsbl_main.c
 */
void XFsbl_PrintFsblBanner(void );
void XFsbl_ErrorLockDown(u32 ErrorStatus);
void XFsbl_FallBack(void );
void XFsbl_UpdateMultiBoot(u32 MultiBootValue);

/**
 * Functions defined in xfsbl_initialization.c
 */
void XFsbl_CfgInitialize (XFsblPs * FsblInstancePtr);
u32 XFsbl_Initialize(XFsblPs * FsblInstancePtr);
u32 XFsbl_BootDeviceInitAndValidate(XFsblPs * FsblInstancePtr);

/**
 * Functions defined in xfsbl_partition_load.c
 */
u32 XFsbl_PartitionLoad(XFsblPs * FsblInstancePtr, u32 PartitionNum);

/**
 * Functions defined in xfsbl_handoff.c
 */
u32 XFsbl_Handoff (XFsblPs * FsblInstancePtr);
void XFsbl_HandoffExit(u64 HandoffAddress, u32 Flags);
/************************** Variable Definitions *****************************/


#ifdef __cplusplus
}
#endif

#endif  /* XFSBL_MAIN_H */