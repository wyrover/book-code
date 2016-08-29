// MenuComb.cpp : implementation file
//

#include "stdafx.h"
#include "menuitem.h"
#include "MenuComb.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenuCombo

CMenuCombo::CMenuCombo()
{
 maxlen = 0;  // no limit
}

CMenuCombo::~CMenuCombo()
{
}


BEGIN_MESSAGE_MAP(CMenuCombo, CComboBox)
        //{{AFX_MSG_MAP(CMenuCombo)
        ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropdown)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuCombo message handlers

/****************************************************************************
*                              CMenuCombo::FindItem
* Inputs:
*       HMENU menu: Menu to find
* Result: int
*       Index of menu item, or CB_ERR if not found
****************************************************************************/

int CMenuCombo::FindItem(HMENU menu)
    {
     for(int i = 0; i < GetCount(); i++)
        { /* select */
         menuitem * item = (menuitem *)GetItemData(i);
         if(item == NULL)
            continue;
         if(item->hmenu == menu)
            { /* found it */
             return i;
            } /* found it */
        } /* select */

     return CB_ERR;
     
    }

/****************************************************************************
*                              CMenuCombo::Select
* Inputs:
*       HMENU menu: ItemData value
* Result: int
*       The index of the selected value, or CB_ERR if no selection possible
* Effect: 
*       If the value exists in the list, selects it, otherwise selects -1
****************************************************************************/

int CMenuCombo::Select(HMENU menu)
    {
     int found = FindItem(menu);
     SetCurSel(found);
     return found;
    }

/****************************************************************************
*                             CMenuCombo::AddString
* Inputs:
*       HMENU menu: menu item to add
*       DWORD flags: Flags to associate with it
*       CString * name: Name of menu, or NULL permitted if no name
* Result: int
*       Index of insertion, or CB_ERR
* Effect: 
*       Adds the string designated by the ID to the combo box, and adds its
*       value to the itemdata
****************************************************************************/

int CMenuCombo::AddString(HMENU menu, DWORD flags, CString * name)
    {
     menuitem * item = new menuitem;

     if(item == NULL)
        return CB_ERR;

     item->hmenu = menu;
     item->flags = flags;
     if(name != NULL)
        item->name = *name;

     return CComboBox::AddString((LPCTSTR)item);
    }

/****************************************************************************
*                            CMenuCombo::OnDropdown
* Result: void
*       
* Effect: 
*       Sizes the combo to a minimum size required to show everything
* Notes:
*       maxlen determines the maximum length of the combo dropdown
*               0 - no limit, sized so there is no scrollbar
*              >0 - limit to maxlen items, but at least 2
*              -1 - implicitly limit to screen size (NYI)
****************************************************************************/

void CMenuCombo::OnDropdown() 
{
 int n = GetCount();
 n = max(n, 2);

 int ht = GetItemHeight(0);
 CRect r;
 GetWindowRect(&r);

 if(maxlen > 0)
    n = max(maxlen, 2);

 CSize sz;
 sz.cx = r.Width();
 sz.cy = ht * (n + 2);

 if(maxlen < 0)
    { /* screen limit */
     if(r.top - sz.cy < 0 || r.bottom + sz.cy > ::GetSystemMetrics(SM_CYSCREEN))
        { /* invoke limit */
         // Compute the largest distance the dropdown can appear, 
         // relative to the screen (not the window!)

         int k = max( (r.top / ht), 
                      (::GetSystemMetrics(SM_CYSCREEN) - r.bottom) / ht);

         // compute new space. Note that we don't really fill the screen.
         // We only have to use this size if it is smaller than the max size
         // actually required
         int ht2 = ht * k;
         sz.cy = min(ht2, sz.cy);
        } /* invoke limit */
    } /* screen limit */

 SetWindowPos(NULL, 0, 0, sz.cx, sz.cy, SWP_NOMOVE | SWP_NOZORDER);
}

/****************************************************************************
*                            CMenuCombo::GetCurItem
* Result: menuitem *
*       The itemdata of the current selection, or NULL if there is no selection
****************************************************************************/

menuitem * CMenuCombo::GetCurItem()
    {
     int i = GetCurSel();
     if(i == CB_ERR)
        return NULL;
     return (menuitem *)GetItemData(i);
    }

void CMenuCombo::DeleteItem(LPDELETEITEMSTRUCT dis) 
{
        delete (menuitem *)dis->itemData;
}

void CMenuCombo::DrawItem(LPDRAWITEMSTRUCT dis) 
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
     menuitem * mi = (menuitem *)(dis->itemData);

     CString s;
     s.Format(_T("0x%08x %s"), mi->hmenu, mi->name);

     
     // Since we don't know the actual text size, and TextOut will 
     // write anywhere it wants to (including over top of the dropdown
     // button, at least on NT3.51), we force a clipping region.

     int tsaved = dc->SaveDC();
     CRgn hrgn;

     hrgn.CreateRectRgnIndirect(&dis->rcItem);
     dc->SelectClipRgn(&hrgn);
     
     dc->TextOut(r.left, r.top, s);

     dc->RestoreDC(tsaved);
    } /* has item */
 
 if(dis->itemState & ODS_FOCUS && dis->itemAction != ODA_SELECT)
    dc->DrawFocusRect(&r);

 dc->RestoreDC(saved);
        
}

void CMenuCombo::MeasureItem(LPMEASUREITEMSTRUCT mis) 
{
 // do nothing for now, default is OK
        
}

int CMenuCombo::CompareItem(LPCOMPAREITEMSTRUCT cis) 
{
        // return -1 = item 1 sorts before item 2
        // return 0 = item 1 and item 2 sort the same
        // return 1 = item 1 sorts after item 2
        
        return ((menuitem *)cis->itemData1)->hmenu - 
               ((menuitem *)cis->itemData2)->hmenu;
}

/****************************************************************************
*                           CMenuCombo::DeleteItem
* Inputs:
*       HMENU menu: menu item to delete
* Result: void
*       
* Effect: 
*       Deletes the menu whose itemdata matches 'menu' from the list
****************************************************************************/

void CMenuCombo::DeleteItem(HMENU menu)
    {
     int found = FindItem(menu);
     if(found == CB_ERR)
        return;
     DeleteString(found);
    }
