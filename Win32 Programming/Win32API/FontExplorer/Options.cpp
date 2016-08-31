// Options.cpp : implementation file
//

#include "stdafx.h"
#include "FontExplorer.h"
#include "dispopts.h"
#include "Options.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptions dialog


COptions::COptions(CWnd* pParent /*=NULL*/)
        : CDialog(COptions::IDD, pParent)
{
        //{{AFX_DATA_INIT(COptions)
        m_Sample = _T("");
        m_SmallSample = _T("");
        //}}AFX_DATA_INIT
        opts.debug = FALSE;
        opts.vrule = FALSE;
        opts.chrule = FALSE;
        opts.colors = TRUE;
        opts.white = FALSE;
        opts.track = FALSE;
        opts.trackval = 0;
        opts.kerning = FALSE;
}


void COptions::DoDataExchange(CDataExchange* pDX)
{
#define LOWKERN   0
#define HIKERN   10
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(COptions)
        DDX_Control(pDX, IDC_KERNSPIN, c_SpinKern);
        DDX_Control(pDX, IDC_SAMPLE, c_Sample);
        DDX_Text(pDX, IDC_SAMPLE, m_Sample);
        DDX_Text(pDX, IDC_SMALLSAMPLE, m_SmallSample);
        //}}AFX_DATA_MAP
        DDX_Check(pDX, IDC_DEBUG, opts.debug);
        DDX_Check(pDX, IDC_HRULE, opts.hrule);
        DDX_Check(pDX, IDC_VRULE, opts.vrule);
        DDX_Check(pDX, IDC_CHRULE, opts.chrule);
        DDX_Check(pDX, IDC_COLORS, opts.colors);
        DDX_Check(pDX, IDC_WHITE, opts.white);
        DDX_Check(pDX, IDC_TRACK, opts.track);
        DDX_Check(pDX, IDC_PAIR, opts.kerning);
        DDX_Text(pDX, IDC_KERN, opts.trackval);
        DDV_MinMaxInt(pDX, opts.trackval, LOWKERN, HIKERN);
}


BEGIN_MESSAGE_MAP(COptions, CDialog)
        //{{AFX_MSG_MAP(COptions)
        ON_EN_CHANGE(IDC_SAMPLE, OnChangeSample)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptions message handlers

void COptions::OnChangeSample() 
{
        // TODO: Add your control notification handler code here
        
}

BOOL COptions::OnInitDialog() 
{
        CDialog::OnInitDialog();
        
        c_SpinKern.SetRange(LOWKERN, HIKERN);
        
        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}
