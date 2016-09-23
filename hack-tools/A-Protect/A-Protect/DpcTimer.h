#include "stdafx.h"

#define MAX_DPCTIMER_COUNT 250

typedef struct _MyDpcTimerInfo{
	ULONG  TimerAddress;    //KTIMER结构地址
	ULONG  Period;        //循环间隔
	ULONG  DpcAddress;      //DPC结构地址
	ULONG  DpcRoutineAddress;  //例程地址
	char lpszModule[260];
}MyDpcTimerInfo,*PMyDpcTimerInfo;

typedef struct _MyDpcTimer{
	ULONG  ulCount;
	MyDpcTimerInfo MyTimer[1];
}MyDpcTimer,*PMyDpcTimer;

PMyDpcTimer DpcTimer;

//CString DpcTimerNum;
CImageList DpcTimerImg;// 进程图标

CHAR* setClipboardText(CHAR* str);