// PSizes.cpp : implementation file
//

#include "stdafx.h"
#include "PrintExplorer.h"
#include "PData.h"
#include "PSizes.h"
#include "mmin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPaperSizes

CPaperSizes::CPaperSizes()
{
}

CPaperSizes::~CPaperSizes()
{
}


BEGIN_MESSAGE_MAP(CPaperSizes, CPaperData)
        //{{AFX_MSG_MAP(CPaperSizes)
                // NOTE - the ClassWizard will add and remove mapping macros here.
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPaperSizes message handlers

void CPaperSizes::DrawLine(CDC * dc, LPPaperInfo pi, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
 CRect r;
 CString id;

 r = lpDrawItemStruct->rcItem;
 r.right = tabs[0];
 r.left += margin;
 id.Format("%d", pi->paper);
 dc->DrawText(id, &r, DT_RIGHT);

 r.right += margin;
 dc->MoveTo(r.right, r.top);
 dc->LineTo(r.right, r.bottom);

 r.left = r.right + margin;
 r.right = tabs[1];
 dc->DrawText(pi->papername, -1, &r, DT_LEFT);

 dc->MoveTo(r.right, r.top);
 dc->LineTo(r.right, r.bottom);

 r.right += margin;
 r.left = r.right;
 r.right = lpDrawItemStruct->rcItem.right;

 CString s;
 s.Format(_T(MM_1_FMT " x " MM_1_FMT " mm (" IN_001_FMT " x  " IN_001_FMT " in)"), 
               mm_1_Int(pi->papersize.x), mm_1_Frac(pi->papersize.x),
               mm_1_Int(pi->papersize.y), mm_1_Frac(pi->papersize.y),
               in_001_Int(pi->papersize.x), in_001_Frac(pi->papersize.x),
               in_001_Int(pi->papersize.y), in_001_Frac(pi->papersize.y));

 if(!(lpDrawItemStruct->itemState & ODS_SELECTED))
    showPreferredRectangle(dc, &r);

 dc->DrawText(s, &r, DT_LEFT);

}

/****************************************************************************
*                      CPaperSizes::recompute_widths
* Result: void
*       
* Effect: 
*       Runs through the table computing the widths for proper layout
****************************************************************************/

void CPaperSizes::recompute_widths()
    {
     int count = GetCount();
     int width = 0;

     if(count == 0)
        return;

     CClientDC dc(this);
     CSize sz;

     sz = dc.GetTextExtent(_T("0000"), 4);
     tabs[0] = sz.cx;

     for(int i = 0; i < count; i++)
        { /* scan entries */
         LPPaperInfo pi = GetItemData(i);
         sz = dc.GetTextExtent(pi->papername);
         width = max(width, sz.cx);
        } /* scan entries */

     sz = dc.GetTextExtent(_T(" "), 1);
     margin = sz.cx;

     tabs[1] = tabs[0] + width + margin;
    }
