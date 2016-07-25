// EVENTLOG.CPP - Methods for EventLog class
#include <windows.h>
#include "eventlog.h"

#define EVENT_APP  "SYSTEM\\CurrentControlSet\\Services\
\\EventLog\\Application\\"

/**----------------------------------------------------------**/
EventLog::EventLog(char *szSourceName, char *szMessageDLL,
   BOOL bClearOnExit, DWORD *dwStatus)
{
   HKEY hKey;
   DWORD dwData;
   
   bClearReg = bClearOnExit;
   strcpy(szSource, szSourceName);

   // just try creating it, if it exists, it will be opened
   strcpy(szRegKey, EVENT_APP);
   strcat(szRegKey, szSource);
   if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, szRegKey, 0, "\0",
      REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | KEY_WRITE, 
      NULL, &hKey, &dwData) == ERROR_SUCCESS) {

      // add the message file value
      strcpy(szRegKey, "%SystemRoot%\\System32\\");
      strcat(szRegKey, szMessageDLL);
      RegSetValueEx(hKey, "EventMessageFile", 0, REG_EXPAND_SZ,
         (LPBYTE)szRegKey, strlen(szRegKey)+1);

      // add the supported types value
      dwData = EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | 
         EVENTLOG_INFORMATION_TYPE;
      RegSetValueEx(hKey, "TypesSupported", 0, REG_DWORD, 
         (LPBYTE)&dwData, sizeof(DWORD));
      RegCloseKey(hKey);
      *dwStatus = TRUE;   
   } 
   else *dwStatus = FALSE;
} // EventLog::EventLog

/**----------------------------------------------------------**/
EventLog::~EventLog()
{
   HKEY hKey;

   if (bClearReg) {
      // caller specified for registry entry to be removed
      if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, EVENT_APP, 0, 
         KEY_WRITE, &hKey) == ERROR_SUCCESS) {
            RegDeleteKey(hKey, szSource); 
            RegCloseKey(hKey);
      }
   }
} // EventLog::~EventLog

/**----------------------------------------------------------**/
DWORD EventLog::Backup(LPTSTR szBackup)
{
   HANDLE hSource;

   if ((hSource = OpenEventLog(NULL, szSource)) != NULL) { 
      BackupEventLog(hSource, (LPCTSTR)szBackup);
      CloseEventLog(hSource);
      return TRUE;
   } else return FALSE;
} // EventLog::Backup

/**----------------------------------------------------------**/
DWORD EventLog::Clear(LPTSTR szBackup)
{
   HANDLE hSource;

   if ((hSource = OpenEventLog(NULL, szSource)) != NULL) { 
      ClearEventLog(hSource, (LPCTSTR)szBackup);
      CloseEventLog(hSource);
      return TRUE;
   } else return FALSE;
} // EventLog::Clear

/**----------------------------------------------------------**/
DWORD EventLog::Query(DWORD *dwNumRecords, DWORD *dwOldestRecord)
{
   HANDLE hSource;

   if ((hSource = OpenEventLog(NULL, szSource)) != NULL) { 
      GetNumberOfEventLogRecords(hSource, dwNumRecords);
      GetOldestEventLogRecord(hSource, dwOldestRecord);
      CloseEventLog(hSource);
      return TRUE;
   } else return FALSE;
} // EventLog::Query

/**----------------------------------------------------------**/
DWORD EventLog::Read(DWORD dwRec, EVENTLOGRECORD *pData, 
   DWORD dwSize)
{
   HANDLE hSource;
   DWORD dwRead, dwRequired, dwStatus = TRUE;

   if ((hSource = OpenEventLog(NULL, szSource)) != NULL)
      return FALSE;
   if (!ReadEventLog(hSource, EVENTLOG_FORWARDS_READ | 
      EVENTLOG_SEEK_READ, dwRec, pData, dwSize, &dwRead, 
      &dwRequired)) dwStatus = dwRequired;
   CloseEventLog(hSource);
   return dwStatus;      
} // EventLog::Read

/**----------------------------------------------------------**/
DWORD EventLog::Write(DWORD dwEventID, WORD wEventType, 
   WORD wStrings, LPCTSTR *pszStrings, DWORD dwData, 
   LPBYTE lpData)
{
   HANDLE hSource;

   if ((hSource = RegisterEventSource(NULL, szSource)) != NULL) {
      ReportEvent(hSource, wEventType, 0, dwEventID, NULL, 
         wStrings, dwData, pszStrings, lpData);
      DeregisterEventSource(hSource);
      return TRUE;
   } else return FALSE;      
} // EventLog::Write

/**----------------------------------------------------------**/
DWORD EventLog::WriteCustom(LPTSTR pString, WORD wEventType)
{
   HANDLE hSource;

   if ((hSource = RegisterEventSource(NULL, szSource)) != NULL) {
      ReportEvent(hSource, wEventType, 0, 0, NULL, 1, 0, 
         (LPCTSTR *)&pString, NULL);
      DeregisterEventSource(hSource);
      return TRUE;
   } else return FALSE;      
} // EventLog::WriteCustom
