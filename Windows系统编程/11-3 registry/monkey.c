/*************************************************************************\
*
*  PROGRAM: Monkey: the Registry Monkey Utility.
*  PURPOSE: To demonstrate Registry API.
*  COMMENTS:
*
\*************************************************************************/


#define STRICT
#include <windows.h>
#include <string.h>
#define _MBCS
#include <mbstring.h>
#include <stdlib.h>
#include <stdio.h>
#include "monkey.h"


HANDLE hInst;
HWND   hDlg;

HANDLE hHeap;


/*************************************************************************\
*
*  FUNCTION: WinMain(HANDLE, HANDLE, LPSTR, int)
*
*  PURPOSE: Creates the dialogbox.
*
*  COMMENTS:
*
\*************************************************************************/

int APIENTRY WinMain (HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPSTR     lpCmdLine,
                      int       nCmdShow)


{
  DWORD retCode;

  UNREFERENCED_PARAMETER( nCmdShow );
  UNREFERENCED_PARAMETER( lpCmdLine );
  UNREFERENCED_PARAMETER( hPrevInstance );

  hInst   = hInstance;
  hHeap   = HeapCreate (0, 0, 0);

  retCode = DialogBox ((HANDLE)hInst, (LPCSTR)"MonkeyDlg",
                        NULL, (DLGPROC)MonkeyDlgProc);

  HeapDestroy (hHeap);
  return  (retCode);

}

/************************************************************************\
*
*  FUNCTION: MonkeyDlgProc();
*
*  PURPOSE:  Handle the Monkey dialog box messages.
*
*  MESSAGES:
*
*    WM_INITDIALOG  - Posts WM_GETFIRSTKEY message.
*
*    WM_GETFIRSTKEY - Puts the first 4 pre-defined keys in the listbox.
*
*    IDL_LISTBOX    - Trapped when an item in the left hand listbox
*                     has been double clicked.  It posts a IDB_NEXT message.
*
*    IDL_LISTBOX2   - Trapped when an item in the right hand listbox has
*                     been double clicked.  It basically calls DisplayKeyData,
*                     which fills the Value edit fields with the data from
*                     the current key's specified value information.
*
*    IDB_PRINT      - Basically calls PrintTree() which does a recursive
*                     print of the Registry from the current key to the
*                     end of it's branches.
*
*    IDB_BACK       - Sets the dialog box with the information from the
*                     previously selected key (one closer to the root of
*                     the registry, the parent of the current key).
*
*    IDB_NEXT       - Sets the dialog box with the information on the
*                     selected key child.
*
*    IDR_FULL       - Sets a global variable used to determine if the
*                     user wants to print full Registry information
*                     or only information from keys that have value
*                     associated with it.  Variable is set to TRUE.
*
*    IDR_TRIMMED    - Same as above, only the variable is set to FALSE.
*
\************************************************************************/

