// RgnCont.cpp : implementation file
//

#include "stdafx.h"
#include "GDIExplorer.h"
#include "RgnDisp.h"
#include "RgnCont.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegionDisplay

CRegionDisplayContained::CRegionDisplayContained()
{
}

CRegionDisplayContained::~CRegionDisplayContained()
{
}


BEGIN_MESSAGE_MAP(CRegionDisplayContained, CRegionDisplay)
        //{{AFX_MSG_MAP(CRegionDisplayContained)
                // NOTE - the ClassWizard will add and remove mapping macros here.
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegionDisplayContained message handlers

/****************************************************************************
*                     CRegionDisplayContained::initialize
* Result: void
*       
* Effect: 
*       Creates two regions, one contained in the other
****************************************************************************/

void CRegionDisplayContained::initialize()
    {
     CRect r1;
     CRect r2;

     GetClientRect(&r1);
     r1.InflateRect(-2, -2);

     Rgn1 = CreateEllipticRgnIndirect(&r1);

     r2 = r1;
     r2.left += r1.Width() / 4;
     r2.right -= r1.Width() / 4;
     r2.top += r1.Height() / 4;
     r2.bottom -= r1.Height() / 4;
     Rgn2 = CreateEllipticRgnIndirect(&r2);

     Result = CreateRectRgnIndirect(&r2); // doesn't matter what value,
                                          // we recompute it
     
    }
