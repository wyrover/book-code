#include <windows.h>
#include <tchar.h>
#include <stdio.h>

int main(void)
{
    STARTUPINFO	pStartInfo = {0};
    PROCESS_INFORMATION	pProcInfo = {0};
    pStartInfo.cb = sizeof(STARTUPINFO);

    if (!CreateProcess(_T("c:\\windows\\system32\\notepad.exe")
        ,_T("")
        ,NULL,NULL,FALSE,CREATE_SUSPENDED |DEBUG_ONLY_THIS_PROCESS,NULL,
        _T("c:\\windows\\system32\\"),&pStartInfo,&pProcInfo))
    {
        return -1;
    }

    const void* lpLdrInitializeThunk = (const void*)GetProcAddress(
                GetModuleHandle(_T("NTDLL")),
                "LdrInitializeThunk");
    //双int 3因为LdrInitializeThunk函数为mov edi,edi
    //和某些调试器的步过
    BYTE loopBytes[] = {0xCC,0xCC};
    SIZE_T szWriten = 0;
    WriteProcessMemory(pProcInfo.hProcess,
                       (LPVOID)lpLdrInitializeThunk,
                       loopBytes,sizeof(loopBytes),&szWriten);
    DebugActiveProcessStop(pProcInfo.dwProcessId);
    return 0;
}

