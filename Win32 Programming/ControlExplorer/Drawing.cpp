// Drawing.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "Drawing.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int CDrawing::UWM_QUERY_XOFFSET = 0;
int CDrawing::UWM_QUERY_YOFFSET = 0;
int CDrawing::UWM_QUERY_SIZE    = 0;
int CDrawing::UWM_QUERY_PENSIZE = 0;

/////////////////////////////////////////////////////////////////////////////
// CDrawing

CDrawing::CDrawing()
{
 UWM_QUERY_XOFFSET = ::RegisterWindowMessage("drawing.xoffset");
 UWM_QUERY_YOFFSET = ::RegisterWindowMessage("drawing.yoffset");
 UWM_QUERY_SIZE    = ::RegisterWindowMessage("drawing.size");
 UWM_QUERY_PENSIZE = ::RegisterWindowMessage("drawing.pensize");
}

CDrawing::~CDrawing()
{
}


BEGIN_MESSAGE_MAP(CDrawing, CStatic)
        //{{AFX_MSG_MAP(CDrawing)
        ON_WM_PAINT()
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawing message handlers

void CDrawing::OnPaint() 
{
        CPaintDC dc(this); // device context for painting
        
        CRect size;
        GetClientRect(&size);

        CRgn rgn;
        rgn.CreateRectRgnIndirect(&size);

        dc.SelectClipRgn(&rgn);

        CBrush br(GetSysColor(COLOR_3DFACE));

        dc.FillRect(&size, &br);

        int xoffset = GetParent()->SendMessage(UWM_QUERY_XOFFSET, 0, 0);
        int yoffset = GetParent()->SendMessage(UWM_QUERY_YOFFSET, 0, 0);

        CRect r;
        GetParent()->SendMessage(UWM_QUERY_SIZE, 0, (LPARAM)&r);

        int pensize = GetParent()->SendMessage(UWM_QUERY_PENSIZE, 0, 0);

        dc.Ellipse(xoffset - r.Width() / 2, yoffset - r.Height() / 2,
                   xoffset + r.Width() / 2, yoffset + r.Height() / 2);

        // Draw a crosshair

        dc.MoveTo(0, yoffset);
        dc.LineTo(size.Width(), yoffset);
        dc.MoveTo(xoffset, 0);
        dc.LineTo(xoffset, size.Height());

        
        // Do not call CStatic::OnPaint() for painting messages
}
