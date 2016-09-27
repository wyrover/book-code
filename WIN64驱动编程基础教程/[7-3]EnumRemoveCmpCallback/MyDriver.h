#include <ntddk.h>

#define dprintf             DbgPrint

#define DEVICE_NAME         L"\\Device\\EnumRemoveCmpCallback"
#define LINK_NAME           L"\\DosDevices\\EnumRemoveCmpCallback"
#define LINK_GLOBAL_NAME    L"\\DosDevices\\Global\\EnumRemoveCmpCallback"

//#define IOCTL_ULR3IN  CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS) //In LONG