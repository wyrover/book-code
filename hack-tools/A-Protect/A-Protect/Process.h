#include "stdafx.h"
#include "Md5.h"

extern unsigned int conv(unsigned int a);

extern WCHAR WindowsXPSP3_CN[1900][50];
extern WCHAR WindowsXPSP2_CN[1753][50];

extern WCHAR Windows2003SP1_CN[1776][50];
extern WCHAR Windows2003SP2_CN[1856][50];

extern WCHAR Windows7Home_CN[2409][50];
extern WCHAR Windows7SP1_CN[2526][50];

BOOL FileVerify(char *lpszFullPath,WCHAR *lpwzFileMd5,WCHAR *lpwzTrue);
CHAR* setClipboardText(CHAR* str);

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

PPROCESSINFO NormalProcessInfo=NULL;

ULONG ulPID;
CImageList ProcessImg;//进程图标
//CImageList ImgList11;

BOOL NtFilePathToDosFilePath(__in LPCTSTR lpwzNtFilePath,__out LPCTSTR lpwzDosFilePath);

extern BOOL bIsPhysicalCheck;
extern BOOL bIsProcMD5Check;
extern WCHAR PhysicalFile[260];
extern void SaveToFile(CHAR *lpszBuffer,WCHAR *lpwzFilePath);
