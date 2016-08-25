#include <windows.h>
#include <stdio.h>
//使用CTL_CODE必须加入winioctl.h
#include <winioctl.h>
#include "..\NT_Driver\Ioctls.h"

int main()
{
	HANDLE hDevice = 
		CreateFile("\\\\.\\HelloDDK",
					GENERIC_READ | GENERIC_WRITE,
					0,		// share mode none
					NULL,	// no security
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL,
					NULL );		// no template

	if (hDevice == INVALID_HANDLE_VALUE)
	{
		printf("Failed to obtain file handle to device: "
			"%s with Win32 error code: %d\n",
			"MyWDMDevice", GetLastError() );
		return 1;
	}

	DWORD dwOutput;
	DWORD dwMircoSeconds = 1000*1000*2;

	DeviceIoControl(hDevice, IOCTL_START_TIMER, &dwMircoSeconds, sizeof(DWORD), NULL, 0, &dwOutput, NULL);

	Sleep(10000);

 	DeviceIoControl(hDevice, IOCTL_STOP_TIMER, NULL, 0, NULL, 0, &dwOutput, NULL);

	CloseHandle(hDevice);

	return 0;
}