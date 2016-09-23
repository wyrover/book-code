#include "stdafx.h"
#include <stdio.h>
#include <Windows.h>
#include <DbgHelp.h>

#pragma comment(lib , "DbgHelp.lib")
#pragma comment(lib , "ntdll.lib")

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

extern "C" NTSTATUS __stdcall  ZwQuerySystemInformation(
  __in       ULONG SystemInformationClass,
  __inout    PVOID SystemInformation,
  __in       ULONG SystemInformationLength,
  __out_opt  PULONG ReturnLength
);
//-------------------------------------------------------------------------
//保存所有内核函数的一个结构
//-------------------------------------------------------------------------
typedef struct _KERNELFUNC_ADDRESS_INFORMATION {
	ULONG ulAddress;
	ULONG ulReloadAddress;
	WCHAR FuncName[50];
	int NumberOfFunctions; //函数在导出表里面的位置
}KERNELFUNC_ADDRESS_INFORMATION, *PKERNELFUNC_ADDRESS_INFORMATION;

typedef struct _NTOSFUNCINFO {          //PNTOSFUNCINFO
	ULONG ulCount;
	ULONG ulModuleBase;    //模块的基址
	WCHAR ModulePath[260];
	KERNELFUNC_ADDRESS_INFORMATION NtosFuncInfo[1];
} NTOSFUNCINFO, *PNTOSFUNCINFO;

PNTOSFUNCINFO NtosFuncAddressInfo;
PNTOSFUNCINFO FuncAddressInfo;
//-------------------------------------------------------------------------

typedef PLOADED_IMAGE(__stdcall *IMAGELOAD)(
								 __in  PSTR DllName,
								 __in  PSTR DllPath
								 );
IMAGELOAD pImageLoad;

typedef BOOL(__stdcall *IMAGEUNLOAD)(
	__in  PLOADED_IMAGE LoadedImage
	);
IMAGEUNLOAD pImageUnload;

typedef BOOL(__stdcall *SYMGETSYMBOLFILE)(
	__in_opt HANDLE hProcess,
	__in_opt PCSTR SymPath,
	__in PCSTR ImageFile,
	__in DWORD Type,
	__out_ecount(cSymbolFile) PSTR SymbolFile,
	__in size_t cSymbolFile,
	__out_ecount(cDbgFile) PSTR DbgFile,
	__in size_t cDbgFile
	);
SYMGETSYMBOLFILE pSymGetSymbolFile;

HANDLE hProcess;
PLOADED_IMAGE pli;
char* SymbolsUrl = "http://msdl.microsoft.com/download/symbols";
ULONG ulSize;
LPSTR ExtractFileName(LPSTR lpcFullFileName);
