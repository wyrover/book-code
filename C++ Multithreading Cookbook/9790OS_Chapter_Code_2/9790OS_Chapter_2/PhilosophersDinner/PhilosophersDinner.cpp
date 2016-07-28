#include "stdafx.h"

#define BUTTON_CLOSE            100
#define PHILOSOPHER_COUNT       5
#define WM_INVALIDATE           WM_USER + 1

typedef struct _tagCOMMUNICATIONOBJECT {
    HWND    hWnd;
    bool    bExitApplication;
    int     iPhilosopherArray[PHILOSOPHER_COUNT];
    int     PhilosopherCount;
} COMMUNICATIONOBJECT, *PCOMMUNICATIONOBJECT;

HWND InitInstance(HINSTANCE hInstance, int nCmdShow);
ATOM MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int PhilosopherPass(int iPhilosopher);
void FillEllipse(HWND hWnd, HDC hDC, int iLeft, int iTop, int iRight, int iBottom, int iPass);

TCHAR* szTitle = TEXT("Philosophers Dinner Demo");
TCHAR* szWindowClass = TEXT("__PD_WND_CLASS__");
TCHAR* szSemaphoreName = TEXT("__PD_SEMAPHORE__");
TCHAR* szMappingName = TEXT("__SHARED_FILE_MAPPING__");
PCOMMUNICATIONOBJECT pCommObject = NULL;

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    HANDLE hMapping = CreateFileMapping((HANDLE) - 1, NULL, PAGE_READWRITE, 0, sizeof(COMMUNICATIONOBJECT), szMappingName);

    if (!hMapping) {
        MessageBox(NULL, TEXT("Cannot open file mapping"), TEXT("Error!"), MB_OK);
        return 1;
    }

    pCommObject = (PCOMMUNICATIONOBJECT) MapViewOfFile(hMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);

    if (!pCommObject) {
        MessageBox(NULL, TEXT("Cannot get access to file mapping! "), TEXT("Error!"), MB_OK);
        CloseHandle(hMapping);
        return 1;
    }

    InitCommonControls();
    MyRegisterClass(hInstance);
    HWND hWnd = NULL;

    if (!(hWnd = InitInstance(hInstance, nCmdShow))) {
        return FALSE;
    }

    pCommObject->bExitApplication = false;
    pCommObject->hWnd = hWnd;
    memset(pCommObject->iPhilosopherArray, 0, sizeof(*pCommObject->iPhilosopherArray));
    pCommObject->PhilosopherCount = PHILOSOPHER_COUNT;
    HANDLE hSemaphore = CreateSemaphore(NULL, int(PHILOSOPHER_COUNT / 2), int(PHILOSOPHER_COUNT / 2), szSemaphoreName);
    STARTUPINFO startupInfo[PHILOSOPHER_COUNT] = { { 0 }, { 0 }, { 0 }, { 0 }, { 0 } };
    PROCESS_INFORMATION processInformation[PHILOSOPHER_COUNT] = { { 0 }, { 0 }, { 0 }, { 0 }, { 0 } };
    HANDLE hProcesses[PHILOSOPHER_COUNT];
    TCHAR szBuffer[8];

    for (int iIndex = 0; iIndex < PHILOSOPHER_COUNT; iIndex++) {
#ifdef UNICODE
        wsprintf(szBuffer, L"%d", iIndex);
#else
        sprintf(szBuffer, "%d", iIndex);
#endif

        if (CreateProcess(TEXT("..\\Debug\\Philosopher.exe"), szBuffer, NULL, NULL,
                          FALSE, 0, NULL, NULL, &startupInfo[iIndex], &processInformation[iIndex])) {
            hProcesses[iIndex] = processInformation[iIndex].hProcess;
        }
    }

    MSG msg = { 0 };

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    pCommObject->bExitApplication = true;
    UnmapViewOfFile(pCommObject);
    WaitForMultipleObjects(PHILOSOPHER_COUNT, hProcesses, TRUE, INFINITE);

    for (int iIndex = 0; iIndex < PHILOSOPHER_COUNT; iIndex++) {
        CloseHandle(hProcesses[iIndex]);
    }

    CloseHandle(hSemaphore);
    CloseHandle(hMapping);
    return (int)msg.wParam;
}
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wndEx;
    wndEx.cbSize = sizeof(WNDCLASSEX);
    wndEx.style = CS_HREDRAW | CS_VREDRAW;
    wndEx.lpfnWndProc = WndProc;
    wndEx.cbClsExtra = 0;
    wndEx.cbWndExtra = 0;
    wndEx.hInstance = hInstance;
    wndEx.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wndEx.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndEx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wndEx.lpszMenuName = NULL;
    wndEx.lpszClassName = szWindowClass;
    wndEx.hIconSm = LoadIcon(wndEx.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    return RegisterClassEx(&wndEx);
}
HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION
                             | WS_SYSMENU | WS_MINIMIZEBOX, 200, 200, 540, 590, NULL, NULL, hInstance, NULL);

    if (!hWnd) {
        return NULL;
    }

    HFONT hFont = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, BALTIC_CHARSET, OUT_DEFAULT_PRECIS,
                             CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_MODERN, TEXT("Microsoft Sans Serif"));
    HWND hButton = CreateWindow(TEXT("BUTTON"), TEXT("Close"), WS_CHILD | WS_VISIBLE
                                | BS_PUSHBUTTON | WS_TABSTOP, 410, 520, 100, 25, hWnd, (HMENU)BUTTON_CLOSE, hInstance, NULL);
    SendMessage(hButton, WM_SETFONT, (WPARAM)hFont, TRUE);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return hWnd;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case BUTTON_CLOSE: {
            DestroyWindow(hWnd);
            break;
        }
        }

        break;
    }

    case WM_INVALIDATE: {
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }

    case WM_PAINT: {
        PAINTSTRUCT paintStruct;
        HDC hDC = BeginPaint(hWnd, &paintStruct);
        FillEllipse(hWnd, hDC, 210, 10, 310, 110, PhilosopherPass(1));
        FillEllipse(hWnd, hDC, 410, 170, 510, 270, PhilosopherPass(2));
        FillEllipse(hWnd, hDC, 335, 400, 435, 500, PhilosopherPass(3));
        FillEllipse(hWnd, hDC, 80, 400, 180, 500, PhilosopherPass(4));
        FillEllipse(hWnd, hDC, 10, 170, 110, 270, PhilosopherPass(5));
        EndPaint(hWnd, &paintStruct);
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
int PhilosopherPass(int iPhilosopher)
{
    return pCommObject->iPhilosopherArray[iPhilosopher - 1];
}
void FillEllipse(HWND hWnd, HDC hDC, int iLeft, int iTop, int iRight, int iBottom, int iPass)
{
    HBRUSH hBrush = NULL;

    if (iPass) {
        hBrush = CreateSolidBrush(RGB(255, 0, 0));
    } else {
        hBrush = CreateSolidBrush(RGB(255, 255, 255));
    }

    HBRUSH hOldBrush = (HBRUSH) SelectObject(hDC, hBrush);
    Ellipse(hDC, iLeft, iTop, iRight, iBottom);
    SelectObject(hDC, hOldBrush);
    DeleteObject(hBrush);
}
