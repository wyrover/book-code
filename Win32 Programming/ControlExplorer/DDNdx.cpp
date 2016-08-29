// ddndx.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "ixable.h"
#include "IndexPg.h"
#include "ddNdx.h"

#include "tinybutt.h"
#include "IntEdit.h"
#include "HexDisp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static ixlist entries[] = {
     {IDS_CB_ADDSTRING,            IDC_ADDSTRING,       IDD_DDTEXT          },
     {IDS_CB_DELETESTRING,         IDC_DELETESTRING,    IDD_LISTBOXSTRING   },
//   {IDS_CB_DIR,                  IDC_DIR,             IDD_LISTBOXDIR      },
     {IDS_CB_FINDSTRING,           IDC_FINDSTRING,      IDD_DDTEXT          },
     {IDS_CB_FINDSTRINGEXACT,      IDC_FINDSTRINGEXACT, IDD_DDTEXT          },
     {IDS_CB_GETCOUNT,             IDC_GETCOUNT,        IDD_DROPDOWNMESSAGES},
     {IDS_CB_GETCURSEL,            IDC_GETCURSEL,       IDD_DROPDOWNMESSAGES},
     {IDS_CB_GETDROPPEDCONTROLRECT,IDC_GETDROPPEDCONTROLRECT, IDD_DDEFFECTS },
     {IDS_CB_GETDROPPEDSTATE,      IDC_GETDROPPEDSTATE, IDD_DDEFFECTS       },
     {IDS_CB_GETDROPPEDWIDTH,      IDC_GETDROPPEDWIDTH, IDD_DDEFFECTS       },
     {IDS_CB_GETEDITSEL,           IDC_GETEDITSEL,      IDD_DDTEXT          },
     {IDS_CB_GETEXTENDEDUI,        IDC_GETEXTENDEDUI,   IDD_DDEFFECTS       },
     {IDS_CB_GETHORIZONTALEXTENT,  IDC_GETHORIZONTALEXTENT,
                                                        IDD_DROPDOWNMESSAGES},
     {IDS_CB_GETITEMDATA,          IDC_GETITEMDATA,     IDD_DDTEXT          },
     {IDS_CB_GETITEMHEIGHT,        IDC_GETITEMHEIGHT,   IDD_DDTEXT          },
     {IDS_CB_GETLOCALE,            IDC_GETLOCALE,       IDD_DROPDOWNMESSAGES},
     {IDS_CB_GETLBTEXT,            IDC_GETLBTEXT,       IDD_DDTEXT          },
     {IDS_CB_GETLBTEXTLEN,         IDC_GETLBTEXTLENGTH, IDD_DDTEXT          },
     {IDS_CB_GETTOPINDEX,          IDC_GETTOPINDEX,     IDD_DROPDOWNMESSAGES},
 //  {IDS_CB_INITSTORAGE,          0, IDD_ },
     {IDS_CB_INSERTSTRING,         IDC_INSERTSTRING,    IDD_DDTEXT          },
 //  {IDS_CB_ITEMFROMPOINT,        0, IDD_ },
     {IDS_CB_LIMITTEXT,            IDC_LIMITTEXT,       IDD_DROPDOWNMESSAGES},
     {IDS_CB_RESETCONTENT,         IDC_RESETCONTENT,    IDD_DROPDOWNMESSAGES},
     {IDS_CB_SELECTSTRING,         IDC_SELECTSTRING,    IDD_DDTEXT          },
     {IDS_CB_SETCURSEL,            IDC_SETCURSEL,       IDD_DROPDOWNMESSAGES},
     {IDS_CB_SETDROPPEDWIDTH,      IDC_SETDROPPEDWIDTH, IDD_DDEFFECTS       },
     {IDS_CB_SETEDITSEL,           IDC_SETEDITSEL,      IDD_DDTEXT          },
     {IDS_CB_SETEXTENDEDUI,        IDC_SETEXTENDEDUI,   IDD_DDEFFECTS       },
     {IDS_CB_SETHORIZONTALEXTENT,  IDC_SETHORIZONTALEXTENT,
                                                        IDD_DROPDOWNMESSAGES},
     {IDS_CB_SETITEMDATA,          IDC_SETITEMDATA,     IDD_DDTEXT          },
     {IDS_CB_SETITEMHEIGHT,        IDC_SETITEMHEIGHT,   IDD_DDTEXT          },
     {IDS_CB_SETLOCALE,            IDC_SETLOCALE,       IDD_DROPDOWNMESSAGES},
     {IDS_CB_SETTOPINDEX,          IDC_SETTOPINDEX,     IDD_DROPDOWNMESSAGES},
//   {IDS_WM_SETREDRAW,            IDC_SETREDRAW,       0 },
     {IDS_CB_SHOWDROPDOWN,         IDC_SHOWDROPDOWN,    IDD_DDEFFECTS       },
     //---------------------------------------------------
     {0, 0, 0}  // EOT
                          };

/////////////////////////////////////////////////////////////////////////////
// CComboIndex property page

IMPLEMENT_DYNCREATE(CComboIndex, CIndexPage)

ixlist * CComboIndex::getIndexTable() { return entries; }

CComboIndex::CComboIndex() : CIndexPage()
{
        //{{AFX_DATA_INIT(CListIndex)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

CComboIndex::~CComboIndex()
{
}


BEGIN_MESSAGE_MAP(CComboIndex, CIndexPage)
        //{{AFX_MSG_MAP(CComboIndex)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CComboIndex::DoDataExchange(CDataExchange* pDX)
{
        CIndexPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CComboIndex)
                // NOTE: the ClassWizard will add DDX and DDV calls here
        //}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CComboIndex message handlers

