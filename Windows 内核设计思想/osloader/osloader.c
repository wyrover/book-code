#include <ntoskrnl.h>
#include <freeldr.h>
#define NDEBUG
#include <debug.h>

#include <mm\ARM3\miarm.h>
#include "osloader.h"

ULONG MmUserProbeAddress;
PVOID MmSystemRangeStart;
PVOID MmHighestUserAddress;
LIST_ENTRY PsActiveProcessHead;
PHANDLE_TABLE PspCidTable;
PVOID MmNonPagedPoolStart;
PVOID MmNonPagedPoolEnd0;
PVOID MmPagedPoolStart = MI_PAGED_POOL_START;
PVOID MmPagedPoolEnd;

ULONG HeapFreeBase = 0x62000, HeapLimit = 0xA0000;
ULONG_PTR PermanentHeap = 0x39000;
ULONG_PTR TemporaryHeap = 0x50000;

MMPTE ValidKernelPde2 = {{PTE_VALID | PTE_READWRITE | PTE_DIRTY | PTE_ACCESSED}};
PMMPTE MmDebugPte2 = NULL;
PMMPFN MmPfnDatabase = 0xB0000000;
POBJECT_DIRECTORY ObpRootDirectoryObject;
POBJECT_TYPE ObpTypeObjectType = NULL;

char* MemoryTypeString[] = {
    "LoaderExceptionBlock",
    "LoaderSystemBlock",
    "LoaderFree",
    "LoaderBad",
    "LoaderLoadedProgram",
    "LoaderFirmwareTemporary",
    "LoaderFirmwarePermanent",
    "LoaderOsloaderHeap",
    "LoaderOsloaderStack",
    "LoaderSystemCode",
    "LoaderHalCode",
    "LoaderBootDriver",
    "LoaderConsoleInDriver",
    "LoaderConsoleOutDriver",
    "LoaderStartupDpcStack",
    "LoaderStartupKernelStack",
    "LoaderStartupPanicStack",
    "LoaderStartupPcrPage",
    "LoaderStartupPdrPage",
    "LoaderRegistryData",
    "LoaderMemoryData",
    "LoaderNlsData",
    "LoaderSpecialMemory",
    "LoaderBBTMemory",
    "LoaderReserve",
    "LoaderXIPRom",
    "LoaderHALCachedMemory",
    "LoaderLargePageFiller",
    "LoaderErrorLogMemory",
    "LoaderMaximum"
};

KIRQL NTAPI KeGetCurrentIrql(VOID)
/*
 * PURPOSE: Returns the current irq level
 * RETURNS: The current irq level
 */
{
    return (KeGetPcr()->Irql);
}

NTSTATUS
NTAPI
MmAccessFault(IN BOOLEAN StoreInstruction,
              IN PVOID Address,
              IN KPROCESSOR_MODE Mode,
              IN PVOID TrapInformation)
{
    return STATUS_ACCESS_VIOLATION;
}

VOID
NTAPI
KeBugCheckEx(
    _In_ ULONG BugCheckCode,
    _In_ ULONG_PTR BugCheckParameter1,
    _In_ ULONG_PTR BugCheckParameter2,
    _In_ ULONG_PTR BugCheckParameter3,
    _In_ ULONG_PTR BugCheckParameter4)
{
    _asm int 3
    _asm cli

    for (;;)
        _asm hlt
    }

PVOID
NTAPI
RtlpAllocateMemory(ULONG Bytes,
                   ULONG Tag)
{
    return MmHeapAlloc(Bytes);
}

VOID
NTAPI
RtlpFreeMemory(PVOID Mem,
               ULONG Tag)
{
    MmHeapFree(Mem);
}


PVOID
AllocateHeapPermanent(SIZE_T MemorySize)
{
    MemorySize = ALIGN_UP_BY(MemorySize, MM_PAGE_SIZE);

    if (PermanentHeap + MemorySize <= TemporaryHeap) {
        memset((void *)PermanentHeap, 0, MemorySize);
        PermanentHeap += MemorySize;
        return PermanentHeap - MemorySize;
    }

    return NULL;
}

PVOID
MmHeapAllocEx(SIZE_T MemorySize)
{
    MemorySize = ALIGN_UP_BY(MemorySize, 4);

    if (HeapFreeBase + MemorySize <= HeapLimit) {
        memset((void *)HeapFreeBase, 0, MemorySize);
        HeapFreeBase += MemorySize;
        return PaToVa((PVOID)(HeapFreeBase - MemorySize));
    }

    return NULL;
}

