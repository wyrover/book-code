// EdgeSamp.cpp : implementation file
//

#include "stdafx.h"
#include "GDIExplorer.h"
#include "EdgeSamp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEdgeSample

CEdgeSample::CEdgeSample()
{
}

CEdgeSample::~CEdgeSample()
{
}


BEGIN_MESSAGE_MAP(CEdgeSample, CStatic)
        //{{AFX_MSG_MAP(CEdgeSample)
        ON_WM_PAINT()
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEdgeSample message handlers

void CEdgeSample::OnPaint() 
{
        CPaintDC dc(this); // device context for painting
        
        CRect r;
        GetClientRect(&r);

        DWORD style = ::GetWindowLong(m_hWnd, GWL_USERDATA);

        if(!dc.DrawEdge(&r, LOWORD(style), HIWORD(style)))
           ::MessageBeep(0);
        
        // Do not call CStatic::OnPaint() for painting messages
}
