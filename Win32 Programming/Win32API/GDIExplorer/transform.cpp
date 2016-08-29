// transform.cpp : implementation file
//

#include "stdafx.h"
#include "afxcmn.h"
#include "GDIExplorer.h"
#include "ctl.h"
#include "xform.h"
#include "mdisp.h"
#include "transform.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CTransform dialog


CTransform::CTransform(CWnd* pParent /*=NULL*/)
        : CDialog(CTransform::IDD, pParent)
{
        //{{AFX_DATA_INIT(CTransform)
        //}}AFX_DATA_INIT
}

void CTransform::DoDataExchange(CDataExchange* pDX)
{
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CTransform)
        DDX_Control(pDX, IDC_STACK6, c_Stack6);
        DDX_Control(pDX, IDC_STACK5, c_Stack5);
        DDX_Control(pDX, IDC_STACK4, c_Stack4);
        DDX_Control(pDX, IDC_STACK3, c_Stack3);
        DDX_Control(pDX, IDC_STACK2, c_Stack2);
        DDX_Control(pDX, IDC_STACK1, c_Stack1);
        DDX_Control(pDX, IDC_STACK0, c_Stack0);
        DDX_Control(pDX, IDC_CTM, c_CTM);
        DDX_Control(pDX, IDC_M22s, c_M22s);
        DDX_Control(pDX, IDC_M22c, c_M22c);
        DDX_Control(pDX, IDC_M22, c_M22);
        DDX_Control(pDX, IDC_M21s, c_M21s);
        DDX_Control(pDX, IDC_M21c, c_M21c);
        DDX_Control(pDX, IDC_M21, c_M21);
        DDX_Control(pDX, IDC_DX, c_DX);
        DDX_Control(pDX, IDC_DY, c_DY);
        DDX_Control(pDX, IDC_M12s, c_M12s);
        DDX_Control(pDX, IDC_M12c, c_M12c);
        DDX_Control(pDX, IDC_M12, c_M12);
        DDX_Control(pDX, IDC_M11, c_M11);
        DDX_Control(pDX, IDC_M11c, c_M11c);
        DDX_Control(pDX, IDC_M11s, c_M11s);
        DDX_Control(pDX, IDC_DXs, c_DXs);
        DDX_Control(pDX, IDC_DYs, c_DYs);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTransform, CDialog)
        //{{AFX_MSG_MAP(CTransform)
        ON_EN_CHANGE(IDC_DX, OnChangeDx)
        ON_EN_CHANGE(IDC_DY, OnChangeDy)
        ON_BN_CLICKED(IDC_IDENTITY, OnIdentity)
        ON_EN_CHANGE(IDC_M11, OnChangeM11)
        ON_EN_CHANGE(IDC_M12, OnChangeM12)
        ON_EN_CHANGE(IDC_M21, OnChangeM21)
        ON_EN_CHANGE(IDC_M22, OnChangeM22)
        ON_BN_CLICKED(IDC_ROTATE, OnRotate)
        ON_CBN_SELCHANGE(IDC_M11c, OnSelchangeM11c)
        ON_CBN_SELCHANGE(IDC_M12c, OnSelchangeM12c)
        ON_CBN_SELCHANGE(IDC_M21c, OnSelchangeM21c)
        ON_CBN_SELCHANGE(IDC_M22c, OnSelchangeM22c)
        ON_BN_CLICKED(IDC_CTMx, OnCTMx)
        ON_BN_CLICKED(IDC_XCTM, OnXctm)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransform message handlers

void CTransform::OnChangeDx() 
{
        // TODO: Add your control notification handler code here
        
}

void CTransform::OnChangeDy() 
{
        // TODO: Add your control notification handler code here
        
}

void CTransform::OnIdentity() 
{
 matrix.SetIdentity();
        
}

void CTransform::OnChangeM11() 
{
        // TODO: Add your control notification handler code here
        
}

void CTransform::OnChangeM12() 
{
        // TODO: Add your control notification handler code here
        
}

void CTransform::OnChangeM21() 
{
        // TODO: Add your control notification handler code here
        
}

void CTransform::OnChangeM22() 
{
        // TODO: Add your control notification handler code here
        
}

void CTransform::OnRotate() 
{
        // TODO: Add your control notification handler code here
        
}

void CTransform::OnSelchangeM11c() 
{
        // TODO: Add your control notification handler code here
        
}

void CTransform::OnSelchangeM12c() 
{
        // TODO: Add your control notification handler code here
        
}

void CTransform::OnSelchangeM21c() 
{
        // TODO: Add your control notification handler code here
        
}

void CTransform::OnSelchangeM22c() 
{
        // TODO: Add your control notification handler code here
        
}

void CTransform::OnCTMx() 
{
        // TODO: Add your control notification handler code here
        
}

void CTransform::OnXctm() 
{
        // TODO: Add your control notification handler code here
        
}
