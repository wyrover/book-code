#include <windows.h>
#include <stdio.h>

int main()
{

	HANDLE hDevice = 
		CreateFile("\\\\.\\HelloDDKA",
					GENERIC_READ | GENERIC_WRITE,
					0,		// share mode none
					NULL,	// no security
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL,
					NULL );		// no template

	if (hDevice == INVALID_HANDLE_VALUE)
	{
		printf("Failed to obtain file handle to device "
			"with Win32 error code: %d\n",
			 GetLastError() );
		return 1;
	}

	const int BUFFER_LENGTH = 1024*4;
	UCHAR buffer[BUFFER_LENGTH];//4k
	DWORD dRead;

	ReadFile(hDevice,buffer,BUFFER_LENGTH,&dRead,NULL);
	
	CloseHandle(hDevice);

	return 0;
}