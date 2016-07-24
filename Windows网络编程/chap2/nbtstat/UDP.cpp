// UDP.cpp : implementation file
//

#include "stdafx.h"
#include "NBTSTAT.h"
#include "UDP.h"

#include "NBTSTATDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUDP
//-----------------------------------------------
extern CNBTSTATDlg* pDlg;
//-----------------------------------------------

CUDP::CUDP()
{
}

CUDP::~CUDP()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CUDP, CSocket)
	//{{AFX_MSG_MAP(CUDP)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CUDP member functions

void CUDP::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	pDlg->OnReceive();
	CSocket::OnReceive(nErrorCode);
}
