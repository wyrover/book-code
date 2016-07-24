
#include "pwalk.h"

#define ONE_K	      1024
#define ONE_MEG       1048576

extern HFONT	     hFont;
extern LPVOID	     lpWalkerList;
extern DBGPROCESS    *lpChildProcess;
extern int	     yChar;
extern SIZE	     sChar0;

/* static variables that determine size of 3D status box */
int	xSTATBOX;
int	cxSTATBOX;
int	dxSTATBOX;
int	ySTATBOX;
int	dyuSTATBOX;
int	dylSTATBOX;



void WINAPI TallyProcessStats (HWND, int *, int *, int *);
void WINAPI DrawBorders (HWND, LPPAINTSTRUCT);
void WINAPI DrawStatusText (HWND, LPPAINTSTRUCT, char *, COLORREF);


/* status window proc */
LONG WINAPI StatusWndProc (
    HWND    hWnd,
    UINT    uMsg,
    WPARAM  wParam,
    LPARAM  lParam)
{
	  LONG	      lRet = 1;
static	  BOOL	      bDefault;
static	  char	      szDefault[MAX_PATH];
static	  COLORREF    crText;

    switch (uMsg)
	{
	case WM_CREATE:
	    {
	    RECT    rc;

	    SetWindowLong (hWnd,
			   WXB_HPENHILITE,
			   (LONG)CreatePen (PS_SOLID, 0, GetSysColor (COLOR_BTNHIGHLIGHT)));
	    SetWindowLong (hWnd,
			   WXB_HPENSHADOW,
			   (LONG)CreatePen (PS_SOLID, 0, GetSysColor (COLOR_BTNSHADOW)));

	    /* initialize default status text */
	    LoadString (GetModuleHandle (NULL),
			IDS_STATUSREADY,
			szDefault,
			sizeof (szDefault));
	    crText = GetSysColor (COLOR_WINDOWTEXT);

	    /* add rewalk button */
	    GetClientRect (GetParent (hWnd), &rc);
	    CreateWindow ("button",
			  "Rewalk",
			  WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			  rc.right-74, -1, 75, sChar0.cy+10*GetSystemMetrics (SM_CYBORDER),
			  hWnd,
			  (HMENU)IDC_REWALK,
			  GetModuleHandle (NULL),
			  NULL);

	    bDefault = TRUE;
	    }
	    break;

	case WM_SIZE:
	    {
	    RECT    rc;

	    GetClientRect (GetParent (hWnd), &rc);

	    xSTATBOX = 10 * GetSystemMetrics (SM_CXBORDER);
	    cxSTATBOX = rc.right/3 + 50;
	    dxSTATBOX = 4 * GetSystemMetrics (SM_CXBORDER);
	    ySTATBOX = 2 * GetSystemMetrics (SM_CYBORDER);
	    dyuSTATBOX = 1 * GetSystemMetrics (SM_CYBORDER);
	    dylSTATBOX = 2 * GetSystemMetrics (SM_CYBORDER);

	    /* move child windows in view */
	    MoveWindow (GetDlgItem (hWnd, IDC_REWALK),
			rc.right-74,
			-1,
			75,
			yChar+10*GetSystemMetrics (SM_CYBORDER),
			TRUE);

	    /* make sure we repaint correctly */
	    InvalidateRect (hWnd, NULL, TRUE);
	    }
	    break;

	case WM_TIMER:
	    {
	    RECT    rcStatus;

	    KillTimer (hWnd, IDT_STATUS);
	    bDefault = TRUE;
	    /* invalidate status text area */
	    SetRect (&rcStatus,
		     xSTATBOX+1,
		     ySTATBOX+1,
		     xSTATBOX+cxSTATBOX,
		     ySTATBOX+dylSTATBOX+dyuSTATBOX+yChar);
	    InvalidateRect (hWnd, &rcStatus, TRUE);
	    crText = GetSysColor (COLOR_WINDOWTEXT);
	    }
	    break;

	case WM_SETTEXT:
	    {
	    RECT    rcStatus;

	    /* release current string */
	    if (GetWindowLong (hWnd, WXB_LPWINDOWTEXT))
		LocalFree ((HLOCAL)GetWindowLong (hWnd, WXB_LPWINDOWTEXT));
	    SetWindowLong (hWnd, WXB_LPWINDOWTEXT, 0);

	    /* if null string, reset to default */
	    if ((char *)lParam == NULL)
		{
		bDefault = TRUE;
		crText = GetSysColor (COLOR_WINDOWTEXT);
		}
	    else
		{
		crText = (COLORREF)wParam;

		/* allocate space for new string */
		SetWindowLong (hWnd,
			       WXB_LPWINDOWTEXT,
			       (LONG)LocalAlloc (LPTR, strlen ((LPSTR)lParam)+1));
		strcpy ((char *)GetWindowLong (hWnd, WXB_LPWINDOWTEXT), (char *)lParam);

		/* restart five second timer */
		KillTimer (hWnd, IDT_STATUS);
		SetTimer (hWnd, IDT_STATUS, 5000, NULL);

		bDefault = FALSE;
		}

	    /* invalidate status text area */
	    SetRect (&rcStatus,
		     xSTATBOX+1,
		     ySTATBOX+1,
		     xSTATBOX+cxSTATBOX,
		     ySTATBOX+dylSTATBOX+dyuSTATBOX+yChar);
	    InvalidateRect (hWnd, &rcStatus, TRUE);
	    }
	    break;

	case WM_PAINT:
	    {
	    PAINTSTRUCT    ps;
	    RECT	   rc;

	    BeginPaint (hWnd, &ps);
	    GetClientRect (hWnd, &rc);

	    /* draw 3D effects in window */
	    DrawBorders (hWnd, &ps);

	    /* update text */
	    if (bDefault)
		DrawStatusText (hWnd, &ps, szDefault, crText);
	    else
		DrawStatusText (hWnd,
				&ps,
				(char *)GetWindowLong (hWnd, WXB_LPWINDOWTEXT),
				crText);

	    EndPaint (hWnd, &ps);
	    }
	    break;

	/* user defined message for updating child process information */
	case UM_UPDATE:
	    EnableWindow (GetDlgItem (hWnd, IDC_REWALK), ((LPVOID)wParam != NULL));
	    break;

	case WM_COMMAND:
	    if (LOWORD (wParam) == IDC_REWALK)
		PostMessage (GetParent (hWnd), WM_COMMAND, IDM_PROCESSREWALK, 0);
	    break;

	case WM_DESTROY:
	    /* free pen objects */
	    DeleteObject ((HPEN)GetWindowLong (hWnd, WXB_HPENHILITE));
	    DeleteObject ((HPEN)GetWindowLong (hWnd, WXB_HPENSHADOW));

	    /* free local memory from window text */
	    if (GetWindowLong (hWnd, WXB_LPWINDOWTEXT))
		LocalFree ((HLOCAL)GetWindowLong (hWnd, WXB_LPWINDOWTEXT));
	    break;

	default:
	    lRet = DefWindowProc (hWnd, uMsg, wParam, lParam);
	    break;
	}

    return lRet;
}




