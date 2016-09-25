#include <ntddk.h>

#define dprintf             DbgPrint

#define DEVICE_NAME         L"\\Device\\EnumRemoveImageNotify"
#define LINK_NAME           L"\\DosDevices\\EnumRemoveImageNotify"
#define LINK_GLOBAL_NAME    L"\\DosDevices\\Global\\EnumRemoveImageNotify"

//#define IOCTL_ULR3IN  CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS) //In LONG