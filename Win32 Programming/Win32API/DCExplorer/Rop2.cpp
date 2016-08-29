// Rop2.cpp : implementation file
//

#include "stdafx.h"
#include "DCExplorer.h"
#include "sample.h"
#include "idcombo.h"
#include "ropcodes.h"
#include "Rop2.h"
#include "uwm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRop2 dialog


CRop2::CRop2(CWnd* pParent /*=NULL*/)
	: CDialog(CRop2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRop2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CRop2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRop2)
	DDX_Control(pDX, IDC_RGB, c_RGB);
	DDX_Control(pDX, IDC_ROP2, c_Rop2);
	DDX_Control(pDX, IDC_TOP, c_Top);
	DDX_Control(pDX, IDC_LEFT, c_Left);
	DDX_Control(pDX, IDC_SAMPLE, c_Sample);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRop2, CDialog)
	//{{AFX_MSG_MAP(CRop2)
	ON_CBN_SELENDOK(IDC_ROP2, OnSelendokRop2)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(UWM_COLOR_CHANGE, OnColorChange)
	ON_MESSAGE(UWM_COLOR_CLEAR,  OnColorClear)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRop2 message handlers

BOOL CRop2::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
 	SetWindowText(ropname);

	c_Sample.rop = rop;
	c_Sample.hv |= hv_2v;  // use ROP2 for this one's vertical
	
	c_Left.rop = R2_COPYPEN;
	c_Left.hv  = hv_H;

	c_Top.rop  = R2_COPYPEN;
	c_Top.hv   = hv_V;
	
	c_Rop2.Load(rop);

	// The following bizarre code handles making the windows adjacent
	// which is not guaranteed because of how dialog box units quantize

	CRect sample;
	CRect left;
	CRect top;

	c_Sample.GetWindowRect(&sample);
	c_Left.GetWindowRect(&left);
	c_Top.GetWindowRect(&top);

	ScreenToClient(&sample);
	ScreenToClient(&left);
	ScreenToClient(&top);

	c_Left.SetWindowPos(NULL, sample.left - left.Width(),
				  sample.top,
				  0, 0,
				  SWP_NOSIZE | SWP_NOZORDER);
	c_Top.SetWindowPos(NULL,  sample.left,
				  sample.top - top.Height(),
				  0, 0,
				  SWP_NOSIZE | SWP_NOZORDER);

        c_RGB.SetWindowText(_T(""));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CRop2::OnSelendokRop2() 
{
 c_Sample.rop = c_Rop2.GetCurItem();

 CString s;
 c_Rop2.GetLBText(c_Rop2.GetCurSel(), s);
 SetWindowText(s);
 c_Sample.InvalidateRect(NULL);

}

/****************************************************************************
*                            CRop2::OnColorChange
* Inputs:
*       WPARAM wParam: (COLORREF) new color
*	LPARAM: unused
* Result: LRESULT (logically, void)
*       0, always
* Effect: 
*       Updates the color display
****************************************************************************/

LRESULT CRop2::OnColorChange(WPARAM wParam, LPARAM)
    {
     CString s;

     s.Format(_T("RGB(%03d, %03d, %03d)"), GetRValue(wParam),
     					   GetGValue(wParam),
					   GetBValue(wParam));
     c_RGB.SetWindowText(s);
     return 0;
    }

/****************************************************************************
*                             CRop2::OnColorClear
* Inputs:
*       WPARAM: unused
*	LPARAM: unused
* Result: LRESULT (logically void)
*       0, always
* Effect: 
*       Clears the color display
****************************************************************************/

LRESULT CRop2::OnColorClear(WPARAM, LPARAM)
    {
     c_RGB.SetWindowText(_T(""));
     return 0;
    }

void CRop2::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CRop2::OnOK() 
{
 rop = c_Rop2.GetCurItem();
 CDialog::OnOK();
}

void CRop2::OnMouseMove(UINT nFlags, CPoint point) 
{
 c_Sample.MouseMove(nFlags, point);
 c_Left.MouseMove(nFlags, point);
 c_Top.MouseMove(nFlags, point);
	
	CDialog::OnMouseMove(nFlags, point);
}
