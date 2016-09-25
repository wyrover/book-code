/*
 *  FreeLoader
 *
 *  Copyright (C) 1998-2003  Brian Palmer    <brianp@sginet.com>
 *  Copyright (C) 2006       Aleksey Bragin  <aleksey@reactos.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#include <ntoskrnl.h>
#include <freeldr.h>

#include <ndk/ldrtypes.h>
#include <debug.h>

//++ ree
#include "..\osloader.h"

extern PCONFIGURATION_COMPONENT_DATA FwConfigurationTreeEx;
ARC_STATUS
BlConfigurationInitializeEx(
    IN PCONFIGURATION_COMPONENT Parent,
    IN PCONFIGURATION_COMPONENT_DATA ParentEntry
);
//--

DBG_DEFAULT_CHANNEL(WINDOWS);

//FIXME: Do a better way to retrieve Arc disk information
extern ULONG reactos_disk_count;
extern ARC_DISK_SIGNATURE reactos_arc_disk_info[];

extern ULONG LoaderPagesSpanned;
extern BOOLEAN AcpiPresent;

extern HEADLESS_LOADER_BLOCK LoaderRedirectionInformation;
extern BOOLEAN WinLdrTerminalConnected;
extern void WinLdrSetupEms(IN PCHAR BootOptions);

PLOADER_SYSTEM_BLOCK WinLdrSystemBlock;

// debug stuff
VOID DumpMemoryAllocMap(VOID);

// Init "phase 0"
VOID
AllocateAndInitLPB(PLOADER_PARAMETER_BLOCK *OutLoaderBlock)
{
    PLOADER_PARAMETER_BLOCK LoaderBlock;
    /* Allocate and zero-init the LPB */
    WinLdrSystemBlock = MmAllocateMemoryWithType(sizeof(LOADER_SYSTEM_BLOCK),
                        LoaderSystemBlock);

    if (WinLdrSystemBlock == NULL) {
        UiMessageBox("Failed to allocate memory for system block!");
        return;
    }

    RtlZeroMemory(WinLdrSystemBlock, sizeof(LOADER_SYSTEM_BLOCK));
    LoaderBlock = &WinLdrSystemBlock->LoaderBlock;
    LoaderBlock->NlsData = &WinLdrSystemBlock->NlsDataBlock;
    /* Init three critical lists, used right away */
    InitializeListHead(&LoaderBlock->LoadOrderListHead);
    InitializeListHead(&LoaderBlock->MemoryDescriptorListHead);
    InitializeListHead(&LoaderBlock->BootDriverListHead);
    *OutLoaderBlock = LoaderBlock;
}

