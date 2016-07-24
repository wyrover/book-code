/**************************************************************************\
* uconvert.c -- convert to/from unicode using
*                MulitByteToWideChar & WideCharToMulitByte
*
*
\**************************************************************************/
#define UNICODE

#include <windows.h>
#include <commdlg.h>
#include "uconvert.h"
#include "install.h"






/**************************************************************************\
*  Global variables.
\**************************************************************************/

HANDLE hInst;

/* declare global HWNDs for the child windows.
*   They are created at WM_CREATE  of the main window.
*   Also used in the "View" dialogs.
*/
HWND hwndLabel0, hwndLabel1;
HWND hwndName0, hwndName1;
HWND hwndSize0, hwndSize1;
HWND hwndCodePage0, hwndCodePage1;
HWND hwndByteOrder0, hwndByteOrder1;
HWND hwndButton0, hwndButton1;

/* Global variables storing the source and destination "type" information.
*
* used to communicate between main wnd proc, and *OptionsProc.
*
* gTypeSource - stores the type interpretation of the source data
*       (and implicitly the destination data.)
*   TYPEUNKNOWN: indeterminant... not set. Can not do conversion.
*   TYPEUNICODE: source unicode & destination giDestinationCodePage.
*   TYPECODEPAGE: source giSourceCodePage & destination unicode.
*
* giSourceCodePage stores valid source code page iff gTypeSource == TRUE
* giDestinationCodePage stores valid destination code page iff gTypeSource == FALSE
*
*/
int gTypeSource;
UINT giSourceCodePage;
UINT giDestinationCodePage;

/* Pointers to the source and destination data, and the
 *  count of bytes in each of the buffers.
 */
#define NODATA 0
PBYTE pSourceData =       NULL;
PBYTE pDestinationData =  NULL;
int   nBytesSource =      NODATA;
int   nBytesDestination = NODATA;

/* Conversion Options variables. */
DWORD gMBFlags = MB_PRECOMPOSED;
DWORD gWCFlags = 0;

char  glpDefaultChar[4] = "?";
BOOL  gUsedDefaultChar = FALSE;

/* Handling the Byte Order Mark (BOM).
*
* If the input file begins with a BOM, then we know it is unicode,
*  we skip over the BOM and decrement the size of data by SIZEOFBOM.
*
*
* Before writing data that we know is unicode, write the szBOM string
*  to the file.
*
* Notice that this means that the file sizes we show in the window
*  do NOT include the BOM.
*/

char szBOM[] = "\377\376";  // 0xFF, 0xFE  // leave off TEXT() macro.
char szRBOM[] = "\376\377";  // 0xFF, 0xFE  // leave off TEXT() macro.
#define SIZEOFBOM 2

/* Title of main window */
TCHAR TitleMBToWC[]= TEXT("UConvert -- MultiByteToWideChar()");
TCHAR TitleWCToMB[]= TEXT("UConvert -- WideCharToMultiByte()");
TCHAR TitleUnknown[]= TEXT("UConvert.");

/* file name of the online help file */
TCHAR szHelpPathName[] = TEXT("uconvert.HLP");

/* Strings used to fill onscreen windows. */
TCHAR szBlank[] = TEXT("");

/* MessageBox() strings and flags. */
TCHAR MBTitle[30]= TEXT("");
UINT  MBFlags = MB_OK | MB_ICONEXCLAMATION;


/* misc. defines affecting size and placement of child windows */
#define BORDER    GetSystemMetrics (SM_CXFRAME)*4
#define WHEIGHT   GetSystemMetrics (SM_CYMENU)



/**************************************************************************\
*
*  function:  WinMain()
*
*
\**************************************************************************/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine, int nCmdShow)
{
    MSG    msg;
    WNDCLASS  wc;
    HWND   hwndMain;
    HACCEL haccel;


    UNREFERENCED_PARAMETER( lpCmdLine );
    UNREFERENCED_PARAMETER(  nCmdShow );
    hInst = hInstance;


    /* Check for previous instance.  If none, then register class. */
    if (!hPrevInstance) {

      wc.style = 0;
      wc.lpfnWndProc = (WNDPROC)MainWndProc;

      wc.cbClsExtra = 0;
      wc.cbWndExtra = 0;
      wc.hInstance = hInstance;
      wc.hIcon = LoadIcon(hInstance, TEXT("uconvertIcon"));
      wc.hCursor = LoadCursor(NULL, IDC_ARROW);
      wc.hbrBackground = GetStockObject (LTGRAY_BRUSH);
      wc.lpszMenuName =  TEXT("uconvertMenu");
      wc.lpszClassName = TEXT("uconvert");

      if (!RegisterClass(&wc)) return (FALSE);

    }  /* class registered o.k. */


    /* Create the main window.  Return false if CreateWindow() fails */
    hwndMain = CreateWindow(
        TEXT("uconvert"),
        TitleUnknown,
        (WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX)) | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        512,  // Big enough for most of the text.
        16*WHEIGHT,
        NULL, NULL, hInst, NULL);

    if (!hwndMain) return (FALSE);


    /* Load the accelerator table that provides clipboard support. */
    haccel = LoadAccelerators (hInst, TEXT("uconvertAccel"));

    LoadString(hInst,IDS_APP_WARNING,MBTitle,sizeof(MBTitle));

    /* Loop getting messages and dispatching them. */
    while (GetMessage(&msg, NULL, 0,0)) {
      if (!TranslateAccelerator(hwndMain, haccel, &msg)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }

    return (msg.wParam);
}




