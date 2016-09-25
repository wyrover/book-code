#include <stdio.h>
#include <Windows.h>
#include "DrvCtrl.h"

void LoadDriver(int opr)
{
    if (opr != 0)
        hMyDrv = openDriver();
    else
        uninstallDriver();
}

void RKM(UINT64 Address, PVOID Buffer, SIZE_T Length)
{
    IoControl(hMyDrv , CTL_CODE_GEN(0x809), &Address, 8, NULL, 0);      //address
    IoControl(hMyDrv , CTL_CODE_GEN(0x80A), &Length, 8, NULL, 0);       //length
    IoControl(hMyDrv , CTL_CODE_GEN(0x804), NULL, 0, Buffer, Length);   //get buffer
}

void WKM(UINT64 Address, PVOID Buffer, SIZE_T Length)
{
    IoControl(hMyDrv , CTL_CODE_GEN(0x809), &Address, 8, NULL, 0);      //address
    IoControl(hMyDrv , CTL_CODE_GEN(0x80A), &Length, 8, NULL, 0);       //length
    IoControl(hMyDrv , CTL_CODE_GEN(0x805), Buffer, Length, NULL, 0);   //set buffer
}

UINT64 GetQWORD(UINT64 address)
{
    UINT64 y = 0;
    RKM(address, &y, 8);
    return y;
}

UINT32 GetDWORD(UINT64 address)
{
    UINT32 y = 0;
    RKM(address, &y, 4);
    return y;
}

PUCHAR GetPNbyET(UINT64 ethread)
{
    PUCHAR y = (PUCHAR)malloc(16);
    IoControl(hMyDrv , CTL_CODE_GEN(0x7FF), &ethread, 8, y, 16);
    return y;
}

/*
lkd> dt win32k!tagsharedinfo
   +0x000 psi              : Ptr64 tagSERVERINFO
   +0x008 aheList          : Ptr64 _HANDLEENTRY
   +0x010 HeEntrySize      : Uint4B
   +0x018 pDispInfo        : Ptr64 tagDISPLAYINFO
   +0x020 ulSharedDelta    : Uint8B
   +0x028 awmControl       : [31] _WNDMSG
   +0x218 DefWindowMsgs    : _WNDMSG
   +0x228 DefWindowSpecMsgs : _WNDMSG

lkd> dt win32k!tagSERVERINFO
   +0x000 dwSRVIFlags      : Uint4B
   +0x008 cHandleEntries   : Uint8B
   +0x010 mpFnidPfn        : [32] Ptr64     int64
   +0x110 aStoCidPfn       : [7] Ptr64     int64
   [省略......]
*/

/*win32k!_HANDLEENTRY
   +0x000 phead            : Ptr64 _HEAD
   +0x008 pOwner           : Ptr64 Void
   +0x010 bType            : UChar
   +0x011 bFlags           : UChar
   +0x012 wUniq            : Uint2B*/

typedef struct _HANDLEENTRY {
    UINT64  phead;
    UINT64  pOwner;
    UCHAR   bType;
    UCHAR   bFlags;
    USHORT  wUniq;
} HANDLEENTRY, *PHANDLEENTRY;

/*
lkd> dt win32k!taghook
   +0x000 head             : _THRDESKHEAD
   +0x028 phkNext          : Ptr64 tagHOOK
   +0x030 iHook            : Int4B
   +0x038 offPfn           : Uint8B
   +0x040 flags            : Uint4B
   +0x044 ihmod            : Int4B
   +0x048 ptiHooked        : Ptr64 tagTHREADINFO
   +0x050 rpdesk           : Ptr64 tagDESKTOP
   +0x058 nTimeout         : Pos 0, 7 Bits
   +0x058 fLastHookHung    : Pos 7, 1 Bit
lkd> dt_THRDESKHEAD
win32k!_THRDESKHEAD
   +0x000 h                : Ptr64 Void
   +0x008 cLockObj         : Uint4B
   +0x010 pti              : Ptr64 tagTHREADINFO
   +0x018 rpdesk           : Ptr64 tagDESKTOP
   +0x020 pSelf            : Ptr64 UChar
*/

