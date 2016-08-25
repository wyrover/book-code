#include <windows.h>
#include <stdio.h>
#include <winioctl.h>

#include "function.h"

#include "../MyDriver/guid.h"
#include "../MyDriver/Ioctls.h"

int main()
{
	HANDLE hDevice = GetDeviceViaInterface((LPGUID)&MY_WDM_DEVICE,0);

	if (hDevice == INVALID_HANDLE_VALUE)
	{
		printf("Failed to obtain file handle to device: "
			"%s with Win32 error code: %d\n",
			"MyWDMDevice", GetLastError() );
		return 1;
	}

	DWORD dRet;
	DeviceIoControl(hDevice,GET_PCI_CONFIG,NULL,0,NULL,0,&dRet,NULL);

	CloseHandle(hDevice);
	return 0;
}