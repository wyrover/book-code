// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <WtsApi32.h>

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"WtsApi32.lib")

char *cs(char *str1, char *str2) //connect string
{
    long newstrlen = strlen(str1) + strlen(str2) + 1;
    char *newstr = (char*)malloc(newstrlen);
    memcpy(newstr, str1, strlen(str1));
    memcpy(newstr + strlen(str1), str2, strlen(str2) + 1);
    return newstr;
}

long InStrRev(long startpos, char *str1, char *str2, long txtcmp) //vbBinraryCompare=0;vbTextCompare=1
{
    char *str1new = (char*)malloc(strlen(str1) + 1);
    memset(str1new, 0x0, strlen(str1) + 1);
    strcpy(str1new, str1);
    str1new = strrev(str1new);
    char *str2new = (char*)malloc(strlen(str2) + 1);
    memset(str2new, 0x0, strlen(str2) + 1);
    strcpy(str2new, str2);
    char *posfound = NULL;

    if (txtcmp == 1) {
        str1new = strlwr(str1new);
        str2new = strlwr(str2new);
    }

    posfound = strstr(str1new + startpos - 1, str2new); //hello,world! <-> llo

    if (posfound == NULL)
        return 0;

    return (long)posfound - (long)str1new + 1;
}

char *Right(char *string, long length)
{
    long dwstrlen = strlen(string);
    long dwstartpos = 0;
    char *newstr;

    if (length > dwstrlen)
        length = dwstrlen;

    dwstartpos = dwstrlen - length;
    newstr = (char*)malloc(length + 1);
    memcpy(newstr, string + dwstartpos, length + 1);
    return newstr;
}

char *GetCurrentProcessName()
{
    char sz[260] = {0};
    char *pn;
    GetModuleFileNameA(0, sz, 260);
    long pos = InStrRev(0, sz, "\\", 1);
    return Right(sz, pos - 1);
}

char *LongToString(long lng)
{
    char *buf = (char*)malloc(8);
    itoa(lng, buf, 10);
    return buf;
}

VOID OutputData()
{
    char *DirPath = NULL;
    DirPath = cs("c:\\[", LongToString(GetCurrentProcessId()));
    DirPath = cs(DirPath, "]");
    DirPath = cs(DirPath, GetCurrentProcessName());
    CreateDirectory(DirPath, NULL);
}

void ShowMessage(LPSTR lpszMessage, LPSTR lpszTitle)
{
    // 获得当前Session ID
    DWORD dwSession = WTSGetActiveConsoleSessionId();
    DWORD dwResponse = 0;
    // 显示消息对话框
    WTSSendMessageA(WTS_CURRENT_SERVER_HANDLE, dwSession,
                    lpszTitle,
                    static_cast<DWORD>((strlen(lpszTitle) + 1) * sizeof(char)),
                    lpszMessage,
                    static_cast<DWORD>((strlen(lpszMessage) + 1) * sizeof(char)),
                    0, 0, &dwResponse, FALSE);
}

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved
                     )
{
    char xxoo[260] = {0};

    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH: {
        OutputData();
        char num[8] = {0};
        char sz[260] = {0};
        GetModuleFileNameA(0, sz, 260);
        itoa(GetCurrentProcessId(), num, 10);
        ShowMessage(sz, cs("PID: ", num));
        break;
    }

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }

    return TRUE;
}