/**************************************************************************\
*
*  function:  MainWndProc()
*
*
* On WM_CREATE create all of the child windows.
* On WM_DESTROY make sure that all dynamically allocated memory is freed.
* On WM_PAINT, outline many of the child windows.
* On WM_COMMAND, respond to the command messages properly.
*
\**************************************************************************/
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{


/* misc. variables used in multiple messages cases. */
RECT clientrect;
RECT rect;
TCHAR buffer[50];
static TCHAR szFilter[MAX_PATH];

  switch (message) {

    /**********************************************************************\
    *  WM_CREATE
    *
    * Create all of the child windows used on this main window.
    *  Assign the HWNDs to the correct static variables.
    *
    \**********************************************************************/
    case WM_CREATE: {
      GetClientRect (hwnd, &clientrect);

      /* Create Source Windows. */
      CopyRect (&rect, &clientrect);
      rect.right = (clientrect.right - clientrect.left) /2;
      InflateRect (&rect, -2*BORDER, -BORDER);
      createwindows(&rect,
                   hwnd,
                   &hwndLabel0,
                   &hwndName0,
                   &hwndSize0,
                   &hwndCodePage0,
                   &hwndByteOrder0,
                   &hwndButton0);

      /* Create Destination Windows. */
      CopyRect (&rect, &clientrect);
      rect.left = (clientrect.right - clientrect.left) /2;
      InflateRect (&rect, -2*BORDER, -BORDER);
      createwindows(&rect,
                   hwnd,
                   &hwndLabel1,
                   &hwndName1,
                   &hwndSize1,
                   &hwndCodePage1,
                   &hwndByteOrder1,
                   &hwndButton1);

      /* fill in window information that is different for source/destination */
      SetWindowText (hwndLabel0, LoadResourceString(IDS_SOURCE));
      SetWindowText (hwndLabel1, LoadResourceString(IDS_DESTINATION));

      SetWindowText (hwndButton0, LoadResourceString(IDS_VIEW_SOURCE_BTN));
      SetWindowText (hwndButton1, LoadResourceString(IDS_VIEW_DESTINATION_BTN));

      SetWindowLong (hwndButton0, GWL_ID, BID_VIEWSOURCE      );
      SetWindowLong (hwndButton1, GWL_ID, BID_VIEWDESTINATION );

      gTypeSource = TYPEUNKNOWN;
      giSourceCodePage =      GetACP();  // Just some reasonable initializer.
      giDestinationCodePage = GetACP();  // Just some reasonable initializer.

      /* initialize source & destination data correctly */
      SendMessage (hwnd, WM_COMMAND, MID_CLEARSOURCE,      0);
      SendMessage (hwnd, WM_COMMAND, MID_CLEARDESTINATION, 0);

      /* Build up the correct filter strings for OPENFILENAME structure
       *  Do it here so that we only have to do it once.
       */
      {
        TCHAR *p;

        p = szFilter;
        lstrcpy (buffer,LoadResourceString(IDS_FILE_FILTER_SPEC1));
        lstrcpy (p,buffer);
        p += lstrlen (buffer) +1;
        lstrcpy (buffer,TEXT("*.*"));
        lstrcpy (p,buffer);
        p += lstrlen (buffer) +1;

        lstrcpy (buffer,LoadResourceString(IDS_FILE_FILTER_SPEC2));
        lstrcpy (p,buffer);
        p += lstrlen (buffer) +1;
        lstrcpy (buffer,TEXT("*.txt"));
        lstrcpy (p,buffer);
        p += lstrlen (buffer) +1;

        lstrcpy (buffer,LoadResourceString(IDS_FILE_FILTER_SPEC3));
        lstrcpy (p,buffer);
        p += lstrlen (buffer) +1;
        lstrcpy (buffer,TEXT("*.utf"));
        lstrcpy (p,buffer);
        p += lstrlen (buffer) +1;

        lstrcpy (p,TEXT("\0"));
      }
    } break; /* end WM_CREATE */



    /**********************************************************************\
    *  WM_DESTROY
    *
    * Release the Online help, and free allocated memory if any.
    \**********************************************************************/
    case WM_DESTROY:
      WinHelp( hwnd, szHelpPathName, (UINT) HELP_QUIT, (DWORD) NULL );
      ManageMemory (MMFREE, MMSOURCE,      0, pSourceData);
      ManageMemory (MMFREE, MMDESTINATION, 0, pDestinationData);
      PostQuitMessage(0);
    break;


    /**********************************************************************\
    *  WM_CTLCOLOR*
    *
    * Set the background of the child controls to be gray here.
    \**********************************************************************/
    case WM_CTLCOLORBTN:
    case WM_CTLCOLORSTATIC: {
      HDC hdc;

      hdc = (HDC) wParam;
      SetBkMode (hdc, TRANSPARENT);
      return (LRESULT)GetStockObject (LTGRAY_BRUSH);
    } break;




    /**********************************************************************\
    *  WM_PAINT
    *
    * Simply draw the two vertical divider lines, and 3D frame the children.
    *
    \**********************************************************************/
    case WM_PAINT: {
      HDC hdc;
      PAINTSTRUCT ps;

      hdc = BeginPaint(hwnd, &ps);
      GetClientRect (hwnd, &clientrect);

      /* draw vertical separator line in the center */
      rect.left = (clientrect.right - clientrect.left ) /2 -1;
      rect.top = clientrect.top;
      rect.right = rect.left +1;;
      rect.bottom = clientrect.bottom;
      FrameRect (hdc, &rect, GetStockObject (GRAY_BRUSH));
      SelectObject (hdc, GetStockObject (WHITE_PEN));
      MoveToEx (hdc, rect.right, rect.top, NULL);
      LineTo (hdc,rect.right, rect.bottom);

      /* draw 3D outlines of child windows. */
      framechildwindow (hdc, hwnd, hwndName0);
      framechildwindow (hdc, hwnd, hwndSize0);
      framechildwindow (hdc, hwnd, hwndCodePage0);
      framechildwindow (hdc, hwnd, hwndByteOrder0);

      framechildwindow (hdc, hwnd, hwndName1);
      framechildwindow (hdc, hwnd, hwndSize1);
      framechildwindow (hdc, hwnd, hwndCodePage1);
      framechildwindow (hdc, hwnd, hwndByteOrder1);

      /* underline the labels */
      underlinechildwindow (hdc, hwnd, hwndLabel0);
      underlinechildwindow (hdc, hwnd, hwndLabel1);

      EndPaint (hwnd, &ps);
    } break; /* end WM_PAINT */




    /**********************************************************************\
    *  WMU_ADJUSTFORNEWSOURCE
    *
    * lParam - szName of source (file, clipboard, ...)
    *
    * global - nBytesSource
    *
    * "user message."  Set the text of the Source windows
    \**********************************************************************/
    case WMU_ADJUSTFORNEWSOURCE: {
      LPVOID szName;

      szName = (LPVOID) lParam;

      /* Set Window text appropriately */
      SetWindowText (hwndName0, szName);
      wsprintf (buffer, LoadResourceString(IDS_BYTES), nBytesSource);
      SetWindowText (hwndSize0, buffer);
      SetWindowText (hwndByteOrder0, szBlank);

      /* Clear the destination data if any to avoid user confusion. */
      SendMessage (hwnd, WM_COMMAND, MID_CLEARDESTINATION, 0);

      /* Reset the "type strings" based on new gTypeSource. */
      SendMessage (hwnd, WMU_SETTYPESTRINGS, 0,0);
    } break;




    /**********************************************************************\
    *  WMU_SETTYPESTRINGS
    *
    * "user message."  Set the text of the "type" windows to reflect
    *  the state stored in gTypeSource and gi*CodePage.
    *
    \**********************************************************************/
    case WMU_SETTYPESTRINGS:
      switch (gTypeSource) {
        case TYPEUNICODE:
          SetWindowText (hwndCodePage0, TEXT("Unicode"));
          wsprintf (buffer, LoadResourceString(IDS_CODE_PAGE),
                  giDestinationCodePage);
          SetWindowText (hwndCodePage1, buffer);
          SetWindowText (hwnd, TitleWCToMB);
        break;
        case TYPECODEPAGE:
          wsprintf (buffer, LoadResourceString(IDS_CODE_PAGE),
                  giSourceCodePage);
          SetWindowText (hwndCodePage0, buffer);
          SetWindowText (hwndCodePage1, TEXT("Unicode"));
          SetWindowText (hwnd, TitleMBToWC);
        break;
        case TYPEUNKNOWN:
          SetWindowText (hwndCodePage0, szBlank);
          SetWindowText (hwndCodePage1, szBlank);
          SetWindowText (hwnd, TitleUnknown);
        break;
      } /* end switch gTypeSource */
    break;


    /**********************************************************************\
    *  WM_INITMENU
    *
    * Manage the enabled state of all of the menus.
    *  Notice that the button enabled state is taken care of in ManageMemory().
    *
    * In general, this is dependent upon pSourceData & pDestinationData.
    *  They are either NULL or non-NULL, and menu items are dependent upon
    *  this state.
    *
    * One exception is the "Paste from Clipboard menu" which is enabled
    *  conditional upon there being text data in the clipboard.
    *
    \**********************************************************************/
    case WM_INITMENU:

      /* Adjust the "Paste from Clipboard menu" */
      OpenClipboard (hwnd);
      if (IsClipboardFormatAvailable (CF_UNICODETEXT) ||
          IsClipboardFormatAvailable (CF_OEMTEXT) ||
          IsClipboardFormatAvailable (CF_TEXT))
        EnableMenuItem (GetMenu (hwnd),MID_PASTESOURCE,      MF_ENABLED);
      else
        EnableMenuItem (GetMenu (hwnd),MID_PASTESOURCE,      MF_GRAYED);
      CloseClipboard ();


      /* Adjust the source data dependent menus. */
      if (pSourceData != NULL) {
        EnableMenuItem (GetMenu (hwnd),MID_SOURCEOPT,        MF_ENABLED);
        EnableMenuItem (GetMenu (hwnd),MID_SWAPSOURCE,       MF_ENABLED);
        EnableMenuItem (GetMenu (hwnd),MID_CLEARSOURCE,      MF_ENABLED);
        EnableMenuItem (GetMenu (hwnd),MID_CONVERTNOW,       MF_ENABLED);
        EnableMenuItem (GetMenu (hwnd),MID_CONVERSIONOPT,    MF_ENABLED);
        EnableMenuItem (GetMenu (hwnd),MID_DESTINATIONOPT,   MF_ENABLED);
      } else {
        EnableMenuItem (GetMenu (hwnd),MID_SOURCEOPT,        MF_GRAYED);
        EnableMenuItem (GetMenu (hwnd),MID_SWAPSOURCE,       MF_GRAYED);
        EnableMenuItem (GetMenu (hwnd),MID_CLEARSOURCE,      MF_GRAYED);
        EnableMenuItem (GetMenu (hwnd),MID_CONVERTNOW,       MF_GRAYED);
        EnableMenuItem (GetMenu (hwnd),MID_CONVERSIONOPT,    MF_GRAYED);
        EnableMenuItem (GetMenu (hwnd),MID_DESTINATIONOPT,   MF_GRAYED);
      }


      /* Adjust the destination data dependent menus. */
      if (pDestinationData != NULL) {
        EnableMenuItem (GetMenu (hwnd),MID_SAVEAS,           MF_ENABLED);
        EnableMenuItem (GetMenu (hwnd),MID_SWAPDESTINATION,  MF_ENABLED);
        EnableMenuItem (GetMenu (hwnd),MID_COPYDESTINATION,  MF_ENABLED);
        EnableMenuItem (GetMenu (hwnd),MID_CLEARDESTINATION, MF_ENABLED);
      } else {
        EnableMenuItem (GetMenu (hwnd),MID_SAVEAS,           MF_GRAYED);
        EnableMenuItem (GetMenu (hwnd),MID_SWAPDESTINATION,  MF_GRAYED);
        EnableMenuItem (GetMenu (hwnd),MID_COPYDESTINATION,  MF_GRAYED);
        EnableMenuItem (GetMenu (hwnd),MID_CLEARDESTINATION, MF_GRAYED);
      }

    break;





    /**********************************************************************\
    *  WM_COMMAND
    *
    * Just switch() on the command ID.  Notice that menu and button
    *  command messages are treated the same.
    *
    \**********************************************************************/
    case WM_COMMAND:
      switch (LOWORD(wParam)) {


        /******************************************************************\
        *  WM_COMMAND, MID_OPEN
        *
        * Put up common dialog, try to open & read file.
        *  Fill windows with correct text & fill pSourceData.
        \******************************************************************/
        case MID_OPEN  : {
          HANDLE hFile;
          DWORD nBytesRead;
          TCHAR szFile[MAX_PATH],szFileTitle[MAX_PATH];

          /* First set up the structure for the GetOpenFileName
           *  common dialog.
           */
          {
            OPENFILENAME OpenFileName;
            /* buffers for the file names. */

            wsprintf (szFile, szBlank);
            wsprintf (szFileTitle, szBlank);


            OpenFileName.lStructSize       = sizeof(OPENFILENAME);
            OpenFileName.hwndOwner         = hwnd;
            OpenFileName.hInstance         = (HANDLE) hInst;
            OpenFileName.lpstrFilter       = szFilter; // built in WM_CREATE
            OpenFileName.lpstrCustomFilter = NULL;
            OpenFileName.nMaxCustFilter    = 0L;
            OpenFileName.nFilterIndex      = 1L;
            OpenFileName.lpstrFile         = szFile;
            OpenFileName.nMaxFile          = MAX_PATH;
            OpenFileName.lpstrFileTitle    = szFileTitle;
            OpenFileName.nMaxFileTitle     = MAX_PATH;
            OpenFileName.lpstrInitialDir   = NULL;
            OpenFileName.lpstrTitle        = LoadResourceString(IDS_OPEN_FILE_TITLE);

            OpenFileName.nFileOffset       = 0;
            OpenFileName.nFileExtension    = 0;
            OpenFileName.lpstrDefExt       = NULL;

            OpenFileName.lCustData         = 0;
            OpenFileName.lpfnHook          = NULL;
            OpenFileName.lpTemplateName    = NULL;

            OpenFileName.Flags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;

            if (!GetOpenFileName(&OpenFileName)) return 0;
          }


          /* User has filled in the file information.
           *  Try to open that file for reading.
           */
          hFile = CreateFile(szFile,
                       GENERIC_READ,
                       0,
                       NULL,
                       OPEN_EXISTING,
                       FILE_ATTRIBUTE_NORMAL,
                       NULL);
          if (hFile == INVALID_HANDLE_VALUE) {
            MessageBox (hwnd, LoadResourceString(IDS_OPEN_FILE_FAILED),
                    MBTitle, MBFlags);
            return 0;
          }


          /* make sure file is not too big... i.e. > 2^32
           *  If it is OK, write the file size in hwndSize0
           */
          {
            BY_HANDLE_FILE_INFORMATION bhfi;

            GetFileInformationByHandle (hFile, &bhfi);
            if (bhfi.nFileSizeHigh != 0) {
              MessageBox (hwnd, LoadResourceString(IDS_FILE_TOO_BIG),
                      MBTitle, MBFlags);
              CloseHandle (hFile);
              return 0;
            }

            nBytesSource= bhfi.nFileSizeLow;

          }

          /* Allocate space for string, including potential UNICODE_NULL */
          pSourceData = ManageMemory (MMALLOC, MMSOURCE, nBytesSource +2, pSourceData);
          if (pSourceData == NULL) {
            CloseHandle (hFile);
            return 0;
          }

          if (nBytesSource < SIZEOFBOM) {
              gTypeSource = TYPEUNKNOWN;
              goto no_bom;
          }

          /* first read two bytes and look for BOM */
          if (!ReadFile (hFile, pSourceData,SIZEOFBOM, &nBytesRead, NULL)) {
            MessageBox (hwnd, LoadResourceString(IDS_READFILE_FAILED),
                    MBTitle, MBFlags);
            CloseHandle (hFile);
            pSourceData = ManageMemory (MMFREE, MMSOURCE, 0, pSourceData);
            return 0;
          }



          /* If file begins with BOM, then we know the type,
           *  we'll decrement the number of bytes by two,
           *  and read the rest of the data.
           */
          if (IsBOM (pSourceData)) {
            gTypeSource = TYPEUNICODE;
            nBytesSource -=SIZEOFBOM;

          /* If file begins with Reverse BOM, then we know the type,
           *  we'll decrement the number of bytes by two,
           *  and read the rest of the data, and post a message so
           *  that we know to swap the order later.
           */
          } else if (IsRBOM (pSourceData)) {
            gTypeSource = TYPEUNICODE;
            nBytesSource -=SIZEOFBOM;
            MessageBox (hwnd, LoadResourceString(IDS_SWAPPING_BYTE_ORDER),
                    MBTitle, MBFlags);
            PostMessage (hwnd, WM_COMMAND, MID_SWAPSOURCE, 0);

          /* Oops, does not begin with BOM.
           *  Reset file pointer, and read data.
           */
          } else {
            gTypeSource = TYPEUNKNOWN;
            SetFilePointer (hFile, -SIZEOFBOM, NULL, FILE_CURRENT);
          }

          no_bom:


          /* try to read all of it into memory */
          if (!ReadFile (hFile, pSourceData,nBytesSource, &nBytesRead, NULL)) {
            MessageBox (hwnd, LoadResourceString(IDS_READFILE_FAILED),
                    MBTitle, MBFlags);
            CloseHandle (hFile);
            pSourceData = ManageMemory (MMFREE, MMSOURCE, 0, pSourceData);
            return 0;
          }

          CloseHandle (hFile);

          /* If we don't know the file type at this point,
           *  try to determine if it is unicode.
           */
          if (gTypeSource == TYPEUNKNOWN) {
            if (IsUnicode (pSourceData)) {
              gTypeSource = TYPEUNICODE;
              pSourceData[nBytesSource]   = 0;  // UNICODE_NULL
              pSourceData[nBytesSource+1] = 0;
            } else {
              gTypeSource = TYPECODEPAGE;
              pSourceData[nBytesSource] = 0;
            }
          }

          SendMessage (hwnd, WMU_ADJUSTFORNEWSOURCE, 0, (LPARAM)szFile);



        } break;  /* end case MID_OPEN */



        /******************************************************************\
        *  WM_COMMAND, MID_SAVEAS
        *
        * Put up common dialog, try to open file, and write data to it.
        \******************************************************************/
        case MID_SAVEAS: {
          HANDLE hFile;
          DWORD nBytesRead;
          TCHAR szFile[MAX_PATH],szFileTitle[MAX_PATH];

          if (nBytesDestination == NODATA ) {
            MessageBox (hwnd, LoadResourceString(IDS_NOTEXT_TO_SAVE),
                    MBTitle, MBFlags);
            return 0;
          }


          /* Set up the structure for the GetSaveFileName
           *  common dialog.
           */
          {
            OPENFILENAME OpenFileName;
            /* buffers for the file names. */

            wsprintf (szFile, szBlank);
            wsprintf (szFileTitle, szBlank);

            OpenFileName.lStructSize       = sizeof(OPENFILENAME);
            OpenFileName.hwndOwner         = hwnd;
            OpenFileName.hInstance         = (HANDLE) hInst;
            OpenFileName.lpstrFilter       = szFilter;
            OpenFileName.lpstrCustomFilter = NULL;
            OpenFileName.nMaxCustFilter    = 0L;
            OpenFileName.nFilterIndex      = 1L;
            OpenFileName.lpstrFile         = szFile;
            OpenFileName.nMaxFile          = MAX_PATH;
            OpenFileName.lpstrFileTitle    = szFileTitle;
            OpenFileName.nMaxFileTitle     = MAX_PATH;
            OpenFileName.lpstrInitialDir   = NULL;
            OpenFileName.lpstrTitle        = LoadResourceString(IDS_SAVE_AS_TITLE);

            OpenFileName.nFileOffset       = 0;
            OpenFileName.nFileExtension    = 0;
            OpenFileName.lpstrDefExt       = NULL;

            OpenFileName.lCustData         = 0;
            OpenFileName.lpfnHook          = NULL;
            OpenFileName.lpTemplateName    = NULL;

            OpenFileName.Flags = OFN_HIDEREADONLY;

            if (!GetSaveFileName(&OpenFileName)) return 0;
          }


          /* User has filled in the file information.
           *  Try to open that file for writing.
           */
          hFile = CreateFile(szFile,
                      GENERIC_WRITE,
                      0,
                      NULL,
                      CREATE_ALWAYS,
                      FILE_ATTRIBUTE_NORMAL,
                      NULL);

          if (hFile == INVALID_HANDLE_VALUE) {
            MessageBox (hwnd, LoadResourceString(IDS_CREATEFILE_FAILED),
                    MBTitle, MBFlags);
            return 0;
          }


          /* if destination is unicode, try to write BOM first.
           *  unless the bytes have been swapped
           *  (criterion: hwndByteOrder contains text)
           *  in which case, write a Reverse Byte Order Mark.
           */
          if (gTypeSource == TYPECODEPAGE) {
            if (GetWindowTextLength (hwndByteOrder1) == 0) {

              if (!WriteFile (hFile, szBOM, SIZEOFBOM, &nBytesRead, NULL)) {
                MessageBox (hwnd, LoadResourceString(IDS_WRITEFILE_FAILED),
                        MBTitle, MBFlags);
                CloseHandle (hFile);
                return 0;
              }

            }else {
              if (!WriteFile (hFile, szRBOM, SIZEOFBOM, &nBytesRead, NULL)) {
                MessageBox (hwnd, LoadResourceString(IDS_WRITEFILE_FAILED),
                        MBTitle, MBFlags);
                CloseHandle (hFile);
                return 0;
              }

            }
          }


          /* try to write all of it into memory */
          if (!WriteFile (hFile, pDestinationData,nBytesDestination, &nBytesRead, NULL)) {
            MessageBox (hwnd, LoadResourceString(IDS_WRITEFILE_FAILED),
                    MBTitle, MBFlags);
            CloseHandle (hFile);
            return 0;
          }

          SetWindowText (hwndName1, szFile);
          CloseHandle (hFile);

        } break;


        /**********************************************************************\
        *  WM_COMMAND, MID_PASTESOURCE
        *
        * Paste the clipboard's prefered data format into the source.
        *  Fills pSourceData.
        \**********************************************************************/
        case MID_PASTESOURCE: {
          UINT  iFormat;
          PVOID pData;

          OpenClipboard (hwnd);

          iFormat = 0;
          while (iFormat = EnumClipboardFormats(iFormat))
            if ((iFormat == CF_UNICODETEXT) || (iFormat == CF_OEMTEXT) || (iFormat == CF_TEXT)) {

              HGLOBAL hMem;

              hMem = GetClipboardData (iFormat);
              pData = GlobalLock(hMem);

              switch (iFormat) {
                case CF_UNICODETEXT:
                  nBytesSource = lstrlenW (pData) *2;
                  pSourceData= ManageMemory (MMALLOC, MMSOURCE, nBytesSource+2, pSourceData);
                  lstrcpyW ((LPVOID)pSourceData, pData);
                  gTypeSource = TYPEUNICODE;
                break;

                case CF_OEMTEXT:
                  nBytesSource = lstrlenA (pData);
                  pSourceData= ManageMemory (MMALLOC, MMSOURCE, nBytesSource+1, pSourceData);
                  lstrcpyA (pSourceData, pData);
                  gTypeSource = TYPECODEPAGE;
                  giSourceCodePage = GetOEMCP();
                break;

                case CF_TEXT:
                  nBytesSource = lstrlenA (pData);
                  pSourceData= ManageMemory (MMALLOC, MMSOURCE, nBytesSource+1, pSourceData);
                  lstrcpyA (pSourceData, pData);
                  gTypeSource = TYPECODEPAGE;
                  giSourceCodePage = GetACP();
                break;

                default: break;  // shouldn't get here
              } /* end switch (iFormat) */

              SendMessage (hwnd, WMU_ADJUSTFORNEWSOURCE, 0,
                            (LPARAM)LoadResourceString(IDS_FROM_CLIPBOARD));

              GlobalUnlock(hMem);
            break;  /* break out of while loop. */
            } /* end if iFormat */


          CloseClipboard ();

        } break;



        /**********************************************************************\
        *  WM_COMMAND, MID_COPYDESTINATION
        *
        * Copy destination data to the clipboard.
        \**********************************************************************/
        case MID_COPYDESTINATION:
        {
          HGLOBAL hMem;
          if (pDestinationData == NULL) return FALSE;

          if (gTypeSource != TYPEUNICODE) {
              if (!(hMem = GlobalAlloc(
                      GMEM_MOVEABLE | GMEM_DDESHARE,
                      (lstrlenW((LPWSTR)pDestinationData)+1) * 2))) {
                  return FALSE;
              }
              lstrcpyW(GlobalLock(hMem), (LPWSTR)pDestinationData);
              GlobalUnlock(hMem);
          } else {
              if (!(hMem = GlobalAlloc(
                      GMEM_MOVEABLE | GMEM_DDESHARE,
                      lstrlenA(pDestinationData)+1))) {
                  return FALSE;
              }
              lstrcpyA(GlobalLock(hMem), pDestinationData);
              GlobalUnlock(hMem);
          }

          OpenClipboard (hwnd);
          EmptyClipboard();

          /* if source NOT unicode, then destination is, else look at dest CP */
          if (gTypeSource != TYPEUNICODE)
            SetClipboardData (CF_UNICODETEXT, hMem);
          else if (giDestinationCodePage == GetOEMCP())
            SetClipboardData (CF_OEMTEXT, hMem);
          else
            SetClipboardData (CF_TEXT, hMem);


          CloseClipboard ();

        break;
        }



        /******************************************************************\
        *  WM_COMMAND, MID_CONVERTNOW
        *
        * This is where the conversion actually takes place.
        *  In either case, make the call twice.  Once to determine how
        *  much memory is needed, allocate space, and then make the call again.
        *
        *  If conversion succeeds, it fills pDestinationData.
        \******************************************************************/
        case MID_CONVERTNOW: {
          int nBytesNeeded, nWCharNeeded, nWCharSource;


          if (nBytesSource == NODATA ) {
            MessageBox (hwnd, LoadResourceString(IDS_LOAD_SOURCE_FILE),
                    MBTitle, MBFlags);
            return 0;
          }


          /* Converting UNICODE -> giDestinationCodePage*/
          if (gTypeSource == TYPEUNICODE) 
		  {

            nWCharSource = nBytesSource/2;

            /* Query the number of bytes required to store the Dest string */
            nBytesNeeded = WideCharToMultiByte(giDestinationCodePage, gWCFlags,
                             (LPWSTR)pSourceData, nWCharSource,
                             NULL, 0,
                             glpDefaultChar, &gUsedDefaultChar);

            /* Allocate the required amount of space */

            if (nBytesNeeded == 0) 
			{
                MessageBox (hwnd, LoadResourceString(IDS_FIRSTCALL_FAILED),
                        MBTitle, MBFlags);
                break;
            }

            /* We need more 1 byte for '\0' */
            pDestinationData= ManageMemory (MMALLOC, MMDESTINATION, nBytesNeeded + 2, pDestinationData);

            /* Do the conversion */
            nBytesDestination = WideCharToMultiByte(giDestinationCodePage, gWCFlags,
                             (LPWSTR)pSourceData, nWCharSource,
                             pDestinationData, nBytesNeeded, glpDefaultChar, &gUsedDefaultChar);
            if (nBytesNeeded == 0) {
                MessageBox (hwnd, LoadResourceString(IDS_FIRSTCALL_FAILED),
                        MBTitle, MBFlags);
                break;
            }
            *(LPSTR)((LPSTR)pDestinationData + nBytesNeeded) = '\0';
          }


          /* converting giSourceCodePage -> UNICODE */
          else if (gTypeSource == TYPECODEPAGE) 
		  {

            /* Query the number of WChar required to store the Dest string */
            nWCharNeeded = MultiByteToWideChar(giSourceCodePage, gMBFlags,
                             pSourceData, nBytesSource, NULL, 0 );

            /* Allocate the required amount of space */

            /* We need more 2 bytes for '\0' */
            pDestinationData= ManageMemory (MMALLOC, MMDESTINATION, (nWCharNeeded+1)*2, pDestinationData);

            /* Do the conversion */
            nWCharNeeded = MultiByteToWideChar(giSourceCodePage, gMBFlags,
                             pSourceData, nBytesSource,
                             (LPWSTR)pDestinationData, nWCharNeeded);

            *(LPWSTR)((LPWSTR)pDestinationData + nWCharNeeded) = L'\0';

            /* MultiByteToWideChar returns # WCHAR, so multiply by 2 */
            nBytesDestination = 2*nWCharNeeded ;
          } else {
            MessageBox (hwnd, LoadResourceString(IDS_SOURCE_TYPE_UNKNOWN),
                    MBTitle, MBFlags);
            return 0;
          }


          /* code common to all conversions... */
          SetWindowText (hwndName1, LoadResourceString(IDS_DATA_NOT_SAVED));
          wsprintf (buffer, LoadResourceString(IDS_BYTES), nBytesDestination);
          SetWindowText (hwndSize1, buffer);
          SetWindowText (hwndByteOrder1, szBlank);


          /* Throw up "Save as" dialog to help the user along.
           *  They can always <esc> if need be.
           */
          SendMessage (hwnd, WM_COMMAND, MID_SAVEAS, 0);

        } break; /* end  case BID_CONVERT */



        /******************************************************************\
        *  WM_COMMAND, BID_VIEWSOURCE
        *
        \******************************************************************/
        case BID_VIEWSOURCE:
          if (gTypeSource == TYPEUNICODE)
            DialogBoxW (hInst, L"ShowTextDlg", hwnd, (DLGPROC)ViewSourceProc);
          else
            DialogBoxA (hInst, "ShowTextDlg", hwnd, (DLGPROC)ViewSourceProc);
        break;

        /******************************************************************\
        *  WM_COMMAND, BID_VIEWDESTINATION
        *
        \******************************************************************/
        case BID_VIEWDESTINATION:
          if (gTypeSource == TYPEUNICODE)
            DialogBoxA (hInst, "ShowTextDlg", hwnd, (DLGPROC)ViewDestinationProc);
          else
            DialogBoxW (hInst, L"ShowTextDlg", hwnd, (DLGPROC)ViewDestinationProc);
        break;



        /******************************************************************\
        *  WM_COMMAND, MID_SOURCEOPT
        *
        * Allows user to change interpretation options for the source data.
        *
        *  Put up appropriate dialog box, and reset window text in response.
        \******************************************************************/
        case MID_SOURCEOPT:
          if (DialogBox (hInst, TEXT("DataOptionsDlg"), hwnd, (DLGPROC)SourceOptionsProc)) {
            SendMessage (hwnd, WMU_SETTYPESTRINGS, 0,0);
            SendMessage (hwnd, WM_COMMAND, MID_CLEARDESTINATION, 0);
          }
        break;

        /******************************************************************\
        *  WM_COMMAND, MID_DESTINATIONOPT
        *
        * Allows user to change options for destination data.
        *
        *  Put up appropriate dialog box, and reset window text in response.
        \******************************************************************/
        case MID_DESTINATIONOPT:
          if (DialogBox (hInst, TEXT("DataOptionsDlg"), hwnd, (DLGPROC)DestinationOptionsProc)) {
            SendMessage (hwnd, WMU_SETTYPESTRINGS, 0,0);
            SendMessage (hwnd, WM_COMMAND, MID_CLEARDESTINATION, 0);
          }
        break;

        /******************************************************************\
        *  WM_COMMAND, MID_CONVERSIONOPT
        *
        \******************************************************************/
        case MID_CONVERSIONOPT:
          if (DialogBox (hInst, TEXT("ConversionOptionsDlg"), hwnd, (DLGPROC)ConversionOptionsProc)) {
            SendMessage (hwnd, WM_COMMAND, MID_CLEARDESTINATION, 0);
          }
        break;



        /******************************************************************\
        *  WM_COMMAND, MID_SWAPSOURCE
        *
        * Allows user to reverse byte order of data.
        *
        \******************************************************************/
        case MID_SWAPSOURCE: {
          int i, end;
          BYTE temp;

          if (pSourceData == NULL) return FALSE;

          end =  nBytesSource - 2;
          for (i = 0; i<= end; i+=2) {
            temp             = pSourceData[i];
            pSourceData[i]   = pSourceData[i+1];
            pSourceData[i+1] = temp;
          }

          if (GetWindowTextLength (hwndByteOrder0) == 0)
            SetWindowText (hwndByteOrder0,
                    LoadResourceString(IDS_BYTE_ORDER_REVERSED));
          else
            SetWindowText (hwndByteOrder0, szBlank);

          /* Since source is different, invalidate Destination data. */
          SendMessage (hwnd, WM_COMMAND, MID_CLEARDESTINATION, 0);

        } break;



        /******************************************************************\
        *  WM_COMMAND, MID_SWAPDESTINATION
        *
        * Allows user to reverse byte order of data.
        *
        \******************************************************************/
        case MID_SWAPDESTINATION: {
          int i, end;
          BYTE temp;

          if (pDestinationData == NULL) return FALSE;

          end =  nBytesDestination - 2;
          for (i = 0; i<= end; i+=2) {
            temp             = pDestinationData[i];
            pDestinationData[i]   = pDestinationData[i+1];
            pDestinationData[i+1] = temp;
          }

          if (GetWindowTextLength (hwndByteOrder1) == 0)
            SetWindowText (hwndByteOrder1,
                    LoadResourceString(IDS_BYTE_ORDER_REVERSED));
          else
            SetWindowText (hwndByteOrder1, szBlank);

        } break;


        /**********************************************************************\
        *  WM_COMMAND, MID_CLEARDESTINATION
        *
        * Clear the destination information.  May cause data to be lost.
        \**********************************************************************/
        case MID_CLEARDESTINATION:
          SetWindowText (hwndSize1, szBlank);
          SetWindowText (hwndName1, szBlank);
          SetWindowText (hwndByteOrder1, szBlank);
          pDestinationData= ManageMemory (MMFREE, MMDESTINATION, 0, pDestinationData);
        break;


        /**********************************************************************\
        *  WM_COMMAND, MID_CLEARSOURCE
        *
        * Clear the SOURCE information.  May cause data to be lost.
        \**********************************************************************/
        case MID_CLEARSOURCE:
          SetWindowText (hwndSize0, szBlank);
          SetWindowText (hwndName0, szBlank);
          SetWindowText (hwndByteOrder0, szBlank);
          pSourceData= ManageMemory (MMFREE, MMSOURCE, 0, pSourceData);
        break;





        /******************************************************************\
        *  WM_COMMAND, MID_INSTALLTABLES
        *
        \******************************************************************/
        case MID_INSTALLTABLES:
          DialogBox (hInst, TEXT("InstallTableDlg"), hwnd, (DLGPROC)InstallTableProc);
        break;




        /* Simply call WinHelp to display the OnLine help file. */
        case MID_HELP:
          WinHelp( hwnd, szHelpPathName, HELP_INDEX, (DWORD) NULL );
        break;


        /* No-op Window procedure to simply display the dialog box. */
        case MID_ABOUT:
          ShellAbout (hwnd, TEXT("UConvert"), NULL, LoadIcon (hInst, TEXT("uconvertIcon")));
        break;

        /* Just close the window. */
        case MID_EXIT:
          PostMessage (hwnd, WM_CLOSE, 0,0);
        break;




      } /* end switch (LOWORD(wParam)) */
    break;  /* end WM_COMMAND */



    default: break;
  } /* end switch */

  return (DefWindowProc(hwnd, message, wParam, lParam));
}





