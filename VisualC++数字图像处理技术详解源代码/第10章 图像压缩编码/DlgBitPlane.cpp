#include "stdafx.h"
#include "ImageProcessing.h"
#include "DlgCoding.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgBitPlane dialog


CDlgBitPlane::CDlgBitPlane(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBitPlane::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgBitPlane)
	m_BItNumber = 0;
	//}}AFX_DATA_INIT
}


void CDlgBitPlane::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBitPlane)
	DDX_Text(pDX, IDC_BIT, m_BItNumber);
	DDV_MinMaxByte(pDX, m_BItNumber, 1, 8);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBitPlane, CDialog)
	//{{AFX_MSG_MAP(CDlgBitPlane)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBitPlane message handlers

void CDlgBitPlane::OnOK() 
{	
	CDialog::OnOK();
}
