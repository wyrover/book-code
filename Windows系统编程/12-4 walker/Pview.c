
#include "pwalk.h"

#define BYTE_WIN_WIDTH	    77
#define CLINESSHOW   8
#define ABS(x) ((x) < 0? -(x) : (x))

/* externally defined system constants */
extern int		xChar,
			    yChar,
			    xScreen,
			    yScreen,
			    yFrame,
			    xFrame,
			    yCaption,
			    xVScrollBar;
extern HFONT hFont;
extern HWND  hMemWnd;

void	WINAPI VScrollBytes (HWND, int, int, LPMEMVIEW);
void	WINAPI KeyToScrollMsg (HWND, WPARAM);
void	WINAPI DisplayBytes (HDC, LPRECT, LPMEMVIEW, LPMEMVIEW);
void	WINAPI GetBytesLine (LPBYTE, UINT, DWORD, int, char *);
BOOL	WINAPI parcmp (LPBYTE, LPBYTE, int, LPBYTE);




HWND   WINAPI EnumViewWindows (
    HWND    hWndParent,
    HWND    hWndLast)
{

    HMENU   hMenu = GetMenu (hWndParent);
    HMENU   hViewMenu = GetSubMenu (hMenu, 2);
    char    szClass[MAX_PATH];
    char    szCaption[MAX_PATH];
    HWND    hWnd;
    int     i;

static	  int	  nWindows;
static	  ATOM	  *aMenuItems = NULL;
static	  int	  iWnd;

    /* load window class name */
    LoadString (GetModuleHandle (NULL), IDS_MEMVIEWCLASS, szClass, MAX_PATH);

    if (hWndLast == NULL)
	{
	/* start with first window in menu list */
	iWnd = 0;

	/* determine number of windows */
	nWindows = GetMenuItemCount (hViewMenu) - 5;

	/* if no popup windows, just return */
	if (nWindows <= 0)
	    return NULL;

	/* free memory from last time, if any */
	if (aMenuItems != NULL)
	    LocalFree ((HANDLE)aMenuItems);

	/* get menuitem IDs */
	aMenuItems = (ATOM *)LocalAlloc (LPTR, nWindows*sizeof (ATOM));
	for (i=5; i-5<nWindows; i++)
	    aMenuItems[i-5] = (ATOM)GetMenuItemID (hViewMenu, i);
	}

    else
	if (iWnd >= nWindows)
	    return (NULL);

    GetAtomName (aMenuItems[iWnd], szCaption, MAX_PATH);
    hWnd = FindWindow (szClass, szCaption);
    iWnd++;

    return (hWnd);
}



/* activate view window identified by atom */
void   WINAPI ActivateViewWindow (
    ATOM    aCaption)
{
    char    szClass[MAX_PATH];
    char    szCaption[MAX_PATH];
    HWND    hWnd;

    GetAtomName (aCaption, szCaption, MAX_PATH);
    LoadString (GetModuleHandle (NULL), IDS_MEMVIEWCLASS, szClass, MAX_PATH);
    hWnd = FindWindow (szClass, szCaption);
    BringWindowToTop (hWnd);
}



/* function creates a window for viewing memory */
HWND  WINAPI ViewMemory (
    HWND      hWndParent,
    char      *lpszObject,
    LPVOID    lpMem,
    int       nSize,
    int       nBase)
{
    char      szClass[MAX_PATH];

    /* load resource strings for class and window title */
    LoadString (GetModuleHandle (NULL), IDS_MEMVIEWCLASS, szClass, MAX_PATH);

    /* create memory view window */
    return (CreateWindow (szClass,
			  lpszObject,
			  WS_POPUPWINDOW | WS_VISIBLE | WS_CAPTION |
			  WS_VSCROLL | WS_THICKFRAME | WS_MAXIMIZEBOX,
			  nSize,   /* use xposition to pass memory size to window */
			  nBase,   /* use yposition to pass base memory location  */
			  BYTE_WIN_WIDTH * xChar + xVScrollBar + 2*xFrame + xChar,
			  yScreen/2,
			  hWndParent,
			  NULL,
			  GetModuleHandle (NULL),
			  lpMem));
}



