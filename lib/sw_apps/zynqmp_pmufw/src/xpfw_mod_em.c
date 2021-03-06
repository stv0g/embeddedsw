/******************************************************************************
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
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
******************************************************************************/

#include "xpfw_default.h"
#include "xpfw_module.h"
#include "xpfw_error_manager.h"
#include "xpfw_resets.h"
#include "xpfw_events.h"
#include "xpfw_core.h"

#ifdef ENABLE_EM
/**
 * Example Error handler for RPU lockstep error
 *
 * This handler should be called when an R5 lockstep error occurs
 * and it resets the RPU gracefully
 */

static void RpuLsHandler(u8 ErrorId)
{
	fw_printf("EM: RPU Lock-Step Error Occurred (Error ID: %d)\r\n", ErrorId);
	fw_printf("EM: Initiating RPU Reset \r\n");
	XPfw_ResetRpu();
}


/* CfgInit Handler */
static void EmCfgInit(const XPfw_Module_t *ModPtr, const u32 *CfgData,
		u32 Len)
 {
	/* Register for Error events from Core */
	(void) XPfw_CoreRegisterEvent(ModPtr, XPFW_EV_ERROR_1);
	(void) XPfw_CoreRegisterEvent(ModPtr, XPFW_EV_ERROR_2);

	/* Init the Error Manager */
	XPfw_EmInit();
	/* Set handlers for error manager */
	XPfw_EmSetAction(EM_ERR_ID_RPU_LS, EM_ACTION_CUSTOM, RpuLsHandler);

	fw_printf("Error Management Module (MOD-%d): Initialized.\r\n",
			ModPtr->ModId);
}

/**
 * Events handler receives error events from core and routes them to
 * Error Manager for processing
 */
static void EmEventHandler(const XPfw_Module_t *ModPtr, u32 EventId)
{
	switch (EventId) {
	case XPFW_EV_ERROR_1:
		XPfw_EmProcessError(EM_ERR_TYPE_1);
		break;
	case XPFW_EV_ERROR_2:
		XPfw_EmProcessError(EM_ERR_TYPE_2);
		break;
	default:
		fw_printf("EM:Unhandled Event(ID:%d)\r\n", EventId);
		break;
	}
}

/*
 * Create a Mod and assign the Handlers. We will call this function
 * from XPfw_UserStartup()
 */
void ModEmInit(void)
{
	const XPfw_Module_t *EmModPtr = XPfw_CoreCreateMod();

	(void) XPfw_CoreSetCfgHandler(EmModPtr, EmCfgInit);
	(void) XPfw_CoreSetEventHandler(EmModPtr, EmEventHandler);
}

#else /* ENABLE_EM */
void ModEmInit(void) { }
#endif /* ENABLE_EM */
