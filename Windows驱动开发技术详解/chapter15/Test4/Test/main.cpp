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

	DWORD dwOutput ;
	DWORD inputBuffer[3] =
	{
		0x378,//对0x378进行操作
		0,//输出字节0
		1//1代表8位操作，2代表16位操作，4代表32位操作
	};

	//类似于Out_8((PUCHAR)0x378,0);
	DeviceIoControl(hDevice, WRITE_PORT, inputBuffer, sizeof(inputBuffer), NULL, 0, &dwOutput, NULL);

	CloseHandle(hDevice);

	return 0;
}