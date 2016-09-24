#include <windows.h>
#include <tchar.h>
#include <stdio.h>

int main(void)
{
    STARTUPINFO	pStartInfo = {0};
    PROCESS_INFORMATION	pProcInfo = {0};
    pStartInfo.cb = sizeof(STARTUPINFO);

    //调用中断函数，直接定位到启动函数位置
    DebugBreak();
    if (!CreateProcess(_T("c:\\windows\\system32\\notepad.exe")
        ,_T("")
        ,NULL,NULL,FALSE,CREATE_SUSPENDED,NULL,
        _T("c:\\windows\\system32\\"),&pStartInfo,&pProcInfo))
    {
        return -1;
    }
    return 0;
}

