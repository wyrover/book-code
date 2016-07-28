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
    void* params[2] = { hProgress, hResult };
    HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ListenerRoutine, params, 0, NULL);
    Sleep(100);
    CloseHandle(hThread);
    ShowWindow(hWnd, iWndShow);
    HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, EVENT_NAME);
    HANDLE hMutex = CreateMutex(NULL, FALSE, MUTEX_NAME);
    HANDLE hMaping = CreateFileMapping((HANDLE) - 1, NULL, PAGE_READWRITE, 0, sizeof(QueueElement), MAPPING_NAME);
    MSG msg;

    while (GetMessage(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    CloseHandle(hMaping);
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
        HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, EVENT_NAME);
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
                wsprintfA(szBuffer, "  File: %s Press \"Start\" now.", strrchr(szFilePath, '\\') + 1);
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
                int iCount = queue.Count();

                for (int iIndex = 0; iIndex < iCount; iIndex++) {
                    StartProcess(GetDlgItem(hWnd, CONTROL_TEXT));
                    Sleep(100);
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

DWORD WINAPI ListenerRoutine(LPVOID lpParameter)
{
    void** lpParameters = (void**)lpParameter;
    HWND hWndProgress = (HWND)lpParameters[0];
    HWND hWndResult = (HWND)lpParameters[1];
    HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, EVENT_NAME);

    while (TRUE) {
        DWORD dwStatus = WaitForSingleObject(hEvent, INFINITE);

        if (dwStatus == WAIT_OBJECT_0) {
            break;
        }

        HANDLE hPipe = CreateNamedPipe(PIPE_NAME, PIPE_ACCESS_DUPLEX, PIPE_WAIT | PIPE_READMODE_MESSAGE
                                       | PIPE_TYPE_MESSAGE, PIPE_UNLIMITED_INSTANCES, BUFFER_SIZE, BUFFER_SIZE, 0, NULL);

        if (hPipe == INVALID_HANDLE_VALUE) {
            char szBuffer[MAX_PATH];
            wsprintfA(szBuffer, "  Error: [%u]\n", GetLastError());
            SetWindowTextA(hWndResult, szBuffer);
            return 2L;
        }

        if (ConnectNamedPipe(hPipe, NULL)) {
            void* params[2] = { hPipe, hWndResult };
            HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartAddress, params, 0, NULL);
            Sleep(100);
            CloseHandle(hThread);
        }

        SendMessage(hWndProgress, PBM_STEPIT, 0, 0);
    }

    CloseHandle(hEvent);
    return 0L;
}

DWORD WINAPI StartAddress(LPVOID lpParameter)
{
    void** lpParameters = (void**)lpParameter;
    HANDLE hPipe = (HANDLE)lpParameters[0];
    HWND hWndResult = (HWND)lpParameters[1];
    char* szRequest = (char*)HeapAlloc(GetProcessHeap(), 0, BUFFER_SIZE * sizeof(char));
    char* szReply = (char*)HeapAlloc(GetProcessHeap(), 0, BUFFER_SIZE * sizeof(char));
    DWORD dwBytesRead = 0;
    DWORD dwReplyBytes = 0;
    DWORD dwBytesWritten = 0;
    memset(szRequest, 0, BUFFER_SIZE * sizeof(char));
    memset(szReply, 0, BUFFER_SIZE * sizeof(char));

    if (!ReadFile(hPipe, szRequest, BUFFER_SIZE * sizeof(char), &dwBytesRead, NULL)) {
        return 2L;
    }

    char szBuffer[1024] = { 0 };
    wsprintfA(szBuffer, "  PID: [%s] connected.\n", szRequest);
    strcat_s(szResult, szBuffer);
    SetWindowTextA(hWndResult, szResult);
    HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, MUTEX_NAME);
    WaitForSingleObject(hMutex, INFINITE);
    QueueElement* pQElement = (QueueElement*)queue.Dequeue();
    HANDLE hMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, MAPPING_NAME);
    QueueElement* pMapping = (QueueElement*)MapViewOfFile(hMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    memcpy(pMapping, pQElement, sizeof(QueueElement));
    UnmapViewOfFile(pMapping);
    CloseHandle(hMapping);
    ReleaseMutex(hMutex);
    CloseHandle(hMutex);
    sprintf_s(szReply, BUFFER_SIZE * sizeof(char), "OK%s", szRequest);
    dwReplyBytes = (DWORD)((strlen(szReply) + 1) * sizeof(char));

    if (!WriteFile(hPipe, szReply, dwReplyBytes, &dwBytesWritten, NULL)) {
        return 2L;
    }

    memset(szRequest, 0, BUFFER_SIZE * sizeof(char));

    if (!ReadFile(hPipe, szRequest, BUFFER_SIZE * sizeof(char), &dwBytesRead, NULL)) {
        return 2L;
    }

    wsprintfA(szBuffer, "%s", szRequest);
    strcat_s(szResult, szBuffer);
    SetWindowTextA(hWndResult, szResult);
    delete pQElement;
    FlushFileBuffers(hPipe);
    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);
    HeapFree(GetProcessHeap(), 0, szRequest);
    HeapFree(GetProcessHeap(), 0, szReply);
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

BOOL StartProcess(HWND hWndResult)
{
    STARTUPINFO startupInfo = { 0 };
    PROCESS_INFORMATION processInformation = { 0 };
    BOOL bSuccess = CreateProcess(_T("..\\x64\\Debug\\ClientProcess.exe"), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInformation);

    if (!bSuccess) {
        char szBuffer[MAX_PATH];
        wsprintfA(szBuffer, "  Process creation fails: [%u]\n", GetLastError());
        SetWindowTextA(hWndResult, szBuffer);
    }

    return bSuccess;
}