int APIENTRY MonkeyDlgProc (HWND hDlg, WORD wMsg, LONG wParam, LONG lParam)
{
   ULONG    KeyClassLength = 256;
   ULONG    KeyNameLength = 256;
   DWORD    indexLB;
   CHAR     *putNullAt;
   CHAR     lpBuffer1[128];
   CHAR     lpBuffer2[128];

   static   CHAR     RegPath[MAX_PATH]  = "";
   static   CHAR     NameLBSelect[256]  = "";
   static   HKEY     hKeyRoot;
   static   DWORD    RegLevel;
   static   BOOL     FullBranches = TRUE;

   static   HANDLE   hFile = INVALID_HANDLE_VALUE;
   static   HANDLE   hBootIni;

  UNREFERENCED_PARAMETER( lParam );

  switch (wMsg)
  {
      case WM_INITDIALOG:
        // Post a message to get the first 4 pre-defined keys, and set
        // Full Branches to be the print default.
        PostMessage (hDlg, WM_GETFIRSTKEY, 0, 0);
        CheckDlgButton (hDlg, IDR_FULL, TRUE);
        return (0);

      case WM_GETFIRSTKEY:
        // Initialize by putting the first 4 predefined keys of the
        // registry in the list box.
        SendMessage (GetDlgItem(hDlg, IDL_LISTBOX),
                     LB_ADDSTRING, 0, (LONG)"HKEY_LOCAL_MACHINE");

        SendMessage (GetDlgItem(hDlg, IDL_LISTBOX),
                     LB_ADDSTRING, 0, (LONG)"HKEY_CURRENT_USER");

        SendMessage (GetDlgItem(hDlg, IDL_LISTBOX),
                     LB_ADDSTRING, 0, (LONG)"HKEY_USERS");

        SendMessage (GetDlgItem(hDlg, IDL_LISTBOX),
                     LB_ADDSTRING, 0, (LONG)"HKEY_CLASSES_ROOT");

        hKeyRoot = 0;                   // Initialize hKeyRoot.
        return (0);

      case WM_SYSCOMMAND:
        if (wParam == SC_CLOSE)
        {
          EndDialog (hDlg, TRUE);
          if (hFile != INVALID_HANDLE_VALUE)
            CloseHandle (hFile);
          return (TRUE);
        }
        break;

      case WM_COMMAND:

        switch (LOWORD(wParam))
        {
            case IDR_FULL:
              // If Full Branches pressed, set global var to TRUE.
              FullBranches = TRUE;
              return (0);

            case IDR_TRIMMED:
              // If Trimmed Branches pressed, set global var to FALSE.
              FullBranches = FALSE;
              return (0);

            case IDL_LISTBOX:
               // If double click in left hand listbox, clear Value
               // edit fields, and execute Next functionality.
               if ( HIWORD (wParam) == LBN_DBLCLK)
               {
                 SetDlgItemText (hDlg, IDE_VALUE1, "");
                 SetDlgItemText (hDlg, IDE_VALUE2, "");
                 PostMessage (hDlg, WM_COMMAND, IDB_NEXT, 0);
               }
               return (0);

            case IDL_LISTBOX2:
                 // If double click right hand listbox, clear Value edit
                 // fields, then display the key's data.
               if ( HIWORD (wParam) == LBN_DBLCLK)
                {
                 SetDlgItemText (hDlg, IDE_VALUE1, "");
                 SetDlgItemText (hDlg, IDE_VALUE2, "");
                 DisplayKeyData (hDlg, RegPath, hKeyRoot);
                }
               return (0);


            case IDB_NEXT:
                 // Get the index of the cursor selection
                 // in the list box.
               indexLB = SendMessage (GetDlgItem (hDlg, IDL_LISTBOX),
                                      LB_GETCURSEL, 0, 0);

               // If nothing is selected, flag user and return, otherwise
               // process the selected key.
               // LB_ERR indicates nothing selected.
               if (indexLB == LB_ERR)
                 {
                 LoadString(hInst, IDS_SELECTMSG, lpBuffer1, sizeof(lpBuffer1));
                 LoadString(hInst, IDS_NAME, lpBuffer2, sizeof(lpBuffer2));
                 MessageBox (hDlg, lpBuffer1, lpBuffer2, MB_OK);
                 return (0);
                 }

               // If listbox item 0 is pressed, user wants to move
               // back up.  Execute the Back functionality.
               if (indexLB == 0 && hKeyRoot)
                {
                PostMessage (hDlg, WM_COMMAND, IDB_BACK, 0);
                return (0);
                }


               // Get text from selection in LB.
               SendMessage (GetDlgItem (hDlg, IDL_LISTBOX),
                            LB_GETTEXT, indexLB, (LPARAM)NameLBSelect);

               // Put name of chosen item in Name field.
               SetDlgItemText (hDlg, IDE_NAME, NameLBSelect);

                                       // Then clear ListBox entries.
               SendMessage (GetDlgItem (hDlg, IDL_LISTBOX),
                            LB_RESETCONTENT, 0, 0);
               SendMessage (GetDlgItem (hDlg, IDL_LISTBOX2),
                            LB_RESETCONTENT, 0, 0);

               EnumerateLevel (hDlg, NameLBSelect, RegPath, &hKeyRoot);

               return (0);



            case IDB_BACK:

                 // For this case (hRootKey = 0)you're at the top level already.
                 // Tell the user, then return
               if (!hKeyRoot)
                 {
                 LoadString(hInst, IDS_TOPLEVEL, lpBuffer1, sizeof(lpBuffer1));
                 LoadString(hInst, IDS_NAME, lpBuffer2, sizeof(lpBuffer2));
                 MessageBox (hDlg, lpBuffer1, lpBuffer2, MB_OK);
                 return (0);
                 }

               //For all remaining cases, clear the listboxes.
               SendMessage (GetDlgItem (hDlg, IDL_LISTBOX),
                            LB_RESETCONTENT, 0, 0);
               SendMessage (GetDlgItem (hDlg, IDL_LISTBOX2),
                            LB_RESETCONTENT, 0, 0);



               // If hRootKey has a value, but the pathname is blank,
               // then you must be 1 level deep, reset to level 0 by
               // posting WM_GETFIRSTKEY.
               if (strcmp (RegPath, "") == 0)
                 {
                 SetDlgItemText (hDlg, IDE_NAME, "");
                 PostMessage (hDlg, WM_GETFIRSTKEY, 0, 0);
                 return (0);
                 }

               // Two cases left.  One in which the path has only one
               // key name in it, and no back slash character (meaning
               // strrchr() will return NULL); and one the other case
               // where there are more than one key name in the path (
               // and at least one back slash for strrchr().  If this
               // is the first case, we want to fakeout EnumerateLevel
               // into thinking we just picked one of the pre-defined keys,
               // and then re-enumerate it's child keys.
               if ((putNullAt = _mbsrchr (RegPath, '\\')) == NULL)
                 {
                 RegPath[0] = '\0';

                 switch ((DWORD)hKeyRoot)
                   {
                   case (DWORD)HKEY_LOCAL_MACHINE:
                     strcpy (NameLBSelect, "HKEY_LOCAL_MACHINE");
                     break;

                   case (DWORD)HKEY_USERS:
                     strcpy (NameLBSelect, "HKEY_USERS");
                     break;

                   case (DWORD)HKEY_CURRENT_USER:
                     strcpy (NameLBSelect, "HKEY_CURRENT_USER");
                     break;

                   case (DWORD)HKEY_CLASSES_ROOT:
                     strcpy (NameLBSelect, "HKEY_CLASSES_ROOT");
                     break;
                   }
                 SetDlgItemText (hDlg, IDE_NAME, NameLBSelect);
                 hKeyRoot = 0;
                 EnumerateLevel (hDlg, NameLBSelect, RegPath, &hKeyRoot);
                 }
               else
                 {
                 // In the final case, we can just trim the last key
                 // name off the path, and re-enumerate the level.
                 *putNullAt = '\0';
                 putNullAt = _mbsrchr (RegPath, '\\');

                 if (putNullAt)
                   {
                   strcpy (NameLBSelect, putNullAt+1);
                   *putNullAt = '\0';
                   }
                 else
                   {
                   strcpy (NameLBSelect, RegPath);
                   *RegPath = '\0';
                   }
                 SetDlgItemText (hDlg, IDE_NAME, NameLBSelect);
                 EnumerateLevel (hDlg, NameLBSelect, RegPath, &hKeyRoot);
                 }
               return (0);

            default:
               return (0);

          }

     }
    return (FALSE);

  }




