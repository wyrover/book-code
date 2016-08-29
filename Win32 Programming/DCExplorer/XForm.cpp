// XForm.cpp : implementation file
//

#include "stdafx.h"
#include "DCExplorer.h"

#include <math.h>
#include <stdlib.h>

#include "dcvals.h"
#include "FltEdit.h"
#include "MatSamp.h"
#include "mattext.h"
#include "matgraf.h"
#include "message.h"
#include "sysmsg.h"
#include "XForm.h"

#include "uwm.h"
#include "degrad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXForm property page

IMPLEMENT_DYNCREATE(CXForm, CPropertyPage)

CXForm::CXForm() : CPropertyPage(CXForm::IDD)
{
	//{{AFX_DATA_INIT(CXForm)
	//}}AFX_DATA_INIT
}

CXForm::~CXForm()
{
}

void CXForm::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXForm)
	DDX_Control(pDX, IDC_ATAN, c_ATan);
	DDX_Control(pDX, IDC_TAN, c_Tan);
	DDX_Control(pDX, IDC_2TAN, c_2Tan);
	DDX_Control(pDX, IDC_VSHEAR, c_VShear);
	DDX_Control(pDX, IDC_OPERATIONS, c_Operations);
	DDX_Control(pDX, IDC_HSHEAR, c_HShear);
	DDX_Control(pDX, IDC_ROTATE, c_Rotate);
	DDX_Control(pDX, IDC_SPINDY, c_SpinDY);
	DDX_Control(pDX, IDC_SPINDX, c_SpinDX);
	DDX_Control(pDX, IDC_SPIN22, c_Spin22);
	DDX_Control(pDX, IDC_SPIN21, c_Spin21);
	DDX_Control(pDX, IDC_SPIN12, c_Spin12);
	DDX_Control(pDX, IDC_SPIN11, c_Spin11);
	DDX_Control(pDX, IDC_BOTTOM, c_Bottom);
	DDX_Control(pDX, IDC_TOP, c_Top);
	DDX_Control(pDX, IDC_BASELINE, c_BaseLine);
	DDX_Control(pDX, IDC_ERROR, c_Error);
	DDX_Control(pDX, IDC_DEGREES_CAPTION, c_c_Degrees);
	DDX_Control(pDX, IDC_ADVANCED, c_Advanced);
	DDX_Control(pDX, IDC_XFORM_CAPTION, c_c_Xform);
	DDX_Control(pDX, IDC_TRIG_CAPTION, c_c_Trig);
	DDX_Control(pDX, IDC_TEXTSAMPLE, c_TextSample);
	DDX_Control(pDX, IDC_IDENTITY, c_Identity);
	DDX_Control(pDX, IDC_GRAPHICSAMPLE, c_GraphicSample);
	DDX_Control(pDX, IDC_COMPATIBLE, c_Compatible);
	DDX_Control(pDX, IDC_DY, c_dY);
	DDX_Control(pDX, IDC_DX, c_dX);
	DDX_Control(pDX, IDC_M22, c_M22);
	DDX_Control(pDX, IDC_M21, c_M21);
	DDX_Control(pDX, IDC_M12, c_M12);
	DDX_Control(pDX, IDC_M11, c_M11);
	DDX_Control(pDX, IDC_ASIN, c_ASin);
	DDX_Control(pDX, IDC_ACOS, c_ACos);
	DDX_Control(pDX, IDC_SIN, c_Sin);
	DDX_Control(pDX, IDC_COS, c_Cos);
	DDX_Control(pDX, IDC_VALUE, c_Value);
	DDX_Control(pDX, IDC_DEGREES, c_Degrees);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXForm, CPropertyPage)
	//{{AFX_MSG_MAP(CXForm)
	ON_BN_CLICKED(IDC_ACOS, OnAcos)
	ON_BN_CLICKED(IDC_ASIN, OnAsin)
	ON_BN_CLICKED(IDC_COS, OnCos)
	ON_BN_CLICKED(IDC_SIN, OnSin)
	ON_EN_CHANGE(IDC_DEGREES, OnChangeDegrees)
	ON_EN_CHANGE(IDC_VALUE, OnChangeValue)
	ON_BN_CLICKED(IDC_IDENTITY, OnIdentity)
	ON_EN_CHANGE(IDC_DX, OnChangeDx)
	ON_EN_CHANGE(IDC_DY, OnChangeDy)
	ON_EN_CHANGE(IDC_M11, OnChangeM11)
	ON_EN_CHANGE(IDC_M12, OnChangeM12)
	ON_EN_CHANGE(IDC_M21, OnChangeM21)
	ON_EN_CHANGE(IDC_M22, OnChangeM22)
	ON_BN_CLICKED(IDC_ADVANCED, OnAdvanced)
	ON_BN_CLICKED(IDC_COMPATIBLE, OnCompatible)
	ON_BN_CLICKED(IDC_BASELINE, OnBaseline)
	ON_BN_CLICKED(IDC_BOTTOM, OnBottom)
	ON_BN_CLICKED(IDC_TOP, OnTop)
	ON_BN_CLICKED(IDC_ROTATE, OnRotate)
	ON_BN_CLICKED(IDC_HSHEAR, OnHshear)
	ON_BN_CLICKED(IDC_VSHEAR, OnVshear)
	ON_BN_CLICKED(IDC_TAN, OnTan)
	ON_BN_CLICKED(IDC_2TAN, On2tan)
	ON_BN_CLICKED(IDC_ATAN, OnAtan)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UWM_UPDATE_ERROR, OnUpdateError)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CXForm message handlers

