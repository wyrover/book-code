// fileManager
// Copyright Ric Vieler, 2004
// Use without path to get/put Alternate Data Streams from/to MASTER_FILE
// Use with full path to get/put regular files from/to the visible file system

#include "ntddk.h"
#include <stdio.h>
#include "fileManager.h"
#include "Ghost.h"


NTSTATUS GetFile( WCHAR* filename, CHAR* buffer, ULONG buffersize, PULONG fileSizePtr )
{
	NTSTATUS			rc;
	WCHAR			ADSName[256];
	HANDLE			hStream;
	OBJECT_ATTRIBUTES	ObjectAttr;
	UNICODE_STRING		FileName;
	IO_STATUS_BLOCK	ioStatusBlock;
	CHAR				string[256];

	// set file size
	*fileSizePtr = 0;	
	// Get from NTFS-ADS if not full path
	if( wcschr( filename, '\\' ) == NULL )
		_snwprintf( ADSName, 255, L"%s:%s", MASTER_FILE, filename );
	else
		wcscpy( ADSName, filename );

	RtlInitUnicodeString( &FileName, ADSName );
	InitializeObjectAttributes( &ObjectAttr,
					&FileName,
					OBJ_CASE_INSENSITIVE,
					NULL,
					NULL);
	
	rc = ZwOpenFile(
		&hStream,
		SYNCHRONIZE | GENERIC_ALL,
		&ObjectAttr,
		&ioStatusBlock,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		FILE_SYNCHRONOUS_IO_NONALERT );

	if ( rc != STATUS_SUCCESS )
	{
		DbgPrint( "comint32: GetFile() ZwOpenFile() failed.\n" );
		_snprintf( string, 255, "comint32: rc = %0x, status = %0x\n",
rc,
ioStatusBlock.Status );
		DbgPrint( string );
		return( STATUS_UNSUCCESSFUL );
	}

	rc = ZwReadFile(
		hStream,
		NULL,
		NULL,
		NULL,
		&ioStatusBlock,
		buffer,
		buffersize,
		NULL,
		NULL );

	if ( rc != STATUS_SUCCESS )
	{
		DbgPrint( "comint32: GetFile() ZwReadFile() failed.\n" );
		_snprintf( string, 255, "comint32: rc = %0x, status = %0x\n",
rc,
ioStatusBlock.Status );
		DbgPrint( string );
		return( STATUS_UNSUCCESSFUL );
	}

	// Read was successfull, return the number of bytes read
	*fileSizePtr = ioStatusBlock.Information;	
	ZwClose( hStream );
	return( STATUS_SUCCESS );
}


NTSTATUS PutFile( WCHAR* filename, CHAR* buffer, ULONG buffersize )
{
	NTSTATUS			rc;
	WCHAR				ADSName[256];
	HANDLE				hStream;
	OBJECT_ATTRIBUTES	ObjectAttr;
	UNICODE_STRING		FileName;
	IO_STATUS_BLOCK		ioStatusBlock;
	CHAR				string[256];

	// Put to NTFS-ADS if not full path
	if( wcschr( filename, '\\' ) == NULL )
		_snwprintf( ADSName, 255, L"%s:%s", MASTER_FILE, filename );
	else
		wcscpy( ADSName, filename );

	RtlInitUnicodeString( &FileName, ADSName );
	InitializeObjectAttributes( &ObjectAttr,
								&FileName,
								OBJ_CASE_INSENSITIVE,
								NULL,
								NULL);
	
	rc = ZwCreateFile(
		&hStream,
		SYNCHRONIZE | GENERIC_ALL,
		&ObjectAttr,
		&ioStatusBlock,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		FILE_OVERWRITE_IF,
		FILE_SYNCHRONOUS_IO_NONALERT,
		NULL,
		0);

	if ( rc != STATUS_SUCCESS )
	{
		DbgPrint( "comint32: PutFile() ZwCreateFile() failed.\n" );
		_snprintf( string, 255, "comint32: rc = %0x, status = %0x\n", rc, ioStatusBlock.Status );
		DbgPrint( string );
		return( STATUS_UNSUCCESSFUL );
	}

	rc = ZwWriteFile(
		hStream,
		NULL,
		NULL,
		NULL,
		&ioStatusBlock,
		buffer,
		buffersize,
		NULL,
		NULL );

	if ( rc != STATUS_SUCCESS )
	{
		DbgPrint( "comint32: PutFile() ZwWriteFile() failed.\n" );
		_snprintf( string, 255, "comint32: rc = %0x, status = %0x\n", rc, ioStatusBlock.Status );
		DbgPrint( string );
		ZwClose( hStream );
		return( STATUS_UNSUCCESSFUL );
	}

	ZwClose( hStream );
	return( STATUS_SUCCESS );
}