/************************************************************************\
*
*  FUNCTION: EnumerateLevel();
*
*  PURPOSE: To get a valid key handle (either to determine if the one sent
*           to the function was one of the pre-defined, or to open a key
*           specified by the path), and to pass that key handle along
*           to QueryKey().
*
*           To enumerate the children of a key, you must have
*           an open handle to it.  The four top keys of the
*           Registry are predefined and open for use:
*           HKEY_LOCAL_MACHINE, HKEY_USERS, HKEY_CURRENT_USER,
*           and HKEY_CLASSES_ROOT.  These 4 can be used for
*           RegEnumKey as is; but to RegEnumKey on any of the
*           children of these you must first have an open key
*           handle to the child.
*
*           If hKeyRoot != 0, assume you are lower than the
*           first level of the Registry and the user is trying
*           to enumerate one of the children.  First calculate
*           the name of the child, and then use RegOpenKey to
*           get an open handle.
*
*           If hKeyRoot == 0, assume you are at the top level
*           of the Registry, and set the hKey to be enumerated
*           to be one of the 4 predefined values, the specific
*           one indicated by the ListBox selection.
*
\************************************************************************/
  VOID EnumerateLevel (HWND hDlg, LPTSTR NameLBSelect,
                       LPTSTR RegPath, HKEY *hKeyRoot)
  {

    HKEY hKey;
    DWORD  retCode;
    CHAR   Buf[100];
    CHAR   lpBuffer[128];


    if (*hKeyRoot)
      {
                                       // If RegPath is not NULL, then
                                       // you have to add a backslash to the
                                       // path name before appending the next
                                       // level child name.
      if (strcmp (RegPath, "") != 0)
        strcat (RegPath, "\\");

                                       // Add the next level child name.
      strcat (RegPath, NameLBSelect);

                                       // Use RegOpenKeyEx() with the new
                                       // Registry path to get an open handle
                                       // to the child key you want to
                                       // enumerate.
      retCode = RegOpenKeyEx (*hKeyRoot,
                              RegPath,
                              0,
                              KEY_ENUMERATE_SUB_KEYS |
                              KEY_EXECUTE |
                              KEY_QUERY_VALUE,
                              &hKey);

      if (retCode != ERROR_SUCCESS)
        {
        if (retCode == ERROR_ACCESS_DENIED) {
          LoadString(hInst, IDS_CANTOPENKEY, lpBuffer, sizeof(lpBuffer));
          wsprintf (Buf, lpBuffer);
        }
        else {
          LoadString(hInst, IDS_OPENKEYERR, lpBuffer, sizeof(lpBuffer));
          wsprintf (Buf, lpBuffer, retCode, __LINE__);
        }
        MessageBox (hDlg, Buf, "", MB_OK);
        PostMessage (hDlg, WM_COMMAND, IDB_BACK, 0);
        return;
        }

      }
    else
      {
      // Set the *hKeyRoot handle based
      // on the text taken from the ListBox.

      if (strcmp (NameLBSelect, "HKEY_CLASSES_ROOT") == 0)
        *hKeyRoot = HKEY_CLASSES_ROOT;

      if (strcmp (NameLBSelect, "HKEY_USERS") == 0)
        *hKeyRoot = HKEY_USERS;

      if (strcmp (NameLBSelect, "HKEY_LOCAL_MACHINE") == 0)
        *hKeyRoot = HKEY_LOCAL_MACHINE;

      if (strcmp (NameLBSelect, "HKEY_CURRENT_USER") == 0)
        *hKeyRoot = HKEY_CURRENT_USER;

      hKey = *hKeyRoot;     // hKey is used in RegEnumKey().

      }//end if/else *hKeyRoot

      QueryKey (hDlg, hKey);


      RegCloseKey (hKey);   // Close the key handle.

//      rect.top = 0; rect.left = 5; rect.right = 1200; rect.bottom = 25;
//      hDC = GetDC (hDlg);
//      FillRect (hDC, &rect, GetStockObject(WHITE_BRUSH));
//      TextOut (hDC, 5, 5, RegPath, strlen(RegPath));
//      ReleaseDC (hDlg, hDC);
      SetDlgItemText (hDlg, IDE_TEXTOUT, RegPath);


  }


