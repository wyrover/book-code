// 2_7_1_3.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"

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
    LoadString(hInstance, IDC_MY2_7_1_3, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }

    hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_MY2_7_1_3);

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
    wcex.hIcon          = LoadIcon(hInstance, (LPCTSTR)IDI_MY2_7_1_3);
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = (LPCSTR)IDC_MY2_7_1_3;
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
        unsigned char* lpGlobal, *lpTemplate;
        DLGTEMPLATE* lpdt;
        DLGITEMTEMPLATE* lpdit;
        LPWORD  lpw;
        int length;
        int num;
        RECT rect;
        char szName[64];
        unsigned long temp;
//为对话框模板分配内存
        lpGlobal = (unsigned char*)LocalAlloc(LMEM_FIXED, 2048);
        memset(lpGlobal, 0, 2048);
        lpTemplate = lpGlobal;
        lpdt = (DLGTEMPLATE*)lpGlobal;
        lpdt->style = WS_SYSMENU | WS_OVERLAPPEDWINDOW | WS_DLGFRAME;
        lpdt->dwExtendedStyle = 0;
        lpdt->cdit = 3;
        lpdt->x = 0;
        lpdt->y = 0;
        lpdt->cx = 150;
        lpdt->cy = 60;
        length = (sizeof(DLGTEMPLATE) + 1) / 2;
        lpw = (LPWORD)(((LPWORD)lpGlobal) + length);
        *lpw = 0; //没有菜单
        lpw += 1;
        *lpw = 0; //使用预定义控件
        lpw += 1;
        wsprintf(szName, "%s", "About");
        num = MultiByteToWideChar(CP_ACP, MB_COMPOSITE,
                                  szName, -1, (LPWSTR)lpw, 0);
        MultiByteToWideChar(CP_ACP, MB_COMPOSITE,
                            szName, -1, (LPWSTR)lpw, num);
        length = wcslen(lpw) + 1;
        lpw += length;
        lpGlobal = (unsigned char*)lpw;
//将lpGlobal以DWORD对齐
        temp = ((unsigned long)lpGlobal) & 3;

        if (temp) {
            lpGlobal = (unsigned char*)(((unsigned long)lpGlobal) + 3);
            lpGlobal = (unsigned char*)(((unsigned long)lpGlobal) & 0xfffffffc);
        }

        lpdit = (DLGITEMTEMPLATE*)lpGlobal;
        lpdit->style = WS_VISIBLE | WS_CHILD | SS_ICON;
        lpdit->dwExtendedStyle = 0;
        lpdit->x = 10;
        lpdit->y = 20;
        lpdit->cx = 32;
        lpdit->cy = 32;
        lpdit->id = IDC_MYICON;
        lpGlobal += sizeof(DLGITEMTEMPLATE);
        lpw = (LPWORD)lpGlobal;
        *lpw = 0xffff;
        lpw += 1;
        *lpw = 0x0082; //静态图标框
        lpw += 1;
        *lpw = 0xffff;
        lpw += 1;
        *lpw = IDI_CHAPTER3; //图标ID
        lpw += 1;
        *lpw = 0x0; //没有额外的数据
        lpw += 1;
        lpGlobal = (unsigned char*)lpw;
        temp = ((unsigned long)lpGlobal) & 3;

        if (temp) {
            lpGlobal = (unsigned char*)(((unsigned long)lpGlobal) + 3);
            lpGlobal = (unsigned char*)(((unsigned long)lpGlobal) & 0xfffffffc);
        }

        lpdit = (DLGITEMTEMPLATE*)lpGlobal;
        lpdit->style = WS_VISIBLE | WS_CHILD;
        lpdit->dwExtendedStyle = 0;
        lpdit->x = 40;
        lpdit->y = 20;
        lpdit->cx = 100;
        lpdit->cy = 20;
        lpdit->id = IDC_STATIC;
        lpGlobal += sizeof(DLGITEMTEMPLATE);
        lpw = (LPWORD)lpGlobal;
        *lpw = 0xffff;
        lpw += 1;
        *lpw = 0x0082; //静态文本框
        lpw += 1;
        wsprintf(szName, "%s", "CopyRight 2002");
        num = MultiByteToWideChar(CP_ACP, MB_COMPOSITE,
                                  szName, -1, (LPWSTR)lpw, 0);
        MultiByteToWideChar(CP_ACP, MB_COMPOSITE,
                            szName, -1, (LPWSTR)lpw, num);
        length = wcslen(lpw) + 1;
        lpw += length;
        *lpw = 0; //no extra data
        lpw += 1;
        lpGlobal = (unsigned char*)lpw;
        temp = ((unsigned long)lpGlobal) & 3;

        if (temp) {
            lpGlobal = (unsigned char*)(((unsigned long)lpGlobal) + 3);
            lpGlobal = (unsigned char*)(((unsigned long)lpGlobal) & 0xfffffffc);
        }

        lpdit = (DLGITEMTEMPLATE*)lpGlobal;
        lpdit->style = WS_VISIBLE | WS_CHILD;
        lpdit->dwExtendedStyle = 0;
        lpdit->x = 115;
        lpdit->y = 20;
        lpdit->cx = 30;
        lpdit->cy = 10;
        lpdit->id = IDOK;
        lpGlobal += sizeof(DLGITEMTEMPLATE);
        lpw = (LPWORD)lpGlobal;
        *lpw = 0xffff;
        lpw += 1;
        *lpw = 0x0080; //Button控件
        lpw += 1;
        wsprintf(szName, "%s", "OK");
        num = MultiByteToWideChar(CP_ACP, MB_COMPOSITE,
                                  szName, -1, (LPWSTR)lpw, 0);
        MultiByteToWideChar(CP_ACP, MB_COMPOSITE,
                            szName, -1, (LPWSTR)lpw, num);
        length = wcslen(lpw) + 1;
        lpw += length;
        *lpw = 0; //no extra data
        lpw += 1;
        lpGlobal = (unsigned char*)lpw;
        HWND hDlg;
        hDlg = CreateDialogIndirect(hInst, (LPDLGTEMPLATE)lpTemplate,
                                    hWnd, (DLGPROC)About);
        LocalFree((HGLOBAL)lpTemplate);
        ShowWindow(hDlg, SW_SHOWNORMAL) ;
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
