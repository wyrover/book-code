// Copyright Ric Vieler, 2004
// Support header for fileManager.c

#ifndef _FILE_MANAGER_H_
#define _FILE_MANAGER_H_

// Though no documentation mentions it, NTFS-ADS works with directories too!
// Each implementation should use a different known directory
// to avoid having the full pathname added to IDS's.
#define MASTER_FILE L"\\??\\C:\\WINDOWS\\Resources"

NTSTATUS GetFile( WCHAR* filename, CHAR* buffer, ULONG buffersize, PULONG fileSizePtr );
NTSTATUS PutFile( WCHAR* filename, CHAR* buffer, ULONG buffersize );

#endif

