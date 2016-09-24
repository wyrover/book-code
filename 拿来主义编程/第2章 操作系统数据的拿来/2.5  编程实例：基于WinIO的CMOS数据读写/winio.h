#ifndef WINIO_H
#define WINIO_H

#include "..\drv\winio_nt.h"

#ifndef WINIO_DLL
    #define WINIO_API _declspec(dllimport)
#else
    #define WINIO_API
#endif

extern "C"
{
    WINIO_API bool _stdcall InitializeWinIo();
    WINIO_API void _stdcall ShutdownWinIo();
    WINIO_API PBYTE _stdcall MapPhysToLin(tagPhysStruct &PhysStruct);
    WINIO_API bool _stdcall UnmapPhysicalMemory(tagPhysStruct &PhysStruct);
    WINIO_API bool _stdcall GetPhysLong(PBYTE pbPhysAddr, PDWORD pdwPhysVal);
    WINIO_API bool _stdcall SetPhysLong(PBYTE pbPhysAddr, DWORD dwPhysVal);
    WINIO_API bool _stdcall GetPortVal(WORD wPortAddr, PDWORD pdwPortVal, BYTE bSize);
    WINIO_API bool _stdcall SetPortVal(WORD wPortAddr, DWORD dwPortVal, BYTE bSize);
    WINIO_API bool _stdcall InstallWinIoDriver(PWSTR pszWinIoDriverPath, bool IsDemandLoaded = false);
    WINIO_API bool _stdcall RemoveWinIoDriver();
}

extern HANDLE hDriver;
extern bool IsWinIoInitialized;
extern bool g_Is64BitOS;

bool _stdcall StartWinIoDriver();
bool _stdcall StopWinIoDriver();

#endif
