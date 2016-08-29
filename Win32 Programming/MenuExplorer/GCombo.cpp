// gCombo.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "idcombo.h"
#include "gcombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CGraphicCombo

CGraphicCombo::CGraphicCombo()
{
 width = 0;
}

CGraphicCombo::~CGraphicCombo()
{
}

/****************************************************************************
*                           CGraphicCombo::SetWidth
* Inputs:
*       int n: Width to et
* Result: void
*       
* Effect: 
*       Sets the width to use for the graphics display box
****************************************************************************/

void CGraphicCombo::SetWidth(int n)
    {
     width = n;
    }


BEGIN_MESSAGE_MAP(CGraphicCombo, CIDCombo)
        //{{AFX_MSG_MAP(CGraphicCombo)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphicCombo message handlers

void CGraphicCombo::DrawItem(LPDRAWITEMSTRUCT dis) 
{
 CRect r = dis->rcItem;
 SIZE w;
 CDC * dc = CDC::FromHandle(dis->hDC);
 CBrush br;
 CBrush * oldbrush;
 COLORREF txcolor;
 COLORREF bkcolor;

 int saved = dc->SaveDC();

 if(dis->itemState & ODS_SELECTED)
    { /* selected */
     bkcolor = GetSysColor(COLOR_HIGHLIGHT);
     txcolor = GetSysColor(COLOR_HIGHLIGHTTEXT);
    } /* selected */
 else
    { /* unselected */
     if(dis->itemState & (ODS_DISABLED | ODS_GRAYED))
        txcolor = GetSysColor(COLOR_GRAYTEXT);
     else
        txcolor = GetSysColor(COLOR_WINDOWTEXT);

     bkcolor = GetSysColor(COLOR_WINDOW);
    } /* unselected */

 dc->SetBkColor(bkcolor);
 dc->SetTextColor(txcolor);

 br.CreateSolidBrush(bkcolor);
 oldbrush = dc->SelectObject(&br);

 dc->PatBlt(r.left, r.top, r.Width(), r.Height(), PATCOPY);

 dc->SelectObject(oldbrush);
 br.DeleteObject();

 if(dis->itemID != (UINT)-1 
    && (dis->itemState & (ODS_DISABLED | ODS_GRAYED)) == 0)
    { /* has item */
     
     if(width <= 0)
        w = dc->GetTextExtent(_T("WW"), 2);
     else
        w.cx = width;

     r.right = r.left + w.cx;

     drawSample(&r, dis);

     r.left = r.right;
     r.right = dis->rcItem.right;
     w = dc->GetTextExtent(_T(" "), 1);
     r.left += w.cx;

     CString s;
     GetLBText(dis->itemID, s);

     
     // Since we don't know the actual text size, and TextOut will 
     // write anywhere it wants to (including over top of the dropdown
     // button, at least on NT3.51), we force a clipping region.

     int tsaved = dc->SaveDC();
     CRgn hrgn;

     hrgn.CreateRectRgnIndirect(&dis->rcItem);
     dc->SelectClipRgn(&hrgn);
     
     dc->TextOut(r.left, r.top, s);

     dc->RestoreDC(tsaved);
    } /* has item */
 
 if(dis->itemState & ODS_FOCUS && dis->itemAction != ODA_SELECT)
    dc->DrawFocusRect(&r);

 dc->RestoreDC(saved);
}