/**************************************************************************\
*
*  function:  IsUnicode()
*
* HACK... eventually use a proper function for IsUnicode
*  Use function from unipad?
*
\**************************************************************************/
BOOL IsUnicode (PBYTE pb)
{
  return (IsBOM (pb));
}



/**************************************************************************\
*
*  function:  IsBOM()
*
* true iff pb points to a Byte Order Mark.
*
\**************************************************************************/
BOOL IsBOM (PBYTE pb)
{
  if ((*pb == 0xFF) & (*(pb+1) == 0xFE))  // BOM
    return TRUE;
  else
    return FALSE;
}


/**************************************************************************\
*
*  function:  IsRBOM()
*
* true iff pb points to a reversed Byte Order Mark.
*
\**************************************************************************/
BOOL IsRBOM (PBYTE pb)
{
  if ((*pb == 0xFE) & (*(pb+1) == 0xFF))  // RBOM
    return TRUE;
  else
    return FALSE;
}




/**************************************************************************\
*
*  function:  framechildwindow()
*
* Simply draw a 3D frame around child window.
*
\**************************************************************************/
VOID framechildwindow (HDC hdc, HWND hwndParent, HWND hwndChild)
{
RECT rect;

      GetWindowRect (hwndChild, &rect);

      /* minor hack... assumes RECT is two points, right field starting first */
      ScreenToClient (hwndParent, (LPPOINT)&rect);
      ScreenToClient (hwndParent, (LPPOINT)&(rect.right));

      InflateRect (&rect, 1, 1);
      FrameRect (hdc, &rect, GetStockObject (GRAY_BRUSH));
      InflateRect (&rect, -1, -1);
      SelectObject (hdc, GetStockObject (WHITE_PEN));
      MoveToEx (hdc, rect.right, rect.top, NULL);
      LineTo (hdc,rect.right, rect.bottom);
      LineTo (hdc,rect.left, rect.bottom);

    return;
}


