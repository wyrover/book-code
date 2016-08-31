// MenuSheet.cpp : implementation file
//

#include "stdafx.h"
#include "MenuSht.h"
#include "bitmaps.h"
#include "intedit.h"
#include "tinybutt.h"
#include "msglog.h"
#include "menuitem.h"
#include "menulist.h"
#include "menudlg.h"
#include "MenuExplorer.h"
#include "uwm.h"
#include "is95.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static struct { int api; BOOL mode; DWORD flags; int id; } flagstrings[] = {
	{ 3, FALSE, TPM_RIGHTBUTTON,  IDS_TPM_LEFTBUTTON},
	{ 3, TRUE,  TPM_RIGHTBUTTON,  IDS_TPM_RIGHTBUTTON},

	{ 3, FALSE, TPM_CENTERALIGN | TPM_RIGHTALIGN, IDS_TPM_LEFTALIGN},
	{ 3, TRUE,  TPM_CENTERALIGN,		   IDS_TPM_CENTERALIGN},
	{ 3, TRUE,  TPM_RIGHTALIGN,		   IDS_TPM_RIGHTALIGN},

	{ 4, FALSE, TPM_VCENTERALIGN | TPM_BOTTOMALIGN, IDS_TPM_TOPALIGN},
	{ 4, TRUE,  TPM_VCENTERALIGN,		   IDS_TPM_VCENTERALIGN},
	{ 4, TRUE,  TPM_BOTTOMALIGN,		   IDS_TPM_BOTTOMALIGN},

	{ 4, FALSE, TPM_VERTICAL,		   IDS_TPM_HORIZONTAL},
	{ 4, TRUE,  TPM_VERTICAL,		   IDS_TPM_VERTICAL},

	{ 4, TRUE,  TPM_NONOTIFY,		   IDS_TPM_NONOTIFY},
	{ 4, TRUE,  TPM_RETURNCMD,		   IDS_TPM_RETURNCMD},
	{ 0, FALSE, 0, 0} // EOT
								    };


/****************************************************************************
*                             CMenuSheet::tpmstr
* Inputs:
*       DWORD flags: Flags to be considered
* Result: CString
*       A sequence of flags, as decoded
****************************************************************************/

CString CMenuSheet::tpmstr(DWORD flags)
    {
     CString result;
     CString sep = _T("");

     for(int i = 0; flagstrings[i].id != 0; i++)
        { /* scan each */
	 if(Is3() && flagstrings[i].api >= 4)
	    continue;  // ignore advanced options if not on platform

	 if((flagstrings[i].mode && (flags & flagstrings[i].flags) != 0) ||
	    (!flagstrings[i].mode && (flags & flagstrings[i].flags) == 0))
	   { /* found it */
	    CString t;
	    t.LoadString(flagstrings[i].id);
	    result += sep;
	    result += t;
	    sep = _T(" | ");
	   } /* found it */
	} /* scan each */

     return result;
    }


/////////////////////////////////////////////////////////////////////////////
// CMenuSheet

IMPLEMENT_DYNAMIC(CMenuSheet, CPropertySheet)

void CMenuSheet::tpmInit()
{
 tpmFlags = TPM_LEFTALIGN | TPM_LEFTBUTTON;
 trackMenu = NULL;
}
	    

CMenuSheet::CMenuSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
 tpmInit();
}

CMenuSheet::CMenuSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
 tpmInit();
}

CMenuSheet::~CMenuSheet()
{
}


BEGIN_MESSAGE_MAP(CMenuSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CMenuSheet)
	ON_WM_CLOSE()
	ON_MESSAGE(UWM_COMMAND, OnMainCommand)
	ON_MESSAGE(UWM_INITMENU, OnMainInitMenu)
	ON_MESSAGE(UWM_INITMENUPOPUP, OnMainInitMenuPopup)
	ON_MESSAGE(UWM_MENUSELECT, OnMainMenuSelect)
	ON_MESSAGE(UWM_TRACKPOPUPMENU, OnTrackPopupMenu)
	ON_MESSAGE(UWM_MENUCHAR, OnMainMenuChar)
	ON_MESSAGE(UWM_ENTERMENULOOP, OnMainEnterMenuLoop)
	ON_MESSAGE(UWM_EXITMENULOOP, OnMainExitMenuLoop)
	ON_MESSAGE(UWM_LOGMSG, OnLogMsg)
	ON_MESSAGE(UWM_CHANGE, OnChange)
	ON_MESSAGE(UWM_SETFLAGS, OnSetFlags)
	ON_MESSAGE(UWM_ADD_OWNERDRAW, OnAddOwnerDraw)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuSheet message handlers

void CMenuSheet::OnClose() 
{
 // do not call CMenuSheet::OnClose();

 // Because of the complexity of trying to track all the various changes
 // that might take place on each restart of the modeless dialog, we do
 // not actually allow the dialog to close.  Instead, we "fake" it by
 // hiding the dialog.  

 ShowWindow(SW_HIDE);

}

