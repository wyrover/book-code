// TreeGetItem.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "NumericEdit.h"
#include "msglog.h"
#include "ixable.h"
#include "loggable.h"
#include "treeOps.h"
#include "tvitem.h"
#include "TreeGetItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DIM(x) (sizeof(x) / sizeof((x)[0]))

/////////////////////////////////////////////////////////////////////////////
// CTreeGetItem property page

IMPLEMENT_DYNCREATE(CTreeGetItem, CTV_ITEM)

CTreeGetItem::CTreeGetItem() : CTV_ITEM(CTreeGetItem::IDD)
{
        //{{AFX_DATA_INIT(CTreeGetItem)
        //}}AFX_DATA_INIT
}

CTreeGetItem::~CTreeGetItem()
{
}

void CTreeGetItem::DoDataExchange(CDataExchange* pDX)
{
        CTV_ITEM::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CTreeGetItem)
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTreeGetItem, CTV_ITEM)
        //{{AFX_MSG_MAP(CTreeGetItem)
        ON_BN_CLICKED(IDC_GETITEM, OnGetitem)
        ON_BN_CLICKED(IDC_SETITEM, OnSetitem)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeGetItem message handlers

void CTreeGetItem::OnGetitem() 
{
 item.pszText = text;
 item.cchTextMax = DIM(text);
 controlsToItem(&item);
 BOOL result = c_Tree->SendMessage(TVM_GETITEM, 0, (LPARAM)&item);
 itemToControls(&item);
 enableControls();
 showResult_N_i_b(IDS_TVM_GETITEM, result);
}

void CTreeGetItem::OnSetitem() 
{
 item.pszText = text;
 item.cchTextMax = DIM(text);
 controlsToItem(&item);
 int result = c_Tree->SendMessage(TVM_SETITEM, 0, (LPARAM)&item);
 showResult_N_i_d(IDS_TVM_SETITEM, result);
 itemToControls(&item);
 enableControls();
}

BOOL CTreeGetItem::OnSetActive() 
{
        loadComboFromTree(&c_hItem);
        enableControls();
        
        return CTV_ITEM::OnSetActive();
}