void CXForm::OnAcos() 
{
 FLOAT val = c_Value.GetVal();

 FLOAT deg = (FLOAT)rad_to_deg((FLOAT)acos(val));

 c_Degrees.SetVal(deg);
 enableControls();
}

void CXForm::OnAsin() 
{
 FLOAT val = c_Value.GetVal();

 FLOAT deg = (FLOAT)rad_to_deg((FLOAT)asin(val));

 c_Degrees.SetVal(deg);
 enableControls();
}

/****************************************************************************
*                           CXForm::enableControls
* Result: void
*       
* Effect: 
*       If the box is non-empty, enables the corresponding controls
****************************************************************************/

void CXForm::enableControls()
    {
     BOOL enable;
     BOOL advanced = c_Advanced.GetCheck();

     enable = !c_Value.IsBlank() > 0  && advanced;
     c_ASin.EnableWindow(enable);
     c_ACos.EnableWindow(enable);
     c_ATan.EnableWindow(enable);

     enable = !c_Degrees.IsBlank() > 0 && advanced;
     c_Sin.EnableWindow(enable);
     c_Cos.EnableWindow(enable);
     c_Tan.EnableWindow(enable);
     c_2Tan.EnableWindow(enable);
     c_Rotate.EnableWindow(enable);
     c_HShear.EnableWindow(enable);
     c_VShear.EnableWindow(enable);
     c_Operations.EnableWindow(enable);


     c_c_Xform.EnableWindow(advanced);
     c_c_Trig.EnableWindow(advanced);
     c_c_Degrees.EnableWindow(advanced);
     c_Degrees.EnableWindow(advanced);
     c_Value.EnableWindow(advanced);
     c_Identity.EnableWindow(advanced);
     c_M11.EnableWindow(advanced);
     c_M12.EnableWindow(advanced);
     c_M21.EnableWindow(advanced);
     c_M22.EnableWindow(advanced);
     c_dX.EnableWindow(advanced);
     c_dY.EnableWindow(advanced);
     c_TextSample.ShowWindow(advanced ? SW_SHOW : SW_HIDE);
     c_GraphicSample.ShowWindow(advanced ? SW_SHOW : SW_HIDE);
     c_Top.EnableWindow(advanced);
     c_BaseLine.EnableWindow(advanced);
     c_Bottom.EnableWindow(advanced);
     c_Spin11.EnableWindow(advanced);
     c_Spin21.EnableWindow(advanced);
     c_Spin12.EnableWindow(advanced);
     c_Spin22.EnableWindow(advanced);
     c_SpinDX.EnableWindow(advanced);
     c_SpinDY.EnableWindow(advanced);
    }

void CXForm::OnCos() 
{
 FLOAT deg = c_Degrees.GetVal();

 FLOAT val = (FLOAT)cos(deg_to_rad(deg));

 if(val < 0.0001f)
    val = 0.0f;

 c_Value.SetVal(val);

 enableControls();
}

