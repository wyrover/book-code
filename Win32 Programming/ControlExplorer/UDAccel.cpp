// UDAccel.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "TinyButt.h"
#include "msglog.h"
#include "ixable.h"
#include "loggable.h"
#include "UDOps.h"
#include "UDAccel.h"
#include "copy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUpDownAccelerators property page

IMPLEMENT_DYNCREATE(CUpDownAccelerators, CUDOps)

CUpDownAccelerators::CUpDownAccelerators() : CUDOps(CUpDownAccelerators::IDD)
{
        //{{AFX_DATA_INIT(CUpDownAccelerators)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

CUpDownAccelerators::~CUpDownAccelerators()
{
}

void CUpDownAccelerators::DoDataExchange(CDataExchange* pDX)
{
        CUDOps::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CUpDownAccelerators)
        DDX_Control(pDX, IDC_ACCELS, c_Accels);
        DDX_Control(pDX, IDC_SETACCEL, c_SetAccel);
        DDX_Control(pDX, IDC_GETACCEL, c_GetAccel);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUpDownAccelerators, CUDOps)
        //{{AFX_MSG_MAP(CUpDownAccelerators)
        ON_BN_CLICKED(IDC_GETACCEL, OnGetaccel)
        ON_BN_CLICKED(IDC_SETACCEL, OnSetaccel)
        ON_BN_CLICKED(IDC_COPY, OnCopy)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUpDownAccelerators message handlers


/****************************************************************************
*                       CUpDownAccelerators::initAccels
* Result: void
*       
* Effect: 
*       Initializes the accelerator listing control
****************************************************************************/

void CUpDownAccelerators::initAccels()
    {
     LV_COLUMN col;
     CString hdr;

     hdr.LoadString(IDS_NSEC);
     col.cx = c_Accels.GetStringWidth(hdr);

     // We have to add the width of the small icon tot he column width
     col.cx += ::GetSystemMetrics(SM_CXSMICON);
     col.fmt = LVCFMT_RIGHT;
     col.pszText = (LPTSTR)(LPCTSTR)hdr;
     col.iSubItem = 0;
     col.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
     c_Accels.InsertColumn(0, &col);

     hdr.LoadString(IDS_NINC);
     col.cx = c_Accels.GetStringWidth(hdr);
     col.cx += ::GetSystemMetrics(SM_CXSMICON);
     col.fmt = LVCFMT_RIGHT;
     col.pszText = (LPTSTR)(LPCTSTR)hdr;
     col.iSubItem = 1;
     col.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
     c_Accels.InsertColumn(1, &col);
    }

BOOL CUpDownAccelerators::OnInitDialog() 
{
        CUDOps::OnInitDialog();
        
        initAccels();
        
        c_SetAccel.EnableWindow(FALSE); // NYI: any way to edit the
                                        // values or otherwise do anything
                                        // that makes setting them useful

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CUpDownAccelerators::OnGetaccel() 
{
 int n = ::SendMessage(c_UpDown->m_hWnd, UDM_GETACCEL, 0, 0);
 showResult_d_d_d(IDS_UDM_GETACCEL, 0, 0, n);

 LPUDACCEL accels = new UDACCEL[n];

 n = ::SendMessage(c_UpDown->m_hWnd, UDM_GETACCEL, n, (LPARAM)accels);

 int i;

 CString x;
 x.Format(_T("&accels=0x%08x"), (int)accels);

 CString result;
 CString sep(_T(""));

 result.Format(_T("%d: ["), n);
 for(i = 0; i < n; i++)
    { /* add each */
     result += sep;
     CString pair;
     pair.Format(_T("(%d,%d)"), accels[i].nSec, accels[i].nInc);
     result += pair;
     sep = _T(", ");
    } /* add each */
 result += _T("]");

 showResult_d_s_s(IDS_UDM_GETACCEL, 0, x, result);

 for(i = 0; i < n; i++)
    { /* load to control */
     int item = c_Accels.GetItemCount();
     CString s;

     s.Format(_T("%d"), accels[i].nSec);
     c_Accels.InsertItem(item, s);

     s.Format(_T("%d"), accels[i].nInc);
     c_Accels.SetItemText(item, 1, s);
    } /* load to control */

 delete accels;
}

void CUpDownAccelerators::OnSetaccel() 
{
        // TODO: Add your control notification handler code here
        
}

void CUpDownAccelerators::OnCopy() 
{
 copyToClipboard(&c_Accels);
        
}