// Init "phase 1"
VOID
WinLdrInitializePhase1(PLOADER_PARAMETER_BLOCK LoaderBlock,
                       LPCSTR Options,
                       LPCSTR SystemRoot,
                       LPCSTR BootPath,
                       USHORT VersionToBoot)
{
    /* Examples of correct options and paths */
    //CHAR  Options[] = "/DEBUGPORT=COM1 /BAUDRATE=115200";
    //CHAR  Options[] = "/NODEBUG";
    CHAR    SystemRootA[] = "\\WINDOWS\\";
    CHAR    ArcBoot[] = "multi(0)disk(0)rdisk(0)partition(1)";
    LPSTR LoadOptions, NewLoadOptions;
    CHAR  HalPath[] = "\\";
    //CHAR  ArcBoot[256];
    //CHAR  MiscFiles[256];
    //ULONG i;
    //ULONG_PTR PathSeparator;
    PLOADER_PARAMETER_EXTENSION Extension;
    /* Construct SystemRoot and ArcBoot from SystemPath */
    /*PathSeparator = strstr(BootPath, "\\") - BootPath;
    strncpy(ArcBoot, BootPath, PathSeparator);
    ArcBoot[PathSeparator] = 0;*/
    TRACE("ArcBoot: %s\n", ArcBoot);
    TRACE("SystemRoot: %s\n", SystemRoot);
    TRACE("Options: %s\n", Options);
    /* Fill Arc BootDevice */
    LoaderBlock->ArcBootDeviceName = WinLdrSystemBlock->ArcBootDeviceName;
    strncpy(LoaderBlock->ArcBootDeviceName, ArcBoot, MAX_PATH);
    LoaderBlock->ArcBootDeviceName = PaToVa(LoaderBlock->ArcBootDeviceName);
    /* Fill Arc HalDevice, it matches ArcBoot path */
    LoaderBlock->ArcHalDeviceName = WinLdrSystemBlock->ArcBootDeviceName;
    LoaderBlock->ArcHalDeviceName = PaToVa(LoaderBlock->ArcHalDeviceName);
    /* Fill SystemRoot */
    LoaderBlock->NtBootPathName = WinLdrSystemBlock->NtBootPathName;
    strncpy(LoaderBlock->NtBootPathName, SystemRootA, MAX_PATH);
    LoaderBlock->NtBootPathName = PaToVa(LoaderBlock->NtBootPathName);
    /* Fill NtHalPathName */
    LoaderBlock->NtHalPathName = WinLdrSystemBlock->NtHalPathName;
    strncpy(LoaderBlock->NtHalPathName, HalPath, MAX_PATH);
    LoaderBlock->NtHalPathName = PaToVa(LoaderBlock->NtHalPathName);
    /* Fill LoadOptions and strip the '/' commutator symbol in front of each option */
    NewLoadOptions = LoadOptions = LoaderBlock->LoadOptions = WinLdrSystemBlock->LoadOptions;
    strncpy(LoaderBlock->LoadOptions, Options, MAX_OPTIONS_LENGTH);

    do {
        while (*LoadOptions == '/')
            ++LoadOptions;

        *NewLoadOptions++ = *LoadOptions;
    } while (*LoadOptions++);

    LoaderBlock->LoadOptions = PaToVa(LoaderBlock->LoadOptions);
    /* Arc devices */
    LoaderBlock->ArcDiskInformation = &WinLdrSystemBlock->ArcDiskInformation;
    InitializeListHead(&LoaderBlock->ArcDiskInformation->DiskSignatureListHead);
    /* Convert ARC disk information from freeldr to a correct format */
    //for (i = 0; i < reactos_disk_count; i++)
    {
        PARC_DISK_SIGNATURE_EX ArcDiskSig;
        /* Allocate the ARC structure */
        /*ArcDiskSig = HeapAllocate(FrLdrDefaultHeap,
                                  sizeof(ARC_DISK_SIGNATURE_EX),
                                  'giSD');*/
        ArcDiskSig = MmHeapAllocEx(sizeof(ARC_DISK_SIGNATURE_EX));
        /* Copy the data over */
        //ArcDiskSig->DiskSignature.Signature = reactos_arc_disk_info[i].Signature;
        //ArcDiskSig->DiskSignature.CheckSum = reactos_arc_disk_info[i].CheckSum;
        /* Copy the ARC Name */
        //strncpy(ArcDiskSig->ArcName, reactos_arc_disk_info[i].ArcName, MAX_PATH);
        memset(ArcDiskSig, 0, sizeof(ARC_DISK_SIGNATURE_EX));
        strcpy(ArcDiskSig->ArcName, "multi(0)disk(0)rdisk(0)");
        ArcDiskSig->DiskSignature.ArcName = PaToVa(ArcDiskSig->ArcName);
        /* Mark partition table as valid */
        ArcDiskSig->DiskSignature.ValidPartitionTable = TRUE;
        /* Insert into the list */
        InsertTailList(&LoaderBlock->ArcDiskInformation->DiskSignatureListHead,
                       &ArcDiskSig->DiskSignature.ListEntry);
    }
    /* Convert all list's to Virtual address */
    /* Convert the ArcDisks list to virtual address */
    List_PaToVa(&LoaderBlock->ArcDiskInformation->DiskSignatureListHead);
    LoaderBlock->ArcDiskInformation = PaToVa(LoaderBlock->ArcDiskInformation);
    /* Convert configuration entries to VA */
    ConvertConfigToVA(LoaderBlock->ConfigurationRoot);
    LoaderBlock->ConfigurationRoot = PaToVa(LoaderBlock->ConfigurationRoot);
    /* Convert all DTE into virtual addresses */
    List_PaToVa(&LoaderBlock->LoadOrderListHead);
    /* this one will be converted right before switching to
       virtual paging mode */
    //List_PaToVa(&LoaderBlock->MemoryDescriptorListHead);
    /* Convert list of boot drivers */
    List_PaToVa(&LoaderBlock->BootDriverListHead);
    /* Initialize Extension now */
    Extension = &WinLdrSystemBlock->Extension;
    Extension->Size = sizeof(LOADER_PARAMETER_EXTENSION);
    Extension->MajorVersion = (VersionToBoot & 0xFF00) >> 8;
    Extension->MinorVersion = VersionToBoot & 0xFF;
    Extension->Profile.Status = 2;
    /* Check if ACPI is present */
    //if (AcpiPresent)
    //{
    //  /* See KiRosFrldrLpbToNtLpb for details */
    //  Extension->AcpiTable = (PVOID)1;
    //}
#ifdef _M_IX86
    /* Set headless block pointer */
    /*if (WinLdrTerminalConnected)
    {
        Extension->HeadlessLoaderBlock = &WinLdrSystemBlock->HeadlessLoaderBlock;
        RtlCopyMemory(Extension->HeadlessLoaderBlock,
                      &LoaderRedirectionInformation,
                      sizeof(HEADLESS_LOADER_BLOCK));
        Extension->HeadlessLoaderBlock = PaToVa(Extension->HeadlessLoaderBlock);
    }*/
#endif
    /* Load drivers database */
    /*strcpy(MiscFiles, BootPath);
    strcat(MiscFiles, "AppPatch\\drvmain.sdb");
    Extension->DrvDBImage = PaToVa(WinLdrLoadModule(MiscFiles,
                                                    &Extension->DrvDBSize,
                                                    LoaderRegistryData));*/
    /* Convert extension and setup block pointers */
    LoaderBlock->Extension = PaToVa(Extension);

    if (LoaderBlock->SetupLdrBlock)
        LoaderBlock->SetupLdrBlock = PaToVa(LoaderBlock->SetupLdrBlock);

    TRACE("WinLdrInitializePhase1() completed\n");
}

