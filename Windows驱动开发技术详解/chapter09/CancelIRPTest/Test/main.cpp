#include <windows.h>
#include <stdio.h>

int main()
{
	HANDLE hDevice = 
		CreateFile("\\\\.\\HelloDDK",
					GENERIC_READ | GENERIC_WRITE,
					0,
					NULL,
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,//此处设置FILE_FLAG_OVERLAPPED
					NULL );

	if (hDevice == INVALID_HANDLE_VALUE)
	{
		printf("Open Device failed!");
		return 1;
	}

	OVERLAPPED overlap1={0};
	OVERLAPPED overlap2={0};

	UCHAR buffer[10];
	ULONG ulRead;
	
	BOOL bRead = ReadFile(hDevice,buffer,10,&ulRead,&overlap1);
	if (!bRead && GetLastError()==ERROR_IO_PENDING)
	{
		printf("The operation is pending\n");
	}
	bRead = ReadFile(hDevice,buffer,10,&ulRead,&overlap2);
	if (!bRead && GetLastError()==ERROR_IO_PENDING)
	{
		printf("The operation is pending\n");
	}
	
	//迫使程序中止2秒
	Sleep(2000);

	//显式的调用CancelIo，其实在关闭设备时会自动运行CancelIo
	CancelIo(hDevice);

	//创建IRP_MJ_CLEANUP IRP
	CloseHandle(hDevice);

	return 0;
}