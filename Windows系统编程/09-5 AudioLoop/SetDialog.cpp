// SetDialog.cpp : implementation file
//

#include "stdafx.h"
#include "test_audio_loop.h"
#include "SetDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetDialog dialog

CSetDialog::CSetDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSetDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetDialog)
	m_workrate = _T("");
	//}}AFX_DATA_INIT
}


void CSetDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetDialog)
	DDX_CBString(pDX, IDC_WORKRATE, m_workrate);
	DDV_MaxChars(pDX, m_workrate, 30);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetDialog, CDialog)
	//{{AFX_MSG_MAP(CSetDialog)
//??	ON_BN_CLICKED(IDC_BUTTON1, OnAdvancedSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetDialog message handlers

