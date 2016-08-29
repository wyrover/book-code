// Pdata.cpp : implementation file
//

#include "stdafx.h"
#include "PrintExplorer.h"
#include "Pdata.h"

#include "enumedit.h"
#include "paper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define UWM_RECOMPUTE_WIDTHS (WM_USER + 100)

/////////////////////////////////////////////////////////////////////////////
// CPaperData

CPaperData::CPaperData()
{
     memset(tabs, 0, sizeof(tabs));
     height = 0;
}

CPaperData::~CPaperData()
{
}


BEGIN_MESSAGE_MAP(CPaperData, CComboBox)
        //{{AFX_MSG_MAP(CPaperData)
        ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropdown)
        ON_MESSAGE(UWM_RECOMPUTE_WIDTHS, OnRecomputeWidths)
        ON_WM_CREATE()
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPaperData message handlers

void CPaperData::DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct) 
{
 CComboBox::DeleteItem(lpDeleteItemStruct);

 delete (LPPaperInfo)lpDeleteItemStruct->itemData;

 PostMessage(UWM_RECOMPUTE_WIDTHS);
        
}

void CPaperData::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
 LPPaperInfo pi = (LPPaperInfo)lpDrawItemStruct->itemData;
 CDC * dc = CDC::FromHandle(lpDrawItemStruct->hDC);
 CPen bar;
 CBrush bkg;

 if(pi == (LPPaperInfo)(-1))
    return;  // do not try to draw with bogus value

 int saved = dc->SaveDC();

 dc->SetBkMode(TRANSPARENT);

 if(lpDrawItemStruct->itemState & ODS_SELECTED)
    { /* selected */
     bar.CreatePen(PS_SOLID, 0, ::GetSysColor(COLOR_HIGHLIGHTTEXT));
     dc->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
     dc->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
     bkg.CreateSolidBrush(::GetSysColor(COLOR_HIGHLIGHT));
    } /* selected */
 else
    { /* unselected */
     bar.CreatePen(PS_SOLID, 0, ::GetSysColor(COLOR_WINDOWTEXT));
     dc->SetBkColor(::GetSysColor(COLOR_WINDOW));
     dc->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
     bkg.CreateSolidBrush(::GetSysColor(COLOR_WINDOW));
    } /* unselected */

 dc->SelectStockObject(NULL_PEN);
 dc->SelectObject(bkg);
 dc->Rectangle(&lpDrawItemStruct->rcItem);

 dc->SelectObject(&bar);

 DrawLine(dc, pi, lpDrawItemStruct);

 dc->RestoreDC(saved);

 CRect rc = lpDrawItemStruct->rcItem;
 rc.InflateRect(-1, -1);
 
 if(lpDrawItemStruct->itemState & ODS_FOCUS && lpDrawItemStruct->itemAction != ODA_SELECT)
    dc->DrawFocusRect(&rc);

}

/****************************************************************************
*                            CPaperData::DrawLine
* Inputs:
*       CDC * dc: A DC all set up for drawing; the current pen, background
*                 and text colors are all set up
*       LPPaperInfo pi: The LPPaperInfo structure
*       LPDRAWITEMSTRUCT lpDrawItemStruct
* Result: void
*       
* Effect: 
*       Does nothing.  This is overridden in derived classes to actually
*       draw
****************************************************************************/

void CPaperData::DrawLine(CDC * dc, LPPaperInfo pi, LPDRAWITEMSTRUCT lpDrawItemStruct)
    {
     // does nothing; override
    }


void CPaperData::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
 CClientDC dc(this);
 TEXTMETRIC tm;
 dc.GetTextMetrics(&tm);
 lpMeasureItemStruct->itemHeight = (height == 0 
                        ? tm.tmHeight - tm.tmInternalLeading 
                        : height);
        
}

