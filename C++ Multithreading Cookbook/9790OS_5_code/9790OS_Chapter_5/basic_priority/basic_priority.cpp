#include <windows.h>
#include <windowsx.h>
#include <time.h>
#include <tchar.h>
#include <math.h>
#include <commctrl.h>

#pragma comment ( lib, "comctl32.lib" )
#pragma comment ( linker, "\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"" )

#define RESET_EVENT     _T( "__tmp_reset_event__" )
#define WINDOW_WIDTH    800
#define WINDOW_HEIGHT   600

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
DWORD WINAPI StartAddress(LPVOID lpParameter);
void DrawProc(HWND hWnd);
unsigned GetNextSeed(void);

int WINAPI _tWinMain(HINSTANCE hThis, HINSTANCE hPrev, LPTSTR szCommandLine, int iWndShow)
{
    UNREFERENCED_PARAMETER(hPrev);
    UNREFERENCED_PARAMETER(szCommandLine);
    TCHAR* szWindowClass = _T("__priority_wnd_class__");
    WNDCLASSEX wndEx = { 0 };
    wndEx.cbSize = sizeof(WNDCLASSEX);
    wndEx.style = CS_HREDRAW | CS_VREDRAW;
    wndEx.lpfnWndProc = WindowProcedure;
    wndEx.cbClsExtra = 0;
    wndEx.cbWndExtra = 0;
    wndEx.hInstance = hThis;
    wndEx.hIcon = LoadIcon(wndEx.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wndEx.hIconSm = LoadIcon(wndEx.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wndEx.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndEx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wndEx.lpszMenuName = NULL;
    wndEx.lpszClassName = szWindowClass;

    if (!RegisterClassEx(&wndEx)) {
        return 1;
    }

    HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, RESET_EVENT);
    InitCommonControls();
    HWND hWnd = CreateWindow(szWindowClass, _T("Basic thread priority"), WS_OVERLAPPED | WS_CAPTION
                             | WS_SYSMENU | WS_MINIMIZEBOX, 50, 50, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, wndEx.hInstance, NULL);

    if (!hWnd) {
        return NULL;
    }

    HFONT hFont = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, BALTIC_CHARSET, OUT_DEFAULT_PRECIS,
                             CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_MODERN, _T("Microsoft Sans Serif"));
    ShowWindow(hWnd, iWndShow);
    MSG msg;

    while (GetMessage(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    CloseHandle(hEvent);
    UnregisterClass(wndEx.lpszClassName, wndEx.hInstance);
    return (int)msg.wParam;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_CREATE: {
        DWORD dwThreadId = 0;
        SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
        HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartAddress, hWnd, 0, &dwThreadId);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)dwThreadId);
        Sleep(100);
        CloseHandle(hThread);
        break;
    }

    case WM_CLOSE: {
        HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, RESET_EVENT);
        SetEvent(hEvent);
        DWORD dwThreadId = (DWORD) GetWindowLongPtr(hWnd, GWLP_USERDATA);
        HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, dwThreadId);
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
        DestroyWindow(hWnd);
        break;
    }

    case WM_DESTROY: {
        PostQuitMessage(0);
        break;
    }

    default: {
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    }

    return 0;
}

DWORD WINAPI StartAddress(LPVOID lpParameter)
{
    HWND hWnd = (HWND)lpParameter;
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL);
    HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, RESET_EVENT);
    DWORD dwWaitResult(1);

    while (dwWaitResult != WAIT_OBJECT_0) {
        dwWaitResult = WaitForSingleObject(hEvent, 100);
        DrawProc(hWnd);
    }

    CloseHandle(hEvent);
    return 0L;
}

void DrawProc(HWND hWnd)
{
    int iTotal = 10;
    srand(GetNextSeed());
    HDC hDC = GetDC(hWnd);

    if (hDC) {
        for (int iCount = 0; iCount < iTotal; iCount++) {
            int xStart = (int)(rand() % WINDOW_WIDTH);
            int xStop = (int)(rand() % WINDOW_WIDTH);
            int yStart = (int)(rand() % WINDOW_HEIGHT);
            int yStop = (int)(rand() % WINDOW_HEIGHT);
            int iRed = rand() & 255;
            int iGreen = rand() & 255;
            int iBlue = rand() & 255;
            HANDLE hBrush = CreateSolidBrush(GetNearestColor(hDC, RGB(iRed, iGreen, iBlue)));
            HANDLE hbrOld = SelectBrush(hDC, hBrush);
            Rectangle(hDC, min(xStart, xStop), max(xStart, xStop), min(yStart, yStop), max(yStart, yStop));
            DeleteBrush(SelectBrush(hDC, hbrOld));
        }

        ReleaseDC(hWnd, hDC);
    }
}

unsigned GetNextSeed(void)
{
    static unsigned seed = (unsigned)time(NULL);
    return ++seed;
}
