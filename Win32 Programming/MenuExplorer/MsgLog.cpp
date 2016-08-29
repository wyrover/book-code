// MsgLog.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "MsgLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DIM(x) (sizeof(x) / sizeof(x[0]))

/////////////////////////////////////////////////////////////////////////////
// CMessageLog

CMessageLog::CMessageLog()
{
 edit = NULL;
 wrap = FALSE;
 autoextend = FALSE;
}

CMessageLog::~CMessageLog()
{
}


BEGIN_MESSAGE_MAP(CMessageLog, CListCtrl)
	//{{AFX_MSG_MAP(CMessageLog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMessageLog message handlers

void CMessageLog::initialize(int n) 
{
	LV_COLUMN col;
	int total = 0;

	CString hdr;
	hdr.LoadString(IDS_HANDLE);
	col.cx = max(GetStringWidth(hdr), 
		     GetStringWidth(_T("0x00000000")));

	// We have to add the width of a small icon to the column width
        col.cx += ::GetSystemMetrics(SM_CXICON);
        total += col.cx;
        col.fmt = LVCFMT_RIGHT;
	col.pszText = (LPTSTR)(LPCTSTR)hdr;
	col.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	col.iSubItem = 0;
	InsertColumn(0, &col);

	hdr.LoadString(IDS_CONTROL_TYPE);
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
	hdr.LoadString(IDS_EVENT);
	col.cx = r.Width() - total; 
	// We have to reduce by the width of a small icon to the column width
        // col.cx -= ::GetSystemMetrics(SM_CXICON);
	col.fmt = LVCFMT_LEFT;
	col.pszText = (LPTSTR)(LPCTSTR)hdr;
	col.iSubItem = 2;
	col.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	InsertColumn(2, &col);
	
}

/****************************************************************************
*                         CMessageLog::getEditControl
* Inputs:
*       int width: Desired width
* Result: CEdit *
*       An edit control whose width is the same as the specified one
* Effect: 
*       If the edit control does not exist, one is created.  If one already
*	exists, it may be resized.
****************************************************************************/

CEdit * CMessageLog::getEditControl(int width)
    {
     if(edit == NULL)
        { /* create a new one */
	 CRect rect(0, 0, width, width);  // height arbitrarily set to width
	 edit = new CEdit;
	 // Note that we create the window invisible and disabled!
	 // it also has no border, and needs multiline and autovscroll
	 edit->Create(WS_DISABLED | WS_CHILD | 
	 	     ES_MULTILINE | ES_AUTOVSCROLL, 
		     				rect, this, 0);
	} /* create a new one */
     else
        { /* use existing one */
	 edit->SetWindowPos(NULL, 0, 0, width, width, SWP_NOMOVE | SWP_NOZORDER);
	} /* use existing one */

     return edit;
    }

/****************************************************************************
*                          CMessageLog::addMessage
* Inputs:
*	HANDLE hwnd: Handle to display, or 0
*	CString type: Control type or other descriptive info
*	CString event: Event information to display
* Result: int
*       Position in which message was added
* Effect: 
*       Logs the message to the message box.  If the message would not be
*	visible, the list box is scrolled to make it visible.
****************************************************************************/

int CMessageLog::addMessage(HANDLE hwnd, CString type, CString event)
    {
     int i = GetItemCount();

     // Since the index is 0-based, the count i is the next available position

     CString s;
     if((UINT)hwnd == 0)
	s = _T("");
     else
	s.Format(_T("0x%08x"), (UINT)hwnd);

     InsertItem(i, s);

     SetItemText(i, 1, type);

     // we need to do line wrap here

     int w = GetStringWidth(event);
     int col = GetColumnWidth(2);

     if(wrap && w > col)
        { /* needs wrap */
	 // This is a fascinating kludge.  We could write a complex line-break
	 // algorithm, but why bother?  Windows already has one!  What we
	 // do is create a dummy invisible edit control that is the same
	 // width as the control column, stuff the text into it, then 
	 // retrieve it a line at a time!  Sleazy hack, but it works and I
	 // could write it quickly!

	 CEdit * edit = getEditControl(col);
	 edit->SetWindowText(event);
	 int lines = edit->GetLineCount();
	 for(int line = 0; line < lines; line++)
	    { /* get each line */
	     TCHAR temp[256];
	     int chars = edit->GetLine(line, temp, DIM(temp));
	     temp[chars] = _T('\0');

	     // If this is the first line, we just insert it since we are
	     // already at the slot where it should go.  If it is a subsequent
	     // line, we must insert a new entry
	     if(line > 0)
	        { /* insert new line */
		 i++;
		 InsertItem(i, _T(""));
		 SetItemText(i, 1, _T(""));
		} /* insert new line */
	     SetItemText(i, 2, temp);
	    } /* get each line */
	} /* needs wrap */
     else
        { /* do it straight */
	 SetItemText(i, 2, event);
	 if(autoextend && w > col)
	    SetColumnWidth(2, w);
	} /* do it straight */

     EnsureVisible(i, FALSE);
     
     return i;
    }
