// InfoBox.cpp : implementation file
//

#include "stdafx.h"
#include "HeapExplorer.h"
#include "InfoBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MakeTag(i) (MAKELONG((i), -1))

/////////////////////////////////////////////////////////////////////////////
// CInfoBox

CInfoBox::CInfoBox()
{
}

CInfoBox::~CInfoBox()
{
}


BEGIN_MESSAGE_MAP(CInfoBox, CComboBox)
        //{{AFX_MSG_MAP(CInfoBox)
        ON_CBN_DROPDOWN(IDC_INFO, OnDropdownInfo)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInfoBox message handlers

void CInfoBox::OnDropdownInfo() 
{
 int count = GetCount();
 int sel = GetCurSel();
 int n = 0;

 if(sel == 0)
    { /* heading line */
     n = 10; // TODO: make this more meaningful
    } /* heading line */
 else
    { /* detail line */
     DWORD itemdata = GetItemData(sel);

     for(int i = sel; i < n; i++)
        { /* scan for same */
         if(GetItemData(i) == itemdata)
            n++;
         else
            break;
        } /* scan for same */
    } /* detail line */
     

 n = max(n, 2);
 
 int h = GetItemHeight(0);  // get default item height
 CRect r;
 GetWindowRect(&r);

 CSize sz;
 sz.cx = r.right - r.left;
 sz.cy = h * (n + 2);

 SetWindowPos(NULL, 0, 0, sz.cx, sz.cy, SWP_NOMOVE | SWP_NOZORDER);
        
}

/****************************************************************************
*                             CInfoBox::AddString
* Inputs:
*       LPCTSTR s: String to add
*       int i: Index of string info
* Result: int
*       Position where it was added
* Effect: 
*       Adds the string and puts MAKELONG(i, -1) in as its ItemData
****************************************************************************/

int CInfoBox::AddString(LPCTSTR s, int i)
    {
     int index = CComboBox::AddString(s);
     if(index != CB_ERR)
        { /* add itemdata */
         SetItemData(index, MakeTag(i));
        } /* add itemdata */
     return index;
    }

/****************************************************************************
*                             CInfoBox::FindItem
* Inputs:
*       int id: ID of item to find
* Result: int
*       ID of first line of selection, or CB_ERR if not found
****************************************************************************/

int CInfoBox::FindItem(int id)
    {
     int count = GetCount();
     for(int i = 0; i < count; i++)
        { /* check it */
         if((DWORD)GetItemData(i) == (DWORD)MakeTag(id))
            return i;
        } /* check it */
     return CB_ERR;
    }
