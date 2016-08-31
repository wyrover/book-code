// TrackRc.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "msglog.h"
#include "ixable.h"
#include "loggable.h"
#include "NumericEdit.h"
#include "TinyButt.h"
#include "TrackOps.h"
#include "TrackRc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrackRect property page

IMPLEMENT_DYNCREATE(CTrackRect, CTrackOps)

CTrackRect::CTrackRect() : CTrackOps(CTrackRect::IDD)
{
        //{{AFX_DATA_INIT(CTrackRect)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

CTrackRect::~CTrackRect()
{
}

void CTrackRect::DoDataExchange(CDataExchange* pDX)
{
        CTrackOps::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CTrackRect)
        DDX_Control(pDX, IDC_THUMBTOP, c_ThumbTop);
        DDX_Control(pDX, IDC_THUMBRIGHT, c_ThumbRight);
        DDX_Control(pDX, IDC_THUMBLEFT, c_ThumbLeft);
        DDX_Control(pDX, IDC_THUMBBOTTOM, c_ThumbBottom);
        DDX_Control(pDX, IDC_CHANNELRIGHT, c_ChannelRight);
        DDX_Control(pDX, IDC_CHANNELLEFT, c_ChannelLeft);
        DDX_Control(pDX, IDC_CHANNELBOTTOM, c_ChannelBottom);
        DDX_Control(pDX, IDC_CHANELLTOP, c_ChannelTop);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrackRect, CTrackOps)
        //{{AFX_MSG_MAP(CTrackRect)
        ON_BN_CLICKED(IDC_GETTHUMBRECT, OnGetthumbrect)
        ON_BN_CLICKED(IDC_GETCHANNELRECT, OnGetchannelrect)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrackRect message handlers

void CTrackRect::OnGetthumbrect() 
{
 CRect r;
 ::SendMessage(c_Slider->m_hWnd, TBM_GETTHUMBRECT, 0, (LPARAM)(LPRECT)&r);
 c_ThumbLeft.SetWindowText(r.left);
 c_ThumbTop.SetWindowText(r.top);
 c_ThumbRight.SetWindowText(r.right);
 c_ThumbBottom.SetWindowText(r.bottom);
 showResult_N_R_N(IDS_TBM_GETTHUMBRECT, &r);
        
}

void CTrackRect::OnGetchannelrect() 
{
 CRect r;

 ::SendMessage(c_Slider->m_hWnd, TBM_GETCHANNELRECT, 0, (LPARAM)(LPRECT)&r);
 c_ChannelLeft.SetWindowText(r.left);
 c_ChannelTop.SetWindowText(r.top);
 c_ChannelRight.SetWindowText(r.right);
 c_ChannelBottom.SetWindowText(r.bottom);
 showResult_N_R_N(IDS_TBM_GETCHANNELRECT, &r);
        
}

BOOL CTrackRect::OnInitDialog() 
{
        CTrackOps::OnInitDialog();
        
        // TODO: Add extra initialization here
        
        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}
