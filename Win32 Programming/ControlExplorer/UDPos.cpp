// UDPos.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "TinyButt.h"
#include "NumericEdit.h"
#include "msglog.h"
#include "ixable.h"
#include "loggable.h"
#include "UDOps.h"
#include "UDPos.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUpDownPos property page

IMPLEMENT_DYNCREATE(CUpDownPos, CUDOps)

CUpDownPos::CUpDownPos() : CUDOps(CUpDownPos::IDD)
{
        //{{AFX_DATA_INIT(CUpDownPos)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

CUpDownPos::~CUpDownPos()
{
}

void CUpDownPos::DoDataExchange(CDataExchange* pDX)
{
        CUDOps::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CUpDownPos)
        DDX_Control(pDX, IDC_SWAP, c_Swap);
        DDX_Control(pDX, IDC_SPINPOS, c_SpinPos);
        DDX_Control(pDX, IDC_SPINLOW, c_SpinLow);
        DDX_Control(pDX, IDC_SPINHIGH, c_SpinHigh);
        DDX_Control(pDX, IDC_SETRANGE, c_SetRange);
        DDX_Control(pDX, IDC_SETPOS, c_SetPos);
        DDX_Control(pDX, IDC_POS, c_Pos);
        DDX_Control(pDX, IDC_LOW, c_Low);
        DDX_Control(pDX, IDC_HIGH, c_High);
        DDX_Control(pDX, IDC_GETRANGE, c_GetRange);
        DDX_Control(pDX, IDC_GETPOS, c_GetPos);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUpDownPos, CUDOps)
        //{{AFX_MSG_MAP(CUpDownPos)
        ON_BN_CLICKED(IDC_SWAP, OnSwap)
        ON_BN_CLICKED(IDC_SETRANGE, OnSetrange)
        ON_BN_CLICKED(IDC_SETPOS, OnSetpos)
        ON_BN_CLICKED(IDC_GETPOS, OnGetpos)
        ON_BN_CLICKED(IDC_GETRANGE, OnGetrange)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUpDownPos message handlers

void CUpDownPos::OnSwap() 
{
 int t = c_High.GetWindowInt();
 c_High.SetWindowText(c_Low.GetWindowInt());
 c_Low.SetWindowText(t);        
}

void CUpDownPos::OnGetrange() 
{
 LONG range = ::SendMessage(c_UpDown->m_hWnd, UDM_GETRANGE, 0, 0);
 showResult_N_N_DW(IDS_UDM_GETRANGE, range);
 c_SpinLow.SetPos((short)LOWORD(range));
 c_SpinHigh.SetPos((short)HIWORD(range));
        
}

void CUpDownPos::OnSetrange() 
{
 LONG range = MAKELONG((short)c_Low.GetWindowInt(), 
                                        (short)c_High.GetWindowInt());
 ::SendMessage(c_UpDown->m_hWnd, UDM_SETRANGE, 0, range);
 showResult_d_DW_N(IDS_UDM_SETRANGE, 0, range);
}

void CUpDownPos::OnSetpos() 
{
 LONG pos = MAKELONG((short)c_Pos.GetWindowInt(), 0);
 int result = ::SendMessage(c_UpDown->m_hWnd, UDM_SETPOS, 0, pos);
 showResult_d_DW_d(IDS_UDM_SETPOS, 0, pos, result);
}

void CUpDownPos::OnGetpos() 
{
 LONG pos = ::SendMessage(c_UpDown->m_hWnd, UDM_GETPOS, 0, 0);
 c_Pos.SetWindowText((short)LOWORD(pos));

 CString error;
 error.LoadString(IDS_UDERROR);

 CString s;
 s.Format(_T("%08x = (%d, %d) %s"), pos, HIWORD(pos), LOWORD(pos),
                HIWORD(pos) ? error : _T(""));
 showResult_N_N_s(IDS_UDM_GETPOS, s);
}

/****************************************************************************
*                          CUpDownPos::setSpinRange
* Inputs:
*       CSpinButtonCtrl * ctl: Control to set
*       int low: Lower limit
*       int high: Upper limit
* Result: void
*       
* Effect: 
*       This sets the spin controls to a canonical set of values.  The
*       lower value of the spin control will be based on the smaller of the two
*       limits, the upper value will be based on the larger of the two limits.
*       The lower limit will be 10 times the low value, and the upper
*       limit 10 times the high value, subject to the constraints that
*       low cannot exceed -32768 and the high cannot exceed 32767.
****************************************************************************/

void CUpDownPos::setSpinRange(CSpinButtonCtrl * ctl, int low, int high)
    {
     int L = low;
     int H = high;

     if (L > H)
        { /* swap */
         L = high;
         H = low;
        } /* swap */

     // Adjust for larger range
     L *= 10;
     H *= 10;

     if(L < -32768)
        L = -32768;
     if(H > 32767)
        H = 32767;

     if(H - L > 32767)
        { /* limit range */
         if(L < 0)
            { /* split difference */
             L = -16384;
             H = 16383;
            } /* split difference */
         else
            L = H - 32767; // arbitrary choice of what to do
        } /* limit range */

     ctl->SetRange(L, H);
    }

BOOL CUpDownPos::OnInitDialog() 
{
        CUDOps::OnInitDialog();
        
        int low;
        int high;
        c_UpDown->GetRange(low, high);
        setSpinRange(&c_SpinLow, low, high);
        setSpinRange(&c_SpinHigh, low, high);
        
        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

