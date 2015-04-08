/******************************************************************************
*
* Copyright (C) 2002 - 2014 Xilinx, Inc. All rights reserved.
*
* This file contains confidential and proprietary information  of Xilinx, Inc.
* and is protected under U.S. and  international copyright and other
* intellectual property  laws.
*
* DISCLAIMER
* This disclaimer is not a license and does not grant any  rights to the
* materials distributed herewith. Except as  otherwise provided in a valid
* license issued to you by  Xilinx, and to the maximum extent permitted by
* applicable law:
* (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND  WITH ALL FAULTS, AND
* XILINX HEREBY DISCLAIMS ALL WARRANTIES  AND CONDITIONS, EXPRESS, IMPLIED,
* OR STATUTORY, INCLUDING  BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* NON-INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE
* and
* (2) Xilinx shall not be liable (whether in contract or tort,  including
* negligence, or under any other theory of liability) for any loss or damage of
* any kind or nature  related to, arising under or in connection with these
* materials, including for any direct, or any indirect,  special, incidental,
* or consequential loss or damage  (including loss of data, profits, goodwill,
* or any type of  loss or damage suffered as a result of any action brought
* by a third party) even if such damage or loss was  reasonably foreseeable
* or Xilinx had been advised of the  possibility of the same.
*
* CRITICAL APPLICATIONS
* Xilinx products are not designed or intended to be fail-safe, or for use in
* any application requiring fail-safe  performance, such as life-support or
* safety devices or  systems, Class III medical devices, nuclear facilities,
* applications related to the deployment of airbags, or any  other applications
* that could lead to death, personal  injury, or severe property or environmental
* damage  (individually and collectively, "Critical  Applications").
* Customer assumes the sole risk and liability of any use of Xilinx products in
* Critical  Applications, subject only to applicable laws and  regulations
* governing limitations on product liability.
*
* THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE
* AT ALL TIMES.
*
******************************************************************************/
/*****************************************************************************/
/**
*
* @file xenv_standalone.h
*
* Defines common services specified by xenv.h.
*
* @note
* 	This file is not intended to be included directly by driver code.
* 	Instead, the generic xenv.h file is intended to be included by driver
* 	code.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00a wgr  02/28/07 Added cache handling macros.
* 1.00a wgr  02/27/07 Simplified code. Deprecated old-style macro names.
* 1.00a rmm  01/24/06 Implemented XENV_USLEEP. Assume implementation is being
*                     used under Xilinx standalone BSP.
* 1.00a xd   11/03/04 Improved support for doxygen.
* 1.00a rmm  03/21/02 First release
* 1.00a wgr  03/22/07 Converted to new coding style.
* 1.00a rpm  06/29/07 Added udelay macro for standalone
* 1.00a xd   07/19/07 Included xparameters.h as XPAR_ constants are referred
*                     to in MICROBLAZE section
* 1.00a ecm  09/19/08 updated for v7.20 of Microblaze, new functionality
*
* </pre>
*
*
******************************************************************************/

#ifndef XENV_STANDALONE_H
#define XENV_STANDALONE_H

#include "xil_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
/******************************************************************************
 *
 * Get the processor dependent includes
 *
 ******************************************************************************/

#include <string.h>

#if defined __MICROBLAZE__
#  include "mb_interface.h"
#  include "xparameters.h"   /* XPAR constants used below in MB section */

#elif defined __PPC__
#  include "sleep.h"
#  include "xcache_l.h"      /* also include xcache_l.h for caching macros */
#endif

/******************************************************************************
 *
 * MEMCPY / MEMSET related macros.
 *
 * The following are straight forward implementations of memset and memcpy.
 *
 * NOTE: memcpy may not work if source and target memory area are overlapping.
 *
 ******************************************************************************/
/*****************************************************************************/
/**
 *
 * Copies a non-overlapping block of memory.
 *
 * @param	DestPtr
 *		Destination address to copy data to.
 *
 * @param	SrcPtr
 * 		Source address to copy data from.
 *
 * @param	Bytes
 * 		Number of bytes to copy.
 *
 * @return	None.
 *
 * @note
 * 		The use of XENV_MEM_COPY is deprecated. Use memcpy() instead.
 *
 * @note
 * 		This implemention MAY BREAK work if source and target memory
 * 		area are overlapping.
 *
 *****************************************************************************/

