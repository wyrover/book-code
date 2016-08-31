// poplist.cpp : implementation file
//

#include "stdafx.h"
#include "FontExplorer.h"
#include "poplist.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPopupList

CPopupList::CPopupList()
{
}

CPopupList::~CPopupList()
{
}


BEGIN_MESSAGE_MAP(CPopupList, CListBox)
        //{{AFX_MSG_MAP(CPopupList)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPopupList message handlers

void CPopupList::PostNcDestroy() 
{
        CListBox::PostNcDestroy();

        delete this;
}

/*****************************************************************************
   The main reason we have to do this is that Microsoft violates all
   principles of "modularity" to achieve "small size", and therefore the
   expected implementation of these functions, by calling
   CListBox::On<<whateverevent>> actually doesn't work; it ignores the
   parameters passed in and instead just uses the parameters from the last
   message, whatever they were.  So we have to violate all good principles of
   modularity and abstraction and read the MFC code, discover how the
   parameters are being split up, and do a SendMessage.  Ugly, but Microsoft
   seems to think this makes sense.

   The actual gain in size seems to be minuscle; the tradeoff to program
   structuring seems profound.  But "small and fast" apparently is more
   important that "infinitesimally larger, unmeasurably slower, and correct".
*****************************************************************************/

void CPopupList::DoLButtonDown(UINT nFlags, CPoint point) 
{
 SendMessage(WM_LBUTTONDOWN, (WPARAM)nFlags, MAKELONG(point.x, point.y));
}

void CPopupList::DoLButtonUp(UINT nFlags, CPoint point)
{
 SendMessage(WM_LBUTTONUP, (WPARAM)nFlags, MAKELONG(point.x, point.y));
}

void CPopupList::DoLButtonDblClk(UINT nFlags, CPoint point) 
{
 SendMessage(WM_LBUTTONDBLCLK, (WPARAM)nFlags, MAKELONG(point.x, point.y));
}

void CPopupList::DoRButtonDblClk(UINT nFlags, CPoint point) 
{
 SendMessage(WM_RBUTTONDBLCLK, (WPARAM)nFlags, MAKELONG(point.x, point.y));
}

void CPopupList::DoRButtonDown(UINT nFlags, CPoint point) 
{
 SendMessage(WM_RBUTTONDOWN, (WPARAM)nFlags, MAKELONG(point.x, point.y));
}

void CPopupList::DoRButtonUp(UINT nFlags, CPoint point) 
{
 SendMessage(WM_RBUTTONUP, (WPARAM)nFlags, MAKELONG(point.x, point.y));
}

BOOL CPopupList::PreCreateWindow(CREATESTRUCT& cs) 
{
 cs.style |= WS_VSCROLL;
        
        return CListBox::PreCreateWindow(cs);
}
