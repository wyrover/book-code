#include <windows.h>
#include <stdio.h>

#define BUFFER_SIZE	512
int main()
{
	HANDLE hDevice = 
		CreateFile("test.dat",
					GENERIC_READ | GENERIC_WRITE,
					0,
					NULL,
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL,//此处没有设置FILE_FLAG_OVERLAPPED
					NULL );

	if (hDevice == INVALID_HANDLE_VALUE) 
	{
		printf("Read Error\n");
		return 1;
	}

	UCHAR buffer[BUFFER_SIZE];
	DWORD dwRead;
	ReadFile(hDevice,buffer,BUFFER_SIZE,&dwRead,NULL);//这里没有设置OVERLAP参数

	CloseHandle(hDevice);

	return 0;
}