#include <ntddk.h>

#define dprintf             if (DBG) DbgPrint

#define DEVICE_NAME         L"\\Device\\Fuck3SDT"
#define LINK_NAME           L"\\DosDevices\\Fuck3SDT"
#define LINK_GLOBAL_NAME    L"\\DosDevices\\Global\\Fuck3SDT"

#define IOCTL_HOOK      CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS) //In LONG
#define IOCTL_UNHOOK    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS) //In BSTR