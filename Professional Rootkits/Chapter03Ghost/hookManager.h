// Copyright Ric Vieler, 2006
// Support header for hookManager.c

#ifndef _HOOK_MANAGER_H_
#define _HOOK_MANAGER_H_

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

#define HOOK_INDEX(function2hook) *(PULONG)((PUCHAR)function2hook+1)

#define HOOK(functionName, newPointer2Function, oldPointer2Function )  \
       oldPointer2Function = (PVOID) InterlockedExchange( (PLONG) &NewSystemCallTable[HOOK_INDEX(functionName)], (LONG) newPointer2Function)

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

NTSTATUS Hook();

#endif

