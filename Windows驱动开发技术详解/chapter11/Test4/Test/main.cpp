#include <windows.h>
#include <stdio.h>

int main()
{

	HANDLE hDevice = 
		CreateFile("\\\\.\\HelloDDKB",
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

	DWORD dRet;
	ReadFile(hDevice,NULL,0,&dRet,NULL);

	CloseHandle(hDevice);

	return 0;
}