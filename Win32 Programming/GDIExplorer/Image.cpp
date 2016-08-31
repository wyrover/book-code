// IDCombo.cpp : implementation file
//

#include "stdafx.h"
#include "image.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageCombo

CImageCombo::CImageCombo()
{
 maxlen = 0;  // no limit
}

CImageCombo::~CImageCombo()
{
}


BEGIN_MESSAGE_MAP(CImageCombo, CComboBox)
        //{{AFX_MSG_MAP(CImageCombo)
        ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropdown)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageCombo message handlers

/****************************************************************************
*                              CImageCombo::Select
* Inputs:
*       DWORD itemval: ItemData value
* Result: int
*       The index of the selected value, or CB_ERR if no selection possible
* Effect: 
*       If the value exists in the list, selects it, otherwise selects -1
****************************************************************************/

int CImageCombo::Select(DWORD itemval)
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
*                             CImageCombo::AddString
* Inputs:
*       int data: Data to add
* Result: int
*       Index of insertion, or CB_ERR
* Effect: 
*       Adds the string designated by the ID to the combo box, and adds its
*       value to the itemdata
****************************************************************************/

int CImageCombo::AddString(int data)
    {
     int i = CComboBox::AddString((LPSTR)data);
     if(i == CB_ERR)
        return CB_ERR;
     return i;
    }

/****************************************************************************
*                            CImageCombo::AddStrings
* Inputs:
*       LPINT data: Pointer to data table
*       int def: ID of default control
* Result: BOOL
*       TRUE if successful
*       FALSE if any error
* Effect: 
*       Adds the strings in the table to the combo box.  The table is 
*       terminated by an entry whose id field is 0
****************************************************************************/

BOOL CImageCombo::AddStrings(LPINT data, int def)
    {
     for(int i = 0; data[i] != 0; i++)
        { /* add it */
         int index = AddString(data[i]);
         if(index == CB_ERR)
           return FALSE;
         if(def != 0 && data[i] == def)
            SetCurSel(index);
        } /* add it */
     return TRUE;
    }

/****************************************************************************
*                            CImageCombo::OnDropdown
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

void CImageCombo::OnDropdown() 
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
         sz.cy = ht * k;
        } /* invoke limit */
    } /* screen limit */

 SetWindowPos(NULL, 0, 0, sz.cx, sz.cy, SWP_NOMOVE | SWP_NOZORDER);
}

/****************************************************************************
*                            CImageCombo::GetCurItem
* Result: DWORD
*       The itemdata of the current selection, or 0 if there is no selection
****************************************************************************/

DWORD CImageCombo::GetCurItem()
    {
     int i = GetCurSel();
     if(i == CB_ERR)
        return 0;
     return GetItemData(i);
    }
