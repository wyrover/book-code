#ifndef _LOOKUPKERNELDATA_H_
#define _LOOKUPKERNELDATA_H_

#include "ntifs.h"
#include <stdio.h>
#include <wchar.h>
#include "ntos.h"

typedef struct _KERNEL_DATA {

	ULONG ulAddress;

	ULONG ulStack1;
	ULONG ulStack2;
	ULONG ulStack3;
	ULONG ulStack4;

} KERNEL_DATA, *PKERNEL_DATA;

typedef struct _LOOKUP_KERNEL_DATA {          //KernelData
	ULONG ulCount;
	KERNEL_DATA KernelData[1];
} LOOKUP_KERNEL_DATA, *PLOOKUP_KERNEL_DATA;

PLOOKUP_KERNEL_DATA LookupKernelData;

#endif