/************************************************************************\
*
*  FUNCTION: QueryKey();
*
*  PURPOSE:  To display the key's children (subkeys) and the names of
*            the Values associated with it.  This function uses RegEnumKey,
*            RegEnumValue, and RegQueryInfoKey.
*
\************************************************************************/
VOID QueryKey (HWND hDlg, HANDLE hKey)
  {
  CHAR     KeyName[MAX_PATH];
  CHAR     ClassName[MAX_PATH] = ""; // Buffer for class name.
  DWORD    dwcClassLen = MAX_PATH;   // Length of class string.
  DWORD    dwcSubKeys;               // Number of sub keys.
  DWORD    dwcMaxSubKey;             // Longest sub key size.
  DWORD    dwcMaxClass;              // Longest class string.
  DWORD    dwcValues;                // Number of values for this key.
  DWORD    dwcMaxValueName;          // Longest Value name.
  DWORD    dwcMaxValueData;          // Longest Value data.
  DWORD    dwcSecDesc;               // Security descriptor.
  FILETIME ftLastWriteTime;          // Last write time.

  DWORD i;
  DWORD retCode;

  DWORD j;
  DWORD retValue;
  CHAR  ValueName[MAX_VALUE_NAME];
  DWORD dwcValueName = MAX_VALUE_NAME;
  CHAR  Buf[80];
  CHAR  lpBuffer[80];


  // Get Class name, Value count.

  RegQueryInfoKey (hKey,              // Key handle.
                   ClassName,         // Buffer for class name.
                   &dwcClassLen,      // Length of class string.
                   NULL,              // Reserved.
                   &dwcSubKeys,       // Number of sub keys.
                   &dwcMaxSubKey,     // Longest sub key size.
                   &dwcMaxClass,      // Longest class string.
                   &dwcValues,        // Number of values for this key.
                   &dwcMaxValueName,  // Longest Value name.
                   &dwcMaxValueData,  // Longest Value data.
                   &dwcSecDesc,       // Security descriptor.
                   &ftLastWriteTime); // Last write time.

  SetDlgItemText (hDlg, IDE_CLASS, ClassName);
  SetDlgItemInt  (hDlg, IDE_CVALUES, dwcValues, FALSE);

  SendMessage (GetDlgItem (hDlg, IDL_LISTBOX),
               LB_ADDSTRING, 0, (LONG)"..");

                                       // Loop until RegEnumKey fails, get
                                       // the name of each child and enter
                                       // it into the box.

  // Enumerate the Child Keys.

  SetCursor (LoadCursor (NULL, IDC_WAIT));
  for (i=0, retCode = ERROR_SUCCESS; retCode == ERROR_SUCCESS; i++)
    {
    retCode = RegEnumKey (hKey, i,
                          KeyName, MAX_PATH);

    if (retCode == (DWORD)ERROR_SUCCESS)
      SendMessage (GetDlgItem(hDlg, IDL_LISTBOX),
                   LB_ADDSTRING, 0, (LONG)KeyName);
    }
  SetCursor (LoadCursor (NULL, IDC_ARROW));


  // Enumerate the Key Values
        SetCursor (LoadCursor (NULL, IDC_WAIT));

        if (dwcValues)
          for (j = 0, retValue = ERROR_SUCCESS; j < dwcValues; j++)
            {
            dwcValueName = MAX_VALUE_NAME;
            ValueName[0] = '\0';
            retValue = RegEnumValue (hKey, j, ValueName,
                                     &dwcValueName,
                                     NULL,
                                     NULL,               //&dwType,
                                     NULL,               //&bData,
                                     NULL);              //&bcData);
            if (retValue != (DWORD)ERROR_SUCCESS &&
                retValue != ERROR_INSUFFICIENT_BUFFER)
              {
              LoadString(hInst, IDS_REGERR, lpBuffer, sizeof(lpBuffer));
              wsprintf (Buf, lpBuffer, __LINE__, j, retValue, dwcValueName);
              LoadString(hInst, IDS_DEBUG, lpBuffer, sizeof(lpBuffer));
              MessageBox (hDlg, Buf, lpBuffer, MB_OK);
              }

            Buf[0] = '\0';
            if (!strlen(ValueName)) {
              LoadString(hInst, IDS_NONAME, lpBuffer, sizeof(lpBuffer));
              strcpy (ValueName, lpBuffer);
            }
            wsprintf (Buf, "%d) %s ", j, ValueName);
            SendMessage (GetDlgItem (hDlg, IDL_LISTBOX2),
                           LB_ADDSTRING, 0, (LONG)Buf);

            }// end for(;;)

        SetCursor (LoadCursor (NULL, IDC_ARROW));

  }



