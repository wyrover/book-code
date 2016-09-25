#include <ntddk.h>

#define dprintf             DbgPrint

#define DEVICE_NAME         L"\\Device\\ClrKIH64"
#define LINK_NAME           L"\\DosDevices\\ClrKIH64"
#define LINK_GLOBAL_NAME    L"\\DosDevices\\Global\\ClrKIH64"

#define IOCTL_GET_CUR_CODE  CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS) //get current code
#define IOCTL_SET_ORI_CODE  CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS) //clear inline hook