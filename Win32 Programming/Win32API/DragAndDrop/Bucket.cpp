// Bucket.cpp : implementation file
//

#include "stdafx.h"
#include "dipper.h"
#include "Bucket.h"
#include "query.h"
#include "status.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HCURSOR CBucket::fulldipper = NULL;
HCURSOR CBucket::emptydipper = NULL;
int CBucket::QueryDrop = 0;
int CBucket::DoDrop = 0;

/////////////////////////////////////////////////////////////////////////////
// CBucket

CBucket::CBucket()
{
 if(fulldipper == NULL)
    fulldipper = AfxGetApp()->LoadCursor(IDC_FULL_DIPPER);

 if(emptydipper == NULL)
    emptydipper = AfxGetApp()->LoadCursor(IDC_EMPTY_DIPPER);

 if(QueryDrop == 0)
    QueryDrop = ::RegisterWindowMessage(_T("Dipper.QueryDrop"));
 if(DoDrop == 0)
    DoDrop = ::RegisterWindowMessage(_T("Dipper.DoDrop"));
}

CBucket::~CBucket()
{
}


BEGIN_MESSAGE_MAP(CBucket, CStatic)
	//{{AFX_MSG_MAP(CBucket)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_REGISTERED_MESSAGE(UWM_DoDrop, dropIt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBucket message handlers

void CBucket::OnLButtonDown(UINT nFlags, CPoint point) 
{
 	SetCapture();
	oldcursor = ::SetCursor(fulldipper);
	
	CStatic::OnLButtonDown(nFlags, point);
}

/****************************************************************************
*                             CBucket::findTarget
* Inputs:
*       CPoint point: Point of mouse, in client coordinates
* Result: HWND
*       Window handle of target of drop, or NULL if cannot be found
* Notes: 
*       This will find the child of the window.
****************************************************************************/

HWND CBucket::findTarget(CPoint point)
    {
     ClientToScreen(&point);  // get screen coordinates
     HWND target = ::WindowFromPoint(point);     
     if(target == NULL)
	return NULL;

     // we may have a reference to our main window.  We want a reference
     // to one of our child windows.  Find it

     // The most general case is to transform coordinates in the target
     // window
     ::ScreenToClient(target, &point);
     HWND child = ::ChildWindowFromPoint(target, point);
     if(child == NULL)
	return target;

     return child;
    }

void CBucket::OnLButtonUp(UINT nFlags, CPoint point) 
{
 CWnd * capture = GetCapture();
 	if(capture != NULL && capture->m_hWnd == this->m_hWnd)
	   { /* release */
	    ReleaseCapture();
	    ::SetCursor(oldcursor);
	    HWND target = findTarget(point);
	    if(target != NULL)
	       if(!::SendMessage(target, UWM_DoDrop, 0, 0))
		  GetParent()->SendMessage(UWM_Status, IDS_WASTE);
               else
		  GetParent()->SendMessage(UWM_Status, IDS_READY);
	   } /* release */
	
	CStatic::OnLButtonUp(nFlags, point);
}

void CBucket::OnMouseMove(UINT nFlags, CPoint point) 
{
 CWnd * wnd = GetCapture();
 if(wnd != NULL && wnd->m_hWnd == this->m_hWnd)
    { /* we have capture */
     HWND target = findTarget(point);
     if(target != NULL)
	{ /* ask it */
	 switch(::SendMessage(target, UWM_QueryDrop, 0, 0))
	    { /* query */
	     case 0: // unknown target
		     GetParent()->SendMessage(UWM_Status, IDS_CARRYING);
		     break;
	     case QUERY_DROP_OK:
		     GetParent()->SendMessage(UWM_Status, IDS_DROPPABLE);
		     break;
	     case QUERY_DROP_FULL:
		     GetParent()->SendMessage(UWM_Status, IDS_FULL);
		     break;
	    } /* query */
	   } /* ask it */
    } /* we have capture */
 else
    { /* no capture */
     GetParent()->SendMessage(UWM_Status, IDS_READY);
    } /* no capture */

 CStatic::OnMouseMove(nFlags, point);
}

UINT CBucket::OnNcHitTest(CPoint point) 
{
 	return HTCLIENT;
}

BOOL CBucket::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
 CWnd * capture = GetCapture();
 	if(capture != NULL && capture->m_hWnd == this->m_hWnd)
	   ::SetCursor(fulldipper);
        else
	   ::SetCursor(emptydipper);
	
	return 0;
}

/****************************************************************************
*                               CBucket::dropIt
* Inputs:
*       WPARAM: ignored
*	LPARAM: ignored
* Result: LRESULT
*       TRUE, always
* Notes: 
*       We put this here so dropping water back into the bucket doesn't
*	generate a "wasted water" message
****************************************************************************/

LRESULT CBucket::dropIt(WPARAM, LPARAM)
    {
     return TRUE;
    }
