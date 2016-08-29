// EditTabs.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "numlist.h"
#include "msglog.h"
#include "IntEdit.h"
#include "TinyButt.h"
#include "HexDisp.h"
#include "ixable.h"
#include "loggable.h"
#include "editops.h"
#include "EditTabs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditTabs property page

IMPLEMENT_DYNCREATE(CEditTabs, CEditOps)

CEditTabs::CEditTabs() : CEditOps(CEditTabs::IDD)
{
        //{{AFX_DATA_INIT(CEditTabs)
        //}}AFX_DATA_INIT
}

CEditTabs::~CEditTabs()
{
}

void CEditTabs::DoDataExchange(CDataExchange* pDX)
{
        CEditOps::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CEditTabs)
        DDX_Control(pDX, IDC_SPINTAB, c_SpinTab);
        DDX_Control(pDX, IDC_REMOVE, c_Remove);
        DDX_Control(pDX, IDC_TABSTOPS, c_TabStops);
        DDX_Control(pDX, IDC_TABVAL, c_TabVal);
        DDX_Control(pDX, IDC_TABS, c_Tabs);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditTabs, CEditOps)
        //{{AFX_MSG_MAP(CEditTabs)
        ON_BN_CLICKED(IDC_ADD, OnAdd)
        ON_BN_CLICKED(IDC_REMOVE, OnRemove)
        ON_BN_CLICKED(IDC_TABSTOPS, OnTabstops)
        ON_LBN_SELCHANGE(IDC_TABS, OnSelchangeTabs)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditTabs message handlers

/****************************************************************************
*                          CEditTabs::enableControls
* Result: void
*       
* Effect: 
*       Enables the buttons
****************************************************************************/

void CEditTabs::enableControls()
    {
     c_Remove.EnableWindow(c_Tabs.GetCount() != 0 && 
                                c_Tabs.GetCurSel() != LB_ERR);
    }


void CEditTabs::OnAdd() 
{
 // The following code was supposed to prevent duplicates from being
 // added but it always returns 0, not LB_ERR.  This seems unusual behavior
 // for an owner-draw listbox without LBS_HASSTRINGS, given our implementation
 // of OnCompareItem.

 //if(c_Tabs.FindString(-1, c_TabVal.GetVal()) != LB_ERR)
 //   return; // tab already in table

 for(int i = 0; i < c_Tabs.GetCount(); i++)
    if((int)c_Tabs.GetItemData(i) == c_TabVal.GetVal())
       return;

 c_Tabs.AddString(c_TabVal.GetVal());
 enableControls();
}

void CEditTabs::OnRemove() 
{
 c_Tabs.DeleteString(c_Tabs.GetCurSel());
 enableControls();
        
}

BOOL CEditTabs::OnInitDialog() 
{
        CEditOps::OnInitDialog();
        
        CRect r;
        
        c_Tabs.GetWindowRect(&r);

        c_SpinTab.SetRange(0, 2 * r.Width());
        enableControls();
        
        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditTabs::OnTabstops() 
{
 int count = c_Tabs.GetCount();
 CString tablist;
 CString sep;

 if(count > 0)
    { /* has tabs */
     int * tabs = new int[count];

     for(int i = 0; i < count; i++)
        { /* get tabs */
         tabs[i] = c_Tabs.GetItemData(i);
        } /* get tabs */

     edit->SendMessage(EM_SETTABSTOPS, count, (LPARAM)tabs);
     
     tablist = _T("");
     sep = _T("");

     CString t;
     for(i = 0; i < count; i++)
        { /* add to list */
         t.Format(_T("%s%d"), sep, tabs[i]);
         tablist += t;
         sep = _T(", ");
        } /* add to list */

     delete tabs;
     tablist = _T("{") + tablist + _T("}");
    } /* has tabs */
 else
    { /* no tabs */
     tablist = _T("NULL");
    } /* no tabs */

 CString t;
 t.LoadString(IDS_EM_SETTABSTOPS);

 CString s;
 s.Format(_T("%s(%d, %s)"), t, count, tablist);
 addMessage(s);
}

void CEditTabs::OnSelchangeTabs() 
{
 enableControls();
        
}
