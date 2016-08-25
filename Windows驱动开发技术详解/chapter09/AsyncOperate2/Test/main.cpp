#include <windows.h>
#include <stdio.h>

#define DEVICE_NAME	"test.dat"
#define BUFFER_SIZE	512
//假设该文件大于或等于BUFFER_SIZE

VOID CALLBACK MyFileIOCompletionRoutine(
  DWORD dwErrorCode,                // 对于此次操作返回的状态
  DWORD dwNumberOfBytesTransfered,  // 告诉已经操作了多少字节,也就是在IRP里的Infomation
  LPOVERLAPPED lpOverlapped         // 这个数据结构
)
{
	printf("IO operation end!\n");
}

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

	//初始化overlap使其内部全部为零
	//不用初始化事件!!
	OVERLAPPED overlap={0};

	//这里没有设置OVERLAP参数，因此是异步操作
	ReadFileEx(hDevice, buffer, BUFFER_SIZE,&overlap,MyFileIOCompletionRoutine);

	//做一些其他操作，这些操作会与读设备并行执行

	//进入alterable
	SleepEx(0,TRUE);

	CloseHandle(hDevice);

	return 0;
}

