/**************************************************************************\
* install.c -- install c_*.nls conversion tables.
*
*         Steve Firebaugh
*         Microsoft Developer Support
*         Copyright (c) 1992-1997 Microsoft Corporation
*
*
*
* Note, this module must have UNICODE defined because the registry
*  code will not work without it.
*
\**************************************************************************/
#define UNICODE

#include <windows.h>
#include <commdlg.h>
#include <string.h>
#include <stdio.h>
#include "uconvert.h"
#include "install.h"

/**************************************************************************\
*  Global variables.
\**************************************************************************/

/* This is the registry key that we store conversion table information under. */
TCHAR NlsRegEntryStr[]=TEXT("SYSTEM\\CurrentControlSet\\Control\\Nls\\CodePage");


/***************************************************************************\
*    FUNCTION: InstallTableProc
*
* Dialog window procedure for the Install *.nls tables dialog.
*
\***************************************************************************/
LRESULT CALLBACK InstallTableProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{


  switch (message) {

    /**********************************************************************\
    *  WM_INITDIALOG
    *
    * Fill dialog with a list of the currently existing tables.
    \**********************************************************************/
    case WM_INITDIALOG:
      if (!ListInstalledTables (GetDlgItem (hwnd, DID_LISTBOX), LB_ADDSTRING, FALSE))
        EndDialog (hwnd, FALSE);
        return TRUE;
    break;

    case WM_COMMAND:
      switch (wParam) {
        case IDCANCEL:
        case IDOK:
          EndDialog (hwnd, TRUE);
        break;


        /**********************************************************************\
        *  WM_COMMAND, BID_ADD
        *
        * Use common dialog, get new *.nls name, and try to install it.
        \**********************************************************************/
        case BID_ADD:
          if (GetTableFileNames(hwnd))
            ListInstalledTables (GetDlgItem (hwnd, DID_LISTBOX), LB_ADDSTRING, FALSE);
        break;
      }
    break; /* end WM_COMMAND */


    case WM_SYSCOMMAND:
      if (wParam == SC_CLOSE)
        EndDialog (hwnd, TRUE);
    break; /* end WM_SYSCOMMAND */


  } /* end switch */
  return FALSE;
}



/***************************************************************************\
*    FUNCTION: GetTableFileNames
*
* Throw up a common dialog to the user, and let them search for the *.nls
*  file to install.
*
* LIMITATION:  Currently only works for one file at a time.
*  Should rewrite to accept multiple files.
*
\***************************************************************************/
int GetTableFileNames (HWND hwnd)
{
    OPENFILENAME OpenFileName;


    /* buffers for the file names. */
    TCHAR szFile[MAX_PATH],szFileTitle[MAX_PATH];
    TCHAR szFilter[MAX_PATH], buffer[50];
    TCHAR *p;

    /* Build up the correct filter strings for OPENFILENAME structure */

    p = szFilter;
    lstrcpy (buffer,LoadResourceString(IDS_FILE_FILTER_SPEC4));
    lstrcpy (p,buffer);
    p += lstrlen (buffer) +1;
    lstrcpy (buffer,TEXT("*.nls"));
    lstrcpy (p,buffer);
    p += lstrlen (buffer) +1;

    lstrcpy (p,TEXT("\0"));


    wsprintf (szFile, TEXT(""));
    wsprintf (szFileTitle, TEXT(""));

    OpenFileName.lStructSize       = sizeof(OPENFILENAME);
    OpenFileName.hwndOwner         = hwnd;
    OpenFileName.hInstance         = NULL;
    OpenFileName.lpstrFilter       = szFilter;
    OpenFileName.lpstrCustomFilter = NULL;
    OpenFileName.nMaxCustFilter    = 0L;
    OpenFileName.nFilterIndex      = 1L;
    OpenFileName.lpstrFile         = szFile;
    OpenFileName.nMaxFile          = MAX_PATH;
    OpenFileName.lpstrFileTitle    = szFileTitle;
    OpenFileName.nMaxFileTitle     = MAX_PATH;
    OpenFileName.lpstrInitialDir   = NULL;
    OpenFileName.lpstrTitle        = LoadResourceString(IDS_TABLE_FILE_TITLE);

    OpenFileName.nFileOffset       = 0;
    OpenFileName.nFileExtension    = 0;
    OpenFileName.lpstrDefExt       = NULL;

    OpenFileName.lCustData         = 0;
    OpenFileName.lpfnHook          = NULL;
    OpenFileName.lpTemplateName    = NULL;

//    OpenFileName.Flags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT;
    OpenFileName.Flags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST ;

    if (!GetOpenFileName(&OpenFileName)) return 0;


    return (InstallFile (szFile,szFileTitle));
}




