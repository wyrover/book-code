// HideMe
// Copyright Ric Vieler, 2006
// Send a hide command to MyDeviceDriver
// and wait for operator to stop the process
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "IoManager.h"

void main(int argc, char *argv[])
{
	HANDLE deviceHandle;
	GHOST_IOCTLDATA control = { 0 };
	ULONG status = 0;

	deviceHandle = CreateFile( GHOST_DEVICE_OPEN_NAME,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if( deviceHandle == INVALID_HANDLE_VALUE )
	{
		printf ("Could not find MyDeviceDriver.\n");
	}
	else
	{  
		control.processId = GetCurrentProcessId();

		if( DeviceIoControl(	deviceHandle,
			GHOST_HIDE_COMMAND,
			&control,
			sizeof(control), // input
			(PVOID)&control,
			sizeof(control), // output
			&status,
			NULL ) )
			printf ("MyDeviceDriver hiding this process (0x%x).\n",
				control.processId );
		else
			printf ("DeviceIoControl failed.\n");

		CloseHandle(deviceHandle);
	}
	
	printf ("Press any key to terminate this process..." );
	getch();
	printf ("\n" );
}