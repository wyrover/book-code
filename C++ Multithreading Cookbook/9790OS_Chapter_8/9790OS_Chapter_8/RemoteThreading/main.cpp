#include <windows.h>
#include <tchar.h>
#include "..\common.h"

int __stdcall RemoteLoadLibrary(HANDLE hProcess, char* szLibraryName)
{
    LPTHREAD_START_ROUTINE lpLoadLibrary = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("Kernel32.dll"), "LoadLibraryA");

    if (lpLoadLibrary == NULL) {
        return -1;
    }

    if (hProcess == NULL) {
        _tprintf_s(_T("Handle to Process 0x%x was NULL!\n"), (int)hProcess);
        return -1;
    }

    size_t uMemSize = strlen(szLibraryName) + 1;
    void* lpRemoteMem = VirtualAllocEx(hProcess, NULL, uMemSize, MEM_COMMIT, PAGE_READWRITE);

    if (lpRemoteMem == NULL) {
        _tprintf_s(_T("Could not allocate remote virtual memory!\nError:\t%u\n"), GetLastError());
        return -1;
    }

    BOOL bSuccess = WriteProcessMemory(hProcess, lpRemoteMem, szLibraryName, uMemSize, NULL);

    if (bSuccess == STATUS_ACCESS_VIOLATION || bSuccess == FALSE) {
        _tprintf_s(_T("Could not write remote virtual memory!\nError:\t%u\n"), GetLastError());
        VirtualFreeEx(hProcess, lpRemoteMem, 0, MEM_RELEASE);
        return -1;
    }

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, lpLoadLibrary, lpRemoteMem, 0, NULL);

    if (hThread == NULL) {
        _tprintf_s(_T("Could not create remote thread!\nError:\t%u\n"), GetLastError());
        VirtualFreeEx(hProcess, lpRemoteMem, 0, MEM_RELEASE);
        return -1;
    }

    WaitForSingleObject(hThread, INFINITE);
    VirtualFreeEx(hProcess, lpRemoteMem, 0, MEM_RELEASE);
    CloseHandle(hThread);
    return 0;
}

int __stdcall RemoteFreeLibrary(HANDLE hProcess, HMODULE hLib)
{
    LPTHREAD_START_ROUTINE lpLoadLibrary = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("Kernel32.dll"), "FreeLibrary");

    if (lpLoadLibrary == NULL) {
        return -1;
    }

    if (hProcess == NULL) {
        _tprintf_s(_T("Handle to Process 0x%x was NULL!\n"), (int)hProcess);
        return -1;
    }

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, lpLoadLibrary, hLib, 0, NULL);

    if (hThread == NULL) {
        _tprintf_s(_T("Could not create remote thread!\nError:\t%u\n"), GetLastError());
        return -1;
    }

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    return 0;
}

int main(void)
{
    char* szLibrary = PHYSICAL_PATH_TO_YOUR_DLL;
    HANDLE hMapping = CreateFileMapping((HANDLE) - 1, NULL, PAGE_READWRITE, 0, sizeof(COMM_OBJECT), MAPPING_NAME);
    HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, EVENT_NAME);
    STARTUPINFO startInfo = { 0 };
    PROCESS_INFORMATION processInfo = { 0 };
    BOOL bSuccess = CreateProcess(_T("C:\\Windows\\System32\\calc.exe"), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &startInfo, &processInfo);

    if (!bSuccess) {
        _tprintf_s(_T("Error:\t%u\n"), GetLastError());
    } else {
        WaitForSingleObject(processInfo.hThread, 500);
        RemoteLoadLibrary(processInfo.hProcess, szLibrary);
        WaitForSingleObject(hEvent, INFINITE);
        PCOMM_OBJECT pCommObject = (PCOMM_OBJECT)MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);

        if (pCommObject) {
            RemoteFreeLibrary(processInfo.hProcess, pCommObject->hLibrary);
        }
    }

    CloseHandle(hEvent);
    CloseHandle(hMapping);
#ifdef _DEBUG
    return system("pause");
#endif
    return 0;
}
