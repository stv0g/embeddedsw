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
* @file xfsbl_image_header.h
*
* This is the image header file which contains definitions for the image header.
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

#ifndef XFSBL_IH_H
#define XFSBL_IH_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "xil_types.h"
#include "xfsbl_misc_drivers.h"

/************************** Constant Definitions *****************************/
#define XIH_MIN_PARTITIONS			(1U)
#define XIH_MAX_PARTITIONS			(32U)
#define XIH_RESERVED_OLD_IH			(0x380U)
#define XIH_PARTITION_WORD_LENGTH		(0x4U)

/**
 * Boot header field offset
 */
#define XIH_BH_IH_OFFSET		(0x3CU)
#define XIH_BH_IMAGE_ATTRB_OFFSET	(0x44U)
#define XIH_BH_IH_TABLE_OFFSET		(0x98U)
#define XIH_BH_PH_TABLE_OFFSET		(0x9CU)

#define XIH_BH_IMAGE_ATTRB_RSA_MASK	(0xC000U)
#define XIH_BH_IMAGE_ATTRB_SHA2_MASK	(0x3000U)

/**
 * Defines for length of the headers
 */
#define XIH_FIELD_LEN				(4U)
#define XIH_IHT_LEN				(64U)
#define XIH_PH_LEN				(64U)

/**
 * Image header table field offsets
 */
#define XIH_IHT_VERSION_OFFSET				(0x0U)
#define XIH_IHT_NO_OF_PARTITONS_OFFSET			(0x4U)
#define XIH_IHT_PH_ADDR_OFFSET	        		(0x8U)
#define XIH_IHT_PPD_OFFSET			        (0x14U)
#define XIH_IHT_CHECKSUM_OFFSET				(0x3CU)

/**
 * Partition Header Fields
 */
#define XIH_PH_ENC_DATAWORD_LENGTH		(0x0U)
#define XIH_PH_UNENC_DATAWORD_LENGTH		(0x4U)
#define XIH_PH_TOTAL_DATAWORD_LENGTH			(0x8U)
#define XIH_PH_NEXT_PARTITION_OFFSET			(0xCU)
#define XIH_PH_DEST_EXECUTION_ADDRESS	        (0x10U)
#define XIH_PH_DEST_LOAD_ADDRESS			(0x18U)
#define XIH_PH_DATA_WORD_OFFSET				(0x20U)
#define XIH_PH_ATTRB_OFFSET				(0x24U)
#define XIH_PH_SECTION_COUNT				(0x28U)
#define XIH_PH_CHECKSUM_WORD_OFFSET			(0x2CU)
#define XIH_PH_IMAGEHEADER_OFFSET			(0x30U)
#define XIH_PH_AUTHCERTIFICATE_OFFSET			(0x34U)
#define XIH_PH_CHECKSUM					(0x3CU)

/**
 * Partition Present Devices(PPD) in IHT
 */
#define XIH_IHT_PPD_SAME	        (0x0U)
#define XIH_IHT_PPD_QSPI	        (0x1U)
#define XIH_IHT_PPD_NAND	        (0x2U)
#define XIH_IHT_PPD_SD			(0x3U)
#define XIH_IHT_PPD_MMC		        (0x4U)
#define XIH_IHT_PPD_USB	        	(0x5U)
#define XIH_IHT_PPD_ETHERNET		(0x6U)
#define XIH_IHT_PPD_PCIE	        (0x7U)
#define XIH_IHT_PPD_SATA	        (0x8U)

/**
 * Partition Attribute fields
 */
#define XIH_PH_ATTRB_PART_OWNER_MASK		(0x30000U)
#define XIH_PH_ATTRB_RSA_SIGNATURE_MASK		(0x8000U)
#define XIH_PH_ATTRB_CHECKSUM_MASK		(0x7000U)
#define XIH_PH_ATTRB_DEST_CPU_MASK		(0x0F00U)
#define XIH_PH_ATTRB_ENCRYPTION_MASK		(0x0080U)
#define XIH_PH_ATTRB_DEST_DEVICE_MASK		(0x0070U)
#define XIH_PH_ATTRB_A53_EXEC_ST_MASK		(0x0008U)


/**
 * Partition Attribute Values
 */
#define XIH_PH_ATTRB_PART_OWNER_FSBL		(0x00000U)
#define XIH_PH_ATTRB_RSA_SIGNATURE		(0x8000U)
#define XIH_PH_ATTRB_NOCHECKSUM			(0x0000U)
#define XIH_PH_ATTRB_CHECKSUM_MD5		(0x1000U)

