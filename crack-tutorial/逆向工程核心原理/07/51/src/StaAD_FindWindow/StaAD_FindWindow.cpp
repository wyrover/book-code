#include "stdio.h"
#include "windows.h"
#include "tchar.h"

void FindDebuggerWindow()
{
    BOOL bDebugging = FALSE;

    // using ClassName
    if( FindWindow(L"OllyDbg", NULL) ||                  // OllyDbg
        FindWindow(L"TIdaWindow", NULL) ||               // IDA Pro
        FindWindow(L"WinDbgFrameClass", NULL) )          // Windbg
        bDebugging = TRUE;

    printf("FindWindow()\n");
    if( bDebugging )    printf("  => Found a debugger window!!!\n\n");
    else                printf("  => Not found a debugger window...\n\n");

    // using WindowName
    bDebugging = FALSE;
    TCHAR szWindow[MAX_PATH] = {0,};

    HWND hWnd = GetDesktopWindow();
    hWnd = GetWindow(hWnd, GW_CHILD);
    hWnd = GetWindow(hWnd, GW_HWNDFIRST);
    while( hWnd )
    {
        if( GetWindowText(hWnd, szWindow, MAX_PATH) )
        {
            if( _tcsstr(szWindow, L"IDA") ||
                _tcsstr(szWindow, L"OllyDbg") ||
                _tcsstr(szWindow, L"WinDbg") )
            {
                bDebugging = TRUE;
                break;
            }
        }

        hWnd = GetWindow(hWnd, GW_HWNDNEXT);
    }

    printf("GetWindowText()\n");
    if( bDebugging )    printf("  => Found a debugger window!!!\n\n");
    else                printf("  => Not found a debugger window...\n\n");
}

int _tmain(int argc, TCHAR* argv[])
{
    FindDebuggerWindow();

    printf("\npress any key to quit...\n");
    _gettch();

    return 0;
}