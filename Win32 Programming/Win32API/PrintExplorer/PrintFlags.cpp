// PrintFlags.cpp : implementation file
//

#include "stdafx.h"
#include <dlgs.h>

#include "PrintExplorer.h"
#include "numericEdit.h"
#include "commerr.h"
#include "PrintFlags.h"
#include "printdlg.h"
#include "printhook.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPrintFlags property page

IMPLEMENT_DYNCREATE(CPrintFlags, CPropertyPage)

CPrintFlags::CPrintFlags() : CPropertyPage(CPrintFlags::IDD)
{
	//{{AFX_DATA_INIT(CPrintFlags)
	//}}AFX_DATA_INIT
}

CPrintFlags::~CPrintFlags()
{
}

void CPrintFlags::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrintFlags)
	DDX_Control(pDX, IDC_PD_ENABLEPRINTTEMPLATE, c_EnablePrintTemplate);
	DDX_Control(pDX, IDC_PD_ENABLEPRINTHOOK, c_EnablePrintHook);
	DDX_Control(pDX, IDC_CUSTOMDLG, c_CustomDlg);
	DDX_Control(pDX, IDC_RESULT2, c_Result);
	DDX_Control(pDX, IDC_PD_SELECTION, c_PD_Selection);
	DDX_Control(pDX, IDC_PD_PAGENUMS, c_PD_PageNums);
	DDX_Control(pDX, IDC_PD_ALLPAGES, c_PD_AllPages);
	DDX_Control(pDX, IDC_PD_RETURNIC, c_PD_ReturnIC);
	DDX_Control(pDX, IDC_PD_RETURNDC, c_PD_ReturnDC);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrintFlags, CPropertyPage)
	//{{AFX_MSG_MAP(CPrintFlags)
	ON_BN_CLICKED(IDC_PD_RETURNDC, OnPdReturndc)
	ON_BN_CLICKED(IDC_PD_RETURNIC, OnPdReturnic)
	ON_BN_CLICKED(IDC_PD_ALLPAGES, OnPdAllpages)
	ON_BN_CLICKED(IDC_PD_SELECTION, OnPdSelection)
	ON_BN_CLICKED(IDC_PD_PAGENUMS, OnPdPagenums)
	ON_BN_CLICKED(IDC_PRINTDLG2, OnPrintdlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CUSTOMDLG, OnCustomdlg)
	ON_BN_CLICKED(IDC_PD_ENABLEPRINTHOOK, OnPdEnableprinthook)
	ON_BN_CLICKED(IDC_PD_ENABLEPRINTTEMPLATE, OnPdEnableprinttemplate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static struct {
	int id;
	DWORD flag;
	       } bits[] = {
	    { IDC_PD_COLLATE,  PD_COLLATE  },
            { IDC_PD_DISABLEPRINTTOFILE, PD_DISABLEPRINTTOFILE },
	    { IDC_PD_ENABLEPRINTHOOK, PD_ENABLEPRINTHOOK },
	    { IDC_PD_ENABLEPRINTTEMPLATE, PD_ENABLEPRINTTEMPLATE},
	    { IDC_PD_ENABLEPRINTTEMPLATEHANDLE, PD_ENABLEPRINTTEMPLATEHANDLE },
	    { IDC_PD_ENABLESETUPHOOK, PD_ENABLESETUPHOOK },
	    { IDC_PD_ENABLESETUPTEMPLATE, PD_ENABLESETUPTEMPLATE },
	    { IDC_PD_HIDEPRINTTOFILE, PD_HIDEPRINTTOFILE },
	    { IDC_PD_NOPAGENUMS, PD_NOPAGENUMS },
	    { IDC_PD_NOSELECTION, PD_NOSELECTION },
	    { IDC_PD_NOWARNING, PD_NOWARNING },
	    { IDC_PD_PAGENUMS, PD_PAGENUMS },
	    { IDC_PD_PRINTSETUP, PD_PRINTSETUP },
	    { IDC_PD_PRINTTOFILE, PD_PRINTTOFILE },
	    { IDC_PD_RETURNDC, PD_RETURNDC },
	    { IDC_PD_RETURNDEFAULT, PD_RETURNDEFAULT },
	    { IDC_PD_RETURNIC, PD_RETURNIC },
	    { IDC_PD_SELECTION, PD_SELECTION }, 
	    { IDC_PD_SHOWHELP, PD_SHOWHELP },
	    { IDC_PD_USEDEVMODECOPIES, PD_USEDEVMODECOPIES },
	    { 0, 0} // EOT
			  };


void CPrintFlags::pd_to_controls()
    {
     int i;
     for(i = 0; bits[i].id != 0; i++)
        { /* process each */
	 ((CButton *)GetDlgItem(bits[i].id))->SetCheck(
	 	bits[i].flag & pd->Flags ? BST_CHECKED : BST_UNCHECKED);
	} /* process each */
     c_PD_AllPages.SetCheck( (pd->Flags & (PD_SELECTION | PD_PAGENUMS))
     				? BST_UNCHECKED
				: BST_CHECKED);
     c_Result.SetWindowText(*result);

     int ph = pd->Flags & (PD_ENABLEPRINTHOOK | PD_ENABLEPRINTTEMPLATE);
     if(ph == (PD_ENABLEPRINTHOOK | PD_ENABLEPRINTTEMPLATE))
	c_CustomDlg.SetCheck(BST_CHECKED);
     else
     if(ph == 0)
	c_CustomDlg.SetCheck(BST_UNCHECKED);
     else
	c_CustomDlg.SetCheck(BST_INDETERMINATE);  // 3-state indicator of incomplete spec
    }

/****************************************************************************
*                         CPrintFlags::controls_to_pd
* Result: void
*       
* Effect: 
*       Transfers controls to pd fields
****************************************************************************/

void CPrintFlags::controls_to_pd()
    {
     int i;
     for(i = 0; bits[i].id != 0; i++)
        { /* process each */
	 CButton * button = (CButton  *)GetDlgItem(bits[i].id);
	 if(button->GetCheck())
	    pd->Flags |= bits[i].flag;
	 else
	    pd->Flags &= ~bits[i].flag;
	} /* process each */
     c_PD_AllPages.SetCheck(pd->Flags & (PD_PAGENUMS | PD_SELECTION)
     			? BST_CHECKED : BST_UNCHECKED);

     if(pd->Flags & (PD_ENABLEPRINTTEMPLATE))
        { /* has template */
	 pd->lpPrintTemplateName = MAKEINTRESOURCE(PRINTDLGORD);
	 pd->hInstance = AfxGetInstanceHandle();
	} /* has template */
     if(pd->Flags & (PD_ENABLEPRINTHOOK))
	pd->lpfnPrintHook = printHook;
    }

/////////////////////////////////////////////////////////////////////////////
// CPrintFlags message handlers

BOOL CPrintFlags::OnSetActive() 
{
 	pd_to_controls();
	
	return CPropertyPage::OnSetActive();
}

BOOL CPrintFlags::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
 	pd_to_controls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPrintFlags::OnKillActive() 
{
 	controls_to_pd();
	
	return CPropertyPage::OnKillActive();
}

void CPrintFlags::OnPdReturndc() 
{
 if(c_PD_ReturnDC.GetCheck())
    c_PD_ReturnIC.SetCheck(BST_UNCHECKED);
	
}

void CPrintFlags::OnPdReturnic() 
{
 if(c_PD_ReturnIC.GetCheck())
    c_PD_ReturnDC.SetCheck(BST_UNCHECKED);
	
}

void CPrintFlags::OnPdAllpages() 
{
 if(c_PD_AllPages.GetCheck())
    { /* all pages */
     c_PD_Selection.SetCheck(BST_UNCHECKED);
     c_PD_PageNums.SetCheck(BST_UNCHECKED);
    } /* all pages */
	
}

void CPrintFlags::OnPdSelection() 
{
 if(c_PD_Selection.GetCheck())
    { /* has PageNums */
     c_PD_PageNums.SetCheck(BST_UNCHECKED);
     c_PD_AllPages.SetCheck(BST_UNCHECKED);
    } /* has PageNums */
	
}

void CPrintFlags::OnPdPagenums() 
{
 if(c_PD_PageNums.GetCheck())
    { /* has PageNums */
     c_PD_Selection.SetCheck(BST_UNCHECKED);
     c_PD_AllPages.SetCheck(BST_UNCHECKED);
    } /* has PageNums */
	
}

void CPrintFlags::OnPrintdlg() 
    {
     controls_to_pd();

     CPrintDlg printdlg(pd);
     if(printdlg.DoModal())
	*result = 0;
     else
        { /* error */
	 *result = CommDlgExtendedError();
	 MessageBeep(0);
	} /* error */

     pd_to_controls();
    }

HBRUSH CPrintFlags::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(nCtlColor == CTLCOLOR_EDIT && pWnd->m_hWnd == c_Result.m_hWnd)
	   { /* result */
	    return ((CCommErr *)pWnd)->OnCtlColor(pDC);
	   } /* result */

	return hbr;
}


void CPrintFlags::OnCustomdlg() 
{
 if(c_CustomDlg.GetCheck() == BST_INDETERMINATE) 
    { /* third state */
     c_CustomDlg.SetCheck(BST_UNCHECKED);  // state 2 goes to state 0 on toggle
    } /* third state */

 if(c_CustomDlg.GetCheck())
    { /* use custom dlg */
     c_EnablePrintTemplate.SetCheck(BST_CHECKED);
     c_EnablePrintHook.SetCheck(BST_CHECKED);
    } /* use custom dlg */
 else
    { /* no custom dlg */
     c_EnablePrintTemplate.SetCheck(BST_UNCHECKED);
     c_EnablePrintHook.SetCheck(BST_UNCHECKED);
    } /* no custom dlg */
}

void CPrintFlags::OnPdEnableprinthook() 
{
 c_EnablePrintTemplate.SetCheck(c_EnablePrintHook.GetCheck());
}

void CPrintFlags::OnPdEnableprinttemplate() 
{
 c_EnablePrintHook.SetCheck(c_EnablePrintTemplate.GetCheck());
}
