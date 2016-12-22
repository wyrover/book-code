#include "stdio.h"
#include "windows.h"
#include "shlobj.h"
#include "Wininet.h"
#include "tchar.h"

#pragma comment(lib, "Wininet.lib")

#define DEF_BUF_SIZE            (4096)
#define DEF_URL                 L"http://www.google.com/index.html"
#define DEF_INDEX_FILE          L"index.html"

HWND g_hWnd = NULL;

#ifdef __cplusplus
extern "C" {
#endif
// IDT 형식을 위한 dummy export function...
__declspec(dllexport) void dummy()
{
    return;
}
#ifdef __cplusplus
}
#endif

BOOL DownloadURL(LPCTSTR szURL, LPCTSTR szFile)
{
    BOOL            bRet = FALSE;
    HINTERNET	    hInternet = NULL, hURL = NULL;
    BYTE            pBuf[DEF_BUF_SIZE] = {0,};
    DWORD           dwBytesRead = 0;
    FILE            *pFile = NULL;
    errno_t         err = 0;

    hInternet = InternetOpen(L"ReverseCore", 
                             INTERNET_OPEN_TYPE_PRECONFIG, 
                             NULL, 
                             NULL, 
                             0);
    if( NULL == hInternet )
    {
        OutputDebugString(L"InternetOpen() failed!");
        return FALSE;
    }

    hURL = InternetOpenUrl(hInternet,
                           szURL,
                           NULL,
                           0,
                           INTERNET_FLAG_RELOAD,
                           0);
    if( NULL == hURL )
    {
        OutputDebugString(L"InternetOpenUrl() failed!");
        goto _DownloadURL_EXIT;
    }

    if( err = _tfopen_s(&pFile, szFile, L"wt") )
    {
        OutputDebugString(L"fopen() failed!");
        goto _DownloadURL_EXIT;
    }

    while( InternetReadFile(hURL, pBuf, DEF_BUF_SIZE, &dwBytesRead) )
    {
        if( !dwBytesRead )
            break;

        fwrite(pBuf, dwBytesRead, 1, pFile);
    }

    bRet = TRUE;

_DownloadURL_EXIT:
    if( pFile )
        fclose(pFile);

    if( hURL )
        InternetCloseHandle(hURL);

    if( hInternet )
        InternetCloseHandle(hInternet);

    return bRet;
}

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
    DWORD dwPID = 0;

    GetWindowThreadProcessId(hWnd, &dwPID);

    if( dwPID == (DWORD)lParam )
    {
        g_hWnd = hWnd;
        return FALSE;
    }

    return TRUE;
}

HWND GetWindowHandleFromPID(DWORD dwPID)
{
    EnumWindows(EnumWindowsProc, dwPID);

    return g_hWnd;
}

BOOL DropFile(LPCTSTR wcsFile)
{
    HWND            hWnd = NULL;
    DWORD           dwBufSize = 0;
    BYTE            *pBuf = NULL; 
	DROPFILES		*pDrop = NULL;
    char            szFile[MAX_PATH] = {0,};
    HANDLE          hMem = 0;

    WideCharToMultiByte(CP_ACP, 0, wcsFile, -1,
                        szFile, MAX_PATH, NULL, NULL);

    dwBufSize = sizeof(DROPFILES) + strlen(szFile) + 1;
    
    if( !(hMem = GlobalAlloc(GMEM_ZEROINIT, dwBufSize)) )
    {
        OutputDebugString(L"GlobalAlloc() failed!!!");
        return FALSE;
    }

    pBuf = (LPBYTE)GlobalLock(hMem);

    pDrop = (DROPFILES*)pBuf; 
    pDrop->pFiles = sizeof(DROPFILES);
    strcpy_s((char*)(pBuf + sizeof(DROPFILES)), strlen(szFile)+1, szFile);

    GlobalUnlock(hMem);

    if( !(hWnd = GetWindowHandleFromPID(GetCurrentProcessId())) )
    {
        OutputDebugString(L"GetWndHandleFromPID() failed!!!");
        return FALSE;
    }

    PostMessage(hWnd, WM_DROPFILES, (WPARAM)pBuf, NULL);

    return TRUE;
}

DWORD WINAPI ThreadProc(LPVOID lParam)
{
    TCHAR szPath[MAX_PATH] = {0,};
    TCHAR *p = NULL;

    OutputDebugString(L"ThreadProc() start...");

    GetModuleFileName(NULL, szPath, sizeof(szPath));
    
    if( p = _tcsrchr(szPath, L'\\') )
    {
        _tcscpy_s(p+1, wcslen(DEF_INDEX_FILE)+1, DEF_INDEX_FILE);

        OutputDebugString(L"DownloadURL()");
        if( DownloadURL(DEF_URL, szPath) )
        {
            OutputDebugString(L"DropFlie()");
            DropFile(szPath);
        }
    }

    OutputDebugString(L"ThreadProc() end...");

    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch( fdwReason )
    {
        case DLL_PROCESS_ATTACH : 
            CloseHandle(CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL));
            break;
    }
   
    return TRUE;
}
