// StrDisp.cpp : implementation file
//

#include "stdafx.h"
#include "HeapExplorer.h"
#include "mystring.h"
#include "StrDisp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStringDisplay

CStringDisplay::CStringDisplay()
{
}

CStringDisplay::~CStringDisplay()
{
}


BEGIN_MESSAGE_MAP(CStringDisplay, CComboBox)
        //{{AFX_MSG_MAP(CStringDisplay)
                // NOTE - the ClassWizard will add and remove mapping macros here.
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/****************************************************************************
*                          CStringDisplay::AddString
* Inputs:
*       CMyData * p: Pointer to data object to add
* Result: int
*       Index of place added or CB_ERR
* Effect: 
*       Adds the data object to the combo box
****************************************************************************/

int CStringDisplay::AddString(CMyData * p)
    {
     SetWindowLong(m_hWnd, GWL_USERDATA, CB_ADDSTRING);
     return CComboBox::AddString((LPCTSTR)p);
    }

/****************************************************************************
*                         CStringDisplay::GetItemData
* Inputs:
*       int index:
* Result: CMyData *
*       NULL if out of range
****************************************************************************/

CMyData * CStringDisplay::GetItemData(int index)
    {
     CMyData * p = (CMyData *)CComboBox::GetItemData(index);
     if(p == (CMyData *)CB_ERR)
        return NULL;
     return p;
    }

/****************************************************************************
*                       CStringDisplay::FindStringExact
* Inputs:
*       int start: Starting position
*       LPCTSTR str: String to find
* Result: int
*       Position where it is found or CB_ERR
****************************************************************************/

int CStringDisplay::FindStringExact(int start, LPCTSTR str)
    {
     SetWindowLong(m_hWnd, GWL_USERDATA, CB_FINDSTRINGEXACT);
     return CComboBox::FindStringExact(start, str);
    }

/****************************************************************************
*                       CStringDisplay::FindStringE
* Inputs:
*       int start: Starting position
*       LPCTSTR str: String to find
* Result: int
*       Position where it is found or CB_ERR
****************************************************************************/

int CStringDisplay::FindString(int start, LPCTSTR str)
    {
     SetWindowLong(m_hWnd, GWL_USERDATA, CB_FINDSTRING);
     return CComboBox::FindString(start, str);
    }

/////////////////////////////////////////////////////////////////////////////
// CStringDisplay message handlers

int CStringDisplay::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct) 
{
        // TODO: Add your code to determine the sorting order of the specified items
        // return -1 = item 1 sorts before item 2
        // return 0 = item 1 and item 2 sort the same
        // return 1 = item 1 sorts after item 2
        
        int kind = GetWindowLong(m_hWnd, GWL_USERDATA);
        CMyData * p;
        LPCSTR s1;
        LPCSTR s2;

        switch(kind)
           { /* what compare */
            case CB_FINDSTRING:
                    s1 = (LPCSTR)(lpCompareItemStruct->itemData1);
                    p = (CMyData *)(lpCompareItemStruct->itemData2);
                    s2 = p->getString();
                    return lstrcmpi(s1, s2); // fix up for leading substring
            case CB_FINDSTRINGEXACT:
                    s1 = (LPCSTR)(lpCompareItemStruct->itemData1);
                    p = (CMyData *)(lpCompareItemStruct->itemData2);
                    s2 = p->getString();
                    return lstrcmpi(s1, s2);
            case CB_ADDSTRING:
                    p = (CMyData *)(lpCompareItemStruct->itemData2);
                    s1 = p->getString();
                    p = (CMyData *)(lpCompareItemStruct->itemData2);
                    s2 = p->getString();
                    return lstrcmpi(s1, s2);
           } /* what compare */
        DebugBreak();
        return 0;
}

void CStringDisplay::DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct) 
{
        CComboBox::DeleteItem(lpDeleteItemStruct);
}

void CStringDisplay::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
   {
    CString s;
    CDC * dc = CDC::FromHandle(lpDrawItemStruct->hDC);
    SIZE sz;
    CRect r = lpDrawItemStruct->rcItem;

    int saved = dc->SaveDC();

    CBrush bk;
    CPen bkpen;

    if((LONG)lpDrawItemStruct->itemID < 0)
       { /* focus only */
        if(lpDrawItemStruct->itemState & ODS_FOCUS)
           { /* has focus */
            r.InflateRect(-1, -1);
            dc->DrawFocusRect(&r);
           } /* has focus */
        dc->RestoreDC(saved);
        return;
       } /* focus only */

    CMyData * p = GetItemData(lpDrawItemStruct->itemID);

    dc->SetBkMode(TRANSPARENT);

    if(lpDrawItemStruct->itemState & ODS_SELECTED)
       { /* selected */
        bk.CreateSolidBrush(::GetSysColor(COLOR_HIGHLIGHT));
        bkpen.CreatePen(PS_SOLID, 0, ::GetSysColor(COLOR_HIGHLIGHT));
        dc->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
       } /* selected */
    else
       { /* not selected */
        bk.CreateSolidBrush(::GetSysColor(COLOR_WINDOW));
        bkpen.CreatePen(PS_SOLID, 0, ::GetSysColor(COLOR_WINDOW));
        dc->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
       } /* not selected */

    dc->SelectObject(bk);
    dc->SelectObject(bkpen);
    dc->Rectangle(&r);

    s.Format(_T("%08x"), (LPARAM)p);
    dc->DrawText(s, &r, DT_LEFT);
    sz = dc->GetTextExtent(s);

    r.left += sz.cx;
    sz = dc->GetTextExtent(_T(" "), 1);
    r.left += 3 * sz.cx;
    dc->DrawText(p->getString(), lstrlen(p->getString()), &r, DT_LEFT);
    dc->RestoreDC(saved);
}

void CStringDisplay::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
 CClientDC dc(this);
 TEXTMETRIC tm;

 dc.GetTextMetrics(&tm);
 lpMeasureItemStruct->itemHeight = tm.tmHeight;
}
