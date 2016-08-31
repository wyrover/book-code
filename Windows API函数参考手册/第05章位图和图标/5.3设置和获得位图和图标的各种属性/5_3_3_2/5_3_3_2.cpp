// 5_3_3_2.cpp : Defines the entry point for the application.
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
    LoadString(hInstance, IDC_MY5_3_3_2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }

    hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_MY5_3_3_2);

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
    wcex.hIcon          = LoadIcon(hInstance, (LPCTSTR)IDI_MY5_3_3_2);
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = (LPCSTR)IDC_MY5_3_3_2;
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
        HBITMAP hBitmap, hOldBitmap;
        BITMAPINFO *pbi;
        BYTE* pbits;
        DWORD addr;
        HDC hDC, hMemDC;
        hDC = GetDC(hWnd);
        hMemDC = CreateCompatibleDC(hDC);
//为一个DIB位图的BITMAPINFOHEADER结构变量分配内存，同时分配16×4个Byte的
//位图调色板内存
        pbi = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER) + 16 * 4);
        pbi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
//该位图的每个像素有4个Bit，从而最多需要24=16个调色板入口项
        pbi->bmiHeader.biBitCount = 4;
        pbi->bmiHeader.biHeight = 16;
        pbi->bmiHeader.biWidth = 16;
        pbi->bmiHeader.biPlanes = 1;
        pbi->bmiHeader.biCompression = BI_RGB;
        hBitmap = CreateDIBSection(hDC, pbi, DIB_RGB_COLORS, (void**)&addr, NULL, 0);
//设置位图的位数据
        int i;
        pbits = (BYTE*)addr;

        for (i = 0; i < 8 * 16; i++)
            pbits[i] = 0x22; //都是第三个颜色,为一个单色块，可以改变该值来观看其变化

//将此新建位图选入一个与窗口DC兼容的内存DC中
        hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
//定义三个颜色
        RGBQUAD rgb[3];
        rgb[0].rgbRed = 0;
        rgb[0].rgbGreen = 255;
        rgb[0].rgbBlue = 0;
        rgb[0].rgbReserved = 0;
        rgb[1].rgbRed = 255;
        rgb[1].rgbGreen = 0;
        rgb[1].rgbBlue = 0;
        rgb[1].rgbReserved = 0;
        rgb[2].rgbRed = 0;
        rgb[2].rgbGreen = 0;
        rgb[2].rgbBlue = 255;
        rgb[2].rgbReserved = 0;
        SetDIBColorTable(hMemDC, 0, 3, rgb);
        hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
        DrawState(hDC, NULL, NULL, (LPARAM)hBitmap, 0, 0, 0, 0, 0, DST_BITMAP);
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
