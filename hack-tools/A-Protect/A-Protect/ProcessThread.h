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

extern PSYSTEM_THREAD_INFO SystemThread;

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

BOOL NtFilePathToDosFilePath(__in LPCTSTR lpwzNtFilePath,__out LPCTSTR lpwzDosFilePath);
BOOL PrintfDosPath(__in LPCTSTR lpwzNtFullPath,__out LPCTSTR lpwzDosFullPath);
ULONG GetDLLFileSize(char *lpszDLLPath);

extern WCHAR lpwzNum[50];

typedef struct _SAFESYSTEM_PROCESS_INFORMATION {          //SAFESYSTEM_PROCESS_INFORMATION
	int   IntHideType;
	ULONG ulPid;
	ULONG EProcess;
	WCHAR lpwzFullProcessPath[256];
	ULONG ulInheritedFromProcessId;  //父进程
	ULONG ulKernelOpen;
}SAFESYSTEM_PROCESS_INFORMATION, *PSAFESYSTEM_PROCESS_INFORMATION;

typedef struct _PROCESSINFO {          //PROCESSINFO
	ULONG ulCount;
	SAFESYSTEM_PROCESS_INFORMATION ProcessInfo[1];
} PROCESSINFO, *PPROCESSINFO;

extern PPROCESSINFO NormalProcessInfo;

typedef struct _DLL_INFORMATION {          //DLL_INFORMATION
	ULONG ulBase;
	WCHAR lpwzDllModule[256];                    //
} DLL_INFORMATION, *PDLL_INFORMATION;

typedef struct _DLLINFO {          //DLL
	ULONG ulCount;
	DLL_INFORMATION DllInfo[1];
} DLLINFO, *PDLLINFO;

//---------------------------------------------------------------------------------------
extern PDLLINFO PDll;

CImageList ThreadImg;//线程图标~~~