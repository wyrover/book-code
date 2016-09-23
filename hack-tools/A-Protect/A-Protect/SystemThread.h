#include "stdafx.h"
#include "Md5.h"

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

typedef struct _SYSTEM_THREAD_INFO {          //InlineHook
	ULONG ulCount;
	THREAD_INFO SystemThreadInfo[1];
} SYSTEM_THREAD_INFO, *PSYSTEM_THREAD_INFO;

PSYSTEM_THREAD_INFO SystemThread;

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

extern unsigned int conv(unsigned int a);

extern WCHAR WindowsXPSP3_CN[1900][50];
extern WCHAR WindowsXPSP2_CN[1753][50];

extern WCHAR Windows2003SP1_CN[1776][50];
extern WCHAR Windows2003SP2_CN[1856][50];

extern WCHAR Windows7Home_CN[2409][50];
extern WCHAR Windows7SP1_CN[2526][50];

BOOL FileVerify(char *lpszFullPath,WCHAR *lpwzFileMd5,WCHAR *lpwzTrue);

CImageList SystemThreadImg;//进程图标

extern BOOL bIsPhysicalCheck;
extern WCHAR PhysicalFile[260];
extern void SaveToFile(CHAR *lpszBuffer,WCHAR *lpwzFilePath);