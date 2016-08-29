// Editndx.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "IndexPg.h"
#include "Editndx.h"

#include "msglog.h"
#include "tinybutt.h"
#include "IntEdit.h"
#include "HexDisp.h"
#include "ixable.h"
#include "loggable.h"
#include "EditOps.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static ixlist entries[] = {
     { IDS_EM_CHARFROMPOS,      0,      IDD_EDITMAPPING},
     { IDS_EM_CANUNDO,          0,      IDD_EDITCONTENTS},
     { IDS_EM_CANUNDO,          0,      IDD_EDITSTATE},
     { IDS_EM_EMPTYUNDOBUFFER,  0,      IDD_EDITCONTENTS},
     { IDS_EM_EMPTYUNDOBUFFER,  0,      IDD_EDITSTATE},
     { IDS_EM_FMTLINES,         0,      IDD_EDITCONTENTS},
     { IDS_EM_GETFIRSTVISIBLELINE, 0,   IDD_EDITMAPPING},
     { IDS_EM_GETLINE,          0,      IDD_EDITCONTENTS},
     { IDS_EM_GETLINECOUNT,     0,      IDD_EDITCONTENTS},
     { IDS_EM_GETLIMITTEXT,     0,      IDD_EDITSTATE},
     { IDS_EM_GETMODIFY,        0,      IDD_EDITSTATE},
     { IDS_EM_GETPASSWORDCHAR,  0,      IDD_EDITSTATE},
     { IDS_EM_GETRECT,          0,      IDD_EDITRECTANGLES},
     { IDS_EM_GETMARGINS,       0,      IDD_EDITRECTANGLES},
     { IDS_EM_GETSEL,           0,      IDD_EDITCONTENTS},
     { IDS_EM_GETSEL,           0,      IDD_EDITMAPPING},
     { IDS_EM_GETSELTEXT,       0,      IDD_EDITCONTENTS},
     { IDS_EM_GETTHUMB,         0,      IDD_EDITSTATE},
     { IDS_EM_LINEFROMCHAR,     0,      IDD_EDITMAPPING},
     { IDS_EM_LINEINDEX,        0,      IDD_EDITMAPPING},
     { IDS_EM_LINELENGTH,       0,      IDD_EDITCONTENTS},
     { IDS_EM_LINESCROLL,       0,      IDD_EDITSCROLL},
     { IDS_EM_POSFROMCHAR,      0,      IDD_EDITMAPPING},
     { IDS_EM_REPLACESEL,       0,      IDD_EDITCONTENTS},
     { IDS_EM_SCROLL,           0,      IDD_EDITSCROLL},
     { IDS_EM_SCROLLCARET,      0,      IDD_EDITSCROLL},
     { IDS_EM_SETLIMITTEXT,     0,      IDD_EDITSTATE},
     { IDS_EM_SETMARGINS,       0,      IDD_EDITRECTANGLES},
     { IDS_EM_SETMODIFY,        0,      IDD_EDITSTATE},
     { IDS_EM_SETPASSWORDCHAR,  0,      IDD_EDITSTATE},
     { IDS_EM_SETREADONLY,      0,      IDD_EDITSTATE},
     { IDS_EM_SETRECT,          0,      IDD_EDITRECTANGLES},
     { IDS_EM_SETRECTNP,        0,      IDD_EDITRECTANGLES},
     { IDS_EM_SETSEL,           0,      IDD_EDITMAPPING},
     { IDS_EM_SETTABSTOPS,      0,      IDD_EDITTABS},
     { IDS_EM_UNDO,             0,      IDD_EDITCONTENTS},
     { IDS_EM_SETSEL,           0,      IDD_EDITCONTENTS},
     { IDS_WM_CLEAR,            0,      IDD_EDITCONTENTS},
     { IDS_WM_COPY,             0,      IDD_EDITCONTENTS},
     { IDS_WM_CUT,              0,      IDD_EDITCONTENTS},
     { IDS_WM_GETTEXT,          0,      IDD_EDITCONTENTS},
     { IDS_WM_PASTE,            0,      IDD_EDITCONTENTS},
     { IDS_WM_SETTEXT,          0,      IDD_EDITCONTENTS},
     { IDS_WM_UNDO,             0,      IDD_EDITCONTENTS},
     //---------------------------------------------------
     {0, 0, 0}  // EOT
                          };

/////////////////////////////////////////////////////////////////////////////
// CEditIndex property page

IMPLEMENT_DYNCREATE(CEditIndex, CIndexPage)

ixlist * CEditIndex::getIndexTable() { return entries; }

CEditIndex::CEditIndex() : CIndexPage()
{
        //{{AFX_DATA_INIT(CEditIndex)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

CEditIndex::~CEditIndex()
{
}


BEGIN_MESSAGE_MAP(CEditIndex, CIndexPage)
        //{{AFX_MSG_MAP(CEditIndex)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CEditIndex::DoDataExchange(CDataExchange* pDX)
{
        CIndexPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CEditIndex)
                // NOTE: the ClassWizard will add DDX and DDV calls here
        //}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CEditIndex message handlers

