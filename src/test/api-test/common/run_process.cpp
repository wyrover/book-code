#include "run_process.h"
#include "environment_variable.h"
#include "string.h"
#include <sstream>
#include <ShellAPI.h>

BOOL GetCommandOutput(LPCSTR lpszCmd, std::string& result)
{
    SECURITY_ATTRIBUTES sa = { 0 };
    STARTUPINFO         si = { 0 };
    PROCESS_INFORMATION pi = { 0 };
    HANDLE              hPipeOutputRead = NULL;
    HANDLE              hPipeOutputWrite = NULL;
    HANDLE              hPipeInputRead = NULL;
    HANDLE              hPipeInputWrite = NULL;
    BOOL bTest = 0;
    DWORD               dwNumberOfBytesRead = 0;
    DWORD               dwNumberOfBytesWrite = 0;
    CHAR                szBuffer[256] = {0};
    DWORD    dwLastError;
    BOOL     bSuccess;
    //std::ostringstream    oss;
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;
    // Create pipe for standard output redirection.
    CreatePipe(&hPipeOutputRead, &hPipeOutputWrite, &sa, 0);
    // Create pipe for standard input redirection.
    CreatePipe(&hPipeInputRead, &hPipeInputWrite, &sa, 0);
    // Make child process use hPipeOutputWrite as standard out,
    // and make sure it does not show on screen.
    si.cb = sizeof(si);
    GetStartupInfo(&si);
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    si.wShowWindow = SW_HIDE;
    si.hStdInput = hPipeInputRead;
    si.hStdOutput = hPipeOutputWrite;
    si.hStdError = hPipeOutputWrite;
    TCHAR szParam[100] = { 0 };
    _tcscpy(szParam, _T("cmd.exe /K"));
    // CREATE_NEW_CONSOLE
    bSuccess = CreateProcess(NULL, szParam, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);

    if (!bSuccess) {
        dwLastError = GetLastError();
    }

    // Now that handles have been inherited, close it to be safe.
    // You don't want to read or write to them accidentally.
    CloseHandle(hPipeOutputWrite);
    CloseHandle(hPipeInputRead);
    // Now test to capture DOS application output by reading
    // hPipeOutputRead.  Could also write to DOS application
    // standard input by writing to hPipeInputWrite.
    //LPCSTR lpszCmd = ("fsutil behavior QUERY DisableDeleteNotify\nexit\n");
    bSuccess = WriteFile(
                   hPipeInputWrite,      // handle of the write end of our pipe
                   lpszCmd,           // address of buffer that send data
                   (DWORD)strlen(lpszCmd) * sizeof(CHAR),           // number of bytes to write
                   &dwNumberOfBytesWrite,// address of number of bytes read
                   NULL                  // non-overlapped.
               );

    if (!bSuccess) {
        dwLastError = GetLastError();
    }

    while (TRUE) {
        DWORD dwExitCode, dwTotalBytesAvail;
        GetExitCodeProcess(pi.hProcess, &dwExitCode);
        PeekNamedPipe(hPipeOutputRead, NULL, 0, NULL, &dwTotalBytesAvail, NULL);

        if (dwTotalBytesAvail == 0 && dwExitCode != STILL_ACTIVE)
            break;

        bTest = ReadFile(hPipeOutputRead, &szBuffer, 256, &dwNumberOfBytesRead, NULL);

        if (!bTest) {
            CHAR szMsg[256];
            sprintf(szMsg, "Error #%d reading pipe.", GetLastError());
            //::MessageBox(hwnd, szMsg, pszCommand, MB_OK);
            break;
        }

        szBuffer[dwNumberOfBytesRead] = 0;
        //printf(szBuffer);
        result.append(szBuffer);
    }

    // Wait for CONSPAWN to finish.
    // WaitForSingleObject(pi.hProcess, 1000); //INFINITE
    WaitForSingleObject(pi.hProcess, INFINITE);
    // Close all remaining handles
    CloseHandle(pi.hProcess);
    CloseHandle(hPipeOutputRead);
    CloseHandle(hPipeInputWrite);
    return bSuccess;
}


void RunCmd(const std::wstring& cmd, LPPROCESS_INFORMATION lpi, int flags, const std::wstring& working_directory, int nShow)
{
    // expand command line, using ShellExecuteEx API function with setting the flag
    // SEE_MASK_DOENVSUBST does not work because environment variables can also be
    // placed in the parameters for the new process
    STARTUPINFO si = { 0 };
    si.cb = sizeof(si);
    DWORD startupInfoFlags = STARTF_USESHOWWINDOW;

    if (nShow == SW_HIDE) {
        startupInfoFlags |= CREATE_NO_WINDOW;
    }

    si.dwFlags = startupInfoFlags;
    si.wShowWindow = nShow;
    PROCESS_INFORMATION pi = { 0 };
    std::wstring cmd_expanded = ExpandEnvironmentVariables(cmd);
    // set the current directory if it was specified
    LPCWSTR lpCurrentDirectory = NULL;

    if (working_directory != L"") {
        lpCurrentDirectory = working_directory.c_str();
    }

    ::CreateProcessW(NULL, & * cmd_expanded.begin(), NULL, NULL, FALSE, flags, NULL, lpCurrentDirectory, & si, lpi == NULL ? & pi : lpi);

    if (lpi == NULL) {
        ::CloseHandle(pi.hThread);
        ::CloseHandle(pi.hProcess);
    }
}

DWORD ExecCmd(const std::wstring& cmd, const std::wstring& working_directory, int nShow)
{
    PROCESS_INFORMATION pi = { 0 };
    RunCmd(cmd, & pi, 0, working_directory, nShow);
    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD dwExitCode = 0;
    ::GetExitCodeProcess(pi.hProcess, & dwExitCode);

    if (pi.hThread)
        ::CloseHandle(pi.hThread);

    if (pi.hProcess)
        ::CloseHandle(pi.hProcess);

    return dwExitCode;
}


void DetachCmd(const std::wstring& cmd, LPPROCESS_INFORMATION lpi)
{
    RunCmd(cmd, lpi, DETACHED_PROCESS);
}


void ShellCmd(const std::wstring& cmd, int * rc, LPHANDLE lpProcessHandle, HWND hWnd, const std::wstring& working_directory, int nShow)
{
    std::wstring cmd_expanded = ExpandEnvironmentVariables(cmd);
    //CHECK_BOOL(! cmd_expanded.empty(), L"Missing command");
    // split arguments
    std::vector<std::wstring> cmd_args;
    cmd_args = cactus::split(cmd_expanded, (cmd_expanded[0] == L'\"') ? L"\" " : L" ", 2);
    std::wstring cmd_file = cactus::trimleft(cmd_args[0], L"\"");
    SHELLEXECUTEINFO sei = { 0 };
    sei.cbSize = sizeof(sei);
    sei.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_UNICODE;
    sei.hwnd = hWnd;
    // set the current directory if it was specified
    LPCWSTR lpCurrentDirectory = NULL;

    if (working_directory != L"") {
        lpCurrentDirectory = working_directory.c_str();
    }

    sei.lpDirectory = lpCurrentDirectory;
    sei.lpFile = cmd_file.c_str();
    sei.lpParameters = cmd_args.size() == 2 ? cmd_args[1].c_str() : NULL;
    sei.nShow = nShow;
    ::ShellExecuteExW(&sei);

    if (NULL != rc) {
        * rc = reinterpret_cast<int>(sei.hInstApp);
    }

    if (NULL != lpProcessHandle) {
        * lpProcessHandle = sei.hProcess;
    }
}