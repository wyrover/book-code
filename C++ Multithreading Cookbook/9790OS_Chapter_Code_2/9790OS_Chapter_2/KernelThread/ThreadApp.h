#include <windows.h>
#include <tchar.h>

#define DRIVER_NAME         TEXT( "TestDriver.sys" )
#define DRIVER_SERVICE_NAME TEXT( "TestDriver" )
#define Message(n)          MessageBox(0, TEXT(n), TEXT("Test Driver Info"), 0)

BOOL StartDriver(LPTSTR szCurrentDriver);
BOOL StopDriver(void);
