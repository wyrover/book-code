
#include <ntoskrnl.h>
#define NDEBUG
#include <debug.h>
#include "osloader.h"


/* ARC Loader Block */
PLOADER_PARAMETER_BLOCK KeLoaderBlock;
PEXTERNAL_SERVICES_TABLE ExternalServicesTable;
LIST_ENTRY PsLoadedModuleList;

UCHAR BootOptions[] = "/KERNEL=WRKX86.EXE /HAL=HALMACPI.DLL /DEBUG /DEBUGPORT=COM1 /BAUDRATE=115200 /SOS /BOOTLOGO";
UCHAR BootPath[] = "multi(0)disk(0)rdisk(0)partition(1)\\WINDOWS\\System32\\";
ULONG Partition1 = 2;

PVOID
NTAPI
MiDbgTranslatePhysicalAddress(IN ULONG PhysicalAddress,
                              IN ULONG Flags);
VOID
NTAPI
NtProcessStartup(
    IN PBOOT_CONTEXT BootContextRecord
)
{
#ifdef SIMULATE_DEBUG
    BOOT_CONTEXT _BootContextRecord;
    _BootContextRecord.OsLoaderBase = ((PPEB)BootContextRecord)->ImageBaseAddress;
    BootContextRecord = &_BootContextRecord;
#endif
#ifdef _WINKD_
    KdpInitSystem(0, NULL);
#ifndef SIMULATE_DEBUG

    if (*(WORD*)BootContextRecord->OsLoaderBase != 0x5A4D)
        memcpy(BootContextRecord->OsLoaderBase, 0x24e70, 0x400);

#endif
    DebugLoadImageSymbols("osloader.exe", BootContextRecord->OsLoaderBase);
    DbgBreakPoint();
#endif
    PcMachInit(BootContextRecord);
    MmInitializeMemoryManager();
    MempTurnOnPaging();
    FsInit();
    RamDiskInitialize();
    HwInitializeBiosDisks();
    /* Allocate and minimalistic-initialize LPB */
    AllocateAndInitLPB(&KeLoaderBlock);
    //WinLdrSetupMachineDependent(KeLoaderBlock);
    //BlSetupFotNt(KeLoaderBlock);
    //testSEH();
    LoadAndBootWindowsCommon(
        _WIN32_WINNT,
        KeLoaderBlock,
        BootOptions,
        BootPath,
        0);

    do {
        _asm hlt
    } while (1);
}
