#include <ntddk.h>

#define dprintf             if (DBG) DbgPrint

#define DEVICE_NAME         L"\\Device\\CmRegMonitor"
#define LINK_NAME           L"\\DosDevices\\CmRegMonitor"
#define LINK_GLOBAL_NAME    L"\\DosDevices\\Global\\CmRegMonitor"