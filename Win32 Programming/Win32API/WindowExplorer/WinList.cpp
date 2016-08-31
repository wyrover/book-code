// WinList.cpp : implementation file
//

#include "stdafx.h"
#include "afxtempl.h"
#include "WndExp.h"
#include "WinList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DIM(x) (sizeof(x) / sizeof((x)[0]))

/////////////////////////////////////////////////////////////////////////////
// CWindowList

CWindowList::CWindowList()
{
}

CWindowList::~CWindowList()
{
}


BEGIN_MESSAGE_MAP(CWindowList, CListCtrl)
	//{{AFX_MSG_MAP(CWindowList)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindowList message handlers

/****************************************************************************
*                           CWindowList::initialize
* Inputs:
*	Needed width of second column (class name)
* Result: void
*       
* Effect: 
*       Initializes the window list.  Sets up the headers
****************************************************************************/

void CWindowList::initialize(int n)
    {
     LV_COLUMN col;
     int total = 0;

     CString hdr;

     hdr.LoadString(IDS_HANDLE);
     col.cx = max(GetStringWidth(hdr), 
     		  GetStringWidth(_T("0x00000000")));
     col.cx += ::GetSystemMetrics(SM_CXICON);
     total += col.cx;
     col.fmt = LVCFMT_RIGHT;
     col.pszText = (LPTSTR)(LPCTSTR)hdr;
     col.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
     col.iSubItem = 0;
     InsertColumn(0, &col);

     hdr.LoadString(IDS_CLASS);
     col.cx = n;
     // We have to add the width of a small icon to the column width
     col.cx += ::GetSystemMetrics(SM_CXICON);
     col.cx = max(col.cx, GetStringWidth(hdr));
     total += col.cx;
     col.fmt = LVCFMT_LEFT;
     col.pszText = (LPTSTR)(LPCTSTR)hdr;
     col.iSubItem = 1;
     col.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
     InsertColumn(1, &col);

     CRect r;
     GetClientRect(&r);
     hdr.LoadString(IDS_CAPTION);
     col.cx = r.Width() - total; 
     // We have to reduce by the width of a small icon to the column width
     col.cx -= ::GetSystemMetrics(SM_CXICON);
     col.fmt = LVCFMT_LEFT;
     col.pszText = (LPTSTR)(LPCTSTR)hdr;
     col.iSubItem = 2;
     col.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
     InsertColumn(2, &col);
    }

/****************************************************************************
*                              CWindowList::Add
* Inputs:
*       HWND hwnd: Window to add to list
* Result: void
*       
* Effect: 
*       Adds the window to the list
****************************************************************************/

void CWindowList::Add(HWND hwnd)
    {
     int i = GetItemCount();
     CWnd * wnd = CWnd::FromHandle(hwnd);

     CString s;
     s.Format(_T("0x%08x"), (UINT)hwnd);

     // This is an aberration.  We have to specify an item position, but
     // it appears to be ignored if the list view is sorted (LVS_SORT*),
     // and a different value is returned.
     i = InsertItem(i, s);

     TCHAR name[256];
     ::GetClassName(hwnd, name, DIM(name));
     SetItemText(i, 1, name);

     CString caption;
     wnd->GetWindowText(caption);
     SetItemText(i, 2, caption);

     SetItemData(i, (DWORD)hwnd);
    }

/****************************************************************************
*			    CWindowList::GetCurItem
* Result: HWND
*       Handle of selected window, or NULL if no selection
****************************************************************************/

HWND CWindowList::GetCurItem()
    {
     LV_ITEM item;
     int count = GetItemCount();
     int selected = -1;

     item.mask = LVIF_STATE | LVIF_PARAM;

     for(int i = 0; i < count; i++)
	{ /* scan each */
	 item.iItem = i;
	 item.iSubItem = 0;  // we want the item itself
	 item.stateMask = 0xFFFFFFFF;
	 GetItem(&item);
	 if(item.state & LVIS_SELECTED)
	    { /* found selection */
	     selected = i;
	     break;
	    } /* found selection */
	} /* scan each */

     if(selected < 0)
	return NULL;

     return (HWND)item.lParam;
    }
