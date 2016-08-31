// PName.cpp : implementation file
//

#include "stdafx.h"
#include "PrintExplorer.h"
#include "PData.h"
#include "PName.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPaperNameData

CPaperNameData::CPaperNameData()
{
}

CPaperNameData::~CPaperNameData()
{
}


BEGIN_MESSAGE_MAP(CPaperNameData, CPaperData)
        //{{AFX_MSG_MAP(CPaperNameData)
                // NOTE - the ClassWizard will add and remove mapping macros here.
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPaperNameData message handlers

void CPaperNameData::DrawLine(CDC * dc, LPPaperInfo pi, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
 CRect r;
 CString id;

 r = lpDrawItemStruct->rcItem;
 r.right = tabs[0];
 r.left += margin;

 id.Format("%d", pi->paper);
 dc->DrawText(id, &r, DT_RIGHT);

 r.right += margin;

 r.left = r.right;
 r.right = tabs[1];

 if(!(lpDrawItemStruct->itemState & ODS_SELECTED))
    showPreferredRectangle(dc, &r);

 dc->MoveTo(r.left, r.top);
 dc->LineTo(r.left, r.bottom);

 r.left += margin;
 dc->DrawText(pi->papername, -1, &r, DT_LEFT);

 findPaperID(pi);

 dc->MoveTo(r.right, r.top);
 dc->LineTo(r.right, r.bottom);

 r.right += margin;
 r.left = r.right;
 r.right = lpDrawItemStruct->rcItem.right;

 if(pi->id != 0)
    { /* has id */
     CString s;
     s.LoadString(pi->id);
     dc->DrawText(s, &r, DT_LEFT);
    } /* has id */


}

/****************************************************************************
*                      CPaperNameData::recompute_widths
* Result: void
*       
* Effect: 
*       Runs through the table computing the widths for proper layout
****************************************************************************/

void CPaperNameData::recompute_widths()
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
