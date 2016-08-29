// Bricks.cpp : implementation file
//

#include "stdafx.h"
#include "DCExplorer.h"
#include "drawbrk.h"
#include "Bricks.h"
#include "copy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBricks dialog


CBricks::CBricks(CWnd* pParent /*=NULL*/)
	: CDialog(CBricks::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBricks)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBricks::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBricks)
	DDX_Control(pDX, IDC_TRANSPARENT, c_Transparent);
	DDX_Control(pDX, IDC_OPAQUE, c_Opaque);
	DDX_Control(pDX, IDC_BRICKS, c_Bricks);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBricks, CDialog)
	//{{AFX_MSG_MAP(CBricks)
	ON_BN_CLICKED(IDC_TRANSPARENT, OnTransparent)
	ON_BN_CLICKED(IDC_OPAQUE, OnOpaque)
	ON_BN_CLICKED(IDC_COPY, OnCopy)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CDrawBricks::UWM_QUERY_BKMODE, OnQueryBkMode)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBricks message handlers

void CBricks::OnTransparent() 
{
 c_Bricks.InvalidateRect(NULL);
	
}

void CBricks::OnOpaque() 
{
 c_Bricks.InvalidateRect(NULL);
}

BOOL CBricks::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
 	CheckRadioButton(IDC_OPAQUE, IDC_TRANSPARENT, IDC_OPAQUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBricks::OnCopy() 
{
 copyToClipboard(&c_Bricks);
	
}

/****************************************************************************
*                           CBricks::OnQueryBkMode
* Inputs:
*       WPARAM: ignored
*	LPARAM: ignored
* Result: LRESULT
*       OPAQUE or TRANSPARENT
****************************************************************************/

LRESULT CBricks::OnQueryBkMode(WPARAM, LPARAM)
    {
     if(c_Transparent.GetCheck())
	return TRANSPARENT;

     return OPAQUE;
    }
