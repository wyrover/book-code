#include "StdAfx.h"

#define  SystemModuleInformation 11

#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS)0xC0000004L)
#define STATUS_SUCCESS        0x00000000 
#define STATUS_UNSUCCESSFUL (0xC0000001L)
typedef LONG NTSTATUS;

typedef struct _SYSTEM_MODULE_INFORMATION  // 系统模块信息
{
	ULONG  Reserved[2];  
	ULONG  Base;        
	ULONG  Size;         
	ULONG  Flags;        
	USHORT Index;       
	USHORT Unknown;     
	USHORT LoadCount;   
	USHORT ModuleNameOffset;
	CHAR   ImageName[256];   
} SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;

typedef struct _tagSysModuleList {          //模块链结构
	ULONG ulCount;
	SYSTEM_MODULE_INFORMATION smi[1];
} MODULES, *PMODULES;

extern BOOL bIsStopHookScan;

extern "C" NTSTATUS __stdcall  ZwQuerySystemInformation(
	__in       ULONG SystemInformationClass,
	__inout    PVOID SystemInformation,
	__in       ULONG SystemInformationLength,
	__out_opt  PULONG ReturnLength
	);
//*****************************************************************************************

typedef struct _INLINEHOOKINFO_INFORMATION {          //INLINEHOOKINFO_INFORMATION
	ULONG ulHookType;
	ULONG ulMemoryFunctionBase;    //被挂钩地址
	ULONG ulRealFunctionBase;      //原始地址
	ULONG ulMemoryHookBase;        //HOOK 地址
	CHAR lpszFunction[256];
	CHAR lpszHookModuleImage[256];
	ULONG ulHookModuleBase;
	ULONG ulHookModuleSize;

	WCHAR lpwzRealModuleImage[256];   //原始模块
	ULONG ulRealModuleBase;

} INLINEHOOKINFO_INFORMATION, *PINLINEHOOKINFO_INFORMATION;

typedef struct _INLINEHOOKINFO {          //InlineHook
	ULONG ulCount;
	INLINEHOOKINFO_INFORMATION InlineHook[1];
} INLINEHOOKINFO, *PINLINEHOOKINFO;

PINLINEHOOKINFO InlineHookInfo;

CHAR* setClipboardText(CHAR* str);
DWORD StringToHex(char* strSource);
VOID FixSelectModuleToKernel(ULONG ulModuleBase,WCHAR *ModulePath,char *lpszModulePath);
char *ExtractFileName(char *lpFullFile);
ULONG GetKernelInfo(char *lpKernelName,ULONG *ulBase,ULONG *ulSize);

CImageList KernelHookImg;

extern BOOL bIsPhysicalCheck;
extern WCHAR PhysicalFile[260];
extern void SaveToFile(CHAR *lpszBuffer,WCHAR *lpwzFilePath);