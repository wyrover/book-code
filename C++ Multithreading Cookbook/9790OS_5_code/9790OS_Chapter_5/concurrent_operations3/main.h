#pragma once

#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include <psapi.h>
#include <strsafe.h>
#include <string.h>
#include <sstream>
#include <fstream>
#include <commdlg.h>
#include "CQueue.h"

#pragma comment ( lib, "comctl32.lib" )
#pragma comment ( linker, "\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"" )

using namespace std;

#define CONTROL_BROWSE      100
#define CONTROL_START       101
#define CONTROL_RESULT      103
#define CONTROL_TEXT        104
#define CONTROL_PROGRESS    105

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT uMsg,
                                 WPARAM wParam, LPARAM lParam);
DWORD WINAPI StartAddress(LPVOID lpParameter);
BOOL FileDialog(HWND hWnd, LPSTR szFileName);

typedef struct {
    int iA1;
    int iB1;
    int iC1;
    int iA2;
    int iB2;
    int iC2;
    HWND hWndProgress;
    HWND hWndResult;
} QueueElement, *PQueueElement;