static BOOLEAN
WinLdrLoadDeviceDriver(PLIST_ENTRY LoadOrderListHead,
                       LPSTR BootPath,
                       PUNICODE_STRING FilePath,
                       ULONG Flags,
                       PLDR_DATA_TABLE_ENTRY *DriverDTE)
{
    CHAR FullPath[1024];
    CHAR DriverPath[1024];
    CHAR DllName[1024];
    PCHAR DriverNamePos;
    BOOLEAN Status;
    PVOID DriverBase;
    // Separate the path to file name and directory path
    _snprintf(DriverPath, sizeof(DriverPath), "%wZ", FilePath);
    DriverNamePos = strrchr(DriverPath, '\\');

    if (DriverNamePos != NULL) {
        // Copy the name
        strcpy(DllName, DriverNamePos + 1);
        // Cut out the name from the path
        *(DriverNamePos + 1) = 0;
    } else {
        // There is no directory in the path
        strcpy(DllName, DriverPath);
        DriverPath[0] = 0;
    }

    TRACE("DriverPath: %s, DllName: %s, LPB\n", DriverPath, DllName);
    // Check if driver is already loaded
    Status = WinLdrCheckForLoadedDll(LoadOrderListHead, DllName, DriverDTE);

    if (Status) {
        // We've got the pointer to its DTE, just return success
        return TRUE;
    }

    // It's not loaded, we have to load it
    _snprintf(FullPath, sizeof(FullPath), "%s%wZ", BootPath, FilePath);
    Status = WinLdrLoadImage(FullPath, LoaderBootDriver, &DriverBase);

    if (!Status)
        return FALSE;

    // Allocate a DTE for it
    Status = WinLdrAllocateDataTableEntry(LoadOrderListHead, DllName, DllName, DriverBase, DriverDTE);

    if (!Status) {
        ERR("WinLdrAllocateDataTableEntry() failed\n");
        return FALSE;
    }

    // Modify any flags, if needed
    (*DriverDTE)->Flags |= Flags;
    // Look for any dependencies it may have, and load them too
    sprintf(FullPath, "%s%s", BootPath, DriverPath);
    Status = WinLdrScanImportDescriptorTable(LoadOrderListHead, FullPath, *DriverDTE);

    if (!Status) {
        ERR("WinLdrScanImportDescriptorTable() failed for %s\n", FullPath);
        return FALSE;
    }

    return TRUE;
}

