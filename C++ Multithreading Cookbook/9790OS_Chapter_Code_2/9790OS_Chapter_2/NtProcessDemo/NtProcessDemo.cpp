#include "stdafx.h"
#include <Windows.h>
#include <Winternl.h>
#include <iostream>

using namespace std;

typedef NTSTATUS(NTAPI* QEURYINFORMATIONPROCESS)(
    IN      HANDLE ProcessHandle,
    IN      PROCESSINFOCLASS ProcessInformationClass,
    OUT PVOID ProcessInformation,
    IN      ULONG ProcessInformationLength,
    OUT PULONG ReturnLength OPTIONAL
);

int _tmain(int argc, _TCHAR* argv[])
{
    STARTUPINFO startupInfo = { 0 };
    PROCESS_INFORMATION processInformation = { 0 };
    BOOL bSuccess = CreateProcess(
                        TEXT("C:\\Windows\\notepad.exe"), NULL, NULL,
                        NULL, FALSE, NULL, NULL, NULL, &startupInfo,
                        &processInformation);

    if (bSuccess) {
        cout << "Process started." << endl << "Process ID:\t"
             << processInformation.dwProcessId << endl;
        PROCESS_BASIC_INFORMATION pbi;
        ULONG uLength = 0;
        HMODULE hDll = LoadLibrary(
                           TEXT("C:\\Windows\\System32\\ntdll.dll"));

        if (hDll) {
            QEURYINFORMATIONPROCESS QueryInformationProcess =
                (QEURYINFORMATIONPROCESS)GetProcAddress(
                    hDll, "NtQueryInformationProcess");

            if (QueryInformationProcess) {
                NTSTATUS ntStatus = QueryInformationProcess(
                                        processInformation.hProcess,
                                        PROCESSINFOCLASS::ProcessBasicInformation,
                                        &pbi, sizeof(pbi), &uLength);

                if (NT_SUCCESS(ntStatus)) {
                    cout << "Process ID (from PCB):\t"
                         << pbi.UniqueProcessId << endl;
                } else {
                    cout << "Cannot open PCB!" << endl
                         << "Error code:\t" << GetLastError()
                         << endl;
                }
            } else {
                cout << "Cannot get "
                     << "NtQueryInformationProcess function!"
                     << endl << "Error code:\t"
                     << GetLastError() << endl;
            }

            FreeLibrary(hDll);
        } else {
            cout << "Cannot load ntdll.dll!" << endl
                 << "Error code:\t" << GetLastError() << endl;
        }
    } else {
        cout << "Cannot start process!" << endl
             << "Error code:\t" << GetLastError() << endl;
    }

    return 0;
}