/***************************************************************************\
*    FUNCTION: InstallFile
*
* Given full path name, and just file name, copy that file into the
*  system directory, and change the registry to indicate that the file
*  has now been "installed."
*
\***************************************************************************/
int InstallFile (TCHAR* szPathAndName, TCHAR* szName)
{
TCHAR szTargetFile[MAX_PATH], buffer[MAX_PATH];
TCHAR keyname[MAX_PATH];
HKEY hKey;
int cp, nChar;
LONG rValue;

    /* First verify that they have selected a valid file name. */
    CharLowerBuff (szName,lstrlen (szName));
    if (myScanf (szName, &cp) != 1) {
      MessageBox (NULL, LoadResourceString(IDS_INCORRECT_FILE_TYPE),
                        NULL, MB_ICONSTOP | MB_OK);
      return FALSE;
    }




    /* Build up a complete path name for the target file.
     *  Get the system directory, and prepend it before szName.
     */
    GetSystemDirectory (buffer, MAX_PATH);
    nChar = wsprintf (szTargetFile, TEXT("%s\\%s"), buffer, szName);

    if (nChar >= MAX_PATH) {
      MessageBox (NULL, LoadResourceString(IDS_FILENAME_OVERFLOW),NULL, MB_ICONSTOP | MB_OK);
      return FALSE;
    }



    /* Now, try to open the registry for writing... This may fail if
     *  the current user has insufficient privilege, or it may fail
     *  for other, unforeseen, reasons.
     */
    rValue = RegOpenKeyEx (HKEY_LOCAL_MACHINE, NlsRegEntryStr, 0, KEY_SET_VALUE, &hKey);
    if (rValue == ERROR_ACCESS_DENIED) {
      MessageBox (NULL, LoadResourceString(IDS_LOGON_AS_ADMIN),
               LoadResourceString(IDS_ACCESS_DENIED), MB_ICONSTOP | MB_OK);
      return FALSE;
    }
    if (rValue != ERROR_SUCCESS) {
      MessageBox (NULL, LoadResourceString(IDS_REGOPENKEYEX_FAILED),NULL, MB_ICONSTOP | MB_OK);
      return FALSE;
    }


    /* Try to copy file... one reason for failure is file already
     *  exists.  If so, query the user to try again.
     *  If fails again, just report problem and exit.
     */
    if (!CopyFile (szPathAndName, szTargetFile, TRUE)) {

      /* if failure was from existing file, query user preference
       *  regarding replacing it.
       */
      if (GetLastError() == ERROR_FILE_EXISTS) {
        if (MessageBox (NULL, LoadResourceString(IDS_FILE_ALREADY_EXISTS),
                        LoadResourceString(IDS_APP_WARNING),
                        MB_ICONEXCLAMATION | MB_YESNO) == IDNO) {
          goto close_and_exit;
        } else {
          if (!CopyFile (szPathAndName, szTargetFile, FALSE)) {
            MessageBox (NULL, LoadResourceString(IDS_FILE_CP_FAILED_AGAIN),
                          NULL, MB_ICONSTOP | MB_OK);
            goto close_and_exit;
          }
        }

      /* no duplicate file, CopyFile() failed for other reasons
       *  report failure and return.
       */
      } else {
        MessageBox (NULL, LoadResourceString(IDS_FILE_CP_FAILED),
                      NULL, MB_ICONSTOP | MB_OK);
        goto close_and_exit;

      }
    }


    /* Finally, write the new key value to the registry. */
    if (myScanf (szName, &cp) == 1) {
      wsprintf (keyname, TEXT("%d"), cp);
      RegSetValueEx (hKey, keyname, 0, REG_SZ, (LPBYTE)szName,
                     (DWORD)((lstrlen(szName) +1)*sizeof(TCHAR)));

    } else
      MessageBox (NULL, szName, LoadResourceString(IDS_FILE_PARSE_FAILED),
              MB_ICONSTOP | MB_OK);


close_and_exit:

    RegCloseKey (hKey);

    return TRUE;
}


