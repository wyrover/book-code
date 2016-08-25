#include <windows.h>
#include <stdio.h>

#define GetFilePointer(hFile) SetFilePointer(hFile, 0, NULL, FILE_CURRENT)

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

	UCHAR buffer[10];
	memset(buffer,0xBB,10);
	ULONG ulRead;
	ULONG ulWrite;
	BOOL bRet;
	bRet = WriteFile(hDevice,buffer,10,&ulWrite,NULL);
	if (bRet)
	{
		printf("Write %d bytes\n",ulWrite);
	}

	bRet = ReadFile(hDevice,buffer,10,&ulRead,NULL);
	if (bRet)
	{
		printf("Read %d bytes:",ulRead);
		for (int i=0;i<(int)ulRead;i++)
		{
			printf("%02X ",buffer[i]);
		}

		printf("\n");
	}

	printf("file length:%d\n",GetFileSize(hDevice,NULL));

	CloseHandle(hDevice);

	return 0;
}