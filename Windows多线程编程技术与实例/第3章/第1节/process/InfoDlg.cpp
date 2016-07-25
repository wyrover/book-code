// InfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "process.h"
#include "InfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInfoDlg dialog


CInfoDlg::CInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInfoDlg)
	m_strExeFile = _T("");
	
	//}}AFX_DATA_INIT
}


void CInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInfoDlg)
	DDX_Text(pDX, IDC_EXEFILE, m_strExeFile);
	DDX_Text(pDX,IDC_DWSIZE,dwSize);
	DDX_Text(pDX,IDC_CNTUSAGE,cntUsage);
	DDX_Text(pDX,IDC_PROCESSID,processID);
	DDX_Text(pDX,IDC_DEFAULTHEAPID,defaultHeapID);
	DDX_Text(pDX,IDC_MODULEID,moduleID);
	DDX_Text(pDX,IDC_CNTTHREADS,cntThreads);
	DDX_Text(pDX,IDC_PARENTPROCESSID,parentProcessID);
	DDX_Text(pDX,IDC_CLASSBASE,classBase);
	DDX_Text(pDX,IDC_DWFLAGS,dwFlags);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CInfoDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInfoDlg message handlers

HBRUSH CInfoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor==CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(160,180,220));
		HBRUSH br=CreateSolidBrush(RGB(160,180,235));
		return br;
	}
	else if(nCtlColor==CTLCOLOR_DLG)
	{
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(160,180,220));
		HBRUSH br=CreateSolidBrush(RGB(160,180,235));
		return br;
	}
	return hbr;
}
