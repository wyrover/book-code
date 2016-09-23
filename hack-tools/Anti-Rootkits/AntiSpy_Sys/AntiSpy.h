#ifndef CXX_ANTISPY_H
#define CXX_ANTISPY_H

#include <ntifs.h>
#include "IOControlCode.h"
#include "SSDT.h"
#include "File.h"
#include "ProcessManager.h"
#include "ProcessThread.h"
#include "ProcessHandle.h"
#include "ProcessModule.h"
#include "ProcessMemory.h"


VOID      DriverUnload(PDRIVER_OBJECT DriverObject);
NTSTATUS  DispatchFunction(PDEVICE_OBJECT  DeviceObject,PIRP Irp);
NTSTATUS  DispatchContorRoutine(PDEVICE_OBJECT  DeviceObject,PIRP Irp);
VOID      Sleep(LONG MSec);

#endif