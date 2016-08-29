// AlgnSamp.cpp : implementation file
//

#include "stdafx.h"
#include "DCExplorer.h"
#include "AlgnSamp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlignSample

CAlignSample::CAlignSample()
{
}

CAlignSample::~CAlignSample()
{
}


BEGIN_MESSAGE_MAP(CAlignSample, CStatic)
	//{{AFX_MSG_MAP(CAlignSample)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlignSample message handlers

void CAlignSample::OnPaint() 
{
	CBrush bk(GetSysColor(COLOR_WINDOW));
	CPen  pen(PS_SOLID, 0, GetSysColor(COLOR_GRAYTEXT));

	// We want to make sure the DC is released before we
	// see the destructors for the pen and brush selected
	// into it.  So we declare an inner scope for the DC.

	{ /* DC */
	 CPaintDC dc(this); // device context for painting
	
	 CRect r;
	
	 GetClientRect(&r);

	 CPoint center;

	 center.x = r.Width() / 2;
	 center.y = r.Height() / 2;

	 dc.FillRect(&r, &bk);
	 
	 dc.SetTextAlign(align);

	 // Make sure we can see the lines after the text is drawn
	 dc.SetBkMode(TRANSPARENT);
	
	 // We want gray lines
	 dc.SelectObject(&pen);

	 // Draw the vertical line for the center
	 dc.MoveTo(center.x, r.top);
	 dc.LineTo(center.x, r.bottom);

	 // Draw the horizontal line for the center
	 dc.MoveTo(r.left, center.y);
	 dc.LineTo(r.right, center.y);

	 // Now draw the text, using the alignment mode specified

	 CString s;
	 GetWindowText(s);

	 if(align & TA_UPDATECP)
	    { /* update */
	     dc.MoveTo(center.x, center.y);
	     CString t;

	     t = s + _T("1");
	     dc.TextOut(0, 0, t);

	     t = s + _T("2");
	     dc.TextOut(0, 0, t); // show effect of UpdateCP
	    } /* update */
	 else
	    { /* normal */
	     dc.TextOut(center.x, center.y, s);
	    } /* normal */

	} /* DC */

     // Do not call CStatic::OnPaint() for painting messages
}

BOOL CAlignSample::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CStatic::OnEraseBkgnd(pDC);
}