/************************************************************************\
*
*  FUNCTION: DisplayKeyData();
*
*  PURPOSE:  To display the keys values and value types to the Value edit
*            field.  This function is called when the right hand listbox
*            is double clicked.  The functionality is much like that found
*            in the function PrintTree, please see it for more details.
*
\************************************************************************/


VOID DisplayKeyData (HWND hDlg, CHAR *RegPath, HANDLE hKeyRoot)
  {
  HKEY   hKey;
  DWORD  dwLBIndex;
  CHAR   Buf[LINE_LEN];
  CHAR   ValueName[MAX_VALUE_NAME];
  DWORD  cbValueName = MAX_VALUE_NAME;
  DWORD  dwType;
  DWORD  retCode;
  CHAR   lpBuffer[128];
  CHAR   lpBuffer2[128];

  CHAR   ClassName[MAX_PATH];
  DWORD  dwcClassLen = MAX_PATH;
  DWORD  dwcSubKeys;
  DWORD  dwcMaxSubKey;
  DWORD  dwcMaxClass;
  DWORD  dwcValues;
  DWORD  dwcMaxValueName;
  DWORD  dwcMaxValueData;
  DWORD  dwcSecDesc;
  FILETIME  ftLastWriteTime;


  BYTE   *bData;
  DWORD  cbData;

  CHAR   *outBuf;
  DWORD  i;
  DWORD  cStrLen;

  CHAR   *BinaryStrBuf;
  CHAR   ByteBuf[4];

  CHAR   *ptr;

  // OPEN THE KEY.

                                // LBIndex should == value index.
  dwLBIndex = SendMessage (GetDlgItem (hDlg, IDL_LISTBOX2),
                           LB_GETCURSEL, 0, 0);

  retCode = RegOpenKeyEx (hKeyRoot,    // Key handle at root level.
                          RegPath,     // Path name of child key.
                          0,           // Reserved.
                          KEY_EXECUTE, // Requesting read access.
                          &hKey);      // Address of key to be returned.

  if (retCode)
    {
    LoadString(hInst, IDS_REGOPENERR, lpBuffer, sizeof(lpBuffer));
    wsprintf (Buf, lpBuffer, retCode);
    MessageBox (hDlg, Buf, "DisplayKeyData()", MB_OK);
    return;
    }

// ADD A QUERY AND ALLOCATE A BUFFER FOR BDATA.

  retCode =
  RegQueryInfoKey (hKey,              // Key handle.
                   ClassName,         // Buffer for class name.
                   &dwcClassLen,      // Length of class string.
                   NULL,              // Reserved.
                   &dwcSubKeys,       // Number of sub keys.
                   &dwcMaxSubKey,     // Longest sub key size.
                   &dwcMaxClass,      // Longest class string.
                   &dwcValues,        // Number of values for this key.
                   &dwcMaxValueName,  // Longest Value name.
                   &dwcMaxValueData,  // Longest Value data.
                   &dwcSecDesc,       // Security descriptor.
                   &ftLastWriteTime); // Last write time.

   if (retCode)
    {
    LoadString(hInst, IDS_REGQUERYERR, lpBuffer, sizeof(lpBuffer));
    wsprintf (Buf, lpBuffer, retCode, __LINE__);
    MessageBox (hDlg, Buf, "", MB_OK);
    }

   bData = HeapAlloc (hHeap, 0, dwcMaxValueData);
   cbData = dwcMaxValueData;


  // ENUMERATE THE KEY.

  retCode = RegEnumValue (hKey,        // Key handle returned from RegOpenKeyEx.
                          dwLBIndex,   // Value index, taken from listbox.
                          ValueName,   // Name of value.
                          &cbValueName,// Size of value name.
                          NULL,        // Reserved, dword = NULL.
                          &dwType,     // Type of data.
                          bData,       // Data buffer.
                          &cbData);    // Size of data buffer.

  if (retCode != ERROR_SUCCESS)
    {

    if (dwType < REG_FULL_RESOURCE_DESCRIPTOR)
      {
      LoadString(hInst, IDS_REGENUMERR, lpBuffer, sizeof(lpBuffer));
      wsprintf (Buf, lpBuffer, retCode, cbData, __LINE__);
      MessageBox (hDlg, Buf, "", MB_OK);
      }
    }


  switch (dwType)
    {
//    REG_NONE                    ( 0 )   // No value type
//    REG_SZ                      ( 1 )   // Unicode nul terminated string
//    REG_EXPAND_SZ               ( 2 )   // Unicode nul terminated string
                                            // (with environment variable references)
//    REG_BINARY                  ( 3 )   // Free form binary
//    REG_DWORD                   ( 4 )   // 32-bit number
//    REG_DWORD_LITTLE_ENDIAN     ( 4 )   // 32-bit number (same as REG_DWORD)
//    REG_DWORD_BIG_ENDIAN        ( 5 )   // 32-bit number
//    REG_LINK                    ( 6 )   // Symbolic Link (unicode)
//    REG_MULTI_SZ                ( 7 )   // Multiple Unicode strings
//    REG_RESOURCE_LIST           ( 8 )   // Resource list in the resource map
//    REG_FULL_RESOURCE_DESCRIPTOR ( 9 )  // Resource list in the hardware description

    case REG_NONE:
      LoadString(hInst, IDS_REGNONE, lpBuffer, sizeof(lpBuffer));
      SetDlgItemText (hDlg, IDE_VALUE1, lpBuffer);
      break;

    case REG_SZ:
      LoadString(hInst, IDS_REGSZ, lpBuffer, sizeof(lpBuffer));
      SetDlgItemText (hDlg, IDE_VALUE1, lpBuffer);

      outBuf = HeapAlloc (hHeap, 0, cbData + 2);
      *outBuf = '\0';

      strcat (outBuf, "\"");
      strcat (outBuf, bData);
      strcat (outBuf, "\"");

      SetDlgItemText (hDlg, IDE_VALUE2, outBuf);
      HeapFree (hHeap, 0, outBuf);
      break;

    case REG_EXPAND_SZ:
      LoadString(hInst, IDS_REGEXPSZ, lpBuffer, sizeof(lpBuffer));
      SetDlgItemText (hDlg, IDE_VALUE1, lpBuffer);
      outBuf = HeapAlloc (hHeap, 0, cbData + 2);
      *outBuf = '\0';

      strcat (outBuf, "\"");
      strcat (outBuf, bData);
      strcat (outBuf, "\"");

      SetDlgItemText (hDlg, IDE_VALUE2, outBuf);
      HeapFree (hHeap, 0, outBuf);
      break;

    case REG_BINARY:
      LoadString(hInst, IDS_REGBIN, lpBuffer, sizeof(lpBuffer));
      SetDlgItemText (hDlg, IDE_VALUE1, lpBuffer);
      SetCursor (LoadCursor (NULL, IDC_WAIT));

      BinaryStrBuf = HeapAlloc (hHeap, 0, (3 * cbData) + 1);
      if (BinaryStrBuf)
        {
        *BinaryStrBuf = '\0';
        *ByteBuf = '\0';
        for (i = 0; i < cbData; i++)
          {
          sprintf (ByteBuf, "%02x ", (BYTE)bData[i]);
          strcat (BinaryStrBuf, ByteBuf);
          }
        SetDlgItemText (hDlg, IDE_VALUE2, BinaryStrBuf);
        }
      else
        {
        LoadString(hInst, IDS_MALLOCERR, lpBuffer, sizeof(lpBuffer));
        LoadString(hInst, IDS_DBGTITLE, lpBuffer2, sizeof(lpBuffer2));
        MessageBox (hDlg, lpBuffer, lpBuffer2, MB_OK);
        }
      SetDlgItemText (hDlg, IDL_LISTBOX2, BinaryStrBuf);
      HeapFree (hHeap, 0, BinaryStrBuf);
      SetCursor (LoadCursor (NULL, IDC_ARROW));

      break;

    case REG_DWORD:
      LoadString(hInst, IDS_REGDWORD, lpBuffer, sizeof(lpBuffer));
      SetDlgItemText (hDlg, IDE_VALUE1, lpBuffer);
      SetDlgItemInt (hDlg, IDE_VALUE2, *(UINT *)bData, FALSE);
      break;

    case REG_DWORD_BIG_ENDIAN:
      LoadString(hInst, IDS_REGBIGEND, lpBuffer, sizeof(lpBuffer));
      SetDlgItemText (hDlg, IDE_VALUE1, lpBuffer);
      SetDlgItemInt (hDlg, IDE_VALUE2, *(UINT *)bData, TRUE);
      break;

    case REG_LINK:
      LoadString(hInst, IDS_REGLINK, lpBuffer, sizeof(lpBuffer));
      SetDlgItemText (hDlg, IDE_VALUE1, lpBuffer);
      SetDlgItemText (hDlg, IDE_VALUE2, bData);
      break;

    case REG_MULTI_SZ:
      LoadString(hInst, IDS_REGMULTI, lpBuffer, sizeof(lpBuffer));
      SetDlgItemText (hDlg, IDE_VALUE1, lpBuffer);
      SetCursor (LoadCursor (NULL, IDC_WAIT));
                                       // Count the NULLs in the buffer to
                                       // find out how many strings there are.

      for (i=0, cStrLen=4; i < cbData; i++)
        if (!bData[i])
          cStrLen+=4;                  // Add room for two quotes and two
                                       // spaced per string.

      outBuf = HeapAlloc (hHeap, 0, cbData + cStrLen);

      ptr = bData;                     // Set ptr to beginning of buffer.
      *outBuf = '\0';                  // Initialize output string.

      strcat (outBuf, "{ ");           // Do first bracket.
      while (*ptr)                     // Loop til you hit 2 NULLs in a row.
        {
         strcat (outBuf, "\"");        // Put quotes around each string.
         strcat (outBuf, ptr);
         strcat (outBuf, "\"  ");
         ptr += strlen(ptr)+1;
        }
      strcat (outBuf, "}");            // Add final bracket.
      SetDlgItemText (hDlg, IDE_VALUE2, outBuf);

      SetCursor (LoadCursor (NULL, IDC_ARROW));
      HeapFree (hHeap, 0, outBuf);                 // free output string.
      break;


    case REG_RESOURCE_LIST:            // CM_RESOURCE_LIST is complex.  Print it
                                       // as a free formed binary data for now.

      LoadString(hInst, IDS_REGRESLST, lpBuffer, sizeof(lpBuffer));
      SetDlgItemText (hDlg, IDE_VALUE1, lpBuffer);

      BinaryStrBuf = HeapAlloc (hHeap, 0, (3 * cbData) + 1);
      if (BinaryStrBuf)
        {
        *BinaryStrBuf = '\0';
        *ByteBuf = '\0';
        for (i = 0; i < cbData; i++)
          {
          sprintf (ByteBuf, "%02x ", (BYTE)bData[i]);
          strcat (BinaryStrBuf, ByteBuf);
          }
        SetDlgItemText (hDlg, IDE_VALUE2, BinaryStrBuf);
        }
      else
        {
        LoadString(hInst, IDS_MALLOCERR, lpBuffer, sizeof(lpBuffer));
        LoadString(hInst, IDS_DBGTITLE, lpBuffer2, sizeof(lpBuffer2));
        MessageBox (hDlg, lpBuffer, lpBuffer2, MB_OK);
        }
      SetDlgItemText (hDlg, IDL_LISTBOX2, BinaryStrBuf);
      HeapFree (hHeap, 0, BinaryStrBuf);

      break;

    case REG_FULL_RESOURCE_DESCRIPTOR:
      LoadString(hInst, IDS_REGFULLDESC, lpBuffer, sizeof(lpBuffer));
      SetDlgItemText (hDlg, IDE_VALUE1, lpBuffer);
      break;



    default:
      LoadString(hInst, IDS_REGUNDEFINE, lpBuffer, sizeof(lpBuffer));
      wsprintf (Buf, lpBuffer, dwType);
      SetDlgItemText (hDlg, IDE_VALUE1, Buf);
      break;

    } // end switch


    HeapFree (hHeap, 0, bData);
  }
