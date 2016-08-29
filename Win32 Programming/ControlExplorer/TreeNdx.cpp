// Listndx.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "ixable.h"
#include "IndexPg.h"
#include "TreeNdx.h"

#include "tinybutt.h"
#include "IntEdit.h"
#include "HexDisp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static ixlist entries[] = {
    {IDS_TVM_DELETEITEM,    IDC_DELETEITEM,     IDD_TREEMISC},
    {IDS_TVM_EDITLABEL,     IDC_EDITLABEL,      IDD_TREEMISC},
    {IDS_TVM_ENDEDITLABELNOW, IDC_ENDEDITLABELNOW, IDD_TREEMISC},
    {IDS_TVM_GETCOUNT,      IDC_GETCOUNT,       IDD_TREEMISC},
    {IDS_TVM_GETINDENT,     IDC_GETINDENT,      IDD_TREEMISC},
    {IDS_TVM_GETITEM,       IDC_GETITEM,        IDD_TREEGETITEM},
    {IDS_TVM_GETITEMRECT,   IDC_GETITEMRECT,    IDD_TREEMISC },
    {IDS_TVM_INSERTITEM,    IDC_INSERTITEM,     IDD_TREEINSERT },
    {IDS_TVM_SELECTITEM,    IDC_SELECTITEM,     IDD_TREEMISC},
    {IDS_TVM_SETINDENT,     IDC_SETINDENT,      IDD_TREEMISC},
    {IDS_TVM_SETITEM,       IDC_SETITEM,        IDD_TREEGETITEM},
    {IDS_TVM_SETIMAGELIST,  IDC_SETIMAGELIST,   IDD_TREEIMAGES},
    {0, 0, 0} // EOT
                          };

/////////////////////////////////////////////////////////////////////////////
// CTreeIndex property page

IMPLEMENT_DYNCREATE(CTreeIndex, CIndexPage)

ixlist * CTreeIndex::getIndexTable() { return entries; }

CTreeIndex::CTreeIndex() : CIndexPage()
{
        //{{AFX_DATA_INIT(CTreeIndex)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

CTreeIndex::~CTreeIndex()
{
}


BEGIN_MESSAGE_MAP(CTreeIndex, CIndexPage)
        //{{AFX_MSG_MAP(CTreeIndex)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CTreeIndex::DoDataExchange(CDataExchange* pDX)
{
        CIndexPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CTreeIndex)
                // NOTE: the ClassWizard will add DDX and DDV calls here
        //}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CTreeIndex message handlers

