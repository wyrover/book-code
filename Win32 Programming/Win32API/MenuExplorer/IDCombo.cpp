// IDCombo.cpp : implementation file
//

#include "stdafx.h"
#include "IDCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIDCombo

CIDCombo::CIDCombo()
{
 maxlen = 0;  // no limit
}

CIDCombo::~CIDCombo()
{
}


BEGIN_MESSAGE_MAP(CIDCombo, CComboBox)
        //{{AFX_MSG_MAP(CIDCombo)
        ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropdown)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIDCombo message handlers

/****************************************************************************
*                              CIDCombo::Select
* Inputs:
*       DWORD itemval: ItemData value
* Result: int
*       The index of the selected value, or CB_ERR if no selection possible
* Effect: 
*       If the value exists in the list, selects it, otherwise selects -1
****************************************************************************/

int CIDCombo::Select(DWORD itemval)
    {
     for(int i = 0; i < GetCount(); i++)
        { /* select */
         DWORD data = GetItemData(i);
         if(itemval == data)
            { /* found it */
             SetCurSel(i);
             return i;
            } /* found it */
        } /* select */

     SetCurSel(-1);
     return CB_ERR;
    }

/****************************************************************************
*                             CIDCombo::AddString
* Inputs:
*       IDData * data: Data to add
* Result: int
*       Index of insertion, or CB_ERR
* Effect: 
*       Adds the string designated by the ID to the combo box, and adds its
*       value to the itemdata
****************************************************************************/

int CIDCombo::AddString(IDData * data)
    {
     CString s;
     if(s.LoadString(data->id) == 0)
        return CB_ERR;

     int i = CComboBox::AddString(s);
     if(i == CB_ERR)
        return CB_ERR;
     CComboBox::SetItemData(i, data->val);
     return i;
    }

/****************************************************************************
*                            CIDCombo::AddStrings
* Inputs:
*       IDData * data: Pointer to data table
*       int def: ID of default control, or 0 if no default control is to be
*                selected.  
* Result: int
*       index of object selected by 'def'
*       CB_ERR if 'def' object not in list, or error loading, or def == 0
* Effect: 
*       Adds the strings in the table to the combo box.  The table is 
*       terminated by an entry whose id field is 0
* Notes:
*       To select value by itemdata, use the explicit Select operation
****************************************************************************/

int CIDCombo::AddStrings(IDData * data, int def)
    {
     BOOL found = FALSE;
     CString s;

     for(int i = 0; data[i].id != 0; i++)
        { /* add it */
         int index = AddString(&data[i]);
         if(index == CB_ERR)
           return CB_ERR;
         if(def != 0 && data[i].id == def)
            { /* has selection */
             GetLBText(index, s);
             found = TRUE;
            } /* has selection */
        } /* add it */
     if(found)
        { /* locate selection */
         int index = FindStringExact(-1, s);
         if(index == CB_ERR)
            return CB_ERR;
         return index;
        } /* locate selection */

     return CB_ERR; // no selection matched
    }

/****************************************************************************
*                            CIDCombo::OnDropdown
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

void CIDCombo::OnDropdown() 
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
*                            CIDCombo::GetCurItem
* Result: DWORD
*       The itemdata of the current selection, or 0 if there is no selection
****************************************************************************/

DWORD CIDCombo::GetCurItem()
    {
     int i = GetCurSel();
     if(i == CB_ERR)
        return 0;
     return GetItemData(i);
    }
