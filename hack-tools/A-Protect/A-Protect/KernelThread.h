#include "stdafx.h"

typedef struct _KERNEL_INFO{
	ULONG ulHideType;           //0 正常模块，在driver_object中有   1 隐藏模块
	ULONG ulStatus;             //线程状态，0运行，1退出
	ULONG KernelThread;         //ETHREAD
	ULONG ThreadStart;          //开始
	CHAR lpszThreadModule[256];

}KERNEL_INFO,*PKERNEL_INFO;

typedef struct _KERNEL_THREAD_INFO {          //InlineHook
	ULONG ulCount;
	KERNEL_INFO KernelThreadInfo[1];
} KERNEL_THREAD_INFO, *PKERNEL_THREAD_INFO;

PKERNEL_THREAD_INFO KernelThread;

CImageList KernelThreadImg;//驱动图标