void CXForm::OnSin() 
{
 FLOAT deg = c_Degrees.GetVal();

 FLOAT val = (FLOAT)sin(deg_to_rad(deg));

 if(val < 0.0001f)
    val = 0.0f;

 c_Value.SetVal(val);
 enableControls();
}

/****************************************************************************
*                             CXform::getAlign
* Result: int
*       Flag value of controls
****************************************************************************/

int CXForm::getAlign()
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

#if 0 // controls for this not yet implemented
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
#endif

     return flags;
    }

BOOL CXForm::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
 	int vertical = IDC_TOP;
	if((dcv->align & TA_BOTTOM) == TA_BOTTOM)
	   vertical = IDC_BOTTOM;
        else
	if((dcv->align & TA_BASELINE) == TA_BASELINE)
	   vertical = IDC_BASELINE;

	CheckRadioButton(IDC_COMPATIBLE, IDC_ADVANCED,
	  (dcv->graphicsmode == GM_COMPATIBLE ? IDC_COMPATIBLE
	  					: IDC_ADVANCED));

	c_Error.SetWindowText(NULL); // no error message

        matrixToControls(&dcv->matrix);
        
#define bounds 5000
	c_Spin11.SetRange(-bounds, bounds);
	c_Spin21.SetRange(-bounds, bounds);
	c_Spin22.SetRange(-bounds, bounds);
	c_Spin12.SetRange(-bounds, bounds);
	c_SpinDX.SetRange(-bounds, bounds);
	c_SpinDY.SetRange(-bounds, bounds);

 	enableControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CXForm::OnChangeDegrees() 
{
 enableControls();
	
}

void CXForm::OnChangeValue() 
{
 enableControls();
	
}

/****************************************************************************
*                                matrixToControls
* Inputs:
*	XFORM * matrix: Matrix to copy to controls
* Result: void
*       
* Effect: 
*       Copies the contents of the matrix to the controls
****************************************************************************/

void CXForm::matrixToControls(XFORM * matrix)
    {
     c_M11.SetVal(matrix->eM11);
     c_M12.SetVal(matrix->eM12);
     c_M21.SetVal(matrix->eM21);
     c_M22.SetVal(matrix->eM22);
     c_dX.SetVal(matrix->eDx);
     c_dY.SetVal(matrix->eDy);

    }

/****************************************************************************
*                          CXForm::controlsToMatrix
* Inputs:
*       XFORM * matrix: Matrix to which to transfer control values
* Result: void
*       
* Effect: 
*       Transfers controls to matrix
****************************************************************************/

void CXForm::controlsToMatrix(XFORM * matrix)
    {
     matrix->eM11 = c_M11.GetVal();
     matrix->eM12 = c_M12.GetVal();
     matrix->eM21 = c_M21.GetVal();
     matrix->eM22 = c_M22.GetVal();
     matrix->eDx  = c_dX.GetVal();
     matrix->eDy  = c_dY.GetVal();
    }

/****************************************************************************
*                            CXForm::updateSamples
* Result: void
*       
* Effect: 
*       Updates the two samples by taking the current matrix value and
*	storing it in their matrix member variable then forcing them to
*	redraw
****************************************************************************/

void CXForm::updateSamples()
    {
     // First, make sure we are not getting a gratuitous change from
     // the autobuddy initialization; if this happens, not all controls
     // are initialized, and we can lose big if we try to access them...

     if(c_M11.m_hWnd == NULL ||
	c_M12.m_hWnd == NULL ||
	c_M21.m_hWnd == NULL ||
	c_M22.m_hWnd == NULL ||
	c_dX.m_hWnd == NULL ||
	c_dY.m_hWnd == NULL)
		return;  // autobuddy startup anomaly.
     controlsToMatrix(&c_TextSample.matrix);
     controlsToMatrix(&c_GraphicSample.matrix);

     c_TextSample.align = getAlign();

     c_TextSample.InvalidateRect(NULL);
     c_GraphicSample.InvalidateRect(NULL);
    }

void CXForm::OnIdentity() 
{
 XFORM identity = {1.0f, 0.0f, 
 	           0.0f, 1.0f,
		   0.0f, 0.0f};

 matrixToControls(&identity);
 updateSamples();
}

