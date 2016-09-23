#include "stdafx.h"

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

CImageList IoTimerImg;// 进程图标

CHAR* setClipboardText(CHAR* str);