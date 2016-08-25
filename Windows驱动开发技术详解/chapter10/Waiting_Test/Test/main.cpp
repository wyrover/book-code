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
			"MyDDKDevice", GetLastError() );
		return 1;
	}

	DWORD dwOutput;

	DWORD dwMicroSecond = 1000;
	DeviceIoControl(hDevice, IOCTL_WAIT_METHOD1, &dwMicroSecond, sizeof(DWORD), NULL, 0, &dwOutput, NULL);
	DeviceIoControl(hDevice, IOCTL_WAIT_METHOD2, &dwMicroSecond, sizeof(DWORD), NULL, 0, &dwOutput, NULL);
	DeviceIoControl(hDevice, IOCTL_WAIT_METHOD3, &dwMicroSecond, sizeof(DWORD), NULL, 0, &dwOutput, NULL);
	DeviceIoControl(hDevice, IOCTL_WAIT_METHOD4, &dwMicroSecond, sizeof(DWORD), NULL, 0, &dwOutput, NULL);

	CloseHandle(hDevice);

	return 0;
}