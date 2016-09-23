#ifndef _SHADOWSSDT_H_
#define _SHADOWSSDT_H_

#include "ntifs.h"
#include "ntos.h"
#include "libdasm.h"

#ifndef WIN_INTERNAL
DECLARE_HANDLE            (HWND);
DECLARE_HANDLE            (HDESK);
DECLARE_HANDLE            (HHOOK);
DECLARE_HANDLE            (HINSTANCE);
#endif

extern PSERVICE_DESCRIPTOR_TABLE OriginalShadowServiceDescriptorTable;
extern PSERVICE_DESCRIPTOR_TABLE ShadowSSDTTable;

extern BYTE *Win32kImageModuleBase;
extern ULONG ulWin32kBase;
extern ULONG ulWin32kSize;

//---------------------------------------------------------------------------------------
//ShadowSSDT
//---------------------------------------------------------------------------------------
typedef struct _SHADOWSSDTINFO_INFORMATION {          //SHADOWSSDTINFO_INFORMATION
	ULONG ulNumber;
	ULONG ulMemoryFunctionBase;
	ULONG ulRealFunctionBase;
	CHAR lpszFunction[256];
	CHAR lpszHookModuleImage[256];
	ULONG ulHookModuleBase;
	ULONG ulHookModuleSize;
	int  IntHookType;                     //
} SHADOWSSDTINFO_INFORMATION, *PSHADOWSSDTINFO_INFORMATION;

typedef struct _SHADOWSSDTINFO {          //SSDT
	ULONG ulCount;
	SHADOWSSDTINFO_INFORMATION SSDT[1];
} SHADOWSSDTINFO, *PSHADOWSSDTINFO;

//---------------------------------------------------------------------------------------

PSHADOWSSDTINFO ShadowSSDTInfo;

BOOL bShadowHooked;
extern BOOL bShadowSSDTAll;

extern HANDLE ProtectProcessId;  //要保护的进程pid，也就是A盾的

int NtUserBuildHwndListIndex;  //EnumWindows 枚举所有顶层窗口
int NtUserGetForegroundWindowIndex;  //GetForegroundWindow 得到当前顶层窗口
int NtUserQueryWindowIndex;   //GetWindowThreadProcessId 获取句柄对应的进程PID
int NtUserFindWindowExIndex;   //FindWindow 查找窗口获取句柄
int NtUserDestroyWindowIndex;  //DestroyWindow 销毁窗口
int NtUserPostMessageIndex;    //PostMessage 发送消息
int NtUserPostThreadMessageIndex; //SendMessage 发送消息
int NtUserSetWindowsHookExIndex; //拦截全局钩子

ULONG LastForegroundWindow;

extern BOOL bDisSetWindowsHook;  //是否允许全局钩子

//要Hook的函数声明
typedef NTSTATUS (__stdcall *NTUSERFINDWINDOWEX)(
	IN HWND hwndParent, 
	IN HWND hwndChild, 
	IN PUNICODE_STRING pstrClassName OPTIONAL, 
	IN PUNICODE_STRING pstrWindowName OPTIONAL, 
	IN DWORD dwType);

typedef NTSTATUS (__stdcall *NTUSERBUILDHWNDLIST)(
	IN HDESK hdesk,
	IN HWND hwndNext, 
	IN ULONG fEnumChildren, 
	IN DWORD idThread, 
	IN UINT cHwndMax, 
	OUT HWND *phwndFirst, 
	OUT ULONG *pcHwndNeeded);

typedef UINT_PTR (__stdcall *NTUSERQUERYWINDOW)(
	IN ULONG WindowHandle,
	IN ULONG TypeInformation);

typedef BOOL (__stdcall *NTUSERDESTROYWINDOW)(
	IN HWND hWnd
	);

typedef ULONG (*NTUSERGETFOREGROUNDWINDOW)(VOID);

typedef NTSTATUS (__stdcall *NTUSERPOSTMESSAGE)(
	IN HWND hWnd,
	IN ULONG pMsg,
	IN ULONG wParam,
	IN ULONG lParam
	);

typedef BOOL (__stdcall *NTUSERPOSTTHREADMESSAGE)(
	IN DWORD idThread,
	IN ULONG Msg,
	IN ULONG wParam,
	IN ULONG lParam
	);

//挂钩NtuserSetWindowsHookex用来拦截全局钩子
typedef HHOOK (*NTUSERSETWINDOWSHOOKEX)(
	HINSTANCE Mod, 
	PUNICODE_STRING UnsafeModuleName, 
	DWORD ThreadId, 
	int HookId, 
	PVOID HookProc, 
	BOOL Ansi
	);
//---------------------------------------------------------------------------------------
NTSTATUS PsLookupThreadByThreadId(
	__in   HANDLE ThreadId,
	__out  PETHREAD *Thread
	);
//-----------------------------------------------------------------------------------------
ULONG GetSystemRoutineAddress(
	int IntType,
	PVOID lpwzFunction
	);

unsigned long __fastcall GetFunctionCodeSize(
	void *Proc
	);

BOOL IsAddressInSystem(
	ULONG ulDriverBase,
	ULONG *ulSysModuleBase,
	ULONG *ulSize,
	char *lpszSysModuleImage
	);

BOOL RestoreShadowInlineHook(ULONG ulNumber);

BOOL MmQueryShadowSSDTAddr(ULONG ImageBase,DWORD *ShadowSSDT);

BOOL IsFuncInInitSection(ULONG ulFuncBase,ULONG ulSize);

BOOL SystemCallEntryShadowSSDTTableHook(char *FunctionName,int *Index,DWORD NewFuctionAddress);

#endif