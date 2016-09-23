#ifndef _SYSTEM_THREAD_H_
#define _SYSTEM_THREAD_H_

#include "ntifs.h"
#include "ntos.h"
#include "KillProcess.h"
#include "ProcessModule.h"

#define THREAD_SUSPEND_RESUME          (0x0002)  // winnt
#define PS_CROSS_THREAD_FLAGS_SYSTEM 0x00000010UL 

extern PSERVICE_DESCRIPTOR_TABLE OriginalServiceDescriptorTable;

typedef enum _KAPC_ENVIRONMENT {
	OriginalApcEnvironment,
	AttachedApcEnvironment,
	CurrentApcEnvironment,
	InsertApcEnvironment
} KAPC_ENVIRONMENT;

typedef enum _THREAD_STATE
{
	StateInitialized,
	StateReady,
	StateRunning,
	StateStandby,
	StateTerminated,
	StateWaiting,
	StateTransition,
	StateUnknown

}THREAD_STATE;
//---------------------------------------------------------------------------------------
//线程堆栈
//---------------------------------------------------------------------------------------
/*
ring3界面需要显示的效果如下：

lkd> dt_kthread 821b95b8
     ...
	 +0x028 KernelStack      : 0xf8ac179c Void

lkd> dd 0xf8ac179c 

ulAddress ulStack1 ulStack2 ulStack3 ulStack4

f8ac179c  f8ac1dcc 00000246 80542822 f8ac17f8
f8ac17ac  821b95b8 ffdff120 821b9628 80501ce6
f8ac17bc  00000000 821b95b8 804faaf2 806d32d0
f8ac17cc  007c3000 00000000 00000100 00000000
f8ac17dc  00000023 00000023 00000000 00000002
f8ac17ec  00000000 821b9628 01ac1dcc f8ac1844
f8ac17fc  8051a4e3 00000002 f8ac182c 00000001
f8ac180c  00000008 00000000 00000000 00000000
*/
typedef struct _THREAD_STACK_INFO{
	ULONG ulAddress;

	ULONG ulStack1;
	ULONG ulStack2;
	ULONG ulStack3;
	ULONG ulStack4;

}THREAD_STACK_INFO,*PTHREAD_STACK_INFO;

typedef struct _THREAD_STACK {          //THREAD_STACK
	ULONG ulCount;
	ULONG ulKthread;     //ring3打印显示需要用到
	ULONG ulKernelStack; //ring3打印显示需要用到
	ULONG KernelStack;   //ring3打印显示需要用到
	THREAD_STACK_INFO StackInfo[1];

} THREAD_STACK, *PTHREAD_STACK;

PTHREAD_STACK ThreadStack;
//---------------------------------------------------------------------------------------
typedef struct _THREAD_INFO{
	ULONG ulHideType;           //1 找不到线程所在模块，既认为隐藏线程，显示粉红色，0 正常
	ULONG ThreadID;
	ULONG KernelThread;
	ULONG TEB;
	ULONG ThreadStart;
	ULONG HideDebug;            //如果是0，则显示褐色
	CHAR lpszThreadModule[256];
	UCHAR ulStatus;
	UCHAR ulPriority;    //优先级
	ULONG ulContextSwitches;

}THREAD_INFO,*PTHREAD_INFO;

typedef struct _SYSTEM_THREAD_INFO {
	ULONG ulCount;
	THREAD_INFO SystemThreadInfo[1];
} SYSTEM_THREAD_INFO, *PSYSTEM_THREAD_INFO;

PSYSTEM_THREAD_INFO SystemThread;
//-----------------------------------------------------------------------------

// ULONG ulThreadProc;
// ULONG ulThreadListHead;
ULONG ulCrossThreadFlagsOffset;
ULONG ulHideDebug;
ULONG ulThreadID;
ULONG ulTEB;
ULONG ulThreadStart;
ULONG ulWin32ThreadStart;
ULONG ulStatus;
ULONG ulPriority;
ULONG ulContextSwitches;   //切换次数

ULONG ulActiveExWorker; //要插APC结束工作队列线程，这个标志必须有!

ULONG ulSuspendCount;   //线程暂停次数

extern PEPROCESS SystemEProcess;

extern BYTE *ImageModuleBase;
extern ULONG SystemKernelModuleBase;

typedef ULONG (__stdcall *KERESUMETHREAD)(
    IN PETHREAD Thread
	);

typedef NTSTATUS (__stdcall *NTRESUMETHREAD)(
    IN PETHREAD Thread,
    OUT PULONG PreviousSuspendCount OPTIONAL
	);

typedef NTSTATUS (__stdcall *PSRESUMETHREAD)(
    IN PETHREAD Thread,
    OUT PULONG PreviousSuspendCount OPTIONAL
	);

typedef NTSTATUS (__stdcall *PSSUSPENDTHREAD)(
	 IN PETHREAD Thread,
    OUT PULONG PreviousSuspendCount OPTIONAL
	);

//bIsQueryThread
//false->kill
//true->query
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

BOOL IniKillObject();

BOOL GetFunctionIndexByName(CHAR *lpszFunctionName,int *Index);

VOID IniOffsetObject();
#endif