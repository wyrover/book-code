#include "IoTimer.h"

ULONG QueryIopTimerQueueHead()
{
	UNICODE_STRING UnicodeFuncName;
	ULONG ulFunction;
	ULONG ulCodeSize;
	ULONG IopTimerQueueHead = 0;
	PUCHAR i=0;

	RtlInitUnicodeString(&UnicodeFuncName,L"IoInitializeTimer");
	ulFunction = (ULONG)MmGetSystemRoutineAddress(&UnicodeFuncName);
	if (ulFunction)
	{
		//KdPrint(("IoInitializeTimer:%08x\n",ulFunction));
		ulCodeSize = GetFunctionCodeSize(ulFunction);
		if (ulCodeSize)
		{
			for (i=ulFunction;i<ulFunction+ulCodeSize;i++)
			{
				if (*i == 0xb9)
				{
					IopTimerQueueHead = *(PULONG)(i+1);
					if (MmIsAddressValidEx(IopTimerQueueHead))
					{
						break;
					}
				}
			}
		}
	}
	return IopTimerQueueHead;
}
// IoStartTimer
//IoStopTimer
VOID QueryIoTimer(PMyIoTimer IoTimer)
{
	PLIST_ENTRY  pList = NULL;
	PLIST_ENTRY pNextList = NULL;
	PIO_TIMER  pTimer = NULL;
	int i = 0;
	ULONG ulModuleBase;
	ULONG ulModuleSize;

	pList = QueryIopTimerQueueHead();
	if (!pList)
		return;

	//KdPrint(("IopTimerQueueHead -> %08x\n",pList));

	for ( pNextList = pList->Blink; pNextList != pList; pNextList = pNextList->Blink )    //遍历blink链
	{
		pTimer = CONTAINING_RECORD(pNextList,IO_TIMER,TimerList);            //得到结构首

		//KdPrint(("DeviceObject:%08x\nTimerRoutine:%08x\r\n\r\n",pTimer->DeviceObject,pTimer->TimerRoutine));

		if (MmIsAddressValidEx(pTimer) &&
			MmIsAddressValidEx(pTimer->DeviceObject) &&
			MmIsAddressValidEx(pTimer->TimerRoutine) &&
			MmIsAddressValidEx(&pTimer->TimerFlag) )                    //过滤
		{
			IoTimer->MyTimer[i].DeviceObject = pTimer->DeviceObject;
			IoTimer->MyTimer[i].IoTimerRoutineAddress = pTimer->TimerRoutine;
			IoTimer->MyTimer[i].ulStatus = pTimer->TimerFlag;
			if (!IsAddressInSystem(
				IoTimer->MyTimer[i].IoTimerRoutineAddress,
				&ulModuleBase,
				&ulModuleSize,
				IoTimer->MyTimer[i].lpszModule))
			{
				strcat(IoTimer->MyTimer[i].lpszModule,"Unknown");
			}
			i++;
			IoTimer->ulCount = i;
		}
		if (!MmIsAddressValidEx(pNextList->Blink))
		{
			break;                  //过滤
		}
	}
}
VOID IoTimerControl(PDEVICE_OBJECT DeviceObject,int Type)
{
	ReLoadNtosCALL(&RIoStartTimer,L"IoStartTimer",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RIoStopTimer,L"IoStopTimer",SystemKernelModuleBase,ImageModuleBase);
	if (RIoStartTimer &&
		RIoStopTimer)
	{
		if (MmIsAddressValidEx(DeviceObject))
		{
			switch (Type)
			{
			case 0:
				RIoStopTimer(DeviceObject);
				break;
			case 1:
				RIoStartTimer(DeviceObject);
				break;
			}
		}
	}
}