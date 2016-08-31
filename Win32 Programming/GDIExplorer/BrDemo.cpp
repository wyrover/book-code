// BrDemo.cpp : implementation file
//

#include "stdafx.h"
#include "GDIExplorer.h"
#include "BrDemo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBrushDemo

CBrushDemo::CBrushDemo()
{
 brush = NULL;
}

CBrushDemo::~CBrushDemo()
{
}


BEGIN_MESSAGE_MAP(CBrushDemo, CStatic)
        //{{AFX_MSG_MAP(CBrushDemo)
        ON_WM_PAINT()
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBrushDemo message handlers

void CBrushDemo::OnPaint() 
{
        CPaintDC dc(this); // device context for painting
        
        CRect r;
        GetClientRect(&r);
        if(brush != NULL && brush->m_hObject != NULL)
           { /* has brush */
            dc.FillRect(&r, brush);
           } /* has brush */
        else
           { /* no brush */
            CBrush br(::GetSysColor(COLOR_3DFACE));
            dc.FillRect(&r, &br);
            dc.SelectStockObject(NULL_BRUSH);
            dc.Rectangle(&r);
            dc.MoveTo(r.left, r.top);
            dc.LineTo(r.right, r.bottom);
            dc.MoveTo(r.right, r.top);
            dc.LineTo(r.left, r.bottom);
           } /* no brush */

        // Do not call CStatic::OnPaint() for painting messages
}
