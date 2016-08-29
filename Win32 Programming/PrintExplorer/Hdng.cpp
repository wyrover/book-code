// Hdng.cpp : implementation file
//

#include "stdafx.h"
#include "PrintExplorer.h"
#include "printit.h"
#include "Hdng.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHdng

CHdng::CHdng()
{
}

CHdng::~CHdng()
{
}


BEGIN_MESSAGE_MAP(CHdng, CComboBox)
        //{{AFX_MSG_MAP(CHdng)
        ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropdown)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHdng message handlers


/****************************************************************************
*                              CHdng::LoadFuncs
* Inputs:
*       LPHdngLoad tbl:
* Result: void
*       
* Effect: 
*       Loads the contents of the table into the combo box
****************************************************************************/

void CHdng::LoadFuncs(LPHdngLoad tbl)
    {
     for(int i = 0; tbl[i].id != 0; i++)
        { /* load table */
         CString s;
         s.LoadString(tbl[i].id);
         
         int index = AddString(s);
         SetItemData(index, (DWORD)tbl[i].func);
        } /* load table */
    }

/****************************************************************************
*                              CHdng::SelectFunc
* Inputs:
*       LPPrintitHeaderFunc func:
* Result: int
*       Index of entry that has the function pointer, or CB_ERR if not found
****************************************************************************/

int CHdng::SelectFunc(LPPrintitHeaderFunc func)
    {
     for(int i = 0; i < GetCount(); i++)
        if(GetItemData(i) == (DWORD)func)
           { /* select and return */
            SetCurSel(i);
            return i;
           } /* select and return */

     return CB_ERR;
    }


#define MAX_DROPDOWN_ITEMS 10

void CHdng::OnDropdown() 
{
 int n = GetCount();
 n = min(max(n, 2), MAX_DROPDOWN_ITEMS);
 int h = GetItemHeight(0);
 CRect r;

 GetWindowRect(&r);

 CSize sz;
 sz.cx = r.right - r.left;
 sz.cy = h * (n + 2);

 SetWindowPos(NULL, 0, 0, sz.cx, sz.cy, SWP_NOMOVE | SWP_NOZORDER);

        
}

/****************************************************************************
*                               CHdng::GetFunc
* Result: LPPrintitHeaderFunc
*       Pointer to selected header function, or NULL if none selected
****************************************************************************/

LPPrintitHeaderFunc CHdng::GetFunc()
    {
     int i = GetCurSel();
     if(i == CB_ERR)
        return NULL;
    
     return (LPPrintitHeaderFunc)GetItemData(i);
    }
