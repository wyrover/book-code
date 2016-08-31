// Listndx.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "ixable.h"
#include "IndexPg.h"
#include "Listndx.h"

#include "tinybutt.h"
#include "IntEdit.h"
#include "HexDisp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static ixlist entries[] = {
 //  {IDS_LB_ADDFILE,              0, IDD_LISTBOXDIR    },
     {IDS_LB_ADDSTRING,            IDC_ADDSTRING,       IDD_LISTBOXSTRING },
     {IDS_LB_DELETESTRING,         IDC_DELETESTRING,    IDD_LISTBOXSTRING },
     {IDS_LB_DIR,                  IDC_DIR,             IDD_LISTBOXDIR    },
     {IDS_LB_FINDSTRING,           IDC_FINDSTRING,      IDD_LISTBOXSTRING },
     {IDS_LB_FINDSTRINGEXACT,      IDC_FINDSTRINGEXACT, IDD_LISTBOXSTRING },
     {IDS_LB_GETANCHORINDEX,       IDC_GETANCHORINDEX,  IDD_LISTBOXQUERY  },
     {IDS_LB_GETCARETINDEX,        IDC_GETCARETINDEX,   IDD_LISTBOXQUERY  },
     {IDS_LB_GETCOUNT,             IDC_GETCOUNT,        IDD_LISTBOXQUERY  },
     {IDS_LB_GETCURSEL,            IDC_GETCURSEL,       IDD_LISTBOXQUERY  },
     {IDS_LB_GETHORIZONTALEXTENT,  IDC_GETHORIZONTALEXTENT,IDD_LISTBOXQUERY},
     {IDS_LB_GETITEMDATA,          IDC_GETITEMDATA,     IDD_LISTBOXSTRING },
     {IDS_LB_GETITEMHEIGHT,        IDC_GETITEMHEIGHT,   IDD_LISTBOXSTRING },
     {IDS_LB_GETITEMHEIGHT,        IDC_GETITEMHEIGHT,   IDD_LISTBOXRECT   },
     {IDS_LB_GETITEMRECT,          IDC_GETITEMRECT,     IDD_LISTBOXRECT   },
     {IDS_LB_GETLOCALE,            IDC_GETLOCALE,       IDD_LISTBOXQUERY  },
     {IDS_LB_GETSEL,               IDC_GETSEL,          IDD_LISTBOXSTRING },
     {IDS_LB_GETSELCOUNT,          IDC_GETSELCOUNT,     IDD_LISTBOXQUERY  },
     {IDS_LB_GETSELITEMS,          IDC_GETSELITEMS,     IDD_LISTBOXQUERY  },
     {IDS_LB_GETTEXT,              IDC_GETTEXT,         IDD_LISTBOXSTRING },
     {IDS_LB_GETTEXTLEN,           IDC_GETTEXTLENGTH,   IDD_LISTBOXSTRING },
     {IDS_LB_GETTOPINDEX,          IDC_GETTOPINDEX,     IDD_LISTBOXQUERY  },
 //  {IDS_LB_INITSTORAGE,          0, IDD_ },
     {IDS_LB_INSERTSTRING,         IDC_INSERTSTRING,    IDD_LISTBOXSTRING },
 //  {IDS_LB_ITEMFROMPOINT,        0, IDD_ },
     {IDS_LB_RESETCONTENT,         IDC_RESETCONTENT,    IDD_LISTBOXSTATE  },
     {IDS_LB_SELECTSTRING,         IDC_SELECTSTRING,    IDD_LISTBOXSTRING },
     {IDS_LB_SELITEMRANGE,         IDC_SELITEMRANGE,    IDD_LISTBOXSTATE  },
     {IDS_LB_SELITEMRANGEEX,       IDC_SELITEMRANGEEX,  IDD_LISTBOXSTATE  },
     {IDS_LB_SETANCHORINDEX,       IDC_SETANCHORINDEX,  IDD_LISTBOXSTATE  },
     {IDS_LB_SETCARETINDEX,        IDC_SETCARETINDEX,   IDD_LISTBOXSTATE  },
     {IDS_LB_SETCOLUMNWIDTH,       IDC_SETCOLUMNWIDTH,  IDD_LISTBOXSTATE  },
 //  {IDS_LB_SETCOUNT,             0, IDD_ },
     {IDS_LB_SETCURSEL,            IDC_SETCURSEL,       IDD_LISTBOXSTRING },
     {IDS_LB_SETHORIZONTALEXTENT,  IDC_SETHORIZONTALEXTENT,IDD_LISTBOXQUERY },
     {IDS_LB_SETITEMDATA,          IDC_SETITEMDATA,     IDD_LISTBOXSTRING },
     {IDS_LB_SETITEMHEIGHT,        IDC_SETITEMHEIGHT,   IDD_LISTBOXSTRING },
     {IDS_LB_SETLOCALE,            IDC_SETLOCALE,       IDD_LISTBOXSTATE  },
     {IDS_LB_SETSEL,               IDC_SETSEL,          IDD_LISTBOXSTRING },
     {IDS_LB_SETTABSTOPS,          IDC_TABSTOPS,        IDD_LISTBOXTABS   },
     {IDS_LB_SETTOPINDEX,          IDC_SETTOPINDEX,     IDD_LISTBOXSTATE  },
     {IDS_WM_SETREDRAW,            IDC_SETREDRAW,       IDD_LISTBOXSTATE  },
     //---------------------------------------------------
     {0, 0, 0}  // EOT
                          };

/////////////////////////////////////////////////////////////////////////////
// CListIndex property page

IMPLEMENT_DYNCREATE(CListIndex, CIndexPage)

ixlist * CListIndex::getIndexTable() { return entries; }

CListIndex::CListIndex() : CIndexPage()
{
        //{{AFX_DATA_INIT(CListIndex)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

CListIndex::~CListIndex()
{
}


BEGIN_MESSAGE_MAP(CListIndex, CIndexPage)
        //{{AFX_MSG_MAP(CListIndex)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CListIndex::DoDataExchange(CDataExchange* pDX)
{
        CIndexPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CListIndex)
                // NOTE: the ClassWizard will add DDX and DDV calls here
        //}}AFX_DATA_MAP
}
/////////////////////////////////////////////////////////////////////////////
// CListIndex message handlers