/* system statistics window proc */
LONG WINAPI SysStatWndProc (
    HWND    hWnd,
    UINT    uMsg,
    WPARAM  wParam,
    LPARAM  lParam)
{
    LONG    lRet = 1;

static	  MEMORYSTATUS	  msInfo, msInfoOld;
static	  SYSTEM_INFO	  sysInfo, sysInfoOld;
static	  DWORD 	  dwFreeDiskOld, dwFreeDisk;
static	  DWORD 	  dwTotalDiskOld, dwTotalDisk;

    switch (uMsg)
	{
	case WM_CREATE:
	    /* get system info */
	    GlobalMemoryStatus (&msInfo);
	    GetSystemInfo (&sysInfo);
	    GetFreeDiskSpace (&dwTotalDisk, &dwFreeDisk);

	    /* update old data with current data */
	    msInfoOld = msInfo;
	    sysInfoOld = sysInfo;
	    dwTotalDiskOld = dwTotalDisk;
	    dwFreeDiskOld = dwFreeDisk;

	    /* create buttons controls */
	    CreateWindow ("button",
			  "Refresh",
			  WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			  100, 210, 75, 25,
			  hWnd,
			  (HMENU)IDC_REFRESH,
			  GetModuleHandle (NULL),
			  NULL);

	    CreateWindow ("button",
			  "Cancel",
			  WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			  205, 210, 75, 25,
			  hWnd,
			  (HMENU)IDC_CANCEL,
			  GetModuleHandle (NULL),
			  NULL);
	    break;

	case WM_CHAR:
	    switch (wParam)
		{
		case VK_RETURN:
		    GlobalMemoryStatus (&msInfo);
		    GetSystemInfo (&sysInfo);
		    GetFreeDiskSpace (&dwTotalDisk, &dwFreeDisk);
		    InvalidateRect (hWnd, NULL, TRUE);
		    UpdateWindow (hWnd);
		    break;

		case VK_ESCAPE:
		    DestroyWindow (hWnd);
		    break;
		}
	    break;

	case WM_COMMAND:
	    switch (LOWORD (wParam))
		{
		case IDC_REFRESH:
		    /* save previous values */
		    msInfoOld = msInfo;
		    sysInfoOld = sysInfo;
		    dwTotalDiskOld = dwTotalDisk;
		    dwFreeDiskOld = dwFreeDisk;

		    /* get current values */
		    GlobalMemoryStatus (&msInfo);
		    GetSystemInfo (&sysInfo);
		    GetFreeDiskSpace (&dwTotalDisk, &dwFreeDisk);
		    InvalidateRect (hWnd, NULL, TRUE);
		    UpdateWindow (hWnd);
		    break;

		case IDC_CANCEL:
		    DestroyWindow (hWnd);
		    break;
		}
	    break;


	case WM_PAINT:
	    {
	    PAINTSTRUCT    ps;
	    char	   szText[100];
	    HFONT	   hOldFont;
	    int 	   nLen;
	    SIZE	   size;
	    POINT	   pt;
	    COLORREF	   crText;

	    BeginPaint (hWnd, &ps);

	    /* set background mode to transparent */
	    SetBkMode (ps.hdc, TRANSPARENT);
	    hOldFont = SelectObject (ps.hdc, hFont);

	    /* draw vertical separating line */
	    MoveToEx (ps.hdc, 395, 5, &pt);
	    LineTo (ps.hdc, 395, 200);

	    /* refresh change header */
	    wsprintf (szText, "%13s", "current value");
	    SetRect (&(ps.rcPaint), 0, 5, 395, 20);
	    ExtTextOut (ps.hdc, 275, 5, ETO_CLIPPED, &(ps.rcPaint), szText, strlen (szText), 0);

	    /* refresh change header */
	    wsprintf (szText, "%10s", "changed by");
	    SetRect (&(ps.rcPaint), 395, 5, 500, 20);
	    ExtTextOut (ps.hdc, 400, 5, ETO_CLIPPED, &(ps.rcPaint), szText, strlen (szText), 0);

	    /* memory load statistic */
	    nLen = wsprintf (szText, "%30s     %10lu", "Relative memory load [0-100]: ", msInfo.dwMemoryLoad);
	    GetTextExtentPoint (ps.hdc, szText, nLen, &size);
	    SetRect (&(ps.rcPaint), 0, 20, 395, 35);
	    ExtTextOut (ps.hdc, 10, 20, ETO_CLIPPED, &(ps.rcPaint), szText, strlen (szText), 0);
	    if (msInfo.dwMemoryLoad > msInfoOld.dwMemoryLoad)
		{
		crText = SetTextColor (ps.hdc, RGB (0, 100, 0));
		wsprintf (szText, "+%-1lu", (msInfo.dwMemoryLoad-msInfoOld.dwMemoryLoad));
		SetRect (&(ps.rcPaint), 395, 20, 500, 35);
		ExtTextOut (ps.hdc, 400, 20, ETO_CLIPPED, &(ps.rcPaint), szText, strlen (szText), 0);
		SetTextColor (ps.hdc, crText);
		}
	    else if (msInfo.dwMemoryLoad < msInfoOld.dwMemoryLoad)
		{
		crText = SetTextColor (ps.hdc, RGB (100, 0, 0));
		wsprintf (szText, "-%-1lu", (msInfoOld.dwMemoryLoad-msInfo.dwMemoryLoad));
		SetRect (&(ps.rcPaint), 395, 20, 500, 65);
		ExtTextOut (ps.hdc, 400, 20, ETO_CLIPPED, &(ps.rcPaint), szText, strlen (szText), 0);
		SetTextColor (ps.hdc, crText);
		}

	    /* pagesize */
	    nLen = wsprintf (szText, "%22s             %10lub", "Pagesize granularity: ", sysInfo.dwPageSize);
	    GetTextExtentPoint (ps.hdc, szText, nLen, &size);
	    SetRect (&(ps.rcPaint), 0, 35, 395, 50);
	    ExtTextOut (ps.hdc, 10, 35, ETO_CLIPPED, &(ps.rcPaint), szText, strlen (szText), 0);

	    /* total physical memory */
	    nLen = wsprintf (szText, "%23s            %10luKb", "Total physical memory: ", msInfo.dwTotalPhys/ONE_K);
	    GetTextExtentPoint (ps.hdc, szText, nLen, &size);
	    SetRect (&(ps.rcPaint), 0, 50, 395, 65);
	    ExtTextOut (ps.hdc, 10, 50, ETO_CLIPPED, &(ps.rcPaint), szText, strlen (szText), 0);

	    /* available physical memory */
	    nLen = wsprintf (szText, "%27s        %10luKb", "Available physical memory: ", msInfo.dwAvailPhys/ONE_K);
	    GetTextExtentPoint (ps.hdc, szText, nLen, &size);
	    SetRect (&(ps.rcPaint), 0, 65, 395, 80);
	    ExtTextOut (ps.hdc, 10, 65, ETO_CLIPPED, &(ps.rcPaint), szText, strlen (szText), 0);
	    if (msInfo.dwAvailPhys > msInfoOld.dwAvailPhys)
		{
		crText = SetTextColor (ps.hdc, RGB (0, 100, 0));
		wsprintf (szText, "+%-1luKb", (msInfo.dwAvailPhys-msInfoOld.dwAvailPhys)/ONE_K);
		SetRect (&(ps.rcPaint), 395, 65, 500, 80);
		ExtTextOut (ps.hdc, 400, 65, ETO_CLIPPED, &(ps.rcPaint), szText, strlen (szText), 0);
		SetTextColor (ps.hdc, crText);
		}
	    else if (msInfo.dwAvailPhys < msInfoOld.dwAvailPhys)
		{
		crText = SetTextColor (ps.hdc, RGB (100, 0, 0));
		wsprintf (szText, "-%-1luKb", (msInfoOld.dwAvailPhys-msInfo.dwAvailPhys)/ONE_K);
		SetRect (&(ps.rcPaint), 395, 65, 500, 80);
		ExtTextOut (ps.hdc, 400, 65, ETO_CLIPPED, &(ps.rcPaint), szText, strlen (szText), 0);
		SetTextColor (ps.hdc, crText);
		}

	    /* total pagefile space */
	    nLen = wsprintf (szText, "%24s           %10luMb", "Maximum pagefile space: ", msInfo.dwTotalPageFile/ONE_MEG);
	    GetTextExtentPoint (ps.hdc, szText, nLen, &size);
	    SetRect (&(ps.rcPaint), 0, 80, 395, 95);
	    ExtTextOut (ps.hdc, 10, 80, ETO_CLIPPED, &(ps.rcPaint), szText, strlen (szText), 0);
	    if (msInfo.dwTotalPageFile > msInfoOld.dwTotalPageFile)
		{
		crText = SetTextColor (ps.hdc, RGB (0, 100, 0));
		wsprintf (szText, "+%-1luMb", (msInfo.dwTotalPageFile-msInfoOld.dwTotalPageFile)/ONE_MEG);
		SetRect (&(ps.rcPaint), 395, 80, 500, 95);
		ExtTextOut (ps.hdc, 400, 80, ETO_CLIPPED, &(ps.rcPaint), szText, strlen (szText), 0);
		SetTextColor (ps.hdc, crText);
		}
	    else if (msInfo.dwTotalPageFile < msInfoOld.dwTotalPageFile)
		{
		crText = SetTextColor (ps.hdc, RGB (100, 0, 0));
		wsprintf (szText, "-%-1luMb", (msInfoOld.dwTotalPageFile-msInfo.dwTotalPageFile)/ONE_MEG);
		SetRect (&(ps.rcPaint), 395, 80, 500, 95);
		ExtTextOut (ps.hdc, 400, 65, ETO_CLIPPED, &(ps.rcPaint), szText, strlen (szText), 0);
		SetTextColor (ps.hdc, crText);
		}

	    /* available pagefile space */
	    nLen = wsprintf (szText, "%26s         %10luMb", "Available pagefile space: ", (msInfo.dwAvailPageFile)/ONE_MEG);
	    GetTextExtentPoint (ps.hdc, szText, nLen, &size);
	    SetRect (&(ps.rcPaint), 0, 95, 395, 110);
	    ExtTextOut (ps.hdc, 10, 95, ETO_CLIPPED, &(ps.rcPaint), szText, strlen (szText), 0);
	    if (msInfo.dwAvailPageFile > msInfoOld.dwAvailPageFile)
		{
		crText = SetTextColor (ps.hdc, RGB (0, 100, 0));
		wsprintf (szText, "+%-1luKb", (msInfo.dwAvailPageFile-msInfoOld.dwAvailPageFile)/ONE_K);
		SetRect (&(ps.rcPaint), 395, 95, 500, 110);
		ExtTextOut (ps.hdc, 400, 95, ETO_CLIPPED, &(ps.rcPaint), szText, strlen (szText), 0);
		SetTextColor (ps.hdc, crText);
		}
	    else if (msInfo.dwAvailPageFile < msInfoOld.dwAvailPageFile)
		{
		crText = SetTextColor (ps.hdc, RGB (100, 0, 0));
		wsprintf (szText, "-%-1luKb", (msInfoOld.dwAvailPageFile-msInfo.dwAvailPageFile)/ONE_K);
		SetRect (&(ps.rcPaint), 395, 95, 500, 110);
		ExtTextOut (ps.hdc, 400, 95, ETO_CLIPPED, &(ps.rcPaint), szText, strlen (szText), 0);
		SetTextColor (ps.hdc, crText);
		}

	    /* total virtual address space */
	    nLen = wsprintf (szText, "%29s      %10luMb", "Total virtual address space: ", msInfo.dwTotalVirtual/ONE_MEG);
	    GetTextExtentPoint (ps.hdc, szText, nLen, &size);
	    SetRect (&(ps.rcPaint), 0, 110, 395, 125);
	    ExtTextOut (ps.hdc, 10, 110, ETO_CLIPPED, &(ps.rcPaint), szText, strlen (szText), 0);

	    /* available virtual address space */
	    nLen = wsprintf (szText, "%33s  %10luMb", "Available virtual address space: ", msInfo.dwAvailVirtual/ONE_MEG);
	    GetTextExtentPoint (ps.hdc, szText, nLen, &size);
	    SetRect (&(ps.rcPaint), 0, 125, 395, 140);
	    ExtTextOut (ps.hdc, 10, 125, ETO_CLIPPED, &(ps.rcPaint), szText, strlen (szText), 0);

	    /* lowest available virtual address */
	    nLen = wsprintf (szText, "%25s            %#08lx", "Minimum virtual address: ", sysInfo.lpMinimumApplicationAddress);
	    GetTextExtentPoint (ps.hdc, szText, nLen, &size);
	    SetRect (&(ps.rcPaint), 0, 140, 395, 155);
	    ExtTextOut (ps.hdc, 10, 140, ETO_CLIPPED, &(ps.rcPaint), szText, strlen (szText), 0);

	    /* highest available virtual address */
	    nLen = wsprintf (szText, "%25s            %#08lx", "Maximum virtual address: ", sysInfo.lpMaximumApplicationAddress);
	    GetTextExtentPoint (ps.hdc, szText, nLen, &size);
	    SetRect (&(ps.rcPaint), 0, 155, 395, 170);
	    ExtTextOut (ps.hdc, 10, 155, ETO_CLIPPED, &(ps.rcPaint), szText, strlen (szText), 0);

	    /* total disk space */
	    nLen = wsprintf (szText, "%24s           %10luMb", "Total local disk space: ", dwTotalDisk/ONE_MEG);
	    GetTextExtentPoint (ps.hdc, szText, nLen, &size);
	    SetRect (&(ps.rcPaint), 0, 170, 395, 185);
	    ExtTextOut (ps.hdc, 10, 170, ETO_CLIPPED, &(ps.rcPaint), szText, strlen (szText), 0);

	    /* free disk disk space */
	    nLen = wsprintf (szText, "%23s            %10luMb", "Free local disk space: ", dwFreeDisk/ONE_MEG);
	    GetTextExtentPoint (ps.hdc, szText, nLen, &size);
	    SetRect (&(ps.rcPaint), 0, 185, 395, 200);
	    ExtTextOut (ps.hdc, 10, 185, ETO_CLIPPED, &(ps.rcPaint), szText, strlen (szText), 0);
	    if (dwFreeDisk > dwFreeDiskOld)
		{
		crText = SetTextColor (ps.hdc, RGB (0, 100, 0));
		wsprintf (szText, "+%-1luMb", (dwFreeDisk-dwFreeDiskOld)/ONE_MEG);
		SetRect (&(ps.rcPaint), 395, 185, 500, 200);
		ExtTextOut (ps.hdc, 400, 185, ETO_CLIPPED, &(ps.rcPaint), szText, strlen (szText), 0);
		SetTextColor (ps.hdc, crText);
		}
	    else if (dwFreeDisk < dwFreeDiskOld)
		{
		crText = SetTextColor (ps.hdc, RGB (100, 0, 0));
		wsprintf (szText, "-%-1luMb", (dwFreeDiskOld-dwFreeDisk)/ONE_MEG);
		SetRect (&(ps.rcPaint), 395, 185, 500, 200);
		ExtTextOut (ps.hdc, 400, 185, ETO_CLIPPED, &(ps.rcPaint), szText, strlen (szText), 0);
		SetTextColor (ps.hdc, crText);
		}

	    /* replace old font */
	    SelectObject (ps.hdc, hOldFont);
	    EndPaint (hWnd, &ps);
	    }
	    break;

	default:
	    lRet = DefWindowProc (hWnd, uMsg, wParam, lParam);
	}

    return lRet;
}




