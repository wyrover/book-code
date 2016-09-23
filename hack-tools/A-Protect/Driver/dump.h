#ifndef _DUMP_H_
#define _DUMP_H_

#include "ntifs.h"
#include "ntos.h"

NTSTATUS SafeCopyMemory(PVOID SrcAddr, PVOID DstAddr, ULONG Size);
#endif