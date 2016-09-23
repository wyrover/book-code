#include "ntifs.h"

extern BOOL DebugOn;

typedef struct _CODE_LINE
{
	int CodeLength;
	BYTE Code[20];
}CODE_LINE,*PCODE_LINE;

typedef struct _CODE_INFO
{
	int LineCount;
	CODE_LINE CodeLine[1];

}CODE_INFO,*PCODE_INFO;

BOOL HookFunctionHeader(DWORD NewFunctionAddress,WCHAR *FunctionName,BOOL bSsdt,DWORD index,PVOID HookZone,int *patchCodeLen,PVOID *lpRet);

void UnHookFunctionHeader(WCHAR *FunctionName,BOOL bSsdt,DWORD index, PVOID HookZone, int patchCodeLen);

BOOL HookFunctionMiddle(BYTE *StartAddress,int MaxLength,DWORD ToAddress,PCODE_INFO CodeInfo,PVOID HookZone,int *patchCodeLen,PVOID *lpRet);

VOID UnHookFunctionMiddle(DWORD PatchAddress,PVOID HookZone,int PatchCodeLen);

BOOL HookFunctionByHeaderAddress(DWORD NewFunctionAddress,DWORD oldFunctionAddress,PVOID HookZone,int *patchCodeLen,PVOID *lpRet);

void UnHookFunctionByHeaderAddress(DWORD oldFunctionAddress,PVOID HookZone, int patchCodeLen);

BOOL GetFunctionIndexByName(CHAR *lpszFunctionName,int *Index);