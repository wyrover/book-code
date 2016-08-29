// TreeIns.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"

#include "NumericEdit.h"
#include "TinyButt.h"
#include "msglog.h"
#include "ixable.h"
#include "loggable.h"
#include "treeOps.h"
#include "tvitem.h"

#include "TreeIns.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DIM(x) (sizeof(x) / sizeof((x)[0]))

/////////////////////////////////////////////////////////////////////////////
// CInsertItem property page

IMPLEMENT_DYNCREATE(CInsertItem, CTV_ITEM)

CInsertItem::CInsertItem() : CTV_ITEM(CInsertItem::IDD)
{
        //{{AFX_DATA_INIT(CInsertItem)
        //}}AFX_DATA_INIT
}

CInsertItem::~CInsertItem()
{
}

void CInsertItem::DoDataExchange(CDataExchange* pDX)
{
        CTV_ITEM::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CInsertItem)
        DDX_Control(pDX, IDC_PARENT, c_hParent);
        DDX_Control(pDX, IDC_INSERT_AFTER, c_hInsertAfter);
        DDX_Control(pDX, IDC_TVI_SORT, c_TVI_SORT);
        DDX_Control(pDX, IDC_TVI_LAST, c_TVI_LAST);
        DDX_Control(pDX, IDC_TVI_FIRST, c_TVI_FIRST);
        DDX_Control(pDX, IDC_INSERTHANDLE, c_InsertHandle);
        DDX_Control(pDX, IDC_INSERTITEM, c_InsertItem);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInsertItem, CTV_ITEM)
        //{{AFX_MSG_MAP(CInsertItem)
        ON_BN_CLICKED(IDC_TVI_FIRST, OnTviFirst)
        ON_BN_CLICKED(IDC_TVI_LAST, OnTviLast)
        ON_BN_CLICKED(IDC_TVI_SORT, OnTviSort)
        ON_BN_CLICKED(IDC_INSERTHANDLE, OnInserthandle)
        ON_BN_CLICKED(IDC_INSERTITEM, OnInsertitem)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInsertItem message handlers

void CInsertItem::OnTviFirst() 
{
        // TODO: Add your control notification handler code here
        
}

void CInsertItem::OnTviLast() 
{
        // TODO: Add your control notification handler code here
        
}

void CInsertItem::OnTviSort() 
{
        // TODO: Add your control notification handler code here
        
}

BOOL CInsertItem::OnInitDialog() 
{
        CTV_ITEM::OnInitDialog();
        
        // TODO: Add extra initialization here
        
        CheckRadioButton(IDC_INSERTHANDLE, IDC_TVI_SORT, IDC_TVI_LAST);
        enableControls();

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CInsertItem::OnInserthandle() 
{
        // TODO: Add your control notification handler code here
        
}

/****************************************************************************
*                      void CInsertItem::enableControls
* Result: void
*       
* Effect: 
*       Enables all controls of the TV_ITEM, then selectively disables
*       some
****************************************************************************/

void CInsertItem::enableControls()
    {
     CTV_ITEM::enableControls();

     c_TVIF_CHILDREN.EnableWindow(FALSE);  // can't set number of children
     c_TVIF_HANDLE.EnableWindow(FALSE);    // can't set handle
     BOOL enable = (c_InsertHandle.GetCheck() == BST_CHECKED);

     c_InsertHandle.EnableWindow(c_hInsertAfter.GetCount() > 0);
     c_hInsertAfter.EnableWindow(c_hInsertAfter.GetCount() > 0);
    }


void CInsertItem::loadParent()
{
 loadComboFromTree(&c_hParent, TRUE);
}


BOOL CInsertItem::OnSetActive() 
{
        loadParent();
        
        enableControls();

        return CTV_ITEM::OnSetActive();
}

void CInsertItem::OnInsertitem() 
{
 TV_INSERTSTRUCT is;

 controlsToItem();  // copy control values to static item (we want them
                    // set in the shared copy!)

 is.item = item;    // copy control values from item to insert struct
 int index;
 index = c_hParent.GetCurSel();

 if(index == CB_ERR)
    is.hParent = NULL;
 else
    is.hParent = (HTREEITEM)c_hParent.GetItemData(index);

 if(c_TVI_FIRST.GetCheck() == BST_CHECKED)
    { /* first */
     is.hInsertAfter = TVI_FIRST;
    } /* first */
 else
 if(c_TVI_LAST.GetCheck() == BST_CHECKED)
    { /* last */
     is.hInsertAfter = TVI_LAST;
    } /* last */
 else
 if(c_TVI_SORT.GetCheck() == BST_CHECKED)
    { /* sort */
     is.hInsertAfter = TVI_SORT;
    } /* sort */
 else
 if(c_InsertHandle.GetCheck() == BST_CHECKED)
    { /* use specific handle */
     // TODO: write this...
    } /* use specific handle */

 HTREEITEM result = (HTREEITEM)c_Tree->SendMessage(TVM_INSERTITEM, 0, (LPARAM)&is);
 // We've now changed the structure

 CString resstr;
 if(result == NULL)
    resstr.LoadString(IDS_NULL);
 else
    resstr.Format(_T("%08x"), result);

 CString s = formatInsertItem(c_Tree, &is);
 showResult_N_s_s(IDS_TVM_INSERTITEM, s, resstr);
 loadParent();
}

/****************************************************************************
*                        CINsertItem::formatInsertItem
* Inputs:
*       TV_INSERTSTRUCT * is:
*       
* Result: CString
*       TV_INSERTSTRUCT, formatted as desired
****************************************************************************/

CString CInsertItem::formatInsertItem(CTreeCtrl * c_Tree, TV_INSERTSTRUCT * is)
    {
     // TVM_INSERTITEM(0, { %08x, %s, &i}) => %08x
     //                   \_____________/
     CString insafter;

     switch((int)is->hInsertAfter)
        { /* insafter */
         case (int)TVI_FIRST:
                 insafter.LoadString(IDS_TVI_FIRST);
                 break;
         case (int)TVI_LAST:
                 insafter.LoadString(IDS_TVI_LAST);
                 break;
         case (int)TVI_SORT:
                 insafter.LoadString(IDS_TVI_SORT);
                 break;
         case (int)NULL:
                 insafter.LoadString(IDS_NULL);
                 break;
         default:
                 {
                  TV_ITEM item;
                  TCHAR text[256];
                  item.hItem = is->hInsertAfter;
                  item.mask = TVIF_TEXT;
                  item.pszText = text;
                  item.cchTextMax = DIM(text);
                  c_Tree->GetItem(&item);
                  insafter.Format(_T("%08x \"%s\""), is->hInsertAfter, text);
                 }
                 break;
        } /* insafter */

     CString parentstr;
     if(is->hParent == NULL)
        parentstr.LoadString(IDS_NULL);
     else
     if(is->hParent == TVI_ROOT)
        parentstr.LoadString(IDS_TVI_ROOT);
     else
        { /* real parent */
         TV_ITEM item;
         TCHAR text[256];
         item.hItem = is->hParent;
         item.mask = TVIF_TEXT;
         item.pszText = text;
         item.cchTextMax = DIM(text);
         c_Tree->GetItem(&item);
         parentstr.Format(_T("%08x \"%s\""), is->hParent, text);
        } /* real parent */

     CString s;
     s.Format(_T("{%s, %s, &i}"),  parentstr, insafter);

     return s;
    }
