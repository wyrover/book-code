// Copyright Ric Vieler, 2006
// Support header for registryManager.c

#ifndef _REGISTRY_MANAGER_H_
#define _REGISTRY_MANAGER_H_

// key data structures
typedef struct _KEY_HANDLE
{
	HANDLE	handle;
	PVOID	keyData;
	struct _KEY_HANDLE *previous;
	struct _KEY_HANDLE *next;
} KEY_HANDLE;

typedef struct _SUBKEY_DATA
{
	ULONG	subkeyIndex;
	ULONG	newIndex;
	struct _SUBKEY_DATA *next;
} SUBKEY_DATA;

typedef struct _REG_KEY_DATA
{
	ULONG	subkeys;
	SUBKEY_DATA* subkeyData;
} REG_KEY_DATA;

// implementation functions
void InitializeKeyTracking();
void FreeKeyTrackingData();
KEY_HANDLE* FindKeyHandle( HANDLE hKey );
void AddNewKeyHandle( KEY_HANDLE* theNewTrack );
ULONG GetSubkeyCount( HANDLE hKey );
void FreeKeyHandle( HANDLE hKey );
KEY_HANDLE* AllocateKeyHandle( HANDLE hKey );
void AddIndices( KEY_HANDLE* pKeyHandle, ULONG index, ULONG newIndex );
SUBKEY_DATA* AdjustNextNewIndex( SUBKEY_DATA* pSubkeyData, int offset );
void AdjustIndices( KEY_HANDLE* pKeyHandle, int hiddenKeys );
ULONG GetNewIndex( HANDLE hKey, ULONG realIndex);
int CreateHiddenKeyIndices( HANDLE hKey );

#endif