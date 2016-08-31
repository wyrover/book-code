// MatText.cpp : implementation file
//

#include "stdafx.h"
#include "DCExplorer.h"
#include "matsamp.h"
#include "MatText.h"
#include "uwm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMatrixText

CMatrixText::CMatrixText()
{
}

CMatrixText::~CMatrixText()
{
}


BEGIN_MESSAGE_MAP(CMatrixText, CMatrixSample)
	//{{AFX_MSG_MAP(CMatrixText)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMatrixText message handlers

void CMatrixText::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRgn rgn;
	
	CRect r;

	GetClientRect(&r);

	rgn.CreateRectRgnIndirect(r);

	// Create a font which is a proportion of the height of the rectangle

	CFont font;

	font.CreateFont( r.Height() / 4, 0, 0, 0, 
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
	
	dc.SetTextAlign(align);

	CString s = _T("G");

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
	    dc.SelectStockObject(BLACK_PEN);

	    dc.MoveTo(r.left, origin.y);
	    dc.LineTo(r.right, origin.y);

	    dc.MoveTo(origin.x, r.top);
	    dc.LineTo(origin.x, r.bottom);

	    dc.SelectClipRgn(&rgn);

	    dc.SetBkMode(TRANSPARENT); // make sure grid stays visible
	    dc.TextOut(origin.x, origin.y, s);
	    BOOL update = (error != 0);
	    error = 0;
	    if(update)
	       GetParent()->SendMessage(UWM_UPDATE_ERROR, error);
	   } /* success */
        else
	   { /* failure */
	    CBrush fillbr(GetSysColor(COLOR_3DFACE));
	    dc.FillRect(&r, &fillbr);
	    error = GetLastError();
	    GetParent()->SendMessage(UWM_UPDATE_ERROR, error);
	   } /* failure */
	   

	// Do not call CStatic::OnPaint() for painting messages
}