BOOLEAN
WinLdrLoadBootDrivers(PLOADER_PARAMETER_BLOCK LoaderBlock,
                      LPSTR BootPath)
{
    PLIST_ENTRY NextBd;
    PBOOT_DRIVER_LIST_ENTRY BootDriver;
    BOOLEAN Status;
    // Walk through the boot drivers list
    NextBd = LoaderBlock->BootDriverListHead.Flink;

    while (NextBd != &LoaderBlock->BootDriverListHead) {
        BootDriver = CONTAINING_RECORD(NextBd, BOOT_DRIVER_LIST_ENTRY, Link);
        TRACE("BootDriver %wZ DTE %08X RegPath: %wZ\n", &BootDriver->FilePath,
              BootDriver->LdrEntry, &BootDriver->RegistryPath);
        // Paths are relative (FIXME: Are they always relative?)
        // Load it
        Status = WinLdrLoadDeviceDriver(&LoaderBlock->LoadOrderListHead, BootPath, &BootDriver->FilePath,
                                        0, &BootDriver->LdrEntry);

        // If loading failed - cry loudly
        //FIXME: Maybe remove it from the list and try to continue?
        if (!Status) {
            UiMessageBox("Can't load boot driver!");
            return FALSE;
        }

        // Convert the RegistryPath and DTE addresses to VA since we are not going to use it anymore
        BootDriver->RegistryPath.Buffer = PaToVa(BootDriver->RegistryPath.Buffer);
        BootDriver->FilePath.Buffer = PaToVa(BootDriver->FilePath.Buffer);
        BootDriver->LdrEntry = PaToVa(BootDriver->LdrEntry);
        NextBd = BootDriver->Link.Flink;
    }

    return TRUE;
}

PVOID WinLdrLoadModule(PCSTR ModuleName, ULONG *Size,
                       TYPE_OF_MEMORY MemoryType)
{
    ULONG FileId;
    PVOID PhysicalBase;
    FILEINFORMATION FileInfo;
    ULONG FileSize;
    ULONG Status;
    ULONG BytesRead;
    //CHAR ProgressString[256];
    /* Inform user we are loading files */
    //sprintf(ProgressString, "Loading %s...", FileName);
    //UiDrawProgressBarCenter(1, 100, ProgressString);
    TRACE("Loading module %s\n", ModuleName);
    *Size = 0;
    /* Open the image file */
    Status = ArcOpen((PCHAR)ModuleName, OpenReadOnly, &FileId);

    if (Status != ESUCCESS) {
        /* In case of errors, we just return, without complaining to the user */
        return NULL;
    }

    /* Get this file's size */
    Status = ArcGetFileInformation(FileId, &FileInfo);

    if (Status != ESUCCESS) {
        ArcClose(FileId);
        return NULL;
    }

    FileSize = FileInfo.EndingAddress.LowPart;
    *Size = FileSize;
    /* Allocate memory */
    PhysicalBase = MmAllocateMemoryWithType(FileSize, MemoryType);

    if (PhysicalBase == NULL) {
        ArcClose(FileId);
        return NULL;
    }

    /* Load whole file */
    Status = ArcRead(FileId, PhysicalBase, FileSize, &BytesRead);
    ArcClose(FileId);

    if (Status != ESUCCESS) {
        return NULL;
    }

    TRACE("Loaded %s at 0x%x with size 0x%x\n", ModuleName, PhysicalBase, FileSize);
    return PhysicalBase;
}

static
PVOID
LoadModule(
    PLOADER_PARAMETER_BLOCK LoaderBlock,
    PCCH Path,
    PCCH File,
    TYPE_OF_MEMORY MemoryType,
    PLDR_DATA_TABLE_ENTRY *Dte,
    ULONG Percentage)
{
    CHAR FullFileName[MAX_PATH];
    //CHAR ProgressString[256];
    NTSTATUS Status;
    PVOID BaseAdress;
    //UiDrawBackdrop();
    //sprintf(ProgressString, "Loading %s...", File);
    //UiDrawProgressBarCenter(Percentage, 100, ProgressString);
    strcpy(FullFileName, Path);
    //strcat(FullFileName, "System32\\");
    strcat(FullFileName, File);
    Status = WinLdrLoadImage(FullFileName, MemoryType, &BaseAdress);
    TRACE("%s loaded with status %d at %p\n",
          File, Status, BaseAdress);
    //strcpy(FullFileName, "WINDOWS\\SYSTEM32\\");
    //strcat(FullFileName, File);
    WinLdrAllocateDataTableEntry(&LoaderBlock->LoadOrderListHead, File,
                                 FullFileName, BaseAdress, Dte);
    return BaseAdress;
}

