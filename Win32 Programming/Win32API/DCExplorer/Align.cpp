// Align.cpp : implementation file
//

#include "stdafx.h"
#include "DCExplorer.h"
#include "is95.h"
#include "AlgnSamp.h"
#include "dcvals.h"
#include "Align.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlign property page

IMPLEMENT_DYNCREATE(CAlign, CPropertyPage)

CAlign::CAlign() : CPropertyPage(CAlign::IDD)
{
	//{{AFX_DATA_INIT(CAlign)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CAlign::~CAlign()
{
}

void CAlign::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlign)
	DDX_Control(pDX, IDC_SAMPLE, c_Sample);
	DDX_Control(pDX, IDC_RTLREADING, c_RTLReading);
	DDX_Control(pDX, IDC_RIGHT, c_Right);
	DDX_Control(pDX, IDC_CENTER, c_Center);
	DDX_Control(pDX, IDC_BOTTOM, c_Bottom);
	DDX_Control(pDX, IDC_BASELINE, c_BaseLine);
	DDX_Control(pDX, IDC_UPDATECP, c_UpdateCP);
	DDX_Control(pDX, IDC_TOP, c_Top);
	DDX_Control(pDX, IDC_LEFT, c_Left);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlign, CPropertyPage)
	//{{AFX_MSG_MAP(CAlign)
	ON_BN_CLICKED(IDC_TOP, OnTop)
	ON_BN_CLICKED(IDC_LEFT, OnLeft)
	ON_BN_CLICKED(IDC_RIGHT, OnRight)
	ON_BN_CLICKED(IDC_CENTER, OnCenter)
	ON_BN_CLICKED(IDC_BOTTOM, OnBottom)
	ON_BN_CLICKED(IDC_RTLREADING, OnRtlreading)
	ON_BN_CLICKED(IDC_UPDATECP, OnUpdatecp)
	ON_BN_CLICKED(IDC_BASELINE, OnBaseline)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlign message handlers


/****************************************************************************
*                             CAlign::getAlign
* Result: int
*       Flag value of controls
****************************************************************************/

int CAlign::getAlign()
    {
     int flags = 0;

     if(c_Top.GetCheck())
	flags |= TA_TOP;
     else
     if(c_Bottom.GetCheck())
	flags |= TA_BOTTOM;
     else
     if(c_BaseLine.GetCheck())
	flags |= TA_BASELINE;

     if(c_Left.GetCheck())
	flags |= TA_LEFT;
     else
     if(c_Center.GetCheck())
	flags |= TA_CENTER;
     else
     if(c_Right.GetCheck())
	flags |= TA_RIGHT;

     if(c_UpdateCP.GetCheck())
	flags |= TA_UPDATECP;

     if(!Is3())
	if(c_RTLReading.GetCheck())
	   flags |= TA_RTLREADING;


     return flags;
    }

void CAlign::OnOK() 
{
	dcv->align = getAlign();
 	dcv->lastpage = ((CPropertySheet *)GetParent())->GetActiveIndex();
	
	CPropertyPage::OnOK();
}

/****************************************************************************
*                            CAlign::updateSample
* Result: void
*       
* Effect: 
*       Copies the current control values into the 'align' member of the
*	sample object, then forces the sample to redraw
****************************************************************************/

void CAlign::updateSample()
    {
     c_Sample.align = getAlign();
     c_Sample.InvalidateRect(NULL);
    }

BOOL CAlign::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
// These flag values were determined by carefully reading the
// definitions in wingdi.h.  There is no other documented way to discover 
// the correct way to check the flags.

#define TA_VERT_FLAGS TA_BASELINE
#define TA_HORZ_FLAGS TA_CENTER

 	int vertical = IDC_TOP;
	if((dcv->align & TA_VERT_FLAGS) == TA_BOTTOM)
	   vertical = IDC_BOTTOM;
        else
	if((dcv->align & TA_VERT_FLAGS) == TA_BASELINE)
	   vertical = IDC_BASELINE;

	CheckRadioButton(IDC_TOP, IDC_BOTTOM, vertical);

	int horizontal = IDC_LEFT;
	if((dcv->align & TA_HORZ_FLAGS) == TA_RIGHT)
	   horizontal = IDC_RIGHT;
        else
	if((dcv->align & TA_HORZ_FLAGS) == TA_CENTER)
	   horizontal = IDC_CENTER;

	CheckRadioButton(IDC_LEFT, IDC_RIGHT, horizontal);

	c_UpdateCP.SetCheck(dcv->align & TA_UPDATECP);

	
	c_RTLReading.EnableWindow(!Is3());

	if(!Is3())
	   c_RTLReading.SetCheck(dcv->align & TA_RTLREADING);

	updateSample();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAlign::OnTop() 
{
 updateSample();
	
}

void CAlign::OnLeft() 
{
 updateSample();
}

void CAlign::OnRight() 
{
 updateSample();
}

void CAlign::OnCenter() 
{
 updateSample();
}

void CAlign::OnBottom() 
{
 updateSample();
}

void CAlign::OnRtlreading() 
{
 updateSample();
}

void CAlign::OnUpdatecp() 
{
 updateSample();
}

void CAlign::OnBaseline() 
{
 updateSample();
}
