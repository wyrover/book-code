#ifndef FUNCTION_H 
#define FUNCTION_H

#include <setupapi.h>
#include <initguid.h>
#include "../MyDriver/Ioctls.h"

HANDLE GetDeviceViaInterface( GUID* pGuid, DWORD instance);

//Read
ULONG ReadFromBase0(HANDLE handle,ULONG Offset,UCHAR *buff,ULONG length);
ULONG ReadFromBase2(HANDLE handle,ULONG Offset,UCHAR *buff,ULONG length);

UCHAR ReadByteFromBase0(HANDLE handle,ULONG Offset);
UCHAR ReadByteFromBase2(HANDLE handle,ULONG Offset);

//Write
ULONG WriteToBase0(HANDLE handle,ULONG Offset,UCHAR *buff,ULONG length);
ULONG WriteToBase2(HANDLE handle,ULONG Offset,UCHAR *buff,ULONG length);

BOOL WriteByteToBase0(HANDLE handle,ULONG Offset,UCHAR input);
BOOL WriteByteToBase2(HANDLE handle,ULONG Offset,UCHAR input);
BOOL WriteDwordToBase0(HANDLE handle,ULONG Offset,DWORD input);
BOOL WriteDwordToBase2(HANDLE handle,ULONG Offset,DWORD input);

//Other
ULONG ReadImage(HANDLE handle,UCHAR *buff);
ULONG WriteImage(HANDLE handle,UCHAR *buff);

BOOL EnableInt(HANDLE handle);
BOOL DisableInt(HANDLE handle);

#endif