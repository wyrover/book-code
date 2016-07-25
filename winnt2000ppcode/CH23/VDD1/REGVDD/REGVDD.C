// REGVDD.C - Sample Virtual Device Driver for Windows NT
//
#include <windows.h>
#include <vddsvc.h>
#include "regvdd.h"

// Function Prototypes
BOOL LibMain(HINSTANCE hInst, ULONG ulReason, LPVOID lpResv);
VOID RegVDDRegisterInit(VOID);
VOID RegVDDTerminateVDM(VOID);
VOID RegVDDDispatch(VOID);

/**----------------------------------------------------------**/
BOOL LibMain(HINSTANCE hInst, ULONG ulReason, LPVOID lpResv)
{
   return TRUE;
} // LibMain

/**----------------------------------------------------------**/
VOID RegVDDRegisterInit(VOID)
{
   setCF(0);           // Clear flags to indicate success
   return;
} // RegVDDRegisterInit 

/**----------------------------------------------------------**/
VOID RegVDDTerminateVDM(VOID)
{
   return;
} // RegVDDTerminateVDM

/**----------------------------------------------------------**/
//   Client Parameters: 
//      Client ES:BX = REGVDD_INFO data structure
//      Client CX = sizeof data structure passed in ES:BX
//      Client DX = 0 if ES is a segment, 1 if ES is a selector
//   Returns:
//      If no Error, Client Carry is clear and dwStatus = 0
//      If Error, Client Carry is set (dwStatus !=0)  
/**----------------------------------------------------------**/
VOID RegVDDDispatch(VOID)
{
   USHORT usSize, usSel;
   REGVDD_INFO * pData;
   ULONG ulAddr;

   HKEY hKey;
   DWORD dwData;
   char szBase[128], szKey[128];
   int i;

   // get data passed in by client
   usSize = getCX();
   usSel = getDX();
   ulAddr = (ULONG)MAKELONG(getBX(), getES());
   pData = (REGVDD_INFO *)GetVDMPointer(ulAddr, usSize, usSel);
   
   // if don't have valid REGVDD_INFO structure, set carry
   if (pData == NULL) {
      setCF(1);
      return;
   }

   pData->dwStatus = TRUE;

   switch (pData->dwCmd)
   {
      case REGVDD_CMD_CREATESUBKEY:
         // if exists already, it will be opened
         if (RegCreateKeyEx((HKEY)pData->dwRootKey, 
            pData->szSubkey, 0, "\0", pData->dwVolatility,
            (REGSAM)pData->dwAccess, NULL, &hKey, &dwData)
            == ERROR_SUCCESS) 
               RegCloseKey(hKey);
         else pData->dwStatus = FALSE; 
         break;

      case REGVDD_CMD_DELETESUBKEY:
         // split szSubkey into base and key values
         strcpy(szBase, pData->szSubkey);
         for (i=strlen(szBase); i>=0 && szBase[i] != '\\'; i--);
         strcpy(szKey, &(szBase[i+1]));
         szBase[i] = '\0';

         if (RegOpenKeyEx((HKEY)pData->dwRootKey, szBase, 0, 
            (REGSAM)pData->dwAccess, &hKey) == ERROR_SUCCESS) 
         {
            if (!(RegDeleteKey(hKey, szKey) == ERROR_SUCCESS))
               pData->dwStatus = FALSE;
            RegCloseKey(hKey);
         }
         else pData->dwStatus = FALSE;
         break;

      case REGVDD_CMD_WRITEVALUE:
         if (RegCreateKeyEx((HKEY)pData->dwRootKey, 
            pData->szSubkey, 0, "\0", pData->dwVolatility,
            (REGSAM)pData->dwAccess, NULL, &hKey, &dwData) == 
            ERROR_SUCCESS) 
         {
            if (!(RegSetValueEx(hKey, pData->szValueName, 0, 
               pData->dwValueType, (BYTE *)(pData->szValueData),
               pData->dwValueDataSize) == ERROR_SUCCESS))
                  pData->dwStatus = FALSE;
            RegCloseKey(hKey);
         }
         else pData->dwStatus = FALSE;
         break;

      case REGVDD_CMD_READVALUE:
         if (RegOpenKeyEx((HKEY)pData->dwRootKey, 
            pData->szSubkey, 0, (REGSAM)pData->dwAccess, 
            &hKey) == ERROR_SUCCESS) 
         {
            if (!(RegQueryValueEx(hKey, pData->szValueName, 0, 
               &(pData->dwValueType), (LPBYTE)(pData->szValueData),
               &(pData->dwValueDataSize)) == ERROR_SUCCESS))
                  pData->dwStatus = FALSE;
            RegCloseKey(hKey);
         }
         else pData->dwStatus = FALSE;
         break;

      case REGVDD_CMD_DELETEVALUE:
         if (RegOpenKeyEx((HKEY)pData->dwRootKey, 
            pData->szSubkey, 0, (REGSAM)pData->dwAccess, 
            &hKey) == ERROR_SUCCESS) 
         {
            if (!(RegDeleteValue(hKey, pData->szValueName)
               == ERROR_SUCCESS)) pData->dwStatus = FALSE;
            RegCloseKey(hKey);
         }
         else pData->dwStatus = FALSE;
         break;
   }

   // Clear carry if success, set if error
   if (pData->dwStatus == FALSE) setCF(0);
   else setCF(1);

   // Deallocate VDM Pointer
   FreeVDMPointer(ulAddr, (ULONG)usSize, (PBYTE)pData, usSel);
   return;
} // RegVDDDispatch

// End of File