LONG WINAPI MemWndProc (
    HWND      hWnd,
    UINT      uMsg,
    WPARAM    wParam,
    LPARAM    lParam)
{
    LONG       lRet = 1;
    MEMVIEW    *pmv = (LPMEMVIEW)GetWindowLong (hWnd, WXB_LPMEMVIEW);

    
    switch (uMsg)
	{
	case WM_CREATE:
	    pmv = (LPMEMVIEW)LocalAlloc (LPTR, sizeof (MEMVIEW));
	    SetWindowLong (hWnd, WXB_LPMEMVIEW, (LONG)pmv);
	    SetWindowLong (hWnd, WXB_LPOLDMEMVIEW, 0);

	    pmv->lpMem = ((LPCREATESTRUCT)lParam)->lpCreateParams;
	    pmv->nSize = ((LPCREATESTRUCT)lParam)->x;
	    pmv->nBase = ((LPCREATESTRUCT)lParam)->y;
	    pmv->nLines = (pmv->nSize+15)/16;
	    pmv->nExtraBytes = (pmv->nSize & 0x0000000F);
	    pmv->PosV = 0;

	    /* reposition window with proper x,y positions */
	    MoveWindow (hWnd,
			15,
			15,
			((LPCREATESTRUCT)lParam)->cx,
			((LPCREATESTRUCT)lParam)->cy,
			TRUE);
            break;

	case WM_SIZE:
	    pmv->xWin = LOWORD (lParam);
	    pmv->yWin = HIWORD (lParam);

	    pmv->RangeV = pmv->nLines-pmv->yWin/yChar;

	    if (pmv->RangeV < 0)
		pmv->RangeV = 0;

	    if (pmv->PosV > pmv->RangeV)
		pmv->PosV = pmv->RangeV;

	    SetScrollRange (hWnd, SB_VERT, 0, pmv->RangeV, FALSE);
	    SetScrollPos (hWnd, SB_VERT, pmv->PosV, TRUE);
            break;

	case WM_PAINT:
	    {
	    PAINTSTRUCT    ps;

	    BeginPaint (hWnd, &ps);
	    DisplayBytes (ps.hdc,
			  &(ps.rcPaint),
			  pmv,
			  (LPMEMVIEW)GetWindowLong (hWnd, WXB_LPOLDMEMVIEW));
	    EndPaint (hWnd, &ps);
	    }
            break;

	case UM_UPDATE:
	    InvalidateRect (hWnd, NULL, TRUE);
	    UpdateWindow (hWnd);
	    break;

        case WM_KEYDOWN:
	    KeyToScrollMsg (hWnd, wParam);
            break;

        case WM_VSCROLL:
	    VScrollBytes (hWnd, LOWORD (wParam), GetScrollPos (hWnd, SB_VERT), pmv);
            break;

	case WM_DESTROY:
	    {
	    char	 szCaption[MAX_PATH];
	    LPMEMVIEW	 lpmv;

	    /* free virtual memory block  and local memory buffer */
	    VirtualFree (pmv->lpMem, 0, MEM_RELEASE);
	    LocalFree ((HANDLE)pmv);

	    /* remove any old view memory */
	    if ((lpmv = (LPMEMVIEW)GetWindowLong (hWnd, WXB_LPOLDMEMVIEW)) != NULL)
		{
		VirtualFree (lpmv->lpMem, 0, MEM_RELEASE);
		LocalFree ((HANDLE)lpmv);
		}

	    /* send message to parent to remove menuitem */
	    GetWindowText (hWnd, szCaption, MAX_PATH);
	    SendMessage (GetParent (hWnd), WM_COMMAND, IDM_REMOVEVIEWWND, (LONG)szCaption);
	    }
            break;

        default:
	    return (DefWindowProc(hWnd, uMsg, wParam, lParam));
            break;
	}
    return 0L;
}




