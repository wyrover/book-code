// Delta.cpp : implementation file
//

#include "stdafx.h"
#include "SynchronizationExplorer.h"
#include "Delta.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CBrush lowerBrush(RGB(0, 0, 255));
static CBrush upperBrush(RGB(255, 0, 0));
static CBrush fillBrush(::GetSysColor(COLOR_3DFACE));
/////////////////////////////////////////////////////////////////////////////
// CDelta

CDelta::CDelta()
{
 loops = 0;
 actual = 0;
}

CDelta::~CDelta()
{
}


BEGIN_MESSAGE_MAP(CDelta, CStatic)
        //{{AFX_MSG_MAP(CDelta)
        ON_WM_PAINT()
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDelta message handlers

void CDelta::OnPaint() 
{
        CPaintDC dc(this); // device context for painting
        
        CRect r;
        GetWindowRect(&r);
        ScreenToClient(&r);

        dc.DrawEdge(&r,EDGE_SUNKEN, BF_RECT);

        // Adjust sizes for borders...
        // Note that we don't actually know the size of a 3D edge...
#define EdgeWidth 3
        r.left += EdgeWidth;
        r.right -= EdgeWidth;
        r.top  += EdgeWidth;
        r.bottom -= EdgeWidth;


        // The value of 'loops' may be larger than 'actual'

        /*
           +-+
           | | 
           | | 
           | | 
           | | 
           | | 
           +-+ ------------
           | |           ^
           | |           |
           | |           |
           +-+ ---       |
           | |  ^        |
           | |  |        Loops
           | |  |        |
           | |  Actual   |
           | |  |        |
           | |  |        |
           | |  v        v
           +-+ ------------
        */

        int scale = 1;
        while(loops > scale * r.Height())
           { /* scale it */
            scale *= 2;
           } /* scale it */

        CRect lower = r;
        lower.top = lower.bottom - actual / scale;

        dc.FillRect(&lower, &lowerBrush);

        CRect upper = r;
        upper.top = upper.bottom - loops / scale;
        upper.bottom = lower.top - 1;

        
        dc.FillRect(&upper, &upperBrush);
        
        CRect rest = r;
        rest.bottom = upper.top + 1;
        
        dc.FillRect(&rest, &fillBrush);

        // Do not call CStatic::OnPaint() for painting messages
}