/***************************************************************************\
*    FUNCTION: ListInstalledTables
*
* Display the *.nls conversion tables currently installed, according to the
*  registry.  Display either the file name, or just the codepage number.
*
* hwndFill - listbox or combobox to fill with names.
* message - LB_ADDSTRING or CB_ADDSTRING
* NumberOnly - FALSE then use full file name, TRUE then just use number.
*
* CRITERION for table being installed:
*  value in registry is c_* where * is number.
*
\***************************************************************************/
int ListInstalledTables (HWND hwndFill, UINT message, int NumberOnly)
{

  TCHAR szKeyname[MAX_PATH], szValue[MAX_PATH];
  DWORD cBytesName, cBytesValue, iSubKey;
  int  cp;
  HKEY hKey;

  /* open the registry key for reading.  If failure, report and exit. */
  if (RegOpenKeyEx (HKEY_LOCAL_MACHINE, NlsRegEntryStr, 0, KEY_QUERY_VALUE, &hKey)) {
    MessageBox (NULL, LoadResourceString(IDS_REGOPENKEYEX_FAILED),
            NULL, MB_ICONSTOP | MB_OK);
    return FALSE;
  }


  /* empty the current contents. */
  if (message == LB_ADDSTRING)
    SendMessage (hwndFill, LB_RESETCONTENT, 0, 0);
  else if (message == CB_ADDSTRING)
    SendMessage (hwndFill, CB_RESETCONTENT, 0, 0);


  iSubKey = 0;
  cBytesName = cBytesValue = MAX_PATH;
  while (!RegEnumValue (hKey, iSubKey, szKeyname, &cBytesName, NULL, NULL, (LPBYTE)szValue, &cBytesValue)) {

    if (myScanf (szValue, &cp) == 1) {

      /* if we are to display only the number, then reformat szValue string */
      if (NumberOnly)
        wsprintf (szValue, TEXT("%d"), cp);


      SendMessage (hwndFill, message, 0 ,(LPARAM) szValue);
    }

    iSubKey++;
    cBytesName = cBytesValue = MAX_PATH;  // undoc.ed feature, must be set each time.
  }

  RegCloseKey (hKey);

  return TRUE;
}








/***************************************************************************\
*    FUNCTION: myScanf
*
* Convert a string into a number (like sscanf).
*  However, this function works independent of UNICODE turned on.
*
* NOT a general function... looking for "c_%d.nls"
*
\***************************************************************************/
int myScanf (TCHAR* pSource, int* pValue)
{
char ansibuffer[MAX_PATH];
int iStrLen;

  iStrLen = lstrlen (pSource);
  if (iStrLen == 0) return 0;

#ifdef UNICODE
  WideCharToMultiByte (CP_ACP, 0, pSource, -1,
           ansibuffer, MAX_PATH, NULL, NULL);
#else
  lstrcpy (ansibuffer, pSource);
#endif

  CharLowerBuffA (ansibuffer,lstrlenA (ansibuffer));

  return (sscanf (ansibuffer, "c_%d.nls", pValue)); // leave off TEXT()
}
