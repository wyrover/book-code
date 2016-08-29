// Progress.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "msgLog.h"
#include "ixable.h"
#include "loggable.h"
#include "NumericEdit.h"
#include "TinyButt.h"
#include "Progress.h"
#include "copy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgress property page

IMPLEMENT_DYNCREATE(CProgress, CPropertyPage)

CProgress::CProgress() : CLoggingPage(CProgress::IDD)
{
        //{{AFX_DATA_INIT(CProgress)
        //}}AFX_DATA_INIT
}

CProgress::~CProgress()
{
}

void CProgress::DoDataExchange(CDataExchange* pDX)
{
        CPropertyPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CProgress)
        DDX_Control(pDX, IDC_PROGRESS, c_Progress);
        DDX_Control(pDX, IDC_SPINPOS, c_SpinPos);
        DDX_Control(pDX, IDC_STEPIT, c_StepIt);
        DDX_Control(pDX, IDC_STEP, c_Step);
        DDX_Control(pDX, IDC_SPINSTEP, c_SpinStep);
        DDX_Control(pDX, IDC_SPINRANGELOW, c_SpinLow);
        DDX_Control(pDX, IDC_SPINRANGEHIGH, c_SpinHigh);
        DDX_Control(pDX, IDC_POSVAL, c_PosVal);
        DDX_Control(pDX, IDC_MESSAGES, c_Messages);
        DDX_Control(pDX, IDC_DELTAPOS, c_DeltaPos);
        DDX_Control(pDX, IDC_DELTA, c_Delta);
        DDX_Control(pDX, IDC_RANGEHIGH, c_RangeHigh);
        DDX_Control(pDX, IDC_SPINDELTA, c_SpinDelta);
        DDX_Control(pDX, IDC_SETSTEP, c_SetStep);
        DDX_Control(pDX, IDC_SETPOS, c_SetPos);
        DDX_Control(pDX, IDC_SETRANGE, c_SetRange);
        DDX_Control(pDX, IDC_RANGELOW, c_RangeLow);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgress, CPropertyPage)
        //{{AFX_MSG_MAP(CProgress)
        ON_BN_CLICKED(IDC_DELTAPOS, OnDeltapos)
        ON_BN_CLICKED(IDC_SETPOS, OnSetpos)
        ON_BN_CLICKED(IDC_SETRANGE, OnSetrange)
        ON_BN_CLICKED(IDC_SETSTEP, OnSetstep)
        ON_BN_CLICKED(IDC_STEPIT, OnStepit)
        ON_BN_CLICKED(IDC_COPY, OnCopy)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgress message handlers


/****************************************************************************
*                              CProgress::addMessage
* Inputs:
*       CString s: Message to log
* Result: void
*       
* Effect: 
*       Logs the message, supplying the boilerplate
****************************************************************************/

void CProgress::addMessage(CString s)
    {
     CString ctlName;
     ctlName.LoadString(IDS_PROGRESS);
     
     c_Messages.addMessage(c_Progress.m_hWnd, ctlName, s);
    }

void CProgress::OnDeltapos() 
{
 int delta = c_Delta.GetWindowInt();
 int result = ::SendMessage(c_Progress.m_hWnd, PBM_DELTAPOS, delta, 0);
 showResult_d_d_d(IDS_PBM_DELTAPOS, delta, 0, result);
        
}

void CProgress::OnSetpos() 
{
 int pos = c_PosVal.GetWindowInt();
 int result = ::SendMessage(c_Progress.m_hWnd, PBM_SETPOS, pos, 0);
 showResult_d_d_d(IDS_PBM_SETPOS, pos, 0, result);
        
}

void CProgress::OnSetrange() 
{
 int low = c_RangeLow.GetWindowInt();
 int high = c_RangeHigh.GetWindowInt();
 int result = ::SendMessage(c_Progress.m_hWnd, PBM_SETRANGE, 0, 
                                MAKELONG(low, high));
 showResult_d_DW_DW(IDS_PBM_SETRANGE, 0, MAKELONG(low, high), result);
}

void CProgress::OnSetstep() 
{
 int step = c_Step.GetWindowInt();
 int result = ::SendMessage(c_Progress.m_hWnd, PBM_SETSTEP, step, 0);
 showResult_d_d_d(IDS_PBM_SETSTEP, step, 0, result);
}

void CProgress::OnStepit() 
{
 int result = ::SendMessage(c_Progress.m_hWnd, PBM_STEPIT, 0, 0);
 showResult_d_d_d(IDS_PBM_STEPIT, 0, 0, result);
}

BOOL CProgress::OnInitDialog() 
{
#define DEFAULT_SPIN_LOW 0
#define DEFAULT_SPIN_HIGH 1000

        initialized = FALSE;

        CPropertyPage::OnInitDialog();
        
        LRESULT range = ::SendMessage(c_Progress.m_hWnd, 
                                        PBM_SETRANGE, 0, MAKELONG(0, 1));
        c_Progress.SetRange(LOWORD(range), HIWORD(range));
        
        int step = c_Progress.SetStep(0);
        c_Progress.SetStep(step);

        c_SpinPos.SetRange(DEFAULT_SPIN_LOW, DEFAULT_SPIN_HIGH);
        c_SpinPos.SetPos(DEFAULT_SPIN_LOW);
        
        c_SpinStep.SetRange(DEFAULT_SPIN_LOW, DEFAULT_SPIN_HIGH);
        c_SpinStep.SetPos( step );

        c_SpinLow.SetRange(DEFAULT_SPIN_LOW, DEFAULT_SPIN_HIGH);
        c_SpinLow.SetPos(LOWORD(range));

        c_SpinHigh.SetRange(DEFAULT_SPIN_LOW, DEFAULT_SPIN_HIGH);
        c_SpinHigh.SetPos(HIWORD(range));

        c_SpinDelta.SetRange(DEFAULT_SPIN_LOW, DEFAULT_SPIN_HIGH);
        c_SpinDelta.SetPos( (HIWORD(range) - LOWORD(range)) / 10);
        
        CString s;
        s.LoadString(IDS_PROGRESS);
        c_Messages.initialize(c_Messages.GetStringWidth(s));
        c_Messages.setWrap(TRUE);

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CProgress::OnCopy() 
{
 copyToClipboard(&c_Progress);
        
}
