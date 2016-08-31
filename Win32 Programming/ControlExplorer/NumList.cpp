// NumericList.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "NumList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNumericList

CNumericList::CNumericList()
{
}

CNumericList::~CNumericList()
{
}


BEGIN_MESSAGE_MAP(CNumericList, CListBox)
        //{{AFX_MSG_MAP(CNumericList)
                // NOTE - the ClassWizard will add and remove mapping macros here.
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNumericList message handlers

int CNumericList::CompareItem(LPCOMPAREITEMSTRUCT cis) 
{
        // return -1 = item 1 sorts before item 2
        // return 0 = item 1 and item 2 sort the same
        // return 1 = item 1 sorts after item 2
        
        return (cis->itemData1 - cis->itemData2);
}

void CNumericList::DrawItem(LPDRAWITEMSTRUCT dis) 
{
 CRect r = dis->rcItem;
 CDC * dc = CDC::FromHandle(dis->hDC);
 CBrush br;
 CBrush * oldbrush;
 COLORREF txcolor;
 COLORREF bkcolor;

 int saved = dc->SaveDC();
 if(dis->itemState & ODS_SELECTED)
    { /* selected */
     bkcolor = GetSysColor(COLOR_HIGHLIGHT);
     txcolor = GetSysColor(COLOR_HIGHLIGHTTEXT);
    } /* selected */
 else
    { /* unselected */
     if(dis->itemState & (ODS_DISABLED | ODS_GRAYED))
        txcolor = GetSysColor(COLOR_GRAYTEXT);
     else
        txcolor = GetSysColor(COLOR_WINDOWTEXT);

     bkcolor = GetSysColor(COLOR_WINDOW);
    } /* unselected */
     
 dc->SetBkColor(bkcolor);
 dc->SetTextColor(txcolor);
        
 br.CreateSolidBrush(bkcolor);
 oldbrush = dc->SelectObject(&br);

 dc->PatBlt(r.left, r.top, r.Width(), r.Height(), PATCOPY);

 dc->SelectObject(oldbrush);
 br.DeleteObject();

 if(dis->itemID != (UINT)-1 
    && (dis->itemState & (ODS_DISABLED | ODS_GRAYED)) == 0)
    { /* has item */
     CString s;
     s.Format(_T("%d"), dis->itemData);
     
     dc->DrawText(s, &r, DT_RIGHT);
    } /* has item */
 
 if(dis->itemState & ODS_FOCUS && dis->itemAction != ODA_SELECT)
    dc->DrawFocusRect(&r);

 dc->RestoreDC(saved);
}

/****************************************************************************
*                           CNumericList::AddString
* Inputs:
*       int i: integer to add
* Result: int
*       Index of where it was added
* Effect: 
*       Adds the integer to the list box
****************************************************************************/

int CNumericList::AddString(int i)
    {
     return CListBox::AddString((LPCTSTR)i);
    }

/****************************************************************************
*                          CNumericList::FindString
* Inputs:
*       int start: Starting position
*       int i: Integer to find
* Result: int
*       Index of location where found, or LB_ERR
****************************************************************************/

int CNumericList::FindString(int start, int i)
    {
     return CListBox::FindString(start, (LPCTSTR)i);
    }
