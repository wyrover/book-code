#include "memory.h"

ULONG cactus::ZLMemory::GetMemorySize()
{
    MEMORYSTATUSEX statex = {0};
    statex.dwLength = sizeof(statex);
    ::GlobalMemoryStatusEx(&statex);
    return (ULONG)((statex.ullTotalPhys) / (1024 * 1024));
}

ULONGLONG cactus::ZLMemory::GetUsedMem()
{
    MEMORYSTATUSEX statex = {0};
    statex.dwLength = sizeof(statex);
    ::GlobalMemoryStatusEx(&statex);
    return (ULONG)((statex.ullTotalPhys - statex.ullAvailPhys) / (1024 * 1024));
}