/* process statistics window proc */
LONG WINAPI ProStatWndProc (
    HWND    hWnd,
    UINT    uMsg,
    WPARAM  wParam,
    LPARAM  lParam)
{
	  LONG	  lRet = 1;
static	  int	 nTotalFree, nTotalCommit, nTotalReserve;

    switch (uMsg)
	{
	case WM_CREATE:
	    {
	    /* create buttons controls */
	    CreateWindow ("button",
			  "Refresh",
			  WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			  45, 55, 75, 25,
			  hWnd,
			  (HMENU)IDC_REFRESH,
			  GetModuleHandle (NULL),
			  NULL);

	    CreateWindow ("button",
			  "Rewalk",
			  WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			  140, 55, 75, 25,
			  hWnd,
			  (HMENU)IDM_PROCESSREWALK,
			  GetModuleHandle (NULL),
			  NULL);

	    CreateWindow ("button",
			  "Cancel",
			  WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			  235, 55, 75, 25,
			  hWnd,
			  (HMENU)IDC_CANCEL,
			  GetModuleHandle (NULL),
			  NULL);

	    /* tally process statictics */
	    TallyProcessStats (hWnd, &nTotalFree, &nTotalReserve, &nTotalCommit);
	    }
	    break;

	case WM_CHAR:
	    switch (wParam)
		{
		case VK_RETURN:
		    InvalidateRect (hWnd, NULL, TRUE);
		    UpdateWindow (hWnd);
		    break;

		case VK_ESCAPE:
		    DestroyWindow (hWnd);
		    break;
		}
	    break;

	case WM_PAINT:
	    {
	    PAINTSTRUCT    ps;
	    HFONT	   hOldFont;
	    char	   szText[100];

	    BeginPaint (hWnd, &ps);
	    hOldFont = SelectObject (ps.hdc, hFont);
	    SetBkMode (ps.hdc, TRANSPARENT);

	    /* total free memory */
	    wsprintf (szText,
		      "%19s          %10luKb",
		      "Total free memory: ",
		      nTotalFree/ONE_K);
	    SetRect (&(ps.rcPaint), ps.rcPaint.left, 0, ps.rcPaint.right, 20);
	    ExtTextOut (ps.hdc,
			10,
			5,
			ETO_CLIPPED,
			&(ps.rcPaint),
			szText,
			strlen (szText),
			0);

	    /* total committed memory */
	    wsprintf (szText,
		      "%24s     %10luKb",
		      "Total committed memory: ",
		      nTotalCommit/ONE_K);
	    SetRect (&(ps.rcPaint), ps.rcPaint.left, 20, ps.rcPaint.right, 35);
	    ExtTextOut (ps.hdc,
			10,
			20,
			ETO_CLIPPED,
			&(ps.rcPaint),
			szText,
			strlen (szText),
			0);

	    /* total reserved memory */
	    wsprintf (szText,
		      "%22s       %10luKb",
		      "Total reserved memory:",
		      nTotalReserve/ONE_K);
	    SetRect (&(ps.rcPaint), ps.rcPaint.left, 35, ps.rcPaint.right, 50);
	    ExtTextOut (ps.hdc,
			10,
			35,
			ETO_CLIPPED,
			&(ps.rcPaint),
			szText,
			strlen (szText),
			0);

	    SelectObject (ps.hdc, hOldFont);
	    EndPaint (hWnd, &ps);
	    }
	    break;

	case WM_COMMAND:
	    switch (LOWORD (wParam))
		{
		case IDM_PROCESSREWALK:
		    {
		    /* send message to parent to rewalk process */
		    SendMessage (GetParent (hWnd), uMsg, wParam, lParam);

		    /* tally new process statistics */
		    TallyProcessStats (hWnd, &nTotalFree, &nTotalReserve, &nTotalCommit);

		    InvalidateRect (hWnd, NULL, TRUE);
		    UpdateWindow (hWnd);
		    }
		    break;

		case IDC_REFRESH:
		    InvalidateRect (hWnd, NULL, TRUE);
		    UpdateWindow (hWnd);
		    break;

		case IDC_CANCEL:
		    DestroyWindow (hWnd);
		    break;
		}
	    break;

	default:
	    lRet = DefWindowProc (hWnd, uMsg, wParam, lParam);
	}

    return lRet;
}




