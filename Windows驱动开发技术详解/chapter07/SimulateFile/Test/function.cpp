#include <windows.h>
#include <stdio.h>

#include "function.h"

// DWORD ReadPort(HANDLE hDevice,DWORD port)
// {
// 	DWORD dwOutput;
// 	DWORD dwRead;
// 	DeviceIoControl(hDevice, IOCTL_READ_PORT_ULONG, &port, 4, &dwOutput, 4, &dwRead, NULL);
// 	return dwOutput;
// }
// VOID WritePort(HANDLE hDevice,DWORD port,DWORD value)
// {
// 	PVOID buffer[2];
// 	buffer[0] = (PVOID)port;
// 	buffer[1] = (PVOID)value;
// 	DWORD dwWrite;
// 	DeviceIoControl(hDevice, IOCTL_WRITE_PORT_ULONG, &port, 8, NULL, 0, &dwWrite, NULL);
// }
// VOID TestDriver(HANDLE hDevice)
// {
// 	DWORD dwOutput;
// 	DeviceIoControl(hDevice, IOCTL_TEST, NULL, 0, NULL, 0, &dwOutput, NULL);
// }