void  WINAPI VScrollBytes (
    HWND	 hWnd,
    int 	 cmd,
    int 	 pos,
    LPMEMVIEW	 lpmv)
{
    int    nScrollInc;

    switch (cmd)
	{
        case SB_TOP:
	    nScrollInc = -lpmv->PosV;
            break ;

        case SB_BOTTOM:
	    nScrollInc = lpmv->RangeV - lpmv->PosV;
            break ;

        case SB_LINEUP:
            nScrollInc = -1;
            break;

        case SB_LINEDOWN:
            nScrollInc = 1;
            break;

        case SB_PAGEDOWN:
	    nScrollInc = max (1, lpmv->yWin/yChar);
            break;

        case SB_PAGEUP:
	    nScrollInc = min (-1, -(lpmv->yWin/yChar));
            break;
    
        case SB_THUMBPOSITION:
	    nScrollInc = pos - lpmv->PosV;
            break;

	case SB_THUMBTRACK:
	    nScrollInc = pos - lpmv->PosV;
            break;

        default:
            nScrollInc = 0;
	}

    if (nScrollInc > lpmv->RangeV-lpmv->PosV)
	nScrollInc = lpmv->RangeV-lpmv->PosV;

    if (nScrollInc < -lpmv->PosV)
	nScrollInc = -lpmv->PosV;

    if (nScrollInc)
	{
	lpmv->PosV += nScrollInc;

	if (ABS (nScrollInc) < lpmv->yWin/yChar)
	    ScrollWindow (hWnd, 0, -(yChar*nScrollInc), NULL, NULL);
	else
	    InvalidateRect (hWnd, NULL, TRUE);

	SetScrollPos (hWnd, SB_VERT, lpmv->PosV, TRUE);
	UpdateWindow(hWnd);
	}
}



void  WINAPI KeyToScrollMsg (
    HWND      hWnd,
    WPARAM    wParam)
{
    int       i, nKeys;
    static    struct
	{
        WORD wVirtKey;
        int  iMessage;
        WORD wRequest;
	}key2scroll [] = {
			 VK_HOME,  WM_VSCROLL, SB_TOP,	  VK_END,   WM_VSCROLL,
			 SB_BOTTOM, VK_PRIOR, WM_VSCROLL, SB_PAGEUP, VK_NEXT,
			 WM_VSCROLL, SB_PAGEDOWN, VK_UP,    WM_VSCROLL, SB_LINEUP,
			 VK_DOWN,  WM_VSCROLL, SB_LINEDOWN, VK_LEFT,  WM_HSCROLL,
			 SB_LINEUP, VK_RIGHT, WM_HSCROLL, SB_LINEDOWN,
			 };

    nKeys = sizeof key2scroll / sizeof key2scroll[0];

    for (i=0; i<nKeys; i++)
	{
        if (wParam == key2scroll[i].wVirtKey)   
	    {
            SendMessage (hWnd, key2scroll[i].iMessage, key2scroll[i].wRequest, 0L);
            return;
	    }
	}
}



