#ifndef _FILE_H_
#define _FILE_H_

#include "ntifs.h"
#include "ntos.h"

extern PSERVICE_DESCRIPTOR_TABLE OriginalServiceDescriptorTable;
int ZwWriteFileIndex;  //写文件

extern BOOL DebugOn;
extern BOOL bDisWriteFile;
extern PEPROCESS ProtectEProcess;

//写文件
typedef NTSTATUS (__stdcall* ZWWRITEFILE)(
	__in      HANDLE FileHandle,
	__in_opt  HANDLE Event,
	__in_opt  PIO_APC_ROUTINE ApcRoutine,
	__in_opt  PVOID ApcContext,
	__out     PIO_STATUS_BLOCK IoStatusBlock,
	__in      PVOID Buffer,
	__in      ULONG Length,
	__in_opt  PLARGE_INTEGER ByteOffset,
	__in_opt  PULONG Key
	);

BOOL SystemCallEntryTableHook(
	PUNICODE_STRING FunctionName,
	int *Index,
	DWORD NewFuctionAddress
	);

NTSTATUS SafeCopyMemory(PVOID SrcAddr, PVOID DstAddr, ULONG Size);

#endif