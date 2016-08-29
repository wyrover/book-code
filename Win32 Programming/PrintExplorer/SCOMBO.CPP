// scombo.cpp : implementation file
//

#include "stdafx.h"
#include "PrintExplorer.h"
#include "scombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSizeCombo

CSizeCombo::CSizeCombo()
{
}

CSizeCombo::~CSizeCombo()
{
}


BEGIN_MESSAGE_MAP(CSizeCombo, CComboBox)
        //{{AFX_MSG_MAP(CSizeCombo)
        ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropdown)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSizeCombo message handlers

void CSizeCombo::OnDropdown() 
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
