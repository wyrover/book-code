#ifndef _COMMON_H_
#define _COMMON_H_

#include "ntiologc.h"
#include "ntimage.h"
///////////////////////////////////////////////

typedef struct _PROCESS_MODULE_INFORMATION {
	ULONG	ModuleBase;
	ULONG   ModuleLength;
	WCHAR	szFilePath[256];
}PROCESS_MODULE_INFORMATION,*PPROCESS_MODULE_INFORMATION;

typedef struct _PROCESS_MEMORY_CONTROL_THUNK{
	PEPROCESS	Process;
	PVOID		Address;
	ULONG		isWrite; //if equal to 0, means read
	PVOID		Buffer;
	ULONG		Length;
}PROCESS_MEMORY_CONTROL_THUNK,*PPROCESS_MEMORY_CONTROL_THUNK;

typedef struct _PROCESS_UNLOAD_MODULE{
	PEPROCESS	Process;
	PVOID		ModuleBase;
}PROCESS_UNLOAD_MODULE,*PPROCESS_UNLOAD_MODULE;

typedef struct _DRIVER_FUNCTION_CALL_INFORMATION
{
	ULONG FunctionCode;
	PVOID InUserBuffer;
	ULONG InUserBufferLength;
	PVOID OutUserBuffer;
	ULONG OutUserBufferLength;
	ULONG LengthReturned;
	ULONG LengthRequired;
	NTSTATUS ReturnStatus;
}DRIVER_FUNCTION_CALL_INFORMATION,*PDRIVER_FUNCTION_CALL_INFORMATION;

///////////////////////////////////////////////

typedef struct _OBJECT_TYPE_INITIALIZER {
	USHORT Length;
	BOOLEAN UseDefaultObject;
	BOOLEAN CaseInsensitive;
	ULONG InvalidAttributes;
	GENERIC_MAPPING GenericMapping;
	ULONG ValidAccessMask;
	BOOLEAN SecurityRequired;
	BOOLEAN MaintainHandleCount;
	BOOLEAN MaintainTypeList;
	POOL_TYPE PoolType;
	ULONG DefaultPagedPoolCharge;
	ULONG DefaultNonPagedPoolCharge;
	PVOID DumpProcedure;
	PVOID OpenProcedure;
	PVOID CloseProcedure;
	PVOID DeleteProcedure;
	PVOID ParseProcedure;
	PVOID SecurityProcedure;
	PVOID QueryNameProcedure;
	PVOID OkayToCloseProcedure;
} OBJECT_TYPE_INITIALIZER, *POBJECT_TYPE_INITIALIZER;

typedef struct _OBJECT_TYPE {
	ERESOURCE Mutex;
	LIST_ENTRY TypeList;
	UNICODE_STRING Name;            // Copy from object header for convenience
	PVOID DefaultObject;
	ULONG Index;
	ULONG TotalNumberOfObjects;
	ULONG TotalNumberOfHandles;
	ULONG HighWaterNumberOfObjects;
	ULONG HighWaterNumberOfHandles;
	OBJECT_TYPE_INITIALIZER TypeInfo;
#ifdef POOL_TAGGING
	ULONG Key;
#endif //POOL_TAGGING
} OBJECT_TYPE, *POBJECT_TYPE;

typedef struct _OBJECT_HEADER 
{
	LONG_PTR PointerCount;
	union 
	{
		LONG_PTR HandleCount;
		PVOID NextToFree;
	};
	POBJECT_TYPE Type;
	UCHAR NameInfoOffset;
	UCHAR HandleInfoOffset;
	UCHAR QuotaInfoOffset;
	UCHAR Flags;
	union
	{
		PVOID ObjectCreateInfo;
		PVOID QuotaBlockCharged;
	};
	PSECURITY_DESCRIPTOR SecurityDescriptor;
	QUAD Body;
} OBJECT_HEADER, *POBJECT_HEADER;

#define RVATOVA(base,rva) ((ULONG)rva-(ULONG)base)

typedef struct _RTL_PROCESS_MODULE_INFORMATION {
	HANDLE Section;                 // Not filled in
	PVOID MappedBase;
	PVOID ImageBase;
	ULONG ImageSize;
	ULONG Flags;
	USHORT LoadOrderIndex;
	USHORT InitOrderIndex;
	USHORT LoadCount;
	USHORT OffsetToFileName;
	CHAR  FullPathName[ 256 ];
} RTL_PROCESS_MODULE_INFORMATION, *PRTL_PROCESS_MODULE_INFORMATION;

