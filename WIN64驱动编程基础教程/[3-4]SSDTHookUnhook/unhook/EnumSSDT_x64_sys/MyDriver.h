#include <ntddk.h>

#define dprintf             if (DBG) DbgPrint

#define DEVICE_NAME         L"\\Device\\emSSDT64"
#define LINK_NAME           L"\\DosDevices\\emSSDT64"
#define LINK_GLOBAL_NAME    L"\\DosDevices\\Global\\emSSDT64"

#define IOCTL_ClrSSDTHOOK   CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS) //Clear ssdt hook
#define IOCTL_GetKiSrvTab   CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS) //Get KiServiceTable
#define IOCTL_GetFuncAddr   CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS) //Get function address