#define XENV_MEM_COPY(DestPtr, SrcPtr, Bytes) \
	memcpy((void *) DestPtr, (const void *) SrcPtr, (size_t) Bytes)



/*****************************************************************************/
/**
 *
 * Fills an area of memory with constant data.
 *
 * @param	DestPtr
 *		Destination address to copy data to.
 *
 * @param	Data
 * 		Value to set.
 *
 * @param	Bytes
 * 		Number of bytes to copy.
 *
 * @return	None.
 *
 * @note
 * 		The use of XENV_MEM_FILL is deprecated. Use memset() instead.
 *
 *****************************************************************************/

#define XENV_MEM_FILL(DestPtr, Data, Bytes) \
	memset((void *) DestPtr, (s32) Data, (size_t) Bytes)



/******************************************************************************
 *
 * TIME related macros
 *
 ******************************************************************************/

/**
 * A structure that contains a time stamp used by other time stamp macros
 * defined below. This structure is processor dependent.
 */
typedef s32 XENV_TIME_STAMP;

/*****************************************************************************/
/**
 *
 * Time is derived from the 64 bit PPC timebase register
 *
 * @param   StampPtr is the storage for the retrieved time stamp.
 *
 * @return  None.
 *
 * @note
 *
 * Signature: void XENV_TIME_STAMP_GET(XTIME_STAMP *StampPtr)
 * <br><br>
 * This macro must be implemented by the user.
 *
 *****************************************************************************/
#define XENV_TIME_STAMP_GET(StampPtr)

/*****************************************************************************/
/**
 *
 * This macro is not yet implemented and always returns 0.
 *
 * @param   Stamp1Ptr is the first sampled time stamp.
 * @param   Stamp2Ptr is the second sampled time stamp.
 *
 * @return  0
 *
 * @note
 *
 * This macro must be implemented by the user.
 *
 *****************************************************************************/
#define XENV_TIME_STAMP_DELTA_US(Stamp1Ptr, Stamp2Ptr)     (0)

/*****************************************************************************/
/**
 *
 * This macro is not yet implemented and always returns 0.
 *
 * @param   Stamp1Ptr is the first sampled time stamp.
 * @param   Stamp2Ptr is the second sampled time stamp.
 *
 * @return  0
 *
 * @note
 *
 * This macro must be implemented by the user.
 *
 *****************************************************************************/
#define XENV_TIME_STAMP_DELTA_MS(Stamp1Ptr, Stamp2Ptr)     (0)

/*****************************************************************************/
/**
 * XENV_USLEEP(unsigned delay)
 *
 * Delay the specified number of microseconds. Not implemented without OS
 * support.
 *
 * @param	delay
 * 		Number of microseconds to delay.
 *
 * @return	None.
 *
 *****************************************************************************/

#ifdef __PPC__
#define XENV_USLEEP(delay)	usleep(delay)
#define udelay(delay)	usleep(delay)
#else
#define XENV_USLEEP(delay)
#define udelay(delay)
#endif


/******************************************************************************
 *
 * CACHE handling macros / mappings
 *
 ******************************************************************************/
/******************************************************************************
 *
 * Processor independent macros
 *
 ******************************************************************************/

#define XCACHE_ENABLE_CACHE()	\
		{ XCACHE_ENABLE_DCACHE(); XCACHE_ENABLE_ICACHE(); }

#define XCACHE_DISABLE_CACHE()	\
		{ XCACHE_DISABLE_DCACHE(); XCACHE_DISABLE_ICACHE(); }


/******************************************************************************
 *
 * MicroBlaze case
 *
 * NOTE: Currently the following macros will only work on systems that contain
 * only ONE MicroBlaze processor. Also, the macros will only be enabled if the
 * system is built using a xparameters.h file.
 *
 ******************************************************************************/

#if defined __MICROBLAZE__

/* Check if MicroBlaze data cache was built into the core.
 */
