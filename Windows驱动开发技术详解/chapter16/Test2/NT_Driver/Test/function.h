#ifndef FUNCTION_H 
#define FUNCTION_H

//#include "../MyDriver/Ioctls.h"

DWORD ReadPort(HANDLE hDevice,DWORD port);
VOID WritePort(HANDLE hDevice,DWORD port,DWORD value);
VOID TestDriver(HANDLE hDevice);
#endif