void  WINAPI DisplayBytes (
    HDC 	 hDC,
    RECT	 *pRect,
    LPMEMVIEW	 lpmv,
    LPMEMVIEW	 lpmvOld)
{
    UINT      i, r, iFirst, iLast, y;
    char      szBuf[MAX_PATH], szText[MAX_PATH];
    LPBYTE    pMem = (LPBYTE)lpmv->lpMem;
    LPBYTE    pOldMem;
    BYTE      bRes[16];
    COLORREF  crOldColor = 0;
    SIZE      sz;
    int       cx;

    if (lpmvOld != NULL)
	{
	pOldMem = (LPBYTE)lpmvOld->lpMem;

	/* normalize to beginning of section relative to current memory */
	(int)pOldMem += lpmvOld->nBase - lpmv->nBase;
	}

    SelectObject (hDC, hFont);
    SetTextColor (hDC, GetSysColor (COLOR_WINDOWTEXT));
    SetBkColor (hDC, GetSysColor (COLOR_WINDOW));

    y = lpmv->PosV;

    iFirst = y + pRect->top/yChar-1;

    if (iFirst == 0xFFFFFFFFL)
	iFirst = 0;

    iLast = min (y + pRect->bottom/yChar+1, (UINT)lpmv->nLines-1);
    pMem += iFirst << 4;
    pOldMem += iFirst << 4;

    if (lpmv->nExtraBytes)
        iLast--;

    /* paint complete lines (lines with 16 bytes) */
    GetBytesLine (NULL, 0, 0, 0, NULL);

    y = (iFirst - y) * yChar;
    for (i = iFirst; i<iLast; i++)
	{
	GetBytesLine (pMem, i, lpmv->nBase, 16, szBuf);

	if (lpmvOld != NULL &&
	    ((int)pMem-(int)lpmv->lpMem + lpmv->nBase) >= lpmvOld->nBase &&
	    ((int)pMem-(int)lpmv->lpMem + lpmv->nBase) < (lpmvOld->nBase + lpmvOld->nSize) &&
	    !parcmp (pMem, pOldMem, 16, bRes))
	    {
	    /* write line number and leading space */
	    strncpy (szText, szBuf, 12);
	    szText[12] = 0;
	    GetTextExtentPoint (hDC, szText, 12, &sz);
	    cx = xChar;
	    SetTextColor (hDC, GetSysColor (COLOR_HIGHLIGHTTEXT));
	    TextOut (hDC, cx, LOWORD (y), szText, 12);

	    /* check each hex byte for change */
	    for (r=0; r<16; r++)
		{
		if (bRes[r])
		    SetTextColor (hDC, RGB (191, 0, 0));
		else
		    SetTextColor (hDC, GetSysColor(COLOR_WINDOWTEXT));

		/* draw next byte */
		szText[0] = szBuf[3*r+12];
		szText[1] = szBuf[3*r+1+12];
		szText[2] = szBuf[3*r+2+12];
		szText[3] = 0;
		cx += sz.cx;
		GetTextExtentPoint (hDC, szText, 3, &sz);
		TextOut (hDC, cx, LOWORD (y), szText, 3);
		}

	    /* check each ascii byte */
	    for (r=0; r<16; r++)
		{
		if (bRes[r])
		    SetTextColor (hDC, RGB (191, 0, 0));
		else
		    SetTextColor (hDC, GetSysColor(COLOR_WINDOWTEXT));

		/* draw next byte */
		szText[0] = szBuf[r+60];
		szText[1] = 0;
		cx += sz.cx;
		GetTextExtentPoint (hDC, szText, 1, &sz);
		TextOut (hDC, cx, LOWORD (y), szText, 3);
		}
	    }

	else
	    {
	    SetTextColor (hDC, GetSysColor(COLOR_WINDOWTEXT));
	    TextOut (hDC, xChar, LOWORD (y), szBuf, lstrlen (szBuf));
	    }

	pMem += 16;
	pOldMem += 16;
	y += yChar;
	}

    /* paint last partial line if any (line with less than 16 bytes) */
    if (lpmv->nExtraBytes)
	{
	GetBytesLine (pMem, i, lpmv->nBase, lpmv->nExtraBytes, szBuf);

	if (lpmvOld != NULL &&
	    ((int)pMem-(int)lpmv->lpMem + lpmv->nBase) >= lpmvOld->nBase &&
	    ((int)pMem-(int)lpmv->lpMem + lpmv->nBase) < (lpmvOld->nBase + lpmvOld->nSize) &&
	    !parcmp (pMem, pOldMem, lpmv->nExtraBytes, bRes))
	    {
	    /* write line number and leading space */
	    strncpy (szText, szBuf, 12);
	    szText[12] = 0;
	    GetTextExtentPoint (hDC, szText, 12, &sz);
	    cx = xChar;
	    SetTextColor (hDC, RGB (191, 0, 0));
	    TextOut (hDC, cx, LOWORD (y), szText, 12);

	    /* check each hex byte for change */
	    for (r=0; r<(UINT)lpmv->nExtraBytes; r++)
		{
		if (bRes[r])
		    SetTextColor (hDC, RGB (191, 0, 0));
		else
		    SetTextColor (hDC, GetSysColor(COLOR_WINDOWTEXT));

		/* draw next byte */
		szText[0] = szBuf[3*r+12];
		szText[1] = szBuf[3*r+1+12];
		szText[2] = szBuf[3*r+2+12];
		szText[3] = 0;
		cx += sz.cx;
		GetTextExtentPoint (hDC, szText, 3, &sz);
		TextOut (hDC, cx, LOWORD (y), szText, 3);
		}

	    /* check each ascii byte */
	    for (r=0; r<(UINT)lpmv->nExtraBytes; r++)
		{
		if (bRes[r])
		    SetTextColor (hDC, RGB (191, 0, 0));
		else
		    SetTextColor (hDC, GetSysColor(COLOR_WINDOWTEXT));

		/* draw next byte */
		szText[0] = szBuf[r+60];
		szText[1] = 0;
		cx += sz.cx;
		GetTextExtentPoint (hDC, szText, 1, &sz);
		TextOut (hDC, cx, LOWORD (y), szText, 1);
		}
	    }

	else
	    {
	    SetTextColor (hDC, GetSysColor(COLOR_WINDOWTEXT));
	    TextOut(hDC, xChar, y, szBuf, lstrlen (szBuf));
	    }
	}

    SelectObject(hDC, GetStockObject(SYSTEM_FONT));
}





