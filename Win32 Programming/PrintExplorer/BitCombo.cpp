// BitCombo.cpp : implementation file
//

#include "stdafx.h"
#include "BitCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBitCombo

CBitCombo::CBitCombo()
{
 deffmt = NULL;
}

CBitCombo::CBitCombo(LPBITCOMBO fmt)
{
 deffmt = fmt;
}
	    
CBitCombo::~CBitCombo()
{
}


BEGIN_MESSAGE_MAP(CBitCombo, CComboBox)
	//{{AFX_MSG_MAP(CBitCombo)
	ON_CONTROL_REFLECT(CBN_SELENDCANCEL, OnSelendcancel)
	ON_CONTROL_REFLECT(CBN_SELENDOK, OnSelendok)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropdown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBitCombo message handlers

/****************************************************************************
*                            CBitCombo::AddBits
* Inputs:
*       DWORD bits: A bit string
*	LPBITCOMBO fmt: A format table, or NULL to use the default format
*			table
* Result: BOOL
*       TRUE if successful
*	FALSE if error
* Effect: 
*       Adds all the bits in the string to the combo box using the
****************************************************************************/

BOOL CBitCombo::AddBits(DWORD bits, LPBITCOMBO fmt)
    {
     ResetContent();

     if(fmt == NULL)
	fmt = deffmt;
     if(fmt == NULL)
	return FALSE;
 
     CString b;
     b.Format(_T("0x%08X"), bits);
     AddString(b);

     for(int i = 0; fmt[i].id != 0; i++)
        { /* add each bit */
	 if(fmt[i].bit & bits)
	    { /* has bit */
	     CString s;
	     s.LoadString(fmt[i].id);
	     int index = AddString(s);
	     SetItemData(index, fmt[i].bit);
	    } /* has bit */
	} /* add each bit */
     SetCurSel(0);
     return TRUE;
    }

void CBitCombo::OnSelendcancel() 
{
 SetCurSel(0);
	
}

void CBitCombo::OnSelendok() 
{
 SetCurSel(0);
	
}

void CBitCombo::OnDropdown() 
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
