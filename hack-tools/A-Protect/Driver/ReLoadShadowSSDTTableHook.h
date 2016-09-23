#ifndef __RELOAD_SHADOWSSDT_HOOK_H_
#define __RELOAD_SHADOWSSDT_HOOK_H_

#include "ntifs.h"
#include "InitWindowsVersion.h"

extern CHAR XPProcName[][40];
extern CHAR Win2003ProcName[][50];
extern CHAR Win7ProcName[][50];

BOOL MmIsAddressValidEx(
	IN PVOID Pointer
	);

#endif