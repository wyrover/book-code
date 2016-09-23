#include "stdafx.h"

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

CImageList WorkQueueImg;//图标