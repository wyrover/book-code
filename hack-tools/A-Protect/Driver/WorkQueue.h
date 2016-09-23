#ifndef _WORK_QUEUE_H_
#define _WORK_QUEUE_H_

#include "ntifs.h"
#include "InitWindowsVersion.h"
#include "ntos.h"
//---------------------------------------------------------------------------------------
//工作队列线程
//---------------------------------------------------------------------------------------
typedef struct _WORKQUEUE_INFORMATION {          //WORKQUEUE_INFORMATION
	ULONG ulEthread;
	PUCHAR ulBasePriority;
	ULONG ulWorkerRoutine;
	CHAR lpszModule[256];                    //
	ULONG SuspendCount;               //暂停次数
} WORKQUEUE_INFORMATION, *PWORKQUEUE_INFORMATION;

typedef struct _WORKQUEUE {          //工作队列线程
	ULONG ulCount;
	WORKQUEUE_INFORMATION WorkQueueInfo[1];
} WORKQUEUE, *PWORKQUEUE;

PWORKQUEUE WorkQueueThread;

extern PEPROCESS SystemEProcess;
extern BOOL DebugOn;

VOID IniOffsetObject();

PETHREAD NTAPI GetNextProcessThread(
	IN PEPROCESS Process,
	IN PETHREAD Thread OPTIONAL,
	IN BOOL bIsQueryThread
	);

BOOL IsAddressInSystem(
	ULONG ulDriverBase,
	ULONG *ulSysModuleBase,
	ULONG *ulSize,
	char *lpszSysModuleImage
	);

#endif