#include "stdafx.h"

#ifndef MSIHANDLE
typedef unsigned long MSIHANDLE;
#endif

#define LOG_DIRECTORY L"c:\\temp"

extern "C"
UINT
WINAPI
TestCustomAction(
    __in MSIHANDLE hInstall
    )
{
    CHandle shFile;
    DWORD dwAttribute;


    ChkProlog();

    dwAttribute = ::GetFileAttributes(LOG_DIRECTORY);
    if (dwAttribute == INVALID_FILE_ATTRIBUTES ||
        !(dwAttribute & FILE_ATTRIBUTE_DIRECTORY))
    {
        ChkWin32(::CreateDirectory(LOG_DIRECTORY, NULL));
        ChkWin32(INVALID_FILE_ATTRIBUTES !=
            ::GetFileAttributes(LOG_DIRECTORY));
    }

    shFile.Attach(CreateFile(
        LOG_DIRECTORY L"\\abc.txt",
        GENERIC_WRITE,
        FILE_SHARE_READ,
        NULL,
        CREATE_ALWAYS,
        0,
        NULL
        ));
    if (shFile == INVALID_HANDLE_VALUE)
    {
        shFile.Detach();
        shFile.Attach(NULL);
    }
    ChkWin32(shFile);

    ChkNoCleanupRet(SUCCEEDED(ChkGetHr()) ? EXIT_SUCCESS : EXIT_FAILURE);

    UNREFERENCED_PARAMETER(hInstall);
}

BOOL
WINAPI
DllMain(
    __in HINSTANCE hInstance,
    __in DWORD dwReason,
    __in PVOID pReserved
    )
{
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
            ::DisableThreadLibraryCalls(hInstance);

            ::HeapAlloc(::GetProcessHeap(), 0, 50000);
            ::CreateEvent(NULL, FALSE, FALSE, NULL);

            break;

        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
            ASSERT(FALSE);
            break;

        case DLL_PROCESS_DETACH:
            break;
    }

    return TRUE;

    UNREFERENCED_PARAMETER(pReserved);
}
