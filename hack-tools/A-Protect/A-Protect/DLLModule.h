#include "stdafx.h"
#include "Md5.h"
///-------------------------------------------------
extern unsigned int conv(unsigned int a);

extern WCHAR WindowsXPSP3_CN[1900][50];
extern WCHAR WindowsXPSP2_CN[1753][50];

extern WCHAR Windows2003SP1_CN[1776][50];
extern WCHAR Windows2003SP2_CN[1856][50];

extern WCHAR Windows7Home_CN[2409][50];
extern WCHAR Windows7SP1_CN[2526][50];

BOOL FileVerify(char *lpszFullPath,WCHAR *lpwzFileMd5,WCHAR *lpwzTrue);
CHAR* setClipboardText(CHAR* str);
BOOL IsNumber(CHAR *lpszNumber);
BOOL NtFilePathToDosFilePath(__in LPCTSTR lpwzNtFilePath,__out LPCTSTR lpwzDosFilePath);
char *ExtractFileName(char *lpFullFile);
BOOL PrintfDosPath(__in LPCTSTR lpwzNtFullPath,__out LPCTSTR lpwzDosFullPath);
ULONG GetDLLFileSize(char *lpszDLLPath);
BOOL VerifyEmbeddedSignature( LPCWSTR lpFileName );

WCHAR lpwzCurrentDiskString[10];

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
//---------------------------------------------------------------------------------------
//dll
//---------------------------------------------------------------------------------------
typedef struct _DLL_INFORMATION {          //DLL_INFORMATION
	ULONG ulBase;
	WCHAR lpwzDllModule[256];                    //
} DLL_INFORMATION, *PDLL_INFORMATION;

typedef struct _DLLINFO {          //DLL
	ULONG ulCount;
	DLL_INFORMATION DllInfo[1];
} DLLINFO, *PDLLINFO;

PDLLINFO PDll;

//---------------------------------------------------------------------------------------

typedef struct _SERVICESREGISTRY_INFORMATION {          //SERVICESREGISTRY_INFORMATION
	WCHAR lpwzSrvName[256];
	WCHAR lpwzImageName[256];
	WCHAR lpwzDLLPath[256];
	//WCHAR lpwzDescription[256];
	//ULONG BootType;
	//ULONG RunStatus;     //运行状态

} SERVICESREGISTRY_INFORMATION, *PSERVICESREGISTRY_INFORMATION;

typedef struct _SERVICESREGISTRY {          //SERVICESREGISTRY
	ULONG ulCount;
	SERVICESREGISTRY_INFORMATION SrvReg[1];

} SERVICESREGISTRY, *PSERVICESREGISTRY;

PSERVICESREGISTRY DLLServicesRegistry;

extern ULONG ulPID;
extern WCHAR lpwzNum[50];
extern PPROCESSINFO NormalProcessInfo;
//------------------------------------------------------------------

CImageList DllModuleImg;//图标啊图标