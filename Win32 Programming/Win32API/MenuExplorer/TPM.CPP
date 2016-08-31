// TPM.cpp : implementation file
//

#include "stdafx.h"
#include "MenuExplorer.h"
#include "TPM.h"
#include "uwm.h"
#include "is95.h"
#include "menusht.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTPM property page

IMPLEMENT_DYNCREATE(CTPM, CPropertyPage)

CTPM::CTPM() : CPropertyPage(CTPM::IDD)
{
	//{{AFX_DATA_INIT(CTPM)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CTPM::~CTPM()
{
}

void CTPM::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTPM)
	DDX_Control(pDX, IDC_LOG, c_Log);
	DDX_Control(pDX, IDC_VERTICAL_ALIGNMENT, c_VerticalAlignment);
	DDX_Control(pDX, IDC_TPM_RETURNCMD, c_ReturnCmd);
	DDX_Control(pDX, IDC_TPM_NONOTIFY, c_NoNotify);
	DDX_Control(pDX, IDC_TPM_VCENTERALIGN, c_VCenterAlign);
	DDX_Control(pDX, IDC_TPM_TOPALIGN, c_TopAlign);
	DDX_Control(pDX, IDC_TPM_BOTTOMALIGN, c_BottomAlign);
	DDX_Control(pDX, IDC_TPM_VERTICAL, c_Vertical);
	DDX_Control(pDX, IDC_TPM_RIGHTBUTTON, c_RightButton);
	DDX_Control(pDX, IDC_TPM_RIGHTALIGN, c_RightAlign);
	DDX_Control(pDX, IDC_TPM_LEFTBUTTON, c_LeftButton);
	DDX_Control(pDX, IDC_TPM_LEFTALIGN, c_LeftAlign);
	DDX_Control(pDX, IDC_TPM_HORIZONTAL, c_Horizontal);
	DDX_Control(pDX, IDC_PREFERENCE, c_Preference);
	DDX_Control(pDX, IDC_TPM_CENTERALIGN, c_CenterAlign);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTPM, CPropertyPage)
	//{{AFX_MSG_MAP(CTPM)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONDOWN()
	ON_MESSAGE(UWM_LOGTRACKPOPUP, OnLogTrackPopup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTPM message handlers

/****************************************************************************
*                               CTPM::getFlags
* Result: DWORD
*       Flags as specified by the controls
****************************************************************************/

DWORD CTPM::getFlags()
    {
     DWORD flags = 0;
     if(c_RightButton.GetCheck())
	flags |= TPM_RIGHTBUTTON;

     if(c_CenterAlign.GetCheck())
	flags |= TPM_CENTERALIGN;
     else
     if(c_RightAlign.GetCheck())
	flags |= TPM_RIGHTALIGN;

     if(!Is3() && c_Vertical.GetCheck())
	flags |= TPM_VERTICAL;

     if(!Is3() && c_VCenterAlign.GetCheck())
	flags |= TPM_VCENTERALIGN;
     else
     if(!Is3() && c_BottomAlign.GetCheck())
	flags |= TPM_BOTTOMALIGN;

     if(!Is3() && c_NoNotify.GetCheck())
	flags |= TPM_NONOTIFY;

     if(!Is3() && c_ReturnCmd.GetCheck())
	flags |= TPM_RETURNCMD;
     
     return flags;
    }

BOOL CTPM::OnKillActive() 
{

 	GetParent()->SendMessage(UWM_SETFLAGS, getFlags());
	return CPropertyPage::OnKillActive();
}

BOOL CTPM::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
 	if(Is3())
	   { /* WINNT 3.5x */
	    c_ReturnCmd.EnableWindow(FALSE);
	    c_NoNotify.EnableWindow(FALSE);
	    c_Preference.EnableWindow(FALSE);
	    c_Vertical.EnableWindow(FALSE);
	    c_Horizontal.EnableWindow(FALSE);

	    c_TopAlign.EnableWindow(FALSE);
	    c_VCenterAlign.EnableWindow(FALSE);
	    c_BottomAlign.EnableWindow(FALSE);
	    c_VerticalAlignment.EnableWindow(FALSE);
	   } /* WINNT 3.5x */
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CTPM::OnSetActive() 
{
 	CMenuSheet * sheet = (CMenuSheet *)GetParent();
	
	// Note that the bit tests below are carefully chosen to avoid
	// the "0"-valued flags, e.g.,
	//		TPM_LEFTBUTTON
	//		TPM_LEFTALIGN
	//		TPM_TOPALIGN
	//		TPM_HORIZONTAL

	CheckRadioButton(IDC_TPM_LEFTBUTTON, IDC_TPM_RIGHTBUTTON, 
				(sheet->tpmFlags & TPM_RIGHTBUTTON) 
					? IDC_TPM_RIGHTBUTTON
					: IDC_TPM_LEFTBUTTON);
        CheckRadioButton(IDC_TPM_LEFTALIGN, IDC_TPM_RIGHTALIGN, 
			        (sheet->tpmFlags & TPM_CENTERALIGN 
					? IDC_TPM_CENTERALIGN
					: sheet->tpmFlags & TPM_RIGHTALIGN
						? IDC_TPM_RIGHTALIGN
						: IDC_TPM_LEFTALIGN));
        if(!Is3())
	   { /* 4.0 or higher */
	    CheckRadioButton(IDC_TPM_HORIZONTAL, IDC_TPM_VERTICAL,
				(sheet->tpmFlags & TPM_VERTICAL)
					? IDC_TPM_VERTICAL
					: IDC_TPM_HORIZONTAL);
            CheckRadioButton(IDC_TPM_TOPALIGN, IDC_TPM_BOTTOMALIGN, 
	    		(sheet->tpmFlags & TPM_BOTTOMALIGN) 
					? IDC_TPM_BOTTOMALIGN
					: (sheet->tpmFlags & TPM_VCENTERALIGN)
						? IDC_TPM_VCENTERALIGN
						: IDC_TPM_TOPALIGN);

            c_NoNotify.SetCheck(sheet->tpmFlags & TPM_NONOTIFY);
	    c_ReturnCmd.SetCheck(sheet->tpmFlags & TPM_RETURNCMD);
	   } /* 4.0 or higher */ 

	return CPropertyPage::OnSetActive();
}

void CTPM::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	
}

void CTPM::OnRButtonDown(UINT nFlags, CPoint point) 
{
 	GetParent()->SendMessage(UWM_TRACKPOPUPMENU, getFlags(), 
						MAKELONG(point.x, point.y));
	
	CPropertyPage::OnRButtonDown(nFlags, point);
}

/****************************************************************************
*                            CTPM::OnLogTrackPopup
* Inputs:
*       WPARAM: ignored
*	LPARAM lParam: (LPCTSTR) pointer to message to put into buffer
* Result: LRESULT
*       logically void, 0, always
* Effect: 
*       Logs the message to the logging window
****************************************************************************/

LRESULT CTPM::OnLogTrackPopup(WPARAM, LPARAM lParam)
    {
     c_Log.SetWindowText((LPCTSTR)lParam);
     return 0;
    }
