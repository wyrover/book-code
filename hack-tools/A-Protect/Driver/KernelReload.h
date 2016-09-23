#include "ntifs.h"
#include "FileSystem.h"
#include "InitWindowsVersion.h"
#include "fixrelocation.h"

extern POBJECT_TYPE *IoDriverObjectType;

#define SystemModuleInformation 11

extern BOOL DebugOn;

NTSTATUS 
	ObReferenceObjectByName ( 
	IN PUNICODE_STRING ObjectName, 
	IN ULONG Attributes, 
	IN PACCESS_STATE AccessState OPTIONAL, 
	IN ACCESS_MASK DesiredAccess OPTIONAL, 
	IN POBJECT_TYPE ObjectType, 
	IN KPROCESSOR_MODE AccessMode, 
	IN OUT PVOID ParseContext OPTIONAL, 
	OUT PVOID *Object 
	) ;


NTSTATUS ZwQuerySystemInformation(
	__in       ULONG SystemInformationClass,
	__inout    PVOID SystemInformation,
	__in       ULONG SystemInformationLength,
	__out_opt  PULONG ReturnLength
	);

BOOL
	FixBaseRelocTable (
	PVOID NewImageBase,
	DWORD ExistImageBase
	);

BOOL MmIsAddressValidEx(
	IN PVOID Pointer
	);