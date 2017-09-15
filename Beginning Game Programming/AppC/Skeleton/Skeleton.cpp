//-----------------------------------------------------------------
// Skeleton Application
// C++ Source - Skeleton.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Skeleton.h"

//-----------------------------------------------------------------
// Global Function Declarations
//-----------------------------------------------------------------
LRESULT CALLBACK  WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);

//-----------------------------------------------------------------
// Global Functions
//-----------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   PSTR szCmdLine, int iCmdShow)
{
    static TCHAR  szAppName[] = TEXT("Skeleton");
    WNDCLASSEX    wndclass;
    HWND          hWindow;
    MSG           msg;
    // Create the window class for the main window
    wndclass.cbSize         = sizeof(wndclass);
    wndclass.style          = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc    = WndProc;
    wndclass.cbClsExtra     = 0;
    wndclass.cbWndExtra     = 0;
    wndclass.hInstance      = hInstance;
    wndclass.hIcon          = LoadIcon(hInstance,
                                       MAKEINTRESOURCE(IDI_SKELETON));
    wndclass.hIconSm        = LoadIcon(hInstance,
                                       MAKEINTRESOURCE(IDI_SKELETON_SM));
    wndclass.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wndclass.lpszMenuName   = NULL;
    wndclass.lpszClassName  = szAppName;

    // Register the window class
    if (!RegisterClassEx(&wndclass))
        return 0;

    // Create the window
    hWindow = CreateWindow(szAppName, szAppName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                           CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
    // Show and update the window
    ShowWindow(hWindow, iCmdShow);
    UpdateWindow(hWindow);

    // Enter the main message loop
    while (GetMessage(&msg, NULL, 0, 0)) {
        // Process the message
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HDC         hDC;
    PAINTSTRUCT ps;
    RECT        rect;

    switch (msg) {
    case WM_PAINT:
        // Draw some text centered in the client area of the main window
        hDC = BeginPaint(hWindow, &ps);
        GetClientRect(hWindow, &rect);
        DrawText(hDC, TEXT("This is a skeleton application!"), -1, &rect,
                 DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        EndPaint(hWindow, &ps);
        return 0;

    case WM_DESTROY:
        // Exit the application
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWindow, msg, wParam, lParam);
}