VOID
LoadAndBootWindowsCommon(
    USHORT OperatingSystemVersion,
    PLOADER_PARAMETER_BLOCK LoaderBlock,
    LPCSTR BootOptions,
    LPCSTR BootPath,
    BOOLEAN Setup)
{
    PLOADER_PARAMETER_BLOCK LoaderBlockVA;
    BOOLEAN Status;
    CHAR FileName[MAX_PATH];
    PLDR_DATA_TABLE_ENTRY KernelDTE, HalDTE, KdComDTE = NULL;
    KERNEL_ENTRY_POINT KiSystemStartup;
    LPCSTR SystemRoot;
    //++ ree
    int i;
    LOAD_BOOT_FILE FileList[64] = {0};
    ExternalServicesTable->ReadWriteSector(0x80, Partition1, 0, 4, &FileList, 0x42);
    TRACE("LoadAndBootWindowsCommon()\n");
    /* Convert BootPath to SystemRoot */
    SystemRoot = strstr(BootPath, "\\");
    /* Detect hardware */
    LoaderBlock->ConfigurationRoot = MachHwDetect();
    //if (OperatingSystemVersion == 0)
    //  OperatingSystemVersion = WinLdrDetectVersion();
    /* Load kernel */
    LoadModule(LoaderBlock, BootPath, "NTOSKRNL.EXE", LoaderSystemCode, &KernelDTE, 30);
    /* Load HAL */
    LoadModule(LoaderBlock, BootPath, "HAL.DLL", LoaderHalCode, &HalDTE, 45);

    /* Load kernel-debugger support dll */
    if (OperatingSystemVersion > _WIN32_WINNT_WIN2K) {
        LoadModule(LoaderBlock, BootPath, "KDCOM.DLL", LoaderSystemCode, &KdComDTE, 60);
    }

    /* Load all referenced DLLs for kernel, HAL and kdcom.dll */
    strcpy(FileName, BootPath);
    //strcat(FileName, "System32\\");
    Status = WinLdrScanImportDescriptorTable(&LoaderBlock->LoadOrderListHead, FileName, KernelDTE);
    Status &= WinLdrScanImportDescriptorTable(&LoaderBlock->LoadOrderListHead, FileName, HalDTE);

    if (KdComDTE)
        Status &= WinLdrScanImportDescriptorTable(&LoaderBlock->LoadOrderListHead, FileName, KdComDTE);

    if (!Status) {
        UiMessageBox("Error loading imported dll.");
        return;
    }

    //++ ree
#ifdef _WINKD_
    PsNtosImageBase = KernelDTE->DllBase;
    DebugLoadImageSymbols("NTOSKRNL.EXE", KernelDTE->DllBase);
    DebugLoadImageSymbols("HAL.DLL", HalDTE->DllBase);
#endif
    LoadNLS(LoaderBlock);

    for (i = 11; i < 64; i++) {
        if (FileList[i].FileFlags && FileList[i].FileStart && FileList[i].FileSize) {
            PVOID ImageBase = NULL;
            PLDR_DATA_TABLE_ENTRY DataTableEntry = NULL;

            if (!WinLdrCheckForLoadedDll(&LoaderBlock->LoadOrderListHead, FileList[i].FileName, &DataTableEntry)) {
                PVOID ImageBaseVA = NULL;
                strcpy(FileName, BootPath);
                strcat(FileName, "drivers\\");
                ImageBaseVA = LoadModule(LoaderBlock, FileName, FileList[i].FileName, LoaderSystemCode, &DataTableEntry, 75);

                if (ImageBaseVA == NULL || DataTableEntry == NULL)
                    break;

                Status = WinLdrScanImportDescriptorTable(&LoaderBlock->LoadOrderListHead, FileName, DataTableEntry);

                if (!Status)
                    break;
            }

            WinAddToBootDriverList(&LoaderBlock->BootDriverListHead,
                                   FileList[i].FileName,
                                   FileList[i].FileName,
                                   "SAC",
                                   1,
                                   1,
                                   FALSE,
                                   DataTableEntry);
        }
    }

    //--
    /* Load boot drivers */
    //UiDrawBackdrop();
    //UiDrawProgressBarCenter(100, 100, "Loading boot drivers...");
    //Status = WinLdrLoadBootDrivers(LoaderBlock, (PCHAR)BootPath);
    TRACE("Boot drivers loaded with status %d\n", Status);
    /* Initialize Phase 1 - no drivers loading anymore */
    WinLdrInitializePhase1(LoaderBlock, BootOptions, SystemRoot, BootPath, OperatingSystemVersion);
    /* Save entry-point pointer and Loader block VAs */
    KiSystemStartup = (KERNEL_ENTRY_POINT)KernelDTE->EntryPoint;
    LoaderBlockVA = PaToVa(LoaderBlock);
    /* "Stop all motors", change videomode */
    //MachPrepareForReactOS(Setup);
    /* Debugging... */
    //DumpMemoryAllocMap();
    /* Do the machine specific initialization */
    WinLdrSetupMachineDependent(LoaderBlock);
    /* Map pages and create memory descriptors */
    WinLdrSetupMemoryLayout(LoaderBlock);
    /* Set processor context */
    //WinLdrSetProcessorContext();
    /* Save final value of LoaderPagesSpanned */
    LoaderBlock->Extension->LoaderPagesSpanned = LoaderPagesSpanned;
    TRACE("Hello from paged mode, KiSystemStartup %p, LoaderBlockVA %p!\n",
          KiSystemStartup, LoaderBlockVA);
    // Zero KI_USER_SHARED_DATA page
    memset((PVOID)KI_USER_SHARED_DATA, 0, MM_PAGE_SIZE);
    //WinLdrpDumpMemoryDescriptors(LoaderBlockVA);
    //WinLdrpDumpBootDriver(LoaderBlockVA);
#ifndef _M_AMD64
    //WinLdrpDumpArcDisks(LoaderBlockVA);
#endif
    //++ ree
#ifdef _WINKD_
    KdStopDebugger();
#endif
    InitializeListHead(&LoaderBlock->Extension->FirmwareDescriptorListHead);
    LoaderBlock->Extension->HeadlessLoaderBlock = MmHeapAllocEx(sizeof(HEADLESS_LOADER_BLOCK));
    LoaderBlock->Extension->SMBiosEPSHeader     = MmHeapAllocEx(sizeof(SMBIOS_TABLE_HEADER));
    LoaderBlock->Extension->NetworkLoaderBlock  = MmHeapAllocEx(sizeof(NETWORK_LOADER_BLOCK));
    BlSetupFotNt(LoaderBlockVA);
    //--
    //FIXME: If I substitute this debugging checkpoint, GCC will "optimize away" the code below
    //while (1) {};
    /*asm(".intel_syntax noprefix\n");
        asm("test1:\n");
        asm("jmp test1\n");
    asm(".att_syntax\n");*/
    /* Pass control */
    (*KiSystemStartup)(LoaderBlockVA);
}

