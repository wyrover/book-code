#ifndef _KERNEL_HOOK_CHECK_H
#define _KERNEL_HOOK_CHECK_H

#include "ntifs.h"
#include <ntimage.h>
#include "ntos.h"
#include "ldasm.h"
#include "libdasm.h"
#include "fixrelocation.h"

extern PSERVICE_DESCRIPTOR_TABLE OriginalServiceDescriptorTable;

#define DEEP_LEN 0x03

#define NtosModule 0
#define SelectModule 1

extern WCHAR *SystemKernelFilePath;
extern BYTE *ImageModuleBase;
extern ULONG SystemKernelModuleBase;
extern ULONG SystemKernelModuleSize;

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

PINLINEHOOKINFO SelectModuleInlineHookInfo;

extern BOOL DebugOn;
extern ULONG ulWin32kBase;
extern PDRIVER_OBJECT PDriverObject;

ULONG ulNtDllModuleBase;
int IntHookCount;

extern BOOL bKrnlPDBSuccess;  //是否从ring3里获取了pdb

HANDLE MapFileAsSection(
	PUNICODE_STRING FileName,
	PVOID *ModuleBase
	);

ULONG GetSystemRoutineAddress(
	int IntType,
	PVOID lpwzFunction
	);

BOOL IsAddressInSystem(
	ULONG ulDriverBase,
	ULONG *ulSysModuleBase,
	ULONG *ulSize,
	char *lpszSysModuleImage
	);

PIMAGE_NT_HEADERS RtlImageNtHeader(PVOID ImageBase);

//ULONG GetEatHook(ULONG ulOldAddress,int x,ULONG ulSystemKernelModuleBase,ULONG ulSystemKernelModuleSize);

BOOL EatHookCheck(ULONG ulModuleBase,PINLINEHOOKINFO InlineHookInfo,PNTOSFUNCINFO FuncAddressInfo);

BOOL IsFuncInInitSection(ULONG ulFuncBase,ULONG ulSize);

VOID WcharToChar(__in WCHAR *wzFuncName,__out CHAR *FuncName);

PVOID GetKernelModuleBase(PDRIVER_OBJECT DriverObject,char *KernelModuleName);

#endif