#pragma once

typedef struct _FSCONTEXT_RECORD {
    ULONG BootDrive;
    ULONG Reserved;
    ULONG Reserved2;
} FSCONTEXT_RECORD, *PFSCONTEXT_RECORD;

typedef struct _SU_MEMORY_DESCRIPTOR {
    ULONG BlockBase;
    ULONG BlockSize;
} SU_MEMORY_DESCRIPTOR , *PSU_MEMORY_DESCRIPTOR;

typedef struct _E820FRAME {
    ULONG       ErrorFlag;
    ULONG       Key;
    ULONG       Size;
    struct {
        ULONG       BaseAddrLow;
        ULONG       BaseAddrHigh;
        ULONG       SizeLow;
        ULONG       SizeHigh;
        ULONG       MemoryType;
    } Descriptor;
} E820FRAME, *PE820FRAME;

typedef struct _EXTERNAL_SERVICES_TABLE {
    PVOID Reserved1;
    PVOID Reserved2;
    PVOID Reserved3;
    PVOID Reserved4;
    PVOID Reserved5;
    VOID (_cdecl* DetectHardware)(IN ULONG HeapBase, IN ULONG HeapSize, IN OUT PVOID ConfigurationTree, IN OUT PULONG HeapUsed, IN OUT PVOID Options, IN ULONG OptionsLength);
    PVOID Reserved6;
    PVOID Reserved7;
    PVOID Reserved8;
    PVOID Reserved9;
    PVOID Reserved10;
    VOID (_cdecl* GetMemoryDescriptor)(IN PE820FRAME E820Frame);
    NTSTATUS(_cdecl* ReadWriteSector)(IN ULONG DriveNum, IN ULONG LBNLow, IN ULONG LBNHigh, IN ULONG BlockCount, IN OUT PVOID Buffer, IN ULONG ReadWrite); //Read=0x42,Write=0x43
    PVOID Reserved11;

} EXTERNAL_SERVICES_TABLE, *PEXTERNAL_SERVICES_TABLE;
extern PEXTERNAL_SERVICES_TABLE ExternalServicesTable;


typedef struct _BOOT_CONTEXT {
    FSCONTEXT_RECORD* FSContextRecord;
    EXTERNAL_SERVICES_TABLE* ExternalServicesTable;
    SU_MEMORY_DESCRIPTOR* MemoryDescriptorList;
    ULONG MachineType;
    ULONG OsLoaderStart;
    ULONG OsLoaderEnd;
    ULONG ResourceDirectory;
    ULONG ResourceOffset;
    ULONG OsLoaderBase;
    ULONG OsLoaderExports;
} BOOT_CONTEXT, *PBOOT_CONTEXT;

typedef struct _LOAD_BOOT_FILE {
    ULONG FileFlags;
    ULONG FileStart;
    ULONG FileSize;
    CHAR  FileName[20];
} LOAD_BOOT_FILE;

extern ULONG Partition1;
extern ULONG TssBasePage;
extern PHARDWARE_PTE PDE;
extern PHARDWARE_PTE HalPageTable;
extern PFN_NUMBER LoaderPagesSpanned;

char* MemoryTypeString[];


BOOL
LdfsReadDisk(
    IN ULONG DeviceId,
    IN ULONG Start,
    IN ULONG Size,
    IN ULONG Buffer
);

VOID
NTAPI
RamDiskInitialize(VOID);

BOOLEAN
NTAPI
KdpInitSystem(
    IN ULONG Phase,
    IN PLOADER_PARAMETER_BLOCK LoaderBlock
);

VOID
NTAPI
DebugLoadImageSymbols(
    IN PCHAR ImageName,
    IN PVOID BaseAddress
);