VOID
WinLdrpDumpMemoryDescriptors(PLOADER_PARAMETER_BLOCK LoaderBlock)
{
    PLIST_ENTRY NextMd;
    PMEMORY_ALLOCATION_DESCRIPTOR MemoryDescriptor;
    NextMd = LoaderBlock->MemoryDescriptorListHead.Flink;

    while (NextMd != &LoaderBlock->MemoryDescriptorListHead) {
        MemoryDescriptor = CONTAINING_RECORD(NextMd, MEMORY_ALLOCATION_DESCRIPTOR, ListEntry);
#ifdef _WINKD_
        DbgPrint("BP %08X PC %04X MT %02d(%s)\n", MemoryDescriptor->BasePage,
                 MemoryDescriptor->PageCount, MemoryDescriptor->MemoryType, MemoryTypeString[MemoryDescriptor->MemoryType]);
#endif
        NextMd = MemoryDescriptor->ListEntry.Flink;
    }
}

VOID
WinLdrpDumpBootDriver(PLOADER_PARAMETER_BLOCK LoaderBlock)
{
    PLIST_ENTRY NextBd;
    PBOOT_DRIVER_LIST_ENTRY BootDriver;
    NextBd = LoaderBlock->BootDriverListHead.Flink;

    while (NextBd != &LoaderBlock->BootDriverListHead) {
        BootDriver = CONTAINING_RECORD(NextBd, BOOT_DRIVER_LIST_ENTRY, Link);
        TRACE("BootDriver %wZ DTE %08X RegPath: %wZ\n", &BootDriver->FilePath,
              BootDriver->LdrEntry, &BootDriver->RegistryPath);
        NextBd = BootDriver->Link.Flink;
    }
}

VOID
WinLdrpDumpArcDisks(PLOADER_PARAMETER_BLOCK LoaderBlock)
{
    PLIST_ENTRY NextBd;
    PARC_DISK_SIGNATURE ArcDisk;
    NextBd = LoaderBlock->ArcDiskInformation->DiskSignatureListHead.Flink;

    while (NextBd != &LoaderBlock->ArcDiskInformation->DiskSignatureListHead) {
        ArcDisk = CONTAINING_RECORD(NextBd, ARC_DISK_SIGNATURE, ListEntry);
        TRACE("ArcDisk %s checksum: 0x%X, signature: 0x%X\n",
              ArcDisk->ArcName, ArcDisk->CheckSum, ArcDisk->Signature);
        NextBd = ArcDisk->ListEntry.Flink;
    }
}