typedef struct _HOOK_INFO {
    HANDLE       hHandle;                    //钩子的句柄   句柄是全局的 可以UnhookWindowsHookEx  把钩子卸掉
    int          Unknown1;
    PVOID        Win32Thread;                 //一个指向 win32k!_W32THREAD 结构体的指针
    PVOID        Unknown2;
    PVOID        SelfHook;                   //指向结构体的首地址
    PVOID        NextHook;                   //指向下一个钩子结构体
    int          iHookType;                 //钩子的类型， winuser.h 中有定义
    PVOID        OffPfn;                    //钩子函数的地址偏移，相对于所在模块的偏移
    int          iHookFlags;
    int          iMod;                      //钩子函数做在模块的索引号码，通过查询 WowProcess 结构可以得到模块的基地址。
    PVOID        Win32ThreadHooked;         // ？？？被钩的线程的结构指针，不知道
    //下面还有，省略。。。
} HOOK_INFO, *PHOOK_INFO;

char *GetHookType(int Id)
{
    char *string;
    string = (char*)malloc(32);

    switch (Id) {
    case -1: {
        strcpy(string, "WH_MSGFILTER");
        break;
    }

    case 0: {
        strcpy(string, "WH_JOURNALRECORD");
        break;
    }

    case 1: {
        strcpy(string, "WH_JOURNALPLAYBACK");
        break;
    }

    case 2: {
        strcpy(string, "WH_KEYBOARD");
        break;
    }

    case 3: {
        strcpy(string, "WH_GETMESSAGE");
        break;
    }

    case 4: {
        strcpy(string, "WH_CALLWNDPROC");
        break;
    }

    case 5: {
        strcpy(string, "WH_CBT");
        break;
    }

    case 6: {
        strcpy(string, "WH_SYSMSGFILTER");
        break;
    }

    case 7: {
        strcpy(string, "WH_MOUSE");
        break;
    }

    case 8: {
        strcpy(string, "WH_HARDWARE");
        break;
    }

    case 9: {
        strcpy(string, "WH_DEBUG");
        break;
    }

    case 10: {
        strcpy(string, "WH_SHELL");
        break;
    }

    case 11: {
        strcpy(string, "WH_FOREGROUNDIDLE");
        break;
    }

    case 12: {
        strcpy(string, "WH_CALLWNDPROCRET");
        break;
    }

    case 13: {
        strcpy(string, "WH_KEYBOARD_LL");
        break;
    }

    case 14: {
        strcpy(string, "WH_MOUSE_LL");
        break;
    }

    default: {
        strcpy(string, "????");
        break;
    }
    }

    return string;
}

char *GetHookFlagString(int Flag)
{
    char *string;
    string = (char*)malloc(8);

    if (Flag == 1 || Flag == 3)
        strcpy(string, "Global");
    else
        strcpy(string, "Local");

    return string;
}

void EnumMsgHook()
{
    int i = 0;
    UINT64 pgSharedInfo = (UINT64)GetProcAddress(GetModuleHandleA("user32.dll"), "gSharedInfo");
    UINT64 phe = GetQWORD(pgSharedInfo + 8); //+0x008 aheList          : Ptr64 _HANDLEENTRY
    UINT64 count = GetQWORD(GetQWORD(pgSharedInfo) + 8);
    HANDLEENTRY heStruct = {0};
    HOOK_INFO Hook = {0};

    for (i = 0; i < count; i++) {
        memcpy(&heStruct, (PVOID)(phe + i * sizeof(HANDLEENTRY)), sizeof(HANDLEENTRY));

        if (heStruct.bType == 5) {
            RKM(heStruct.phead, &Hook, sizeof(HOOK_INFO));
            printf("hHandle:     0x%llx\n", Hook.hHandle);
            printf("iHookFlags:  %s\n", GetHookFlagString(Hook.iHookFlags));
            printf("iHookType:   %s\n", GetHookType(Hook.iHookType));
            printf("OffPfn:      0x%llx\n", Hook.OffPfn);
            printf("ETHREAD:     0x%llx\n", GetQWORD((UINT64)(Hook.Win32Thread)));
            printf("ProcessName: %s\n\n", GetPNbyET(GetQWORD((UINT64)(Hook.Win32Thread))));
        }
    }
}

int main()
{
    LoadDriver(1);
    EnumMsgHook();
    LoadDriver(0);
}