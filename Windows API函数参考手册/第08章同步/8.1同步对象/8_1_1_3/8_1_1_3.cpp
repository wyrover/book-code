// 8_1_1_3.cpp : Defines the entry point for the application.
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
    LoadString(hInstance, IDC_MY8_1_1_3, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }

    hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_MY8_1_1_3);

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
    wcex.hIcon          = LoadIcon(hInstance, (LPCTSTR)IDI_MY8_1_1_3);
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = (LPCSTR)IDC_MY8_1_1_3;
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
//定义信号变量和互斥变量
HANDLE hSemaphore;
HANDLE hMutex;
//定义线程函数，在此定义了四个线程函数
DWORD WINAPI        Thread1(LPVOID param);
DWORD WINAPI        Thread2(LPVOID param);
DWORD WINAPI        Thread3(LPVOID param);
DWORD WINAPI        Thread4(LPVOID param);

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

//下面的部分在程序的主消息循环中
    case WM_CREATE:
        //用互斥变量来保证某一时刻只有一个线程在使用DC
        hMutex = CreateMutex(NULL, FALSE, "HDC");
        //定义了一个最大值为三的信号量，它限制同时只能有三个线程能够绘图
        hSemaphore = CreateSemaphore(NULL, 3, 3, NULL);
        break;

    case WM_RBUTTONDOWN:
        HANDLE hThread1, hThread2, hThread3, hThread4;
        DWORD ThreadID;
        //创建四个线程，并将主窗口的DC作为数据参数传给这些线程
        HDC hDCT;
        hDCT = GetDC(hWnd);
        hThread1 = CreateThread(NULL, 0, Thread1, hDCT, NULL, &ThreadID);
        hThread2 = CreateThread(NULL, 0, Thread2, hDCT, NULL, &ThreadID);
        hThread3 = CreateThread(NULL, 0, Thread3, hDCT, NULL, &ThreadID);
        hThread4 = CreateThread(NULL, 0, Thread4, hDCT, NULL, &ThreadID);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}
//下面为具体的线程函数
DWORD WINAPI  Thread1(LPVOID param)
{
    HDC hDC;
    hDC = (HDC)param;
    //注意：下面的两个语句不能互换，否则可能死锁，也就是把信号看作同步量
    WaitForSingleObject(hSemaphore, INFINITE);
    WaitForSingleObject(hMutex, INFINITE);
    Ellipse(hDC, 0, 0, 200, 200);
    TextOut(hDC, 20, 100, "Thread One", 10);
    ReleaseMutex(hMutex);
    ReleaseSemaphore(hSemaphore, 1, NULL);
    return 1;
}

DWORD WINAPI  Thread2(LPVOID param)
{
    HDC hDC;
    hDC = (HDC)param;
    WaitForSingleObject(hSemaphore, INFINITE);
    WaitForSingleObject(hMutex, INFINITE);
    Ellipse(hDC, 0, 200, 200, 400);
    TextOut(hDC, 20, 300, "Thread Two", 10);
    ReleaseMutex(hMutex);
    ReleaseSemaphore(hSemaphore, 1, NULL);
    return 1;
}

DWORD WINAPI  Thread3(LPVOID param)
{
    HDC hDC;
    hDC = (HDC)param;
    WaitForSingleObject(hSemaphore, INFINITE);
    WaitForSingleObject(hMutex, INFINITE);
    Ellipse(hDC, 200, 0, 400, 200);
    TextOut(hDC, 220, 100, "Thread Three", 12);
    ReleaseMutex(hMutex);
    ReleaseSemaphore(hSemaphore, 1, NULL);
    return 1;
}

DWORD WINAPI  Thread4(LPVOID param)
{
    HDC hDC;
    hDC = (HDC)param;
    WaitForSingleObject(hSemaphore, INFINITE);
    WaitForSingleObject(hMutex, INFINITE);
    Ellipse(hDC, 200, 200, 400, 400);
    TextOut(hDC, 220, 300, "Thread Four", 11);
    ReleaseMutex(hMutex);
    ReleaseSemaphore(hSemaphore, 1, NULL);
    return 1;
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