VOID
NTAPI
ProbeForWrite(IN PVOID Address,
              IN SIZE_T Length,
              IN ULONG Alignment)
{
    UNIMPLEMENTED
}

BOOLEAN
NTAPI
DbgkForwardException(IN PEXCEPTION_RECORD ExceptionRecord,
                     IN BOOLEAN DebugPort,
                     IN BOOLEAN SecondChance)
{
    UNIMPLEMENTED
    return TRUE;
}

NTSTATUS
NTAPI
NtTerminateProcess(IN HANDLE ProcessHandle OPTIONAL,
                   IN NTSTATUS ExitStatus)
{
    UNIMPLEMENTED
    return STATUS_SUCCESS;
}

VOID
MmHeapFreeEx(PVOID MemoryPointer)
{
    UNIMPLEMENTED
}

VOID UiMessageBox(PCSTR MessageText)
{
    UNIMPLEMENTED
}

VOID UiMessageBoxCritical(PCSTR MessageText)
{
    UNIMPLEMENTED
}

int TuiPrintf(const char *format, ...)
{
    UNIMPLEMENTED
    return STATUS_SUCCESS;
}

const DEVVTBL* IsoMount(ULONG DeviceId)
{
    UNIMPLEMENTED
    return NULL;
}

const DEVVTBL* FatMount(ULONG DeviceId)
{
    UNIMPLEMENTED
    return NULL;
}

const DEVVTBL* NtfsMount(ULONG DeviceId)
{
    return LdfsMount(DeviceId);
}

const DEVVTBL* Ext2Mount(ULONG DeviceId)
{
    UNIMPLEMENTED
    return NULL;
}

const DEVVTBL* PxeMount(ULONG DeviceId)
{
    UNIMPLEMENTED
    return NULL;
}

NTSTATUS CMAPI
HvInitialize(
    PHHIVE RegistryHive,
    ULONG Operation,
    ULONG HiveType,
    ULONG HiveFlags,
    PVOID HiveData OPTIONAL,
    PALLOCATE_ROUTINE Allocate,
    PFREE_ROUTINE Free,
    PFILE_SET_SIZE_ROUTINE FileSetSize,
    PFILE_WRITE_ROUTINE FileWrite,
    PFILE_READ_ROUTINE FileRead,
    PFILE_FLUSH_ROUTINE FileFlush,
    ULONG Cluster OPTIONAL,
    PUNICODE_STRING FileName)
{
    UNIMPLEMENTED
    return NULL;
}

PVOID CMAPI
HvGetCell(
    PHHIVE RegistryHive,
    HCELL_INDEX CellIndex)
{
    UNIMPLEMENTED
    return NULL;
}

VOID
FASTCALL
KiUnexpectedInterruptTailHandler(IN PKTRAP_FRAME TrapFrame)
{
    UNIMPLEMENTED
}

BOOLEAN DiskReadLogicalSectors(UCHAR DriveNumber, ULONGLONG SectorNumber, ULONG SectorCount, PVOID Buffer)
{
    ExternalServicesTable->ReadWriteSector(DriveNumber, SectorNumber, 0, SectorCount, Buffer, 0x42);
    return TRUE;
}

VOID
NTAPI
MiMapPDEs(
    PVOID StartAddress,
    PVOID EndAddress)
{
    PMMPDE PointerPde;
    MMPDE TmplPde = ValidKernelPde2;

    /* Loop the PDEs */
    for (PointerPde = MiAddressToPde(StartAddress);
         PointerPde <= MiAddressToPde(EndAddress);
         PointerPde++) {
        /* Check if its already mapped */
        if (!PointerPde->u.Hard.Valid) {
            /* No, map it! */
            TmplPde.u.Hard.PageFrameNumber = (ULONG)(AllocateHeapPermanent(4096)) >> MM_PAGE_SHIFT;
            MI_WRITE_VALID_PTE(PointerPde, TmplPde);
            /* Zero out the page table */
            RtlZeroMemory(MiPteToAddress(PointerPde), PAGE_SIZE);
        }
    }
}

