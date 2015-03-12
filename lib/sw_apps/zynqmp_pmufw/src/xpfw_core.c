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

#include "xpfw_default.h"
#include "xpfw_core.h"
#include "xpfw_events.h"
#include "xpfw_interrupts.h"

#define CORE_IS_READY	((u32)0x5AFEC0DEU)
#define CORE_IS_DEAD	((u32)0xDEADBEAFU)

static XPfw_Core_t XPfwCore = { .IsReady = CORE_IS_DEAD };

/* Declare the Core Pointer as constant, since we don't intend to change it */
static XPfw_Core_t * const CorePtr = &XPfwCore;

XStatus XPfw_CoreInit(u32 Options)
{
	u32 Index;
	XStatus Status;
	if (CorePtr != NULL) {

		XPfw_InterruptInit();


		CorePtr->ModCount = (u8)0U;

		for (Index = 0U; Index < ARRAYSIZE(CorePtr->ModList); Index++) {
			Status = XPfw_ModuleInit(&CorePtr->ModList[Index], (u8) 0U);
			/* If there was an error, then just get out of here */
			if (XST_SUCCESS != Status) {
				break;
			}
		}
		/* Got to Scheduler Init only if we were able to init the Mods */
		if (XST_SUCCESS == Status) {
			Status = XPfw_SchedulerInit(&CorePtr->Scheduler,
			PMU_IOMODULE_PIT1_PRELOAD);
		}

	} else {
		Status = XST_FAILURE;
	}
	return Status;
}

XStatus XPfw_CoreConfigure(void)
{
	u32 Idx;
	XStatus Status;

	if (CorePtr != NULL) {
		for (Idx = 0U; Idx < CorePtr->ModCount; Idx++) {

			if (CorePtr->ModList[Idx].CfgInitHandler != NULL) {
				/* TODO: Pass on configure data based on module type */
				CorePtr->ModList[Idx].CfgInitHandler(&CorePtr->ModList[Idx],
						NULL, 0U);
			}
		}

		/* We are ready to take interrupts now */
		CorePtr->IsReady = CORE_IS_READY;
		/* FIXME: Enable IPI0 for PM-> Do it elsewhere */
		XPfw_InterruptEnable(PMU_IOMODULE_IRQ_ENABLE_IPI0_MASK);
		XPfw_InterruptStart();
		Status = XST_SUCCESS;
	} else {
		Status = XST_FAILURE;
	}
	return Status;
}

XStatus XPfw_CoreDispatchEvent(u32 EventId)
{
	XStatus Status;
	u32 Idx;
	u32 CallCount = 0U;
	if ((CorePtr != NULL) && (EventId < XPFW_EV_MAX)) {

		for (Idx = 0U; Idx < CorePtr->ModCount; Idx++) {
			/**
			 * Check if Mod[Idx] is registered for this event
			 */
			if ((XPfw_EventGetModMask(EventId) & ((u32) 1U << Idx))
					== ((u32) 1U << Idx)) {
				CorePtr->ModList[Idx].EventHandler(&CorePtr->ModList[Idx],
						EventId);
				CallCount++;
			}
		}
	}
	/* fw_printf("%s: Event(%d) dispatched to  %d Mods\r\n", __func__, EventId,CallCount); */
	if (CallCount > 0U) {
		Status = XST_SUCCESS;
	} else {
		Status = XST_FAILURE;
	}
	return Status;
}

/*
 * FIXME: After Integrating IPI Driver, redesign to
 * dispatch events based on IPI_ID (first word in msg buffer) Mask
 *
 */
XStatus XPfw_CoreDispatchIpi(u32 IpiNum)
{
	XStatus Status;
	u32 Idx;
	u32 CallCount = 0U;

	if ((CorePtr != NULL) && (IpiNum < 4U)) {
		for (Idx = 0U; Idx < CorePtr->ModCount; Idx++) {
			/**
			 * TODO: Check if Mod[Idx] is registered for this Ipi
			 * TODO: What if it is registered , but Handler is NULL. Report it!
			 */
			if (NULL != CorePtr->ModList[Idx].IpiHandler) {
				CorePtr->ModList[Idx].IpiHandler(&CorePtr->ModList[Idx], IpiNum,
						0U);
				CallCount++;
			}
		}
	}
	/* fw_printf("%s: IPI-%d dispatched to  %d Mods\r\n", __func__, IpiNum,
			CallCount); */
	if (CallCount > 0U) {
		Status = XST_SUCCESS;
	} else {
		Status = XST_FAILURE;
	}

	return Status;
}


const XPfw_Module_t *XPfw_CoreCreateMod(void)
{
	const XPfw_Module_t *ModPtr;

	if (CorePtr != NULL) {
		if (CorePtr->ModCount < XPFW_MAX_MOD_COUNT) {
			CorePtr->ModList[CorePtr->ModCount].ModId = CorePtr->ModCount;
			ModPtr = &CorePtr->ModList[CorePtr->ModCount];
			CorePtr->ModCount++;
		} else {
			ModPtr = NULL;
		}
	} else {
		ModPtr = NULL;
	}

	return ModPtr;
}