/* determine process statistics from existing information */
void WINAPI TallyProcessStats (
    HWND    hWnd,
    int     *nTotalFree,
    int     *nTotalReserve,
    int     *nTotalCommit)
{
    LPVMOBJECT	  lpVMO = (LPVMOBJECT)lpWalkerList;
    int 	  nRegions, i;

    *nTotalFree = 0;
    *nTotalCommit = 0;
    *nTotalReserve = 0;

    /* determine number of memory regions by number of entries in listbox */
    nRegions = (int)SendMessage (GetWindow (GetParent (hWnd), GW_CHILD),
				 LB_GETCOUNT,
				 0,
				 0);

    /* get stats for all memory regions in process */
    for (i=0; i<nRegions; i++)
	{
	if (lpVMO[i].mbi.State & MEM_FREE)
	    *nTotalFree += lpVMO[i].mbi.RegionSize;
	else if (lpVMO[i].mbi.State & MEM_COMMIT)
	    *nTotalCommit += lpVMO[i].mbi.RegionSize;
	else
	    *nTotalReserve += lpVMO[i].mbi.RegionSize;
	}
}




void WINAPI DrawBorders (
    HWND	     hWnd,
    LPPAINTSTRUCT    lpps)
{
    POINT	   pt;
    HPEN	   hOldPen;



    /* draw window hilites for 3D affect */
    hOldPen = SelectObject (lpps->hdc, (HPEN)GetWindowLong (hWnd, WXB_HPENHILITE));
    /* top line across window */
    MoveToEx (lpps->hdc, lpps->rcPaint.left, lpps->rcPaint.top, &pt);
    LineTo (lpps->hdc, lpps->rcPaint.right, lpps->rcPaint.top);
    /* bottom line in status box */
    MoveToEx (lpps->hdc,
	      xSTATBOX,
	      ySTATBOX+yChar+dyuSTATBOX+dylSTATBOX,
	      &pt);
    LineTo (lpps->hdc,
	    xSTATBOX+cxSTATBOX,
	    ySTATBOX+yChar+dylSTATBOX+dyuSTATBOX);
    /* right line in status box */
    MoveToEx (lpps->hdc, xSTATBOX+cxSTATBOX, ySTATBOX, &pt);
    LineTo (lpps->hdc,
	    xSTATBOX+cxSTATBOX,
	    ySTATBOX+yChar+dylSTATBOX+dyuSTATBOX+1);

    /* draw window shadows for 3D affect */
    hOldPen = SelectObject (lpps->hdc, (HPEN)GetWindowLong (hWnd, WXB_HPENSHADOW));
    /* top line in status box */
    MoveToEx (lpps->hdc, xSTATBOX, ySTATBOX, &pt);
    LineTo (lpps->hdc, xSTATBOX+cxSTATBOX+1, ySTATBOX);
    /* left line in status box */
    MoveToEx (lpps->hdc, xSTATBOX, ySTATBOX, &pt);
    LineTo (lpps->hdc, xSTATBOX, ySTATBOX+yChar+dylSTATBOX+dyuSTATBOX+1);

    SelectObject (lpps->hdc, hOldPen);
}




