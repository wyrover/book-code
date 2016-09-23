#ifndef _IOTIMER_H_
#define _IOTIMER_H_

#include "ntifs.h"
#include "InitWindowsVersion.h"
#include "ntos.h"

typedef struct _IO_TIMER
{
	SHORT Type;
	SHORT TimerFlag;
	LIST_ENTRY TimerList;
	PVOID TimerRoutine;
	PVOID Context;
	PDEVICE_OBJECT DeviceObject;

} IO_TIMER, *PIO_TIMER;

//***********************************************************
typedef struct _MyIoTimerInfo{
	ULONG  ulStatus;        //循环间隔
	ULONG  DeviceObject;      //设备对象
	ULONG  IoTimerRoutineAddress;  //例程地址
	char   lpszModule[260];
}MyIoTimerInfo,*PMyIoTimerInfo;

typedef struct _MyIoTimer{
	ULONG  ulCount;
	MyIoTimerInfo MyTimer[1];
}MyIoTimer,*PMyIoTimer;

PMyIoTimer IoTimer; 

//***********************************************************

unsigned long __fastcall GetFunctionCodeSize(void *Proc);

#endif