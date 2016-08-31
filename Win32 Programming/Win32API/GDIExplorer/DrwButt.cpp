// DrwButt.cpp : implementation file
//

#include "stdafx.h"
#include "GDIExplorer.h"
#include "DrwButt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDrawEdgeButton

CDrawEdgeButton::CDrawEdgeButton()
{
 upstyle = 0;
 downstyle = 0;
}

CDrawEdgeButton::~CDrawEdgeButton()
{
}


BEGIN_MESSAGE_MAP(CDrawEdgeButton, CButton)
        //{{AFX_MSG_MAP(CDrawEdgeButton)
                // NOTE - the ClassWizard will add and remove mapping macros here.
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawEdgeButton message handlers

void CDrawEdgeButton::DrawItem(LPDRAWITEMSTRUCT dis) 
{
 CRect r = dis->rcItem;
 CDC * dc = CDC::FromHandle(dis->hDC);
 COLORREF textcolor;

#ifdef _DEBUG_DRAWITEM
 {
  CString s;
  static int line = 0;
  s.Format(_T("%03d: ODA: %s%s%s ODS:%s%s%s\r\n"), line,
                dis->itemAction & ODA_DRAWENTIRE ? _T("D") : _T(""),
                dis->itemAction & ODA_FOCUS      ? _T("F") : _T(""),
                dis->itemAction & ODA_SELECT     ? _T("S") : _T(""),
                dis->itemState  & ODS_CHECKED    ? _T("C") : _T(""),
                dis->itemState  & ODS_DISABLED   ? _T("D") : _T(""),
                dis->itemState  & ODS_FOCUS      ? _T("F") : _T(""),
                dis->itemState  & ODS_GRAYED     ? _T("G") : _T(""));
  line++;
  OutputDebugString(s);
 }
#endif 

 int saved = dc->SaveDC();
        
 CString s;
 GetWindowText(s);


 if(dis->itemState & ODS_DISABLED)
    { /* disabled */
     textcolor = ::GetSysColor(COLOR_GRAYTEXT);
    } /* disabled */
 else
    { /* enabled */
     textcolor = ::GetSysColor(COLOR_WINDOWTEXT);
    } /* enabled */

 dc->SetTextColor(textcolor);

 CSize sz = dc->GetTextExtent(s);

 // deal with all the possible style flags and options

 CSize pos(0,0);

 int hstyle = 0;  // in case no case is the case

 DWORD style = GetStyle();
 style &= (BS_LEFT | BS_RIGHT | BS_CENTER);
     
 switch(style)
    { /* hstyle */
     case BS_LEFT:
             pos.cx = 0;
             break;
     case BS_RIGHT:
             pos.cx = r.Width() - sz.cx;
             break;
     case BS_CENTER:
             pos.cx = (r.Width() - sz.cx) / 2;
             break;
    } /* hstyle */

 style = GetStyle();
 style &=  ( BS_TOP | BS_BOTTOM | BS_VCENTER);

 int vstyle = 0;

 switch(style)
    { /* vstyle */
      
     case BS_TOP:
             pos.cy = 0;
             break;
     case BS_BOTTOM:
             pos.cy = r.Height() - sz.cy;
             break;
     case BS_VCENTER:
             pos.cy = (r.Height() - sz.cy) / 2;
             break;
    } /* vstyle */

 style = GetStyle();

 // Note: we do not support multiline style for this control

 // get the rectangle for the focus
 CRect focus;

 focus.left = pos.cx;
 focus.top  = pos.cy;
 focus.right = pos.cx + sz.cx;
 focus.bottom = pos.cy + sz.cy;
 focus.InflateRect(1,1); // allow a little more space around the text

 // For visual effect, if the button is down, shift the text over and down
 // to make it look like it is being "pushed"

 CSize offset(0, 0);;

 if(dis->itemState & ODS_SELECTED)
    { /* button down */
     offset.cx = 2 * ::GetSystemMetrics(SM_CYBORDER);
     offset.cy += 2 * ::GetSystemMetrics(SM_CXBORDER);
    } /* button down */

 if(dis->itemAction & ODA_SELECT)
    { /* selection changed */
     // We have to erase the focus rectangle if it was drawn, because of
     // the xorness of the drawing action

     if(dis->itemState & ODS_SELECTED)
        { /* pushed down */
         dc->DrawFocusRect(&focus);
        } /* pushed down */
     else
        { /* left up */
         CRect oldfocus = focus;
         focus += offset;
         dc->DrawFocusRect(&oldfocus);
        } /* left up */
    } /* selection changed */

 if(dis->itemState & ODS_SELECTED)
    { /* button down */
     dc->DrawEdge(&r, LOWORD(downstyle), HIWORD(downstyle));
    } /* button down */
 else
    { /* button up */
     dc->DrawEdge(&r, LOWORD(upstyle), HIWORD(upstyle));
    } /* button up */

 dc->TextOut(pos.cx + offset.cx, pos.cy + offset.cy, s);

 focus += offset; // compute new position

 if(dis->itemState & ODS_FOCUS)
    dc->DrawFocusRect(&focus);

 dc->RestoreDC(saved);
}