VOID MempTurnOnPaging(VOID)
{
#pragma pack(2)
    typedef struct {
        USHORT Limit;
        ULONG Base;
    } GDTIDT;
#pragma pack(4)
    GDTIDT GdtDesc, IdtDesc;
    PKGDTENTRY  pGdt;
    ULONG BlockSize;
    MempAllocatePageTables();
    MempSetupPaging(0, 0x1000, TRUE);
    MempSetupPaging((ULONG)PageLookupTableAddress / MM_PAGE_SIZE, TotalPagesInLookupTable * sizeof(PAGE_LOOKUP_TABLE_ITEM) / MM_PAGE_SIZE, FALSE);
    // Get old values of GDT and IDT
    Ke386GetGlobalDescriptorTable(&GdtDesc);
    __sidt(&IdtDesc);

    if (GdtDesc.Base + GdtDesc.Limit + 1 != IdtDesc.Base) {
        while (1);
    }

    BlockSize = GdtDesc.Limit + 1 + GdtDesc.Limit + 1;
    pGdt = AllocateHeapPermanent(BlockSize);
    RtlMoveMemory(pGdt, (void *)GdtDesc.Base, BlockSize);
    GdtDesc.Base = pGdt;
    IdtDesc.Base = (PUCHAR)pGdt + GdtDesc.Limit + 1;
    // Load GDT+IDT
    Ke386SetGlobalDescriptorTable(&GdtDesc);
    __lidt(&IdtDesc);
    __writecr3((unsigned __int64)PDE);
    __writecr0(__readcr0() | CR0_PG);
    /* Setup debug mapping PTE */
    MiMapPDEs((PVOID)MI_DEBUG_MAPPING, (PVOID)MI_DEBUG_MAPPING);
    MmDebugPte2 = MiAddressToPte((PVOID)MI_DEBUG_MAPPING);
#ifdef _WINKD_
    MmDebugPte = MmDebugPte2;
#endif
}

PCONFIGURATION_COMPONENT_DATA
PcHwDetect()
{
    PCONFIGURATION_COMPONENT_DATA ConfigurationTree = NULL;
    ULONG HeapUsed = 0;
    LOAD_BOOT_FILE FileList[64];
    ExternalServicesTable->ReadWriteSector(0x80, Partition1, 0, 4, &FileList, 0x42);
    ExternalServicesTable->ReadWriteSector(0x80, FileList[4].FileStart / 512, 0, FileList[4].FileSize / 512 + 1, (PVOID)0x10000, 0x42);
    ExternalServicesTable->DetectHardware(
        0x58 * PAGE_SIZE,   //HeapBase
        0x8000,             //HeapSize
        &ConfigurationTree, //ConfigurationTree
        &HeapUsed,          //HeapUsed
        NULL,               //Options
        0                   //OptionsLength
    );
    return ConfigurationTree;
}

VOID LoadNLS(PLOADER_PARAMETER_BLOCK LoaderBlock)
{
    PVOID AnsiCodePageData = 0;
    PVOID UnicodeCaseTableData = 0;
    PVOID VgaCodePageData = 0;
    PVOID SystemHiveData = 0;
    LOAD_BOOT_FILE FileList[64];
    ExternalServicesTable->ReadWriteSector(0x80, Partition1, 0, 4, (PUCHAR)&FileList, 0x42);
    // "\WINDOWS\system32\c_936.nls"
    AnsiCodePageData = MmAllocateMemoryWithType(FileList[5].FileSize, LoaderNlsData);
    LdfsReadDisk(0x80, FileList[5].FileStart, FileList[5].FileSize, AnsiCodePageData);
    // "\WINDOWS\system32\l_intl.nls"
    UnicodeCaseTableData = MmAllocateMemoryWithType(FileList[5].FileSize + FileList[6].FileSize, LoaderNlsData);
    LdfsReadDisk(0x80, FileList[6].FileStart, FileList[6].FileSize, UnicodeCaseTableData);
    // "\WINDOWS\FONTS\vgaoem.fon"
    VgaCodePageData = MmAllocateMemoryWithType(FileList[7].FileSize, LoaderNlsData);
    LdfsReadDisk(0x80, FileList[7].FileStart, FileList[7].FileSize, VgaCodePageData);
    // "\WINDOWS\system32\system"
    SystemHiveData = MmAllocateMemoryWithType(FileList[8].FileSize, LoaderSystemBlock);
    LdfsReadDisk(0x80, FileList[8].FileStart, FileList[8].FileSize, SystemHiveData);
    LoaderBlock->NlsData                        = PaToVa(LoaderBlock->NlsData);
    LoaderBlock->NlsData->AnsiCodePageData      = PaToVa(AnsiCodePageData);
    LoaderBlock->NlsData->OemCodePageData       = PaToVa(AnsiCodePageData);
    LoaderBlock->NlsData->UnicodeCodePageData   = PaToVa(UnicodeCaseTableData);
    LoaderBlock->OemFontFile                    = PaToVa(UnicodeCaseTableData);
    LoaderBlock->RegistryBase                   = PaToVa(SystemHiveData);
    LoaderBlock->RegistryLength                 = FileList[8].FileSize;
}

