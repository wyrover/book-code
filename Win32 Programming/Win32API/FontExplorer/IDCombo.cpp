// IDCombo.cpp : implementation file
//

#include "stdafx.h"
#include "FontExplorer.h"
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
*       int def: ID of default control
* Result: BOOL
*       TRUE if successful
*       FALSE if any error
* Effect: 
*       Adds the strings in the table to the combo box.  The table is 
*       terminated by an entry whose id field is 0
****************************************************************************/

BOOL CIDCombo::AddStrings(IDData * data, int def)
    {
     for(int i = 0; data[i].id != 0; i++)
        { /* add it */
         int index = AddString(&data[i]);
         if(index == CB_ERR)
           return FALSE;
         if(def != 0 && data[i].id == def)
            SetCurSel(index);
        } /* add it */
     return TRUE;
    }

void CIDCombo::OnDropdown() 
{
 int n = GetCount();
 n = max(n, 2);

 int ht = GetItemHeight(0);
 CRect r;
 GetWindowRect(&r);

 CSize sz;
 sz.cx = r.Width();
 sz.cy = ht * (n + 2);

 SetWindowPos(NULL, 0, 0, sz.cx, sz.cy, SWP_NOMOVE | SWP_NOZORDER);
}
