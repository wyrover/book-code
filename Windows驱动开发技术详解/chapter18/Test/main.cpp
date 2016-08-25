#include <windows.h>
#include <stdio.h>
#include <winioctl.h>

#include "function.h"

#include "../SDIO_Driver/guid.h"
#define BUFFER_SIZE 0x7c00
UCHAR buffer[BUFFER_SIZE];

int main()
{
	HANDLE hDevice = GetDeviceViaInterface((LPGUID)&GUID_DEVINTERFACE_SDIO_DEVICE,0);

	if (hDevice == INVALID_HANDLE_VALUE)
	{
		printf("Failed to obtain file handle to device: "
			"%s with Win32 error code: %d\n",
			"MyWDMDevice", GetLastError() );
		return 1;
	}

	EnableInt(hDevice);

	Sleep(2000);

 	DWORD dwRead;
 	ReadFile(hDevice,buffer,BUFFER_SIZE,&dwRead,NULL);
	printf("read %x\n",dwRead);

	DisableInt(hDevice);

	CloseHandle(hDevice);

	return 0;

}