ARC_STATUS
WinAddToBootDriverList(
    IN PLIST_ENTRY ListHead,
    IN PCHAR FileName,
    IN PCHAR RegistryName,
    IN PCHAR Group,
    IN ULONG Tag,
    IN ULONG ErrorControl,
    IN BOOLEAN InsertHeadOrTail,
    PLDR_DATA_TABLE_ENTRY DataTableEntry)
{
    WCHAR wzFileName[32] = {0};
    WCHAR wzRegistryName[32] = {0};
    WCHAR wzGroup[32] = {0};
    size_t i;
    ULONG Length;
    PBOOT_DRIVER_NODE BootDriverNode;

    for (i = 0; i < strlen(FileName); i++) {
        wzFileName[i] = FileName[i];
    }

    for (i = 0; i < strlen(RegistryName); i++) {
        if (RegistryName[i] == '.')
            break;

        wzRegistryName[i] = RegistryName[i];
    }

    for (i = 0; i < strlen(Group); i++) {
        wzGroup[i] = Group[i];
    }

    /* allocate boot driver node */
    BootDriverNode = MmHeapAllocEx(sizeof(BOOT_DRIVER_NODE));

    if (!BootDriverNode)
        return ENOMEM;

    BootDriverNode->ListEntry.LdrEntry = DataTableEntry;
    /* allocate file name buffer */
    Length = wcslen(wzFileName) * sizeof(WCHAR) + sizeof(L"System32\\drivers\\");
    BootDriverNode->ListEntry.FilePath.Buffer = MmHeapAllocEx(Length);

    if (!BootDriverNode->ListEntry.FilePath.Buffer)
        return ENOMEM;

    /* set file path */
    BootDriverNode->ListEntry.FilePath.MaximumLength    = (USHORT)Length;
    BootDriverNode->ListEntry.FilePath.Length           = 0;
    RtlAppendUnicodeToString(&BootDriverNode->ListEntry.FilePath, L"System32\\drivers\\");
    RtlAppendUnicodeToString(&BootDriverNode->ListEntry.FilePath, wzFileName);
    /* allocate registry buffer */
    Length = wcslen(wzRegistryName) * sizeof(WCHAR) + sizeof(L"\\Registry\\Machine\\System\\CurrentControlSet\\Services\\");
    BootDriverNode->ListEntry.RegistryPath.Buffer = MmHeapAllocEx(Length);

    if (!BootDriverNode->ListEntry.RegistryPath.Buffer)
        return ENOMEM;

    /* set registry path */
    BootDriverNode->ListEntry.RegistryPath.MaximumLength = (USHORT)Length;
    BootDriverNode->ListEntry.RegistryPath.Length       = 0;
    RtlAppendUnicodeToString(&BootDriverNode->ListEntry.RegistryPath, L"\\Registry\\Machine\\System\\CurrentControlSet\\Services\\");
    RtlAppendUnicodeToString(&BootDriverNode->ListEntry.RegistryPath, wzRegistryName);
    /* allocate group buffer */
    Length = wcslen(wzGroup) * sizeof(WCHAR) + sizeof(WCHAR);
    BootDriverNode->Group.Buffer = MmHeapAllocEx(Length);

    if (!BootDriverNode->Group.Buffer)
        return ENOMEM;

    /* set group */
    BootDriverNode->Group.MaximumLength = (USHORT)Length;
    BootDriverNode->Group.Length = 0;
    RtlAppendUnicodeToString(&BootDriverNode->Group, wzGroup);
    /* allocate name buffer */
    Length = wcslen(wzRegistryName) * sizeof(WCHAR) + sizeof(WCHAR);
    BootDriverNode->Name.Buffer = MmHeapAllocEx(Length);

    if (!BootDriverNode->Name.Buffer)
        return ENOMEM;

    //
    // set name
    //
    BootDriverNode->Name.MaximumLength = (USHORT)Length;
    BootDriverNode->Name.Length = 0;
    RtlAppendUnicodeToString(&BootDriverNode->Name, wzRegistryName);
    /* set tag and error control */
    BootDriverNode->Tag = Tag;
    BootDriverNode->ErrorControl = ErrorControl;

    /* link to list head */
    if (InsertHeadOrTail)
        InsertHeadList(ListHead, &BootDriverNode->ListEntry.Link);
    else
        InsertTailList(ListHead, &BootDriverNode->ListEntry.Link);

    return ESUCCESS;
}

