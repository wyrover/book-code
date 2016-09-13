// Copyright Ric Vieler, 2006
// This program will unload c:\comint32.sys

#include <windows.h>
#include <stdio.h>
#include <process.h>


void main( int argc, char *argv[ ] )
{
	SC_HANDLE sh1;
	SC_HANDLE sh2;
	SERVICE_STATUS ss;
	
	sh1 = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );
	if ( !sh1 )
	{
		printf( "OpenSCManager Failed!\n" );
		return;
	}
	sh2 = OpenService(	sh1,
						"MyDeviceDriver",
						SERVICE_ALL_ACCESS );
	if ( !sh2 )
	{
			printf("OpenService Failed!\n");
			CloseServiceHandle( sh1 );
			exit(1);
	}
	ControlService( sh2, SERVICE_CONTROL_STOP, &ss );
	if ( !DeleteService( sh2 ) )
		printf("Could not unload MyDeviceDriver!\n");
	else
		printf("Unloaded MyDeviceDriver.\n");
	CloseServiceHandle( sh2 );
	CloseServiceHandle( sh1 );
}