void CMenuSheet::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertySheet::CalcWindowRect(lpClientRect, nAdjustType);
}

/****************************************************************************
*                           CMenuSheet::retransmit
* Inputs:
*       int msg: Message to retransmit
*	WPARAM wParam: wParam value to retransmit
*	LPARAM lParam: lParam value to retransmit
* Result: void
*       
* Effect: 
*       Retransmits the command to each of the child windows
****************************************************************************/

void CMenuSheet::retransmit(int msg, WPARAM wParam, LPARAM lParam)
    {
     for(int i = 0; i < GetPageCount(); i++)
        { /* send to each */
	 CWnd * page = GetPage(i);
	 if(page != NULL && page->m_hWnd != NULL)
	    page->SendMessage(msg, wParam, lParam);
	} /* send to each */
    }

/****************************************************************************
*                          CMenuSheet::OnMainCommand
* Inputs:
*       WPARAM wParam:
*	LPARAM lParam:
* Result: LRESULT
*       0, always
* Effect: 
*       Sends the message to each of the CPropertyPage objects
****************************************************************************/

LRESULT CMenuSheet::OnMainCommand(WPARAM wParam, LPARAM lParam)
    {
     retransmit(UWM_COMMAND, wParam, lParam);
     return 0;
    }

/****************************************************************************
*                          CMenuSheet::OnMainInitMenuPopup
* Inputs:
*       WPARAM wParam:
*	LPARAM lParam:
* Result: LRESULT
*       0, always
* Effect: 
*       Sends the message to each of the CPropertyPage objects
****************************************************************************/

LRESULT CMenuSheet::OnMainInitMenuPopup(WPARAM wParam, LPARAM lParam)
    {
     retransmit(UWM_INITMENUPOPUP, wParam, lParam);
     return 0;
    }

/****************************************************************************
*                          CMenuSheet::OnMainInitMenu
* Inputs:
*       WPARAM wParam:
*	LPARAM lParam:
* Result: LRESULT
*       0, always
* Effect: 
*       Sends the message to each of the CPropertyPage objects
****************************************************************************/

LRESULT CMenuSheet::OnMainInitMenu(WPARAM wParam, LPARAM lParam)
    {
     retransmit(UWM_INITMENU, wParam, lParam);
     return 0;
    }

/****************************************************************************
*			CMenuSheet::OnMainEnterMenuLoop
* Inputs:
*       WPARAM wParam:
*	LPARAM lParam:
* Result: LRESULT
*       0, always
* Effect: 
*       Sends the message to each of the CPropertyPage objects
****************************************************************************/

LRESULT CMenuSheet::OnMainEnterMenuLoop(WPARAM wParam, LPARAM lParam)
    {
     retransmit(UWM_ENTERMENULOOP, wParam, lParam);
     return 0;
    }

/****************************************************************************
*			CMenuSheet::OnMainExitMenuLoop
* Inputs:
*       WPARAM wParam:
*	LPARAM lParam:
* Result: LRESULT
*       0, always
* Effect: 
*       Sends the message to each of the CPropertyPage objects
****************************************************************************/

LRESULT CMenuSheet::OnMainExitMenuLoop(WPARAM wParam, LPARAM lParam)
    {
     retransmit(UWM_EXITMENULOOP, wParam, lParam);
     return 0;
    }

/****************************************************************************
*                          CMenuSheet::OnMainMenuChar
* Inputs:
*       WPARAM wParam:
*	LPARAM lParam:
* Result: LRESULT
*       0, always
* Effect: 
*       Sends the message to each of the CPropertyPage objects
****************************************************************************/

LRESULT CMenuSheet::OnMainMenuChar(WPARAM wParam, LPARAM lParam)
    {
     retransmit(UWM_MENUCHAR, wParam, lParam);
     return 0;
    }

/****************************************************************************
*                          CMenuSheet::OnLogMsg
* Inputs:
*       WPARAM wParam:
*	LPARAM lParam:
* Result: LRESULT
*       0, always
* Effect: 
*       Sends the message to each of the CPropertyPage objects
****************************************************************************/

LRESULT CMenuSheet::OnLogMsg(WPARAM wParam, LPARAM lParam)
    {
     retransmit(UWM_LOGMSG, wParam, lParam);
     return 0;
    }

/****************************************************************************
*                          CMenuSheet::OnChange
* Inputs:
*       WPARAM wParam:
*	LPARAM lParam:
* Result: LRESULT
*       0, always
* Effect: 
*       Sends the message to each of the CPropertyPage objects
****************************************************************************/

LRESULT CMenuSheet::OnChange(WPARAM wParam, LPARAM lParam)
    {
     retransmit(UWM_CHANGE, wParam, lParam);
     return 0;
    }

/****************************************************************************
*                          CMenuSheet::OnAddOwnerDraw
* Inputs:
*       WPARAM wParam:
*	LPARAM lParam:
* Result: LRESULT
*       0, always
* Effect: 
*       Sends the message to each of the CPropertyPage objects
****************************************************************************/