XStatus XPfw_CoreScheduleTask(const XPfw_Module_t *ModPtr, u32 Interval,
		VoidFunction_t CallbackRef)
{
	XStatus Status;

	if ((ModPtr != NULL) && (CorePtr != NULL)) {
		Status = XPfw_SchedulerAddTask(&CorePtr->Scheduler, ModPtr->ModId,
				Interval, CallbackRef);
	} else {
		Status = XST_FAILURE;
	}

	return Status;
}

void XPfw_CoreTickHandler(void)
{
	if(CorePtr != NULL){
		XPfw_SchedulerTickHandler(&CorePtr->Scheduler);
	}
	else {
		fw_printf("ERROR: NULL pointer to Core\r\n");
	}
}

XStatus XPfw_CoreIsReady(void)
{
	XStatus Status;

	if (CorePtr != NULL) {
		if (CORE_IS_READY == CorePtr->IsReady) {
			Status = XST_SUCCESS;
		} else {
			Status = XST_FAILURE;
		}
	} else {
		Status = XST_FAILURE;
	}

	return Status;
}

XStatus XPfw_CoreLoop(void)
{

	if(CORE_IS_READY == CorePtr->IsReady)
	{

		#ifdef ENABLE_SCHEDULER
		XPfw_SchedulerStart(&CorePtr->Scheduler);
		XPfw_InterruptEnable(PMU_IOMODULE_IRQ_ENABLE_PIT1_MASK);
		#endif
		do {

		#ifdef SLEEP_WHEN_IDLE
				/*Sleep. Will be waken up when a interrupt occurs*/
				mb_sleep();
		#endif

		#ifdef ENABLE_SCHEDULER
			if(TRUE == CorePtr->Scheduler.Enabled){
				(void)XPfw_SchedulerProcess(&CorePtr->Scheduler);
			}
		#endif

		} while (1);

	}
	/* If we reach this point then there was an error */
	return (XStatus)XST_FAILURE;
}


void XPfw_CorePrintStats(void)
{
	if(CorePtr != NULL) {
	fw_printf("######################################################\r\n");
	fw_printf("Module Count: %d (%d)\r\n", CorePtr->ModCount, XPFW_MAX_MOD_COUNT);
	fw_printf("Scheduler State: %s\r\n",((CorePtr->Scheduler.Enabled == TRUE)?"ENABLED":"DISABLED"));
	fw_printf("Scheduler Ticks: %d\r\n",CorePtr->Scheduler.Tick);
	fw_printf("######################################################\r\n");
	}
}


XStatus XPfw_CoreRegisterEvent(const XPfw_Module_t *ModPtr, u32 EventId)
{
	XStatus Status;

	if (NULL == ModPtr) {
		Status = XST_FAILURE;
	} else {
		Status = XPfw_EventAddOwner(ModPtr->ModId, EventId);
	}
	return Status;
}

XStatus XPfw_CoreDeRegisterEvent(const XPfw_Module_t *ModPtr, u32 EventId)
{
	XStatus Status;

	if (NULL == ModPtr) {
		Status = XST_FAILURE;
	} else {
		Status = XPfw_EventRemoveOwner(ModPtr->ModId, EventId);
	}
	return Status;
}

XStatus XPfw_CoreStopScheduler(void)
{
	XStatus Status;
	if(CorePtr != NULL) {
		Status = XPfw_SchedulerStop(&CorePtr->Scheduler);
		XPfw_InterruptDisable(PMU_IOMODULE_IRQ_ENABLE_PIT1_MASK);
	} else {
		Status = XST_FAILURE;
	}

	return Status;
}

XStatus XPfw_CoreSetCfgHandler(const XPfw_Module_t *ModPtr, XPfwModCfgInitHandler_t CfgHandler)
{
	XStatus Status;
	if ((ModPtr != NULL) && (CorePtr != NULL)) {
		if (ModPtr->ModId < CorePtr->ModCount) {
			CorePtr->ModList[ModPtr->ModId].CfgInitHandler = CfgHandler;
			Status = XST_SUCCESS;
		} else {
			Status = XST_FAILURE;
		}
	} else {
		Status = XST_FAILURE;
	}

	return Status;
}

XStatus XPfw_CoreSetEventHandler(const XPfw_Module_t *ModPtr, XPfwModEventHandler_t EventHandlerFn)
{
	XStatus Status;
	if ((ModPtr != NULL) && (CorePtr != NULL)) {
		if (ModPtr->ModId < CorePtr->ModCount) {
			CorePtr->ModList[ModPtr->ModId].EventHandler = EventHandlerFn;
			Status = XST_SUCCESS;
		} else {
			Status = XST_FAILURE;
		}
	} else {
		Status = XST_FAILURE;
	}

	return Status;
}


XStatus XPfw_CoreSetIpiHandler(const XPfw_Module_t *ModPtr, XPfwModIpiHandler_t IpiHandlerFn, u32 IpiId)
{
	XStatus Status;
	if ((ModPtr != NULL) && (CorePtr != NULL)) {
		if (ModPtr->ModId < CorePtr->ModCount) {
			CorePtr->ModList[ModPtr->ModId].IpiHandler = IpiHandlerFn;
			CorePtr->ModList[ModPtr->ModId].IpiId = IpiId;
			Status = XST_SUCCESS;
		} else {
			Status = XST_FAILURE;
		}
	} else {
		Status = XST_FAILURE;
	}

	return Status;
}