void CXForm::OnOK() 
    {
     CPropertyPage::OnOK();

     dcv->lastpage = ((CPropertySheet *)GetParent())->GetActiveIndex();

     controlsToMatrix(&dcv->matrix);
     
     dcv->graphicsmode = c_Compatible.GetCheck() ? GM_COMPATIBLE : GM_ADVANCED;
    }

void CXForm::OnChangeDx() 
{
 updateSamples();
}

void CXForm::OnChangeDy() 
{
 updateSamples();
}

void CXForm::OnChangeM11() 
{
 updateSamples();
}

void CXForm::OnChangeM12() 
{
 updateSamples();
}

void CXForm::OnChangeM21() 
{
 updateSamples();
}

void CXForm::OnChangeM22() 
{
 updateSamples();
}

void CXForm::OnAdvanced() 
{
 enableControls();
}

void CXForm::OnCompatible() 
{
 enableControls();
}

void CXForm::OnBaseline() 
{
 updateSamples();
	
}

void CXForm::OnBottom() 
{
 updateSamples();
	
}

void CXForm::OnTop() 
{
 updateSamples();
	
}

void CXForm::OnRotate() 
{
 /* 
    The rotation in degrees is computed as
    |  cos x  sin x |
    | -sin x  cos x |
    | 0	      0     |
 */

 double theta = deg_to_rad(c_Degrees.GetVal());
 XFORM matrix;

 matrix.eM11 = (float)cos(theta);
 matrix.eM12 = (float)sin(theta);
 matrix.eM21 = (float)-sin(theta);
 matrix.eM22 = (float)cos(theta);
 matrix.eDx  = 0.0f;
 matrix.eDy  = 0.0f;
 
 matrixToControls(&matrix);

 updateSamples();
	
}

/****************************************************************************
*                            CXForm::OnUpdateError
* Inputs:
*       WPARAM wParam: Error message code, or 0 if no error
*	LPARAM: unused
* Result: LRESULT (void)
*       0, always
* Effect: 
*       Updates the error message found in the error sample
****************************************************************************/

LRESULT CXForm::OnUpdateError(WPARAM wParam, LPARAM)
    {
     c_Error.SetWindowText(wParam);
     return 0;  // logically, void
    }

void CXForm::OnHshear() 
{
 /* 
    The pure horizontal shear in degrees is computed as
    | 1       0 |
    | tan x   1 |
    | 0	      0 |

    We only add the shear factor in and do not change scaling or offset
 */

 double theta = deg_to_rad(c_Degrees.GetVal());
 XFORM matrix;

 controlsToMatrix(&matrix);
 matrix.eM21 = (float)tan(theta);
 matrix.eM12 = 0.0f;
 
 matrixToControls(&matrix);

 updateSamples();
	
}

void CXForm::OnVshear() 
{
 /* 
    The pure vertical shear in degrees is computed as
    | 1       tan x |
    | 0       1     |
    | 0	      0     |

    We only add the shear factor in and do not change scaling or offset
 */

 double theta = deg_to_rad(c_Degrees.GetVal());
 XFORM matrix;

 controlsToMatrix(&matrix);
 matrix.eM12 = (float)tan(theta);
 matrix.eM21 = 0.0f;
 
 matrixToControls(&matrix);

 updateSamples();
}

void CXForm::OnTan() 
{
 FLOAT deg = c_Degrees.GetVal();

 FLOAT val = (FLOAT)tan(deg_to_rad(deg));

 if(val < 0.0001f)
    val = 0.0f;

 c_Value.SetVal(val);
 enableControls();
}

void CXForm::On2tan() 
{
 FLOAT deg = c_Degrees.GetVal();

 FLOAT val = (FLOAT)(2.0 * tan(deg_to_rad(deg)));

 if(val < 0.0001f)
    val = 0.0f;

 c_Value.SetVal(val);
 enableControls();
}

void CXForm::OnAtan() 
{
 FLOAT deg = c_Degrees.GetVal();

 FLOAT val = (FLOAT)(atan(deg_to_rad(deg)));

 if(val < 0.0001f)
    val = 0.0f;

 c_Value.SetVal(val);
 enableControls();
}