typedef struct _RTL_PROCESS_MODULES {
	ULONG NumberOfModules;
	RTL_PROCESS_MODULE_INFORMATION Modules[ 1 ];
} RTL_PROCESS_MODULES, *PRTL_PROCESS_MODULES;


typedef struct _CONTROL_AREA // 0x30
{
	PVOID Segment; // +0x0(0x4)
	LIST_ENTRY DereferenceList; // +0x4(0x8)
	ULONG NumberOfSectionReferences; // +0xc(0x4)
	ULONG NumberOfPfnReferences; // +0x10(0x4)
	ULONG NumberOfMappedViews; // +0x14(0x4)
	USHORT NumberOfSubsections; // +0x18(0x2)
	USHORT FlushInProgressCount; // +0x1a(0x2)
	ULONG NumberOfUserReferences; // +0x1c(0x4)
	ULONG Flags;
	PFILE_OBJECT FilePointer; // +0x24(0x4)
	PVOID WaitingForDeletion; // +0x28(0x4)
	USHORT ModifiedWriteCount; // +0x2c(0x2)
	USHORT NumberOfSystemCacheViews; // +0x2e(0x2)
}CONTROL_AREA,*PCONTROL_AREA;

typedef struct _SEGMENT_OBJECT // 0x30
{
	PVOID BaseAddress; // +0x0(0x4)
	ULONG TotalNumberOfPtes; // +0x4(0x4)
	LARGE_INTEGER SizeOfSegment; // +0x8(0x8)
	ULONG NonExtendedPtes; // +0x10(0x4)
	ULONG ImageCommitment; // +0x14(0x4)
	PCONTROL_AREA ControlArea; // +0x18(0x4)
	PVOID Subsection; // +0x1c(0x4)
	PVOID LargeControlArea; // +0x20(0x4)
	PVOID MmSectionFlags; // +0x24(0x4)
	PVOID MmSubSectionFlags; // +0x28(0x4)
}SEGMENT_OBJECT,*PSEGMENT_OBJECT;

typedef struct _SECTION_OBJECT // 0x18
{
	PVOID StartingVa; // +0x0(0x4)
	PVOID EndingVa; // +0x4(0x4)
	PVOID Parent; // +0x8(0x4)
	PVOID LeftChild; // +0xc(0x4)
	PVOID RightChild; // +0x10(0x4)
	PSEGMENT_OBJECT Segment; // +0x14(0x4)
}SECTION_OBJECT, *PSECTION_OBJECT;

typedef struct _SEGMENT_FLAGS {
	ULONG_PTR TotalNumberOfPtes4132 : 10;
	ULONG_PTR ExtraSharedWowSubsections : 1;
	ULONG_PTR LargePages : 1;
#if defined (_WIN64)
	ULONG_PTR Spare : 52;
#else
	ULONG_PTR Spare : 20;
#endif
} SEGMENT_FLAGS, *PSEGMENT_FLAGS;

typedef struct _SEGMENT {
	PCONTROL_AREA ControlArea;
	ULONG TotalNumberOfPtes;
	ULONG NonExtendedPtes;
	ULONG Spare0;
	//...
} SEGMENT, *PSEGMENT;

typedef struct _MMADDRESS_NODE {
	union {
		LONG_PTR Balance : 2;
		struct _MMADDRESS_NODE *Parent;
	} u1;
	struct _MMADDRESS_NODE *LeftChild;
	struct _MMADDRESS_NODE *RightChild;
	ULONG_PTR StartingVpn;
	ULONG_PTR EndingVpn;
} MMADDRESS_NODE, *PMMADDRESS_NODE;