#define XIH_PH_ATTRB_DEST_CPU_NONE	(0x0000U)
#define XIH_PH_ATTRB_DEST_CPU_A53_0	(0x0100U)
#define XIH_PH_ATTRB_DEST_CPU_A53_1	(0x0200U)
#define XIH_PH_ATTRB_DEST_CPU_A53_2	(0x0300U)
#define XIH_PH_ATTRB_DEST_CPU_A53_3	(0x0400U)
#define XIH_PH_ATTRB_DEST_CPU_R5_0	(0x0500U)
#define XIH_PH_ATTRB_DEST_CPU_R5_1	(0x0600U)
#define XIH_PH_ATTRB_DEST_CPU_R5_L	(0x0700U)

#define XIH_PH_ATTRB_ENCRYPTION		(0x0080U)


#define XIH_PH_ATTRB_DEST_DEVICE_NONE	(0x0000U)
#define XIH_PH_ATTRB_DEST_DEVICE_PS	(0x0010U)
#define XIH_PH_ATTRB_DEST_DEVICE_PL	(0x0020U)
#define XIH_PH_ATTRB_DEST_DEVICE_PMU	(0x0030U)

#define XIH_PH_ATTRB_A53_EXEC_ST_AA32	(0x0008U)
#define XIH_PH_ATTRB_A53_EXEC_ST_AA64	(0x0000U)

/**************************** Type Definitions *******************************/

/**
 * Structure to store the image header table details.
 * It contains all the information of image header table in order.
 */
typedef struct {
	u32 Version; /**< bootgen version used  */
	u32 NoOfPartitions; /**< No of partition present  */
	u32 PartitionHeaderAddress; /**< Address to start of partition header*/
	u32 Reserved_0xC; /**< Reserved */
	u32 Reserved_0x10; /**< Reserved */
	u32 PartitionPresentDevice;
		/**< Partition present device for secondary boot modes*/
	u32 Reserved[9]; /**< Reserved */
	u32 Checksum; /**< Checksum of the image header table */
} XFsblPs_ImageHeaderTable;

/**
 * Structure to store the partition header details.
 * It contains all the information of partition header in order.
 */
typedef struct {
	u32 EncryptedDataWordLength; /**< Encrypted word length of partition*/
	u32 UnEncryptedDataWordLength; /**< unencrypted word length */
	u32 TotalDataWordLength;
		/**< Total word length including the authentication
			certificate if any*/
	u32 NextPartitionOffset; /**< Address of the next partition header*/
	u64 DestinationExecutionAddress; /**< Execution address */
	u64 DestinationLoadAddress; /**< Load address in DDR/TCM */
	u32 DataWordOffset; /**< */
	u32 PartitionAttributes; /**< partition attributes */
	u32 SectionCount; /**< section count */
	u32 ChecksumWordOffset; /**< address to checksum when enabled */
	u32 ImageHeaderOffset; /**< address to image header */
	u32 AuthCertificateOffset;
		/**< address to the authentication certificate when enabled */
	u32 Reserved[1]; /**< Reserved */
	u32 Checksum; /**< checksum of the partition header */
} XFsblPs_PartitionHeader;

/**
 * Structure of the image header which contains
 * information of image header table and
 * partition headers.
 */
typedef struct {
	XFsblPs_ImageHeaderTable ImageHeaderTable;
		/**< Image header table structure */
	XFsblPs_PartitionHeader PartitionHeader[XIH_MAX_PARTITIONS];
		/**< Partition header */
} XFsblPs_ImageHeader;

/***************** Macros (Inline Functions) Definitions *********************/
__inline u32 XFsbl_GetPartitionOwner(
		XFsblPs_PartitionHeader * PartitionHeader);
__inline u32 XFsbl_IsRsaSignaturePresent(
		XFsblPs_PartitionHeader * PartitionHeader);
__inline u32 XFsbl_GetChecksumType(
		XFsblPs_PartitionHeader * PartitionHeader);
__inline u32 XFsbl_GetDestinationCpu(
		XFsblPs_PartitionHeader * PartitionHeader);
__inline u32 XFsbl_IsEncrypted(
		XFsblPs_PartitionHeader * PartitionHeader);
__inline u32 XFsbl_GetDestinationDevice(
		XFsblPs_PartitionHeader * PartitionHeader);
__inline u32 XFsbl_GetA53ExecState(
                XFsblPs_PartitionHeader * PartitionHeader);

/************************** Function Prototypes ******************************/
u32 XFsbl_ValidateChecksum(u32 Buffer[], u32 Length);
u32 XFsbl_ReadImageHeader(XFsblPs_ImageHeader * ImageHeader,
                  XFsblPs_DeviceOps * DeviceOps, u32 FlashImageOffsetAddress);
u32 XFsbl_ValidateImageHeader(XFsblPs_ImageHeaderTable * ImageHeaderTable);
u32 XFsbl_ValidatePartitionHeader(XFsblPs_PartitionHeader * PartitionHeader,
			u32 RunningCpu);

#ifdef __cplusplus
}
#endif

#endif /* XFSBL_IH_H */