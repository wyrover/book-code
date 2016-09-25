#include <ntddk.h>

#define dprintf             DbgPrint
#define DEVICE_NAME         L"\\Device\\monitor_create_process_x64"
#define LINK_NAME           L"\\DosDevices\\monitor_create_process_x64"
#define LINK_GLOBAL_NAME    L"\\DosDevices\\Global\\monitor_create_process_x64"