void  WINAPI GetBytesLine (
    LPBYTE   pBytes,
    UINT     LineNum,
    DWORD    dwBase,
    int      nBytes,
    char     *szLine)
{
    int 	     j;
    unsigned char    ch;
    char	     szAscii[18];
    static BOOL      bDBCS;

    if (NULL == pBytes) {
	bDBCS = FALSE;
	return;
    }

    wsprintf (szLine, "%#08lx  ", dwBase + (LineNum*16));
        
    for (j = 0; j < nBytes; j++)
	{
	ch = *pBytes++;
	wsprintf (szLine, "%s%02X ", (LPSTR)szLine, (WORD)ch);

	if (bDBCS)
	    {
	    szAscii[j] = (0 == j) ? '.' : ch;
	    bDBCS = FALSE;
	    }
	else if (IsDBCSLeadByte(ch))
	    {
	    szAscii[j] = ch;
	    bDBCS = TRUE;
	    }
	else
	    {
	    szAscii[j] = ((ch & 0x7F) >= ' ') ? ch : '.';
	    }
	}

    if (bDBCS && 16 == nBytes)
	szAscii[j++] = *pBytes;
    szAscii[j] = 0;
    wsprintf(szLine, "%-59s%s", (LPSTR)szLine, (LPSTR)szAscii);
}




BOOL	WINAPI parcmp (
    LPBYTE    pMem,
    LPBYTE    pOldMem,
    int       nWidth,
    LPBYTE    pRes)
{
    BOOL    bResult = TRUE;
    int     i;

    /* clear result array */
    for (i=0; i<16; i++)
	pRes[i] = 0;

    for (i=0; i<nWidth; i++)
	{
	if (pMem[i] ^ pOldMem[i])
	    {
	    pRes[i] = 1;
	    bResult = FALSE;
	    }
	}

    return bResult;
}




BOOL WINAPI AddrDlgProc (
    HWND      hDlg,
    UINT      uMsg,
    WPARAM    wParam,
    LPARAM    lParam)
{
    BOOL    bRet = TRUE;
    char    szAddr[MAX_PATH];
    int     nAddr;
    HWND    hAddr = GetDlgItem (hDlg, IDC_ADDR);

    switch (uMsg)
	{
	case WM_INITDIALOG:
	    SetFocus (hAddr);
	    bRet = FALSE;
	    break;

	case WM_COMMAND:
	    switch (LOWORD (wParam))
		{
		case IDC_HEX:
		    if (HIWORD (wParam) == BN_CLICKED)
			{
			GetWindowText (hAddr, szAddr, MAX_PATH);

			if (SendMessage (GetDlgItem (hDlg, IDC_HEX), BM_GETCHECK, 0, 0))
			    {
			    /* convert to hex and return to control */
			    nAddr = atoi (szAddr);
			    wsprintf (szAddr, "%16x", nAddr);
			    SetWindowText (hAddr, szAddr);
			    }
			else
			    {
			    /* convert to decimal and return to control */
			    sscanf (szAddr, "%16x", &nAddr);
			    SetWindowText (hAddr, itoa (nAddr, szAddr, 10));
			    }

			/* select entire string for easy change */
			SendMessage (hAddr, EM_SETSEL, 0, (LONG)-1);
			SetFocus (hAddr);
			}
		    break;

		case IDOK:
		    GetWindowText (hAddr, szAddr, MAX_PATH);

		    /* if hex, convert to decimal integer */
		    if (SendMessage (GetDlgItem (hDlg, IDC_HEX), BM_GETCHECK, 0, 0))
			sscanf (szAddr, "%16x", &nAddr);
		    else
			nAddr = atoi (szAddr);

		    EndDialog (hDlg, nAddr);
		    break;

		case IDCANCEL:
		    EndDialog (hDlg, 0);
		    break;
		}
	    break;

	default:
	    bRet = FALSE;
	}

    return (bRet);
}
