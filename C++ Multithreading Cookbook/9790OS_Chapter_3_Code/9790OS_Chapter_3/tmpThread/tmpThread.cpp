#include "stdafx.h"
#include <windows.h>
#include <iostream>

using namespace std;

DWORD WINAPI StartAddress(LPVOID lpParameter)
{
    cout << "Hello. I am a very simple thread."
         << endl
         << "I am used to demonstrate thread creation."
         << endl;
    return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // Thread creation. Pay attention that we only need
    // to define the StartAddress routine in the same code
    // segment as our main function to start thread and
    // perform concurrent operation.
    HANDLE hThread = CreateThread(NULL, 0, StartAddress, NULL, 0, NULL);
    // Process creation. Pay attention that first argument
    // of CreateProcess points to the file which content
    // needs to be loaded from the disk.
    STARTUPINFO startupInfo = { 0 };
    PROCESS_INFORMATION processInformation = { 0 };
#ifdef _DEBUG
    BOOL bSuccess = CreateProcess(TEXT("..//Debug//tmpProcess.exe"), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInformation);
#else
    BOOL bSuccess = CreateProcess(TEXT("..//Release//tmpProcess.exe"), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInformation);
#endif
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    return system("pause");
}