NTSTATUS
BlSetupFotNt(
    IN PLOADER_PARAMETER_BLOCK LoaderBlock
)
{
// This is needed only for SetProcessorContext routine
#pragma pack(2)
    typedef struct {
        USHORT Limit;
        ULONG Base;
    } GDTIDT;
#pragma pack(4)
    PKIPCR Pcr;
    PKTSS Tss;
    GDTIDT GdtDesc, IdtDesc;
    PKGDTENTRY pGdt;
    Ke386GetGlobalDescriptorTable(&GdtDesc);
    __sidt(&IdtDesc);
    GdtDesc.Base = KSEG0_BASE | GdtDesc.Base;
    IdtDesc.Base = KSEG0_BASE | IdtDesc.Base;
    pGdt = (PKGDTENTRY)GdtDesc.Base;
    Pcr = (PKIPCR)KIP0PCRADDRESS;
    Tss = KSEG0_BASE | (TssBasePage << MM_PAGE_SHIFT);
    /*Initialize selector that points to PCR*/
    pGdt[6].BaseLow  = (USHORT)((ULONG)Pcr & 0xffff);
    pGdt[6].HighWord.Bytes.BaseMid = (UCHAR)(((ULONG)Pcr >> 16) & 0xff);
    pGdt[6].HighWord.Bytes.BaseHi  = (UCHAR)(((ULONG)Pcr >> 24) & 0xff);
    /*Initialize selector that points to TSS*/
    pGdt[5].BaseLow = (USHORT)((ULONG)Tss & 0xffff);
    pGdt[5].HighWord.Bytes.BaseMid = (UCHAR)(((ULONG)Tss >> 16) & 0xff);
    pGdt[5].HighWord.Bytes.BaseHi  = (UCHAR)(((ULONG)Tss >> 24) & 0xff);
    // Load GDT+IDT
    Ke386SetGlobalDescriptorTable(&GdtDesc);
    __lidt(&IdtDesc);
    _asm mov eax, cr3
    _asm mov cr3, eax
    _asm push KGDT_R0_PCR
    _asm pop fs
    Pcr->NtTib.ExceptionList = EXCEPTION_CHAIN_END;
    Pcr->NtTib.StackBase = 0x61ffc;
    Pcr->NtTib.StackLimit = 0x61ffc - 0x1000;
    Pcr->NtTib.Self = &Pcr->NtTib;
    Pcr->Self = Pcr;
    Pcr->Prcb = &Pcr->PrcbData;
    LoaderBlock->Prcb = Pcr->Prcb;

    if (LoaderBlock->Extension)
        LoaderBlock->Extension->DumpHeader = MmDebugPte2;

#ifdef _WINKD_
    //memcpy(Pcr->Prcb, KiProcessorBlock[0], sizeof(KPRCB));
    KiProcessorBlock[0] = Pcr->Prcb;
#endif
    return STATUS_SUCCESS;
}

#ifndef _WINKD_
ULONG
__cdecl
DbgPrint(PCCH Format,
         ...)
{
    return STATUS_SUCCESS;
}
#else
VOID DivideFault();
VOID PageFault();
void BubbleSort(int element[], int n);
VOID testSEH();
#pragma alloc_text(".testSEH", DivideFault)
#pragma alloc_text(".testSEH", PageFault)
#pragma alloc_text(".testSEH", BubbleSort)
#pragma alloc_text(".testSEH", testSEH)

/* Numerator divided by the denominator */
VOID DivideFault()
{
    __try {
        int n = 0;
        n = 2 / n; // error C2124: 被零除或对零求模
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        _asm int 3
    }
}

VOID PageFault()
{
    __try {
        int* p = 0x00000000;   // pointer to NULL
        *p = 13;    // causes an access violation exception;
    } __except (EXCEPTION_CONTINUE_SEARCH) {
        _asm int 3
    }
}

void BubbleSort(int element[], int n)
{
    int i, j, k;

    for (i = 0; i < n; ++i) {
        for (j = 1; j < n - i; ++j) {
            // 对相邻元素进行比较，判断是否需要交换
            if (element[j] < element[j - 1]) {
                k = element[j];
                element[j] = element[j - 1];
                element[j - 1] = k;
            }
        }
    }
}

VOID testSEH()
{
    __try {
        DivideFault();
        PageFault();
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        _asm int 3
    }

    __try {
        int WaitToSort[] = {6, 1, 2, 4, 3};
        BubbleSort(WaitToSort, 5);
        DbgPrint("BubbleSort: %d, %d, %d, %d, %d\n", WaitToSort[0],
                 WaitToSort[1], WaitToSort[2], WaitToSort[3], WaitToSort[4]);
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        _asm int 3
    }

    do {
        _asm nop
    } while (1);
}

#endif