void WINAPI DrawStatusText (
    HWND	     hWnd,
    LPPAINTSTRUCT    lpps,
    char	     *lpszText,
    COLORREF	     crText)
{
    HFONT   hOldFont = SelectObject (lpps->hdc, hFont);
    RECT    rcStatus;

    SetBkColor (lpps->hdc, GetSysColor (COLOR_BTNFACE));
    SetTextColor (lpps->hdc, crText);

    SetRect (&rcStatus,
	     xSTATBOX+1,
	     ySTATBOX+1,
	     xSTATBOX+cxSTATBOX,
	     ySTATBOX+dylSTATBOX+dyuSTATBOX+yChar);

    ExtTextOut (lpps->hdc,
		xSTATBOX+dxSTATBOX,
		ySTATBOX+dyuSTATBOX,
		ETO_OPAQUE | ETO_CLIPPED,
		&rcStatus,
		lpszText,
		strlen (lpszText),
		NULL);

    SelectObject (lpps->hdc, hOldFont);
}




void WINAPI SetStatusText (
    HWND	hWnd,
    int 	nIDS_STATUS,
    COLORREF	crText)
{
    char    szStatus[MAX_PATH];

    LoadString (GetModuleHandle (NULL), nIDS_STATUS, szStatus, sizeof (szStatus));
    SendMessage (GetDlgItem (hWnd, IDC_STATUSWND), WM_SETTEXT, crText, (LONG)szStatus);
}