/**************************************************************************\
*
*  function:  underlinechildwindow()
*
* Underline child window.
*
\**************************************************************************/
VOID underlinechildwindow (HDC hdc, HWND hwndParent, HWND hwndChild)
{
RECT rect;

      GetWindowRect (hwndChild, &rect);

      /* minor hack... assumes RECT is two points, right field starting first */
      ScreenToClient (hwndParent, (LPPOINT)&rect);
      ScreenToClient (hwndParent, (LPPOINT)&(rect.right));

      InflateRect (&rect, 1, 1);
      rect.top = rect.bottom-1;
      FrameRect (hdc, &rect, GetStockObject (GRAY_BRUSH));
      SelectObject (hdc, GetStockObject (WHITE_PEN));
      MoveToEx (hdc, rect.right, rect.bottom, NULL);
      LineTo (hdc,rect.left, rect.bottom);

    return;
}







/**************************************************************************\
*
*  function:  createwindows()
*
* Create the child windows and pass the handles back in parameters.
*  Each Window is created relative to (inside of) prect.
*  top is a spacial pointer to the Y coordinate of the next window.
*
\**************************************************************************/
VOID createwindows(PRECT prect,
                   HWND  hwndParent,
                   HWND* hwndLabel,
                   HWND* hwndName,
                   HWND* hwndSize,
                   HWND* hwndCodePage,
                   HWND* hwndByteOrder,
                   HWND* hwndButton)
{
int top;

  top = prect->top;
  *hwndLabel = CreateWindow(
          TEXT("STATIC"),
          szBlank,
          WS_CHILD | WS_VISIBLE | SS_CENTER,
          prect->left,
          top,
          prect->right - prect->left,
          WHEIGHT,
          hwndParent, NULL, hInst, 0);

  top += WHEIGHT*5/2;
  *hwndName = CreateWindow(
          TEXT("STATIC"),
          szBlank,
          WS_CHILD | WS_VISIBLE | SS_RIGHT,
          prect->left,
          top,
          prect->right - prect->left,
          WHEIGHT,
          hwndParent, NULL, hInst, 0);

  top += WHEIGHT*2;
  *hwndSize = CreateWindow(
          TEXT("STATIC"),
          szBlank,
          WS_CHILD | WS_VISIBLE | SS_LEFT,
          prect->left,
          top,
          (prect->right - prect->left) *3/4,
          WHEIGHT,
          hwndParent, NULL, hInst, 0);

  top += WHEIGHT*2;
  *hwndCodePage = CreateWindow(
          TEXT("STATIC"),
          szBlank,
          WS_CHILD | WS_VISIBLE | SS_LEFT,
          prect->left,
          top,
          (prect->right - prect->left) *3/4,
          WHEIGHT,
          hwndParent, NULL, hInst, 0);

  top += WHEIGHT*2;
  *hwndByteOrder = CreateWindow(
          TEXT("STATIC"),
          szBlank,
          WS_CHILD | WS_VISIBLE | SS_LEFT,
          prect->left,
          top,
          (prect->right - prect->left) *3/4,
          WHEIGHT,
          hwndParent, NULL, hInst, 0);

  top += WHEIGHT*2;
  *hwndButton = CreateWindow(
          TEXT("BUTTON"),
          TEXT("ViewText"),
          WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
          prect->left,
          top,
          prect->right - prect->left,
          WHEIGHT*9/7,
          hwndParent, NULL, hInst, 0);

  return;
}


