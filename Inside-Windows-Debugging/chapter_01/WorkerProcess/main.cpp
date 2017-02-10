#include "stdafx.h"

class CMainApp
{
public:
    static
    HRESULT
    MainHR(
        VOID
        )
    {
        HANDLE hProcess, hPrimaryThread;
        CHandle shProcess, shPrimaryThread;
        CHandle shWorkerJob;
        DWORD dwExitCode;
        JOBOBJECT_EXTENDED_LIMIT_INFORMATION exLimitInfo = {0};
        CStringW shCommandLine = L"notepad.exe";

        ChkProlog();

        //
        // Create the job object, set its processes to terminate on
        // handle close (similar to an explicit call to TerminateJobObject),
        // and then add the current process to the job.
        //
        shWorkerJob.Attach(CreateJobObject(NULL, NULL));
        ChkWin32(shWorkerJob);

        exLimitInfo.BasicLimitInformation.LimitFlags =
            JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;

        ChkWin32(SetInformationJobObject(
            shWorkerJob,
            JobObjectExtendedLimitInformation,
            &exLimitInfo,
            sizeof(exLimitInfo)
            ));

        ChkWin32(AssignProcessToJobObject(
            shWorkerJob,
            ::GetCurrentProcess()
            ));

        //
        // Now launch the new child process (job membership is inherited by default)
        //
        wprintf(L"Launching child process (notepad.exe) ...\n");
        ChkHr(LaunchProcess(
            shCommandLine.GetBuffer(),
            0,
            &hProcess,
            &hPrimaryThread
            ));
        shProcess.Attach(hProcess);
        shPrimaryThread.Attach(hPrimaryThread);

        //
        // Wait for the worker process to exit
        //
        switch (WaitForSingleObject(shProcess, INFINITE))
        {
            case WAIT_OBJECT_0:
                ChkWin32(::GetExitCodeProcess(shProcess, &dwExitCode));
                wprintf(L"Child process exited with exit code %d.\n", dwExitCode);
                break;

            default:
                ChkReturn(E_FAIL);
        }

        ChkNoCleanup();
    }

private:
    static
    HRESULT
    LaunchProcess(
        __in LPWSTR pwszCommandLine,
        __in DWORD dwCreationFlags,
        __out HANDLE* phProcess,
        __out HANDLE* phPrimaryThread
        )
    {
        STARTUPINFO si = {0};
        PROCESS_INFORMATION pi = {0};

        ChkProlog();

        ASSERT(pwszCommandLine);
        ASSERT(phProcess);
        ASSERT(phPrimaryThread);

        si.cb = sizeof(si);
        ChkWin32(CreateProcess(
            NULL,
            pwszCommandLine,
            NULL,
            NULL,
            FALSE,
            dwCreationFlags,
            NULL,
            NULL,
            &si,
            &pi
            ));

        *phPrimaryThread = pi.hThread;
        *phProcess = pi.hProcess;

        ChkNoCleanup();
    }
};

int
__cdecl
wmain(
    VOID
    )
{
    HRESULT hr;

    hr = CMainApp::MainHR();
    if (FAILED(hr))
    {
        wprintf(L"HRESULT: 0x%08X\n", hr);
    }
    else
    {
        wprintf(L"Success.\n");
    }
    return 0;
}
