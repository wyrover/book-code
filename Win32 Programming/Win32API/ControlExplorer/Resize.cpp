// Resize.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "Resize.h"
#include "is95.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResizableTree

CResizableTree::CResizableTree()
{
 limits = NULL;
 coords = NULL;
 bias.x = 0;
 bias.y = 0;
}

CResizableTree::~CResizableTree()
{
}


BEGIN_MESSAGE_MAP(CResizableTree, CTreeCtrl)
        //{{AFX_MSG_MAP(CResizableTree)
        ON_WM_SIZE()
        ON_MESSAGE(WM_EXITSIZEMOVE, OnExitSizeMove)
        ON_WM_GETMINMAXINFO()
        ON_WM_MOUSEMOVE()
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResizableTree message handlers

void CResizableTree::OnSize(UINT nType, int cx, int cy) 
{
        CTreeCtrl::OnSize(nType, cx, cy);
        
        if(::IsWindow(sizer.m_hWnd))
           { /* has sizer */
            CRect r;
            sizer.GetWindowRect(&r);

            // Invalidate a region around the rectangle, compensating
            // for the bug in SBS_SIZEBOXBOTTOMRIGHTALIGN, which points
            // out that there is an off-by-3 error that exists in NT3.51

            int fudge = 3;
            if(Is95())
               fudge = 0; // apparently only Win95 doesn't have the bug

            CRect ivr = r;
            ScreenToClient(&ivr);
            ivr.InflateRect(fudge, fudge);
            InvalidateRect(&ivr);

            sizer.MoveWindow(cx - r.Width() - fudge,  // 
                             cy - r.Height() - fudge,
                             r.Width(),
                             r.Height());
            if(coords == NULL)
               { /* create coord window */
#define XOFFSET 2
#define YOFFSET 5
                
                CRect wr;
                GetWindowRect(&wr);

                createCoordWindow(CPoint(wr.right, wr.bottom));
               } /* create coord window */

            CRect wr;
            GetWindowRect(&wr);
            ScreenToClient(&wr);
            showCoord(CPoint(wr.Width(), wr.Height()));

            CRect w;
            CRect p;
            coords->GetWindowRect(&w);
            GetWindowRect(&p);

            GetParent()->ScreenToClient(&p);
            GetParent()->ScreenToClient(&w);

            coords->MoveWindow(p.right + XOFFSET, 
                               p.bottom - w.Height() - YOFFSET,
                               w.Width(), w.Height());
                
           } /* has sizer */
}

void CResizableTree::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
 if(limits == NULL)
    CTreeCtrl::OnGetMinMaxInfo(lpMMI);
 else
    { /* use local limits */
     lpMMI->ptMaxSize.x = limits->Width();
     lpMMI->ptMaxSize.y = limits->Height();
     lpMMI->ptMaxTrackSize.x = lpMMI->ptMaxSize.x;
     lpMMI->ptMaxTrackSize.y = lpMMI->ptMaxSize.y;
    } /* use local limits */
}

/****************************************************************************
*                          CResizableTree::setResize
* Inputs:
*       BOOL mode: TRUE to enable resizer, FALSE to disable it
* Result: void
*       
* Effect: 
*       Sets or clears the resize control
****************************************************************************/

void CResizableTree::setResize(BOOL mode)
    {
     if(mode)
        { /* enable */
         CRect r;
         GetClientRect(&r);
         sizer.Create(WS_VISIBLE | SBS_SIZEBOX | /* SBS_SIZEGRIP | */
                                    SBS_SIZEBOXBOTTOMRIGHTALIGN,
                                    r,
                                    this, 0);
        } /* enable */
     else
        { /* disable */
         CRect r;
         sizer.GetWindowRect(&r);
         ScreenToClient(&r);
         r.InflateRect(3, 3);
         sizer.DestroyWindow();
         InvalidateRect(&r);
         if(coords != NULL)
            coords->DestroyWindow();
         coords = NULL;
        } /* disable */
     
    }

/****************************************************************************
*                       CResizableTree::OnExitSizeMove
* Inputs:
*       WPARAM: ignored
*       LPARAM: ignored
* Result: LRESULT
*       0, always
* Effect: 
*       When the drag stops, destroys the size display window
****************************************************************************/

LRESULT CResizableTree::OnExitSizeMove(WPARAM, LPARAM)
    {
     if(coords != NULL)
        { /* destroy it */
         delete coords;
         coords = NULL;
        } /* destroy it */
     return 0;
    }


/****************************************************************************
*                          CResizableTree::setLimits
* Inputs:
*       CRect * r: Desired limits of resize, expressed in screen coordinates
*                       or NULL to remove all limits
* Result: void
*       
* Effect: 
*       Establishes the desired limits
****************************************************************************/

void CResizableTree::setLimits(CRect * r)
    {
     if(r == NULL)
        { /* no limits */
         if(limits != NULL)
           { /* delete limits */
            delete limits;
            limits = NULL;
           } /* delete limits */
        } /* no limits */
     else
        { /* desires limits */
         if(limits == NULL)
            limits = new CRect;
         *limits = *r;
        } /* desires limits */
         
    }

void CResizableTree::createCoordWindow(CPoint wp)
{
    CString s(_T("1000,1000"));
    CClientDC dc(this);
    LOGFONT lf;
    CFont * f;
    f = GetFont();
    f->GetObject(sizeof(lf), &lf);

    CFont nf;
    nf.CreateFontIndirect(&lf);
    dc.SelectObject(&nf);
    CSize sz = dc.GetTextExtent(s);

    CRect tr;
    tr.left = wp.x + XOFFSET;
    tr.top  = wp.y - sz.cy - YOFFSET;
    tr.right = tr.left + sz.cx;
    tr.bottom = tr.top + sz.cy;

    coords = new CStatic;
    coords->Create(NULL, SS_CENTER | WS_VISIBLE | WS_BORDER, tr,
                                               GetParent());
    coords->SetFont(f);
    nf.Detach();
    
}

void CResizableTree::showCoord(CPoint p)
{
     CString s;
     s.Format(_T("%d,%d"), p.x, p.y);
     coords->SetWindowText(s);
}


void CResizableTree::OnMouseMove(UINT nFlags, CPoint point) 
{
        if(Is95() && GetCapture() != NULL)
           { /* handle 95 resizing */
            if(coords == NULL)
               { /* create coord window */
                CRect wr;
                sizer.GetWindowRect(&wr);
                ScreenToClient(&wr);
                createCoordWindow(CPoint(wr.right, wr.bottom));
               } /* create coord window */

            CRect w;
            coords->GetWindowRect(&w);
            GetParent()->ScreenToClient(&w);
            coords->MoveWindow(point.x + XOFFSET,
                               point.y - w.Height() - YOFFSET,
                               w.Width(), w.Height());


            showCoord(CPoint(point.x - bias.x, point.y - bias.y));
           } /* handle 95 resizing */
        
        CTreeCtrl::OnMouseMove(nFlags, point);
}
