#ifndef _DELETE_FILE_H_
#define _DELETE_FILE_H_

#include "ntifs.h"
#include "ntos.h"

ULONG ulImageSectionObject;
ULONG ulDataSectionObject;
ULONG ulSharedCacheMap;

BOOLEAN
	SKillDeleteFile(
	IN HANDLE    FileHandle
	);

HANDLE
	SkillIoOpenFile(
	IN PCWSTR FileName,
	IN ACCESS_MASK DesiredAccess,
	IN ULONG ShareAccess
	);
#endif