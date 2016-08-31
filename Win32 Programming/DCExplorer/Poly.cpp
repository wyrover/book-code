// Poly.cpp : implementation file
//

#include "stdafx.h"
#include "DCExplorer.h"
#include "Poly.h"
#include "polydraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPoly

CPoly::CPoly()
{
 fillmode = WINDING;
}

CPoly::~CPoly()
{
}


BEGIN_MESSAGE_MAP(CPoly, CStatic)
	//{{AFX_MSG_MAP(CPoly)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPoly message handlers

void CPoly::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// construct the polygon

	CRect r;

	CBrush br(GetSysColor(COLOR_3DFACE));

	GetClientRect(&r);

	dc.FillRect(&r, &br);

	dc.SetPolyFillMode(fillmode);
	dc.SelectStockObject(GRAY_BRUSH);

	polyDraw(&dc, r);
	
}
