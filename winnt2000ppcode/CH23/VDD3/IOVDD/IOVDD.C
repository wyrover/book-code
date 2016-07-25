// IOVDD.C - Demonstate port and memory-mapped I/O

#include <windows.h>
#include <vddsvc.h>
#include <winioctl.h>
#include "iovdd.h"

// Function Prototypes
BOOL VDDLibMain(HINSTANCE, ULONG, LPVOID);
VOID IoVDD_ReadByte(WORD wPort, PBYTE byteData);
VOID IoVDD_WriteByte(WORD wPort, BYTE byteData);
VOID IoVDD_Memory(ULONG ulAddress, ULONG RWFlags);
VOID Debug(LPSTR szDebug);

// Globals
HANDLE hVDD = NULL;
VDD_IO_PORTRANGE portRange;
ULONG ulAddr;
PBYTE pMem;
BYTE byteStatus = 0;
BYTE byteCount = 0;
BOOLEAN bIoHooked = FALSE;
BOOLEAN bMemHooked= FALSE;
BOOLEAN bMemAllocated = FALSE;
BOOLEAN bDebug = TRUE;

/**---------------------------------------------**/
BOOL VDDLibMain(HINSTANCE hInst, ULONG ulReason, 
   LPVOID lpRsrv)
{
   VDD_IO_HANDLERS ioHandlers;
   hVDD = hInst;
   
   switch(ulReason)
   {
      case DLL_PROCESS_ATTACH:
         Debug("IOVDD: VDDLibMain - Attach\n");

         portRange.First = IOVDD_PORT_START;
         portRange.Last  = IOVDD_PORT_END;

         ioHandlers.inb_handler   = IoVDD_ReadByte;
         ioHandlers.inw_handler   = NULL;
         ioHandlers.insb_handler  = NULL;
         ioHandlers.insw_handler  = NULL;
         ioHandlers.outb_handler  = IoVDD_WriteByte;
         ioHandlers.outw_handler  = NULL;
         ioHandlers.outsb_handler = NULL;
         ioHandlers.outsw_handler = NULL;

         if (!(bIoHooked = VDDInstallIOHook(hVDD,
            1, &portRange, &ioHandlers))) 
         {
            Debug("IOVDD: Port hook failed");
            return FALSE;
         }

         ulAddr = (ULONG)MAKELONG(0, 0xCC00);
         pMem = (PBYTE)GetVDMPointer(ulAddr, 
            IOVDD_MIO_RANGE, 0);
         if (!(bMemHooked = VDDInstallMemoryHook(
            hVDD, pMem, IOVDD_MIO_RANGE, 
            (PVDD_MEMORY_HANDLER)IoVDD_Memory)))
         {
            Debug("IOVDD: Memory hook failed");
            return FALSE;
         }
         break;

      case DLL_PROCESS_DETACH:
         Debug("IOVDD: VDDLibMain - Detach\n");
         if (bIoHooked)
            VDDDeInstallIOHook(hVDD, 1, &portRange);
         if (bMemHooked)
            VDDDeInstallMemoryHook(hVDD, pMem,
               IOVDD_MIO_RANGE);
         if (bMemAllocated)
            VDDFreeMem(hVDD, (PVOID)pMem, 
               IOVDD_MIO_RANGE);
         if (pMem) 
            FreeVDMPointer(ulAddr, IOVDD_MIO_RANGE,
               pMem, FALSE);
         break;

      default: break;
   }

   return TRUE;
} // VDDLibMain

/**---------------------------------------------**/
VOID IoVDD_ReadByte(WORD wPort, PBYTE byteData)
{
   HANDLE hDevice=NULL;
   DWORD dwCount=0L;

   // handle ports that apps reads from
   switch(wPort)
   {
      case IOVDD_PORT_IDENTIFY:
         Debug("IOVDD: IOVDD_PORT_IDENTIFY\n");
         hDevice = CreateFile((LPTSTR)"\\\\.\\simpldrv",
            GENERIC_READ | GENERIC_WRITE, 0, NULL, 
            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
         DeviceIoControl(hDevice, (DWORD)IOCTL_SIMPLDRV_HELLO,
            NULL, 0, byteData, 1, &dwCount, NULL);
         CloseHandle(hDevice);
         break;

      case IOVDD_PORT_STATUS:
         Debug("IOVDD: IOVDD_PORT_STATUS\n");
         *byteData = byteStatus;
         break; 

      case IOVDD_PORT_COUNT:
         Debug("IOVDD: IOVDD_PORT_COUNT\n");
         *byteData = byteCount;
         break;

      default: 
         Debug("IOVDD: Unsupported port read\n");
         break;
   }

   return;
} // IoVDD_ReadByte

/**---------------------------------------------**/
VOID IoVDD_WriteByte(WORD wPort, BYTE byteData)
{
   HANDLE hDevice=NULL;
   DWORD dwCount=0L;

   // handle ports that apps write to
   switch(wPort)
   {
      case IOVDD_PORT_READ:
         Debug("IOVDD: IOVDD_PORT_READ\n");
         byteStatus = FALSE;
         // request kernel-mode driver to read,
         // when read completes, set status
         if (byteCount > 35) byteCount = 35;
         memcpy(pMem, 
            "This would be data read from device",
            byteCount);
         byteStatus = TRUE;
         break;
                  
      case IOVDD_PORT_WRITE:
         Debug("IOVDD: IOVDD_PORT_WRITE\n");
         byteStatus = FALSE;
         // request kernel-mode driver to write,
         // when write request completes, set status
         byteStatus = TRUE;
         break;

      case IOVDD_PORT_COUNT:
         Debug("IOVDD: IOVDD_PORT_COUNT\n");
         // this is how many bytes to write
         byteCount = byteData;
         if (byteCount > IOVDD_MIO_RANGE)
            byteCount = IOVDD_MIO_RANGE;
         break;

      default:
         Debug("IOVDD: Unsupported port write\n");
         break;
   }

   return;
} // IoVDD_WriteByte

/**---------------------------------------------**/
VOID IoVDD_Memory(ULONG ulAddress, ULONG RWFlags)
{
   Debug("IOVDD: IoVDD_Memory\n");
   bMemAllocated = VDDAllocMem(hVDD, pMem, 
      IOVDD_MIO_RANGE);
   return;
} // IoVDD_Memory

/**---------------------------------------------**/
VOID Debug(LPSTR szDebug)
{
   if (bDebug) OutputDebugString(szDebug);
} // Debug

// end of file