LRESULT CMenuSheet::OnAddOwnerDraw(WPARAM wParam, LPARAM lParam)
    {
     retransmit(UWM_ADD_OWNERDRAW, wParam, lParam);
     return 0;
    }

/****************************************************************************
*                          CMenuSheet::OnMainMenuSelect
* Inputs:
*       WPARAM wParam:
*	LPARAM lParam:
* Result: LRESULT
*       0, always
* Effect: 
*       Sends the message to each of the CPropertyPage objects
****************************************************************************/

LRESULT CMenuSheet::OnMainMenuSelect(WPARAM wParam, LPARAM lParam)
    {
     retransmit(UWM_MENUSELECT, wParam, lParam);
     return 0;
    }

/****************************************************************************
*                           CMenuSheet::OnTrackPopupMenu
* Inputs:
*       WPARAM: Flags to use
*	LPARAM lParam: POINTS structure for mouse x,y (client coordinates)
* Result: LRESULT
*       logically void, 0, always
* Effect: 
*       Pops up the specified menu
* Notes:
*	This is the only page that responds to a TrackPopupMenu request
****************************************************************************/

LRESULT CMenuSheet::OnTrackPopupMenu(WPARAM flags, LPARAM lParam)
    {
     if(trackMenu == NULL)
	{ /* no menu */
	 MessageBeep(0);
	 return 0;
	} /* no menu */
     
     tpmFlags = flags;  // store for later use

     CPoint pt(LOWORD(lParam), HIWORD(lParam));

     ClientToScreen(&pt);

     CString fmt;
     UWM_LOGMSG_DATA data;

     fmt.LoadString(IDS_TRACKPOPUPMENU);

     // Log the start of the trackpopupmenu operation

     data.msg.Format(fmt, _T("=>"), trackMenu, tpmstr(tpmFlags), pt.x, pt.y, 
     				AfxGetMainWnd()->m_hWnd);
     
     data.handle = trackMenu;
     
     retransmit(UWM_LOGMSG, 0, (LPARAM)&data);

     retransmit(UWM_LOGTRACKPOPUP, 0, (LPARAM)(LPCTSTR)data.msg);
     

     int result = ::TrackPopupMenu(trackMenu, tpmFlags, pt.x, pt.y, 0, 
     					     AfxGetMainWnd()->m_hWnd, NULL);

     // TrackPopupMenu(0x%08x, %s, %d, %d, 0, 0x%08x, NULL) => %s
     fmt.LoadString(IDS_TRACKPOPUPMENU);

     // Note that the result string depends upon what flags we have set
     CString rstr;
     if(tpmFlags & TPM_RETURNCMD)
        { /* command code */
	 rstr.Format(_T("%d"), result);
	 if(result == 0)
	    data.type.LoadString(IDS_ERROR);
	 else
	    data.type = rstr;
	} /* command code */
     else
        { /* boolean */
	 rstr.LoadString(result ? IDS_TRUE : IDS_FALSE);
	 data.type.LoadString(result ? IDS_OK : IDS_ERROR);
	} /* boolean */

     data.msg.Format(fmt, _T("<="), trackMenu, tpmstr(tpmFlags), pt.x, pt.y, 
     				AfxGetMainWnd()->m_hWnd);
     
     data.msg += _T(" => ");
     data.msg += rstr;

     data.handle = trackMenu;
     
     retransmit(UWM_LOGMSG, 0, (LPARAM)&data);

     retransmit(UWM_LOGTRACKPOPUP, 0, (LPARAM)(LPCTSTR)data.msg);

     return 0;
    }

void CMenuSheet::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	
}

void CMenuSheet::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CPropertySheet::OnRButtonDown(nFlags, point);
}

/****************************************************************************
*                          CMenuSheet::OnInitDialog
* Result: BOOL
*       As returned by CPropertySheet::OnInitDialog
* Effect: 
*       Makes sure the window is in the bottom right corner of the screen
****************************************************************************/

BOOL CMenuSheet::OnInitDialog()
    {
     BOOL result = CPropertySheet::OnInitDialog();
     
     CRect r;
     GetWindowRect(&r);
     SetWindowPos(NULL, ::GetSystemMetrics(SM_CXSCREEN) - r.Width(),
     		        ::GetSystemMetrics(SM_CYSCREEN) - r.Height(),	
			0, 0, 
			SWP_NOSIZE | SWP_NOZORDER);
     return result;
    }

/****************************************************************************
*                           CMenuSheet::OnSetFlags
* Inputs:
*       WPARAM wParam: flag values
*	LPARAM: ignored
* Result: LRESULT
*       logically void, 0, always
* Effect: 
*       Stores the flag values
****************************************************************************/

LRESULT CMenuSheet::OnSetFlags(WPARAM flags, LPARAM)
    {
     tpmFlags = flags;
     return 0;
    }
