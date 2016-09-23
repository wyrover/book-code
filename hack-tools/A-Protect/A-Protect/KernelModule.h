#include "stdafx.h"
#include "Md5.h"

typedef struct _SYSINFO_INFORMATION {          //SYSINFO_INFORMATION
	ULONG ulSysBase;
	ULONG SizeOfImage;
	ULONG DriverObject;

	WCHAR lpwzFullSysName[256];
	WCHAR lpwzBaseSysName[256];
	WCHAR lpwzServiceName[256];

	int  IntHideType;     //是否隐藏

} SYSINFO_INFORMATION, *PSYSINFO_INFORMATION;

typedef struct _SYSINFO {          //sysmodule
	ULONG ulCount;
	SYSINFO_INFORMATION SysInfo[1];
} SYSINFO, *PSYSINFO;

PSYSINFO SysModuleInfo;


extern unsigned int conv(unsigned int a);
CImageList KernalModuleImg;//进程图标

BOOL FileVerify(char *lpszFullPath,WCHAR *lpwzFileMd5,WCHAR *lpwzTrue);
CHAR* setClipboardText(CHAR* str);

extern BOOL bIsPhysicalCheck;
extern WCHAR PhysicalFile[260];
extern void SaveToFile(CHAR *lpszBuffer,WCHAR *lpwzFilePath);