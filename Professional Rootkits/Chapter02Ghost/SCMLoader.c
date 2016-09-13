// Copyright Ric Vieler, 2006
// This program will load c:\comint32.sys

#include <windows.h>
#include <stdio.h>
#include <process.h>


void main( int argc, char *argv[ ] )
{
	SC_HANDLE sh1;
	SC_HANDLE sh2;
	
	sh1 = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );
	if ( !sh1 )
	{
		printf( "OpenSCManager Failed!\n" );
		return;
	}
	sh2 = CreateService(	sh1,
							"MyDeviceDriver",
							"MyDeviceDriver",
							SERVICE_ALL_ACCESS,
							SERVICE_KERNEL_DRIVER,
							SERVICE_DEMAND_START,
							SERVICE_ERROR_NORMAL,
							"c:\\comint32.sys",
							NULL,
							NULL,
							NULL,
							NULL,
							NULL );
	if ( !sh2 )
	{
		if ( GetLastError() == ERROR_SERVICE_EXISTS )
			printf("DeviceDriver already loaded!\n");
		else
			printf("CreateService Failed!\n");
	}
	else
	{
		printf("\nDriver loaded!\n");
	}
}
