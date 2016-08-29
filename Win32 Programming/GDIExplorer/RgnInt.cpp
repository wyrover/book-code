// RgnInt.cpp : implementation file
//

#include "stdafx.h"
#include "GDIExplorer.h"
#include "RgnDisp.h"
#include "RgnInt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegionDisplay

CRegionDisplayIntersect::CRegionDisplayIntersect()
{
}

CRegionDisplayIntersect::~CRegionDisplayIntersect()
{
}


BEGIN_MESSAGE_MAP(CRegionDisplayIntersect, CRegionDisplay)
        //{{AFX_MSG_MAP(CRegionDisplayIntersect)
                // NOTE - the ClassWizard will add and remove mapping macros here.
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegionDisplayIntersect message handlers

/****************************************************************************
*                     CRegionDisplayIntersect::initialize
* Result: void
*       
* Effect: 
*       Creates two regions, one intersecting the other
* Notes:
*       This assumes that the control is rectangular and is wider than high
****************************************************************************/

void CRegionDisplayIntersect::initialize()
    {
     CRect r;
     CRect r1;
     CRect r2;

     GetClientRect(&r);
     r.InflateRect(-2, -2);

     r1 = r;
     r1.right = r1.left + r1.Height();
     Rgn1 = CreateEllipticRgnIndirect(&r1);

     r2 = r;
     r2.left = r2.right - r2.Height();
     Rgn2 = CreateEllipticRgnIndirect(&r2);

     Result = CreateRectRgnIndirect(&r2); // doesn't matter what value,
                                          // we recompute it
     
    }