/**************************************************************************\
*
*  function:  ManageMemory()
*
* Do all memory management here for the source and destination pointers.
*  We also enable/disable the "View..." buttons to reflect existence of data.
*
*
* PARAMETERS
*
*  message : {MMALLOC, MMFREE}
*    Alloc when requested by MMALLOC, and free the existing, passed in, pointer.
*    Free when requested by MMFREE.
*  sourcedestination : {MMSOURCE, MMDESTINATION}
*  nBytes - number to alloc on MMALLOC messages.
*  p - old pointer to be freed.
*
*
* GLOBALS
*
*  hwndButton0, hwndButton1
*
\**************************************************************************/
LPVOID ManageMemory (UINT message, UINT sourcedestination, DWORD nBytes, LPVOID p)
{
  switch (message) {
    case MMFREE :
      if (sourcedestination == MMSOURCE)
        EnableWindow (hwndButton0, FALSE);
      else if (sourcedestination == MMDESTINATION)
        EnableWindow (hwndButton1, FALSE);

      if (p != NULL) GlobalFree (GlobalHandle (p));
      return NULL;
    break;

    case MMALLOC :
      if (sourcedestination == MMSOURCE)
        EnableWindow (hwndButton0, TRUE);
      else if (sourcedestination == MMDESTINATION)
        EnableWindow (hwndButton1, TRUE);

      if (p != NULL) GlobalFree (GlobalHandle (p));
      p = (LPVOID) GlobalAlloc (GPTR, nBytes);
      return p;
    break;

  } /* end switch (message) */
  return NULL;
}


/**************************************************************************\
*
*  function:  LoadResourceString()
*
*  Loads a resource string from string table and returns a pointer
*  to the string.
*
*  PARAMETERS: wID - resource string id
*
\**************************************************************************/
LPTSTR LoadResourceString(UINT wID)
{
    static TCHAR szBuf[512];

    LoadString((HANDLE)GetModuleHandle(NULL),wID,szBuf,sizeof(szBuf));
    return szBuf;
}
