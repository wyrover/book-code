#ifndef _STARTUP_H_
#define _STARTUP_H_

#include "ntifs.h"
#include <stdio.h>
#include <wchar.h>
#include "ntos.h"

typedef struct _STARTUP_INFORMATION {

	WCHAR lpwzName[256];
	WCHAR lpwzKeyPath[256];
	WCHAR lpwzKeyValue[256];

} STARTUP_INFORMATION, *PSTARTUP_INFORMATION;

typedef struct _STARTUP_INFO {          //InlineHook
	ULONG ulCount;
	STARTUP_INFORMATION Startup[1];
} STARTUP_INFO, *PSTARTUP_INFO;

PSTARTUP_INFO StartupInfo;

NTSTATUS SafeCopyMemory(PVOID SrcAddr, PVOID DstAddr, ULONG Size);

#endif