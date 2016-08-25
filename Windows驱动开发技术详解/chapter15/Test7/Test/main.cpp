#include <windows.h>
#include <stdio.h>
//使用CTL_CODE必须加入winioctl.h
#include <winioctl.h>
#include "..\NT_Driver\Ioctls.h"


UCHAR In_8(HANDLE hDevice,USHORT port)
{
	DWORD dwOutput ;
	DWORD inputBuffer[2] =
	{
		port,//对port进行操作
		1//1代表8位操作，2代表16位操作，4代表32位操作
	};
	DWORD dResult;

	DeviceIoControl(hDevice, READ_PORT, inputBuffer, sizeof(inputBuffer), &dResult, sizeof(DWORD), &dwOutput, NULL);

	return (UCHAR) dResult;
	
}
void Out_8(HANDLE hDevice,USHORT port,UCHAR value)
{
	DWORD dwOutput ;
	DWORD inputBuffer[3] =
	{
		port,//对port进行操作
		1,//1代表8位操作，2代表16位操作，4代表32位操作
		value//输出字节
	};

	DeviceIoControl(hDevice, WRITE_PORT, inputBuffer, sizeof(inputBuffer), NULL, 0, &dwOutput, NULL);
}

//发音程序,参数f代表频率
void Sound(HANDLE hDevice,int f)
{  
	//计数为1193180/F
	USHORT   B=1193180/f; 

	//从端口0x61取数  
	UCHAR temp = In_8(hDevice,0x61);
	//两低位置1 
	temp = temp | 3; 
	//输出到0x61端口
	Out_8(hDevice,0x61,temp);

	//输出到0x61端口
	Out_8(hDevice,0x43,0xB6);
	//输出到0x42端口，写低8位
	Out_8(hDevice,0x42,B&0xF);
	//输出到0x42端口，写高8位
	Out_8(hDevice,0x42,(B>>8)&0xF);
}

// 关闭声音  
void SoundOff(HANDLE hDevice)                               
{
	//取端口0x61的字节  
	UCHAR value = In_8(hDevice,0x61);

	//强制置最后两位为0 
	value = value & 0xFC;

	//返送端口0x61  
	Out_8(hDevice,0x61,value);
}

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

	//产生2KHz频率的声音
	Sound(hDevice,2000);
	//持续200毫秒
	Sleep(200);
	SoundOff(hDevice);

	CloseHandle(hDevice);

	return 0;
}