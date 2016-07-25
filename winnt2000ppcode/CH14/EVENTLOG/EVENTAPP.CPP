// eventapp.cpp
#include <windows.h>
#include <stdio.h>        // need for sprintf
#include "eventlog.h"
#include "eventapp.h"
#include "eventmsg.h"

// globals
HINSTANCE hInst;
char szMsg[MAXSTR+1]; 
const char *szMsgArray[2];
DISK disk;
BYTE bData[1024];

// prototypes
LRESULT CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

/**----------------------------------------------------------**/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, 
   LPSTR lpCmdLine, int nShow)
{
   // Event Logging not supported under Win32s, check if NT
   if (GetVersion() & 0x80000000) {
      MessageBox(NULL, "This application requires Windows NT",
         "EventLog", MB_OK);
      return FALSE;
   }
   hInst = hInstance;
   return DialogBox(hInst, "IDD_EVENT", NULL, (DLGPROC)DlgProc);
} // WinMain

/**----------------------------------------------------------**/
LRESULT CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, 
   LPARAM lParam)
{
   DWORD dwStatus;
   static EventLog eventLog("EventApp", "EventMsg.dll", TRUE, 
      &dwStatus);
   static WORD wEventType = EVENTLOG_INFORMATION_TYPE;
   
   switch (uMsg) {
      case WM_INITDIALOG:
         SendDlgItemMessage(hDlg, IDC_CUSTOM, EM_LIMITTEXT,
            (WPARAM)MAXSTR, 0L);
         EnableWindow(GetDlgItem(hDlg, IDC_LOGCUSTOM), FALSE);
         CheckRadioButton(hDlg,IDC_ERR, IDC_INFO, IDC_INFO);
         return TRUE;

      case WM_COMMAND:
         switch(LOWORD(wParam)) {
            case IDC_INFO:
               wEventType = EVENTLOG_INFORMATION_TYPE;
               CheckRadioButton(hDlg,IDC_ERR,IDC_INFO,IDC_INFO);
               break;
            case IDC_WARN:
               wEventType = EVENTLOG_WARNING_TYPE;
               CheckRadioButton(hDlg,IDC_ERR,IDC_INFO,IDC_WARN);
               break;
            case IDC_ERR:
               wEventType = EVENTLOG_ERROR_TYPE;
               CheckRadioButton(hDlg,IDC_ERR,IDC_INFO,IDC_ERR);
               break;

            case IDC_CUSTOM:
               if (HIWORD(wParam) == EN_CHANGE) {
                  GetDlgItemText(hDlg,IDC_CUSTOM,szMsg,MAXSTR);
                  if (strlen(szMsg) > 0)
                     EnableWindow(GetDlgItem(hDlg,IDC_LOGCUSTOM),
                        TRUE);
                     else EnableWindow(GetDlgItem(hDlg,
                        IDC_LOGCUSTOM), FALSE);
               }
               return TRUE;

            case IDC_LOGCUSTOM:
               GetDlgItemText(hDlg, IDC_CUSTOM, szMsg, MAXSTR);
               eventLog.WriteCustom(szMsg, wEventType);
               break;

            case IDC_LOGERR:
               szMsgArray[0] = "somefile.txt";
               eventLog.Write(MSG_CANTFINDFILE, 
                  EVENTLOG_ERROR_TYPE, 1, szMsgArray, 0, NULL);
               break;

            case IDC_LOGWARN:
               // include free disk space info as event data
               DWORD dwSPC, dwBPS, dwFC, dwTC;
               GetDiskFreeSpace("c:",&dwSPC,&dwBPS,&dwFC,&dwTC);
               disk.dwSPC = dwSPC; 
               disk.dwBPS = dwBPS; 
               disk.dwFC = dwFC;
               disk.dwTC = dwTC;
               disk.dwFree = disk.dwFC * disk.dwSPC * disk.dwBPS;
               szMsgArray[0] = "c:"; 
               eventLog.Write(MSG_LOWDISKSPACE, 
                  EVENTLOG_WARNING_TYPE, 1, szMsgArray, 
                  sizeof(DISK), (LPBYTE)&disk);
               break;

            case IDC_LOGINFO:
               szMsgArray[0] = "somedll.dll"; 
               eventLog.Write(MSG_LOADSUCCESS, 
                  EVENTLOG_INFORMATION_TYPE, 1, szMsgArray, 0, 
                  NULL);
               break;

            case IDC_READ:
               DWORD dwTotal, dwOldest, dwRec;
               EVENTLOGRECORD *pRec;
               pRec = (EVENTLOGRECORD *)&bData;

               // query first to see how many records there are
               eventLog.Query(&dwTotal, &dwOldest);
               for (dwRec = 1; dwRec <= dwTotal; dwRec++) {
                  eventLog.Read(dwRec, pRec, sizeof(bData));
                  sprintf(szMsg, 
                     "Record = %u, EventID = %u, Source = %s",
                     pRec->RecordNumber, pRec->EventID, (LPSTR)
                     ((LPBYTE)pRec + sizeof(EVENTLOGRECORD)));
                   MessageBox(hDlg, szMsg, "Read Event", MB_OK);
               }
               break;

            case IDC_BACKUP:
               eventLog.Backup("back0001.evt");
               break;
            case IDC_CLEAR:
               eventLog.Clear(NULL);
               break;
               
            case IDOK:
               EndDialog(hDlg, TRUE);
               return TRUE;
         } // switch on WM_COMMAND
   } // switch on uMsg
   return FALSE;
} // DlgProc
