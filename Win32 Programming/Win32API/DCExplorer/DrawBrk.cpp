// DrawBrk.cpp : implementation file
//

#include "stdafx.h"
#include "DCExplorer.h"
#include "DrawBrk.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int CDrawBricks::UWM_QUERY_BKMODE;

/////////////////////////////////////////////////////////////////////////////
// CDrawBricks

CDrawBricks::CDrawBricks()
{
 UWM_QUERY_BKMODE = ::RegisterWindowMessage(_T("Bricks.Query.BkMode"));
}

CDrawBricks::~CDrawBricks()
{
}


BEGIN_MESSAGE_MAP(CDrawBricks, CStatic)
	//{{AFX_MSG_MAP(CDrawBricks)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawBricks message handlers

void CDrawBricks::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect r;

	CDC bitmap;
	bitmap.CreateCompatibleDC(&dc);

	HBITMAP hbm = GetBitmap();
	CBitmap bm;
	bm.m_hObject = hbm;

	GetClientRect(&r);
	CSize sz(r.Width(), r.Height());

	CBitmap * oldbm = bitmap.SelectObject(&bm);
	dc.BitBlt(0, 0, sz.cx, sz.cy, &bitmap, 0, 0, SRCCOPY);
	
	bitmap.SelectObject(oldbm);
	bm.Detach(); // avoid gratuitous kill of object

	int bkmode = GetParent()->SendMessage(UWM_QUERY_BKMODE);

	dc.SetBkMode(bkmode);

	CString s;
	s.LoadString(IDS_BRICKS);
	dc.DrawText(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	// Do not call CStatic::OnPaint() for painting messages
}
