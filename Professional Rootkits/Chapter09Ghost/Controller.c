// Controller
// Copyright Ric Vieler, 2006
// Send an on/off command to MyDeviceDriver
#include <windows.h>
#include <stdio.h>
#include <io.h>
#include "IoManager.h"

void main(int argc, char *argv[])
{
  HANDLE deviceHandle;
  GHOST_IOCTLDATA control = { 0 };
  ULONG status = 0;

  if(( argc < 2 ) || ((stricmp(argv[1],"on") != 0)) && ((stricmp(argv[1],"off") != 0)))
  {
    printf ("Use Controller on\n");
    printf ("or  Controller off\n");
    return;
  }

  deviceHandle = CreateFile( GHOST_DEVICE_OPEN_NAME,
      GENERIC_READ | GENERIC_WRITE,
      0,
      NULL,
      OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL,
      NULL);

  if (deviceHandle == INVALID_HANDLE_VALUE)
  {
    printf ("Could not find MyDeviceDriver.\n");
    return;
  }
  
  if(stricmp(argv[1],"on") == 0)
	  control.command = GHOST_ON;
  else
	  control.command = GHOST_OFF;

  if( DeviceIoControl(	deviceHandle,
      GHOST_ON_OFF_COMMAND,
      &control,
      sizeof(control), // input
      (PVOID)&control,
      sizeof(control), // output
      &status,
      NULL ) )
    printf ("MyDeviceDriver %s.\n", control.command == GHOST_ON ? "on" : "off" );
  else
    printf ("DeviceIoControl failed.\n");
 
  CloseHandle(deviceHandle);
}