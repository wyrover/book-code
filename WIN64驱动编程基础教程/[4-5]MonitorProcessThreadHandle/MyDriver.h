#include <ntddk.h>

#define dprintf             DbgPrint

#define DEVICE_NAME         L"\\Device\\MyDriver"
#define LINK_NAME           L"\\DosDevices\\MyDriver"
#define LINK_GLOBAL_NAME    L"\\DosDevices\\Global\\MyDriver"
