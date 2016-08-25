#include <windows.h>
#include <stdio.h>

#define BUFFER_SIZE	512
//假设该文件大于或等于BUFFER_SIZE

#define DEVICE_NAME	"test.dat"
int main()
{
	HANDLE hDevice = 
		CreateFile("test.dat",
					GENERIC_READ | GENERIC_WRITE,
					0,
					NULL,
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,//此处设置FILE_FLAG_OVERLAPPED
					NULL );

	if (hDevice == INVALID_HANDLE_VALUE) 
	{
		printf("Read Error\n");
		return 1;
	}

	UCHAR buffer[BUFFER_SIZE];
	DWORD dwRead;

	//初始化overlap使其内部全部为零
	OVERLAPPED overlap={0};

	//创建overlap事件
	overlap.hEvent = CreateEvent(NULL,FALSE,FALSE,NULL);

	//这里没有设置OVERLAP参数，因此是异步操作
	ReadFile(hDevice,buffer,BUFFER_SIZE,&dwRead,&overlap);

	//做一些其他操作，这些操作会与读设备并行执行

	//等待读设备结束
	WaitForSingleObject(overlap.hEvent,INFINITE);

	CloseHandle(hDevice);

	return 0;
}

