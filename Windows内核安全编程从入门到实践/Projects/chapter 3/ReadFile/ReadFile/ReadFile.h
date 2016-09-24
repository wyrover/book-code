#ifndef _READFILE_H_
#define _READFILE_H_

#include "ntddk.h"

#define INVALID_HANDLE_VALUE (HANDLE)(0xffffffff)

void ReadFileUnload(IN PDRIVER_OBJECT DriverObject);

HANDLE KGetTargetFileHandle(PUNICODE_STRING FileName);

NTSTATUS ReadIoCompletion(PDEVICE_OBJECT DeviceObject,
                          PIRP Irp,
                          PVOID Context);

#endif