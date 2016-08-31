// Sample.cpp : implementation file
//

#include "stdafx.h"
#include "FontExplorer.h"
#include "Sample.h"
#include "savedc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSample

CSample::CSample()
{
 mode = GM_COMPATIBLE;
}

CSample::~CSample()
{
}


BEGIN_MESSAGE_MAP(CSample, CStatic)
        //{{AFX_MSG_MAP(CSample)
        ON_WM_PAINT()
        ON_WM_ERASEBKGND()
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSample message handlers

void CSample::OnPaint() 
    {
     CPaintDC dc(this); // device context for painting
     CRect r;
     CFont * font = GetFont();
        
     GetClientRect(&r);
        
     ::SetGraphicsMode(dc.m_hDC, mode);

     dc.SelectObject(font);

     dc.SetBkMode(TRANSPARENT);

     CString s;
     GetWindowText(s);


     dc.DrawText(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    }

BOOL CSample::OnEraseBkgnd(CDC* pDC) 
{
 CBrush bkg;
 CRect r;

 CSaveDC dc(pDC);

 bkg.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
 GetClientRect(&r);

 dc->SelectObject(&bkg);
 dc->PatBlt(r.left, r.top, r.Width(), r.Height(), PATCOPY);

 return CStatic::OnEraseBkgnd(pDC);
}
