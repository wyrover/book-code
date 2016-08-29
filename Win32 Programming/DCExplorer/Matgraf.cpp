// MatGraf.cpp : implementation file
//

#include "stdafx.h"
#include "DCExplorer.h"
#include "matsamp.h"
#include "MatGraf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMatrixGraphic

CMatrixGraphic::CMatrixGraphic()
{
}

CMatrixGraphic::~CMatrixGraphic()
{
}


BEGIN_MESSAGE_MAP(CMatrixGraphic, CMatrixSample)
	//{{AFX_MSG_MAP(CMatrixGraphic)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMatrixGraphic message handlers

void CMatrixGraphic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRgn rgn;
	
	CRect r;

	GetClientRect(&r);

	rgn.CreateRectRgnIndirect(r);

	// Create a font which is a proportion of the height of the rectangle

	CFont font;

	font.CreateFont( r.Height() / 5, 0, 0, 0, 
			 FW_NORMAL, 		// weight
			 FALSE,			// italic
			 FALSE,			// underline
			 FALSE,			// strikeout
			 ANSI_CHARSET,		// character set
			 OUT_DEFAULT_PRECIS, 
			 CLIP_DEFAULT_PRECIS, 
			 DEFAULT_QUALITY, 
			 VARIABLE_PITCH, 
			 _T("Times New Roman") );

	dc.SelectObject(&font);
	dc.SelectClipRgn(&rgn);
	

	CString s;

	s.LoadString(IDS_GRAPHIC_EXAMPLE);


	CString space = _T(" ");

	CSize slen = dc.GetTextExtent(s);
	CSize splen = dc.GetTextExtent(space);


        /*
                       slen.cx+splen.cx
                       :
             0 splen.cx:    2 * splen.cx + slen.cx
             : :       :    :    4 * splen.cx + slen.cx
             : :       :    :    :
        0...............................
             : :       :  p2:....:........... .5 * splen.cy
           p0: :       :    |\   :
1.5*splen.cy.+--------------+ \  :
             |.:       :    p1 \ :
             |  One Way         >:... 2.5 * splen.cy
             |              p5 / :p3
3.5*splen.cy.+--------------+ /  :
           p6:              |/...:...........  4.5 * splen.cy
             :            p4:    :
5*splen.y    ...............................
             :              :


        */


	// We have to use :: global operations here because MFC 4.1 and
	// earlier do not have these as class operations on a CDC

	CPen graypen(PS_SOLID, 0, RGB(192, 192, 192));

	CPoint origin(0, 0);

	CBrush fillbr(GetSysColor(COLOR_WINDOW));
	dc.FillRect(&r, &fillbr);

	dc.MoveTo(r.left, origin.y);
	dc.LineTo(r.right, origin.y);

	dc.MoveTo(origin.x, r.top);
	dc.LineTo(origin.x, r.bottom);

	::SetGraphicsMode(dc.m_hDC, GM_ADVANCED);
	if(::SetWorldTransform(dc.m_hDC, &matrix))
	   { /* success */
	    CPen pen;
	    LOGBRUSH lb;

	    lb.lbStyle = BS_SOLID;
	    lb.lbColor = RGB(0, 0, 0);

	    pen.CreatePen(PS_GEOMETRIC | PS_JOIN_MITER | PS_ENDCAP_SQUARE,
	    			1, &lb);

	    dc.SelectObject(&pen);

	    dc.MoveTo(r.left, origin.y);
	    dc.LineTo(r.right, origin.y);

	    dc.MoveTo(origin.x, r.top);
	    dc.LineTo(origin.x, r.bottom);

	    dc.SelectClipRgn(&rgn);

	    // Draw the "one way" arrow
	    CPoint p0(0, (3 * splen.cy) / 2);
	    CPoint p1(3 * splen.cx + slen.cx, p0.y);
	    CPoint p2(p1.x, splen.cy / 2);
	    CPoint p3(p2.x + 5 * splen.cx, (5 * splen.cy) / 2);
	    CPoint p4(p2.x, (9 * splen.cy) / 2);
	    CPoint p5(p4.x, (7 * splen.cy) / 2);
	    CPoint p6(p0.x, p5.y);

	    dc.BeginPath();
	    dc.MoveTo(p0.x, p0.y);
	    dc.LineTo(p1.x, p1.y);
	    dc.LineTo(p2.x, p2.y);
	    dc.LineTo(p3.x, p3.y);
	    dc.LineTo(p4.x, p4.y);
	    dc.LineTo(p5.x, p5.y);
	    dc.LineTo(p6.x, p6.y);
	    dc.CloseFigure();
	    dc.EndPath();
	    dc.SelectStockObject(BLACK_BRUSH);
	    dc.StrokeAndFillPath();

	    dc.SetTextColor(RGB(255,255,255));
	    dc.SetTextAlign(TA_BOTTOM);
	    dc.SetBkMode(TRANSPARENT); // make sure grid stays visible
	    dc.TextOut(p6.x + splen.cx, p6.y - splen.cy / 2, s);
	    error = 0;
	   } /* success */
        else
	   { /* failure */
	    CBrush fillbr(GetSysColor(COLOR_3DFACE));
	    dc.FillRect(&r, &fillbr);
	    error = GetLastError();
	   } /* failure */
	   

	// Do not call CStatic::OnPaint() for painting messages
}
