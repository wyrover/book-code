// Trackndx.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "ixable.h"
#include "IndexPg.h"
#include "Trackndx.h"

#include "tinybutt.h"
#include "IntEdit.h"
#include "HexDisp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static ixlist entries[] = {
     {IDS_TBM_CLEARSEL,            IDC_CLEARSEL,        IDD_TRACKBAR_SEL},
     {IDS_TBM_CLEARTICS,           IDC_CLEARTICS,       IDD_TRACKBAR_TICKS},
     {IDS_TBM_GETCHANNELRECT,      IDC_GETCHANNELRECT,  IDD_TRACKBAR_RECT},
     {IDS_TBM_GETLINESIZE,         IDC_GETLINESIZE,     IDD_TRACKBAR_POS},
     {IDS_TBM_GETNUMTICS,          IDC_GETNUMTICS,      IDD_TRACKBAR_TICKS},
     {IDS_TBM_GETPAGESIZE,         IDC_GETPAGESIZE,     IDD_TRACKBAR_POS},
     {IDS_TBM_GETPOS,              IDC_GETPOS,          IDD_TRACKBAR_POS},
     {IDS_TBM_GETPTICS,            IDC_GETPTICS,        IDD_TRACKBAR_TICKS},
     {IDS_TBM_GETRANGEMAX,         IDC_GETRANGEMAX,     IDD_TRACKBAR_POS},
     {IDS_TBM_GETRANGEMIN,         IDC_GETRANGEMIN,     IDD_TRACKBAR_POS},
     {IDS_TBM_GETSELEND,           IDC_GETSELEND,       IDD_TRACKBAR_SEL},
     {IDS_TBM_GETSELSTART,         IDC_GETSELSTART,     IDD_TRACKBAR_SEL},
     {IDS_TBM_GETTHUMBLENGTH,      IDC_GETTHUMBLENGTH,  IDD_TRACKBAR_SEL},
     {IDS_TBM_GETTHUMBRECT,        IDC_GETTHUMBRECT,    IDD_TRACKBAR_RECT},
     {IDS_TBM_GETTIC,              IDC_GETTIC,          IDD_TRACKBAR_TICKS},
     {IDS_TBM_GETTICPOS,           IDC_GETTICPOS,       IDD_TRACKBAR_TICKS},
     {IDS_TBM_SETLINESIZE,         IDC_SETLINESIZE,     IDD_TRACKBAR_POS},
     {IDS_TBM_SETPAGESIZE,         IDC_SETPAGESIZE,     IDD_TRACKBAR_POS},
     {IDS_TBM_SETPOS,              IDC_SETPOS,          IDD_TRACKBAR_POS},
     {IDS_TBM_SETRANGEMAX,         IDC_SETRANGEMAX,     IDD_TRACKBAR_POS},
     {IDS_TBM_SETRANGEMIN,         IDC_SETRANGEMIN,     IDD_TRACKBAR_POS},
     {IDS_TBM_SETTIC,              IDC_SETTIC,          IDD_TRACKBAR_TICKS},
     {IDS_TBM_SETTICFREQ,          IDC_SETTICFREQ,      IDD_TRACKBAR_TICKS},
     //---------------------------------------------------
     {0, 0, 0}  // EOT
                          };

/////////////////////////////////////////////////////////////////////////////
// CTrackIndex property page

IMPLEMENT_DYNCREATE(CTrackIndex, CIndexPage)

ixlist * CTrackIndex::getIndexTable() { return entries; }

CTrackIndex::CTrackIndex() : CIndexPage()
{
        //{{AFX_DATA_INIT(CTrackIndex)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

CTrackIndex::~CTrackIndex()
{
}


BEGIN_MESSAGE_MAP(CTrackIndex, CIndexPage)
        //{{AFX_MSG_MAP(CTrackIndex)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CTrackIndex::DoDataExchange(CDataExchange* pDX)
{
        CIndexPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CTrackIndex)
                // NOTE: the ClassWizard will add DDX and DDV calls here
        //}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CTrackIndex message handlers