void CPaperData::OnDropdown() 
{
 int n = GetCount();
 n = max(n, 2);
 
 int h = GetItemHeight(0);  // get default item height
 CRect r;
 GetWindowRect(&r);

 CSize sz;
 sz.cx = r.right - r.left;
 sz.cy = h * (n + 2);

 // TODO: limit height to max (bottom-of-control-to-top-of-screen,
 //                             top-of-control-to-bottom-of-screen)

 SetWindowPos(NULL, 0, 0, sz.cx, sz.cy, SWP_NOMOVE | SWP_NOZORDER);
        
}

/****************************************************************************
*                        CPaperData::recompute_widths
* Result: void
*       
* Effect: 
*       Runs through the table computing the widths for proper layout
****************************************************************************/

void CPaperData::recompute_widths()
    {
     memset(tabs, 0, sizeof(tabs));
    }

/****************************************************************************
*                           CPaperData::GetItemData
* Inputs:
*       int index: Index of 
* Result: LPPaperInfo
*       The paper info, or NULL if index is bad
****************************************************************************/

LPPaperInfo CPaperData::GetItemData(int index)
    {
     LPPaperInfo pi = (LPPaperInfo)CComboBox::GetItemData(index);
     if(pi == (LPPaperInfo)CB_ERR)
        return NULL;
     return pi;
    }

/****************************************************************************
*                          CPaperData::AddString
* Inputs:
*       LPPaperInfo pi: PaperInfo item to add
* Result: void
*       
* Effect: 
*       Adds the item, and computes the new tab stops
****************************************************************************/

int CPaperData::AddString(LPPaperInfo pi)
    {
     int result = CComboBox::AddString((LPCTSTR)pi);
     if(result == CB_ERR)
        return result;

     recompute_widths();
     return result;
    }

/****************************************************************************
*                        CPaperData::OnRecomputeWidths
* Inputs:
*       WPARAM: unused
*       LPARAM: unused
* Result: LRESULT
*       0, always
* Effect: 
*       Recomputes the widths
****************************************************************************/

LRESULT CPaperData::OnRecomputeWidths(WPARAM, LPARAM)
    {
     recompute_widths();
     return 0;
    }

/****************************************************************************
*                            PaperInfo::PaperInfo
* Effect: 
*       Constructor
****************************************************************************/

PaperInfo::PaperInfo()
    {
     id = 0;
    }

PaperInfo::~PaperInfo()
    {
    }

int CPaperData::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
 RECT r;
        if (CComboBox::OnCreate(lpCreateStruct) == -1)
                return -1;
        
        GetWindowRect(&r);
        // height = r.bottom - r.top;
        
        return 0;
}

/****************************************************************************
*                                 findPaperID
* Inputs:
*       LPPaperInfo pi: PaperInfo object
* Result: void
*       
* Effect: 
*       Sets the pi->id field if it needs to be set
****************************************************************************/

void CPaperData::findPaperID(LPPaperInfo pi)
    {
     if(pi->id == 0)
        { /* need to find string */
         for(int n = 0; t_dmPaperSize[n].id != 0; n++)
            { /* locate print name */
             if(t_dmPaperSize[n].val == pi->paper)
                { /* found it */
                 pi->id = t_dmPaperSize[n].id;
                 break;
                } /* found it */
            } /* locate print name */
        } /* need to find string */
     
    }

/****************************************************************************
*                     CPaperData::showPreferredRectangle
* Inputs:
*       CDC * dc: Display context
*       LPRECT r: Rectangle to highlight
* Result: void
*       
* Effect: 
*       Highlights the rectangle in the preferred column color
****************************************************************************/

void CPaperData::showPreferredRectangle(CDC * dc, LPRECT r)
    {
     int saved = dc->SaveDC();
     CBrush br(RGB(255, 255, 192));
     dc->SelectObject(br);
     dc->SelectStockObject(NULL_PEN);
     dc->Rectangle(r);
     dc->RestoreDC(saved);
    }
