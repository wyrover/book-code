#pragma once

/********************************************************
*   You need to specify text file with coordinates.     *
*   Our example will solve system of 2 linear equations *
*   a1x + b1y = c1;                                     *
*   a2x + b2y = c2;                                     *
*                                                       *
*   Each line should have exactly six coordinates.      *
*   File format example:                                *
*                                                       *
*    4 -3  2 -7  5 6                                    *
*   12  1 -7  9 -5 8                                    *
*                                                       *
*   Avoid new line at the end of the file.              *
*********************************************************/
#include "main.h"

char szFilePath[MAX_PATH] = { 0 };
char szResult[4096];
CQueue<QueueElement> queue;
TCHAR* szEvent = _T("__event_879__");
TCHAR* szMutex = _T("__mutex_132__");

int WINAPI _tWinMain(HINSTANCE hThis, HINSTANCE hPrev, LPTSTR szCommandLine, int iWndShow)
{
    UNREFERENCED_PARAMETER(hPrev);
    UNREFERENCED_PARAMETER(szCommandLine);
    TCHAR* szWindowClass = _T("__concurrent_operations__");
    WNDCLASSEX wndEx = { 0 };
    wndEx.cbSize = sizeof(WNDCLASSEX);
    wndEx.style = CS_HREDRAW | CS_VREDRAW;
    wndEx.lpfnWndProc = WindowProcedure;
    wndEx.cbClsExtra = 0;
    wndEx.cbWndExtra = 0;
    wndEx.hInstance = hThis;
    wndEx.hIcon = LoadIcon(wndEx.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wndEx.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndEx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wndEx.lpszMenuName = NULL;
    wndEx.lpszClassName = szWindowClass;
    wndEx.hIconSm = LoadIcon(wndEx.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

    if (!RegisterClassEx(&wndEx)) {
        return 1;
    }

    InitCommonControls();
    HWND hWnd = CreateWindow(szWindowClass, _T("Concurrent operations"), WS_OVERLAPPED
                             | WS_CAPTION | WS_SYSMENU, 50, 50, 305, 250, NULL, NULL, wndEx.hInstance, NULL);

    if (!hWnd) {
        return NULL;
    }

    HFONT hFont = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, BALTIC_CHARSET, OUT_DEFAULT_PRECIS,
                             CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_MODERN, _T("Microsoft Sans Serif"));
    HWND hLabel = CreateWindow(_T("STATIC"), _T("  Press \"Browse\" and choose file with coordinates: "), WS_CHILD
                               | WS_VISIBLE | SS_CENTERIMAGE | SS_LEFT | WS_BORDER, 20, 20, 250, 25, hWnd, (HMENU)CONTROL_TEXT, wndEx.hInstance, NULL);
    SendMessage(hLabel, WM_SETFONT, (WPARAM)hFont, TRUE);
    HWND hResult = CreateWindow(_T("STATIC"), _T(""), WS_CHILD | WS_VISIBLE | SS_LEFT
                                | WS_BORDER, 20, 65, 150, 75, hWnd, (HMENU)CONTROL_RESULT, wndEx.hInstance, NULL);
    SendMessage(hResult, WM_SETFONT, (WPARAM)hFont, TRUE);
    HWND hBrowse = CreateWindow(_T("BUTTON"), _T("Browse"), WS_CHILD | WS_VISIBLE
                                | BS_PUSHBUTTON, 190, 65, 80, 25, hWnd, (HMENU)CONTROL_BROWSE, wndEx.hInstance, NULL);
    SendMessage(hBrowse, WM_SETFONT, (WPARAM)hFont, TRUE);
    HWND hStart = CreateWindow(_T("BUTTON"), _T("Start"), WS_CHILD | WS_VISIBLE
                               | BS_PUSHBUTTON, 190, 115, 80, 25, hWnd, (HMENU)CONTROL_START, wndEx.hInstance, NULL);
    SendMessage(hStart, WM_SETFONT, (WPARAM)hFont, TRUE);
    HWND hProgress = CreateWindow(PROGRESS_CLASS, _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER,
                                  20, 165, 250, 25, hWnd, (HMENU)CONTROL_PROGRESS, wndEx.hInstance, NULL);
    SendMessage(hProgress, PBM_SETSTEP, (WPARAM)1, 0);
    SendMessage(hProgress, PBM_SETPOS, (WPARAM)0, 0);
    ShowWindow(hWnd, iWndShow);
    HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, szEvent);
    HANDLE hMutex = CreateMutex(NULL, FALSE, szMutex);
    MSG msg;

    while (GetMessage(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    CloseHandle(hMutex);
    CloseHandle(hEvent);
    UnregisterClass(wndEx.lpszClassName, wndEx.hInstance);
    return (int)msg.wParam;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_DESTROY: {
        PostQuitMessage(0);
        break;
    }

    case WM_CLOSE: {
        HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, szEvent);
        SetEvent(hEvent);
        CloseHandle(hEvent);
        DestroyWindow(hWnd);
        break;
    }

    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case CONTROL_BROWSE: {
            if (!FileDialog(hWnd, szFilePath)) {
                MessageBox(hWnd, _T("You must choose valid file path!"), _T("Error!"), MB_OK | MB_TOPMOST | MB_ICONERROR);
            } else {
                char szBuffer[MAX_PATH];
                wsprintfA(szBuffer, "\n  File: %s Press \"Start\" now.", strrchr(szFilePath, '\\') + 1);
                SetWindowTextA(GetDlgItem(hWnd, CONTROL_TEXT), szBuffer);
            }

            break;
        }

        case CONTROL_START: {
            if (!*szFilePath) {
                MessageBox(hWnd, _T("You must choose valid file path first!"), _T("Error!"), MB_OK | MB_TOPMOST | MB_ICONERROR);
                break;
            }

            ifstream infile(szFilePath);

            if (infile.is_open()) {
                string line;

                while (std::getline(infile, line)) {
                    QueueElement* pQElement = new QueueElement();
                    istringstream iss(line);

                    if (!(iss >> pQElement->iA1 >> pQElement->iB1 >> pQElement->iC1 >> pQElement->iA2 >> pQElement->iB2 >> pQElement->iC2)) {
                        break;
                    }

                    pQElement->hWndProgress = GetDlgItem(hWnd, CONTROL_PROGRESS);
                    pQElement->hWndResult = GetDlgItem(hWnd, CONTROL_RESULT);
                    queue.Enqueue(pQElement);
                }

                infile.close();
                SendMessage(GetDlgItem(hWnd, CONTROL_PROGRESS), PBM_SETRANGE, 0, MAKELPARAM(0, queue.Count()));
                SYSTEM_INFO sysInfo;
                GetSystemInfo(&sysInfo);

                for (DWORD dwIndex = 0; dwIndex < sysInfo.dwNumberOfProcessors; dwIndex++) {
                    HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartAddress, &queue, 0, NULL);
                    SetThreadIdealProcessor(hThread, dwIndex);
                    Sleep(100);
                    CloseHandle(hThread);
                }
            } else {
                MessageBox(hWnd, _T("Cannot open file!"), _T("Error!"), MB_OK | MB_TOPMOST | MB_ICONERROR);
            }

            break;
        }
        }

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
    // We'll solve the linear system using Cramer's rule:
    CQueue<QueueElement>* pQueue = (CQueue<QueueElement>*) lpParameter;
    HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, szEvent);
    HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, szMutex);
    QueueElement* pQElement = NULL;

    while (true) {
        DWORD dwStatus = WaitForSingleObject(hEvent, 10);

        if (dwStatus == WAIT_OBJECT_0) {
            break;
        }

        WaitForSingleObject(hMutex, INFINITE);
        pQElement = pQueue->Dequeue();
        ReleaseMutex(hMutex);

        if (pQElement == NULL) {
            break;
        }

        char szBuffer[1024];
        double dDeterminant = (pQElement->iA1 * pQElement->iB2) - (pQElement->iB1 * pQElement->iA2);

        if (dDeterminant != 0) {
            double dX = ((pQElement->iC1 * pQElement->iB2) - (pQElement->iB1 * pQElement->iC2)) / dDeterminant;
            double dY = ((pQElement->iA1 * pQElement->iC2) - (pQElement->iC1 * pQElement->iA2)) / dDeterminant;
            sprintf_s(szBuffer, "  x = %8.4lf,\ty = %8.4lf\n", dX, dY);
        } else {
            sprintf_s(szBuffer, "  Determinant is zero.\n");
        }

        strcat_s(szResult, 4096, szBuffer);
        SetWindowTextA(pQElement->hWndResult, szResult);
        SendMessage(pQElement->hWndProgress, PBM_STEPIT, 0, 0);
        delete pQElement;
        Sleep(1000);
    }

    CloseHandle(hMutex);
    CloseHandle(hEvent);
    return 0L;
}

BOOL FileDialog(HWND hWnd, LPSTR szFileName)
{
    OPENFILENAMEA ofn;
    char szFile[MAX_PATH];
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameA(&ofn) == TRUE) {
        strcpy_s(szFileName, MAX_PATH - 1, szFile);
        return TRUE;
    }

    return FALSE;
}
