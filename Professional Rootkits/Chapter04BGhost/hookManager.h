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

// UNDOCUMENTED

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
    IN HANDLE FileHandle OPTIONAL
    );

NTSYSAPI 
NTSTATUS
NTAPI
ZwAllocateVirtualMemory(
  IN HANDLE ProcessHandle,
  IN OUT PVOID *BaseAddress,
  IN ULONG ZeroBits,
  IN OUT PULONG RegionSize,
  IN ULONG AllocationType,
  IN ULONG Protect
  );

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

#endif

