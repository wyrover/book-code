// PaintColorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DSplit.h"
#include "PaintColorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PaintColorDlg dialog


PaintColorDlg::PaintColorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(PaintColorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(PaintColorDlg)
	m_Blue = 0;
	m_Green = 0;
	m_Red = 0;
	//}}AFX_DATA_INIT
}


void PaintColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PaintColorDlg)
	DDX_Text(pDX, IDC_EDIT_Blue, m_Blue);
	DDV_MinMaxInt(pDX, m_Blue, 0, 255);
	DDX_Text(pDX, IDC_EDIT_Green, m_Green);
	DDV_MinMaxInt(pDX, m_Green, 0, 255);
	DDX_Text(pDX, IDC_EDIT_Red, m_Red);
	DDV_MinMaxInt(pDX, m_Red, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PaintColorDlg, CDialog)
	//{{AFX_MSG_MAP(PaintColorDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PaintColorDlg message handlers
