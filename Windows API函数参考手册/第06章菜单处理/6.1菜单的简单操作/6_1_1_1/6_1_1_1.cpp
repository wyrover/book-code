// 6_1_1_1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "stdio.h"
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
TCHAR szTitle[MAX_LOADSTRING];                              // The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];                                // The title bar text

// Foward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
    // TODO: Place code here.
    MSG msg;
    HACCEL hAccelTable;
    // Initialize global strings
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_MY6_1_1_1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }

    hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_MY6_1_1_1);

    // Main message loop:
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = (WNDPROC)WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, (LPCTSTR)IDI_MY6_1_1_1);
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = (LPCSTR)IDC_MY6_1_1_1;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);
    return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd;
    hInst = hInstance; // Store instance handle in our global variable
    hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    if (!hWnd) {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR szHello[MAX_LOADSTRING];
    LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

    switch (message) {
    case WM_COMMAND:
        wmId    = LOWORD(wParam);
        wmEvent = HIWORD(wParam);

        // Parse the menu selections:
        switch (wmId) {
        case IDM_ABOUT:
            DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
            break;

        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }

        break;

    case WM_PAINT:
        break;

    case WM_LBUTTONDOWN:
        HMENU   hMenu, hPopupMenu;
        HMENU   hOldMenu;
        MENUITEMINFO mi;
        char    szExit[16], szAbout[16];
        sprintf(szExit, "%s", "&Exit");
        sprintf(szAbout, "%s", "&About");
//设置一个菜单项的具体内容
        mi.cbSize = sizeof(MENUITEMINFO);   //MENUITEMINFO类型变量的大小
        mi.fMask = MIIM_STATE | MIIM_TYPE; //fState和fType分量有效
        mi.fType = MFT_STRING;              //指定菜单项为字符串类型
        mi.fState = MFS_ENABLED;        //指定菜单项的状态为活跃状态
        mi.wID = 0;                         //指定菜单项的标识ID
        mi.hSubMenu = NULL;                 //指定菜单项包含的子菜单句柄
        mi.hbmpChecked = NULL;              //指定菜单项选中时的位图
        mi.hbmpUnchecked = NULL;        //指定菜单项未选中的位图
        mi.dwItemData = 0;                  //指定菜单项的数据
        mi.dwTypeData = szExit;                 //指定菜单项的文本
        mi.cch = strlen(szExit);                //指定菜单项文本的长度
        hMenu = CreateMenu();
        hPopupMenu = CreatePopupMenu();
        InsertMenuItem(hPopupMenu, 101, FALSE, &mi);
//展示一下MFT_MENUBARBREAK的用法
        mi.fType |= MFT_MENUBARBREAK;
        InsertMenuItem(hPopupMenu, 102, FALSE, &mi);
//添加File菜单
        AppendMenu(hMenu, MF_POPUP, (UINT)hPopupMenu, "&File");
//创建一个新的弹出式菜单
        hPopupMenu = CreatePopupMenu();
        mi.fType &= ~MFT_MENUBARBREAK;
        mi.dwTypeData = szAbout;
        InsertMenuItem(hPopupMenu, 103, FALSE, &mi);
//添加Help菜单
        AppendMenu(hMenu, MF_POPUP, (UINT)hPopupMenu, "&Help");
//将新创建的菜单设置为窗口菜单
        hOldMenu = GetMenu(hWnd);
        SetMenu(hWnd, hMenu);
//使用该菜单
//不再使用该菜单，以后先恢复以前的菜单，然后再将其删除，
        SetMenu(hWnd, hOldMenu);
        DestroyMenu(hMenu);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_INITDIALOG:
        return TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return TRUE;
        }

        break;
    }

    return FALSE;
}
