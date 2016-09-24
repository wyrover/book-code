// HookDll.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "HookDll.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

#pragma data_seg("SharedDataName")
HHOOK g_hKeyBoardHook = NULL;
CListBox *g_pList;
#pragma data_seg()
#pragma comment(linker,"/section:.SharedDataName,rws")



//HHOOK g_hKeyBoardHook;//全局变量，保存钩子的句柄
LRESULT CALLBACK HookKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam);

//
//  Note!
//
//      If this DLL is dynamically linked against the MFC
//      DLLs, any functions exported from this DLL which
//      call into MFC must have the AFX_MANAGE_STATE macro
//      added at the very beginning of the function.
//
//      For example:
//
//      extern "C" BOOL PASCAL EXPORT ExportedFunction()
//      {
//          AFX_MANAGE_STATE(AfxGetStaticModuleState());
//          // normal function body here
//      }
//
//      It is very important that this macro appear in each
//      function, prior to any calls into MFC.  This means that
//      it must appear as the first statement within the
//      function, even before any object variable declarations
//      as their constructors may generate calls into the MFC
//      DLL.
//
//      Please see MFC Technical Notes 33 and 58 for additional
//      details.
//

/////////////////////////////////////////////////////////////////////////////
// CHookDllApp

BEGIN_MESSAGE_MAP(CHookDllApp, CWinApp)
    //{{AFX_MSG_MAP(CHookDllApp)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHookDllApp construction

CHookDllApp::CHookDllApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CHookDllApp object

CHookDllApp theApp;

LRESULT CALLBACK HookKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0) {
        CString strTemp, strAll;
        strTemp.Format("按键：%c[%d] ", wParam, wParam);
        strAll += strTemp;
        strTemp.Format("类型：%s ", nCode == HC_NOREMOVE ? "非移" : "移除");
        strAll += strTemp;
        //0-15位：描述：按下键盘次数。
        //16-23位:指定扫描码. 依赖于OEM
        //24位:当24位为1时候：表示按键是扩展键;当24位为0时候：表示按键是数字键盘按键
        //25-28位:保留位
        //29位:上下文键：为1时： ALT按下，其他情况为0
        //30位:如果是按键按下后发送的消息，30位为0，如果是按键抬起后30位为1；
        //31位:指定转变状态;31位为0时候，按键正在被按下，为1时候，按键正在被释放
        strTemp.Format("按键次数：%d", (UINT)lParam & 0xFFFF);
        strAll += strTemp;
        strTemp.Format("扫描码：%d", (UINT)(lParam & 0x0FF0000) / 0x10000);
        strAll += strTemp;
        strTemp.Format("键类型：%s", (lParam & 0x1000000) == 0 ? "数字键盘" : "扩展键");
        strAll += strTemp;
        strTemp.Format("ALT：%s", (lParam & 0x20000000) == 0 ? "未按" : "按了");
        strAll += strTemp;
        strTemp.Format("当前状态：%s", (lParam & 0x40000000) == 0 ? "未抬起" : "已抬起");
        strAll += strTemp;
        strTemp.Format("转变状态：%s", (lParam & 0x80000000) == 0 ? "正被按下" : "正被释放");
        strAll += strTemp;
        //AfxMessageBox(strAll);
        g_pList->InsertString(0, strAll);
    }

    return CallNextHookEx(g_hKeyBoardHook, nCode, wParam, lParam);
}

void CHookDllApp::StartHook(CListBox *pList)
{
    g_pList = pList;
    g_hKeyBoardHook = SetWindowsHookEx(WH_KEYBOARD, HookKeyBoardProc, NULL, GetCurrentThreadId());
    //CString strTemp;
    //strTemp.Format("%d",g_hKeyBoardHook);
    //AfxMessageBox(strTemp);
}

void CHookDllApp::StopHook()
{
    UnhookWindowsHookEx(g_hKeyBoardHook);
    g_hKeyBoardHook = NULL;
}

