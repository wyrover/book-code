// WeiXinBtn.cpp : implementation file
//

#include "stdafx.h"
#include "Test.h"
#include "WeiXinBtn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWeiXinBtn

CWeiXinBtn::CWeiXinBtn()
{
}

CWeiXinBtn::~CWeiXinBtn()
{
}


BEGIN_MESSAGE_MAP(CWeiXinBtn, CButton)
	//{{AFX_MSG_MAP(CWeiXinBtn)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWeiXinBtn message handlers

void CWeiXinBtn::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ShowWindow(SW_HIDE);
	m_pBtn->ShowWindow(SW_SHOW);
	CButton::OnMouseMove(nFlags, point);
}
