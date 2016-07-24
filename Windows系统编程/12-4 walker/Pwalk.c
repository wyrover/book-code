
#include "pwalk.h"

#define strtok        My_mbstok

#define MAX_DRIVES    26    /* maximum number of logical drives */

/* system constants used externally */
int		    xChar,
		    yChar,
		    xScreen,
		    yScreen,
		    yFrame,
		    xFrame,
		    yCaption,
		    xVScrollBar;

char		szCaptionText[] ="Address\0State\0Prot\0Size\0BaseAddr\0Object\0Section\0Name\0";
char		szFormat[] = "%08lX~%s ~%s ~%lu~%08lX~%s ~%s ~%s ~";
SIZE		sChar0;
char		szFormatPages[] = "%05lX~%s ~%s ~%lu~%05lX~%s ~%s ~%s ~";
int			taColumns[] = {TA_RIGHT,  TA_LEFT, TA_LEFT, TA_RIGHT,
							TA_RIGHT, TA_RIGHT, TA_LEFT,  TA_LEFT,
							};
int			xColumns[]  = {      8,       9,      17,       31,
								40,      46,      47,       55,
							};
BOOL        bNumbersAsBytes = TRUE;
HFONT       hFontVar;

char        szFilePath[MAX_PATH] = "";
char        szFilename[MAX_PATH] = "";
HFONT       hFont;
LPVOID      lpWalkerList = NULL;
int         *Objects;
int		     nSortType = IDM_SORTADDRESS;
HWND		     hWndSysStat, hWndProStat, hInitDlg, hMemWnd;
HANDLE		     hChildEvents[nDEBUGEVENTS];
DBGPROCESS	     *lpChildProcess = NULL;
HMENU		     hPopup[MENUPOPUPS];
char		     szCurPath[MAX_PATH];


/* local functions */
BOOL   WINAPI InitEnvironment (HANDLE, int, char *);
void   WINAPI InitMenu (HWND);
void   WINAPI DrawListItem (DRAWITEMSTRUCT *);
int    WINAPI MakeVMQString (int, char *);
DWORD  WINAPI VMExceptionFilter (EXCEPTION_POINTERS *);
void   WINAPI SortList (HWND, int);
BOOL   WINAPI ViewableMemorySelection (HWND);
static void TextOutFields (HDC, int, LPRECT, LPSTR);


/****************************************************************************
    My_mbschr:  strchr() DBCS version
****************************************************************************/
unsigned char * _CRTAPI1 My_mbschr(
    unsigned char *psz, unsigned short uiSep)
{
    while (*psz != '\0' && *psz != uiSep) {
        psz = CharNext(psz);
    }
    if (*psz == '\0' && uiSep != '\0') {
        return NULL;
    } else {
        return psz;
    }
}
/****************************************************************************
    My_mbstok:  strtok() DBCS version
****************************************************************************/
unsigned char * _CRTAPI1 My_mbstok(
    unsigned char *pszSrc, unsigned char *pszSep)
{
    static char *pszSave = NULL;
    char *pszHead;
    char *psz;

    if (pszSrc == NULL) {
        if (pszSave == NULL) {
            return NULL;
        } else {
            psz = pszSave;
        }
    } else {
        psz = pszSrc;
    }

    /*********************************************/
    /* Skip delimiters to find a head of a token */
    /*********************************************/
    while (*psz) {
        if (IsDBCSLeadByte(*psz)) {
            break;
        } else if (NULL == My_mbschr(pszSep, *psz)) {
            break;
        }
        psz++;
    }
    if (*psz == '\0') {
        //No more token
        return (pszSave = NULL);
    }
    pszHead = psz;

    /******************************/
    /* Search a Tail of the token */
    /******************************/
    while (*psz) {
        if (IsDBCSLeadByte(*psz)) {
            psz += 2;
            continue;
        } else if (NULL != My_mbschr(pszSep, *psz)) {
            break;
        }
        psz++;
    }
    if (*psz == '\0') {
        pszSave = NULL;
    } else {
        //Found next delimiter
        pszSave = psz + 1;
        *psz = '\0';
    }
    return pszHead;
}


/* entry point of this executable */
int WINAPI WinMain (hInstance, hPrevInstance, lpCmdLine, nCmdShow)
    HINSTANCE hInstance;
    HINSTANCE hPrevInstance;
    LPSTR     lpCmdLine;
    int       nCmdShow;
{
    MSG      msg;
    char     *lpszCmdLine = NULL;
    char     *lpCL;
    BOOL     bSwitch;

    /* set current path for use later */
    GetCurrentDirectory (MAX_PATH, szCurPath);

    // parse and copy command line parameters to local memory
    lpCL = GetCommandLine ();
    if (lpszCmdLine = (char *)LocalAlloc (LPTR, strlen (lpCL) + 1))
	GetCmdLine (lpCL, lpszCmdLine, &bSwitch);

    /* start window environment */
    if (!InitEnvironment (hInstance, nCmdShow, IsValidFile (lpszCmdLine) ? lpszCmdLine : NULL))
	return FALSE;

    /* free memory allocated for lpCmdLine */
    if (lpszCmdLine)
	LocalFree ((HLOCAL)lpszCmdLine);

    /* main window message loop */
    while (GetMessage (&msg, NULL, 0, 0))
	{
	TranslateMessage (&msg);
	DispatchMessage (&msg);
	}

    /* return success of application */
    return TRUE;
}