#if (XPAR_MICROBLAZE_USE_DCACHE == 1)
#  define XCACHE_ENABLE_DCACHE()		microblaze_enable_dcache()
#  define XCACHE_DISABLE_DCACHE()		microblaze_disable_dcache()
#  define XCACHE_INVALIDATE_DCACHE()  	microblaze_invalidate_dcache()

#  define XCACHE_INVALIDATE_DCACHE_RANGE(Addr, Len) \
			microblaze_invalidate_dcache_range((s32)(Addr), (s32)(Len))

#if (XPAR_MICROBLAZE_DCACHE_USE_WRITEBACK == 1)
#  define XCACHE_FLUSH_DCACHE()  		microblaze_flush_dcache()
#  define XCACHE_FLUSH_DCACHE_RANGE(Addr, Len) \
			microblaze_flush_dcache_range((s32)(Addr), (s32)(Len))
#else
#  define XCACHE_FLUSH_DCACHE()  		microblaze_invalidate_dcache()
#  define XCACHE_FLUSH_DCACHE_RANGE(Addr, Len) \
			microblaze_invalidate_dcache_range((s32)(Addr), (s32)(Len))
#endif	/*XPAR_MICROBLAZE_DCACHE_USE_WRITEBACK*/

#else
#  define XCACHE_ENABLE_DCACHE()
#  define XCACHE_DISABLE_DCACHE()
#  define XCACHE_INVALIDATE_DCACHE_RANGE(Addr, Len)
#  define XCACHE_FLUSH_DCACHE_RANGE(Addr, Len)
#endif	/*XPAR_MICROBLAZE_USE_DCACHE*/


/* Check if MicroBlaze instruction cache was built into the core.
 */
#if (XPAR_MICROBLAZE_USE_ICACHE == 1)
#  define XCACHE_ENABLE_ICACHE()		microblaze_enable_icache()
#  define XCACHE_DISABLE_ICACHE()		microblaze_disable_icache()

#  define XCACHE_INVALIDATE_ICACHE()  	microblaze_invalidate_icache()

#  define XCACHE_INVALIDATE_ICACHE_RANGE(Addr, Len) \
			microblaze_invalidate_icache_range((s32)(Addr), (s32)(Len))

#else
#  define XCACHE_ENABLE_ICACHE()
#  define XCACHE_DISABLE_ICACHE()
#endif	/*XPAR_MICROBLAZE_USE_ICACHE*/


/******************************************************************************
 *
 * PowerPC case
 *
 *   Note that the XCACHE_ENABLE_xxx functions are hardcoded to enable a
 *   specific memory region (0x80000001). Each bit (0-30) in the regions
 *   bitmask stands for 128MB of memory. Bit 31 stands for the upper 2GB
 *   range.
 *
 *   regions    --> cached address range
 *   ------------|--------------------------------------------------
 *   0x80000000  | [0, 0x7FFFFFF]
 *   0x00000001  | [0xF8000000, 0xFFFFFFFF]
 *   0x80000001  | [0, 0x7FFFFFF],[0xF8000000, 0xFFFFFFFF]
 *
 ******************************************************************************/

#elif defined __PPC__

#define XCACHE_ENABLE_DCACHE()		XCache_EnableDCache(0x80000001)
#define XCACHE_DISABLE_DCACHE()		XCache_DisableDCache()
#define XCACHE_ENABLE_ICACHE()		XCache_EnableICache(0x80000001)
#define XCACHE_DISABLE_ICACHE()		XCache_DisableICache()

#define XCACHE_INVALIDATE_DCACHE_RANGE(Addr, Len) \
		XCache_InvalidateDCacheRange((u32)(Addr), (u32)(Len))

#define XCACHE_FLUSH_DCACHE_RANGE(Addr, Len) \
		XCache_FlushDCacheRange((u32)(Addr), (u32)(Len))

#define XCACHE_INVALIDATE_ICACHE()	XCache_InvalidateICache()


/******************************************************************************
 *
 * Unknown processor / architecture
 *
 ******************************************************************************/

#else
/* #error "Unknown processor / architecture. Must be MicroBlaze or PowerPC." */
#endif


#ifdef __cplusplus
}
#endif

#endif	/* #ifndef XENV_STANDALONE_H */
