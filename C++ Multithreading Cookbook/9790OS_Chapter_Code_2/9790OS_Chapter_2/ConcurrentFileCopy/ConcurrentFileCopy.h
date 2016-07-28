#pragma once

#include <windows.h>
#include <commctrl.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>

#pragma comment ( lib, "comctl32.lib" )
#pragma comment ( linker, "\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' \
				  version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"" )

ATOM RegisterWndClass(HINSTANCE hInstance);
HWND InitializeInstance(HINSTANCE hInstance, int nCmdShow, HWND& hWndPB);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
DWORD WINAPI ReadRoutine(LPVOID lpParameter);
DWORD WINAPI WriteRoutine(LPVOID lpParameter);
BOOL FileDialog(HWND hWnd, LPTSTR szFileName, DWORD dwFileOperation);
DWORD GetBlockSize(DWORD dwFileSize);

#define BUTTON_CLOSE    100
#define FILE_SAVE       0x0001
#define FILE_OPEN       0x0002
#define MUTEX_NAME      _T("__RW_MUTEX__")

typedef struct _tagCOPYDETAILS {
    HINSTANCE hInstance;
    HWND hWndPB;
    LPTSTR szReadFileName;
    LPTSTR szWriteFileName;
} COPYDETAILS, *PCOPYDETAILS;
