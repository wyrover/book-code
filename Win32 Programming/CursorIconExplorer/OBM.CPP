// OBM.cpp : implementation file
//

#include "stdafx.h"
#include "CursorIconExplorer.h"
#include "OBM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(COBM, CPropertyPage)

/////////////////////////////////////////////////////////////////////////////
// COBM dialog


COBM::COBM() : CPropertyPage(COBM::IDD)
{
	//{{AFX_DATA_INIT(COBM)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
  bm = NULL;
}


void COBM::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COBM)
	DDX_Control(pDX, IDC_BITMAP, c_Bitmap);
	DDX_Control(pDX, IDC_AUTOCOPY, c_AutoCopy);
	DDX_Control(pDX, IDC_COPY, c_Copy);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COBM, CPropertyPage)
	//{{AFX_MSG_MAP(COBM)
	ON_BN_CLICKED(IDC_COPY, OnCopy)
	ON_BN_CLICKED(IDC_AUTOCOPY, OnAutocopy)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(OBM_LFARROWI, OBM_OLD_CLOSE, OBMSelect)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COBM message handlers

void COBM::OnCopy() 
{
     if(bm == NULL)
	return;

     OpenClipboard();
     EmptyClipboard();
     SetClipboardData(CF_BITMAP, bm);
     CloseClipboard();
	
}

/****************************************************************************
*                               COBM::OBMSelect
* Inputs:
*       UINT id: ID of control
* Result: void
*       
* Effect: 
*       Loads the standard bitmap
****************************************************************************/

void COBM::OBMSelect(UINT id)
    {
     bm = ::LoadBitmap(NULL, MAKEINTRESOURCE(id));
     c_Copy.EnableWindow(bm != NULL);
     if(c_AutoCopy.GetCheck())
	 OnCopy();

     c_Bitmap.SetBitmap(bm);
    }

void COBM::OnAutocopy() 
{
 c_Copy.ShowWindow(c_AutoCopy.GetCheck() ? SW_HIDE : SW_SHOW);
    
	
}

BOOL COBM::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
         c_Copy.EnableWindow(FALSE);
	// TODO: Add extra initialization here
	
	 for(int i = OBM_LFARROWI; i <= OBM_OLD_CLOSE; i++)
	    { /* enable buttons */
	     HBITMAP bm = ::LoadBitmap(NULL, MAKEINTRESOURCE(i));
	     if(bm == NULL)
		GetDlgItem(i)->EnableWindow(FALSE);
	    } /* enable buttons */

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
