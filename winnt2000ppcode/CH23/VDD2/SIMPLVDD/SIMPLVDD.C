// SIMPLEVDD.C - VDD that routes DOSDRV16 to SIMPLDRV

#include <windows.h>
#include <vddsvc.h>
#include <winioctl.h>
#include "simplvdd.h"

// Function Prototypes
BOOL LibMain(HINSTANCE, ULONG, LPVOID);
VOID VDDInit(VOID);
VOID VDDTerminateVDM(VOID);
VOID VDDDispatch(VOID);

// Globals
HANDLE hDevice=NULL;

/**----------------------------------------------------------**/
BOOL LibMain(HINSTANCE hInst, ULONG ulReason, LPVOID lpRsrv)
{
   return TRUE;
} // LibMain

/**----------------------------------------------------------**/
VOID VDDInit(VOID)
{
   setCF(0);           // Clear flags to indicate success
   return;
} // VDDInit

/**----------------------------------------------------------**/
VOID VDDTerminateVDM(VOID)
{
   return;
} // VDDTerminateVDM

/**---------------------------------------------------------
** VDDDispatch - called when DOSDRV16.SYS sends a request.
** Arguments:
**    Client (DX) = Command code
**    Client (CX) = Buffer size
**    Client (ES:BX) = Request Header
** Returns: 
**    (CX) = Count transfered
**    (DI) = status
**-------------------------------------------------------**/
VOID VDDDispatch(VOID)
{
   PCHAR pBuffer;
   USHORT cb;
   ULONG ulAddr;
   DWORD bytes=0L;

   hDevice = CreateFile((LPTSTR)"\\\\.\\simpldrv",
      GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL, NULL);

   cb = getCX();
   ulAddr = (ULONG)MAKELONG(getBX(), getES());
   pBuffer = (PCHAR)GetVDMPointer(ulAddr, (ULONG)cb, FALSE);
   
   switch (getDX())     // command code is in dx 
   {
      case CMD_READ:
         ReadFile(hDevice, pBuffer, (ULONG)cb, &bytes, 0);
         setDI(STAT_OK);
         setCX((USHORT)bytes);
         break;

      case CMD_WRITE:
      case CMD_WRITE_VFY:
         WriteFile(hDevice, pBuffer, (ULONG)cb, &bytes, 0);
         setDI(STAT_OK);
         setCX((USHORT)bytes);
         break;

      case CMD_IN_IOCTL:
         if (DeviceIoControl(hDevice, IOCTL_SIMPLDRV_HELLO, 
            NULL, 0, pBuffer, cb, &bytes, NULL)) 
               setDI(STAT_OK);
         else setDI(STAT_GF);
         break;

      default:
         setDI(STAT_CE);        // unsupported command
         break;
   }

   FreeVDMPointer(ulAddr, (ULONG)cb, (PCHAR)pBuffer, FALSE);
   CloseHandle(hDevice);
   return;
} // VDDDispatch

// end of file