/*  start app */
BOOL WINAPI InitEnvironment (
    HANDLE    hInstance,
    int       nCmdShow,
    char      *lpszCmdLine)
    {
    WNDCLASS   wc;
    char       szClass[MAX_PATH];
    char       szTitle[MAX_PATH];
    char       szFilename[MAX_PATH];
    HWND       hWnd;


    /* register system statistics window class */
    LoadString (hInstance, IDS_SYSSTATCLASS, szClass, sizeof (szClass));
    wc.style	     = 0;
    wc.lpfnWndProc   = (WNDPROC)SysStatWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon	     = LoadIcon (hInstance, MAKEINTRESOURCE (IDR_SYSSTATICON));
    wc.hCursor	     = LoadCursor (0, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = szClass;
    if (!RegisterClass (&wc) )
	return FALSE;

    /* register process statistics window class */
    LoadString (hInstance, IDS_PROSTATCLASS, szClass, sizeof (szClass));
    wc.style	     = 0;
    wc.lpfnWndProc   = (WNDPROC)ProStatWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon	     = LoadIcon (hInstance, MAKEINTRESOURCE (IDR_PROSTATICON));
    wc.hCursor	     = LoadCursor (0, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = szClass;
    if (!RegisterClass (&wc) )
	return FALSE;

    /* register the status bar window class */
    LoadString (hInstance, IDS_STATUSCLASS, szClass, sizeof (szClass));
    wc.style	     = 0;
    wc.lpfnWndProc   = (WNDPROC)StatusWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = STATUSWXB;
    wc.hInstance     = hInstance;
    wc.hIcon	     = (HICON)NULL;
    wc.hCursor	     = LoadCursor (0, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = szClass;
    if (!RegisterClass (&wc) )
	return FALSE;

    /* register the main frame window class */
    LoadString (hInstance, IDS_MEMVIEWCLASS, szClass, sizeof (szClass));
    wc.style	     = 0;
    wc.lpfnWndProc   = (WNDPROC)MemWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = VIEWWXB;
    wc.hInstance     = hInstance;
    wc.hIcon	     = LoadIcon (hInstance, MAKEINTRESOURCE (IDR_MAINICON));
    wc.hCursor	     = LoadCursor (0, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = szClass;
    if (!RegisterClass (&wc) )
	return FALSE;

    /* register the main frame window class */
    LoadString (hInstance, IDS_WALKERCLASS, szClass, sizeof (szClass));
    wc.style	     = 0;
    wc.lpfnWndProc   = (WNDPROC)WalkerWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon	     = LoadIcon (hInstance, MAKEINTRESOURCE (IDR_MAINICON));
    wc.hCursor	     = LoadCursor (0, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = MAKEINTRESOURCE (IDR_WALKERMENU);
    wc.lpszClassName = szClass;
    if (!RegisterClass (&wc) )
	return FALSE;

    /* create window caption */
    LoadString (hInstance, IDS_CAPTION, szTitle, sizeof (szTitle));
    if (lpszCmdLine != NULL &&
	((lpChildProcess = StartChildProcess (NULL, lpszCmdLine, hChildEvents)) != NULL))
	GetFileFromPath (lpszCmdLine, szFilename);
    else
	LoadString (hInstance, IDS_SELF, szFilename, MAX_PATH);
    strcat (szTitle, szFilename);

    /* create main frame window */
    hWnd = CreateWindow (szClass,
			 szTitle,
			 WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
			 CW_USEDEFAULT,
			 0,
			 CW_USEDEFAULT,
			 0,
			 NULL,
			 NULL,
			 hInstance,
			 NULL);

    /* update parent window handle in child process information structure */
    if (lpChildProcess != NULL)
	lpChildProcess->hWnd = hWnd;

    if (!hWnd)
	return 0;

    ShowWindow (hWnd, nCmdShow);
    UpdateWindow (hWnd);
    return TRUE;
}


/* main window procedure */
LONG WINAPI WalkerWndProc (
    HWND    hWnd,
    UINT    uMsg,
    WPARAM  wParam,
    LPARAM  lParam)
{
       LONG    lRet = 1;

static LOGFONT lf_Font = {
			 -10, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
			 OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			 DEFAULT_QUALITY, FIXED_PITCH | FF_DONTCARE,
                         "Courier",
			 };
static LOGFONT lf_FontVar = {
  			 -10, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
			 OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			 DEFAULT_QUALITY, 0, // FIXED_PITCH | FF_DONTCARE,
                         "MS Sans Serif",
			 };


    /* On a Japanese system, change the fonts...  should generalize to all FE */
    if (PRIMARYLANGID(GetUserDefaultLangID ()) == LANG_JAPANESE) {
        lf_Font.lfCharSet = SHIFTJIS_CHARSET;
        lstrcpy (lf_Font.lfFaceName , "Terminal");

        lf_FontVar.lfHeight= -12;
        lf_FontVar.lfCharSet = SHIFTJIS_CHARSET;
        lstrcpy (lf_Font.lfFaceName , "ÇlÇr ÉSÉVÉbÉN");
    }



    switch (uMsg)
	{
	/* initialize menu before drawing */
	case WM_INITMENU:
	    InitMenu (hWnd);
	    break;

	/* display status messages for menu commands */
	case WM_MENUSELECT:
	    {
	    char     *lpszMenuString;

	    lpszMenuString = LocalAlloc (LPTR, MAX_PATH);

	    if (HIWORD (wParam) ==  0xFFFF)
		{
		LocalFree (lpszMenuString);
		lpszMenuString = NULL;
		}

	    else if (HIWORD (wParam) & MF_POPUP)
		{
		int	 i;
		HMENU	 hPopupMenu = GetSubMenu ((HMENU)lParam, LOWORD (wParam));

		lpszMenuString = LocalAlloc (LPTR, MAX_PATH);

		for (i=0; i<MENUPOPUPS; i++)
		    {
		    if (hPopup[i] == hPopupMenu)
			{
			LoadString (GetModuleHandle (NULL),
				    i+IDM_POPUPMENUS,
				    lpszMenuString,
				    MAX_PATH);
			break;
			}
		    }
		}
	    else
		LoadString (GetModuleHandle (NULL),
			    LOWORD (wParam),
			    lpszMenuString,
			    MAX_PATH);

	    /* send the status string, gray if necessary */
	    SendMessage (GetDlgItem (hWnd, IDC_STATUSWND),
			 WM_SETTEXT,
			 (HIWORD (wParam) & MF_GRAYED) ?
			     (LPARAM)GetSysColor (COLOR_GRAYTEXT):
			     0,
			 (WPARAM)lpszMenuString);

	    LocalFree (lpszMenuString);
	    }
	    break;

	case WM_CREATE:
	    {
	    HCURSOR	  hOldCursor;
	    HDC 	  hDC;
	    TEXTMETRIC	  tm;
	    char	  szWndClass[MAX_PATH];
	    HWND	  hList, hStatus;
	    int 	  i=0, j, k;
	    HMENU	  hMenu;

	    /* build array of popup menu handles */
	    hMenu = GetMenu (hWnd);
	    for (k=0; k<MENUPOPUPS-(i-1); k++)
		{
		hPopup[i] = GetSubMenu (hMenu, k);
		j=0;
		while ((hPopup[i+j+1] = GetSubMenu (hPopup[i], j)) != NULL)
		    j++;
		if (j)
		    i+=j;
		i++;
		}

	    /* put hourglass cursor up */
	    hOldCursor = (HCURSOR)SetClassLong (hWnd, GCL_HCURSOR, 0);
	    SetCursor (LoadCursor (0, IDC_WAIT));

	    hDC = GetDC(hWnd);

	    /* want a font with point size of 10 (smallest size it comes in) */
	    lf_Font.lfHeight = -(10 * GetDeviceCaps(hDC, LOGPIXELSY)/72);
	    hFont = CreateFontIndirect(&lf_Font);
	    hFontVar = CreateFontIndirect(&lf_FontVar);


            // find the width of a '0' in the variable font
            //
            SelectObject(hDC, hFontVar);
            GetTextExtentPoint (hDC, "0", 1, &sChar0);

            SelectObject(hDC, hFont);

	    /* initialize system constants */
	    GetTextMetrics(hDC, &tm);
	    yChar = tm.tmHeight;
	    xChar = tm.tmAveCharWidth;
	    xScreen = GetSystemMetrics(SM_CXSCREEN);
	    yScreen = GetSystemMetrics(SM_CYSCREEN);
	    yFrame = GetSystemMetrics(SM_CYFRAME);
	    xFrame = GetSystemMetrics(SM_CXFRAME);
	    yCaption = GetSystemMetrics(SM_CYCAPTION);
	    xVScrollBar = GetSystemMetrics(SM_CXVSCROLL);

	    //Actually, it's not good that a width of each column
	    // depends on a width of "0"!!
	    if (xChar > sChar0.cx) {
//		sChar0.cx = xChar;
		sChar0.cx = sChar0.cx * 12 / 10;	//sChar0.cx *= 1.2
	    }

            SelectObject(hDC, GetStockObject(SYSTEM_FONT));
            ReleaseDC(hWnd, hDC);

	    /* create listbox for client area */
	    LoadString (GetModuleHandle (NULL),
			IDS_LISTBOX,
			szWndClass,
			sizeof (szWndClass));
	    hList = CreateWindow (szWndClass,
				  NULL,
				  WS_CHILD | WS_VISIBLE | WS_VSCROLL |
				  LBS_EXTENDEDSEL | LBS_NOTIFY | LBS_OWNERDRAWFIXED |
				  LBS_WANTKEYBOARDINPUT | LBS_NOINTEGRALHEIGHT,
				  0, 0, 0, 0,
				  hWnd,
				  (HMENU)IDC_LISTBOX,
				  GetModuleHandle (NULL),
				  NULL);

	    /* if listbox failed, abort app */
	    if (!IsWindow (hList))
		DestroyWindow(hWnd);
	    SendMessage (hList, WM_SETFONT, (WPARAM)hFontVar, 0L);

	    /* create status window for client area */
	    LoadString (GetModuleHandle (NULL),
			IDS_STATUSCLASS,
			szWndClass,
			sizeof (szWndClass));
	    if (!(hStatus = CreateWindow (szWndClass,
					  NULL,
					  WS_CHILD | WS_VISIBLE | WS_BORDER,
					  0, 0, 0, 0,
					  hWnd,
					  (HMENU)IDC_STATUSWND,
					  GetModuleHandle (NULL),
					  NULL)))
		ReportError (IDS_ERRCREATEWINDOW);

	    /* initialize status window */
	    SendMessage (GetDlgItem (hWnd, IDC_STATUSWND),
			 UM_UPDATE,
			 (WPARAM)lpChildProcess,
			 0);

	    /* if child process post message to display initialization dialog */
	    if (lpChildProcess != NULL)
		PostMessage (hWnd, UM_STARTINITDIALOG, 0, 0);

	    /* remove hourglass cursor */
	    SetClassLong (hWnd, GCL_HCURSOR, (LONG)hOldCursor);
	    SetCursor (hOldCursor);
	    }
	    break;

	case UM_STARTINITDIALOG:
	    /* start modal initializing information window */
	    DialogBoxParam (GetModuleHandle (NULL),
			    (char *)IDD_INITIALIZING,
			    hWnd,
			    InitDlgProc,
			    (LPARAM)&hInitDlg);
	    hInitDlg = NULL;
	    break;

	case WM_SETFOCUS:
	    /* keep focus in listbox when possible */
	    SetFocus (GetDlgItem (hWnd, IDC_LISTBOX));
	    break;

	case WM_MEASUREITEM:
            ((MEASUREITEMSTRUCT FAR *)lParam)->itemHeight = sChar0.cy;
            break;

	case WM_DRAWITEM:
	    DrawListItem ((DRAWITEMSTRUCT FAR *)lParam);
            break;

	case WM_PAINT:
	    {
	    PAINTSTRUCT ps;
	    RECT rc;

	    /* draw the caption line above the list box */
	    BeginPaint(hWnd, &ps);
	    SetRect(&rc, 0, 0, GetSystemMetrics (SM_CXSCREEN), sChar0.cy);

	    SelectObject(ps.hdc, hFontVar);
	    SetTextColor(ps.hdc, GetSysColor(COLOR_CAPTIONTEXT));
	    SetBkColor(ps.hdc, GetSysColor(COLOR_ACTIVECAPTION));
            TextOutFields (ps.hdc, 6, &rc, szCaptionText);
	    SelectObject(ps.hdc, GetStockObject(SYSTEM_FONT));
	    EndPaint(hWnd, &ps);
	    }
	    break;

	case WM_SIZE:
	    /* size listbox and status bar */
	    if ((wParam == SIZE_RESTORED) || (wParam == SIZE_MAXIMIZED))
		{
		int    yBorder = GetSystemMetrics (SM_CYBORDER);
		int    xBorder = GetSystemMetrics (SM_CXBORDER);
		int    yStatus = yChar + 10*yBorder;

		/* size listbox */
		MoveWindow(GetDlgItem (hWnd, IDC_LISTBOX),
			   0,
			   sChar0.cy,
			   LOWORD(lParam),
			   HIWORD(lParam)-(sChar0.cy + yStatus - yBorder),
			   TRUE);

		/* size status bar */
		MoveWindow(GetDlgItem (hWnd, IDC_STATUSWND),
			   0-xBorder,
			   HIWORD(lParam)-yStatus+yBorder,
			   LOWORD(lParam) + 2*xBorder,
			   yStatus,
			   TRUE);
		}
	    break;

	case WM_COMMAND:
	    {
	    switch (LOWORD (wParam))
		{
		case IDM_EXIT:
		    SendMessage (hWnd, WM_CLOSE, 0, 0);
		    break;

		case IDM_PROCESSUNLOAD:
		    {
		    char    szFilename[MAX_PATH];
		    char    szTitle[MAX_PATH];
		    HWND    hViewWnd = NULL;

		    /* close child process */
		    CloseChildProcess (lpChildProcess, hChildEvents);
		    lpChildProcess = NULL;
		    SendMessage (GetDlgItem (hWnd, IDC_LISTBOX), LB_RESETCONTENT, 0, 0);
		    SendMessage (GetDlgItem (hWnd, IDC_STATUSWND),
				 UM_UPDATE,
				 0,
				 0);

		    /* reset caption */
		    LoadString (GetModuleHandle (NULL),
				IDS_CAPTION,
				szTitle,
				MAX_PATH);
		    LoadString (GetModuleHandle (NULL),
				IDS_SELF,
				szFilename,
				MAX_PATH);
		    strcat (szTitle, szFilename);
		    SetWindowText (hWnd, szTitle);

		    if (IsWindow (hWndSysStat))
			{
			InvalidateRect (hWndSysStat, NULL, TRUE);
			UpdateWindow (hWndSysStat);
			}

		    if (IsWindow (hWndProStat))
			DestroyWindow (hWndProStat);

		    while ((hViewWnd = EnumViewWindows (hWnd, hViewWnd)) != NULL)
			DestroyWindow (hViewWnd);
		    }
		    break;

		case IDM_PROCESSLOAD:
		    {
		    char      szTitle[MAX_PATH];
		    char      szFilePath[MAX_PATH];
		    HWND      hViewWnd = NULL;

		    /* detaching from old process, okay?? */
		    if (lpChildProcess != NULL)
			{
			strcpy (szTitle, "Detach from process ");
			strcat (szTitle, lpChildProcess->szModule);
			strcat (szTitle, "?");

			LoadString (GetModuleHandle (NULL),
				    IDS_WALKERCLASS,
				    szFilePath,
				    MAX_PATH);
			if (IDYES != MessageBox (hWnd,
						 szTitle,
						 szFilePath,
						 MB_YESNO | MB_ICONQUESTION))
			    break;
			}

		    /* call open file dialog to get filename of exe, and validate */
		    *szFilePath = 0;
		    if (GetFileName (hWnd, szFilePath, NULL))
			{
			if (IsValidFile (szFilePath))
			    {
			    if (lpChildProcess != NULL)
				{
				/* close any open view windows	for this process */
				while ((hViewWnd = EnumViewWindows (hWnd, hViewWnd)) != NULL)
				    DestroyWindow (hViewWnd);

				CloseChildProcess (lpChildProcess, hChildEvents);
				SendMessage (GetDlgItem (hWnd, IDC_LISTBOX),
					     LB_RESETCONTENT,
					     0,
					     0);
				SendMessage (GetDlgItem (hWnd, IDC_STATUSWND),
					     UM_UPDATE,
					     0,
					     0);
				}

			    if ((lpChildProcess =
				 StartChildProcess (hWnd, szFilePath, hChildEvents)) != NULL)
				{
				/* force rewalk of process */
				PostMessage (hWnd, UM_STARTINITDIALOG, 0, 0);
				SendMessage (GetDlgItem (hWnd, IDC_STATUSWND),
					     UM_UPDATE,
					     (WPARAM)lpChildProcess,
					     0);

				/* load new window caption */
				LoadString (GetModuleHandle (NULL),
					    IDS_CAPTION,
					    szTitle,
					    MAX_PATH);

				GetFileFromPath (szFilePath, szFilename);
				strcat (szTitle, szFilename);
				SetWindowText (hWnd, szTitle);
				}
			    }
			}
		    }
		    break;

		case IDM_PROCESSREWALK:
		    {
		    HWND      hList = GetDlgItem (hWnd, IDC_LISTBOX);
		    int       nCnt, nNewCnt, i;
		    LPVOID    lpNewList=NULL, lpTempList=NULL;
		    HWND      hViewWnd = NULL;


		    /* clear listbox of current contents, but first find out how many exist */
		    nCnt = SendMessage (hList, LB_GETCOUNT, 0, 0);
		    SendMessage (hList, WM_SETREDRAW, 0, 0);
		    SendMessage (hList, LB_RESETCONTENT, 0, 0);

		    /* walk process address space */
		    if (lpChildProcess != NULL)
			{
			nNewCnt = WalkProcess (lpChildProcess->hProcess, &lpNewList, &Objects);
			AnalyzeProcess (lpChildProcess, (LPVMOBJECT)lpNewList, nNewCnt);

			/* indentify which objects are new */
			if (nCnt)
			    IdentifyNewObjects (lpWalkerList, nCnt, lpNewList, nNewCnt);

			/* free old list and update cnt */
			lpTempList = lpWalkerList;
			lpWalkerList = lpNewList;
			VirtualFree (lpTempList, TOTALVMRESERVE, MEM_DECOMMIT);
			VirtualFree (lpTempList, 0, MEM_RELEASE);
			nCnt = nNewCnt;
			}

		    for (i=0; i<nCnt; i++)
			SendMessage (hList, LB_ADDSTRING, 0, i);

		    /* sort if other than by address is selected */
		    if (nSortType != IDM_SORTADDRESS)
			SortList (hList, nSortType);

		    /* reenable redraw of listbox */
		    SendMessage (hList, WM_SETREDRAW, 1, 0);
		    InvalidateRect (hList, NULL, TRUE);
		    UpdateWindow (hList);

		    /* if any memory view windows, send update message */
		    while ((hViewWnd = EnumViewWindows (hWnd, hViewWnd)) != NULL)
			{
			LPMEMVIEW		    pmv, pmvOld;
			int			    nAddress, nSize;
			MEMORY_BASIC_INFORMATION    mbi;
			char			    *szCaption;

			/* retrieve view memory range */
			szCaption = HeapAlloc (GetProcessHeap (), HEAP_ZERO_MEMORY, MAX_PATH);
			GetWindowText (hViewWnd, szCaption, MAX_PATH);

			/* validate range */
			sscanf (strtok (szCaption, "-"), "%8x", &nAddress);
			sscanf (strtok (NULL, " \0"), "%8x", &nSize);
			nSize -= nAddress;
			VirtualQueryEx (lpChildProcess->hProcess,
					(LPVOID)nAddress,
					&mbi,
					sizeof (MEMORY_BASIC_INFORMATION));

			if (mbi.State != MEM_COMMIT)
			    {
			    NotifyUser (hWnd, IDS_ERROR, IDS_NOTCOMMITTEDMEMORY, NULL, 0);
			    DestroyWindow (hViewWnd);
			    goto NOT;
			    }

			/* if size of committed region changed, update caption */
			if (mbi.RegionSize != (DWORD)nSize)
			    {
			    wsprintf (szCaption,
				      "%4lx-%-4lx",
				      (DWORD)mbi.BaseAddress,
				      (DWORD)mbi.BaseAddress+mbi.RegionSize);
			    SetWindowText (hViewWnd, szCaption);
			    }

			/* free default heap memory */
			HeapFree (GetProcessHeap (), 0, szCaption);

			/* if an old view structure existed, release virtual memory */
			if ((pmvOld = (LPMEMVIEW)GetWindowLong (hViewWnd, WXB_LPOLDMEMVIEW)) != NULL)
			    VirtualFree (pmvOld->lpMem, 0, MEM_RELEASE);

			pmvOld = (LPMEMVIEW)GetWindowLong (hViewWnd, WXB_LPMEMVIEW);
			/* save past pmv for update comparison */
			SetWindowLong (hViewWnd,
				       WXB_LPOLDMEMVIEW,
				       (LONG)pmvOld);

			/* allocate memory structure for view memory object */
			pmv = (LPMEMVIEW)LocalAlloc (LPTR, sizeof (MEMVIEW));

			/* copy old mem view to new mem view */
			for (i=0; i<sizeof (MEMVIEW); i++)
			    ((LPBYTE)pmv)[i] = ((LPBYTE)pmvOld)[i];

			/* update structure for new mem structure */
			pmv->nBase = (int)mbi.BaseAddress;
			pmv->nSize = (int)mbi.RegionSize;

			if ((pmv->lpMem = VirtualAlloc (NULL, pmv->nSize, MEM_COMMIT, PAGE_READWRITE)) == NULL)
			    {
			    ReportError (IDS_ERRVIRTUALALLOC);
			    DestroyWindow (hViewWnd);
			    }

			else if (AccessProcessMemory (hChildEvents[READMEMORY],
						      hChildEvents[ACKNOWLEDGE],
						      (LPVOID)nAddress,
						      pmv->lpMem,
						      &(pmv->nSize))  && pmv->nSize)
			    {
			    pmv->nLines = (pmv->nSize+15)/16;
			    pmv->nExtraBytes = (pmv->nSize & 0x0000000F);
			    SetWindowLong (hViewWnd, WXB_LPMEMVIEW, (LONG)pmv);

			    /* post message to view window to update */
			    PostMessage (hViewWnd, UM_UPDATE, 0, 0);
			    }

			else
			    {
			    NotifyUser (hWnd, IDS_ERROR, IDS_COULDNOTREADPROCESS, NULL, 0);
			    DestroyWindow (hViewWnd);
			    }

			}

NOT:
		    /* if initialization dialog, send notification to remove */
		    if (IsWindow (hInitDlg))
			PostMessage (hInitDlg, UM_ENDDIALOG, 0, 0);
		    }
		    break;

		case IDM_PROCESSSUSPEND:
		    SetEvent (hChildEvents[SUSPENDDEBUGGER]);
		    break;

		case IDM_PROCESSRESUME:
		    SetEvent (hChildEvents[RESUMEDEBUGGER]);
		    break;

		case IDM_VIEWSYSSTAT:
		    /* if window exists, destroy it */
		    if (IsWindow (hWndSysStat))
			{
			DestroyWindow (hWndSysStat);
			CheckMenuItem (GetMenu (hWnd), wParam, MF_UNCHECKED);
			}
		    else
			{
			char	szClass[100];
			char	szTitle[100];
			RECT	rc;

			GetWindowRect (hWnd, &rc);
			LoadString (GetModuleHandle (NULL), IDS_SYSSTATCLASS, szClass, 100);
			LoadString (GetModuleHandle (NULL), IDS_SYSSTATTITLE, szTitle, 100);
			hWndSysStat = CreateWindow (szClass,
						    szTitle,
						    WS_POPUP | WS_CAPTION | WS_MINIMIZEBOX |
						    WS_SYSMENU | WS_DLGFRAME | WS_VISIBLE,
						    rc.left+50, rc.top+50, 500, 270,
						    hWnd,
						    NULL,
						    GetModuleHandle (NULL),
						    NULL);
			UpdateWindow (hWndSysStat);
			ShowWindow (hWndSysStat, SW_SHOWNORMAL);
			CheckMenuItem (GetMenu (hWnd), wParam, MF_CHECKED);
			}
		    break;

		case IDM_VIEWPROSTAT:
		    /* if window exists, destroy it */
		    if (IsWindow (hWndProStat))
			{
			DestroyWindow (hWndProStat);
			CheckMenuItem (GetMenu (hWnd), wParam, MF_UNCHECKED);
			}
		    else
			{
			char	szClass[100];
			char	szTitle[100];
			RECT	rc;

			GetWindowRect (hWnd, &rc);
			LoadString (GetModuleHandle (NULL), IDS_PROSTATCLASS, szClass, 100);
			LoadString (GetModuleHandle (NULL), IDS_PROSTATTITLE, szTitle, 100);
			hWndProStat = CreateWindow (szClass,
						    szTitle,
						    WS_POPUP | WS_CAPTION | WS_MINIMIZEBOX |
						    WS_SYSMENU | WS_DLGFRAME | WS_VISIBLE,
						    rc.left+75, rc.top+75, 355, 120,
						    hWnd,
						    NULL,
						    GetModuleHandle (NULL),
						    NULL);
			UpdateWindow (hWndProStat);
			ShowWindow (hWndProStat, SW_SHOWNORMAL);
			CheckMenuItem (GetMenu (hWnd), wParam, MF_CHECKED);
			}
		    break;

		/* accept bouble click messages from listbox only */
		case IDC_LISTBOX:
		    if (HIWORD (wParam) != LBN_DBLCLK)
			break;

		case IDM_VIEWMEMORY:
		    if (ViewableMemorySelection (hWnd))
			{
			char	  szBuff[50];
			HWND	  hList = GetDlgItem (hWnd, IDC_LISTBOX);
			int	  iCaret = SendMessage (hList, LB_GETCARETINDEX, 0, 0);
			DWORD	  nAddress =
				  (DWORD)((LPVMOBJECT)lpWalkerList)[Objects[iCaret]].mbi.BaseAddress;
			int	  nSize = ((LPVMOBJECT)lpWalkerList)[Objects[iCaret]].mbi.RegionSize;
			LPVOID	  lpMem;
			HCURSOR   hOldCursor;

			if ((lpMem = VirtualAlloc (NULL, nSize, MEM_COMMIT, PAGE_READWRITE)) == NULL)
			    {
			    ReportError (IDS_ERRVIRTUALALLOC);
			    break;
			    }

			/* put wait cursor up */
			hOldCursor = (HCURSOR)SetClassLong (hWnd, GCL_HCURSOR, 0);
			SetCursor (LoadCursor (0, IDC_WAIT));

			/* signal debugger thread to read process memory */
			if (AccessProcessMemory (hChildEvents[READMEMORY],
						 hChildEvents[ACKNOWLEDGE],
						 (LPVOID)nAddress,
						 lpMem,
						 &nSize)  && nSize)
			    {
			    wsprintf (szBuff, "%4lx-%-4lx", nAddress, nAddress+nSize);
			    ViewMemory (hWnd, szBuff, lpMem, nSize, nAddress);

			    /* if first view window, add separator */
			    if (GetMenuItemCount (GetSubMenu (GetMenu (hWnd), 2)) == 5)
				AppendMenu (GetSubMenu (GetMenu (hWnd), 2),
					    MF_SEPARATOR,
					    0,
					    NULL);

			    AppendMenu (GetSubMenu (GetMenu (hWnd), 2),
					MF_STRING | MF_CHECKED,
					AddAtom (szBuff),
					szBuff);
			    }

			else
			    NotifyUser (hWnd, IDS_ERROR, IDS_COULDNOTREADPROCESS, NULL, 0);


			/* replace wait cursor with old cursor */
			SetClassLong (hWnd, GCL_HCURSOR, (LONG)hOldCursor);
			SetCursor (hOldCursor);
			}
		    else
			{
			NotifyUser (hWnd, IDS_ERROR, IDS_NOTCOMMITTEDMEMORY, NULL, 0);
			break;
			}
		    break;

		case IDM_VIEWADDRESS:
		    {
		    int 			nAddress;
		    MEMORY_BASIC_INFORMATION	mbi;
		    LPVOID			lpMem;
		    char			szBuff[MAX_PATH];
		    int 			nLine;
		    HWND			hViewWnd;


		    if (nAddress = DialogBox (GetModuleHandle (NULL), (char *)IDD_ADDR, hWnd, AddrDlgProc))
			{
			VirtualQueryEx (lpChildProcess->hProcess,
					(LPVOID)nAddress,
					&mbi,
					sizeof (MEMORY_BASIC_INFORMATION));

			if (mbi.State != MEM_COMMIT)
			    {
			    NotifyUser (hWnd, IDS_ERROR, IDS_NOTCOMMITTEDMEMORY, NULL, 0);
			    break;
			    }

			if ((lpMem = VirtualAlloc (NULL, mbi.RegionSize, MEM_COMMIT, PAGE_READWRITE)) == NULL)
			    {
			    ReportError (IDS_ERRVIRTUALALLOC);
			    break;
			    }

			/* signal debugger thread to read process memory */
			if (AccessProcessMemory (hChildEvents[READMEMORY],
						 hChildEvents[ACKNOWLEDGE],
						 (LPVOID)mbi.BaseAddress,
						 lpMem,
						 &(mbi.RegionSize))  && mbi.RegionSize)
			    {
			    wsprintf (szBuff,
				      "%4lx-%-4lx",
				      (int)mbi.BaseAddress,
				      (int)mbi.BaseAddress+mbi.RegionSize);

			    hViewWnd = ViewMemory (hWnd, szBuff, lpMem, mbi.RegionSize, (int)mbi.BaseAddress);

			    /* if first view window, add separator */
			    if (GetMenuItemCount (GetSubMenu (GetMenu (hWnd), 2)) == 4)
				AppendMenu (GetSubMenu (GetMenu (hWnd), 2),
					    MF_SEPARATOR,
					    0,
					    NULL);

			    AppendMenu (GetSubMenu (GetMenu (hWnd), 2),
					MF_STRING | MF_CHECKED,
					AddAtom (szBuff),
					szBuff);

			    /* send WM_VSCROLL message to scroll address into view */
			    nLine = (nAddress - (int)mbi.BaseAddress)/16 - 5;
			    PostMessage (hViewWnd, WM_VSCROLL, MAKELONG (SB_THUMBPOSITION, nLine), 0);
			    }

			else
			    NotifyUser (hWnd, IDS_ERROR, IDS_COULDNOTREADPROCESS, NULL, 0);
			}
		    }
		    break;

		case IDM_REMOVEVIEWWND:
		    {
		    ATOM    aCaption = FindAtom ((char *)lParam);
		    HMENU   hMenu = GetMenu (hWnd);
		    HMENU   hViewMenu = GetSubMenu (hMenu, 2);

		    RemoveMenu (hMenu, (UINT)aCaption, MF_BYCOMMAND);
		    DeleteAtom (aCaption);

		    /* there are 4 menuitems in the view menu without view windows open */
		    if (GetMenuItemCount (hViewMenu) == 6)
			RemoveMenu (hViewMenu, 5, MF_BYPOSITION);
		    }
		    break;

		case IDM_SORTADDRESS:
		case IDM_SORTSTATE:
		case IDM_SORTPROTECTION:
		case IDM_SORTSIZE:
		case IDM_SORTBASEADDRESS:
		    {
		    HWND    hList = GetDlgItem (hWnd, IDC_LISTBOX);
		    HCURSOR hOldCursor;

		    if (nSortType != (int)LOWORD (wParam))
			{
			/* put wait cursor up */
			hOldCursor = (HCURSOR)SetClassLong (hWnd, GCL_HCURSOR, 0);
			SetCursor (LoadCursor (0, IDC_WAIT));

			/* reset menuitems to indicate which sort method is being used */
			CheckMenuItem (GetMenu (hWnd), nSortType, MF_UNCHECKED);
			CheckMenuItem (GetMenu (hWnd), wParam, MF_CHECKED);

			/* save new sort type and resort */
			SortList (hList, nSortType = wParam);

			/* repaint after sorting */
			InvalidateRect (hList, NULL, TRUE);
			UpdateWindow (hList);

			/* replace wait cursor with old cursor */
			SetClassLong (hWnd, GCL_HCURSOR, (LONG)hOldCursor);
			SetCursor (hOldCursor);
			 }
		    }
		    break;

                case IDM_OPTBYTES:
                case IDM_OPTPAGES:
                    {
                    HWND hList = GetDlgItem (hWnd, IDC_LISTBOX);
                    bNumbersAsBytes = (LOWORD(wParam) == IDM_OPTBYTES);
                    InvalidateRect (hList, NULL, TRUE);
                    }
                    break;

		default:
		    /* if popup window, bring to front */
		    ActivateViewWindow (LOWORD (wParam));
		    lRet = TRUE;
		    break;
		}
	    }
	    break;

	case WM_CLOSE:
	case WM_QUERYENDSESSION:
	    /* if child process is active, close it first then exit */
	    if (lpChildProcess != NULL)
		CloseChildProcess (lpChildProcess, hChildEvents);

	    /* destroy this window */
	    DestroyWindow (hWnd);
	    break;

	case WM_DESTROY:
	    PostQuitMessage (0);
	    break;

	default:
	    /* pass all unhandled messages to DefWindowProc */
	    lRet = DefWindowProc (hWnd, uMsg, wParam, lParam);
	    break;
	}

    /* return 1 if handled message, 0 if not */
    return lRet;
}



/* initialize all menuitems */
void WINAPI InitMenu (
    HWND    hWnd)
{
    HMENU	hMenu = GetMenu (hWnd);

    /* if child process exists enable options */
    EnableMenuItem (hMenu,
		    IDM_PROCESSREWALK,
		    MF_BYCOMMAND | ((lpChildProcess != NULL) ? MF_ENABLED : MF_GRAYED));
    EnableMenuItem (hMenu,
		    IDM_PROCESSUNLOAD,
		    MF_BYCOMMAND | ((lpChildProcess != NULL) ? MF_ENABLED : MF_GRAYED));

    /* check appropriate sort menuitem */
    CheckMenuItem (hMenu, nSortType, MF_CHECKED);

    /* check view as bytes/pages menuitems */
    CheckMenuItem (hMenu, IDM_OPTBYTES, bNumbersAsBytes ? MF_CHECKED : MF_UNCHECKED);
    CheckMenuItem (hMenu, IDM_OPTPAGES, bNumbersAsBytes ? MF_UNCHECKED : MF_CHECKED);

    /* enable process and selection stat windows only when child process exists */
    EnableMenuItem (hMenu,
		    IDM_VIEWPROSTAT,
		    MF_BYCOMMAND | ((lpChildProcess != NULL) ? MF_ENABLED : MF_GRAYED));
    EnableMenuItem (hMenu,
		    IDM_VIEWSYSSTAT,
		    MF_BYCOMMAND | ((lpChildProcess != NULL) ? MF_ENABLED : MF_GRAYED));

    /* check all appropriate view menuitem */
    CheckMenuItem (hMenu,
		   IDM_VIEWSYSSTAT,
		   IsWindow (hWndSysStat) ? MF_CHECKED : MF_UNCHECKED);
    CheckMenuItem (hMenu,
		   IDM_VIEWPROSTAT,
		   IsWindow (hWndProStat) ? MF_CHECKED : MF_UNCHECKED);

    /* if child process exists */
    if (lpChildProcess != NULL)
	{
	/* child process is active */
	if (lpChildProcess->bActive)
	    {
	    EnableMenuItem (hMenu, IDM_PROCESSRESUME, MF_GRAYED);
	    EnableMenuItem (hMenu, IDM_PROCESSSUSPEND, MF_ENABLED);
	    }

	else
	    {
	    EnableMenuItem (hMenu, IDM_PROCESSSUSPEND, MF_GRAYED);
	    EnableMenuItem (hMenu, IDM_PROCESSRESUME, MF_ENABLED);
	    }
	}

    /* diasble both menuitems */
    else
	{
	EnableMenuItem (hMenu, IDM_PROCESSSUSPEND, MF_GRAYED);
	EnableMenuItem (hMenu, IDM_PROCESSRESUME, MF_GRAYED);
	}

    /* sort only when process exists */
    EnableMenuItem (hMenu,
		    IDM_SORTADDRESS,
		    ((lpChildProcess != NULL) ? MF_ENABLED : MF_GRAYED));
    EnableMenuItem (hMenu,
		    IDM_SORTSTATE,
		    ((lpChildProcess != NULL) ? MF_ENABLED : MF_GRAYED));
    EnableMenuItem (hMenu,
		    IDM_SORTPROTECTION,
		    ((lpChildProcess != NULL) ? MF_ENABLED : MF_GRAYED));
    EnableMenuItem (hMenu,
		    IDM_SORTSIZE,
		    ((lpChildProcess != NULL) ? MF_ENABLED : MF_GRAYED));
    EnableMenuItem (hMenu,
		    IDM_SORTBASEADDRESS,
		    ((lpChildProcess != NULL) ? MF_ENABLED : MF_GRAYED));

    /* if child process & selection, and selection is committed memory */
    if (lpChildProcess != NULL &&
	ViewableMemorySelection (hWnd))
		EnableMenuItem (hMenu, IDM_VIEWMEMORY, MF_ENABLED);
    else
		EnableMenuItem (hMenu, IDM_VIEWMEMORY, MF_GRAYED);

	/* View address if child process */
	EnableMenuItem (hMenu,
			IDM_VIEWADDRESS,
		    ((lpChildProcess != NULL) ? MF_ENABLED : MF_GRAYED));


}




int WINAPI MakeVMQString (
    int 	  nItem,
    char	  *lpszMem)
{
    char	  szState[10], szProtection[3];
    LPVMOBJECT	  lpVMObject;
    int           nLen;
    int           nBaseAddr;
    int           nRegionSize;
    int           nAllocBase;
    LPSTR         lpszFormat;

    /* lookup object offset in array index */
    lpVMObject = ((VMOBJECT *)lpWalkerList)+Objects[nItem];


    /* determine state of memory object */
    if (lpVMObject->mbi.State & MEM_COMMIT)
	strcpy (szState, "Commit ");
    else if (lpVMObject->mbi.State & MEM_RESERVE)
	strcpy (szState, "Reserve");
    else
	strcpy (szState, "Free   ");

    /* determine protection of memory */
    if (lpVMObject->mbi.Protect & PAGE_READWRITE)
	strcpy (szProtection, "RW");
    else if (lpVMObject->mbi.Protect & PAGE_READONLY)
	strcpy (szProtection, "RO");
    else
	strcpy (szProtection, "NA");

    lpszFormat = szFormat;
    nBaseAddr = (int)(lpVMObject->mbi.BaseAddress);
    nRegionSize = lpVMObject->mbi.RegionSize;
    nAllocBase = (int)(lpVMObject->mbi.AllocationBase);

    if (!bNumbersAsBytes)
       {
       nBaseAddr /= PAGESIZE;
       nRegionSize /= PAGESIZE;
       nAllocBase /= PAGESIZE;
       lpszFormat = szFormatPages;
       }

    /* create list object */
    wsprintf(lpszMem,
	     lpszFormat,
	     nBaseAddr,
	     szState,
	     szProtection,
	     nRegionSize,
	     nAllocBase,
	     lpVMObject->szObjType,
	     lpVMObject->szSection,
       	     lpVMObject->szModule);

    /* return length of resulting string */
    nLen = strlen (lpszMem);

    // convert the ~ separators to \0 for the benefit of
    // TextOutFields
    //
    while (*lpszMem)
       {
       if (*lpszMem == '~') {
           *lpszMem = 0;
           // CharNext() returns the same pointer, if 'lpszMem' points '\0'.
           // So, we must not call CharNext() in this case.
           ++lpszMem;
       } else {
           lpszMem = CharNext(lpszMem);
       }
       }

    return nLen;
}



static void TextOutFields (
   HDC    hDC,
   int    x,
   LPRECT lprc,
   LPSTR  lpszItems)
   {
   int eto = ETO_CLIPPED | ETO_OPAQUE;
   int ii = 0;
   PSTR psz = lpszItems;
   int nLen;

   // copy fields until we get one of zero size.
   //
   do {
      SetTextAlign (hDC, taColumns[ii]);

      ExtTextOut(hDC,
                 lprc->left + (xColumns[ii] * sChar0.cx) + x,
                 lprc->top,
                 eto,
                 lprc,
                 psz,
                 nLen = strlen(psz),
                 0L);

      psz += (nLen + 1);
      eto = ETO_CLIPPED;
      ++ii;
      } while (nLen);
   }



void WINAPI DrawListItem(
    DRAWITEMSTRUCT *lpItem)
{
    DWORD dwBkColor=0xffffffff, dwTextColor=0xffffffff;
    char  szListItem[200];
    int   nLen;

    /* Make sure it is the list box with a valid item ID */
    if (lpItem->CtlType != ODT_LISTBOX ||
	lpItem->CtlID != IDC_LISTBOX)
        return;

    if (lpItem->itemAction & (ODA_DRAWENTIRE | ODA_SELECT))
	{
        /* Alter the bk and text color for selected items */
	if (lpItem->itemState & ODS_SELECTED)
	    {
	    dwBkColor = SetBkColor (lpItem->hDC, GetSysColor(COLOR_HIGHLIGHT));
	    dwTextColor = SetTextColor (lpItem->hDC, GetSysColor(COLOR_HIGHLIGHTTEXT));
	    }

	/* change TextColor for new entries too */
	else if ((((LPVMOBJECT)lpWalkerList)+Objects[lpItem->itemData])->bNew)
	    {
	    dwBkColor = SetBkColor (lpItem->hDC, GetSysColor(COLOR_HIGHLIGHT));
	    dwTextColor = SetTextColor (lpItem->hDC, GetSysColor(COLOR_HIGHLIGHTTEXT));
	    }

	/* make listbox string from virtual memory object */
	nLen = MakeVMQString (lpItem->itemData, szListItem);
        TextOutFields (lpItem->hDC, 6, &lpItem->rcItem, szListItem);

	/* Restore previous bk and text color if necessary */
	if (dwBkColor != 0xffffffff)
            SetBkColor(lpItem->hDC, dwBkColor);
	if (dwTextColor != 0xffffffff)
	    SetTextColor(lpItem->hDC, dwTextColor);

        if (lpItem->itemState & ODS_FOCUS)
	    lpItem->itemAction |= ODA_FOCUS;

	}

    if (lpItem->itemAction & ODA_FOCUS)
        DrawFocusRect(lpItem->hDC, &lpItem->rcItem);
}



/* perform bubble sort on indexes to virtual memory objects as stored in
   ownerdraw listbox do not actually sort the objects, just the indexes  */
void  WINAPI SortList (
    HWND    hList,
    int     nSort)
{
    int 	  nItems = SendMessage (hList, LB_GETCOUNT, 0, 0);
    int 	  i, j, t;
    LPVMOBJECT	  lpVMO = (LPVMOBJECT)lpWalkerList;

    /* loop through all items in list box */
    for (i=0; i<nItems-1; i++)
	for (j=i+1; j<nItems; j++)
	    {
	    /* compare on sort order */
	    switch (nSort)
		{
		case IDM_SORTADDRESS:
		    if (lpVMO[Objects[i]].mbi.BaseAddress > lpVMO[Objects[j]].mbi.BaseAddress)
			{
			/* swap */
			t = Objects[j];
			Objects[j] = Objects[i];
			Objects[i] = t;
			}
		    break;

		case IDM_SORTSTATE:
		    if ((lpVMO[Objects[i]].mbi.State > lpVMO[Objects[j]].mbi.State) ||
			(lpVMO[Objects[i]].mbi.State == lpVMO[Objects[j]].mbi.State &&
			 lpVMO[Objects[i]].mbi.BaseAddress > lpVMO[Objects[j]].mbi.BaseAddress))
			{
			/* swap */
			t = Objects[j];
			Objects[j] = Objects[i];
			Objects[i] = t;
			}
		    break;

		case IDM_SORTPROTECTION:
		    if ((lpVMO[Objects[i]].mbi.Protect > lpVMO[Objects[j]].mbi.Protect) ||
			(lpVMO[Objects[i]].mbi.Protect == lpVMO[Objects[j]].mbi.Protect &&
			 lpVMO[Objects[i]].mbi.BaseAddress > lpVMO[Objects[j]].mbi.BaseAddress))
			{
			/* swap */
			t = Objects[j];
			Objects[j] = Objects[i];
			Objects[i] = t;
			}
		    break;

		case IDM_SORTSIZE:
		    if ((lpVMO[Objects[i]].mbi.RegionSize > lpVMO[Objects[j]].mbi.RegionSize) ||
			(lpVMO[Objects[i]].mbi.RegionSize == lpVMO[Objects[j]].mbi.RegionSize &&
			 lpVMO[Objects[i]].mbi.BaseAddress > lpVMO[Objects[j]].mbi.BaseAddress))
			{
			/* swap */
			t = Objects[j];
			Objects[j] = Objects[i];
			Objects[i] = t;
			}
		    break;

		case IDM_SORTBASEADDRESS:
		    if ((lpVMO[Objects[i]].mbi.AllocationBase > lpVMO[Objects[j]].mbi.AllocationBase) ||
			(lpVMO[Objects[i]].mbi.AllocationBase == lpVMO[Objects[j]].mbi.AllocationBase &&
			 lpVMO[Objects[i]].mbi.BaseAddress > lpVMO[Objects[j]].mbi.BaseAddress))
			{
			/* swap */
			t = Objects[j];
			Objects[j] = Objects[i];
			Objects[i] = t;
			}
		    break;
		}
	    }
}




/* get free disk space on all fixed drives */
BOOL   WINAPI GetFreeDiskSpace (
    LPDWORD    lpdwTotalSpace,
    LPDWORD    lpdwFreeSpace)
{
    DWORD    dwBytesPerSector, dwSectorsPerCluster, dwFreeClusters, dwTotalClusters;
    DWORD    dwDriveMask = GetLogicalDrives();
    int      i;
    char     szDir[4];

    *lpdwTotalSpace = 0;
    *lpdwFreeSpace = 0;
    szDir[1] = TEXT(':');
    szDir[2] = TEXT('\\');
    szDir[3] = 0;

    /* enumerate all logical, fixed drives */
    for (i = 0; i < MAX_DRIVES; dwDriveMask >>= 1, i++)
	{
	/* if logical drive exists */
	if (dwDriveMask & 0x01)
	    {
	    szDir[0] = TEXT('A') + i;

	    /* if it is a fixed drive */
	    if (GetDriveType(szDir) == DRIVE_FIXED)
		{
		/* determine free space and total capacity */
		GetDiskFreeSpace (szDir,
				  &dwSectorsPerCluster,
				  &dwBytesPerSector,
				  &dwFreeClusters,
				  &dwTotalClusters);

		*lpdwTotalSpace += dwTotalClusters * dwSectorsPerCluster * dwBytesPerSector;
		*lpdwFreeSpace += dwFreeClusters * dwSectorsPerCluster * dwBytesPerSector;
		}
	    }
	}

    return (*lpdwTotalSpace || *lpdwFreeSpace);
}



/* generic message notification */
int WINAPI NotifyUser (
    HWND    hWndParent,
    int     nTitle,
    int     nError,
    char    *lpszAppend,
    UINT    uFlags)
{
    char    szError[MAX_PATH];
    char    szTitle[MAX_PATH];

    LoadString (GetModuleHandle (NULL), nTitle, szTitle, MAX_PATH);
    LoadString (GetModuleHandle (NULL), nError, szError, MAX_PATH);

    if (lpszAppend != NULL && *lpszAppend != 0)
	strcat (szError, lpszAppend);

    if (!uFlags)
	uFlags = MB_ICONSTOP | MB_OK | MB_TASKMODAL | MB_SETFOREGROUND;

    /* return message box response */
    return (MessageBox (hWndParent, szError, szTitle, uFlags));
}




void   WINAPI ReportError (
    int     nIDS_CAPTION)
{
    char    *lpszError;
    char    szText[MAX_PATH];

    /* get formatted error message from system */
    if (!FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			GetLastError (),
			MAKELONG (MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), 0),
			(LPTSTR)&lpszError,
			0,
			NULL))
	return;

    /* if resource string provided, load caption string */
    if (nIDS_CAPTION)
	LoadString (GetModuleHandle (NULL), nIDS_CAPTION, szText, MAX_PATH);
    else
	strcpy (szText, "Error");

    MessageBox (NULL,
		lpszError,
		szText,
		MB_ICONSTOP | MB_OK | MB_TASKMODAL | MB_SETFOREGROUND);
}





BOOL WINAPI ViewableMemorySelection (
    HWND    hWnd)
{
    HWND   hList = GetDlgItem (hWnd, IDC_LISTBOX);
    int    iCaret = SendMessage (hList, LB_GETCARETINDEX, 0, 0);
    int    iAnchor = SendMessage (hList, LB_GETANCHORINDEX, 0, 0);

    if (iCaret > -1 &&
	iAnchor > -1 &&
	SendMessage (hList, LB_GETSEL, iCaret, 0) &&
	CommittedMemoryRange (iCaret,
			      iAnchor,
			      (LPVMOBJECT)lpWalkerList,
			      Objects))
	return TRUE;
    else
	return FALSE;
}




BOOL WINAPI InitDlgProc (
    HWND      hDlg,
    UINT      uMsg,
    WPARAM    wParam,
    LPARAM    lParam)
{
    switch (uMsg)
	{
	case WM_INITDIALOG:
	    *(HANDLE *)lParam = hDlg;
	    break;

	case WM_CLOSE:
	case UM_ENDDIALOG:
	    EndDialog (hDlg, TRUE);
	    break;

	case WM_COMMAND:
	    if (LOWORD (wParam) == IDCANCEL ||
		LOWORD (wParam) == IDOK)
		EndDialog (hDlg, TRUE);
	    break;

	default:
	    return FALSE;
	}

    return TRUE;
}
