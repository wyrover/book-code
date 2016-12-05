#include "windows.h"
#include "stdio.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

TCHAR szAppName[] = L"TextView" ;
TCHAR szFile[MAX_PATH] = {0,};
TCHAR szMsg[2048] = {0,};

#define MAX_BUF_SIZE (32768)

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    HWND       hwnd ;
    MSG        msg ;
    WNDCLASSEX wndclass ;

    wndclass.cbSize        = sizeof(wndclass);
    wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
    wndclass.lpfnWndProc   = WndProc ;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.hInstance     = hInstance ;
    wndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName  = NULL ;
    wndclass.lpszClassName = szAppName ;
    wndclass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassEx(&wndclass);

    hwnd = CreateWindow(
                szAppName, szAppName,
                WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT, CW_USEDEFAULT,
                CW_USEDEFAULT, CW_USEDEFAULT,
                NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd); 

    while( GetMessage(&msg, NULL, 0, 0) )
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam ;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND hwndEdit ;
    HFONT hFont;

    switch(iMsg)
    {
    case WM_CREATE :
        hwndEdit = CreateWindow(L"Edit", NULL,
                        WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL |
                        WS_BORDER | ES_LEFT | ES_MULTILINE |
                        ES_AUTOHSCROLL | ES_AUTOVSCROLL,
                        0, 0, 0, 0,
                        hwnd,(HMENU) 1,
                       ((LPCREATESTRUCT)lParam)->hInstance, NULL);

        hFont=CreateFont(16,0,0,0,0,0,0,0,0,0,0,0,0,L"Courier New");
		SendMessage(hwndEdit, WM_SETFONT, (WPARAM)hFont, (LPARAM)FALSE);

        DragAcceptFiles(hwnd, TRUE);

        return 0;

    case WM_DROPFILES :
        if( DragQueryFile((HDROP)wParam, 0, szFile, MAX_PATH) )
        {
            HANDLE hFile = CreateFile(szFile, GENERIC_READ, FILE_SHARE_READ, 
                                NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            if( hFile == INVALID_HANDLE_VALUE )
            {
                wsprintf(szMsg, L"file(\"%s\") open error!!! [%d]\n", szFile, GetLastError());
                MessageBox(hwndEdit, szMsg, szAppName, MB_OK);
                return 0;
            }

            DWORD dwBytesRead = 0;
            char *pBuf = new char[MAX_BUF_SIZE];
            ZeroMemory(pBuf, MAX_BUF_SIZE);

            ReadFile(hFile, pBuf, MAX_BUF_SIZE, &dwBytesRead, NULL);

            SetWindowTextA(hwndEdit, pBuf);

            wsprintf(szMsg, L"TextView (%s)", szFile);
            SetWindowText(hwnd, szMsg);

            delete []pBuf;

            CloseHandle(hFile);
        }

        return 0;

    case WM_SETFOCUS :
        SetFocus(hwndEdit);
        return 0;

    case WM_SIZE : 
        MoveWindow(hwndEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
        return 0;

    case WM_DESTROY :
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