typedef struct _MMSECTION_FLAGS {
	unsigned BeingDeleted : 1;
	unsigned BeingCreated : 1;
	unsigned BeingPurged : 1;
	unsigned NoModifiedWriting : 1;

	unsigned FailAllIo : 1;
	unsigned Image : 1;
	unsigned Based : 1;
	unsigned File : 1;

	unsigned Networked : 1;
	unsigned NoCache : 1;
	unsigned PhysicalMemory : 1;
	unsigned CopyOnWrite : 1;

	unsigned Reserve : 1;  // not a spare bit!
	unsigned Commit : 1;
	unsigned FloppyMedia : 1;
	unsigned WasPurged : 1;

	unsigned UserReference : 1;
	unsigned GlobalMemory : 1;
	unsigned DeleteOnClose : 1;
	unsigned FilePointerNull : 1;

	unsigned DebugSymbolsLoaded : 1;
	unsigned SetMappedFileIoComplete : 1;
	unsigned CollidedFlush : 1;
	unsigned NoChange : 1;

	unsigned filler0 : 1;
	unsigned ImageMappedInSystemSpace : 1;
	unsigned UserWritable : 1;
	unsigned Accessed : 1;

	unsigned GlobalOnlyPerSession : 1;
	unsigned Rom : 1;
	unsigned WriteCombined : 1;
	unsigned filler : 1;
} MMSECTION_FLAGS;

typedef ULONG MM_PROTECTION_MASK;

typedef struct _SECTION {
	MMADDRESS_NODE Address;
	PSEGMENT Segment;
	LARGE_INTEGER SizeOfSection;
	union {
		ULONG LongFlags;
		MMSECTION_FLAGS Flags;
	} u;
	MM_PROTECTION_MASK InitialPageProtection;
} SECTION, *PSECTION;

struct _KPRCB // 0xc50
{
	USHORT MinorVersion; // +0x0(0x2)
	USHORT MajorVersion; // +0x2(0x2)
	PETHREAD CurrentThread; // +0x4(0x4)
	PETHREAD NextThread; // +0x8(0x4)
	PETHREAD IdleThread; // +0xc(0x4)
	CHAR Number; // +0x10(0x1)
	CHAR Reserved; // +0x11(0x1)
	USHORT BuildType; // +0x12(0x2)
	ULONG SetMember; // +0x14(0x4)
	CHAR CpuType; // +0x18(0x1)
	CHAR CpuID; // +0x19(0x1)
	USHORT CpuStep; // +0x1a(0x2)
}KPRCB,*PKPRCB;

typedef struct _HANDLE_TABLE_ENTRY_INFO {
	ACCESS_MASK AuditMask;
} HANDLE_TABLE_ENTRY_INFO, *PHANDLE_TABLE_ENTRY_INFO;

typedef struct _HANDLE_TABLE_ENTRY {
	union {
		PVOID Object;
		ULONG ObAttributes;
		PHANDLE_TABLE_ENTRY_INFO InfoTable;
		ULONG_PTR Value;
	};
	union {
		union {
			ACCESS_MASK GrantedAccess;
			struct {

				USHORT GrantedAccessIndex;
				USHORT CreatorBackTraceIndex;
			};
		};
		LONG NextFreeTableEntry;
	};
} HANDLE_TABLE_ENTRY, *PHANDLE_TABLE_ENTRY;

typedef struct _AUX_ACCESS_DATA {
	PPRIVILEGE_SET PrivilegesUsed;
	GENERIC_MAPPING GenericMapping;
	ACCESS_MASK AccessesToAudit;
	ACCESS_MASK MaximumAuditMask;
	ULONG Unknown[41];
} AUX_ACCESS_DATA, *PAUX_ACCESS_DATA;

extern POBJECT_TYPE *LpcPortObjectType;
extern POBJECT_TYPE *PsProcessType;

NTKERNELAPI
	NTSTATUS
	SeCreateAccessState(
	PACCESS_STATE AccessState,
	PAUX_ACCESS_DATA AuxData,
	ACCESS_MASK DesiredAccess,
	PGENERIC_MAPPING GenericMapping
	);

NTKERNELAPI
	VOID
	SeDeleteAccessState(
	PACCESS_STATE AccessState
	);

NTKERNELAPI				
	NTSTATUS
	ObCreateObject(
	IN KPROCESSOR_MODE ProbeMode,
	IN POBJECT_TYPE ObjectType,
	IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
	IN KPROCESSOR_MODE OwnershipMode,
	IN OUT PVOID ParseContext OPTIONAL,
	IN ULONG ObjectBodySize,
	IN ULONG PagedPoolCharge,
	IN ULONG NonPagedPoolCharge,
	OUT PVOID *Object
	);

#endif