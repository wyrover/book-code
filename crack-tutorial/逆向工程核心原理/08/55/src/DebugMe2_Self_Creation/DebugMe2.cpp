#include <windows.h>
#include <tchar.h>
#include <stdio.h>


void ChildProc()
{
    MessageBox(NULL, L"This is a child process!", L"DebugMe2", MB_OK);

    ExitProcess(0);
}


void _tmain(int argc, TCHAR *argv[]) 
{
    TCHAR                   szPath[MAX_PATH] = {0,};
    STARTUPINFO				si = {sizeof(STARTUPINFO),};
    PROCESS_INFORMATION		pi = {0,};
    CONTEXT                 ctx = {0,};

    _tprintf(L"This is a parent process!\n");

    if( !GetModuleFileName(NULL, szPath, sizeof(TCHAR) * MAX_PATH) )
    {
        printf("GetModuleFileName() failed! [%d]\n", GetLastError());
        return;
    }

    // Create Child Process
    if( !CreateProcess(
            szPath,
            NULL,
            NULL,
            NULL,
            FALSE,
            CREATE_SUSPENDED,
            NULL,
            NULL,
            &si,
            &pi) )
    {
        printf("CreateProcess() failed! [%d]\n", GetLastError());
        return;
    }

    // Change EIP
    ctx.ContextFlags = CONTEXT_FULL;
    if( !GetThreadContext(pi.hThread, &ctx) )
    {
        printf("GetThreadContext() failed! [%d]\n", GetLastError());
        return;
    }

    ctx.Eip = (DWORD)ChildProc;

    if( !SetThreadContext(pi.hThread, &ctx) )
    {
        printf("SetThreadContext() failed! [%d]\n", GetLastError());
        return;
    }

    // Resume Main Thread
    if( -1 == ResumeThread(pi.hThread) )
    {
        printf("ResumeThread() failed! [%d]\n", GetLastError());
        return;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}
