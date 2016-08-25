#ifndef FUNCTION_H 
#define FUNCTION_H

#include <setupapi.h>
#include <initguid.h>
#include "../SDIO_Driver/Ioctls.h"

HANDLE GetDeviceViaInterface( GUID* pGuid, DWORD instance);

VOID EnableInt(HANDLE hDevice);
VOID DisableInt(HANDLE hDevice);
#endif