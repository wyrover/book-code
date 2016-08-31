// Palette.cpp : implementation file
//

#include "stdafx.h"
#include "DCExplorer.h"
#include "idcombo.h"
#include "gcombo.h"
#include "color.h"
#include "pcombo.h"
#include "dcvals.h"
#include "Palette.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPaletteDisplay property page

IMPLEMENT_DYNCREATE(CPaletteDisplay, CPropertyPage)

CPaletteDisplay::CPaletteDisplay() : CPropertyPage(CPaletteDisplay::IDD)
{
	//{{AFX_DATA_INIT(CPaletteDisplay)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPaletteDisplay::~CPaletteDisplay()
{
}

void CPaletteDisplay::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPaletteDisplay)
	DDX_Control(pDX, IDC_PALETTE, c_Palette);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPaletteDisplay, CPropertyPage)
	//{{AFX_MSG_MAP(CPaletteDisplay)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPaletteDisplay message handlers

BOOL CPaletteDisplay::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
 	c_Palette.maxlen = -1;

 // Note that we only do this if the number of palette entries available
 // is <= 256.

 	if(c_Palette.Load() == CB_ERR)
	   c_Palette.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
