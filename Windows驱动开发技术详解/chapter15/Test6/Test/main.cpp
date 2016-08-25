#include <windows.h>
#include <stdio.h>
//使用CTL_CODE必须加入winioctl.h
#include <winioctl.h>
#include "..\NT_Driver\Ioctls.h"

UCHAR In_8 (PUCHAR Port)
{
   UCHAR Value;
	__asm
	{
		mov edx, Port
		in al, dx
		mov Value, al
		//插入几个空指令
		nop
		nop
	}

   return(Value);
}

USHORT In_16 (PUSHORT Port)
{
   USHORT Value;

	__asm
	{
		mov edx, Port
		in ax, dx
		mov Value, ax
		//插入几个空指令
		nop
		nop
	}
   return(Value);
}

ULONG In_32 (PULONG Port)
{
   ULONG Value;
	__asm
	{
		mov edx, Port
		in eax, dx
		mov Value, eax
		//插入几个空指令
		nop
		nop
	}
   return(Value);
}

void Out_32(PULONG Port,ULONG Value)
{
	__asm
	{
		mov edx, Port
		mov eax, Value
		out dx,eax
		//插入几个空指令
		nop
		nop
	}
}
void Out_16 (PUSHORT Port,USHORT Value)
{
	__asm
	{
		mov edx, Port
		mov ax, Value
		out dx,ax
		//插入几个空指令
		nop
		nop
	}
}

void Out_8 (PUCHAR Port,UCHAR Value)
{
	__asm
	{
		mov edx, Port
		mov al, Value
		out dx,al
		//插入几个空指令
		nop
		nop
	}
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

	DWORD dwOutput ;
	//开启直接IO
	DeviceIoControl(hDevice, IOCTL_ENABLEDIRECTIO, NULL,0, NULL, 0, &dwOutput, NULL);

	Out_8((PUCHAR)0x378,0);

	//关闭直接IO
	DeviceIoControl(hDevice, IOCTL_DISABLEDIRECTIO, NULL,0, NULL, 0, &dwOutput, NULL);

	CloseHandle(hDevice);

	return 0;
}