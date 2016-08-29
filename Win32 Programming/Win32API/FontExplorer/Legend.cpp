// Legend.cpp : implementation file
//

#include "stdafx.h"
#include "FontExplorer.h"
#include "legendi.h"
#include "Legend.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLegend

CLegend::CLegend()
{
}

CLegend::~CLegend()
{
}


BEGIN_MESSAGE_MAP(CLegend, CComboBox)
        //{{AFX_MSG_MAP(CLegend)
        ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropdown)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLegend message handlers

void CLegend::DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct) 
{
        // TODO: Add your specialized code here and/or call the base class
        
        CComboBox::DeleteItem(lpDeleteItemStruct);
}

void CLegend::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
 SIZE sz;

 if(lpDrawItemStruct->itemData == (DWORD)-1)
    return;

 LegendInfo * li = (LegendInfo *)lpDrawItemStruct->itemData;

 CRect r = lpDrawItemStruct->rcItem;
 CDC * dc = CDC::FromHandle(lpDrawItemStruct->hDC);
 dc->SelectObject(li->pen);
 dc->SelectObject(li->brush);

 sz = dc->GetTextExtent(_T(" "), 1);

 CString s;
 s.LoadString(li->id);

 r.right = r.left + ::GetSystemMetrics(SM_CXVSCROLL);  // random useful value
 dc->Rectangle(&r);

 r.left = r.right + 2 * sz.cx;
 r.right = lpDrawItemStruct->rcItem.right;
 dc->DrawText(s, &r, DT_LEFT);
}

void CLegend::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
 TEXTMETRIC tm;
 CClientDC dc(this);
 dc.GetTextMetrics(&tm);
 lpMeasureItemStruct->itemHeight = tm.tmHeight + tm.tmExternalLeading;
        
}

/****************************************************************************
*                             CLegend::AddString
* Inputs:
*       LegendInfo * li: 
* Result: int
*       Places where it was inserted
* Effect: 
*       Inserts the object in the list
****************************************************************************/

int CLegend::AddString(LegendInfo * li)
    {
     return CComboBox:: AddString((LPCSTR)li);
    }

/****************************************************************************
*                             CLegend::OnDropdown
* Result: void
*       
* Effect: 
*       Sizes the legend to show the full legend set
****************************************************************************/

void CLegend::OnDropdown() 
{
 int n = GetCount();
 n = max(n, 2);
 
 int h = GetItemHeight(0);  // get default item height
 CRect r;
 GetWindowRect(&r);

 CSize sz;
 sz.cx = r.right - r.left;
 sz.cy = h * (n + 2);

 SetWindowPos(NULL, 0, 0, sz.cx, sz.cy, SWP_NOMOVE | SWP_NOZORDER);
        
        
}

/****************************************************************************
*                             CLegend::AddLegends
* Inputs:
*       LegendInfo ** li: Pointer to legend pointer array
* Result: void
*       
* Effect: 
*       Adds all the legends to the table
****************************************************************************/

void CLegend::AddLegends(LegendInfo ** li)
    {
     for(int i = 0; li[i] != NULL; i++)
        AddString(li[i]);

     SetCurSel(0);
    }
