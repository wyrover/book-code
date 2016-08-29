// Hatch.cpp : implementation file
//

#include "stdafx.h"
#include "DCExplorer.h"
#include "Hatch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHatchSample

CHatchSample::CHatchSample()
{
 bkg = RGB(255, 255, 255);
 BkMode = OPAQUE;
}

CHatchSample::~CHatchSample()
{
}


BEGIN_MESSAGE_MAP(CHatchSample, CStatic)
	//{{AFX_MSG_MAP(CHatchSample)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHatchSample message handlers

void CHatchSample::OnPaint() 
    {
     CBrush br(HS_DIAGCROSS, RGB(0,0,0));
     {
      CPaintDC dc(this); // device context for painting

      CRect r;

      GetClientRect(&r);

      CBrush fillbr1(GetSysColor(COLOR_WINDOW));
      CBrush fillbr2(RGB(128, 128, 0));

      // create a bi-colored background
      CRect rf;

      rf = r;
      rf.right = rf.Width() / 2;
      dc.FillRect(&rf, &fillbr1);

      rf = r;
      rf.left = rf.Width() / 2;
      dc.FillRect(&rf, &fillbr2);

      // Set the background color and mode according to the user selection

      dc.SetBkColor(bkg);
      dc.SetBkMode(BkMode);

#if 0
      LOGBRUSH lb;
      lb.lbColor = RGB(0,0,0);
      lb.lbStyle = BS_HATCHED;
      lb.lbHatch = HS_DIAGCROSS;

      CPen pen(PS_GEOMETRIC | PS_ENDCAP_FLAT, r.Height(), &lb);

      dc.SelectObject(pen);

      dc.BeginPath();
      dc.MoveTo(r.left, r.Height() / 2);
      dc.LineTo(r.right, r.Height() / 2);
      dc.EndPath();
      dc.StrokePath();
      dc.SelectStockObject(NULL_PEN);
#else
      dc.SelectObject(&br);
      dc.Rectangle(&r);
#endif
     }
    }
