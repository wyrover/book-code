// 3D.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "3D.h"
#include "is95.h"
#include "copy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3D property page

IMPLEMENT_DYNCREATE(C3D, CPropertyPage)

C3D::C3D() : CPropertyPage(C3D::IDD)
{
        //{{AFX_DATA_INIT(C3D)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

C3D::~C3D()
{
}

void C3D::DoDataExchange(CDataExchange* pDX)
{
        CPropertyPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(C3D)
        DDX_Control(pDX, IDC_ETCHED, c_Etched);
        DDX_Control(pDX, IDC_3D, c_3D);
        DDX_Control(pDX, IDC_COPYFRAME, c_CopyFrame);
        //}}AFX_DATA_MAP

        DDX_Control(pDX, IDC_WHITERECT, Controls_3D.c_WhiteRect);
        DDX_Control(pDX, IDC_WHITEFRAME, Controls_3D.c_WhiteFrame);
        DDX_Control(pDX, IDC_GRAYRECT, Controls_3D.c_GrayRect);
        DDX_Control(pDX, IDC_GRAYFRAME, Controls_3D.c_GrayFrame);
        DDX_Control(pDX, IDC_ETCHEDRECT, Controls_3D.c_EtchedRect);
        DDX_Control(pDX, IDC_ETCHEDFRAME, Controls_3D.c_EtchedFrame);
        DDX_Control(pDX, IDC_BLACKRECT, Controls_3D.c_BlackRect);
        DDX_Control(pDX, IDC_BLACKFRAME, Controls_3D.c_BlackFrame);
}


BEGIN_MESSAGE_MAP(C3D, CPropertyPage)
        //{{AFX_MSG_MAP(C3D)
        ON_BN_CLICKED(IDC_3D, On3d)
        ON_BN_CLICKED(IDC_COPY, OnCopy)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3D message handlers


/****************************************************************************
*                              C3D::showWindows
* Inputs:
*       CStatic * array[]: Array of pointers to window objects
*       BOOL show: TRUE to show, FALSE to hide
* Result: void
*       
* Effect: 
*       Shows or hides the controls in the array
****************************************************************************/

void C3D::showWindows(CStatic * array[], BOOL show)
    {
     for(int i = 0; i < ControlCount; i++)
        array[i]->ShowWindow(show ? SW_SHOW : SW_HIDE);
    }

void C3D::On3d() 
{
 showWindows(Array3D, c_3D.GetCheck());
 showWindows(ArrayFlat, !c_3D.GetCheck());
}

void C3D::OnCopy() 
{
 copyToClipboard(&c_CopyFrame);
}


/****************************************************************************
*                            C3D::controlsToArray
* Inputs:
*       CStatic * array[]: Array of pointers to objects
*       ControlSet * ctls: Control set structure
* Result: void
*       
* Effect: 
*       Makes the control set into an array.  Cleaner.
****************************************************************************/

void C3D::controlsToArray(CStatic * array[], ControlSet * ctls)
    {
     array[0] = &ctls->c_WhiteRect;
     array[1] = &ctls->c_WhiteFrame;
     array[2] = &ctls->c_GrayRect;
     array[3] = &ctls->c_GrayFrame;
     array[4] = &ctls->c_EtchedRect;
     array[5] = &ctls->c_EtchedFrame;
     array[6] = &ctls->c_BlackRect;
     array[7] = &ctls->c_BlackFrame;
    }

/****************************************************************************
*                               C3D::evilClone
* Inputs:
*       CStatic * flat: Flat window to be created
*       CStatic * c3D: 3-D window to clone
* Result: void
*       
* Effect: 
*       Makes an (invisible) copy of the 3D window
****************************************************************************/

void C3D::evilClone(CStatic * flat, CStatic * c3D)
    {
     DWORD style = ::GetWindowLong(c3D->m_hWnd, GWL_STYLE);
     CRect r;

     c3D->GetWindowRect(&r);
     ScreenToClient(&r);  // convert to client coordinates

     flat->Create(NULL, style & ~WS_VISIBLE, r, this);
    }

/****************************************************************************
*                              C3D::makeEvilTwin
* Inputs:
*       CStatic * flat[]: Array of pointers to flat controls
*       CStatic * c3D[]: Array of pointers to 3D controls
* Result: void
*       
* Effect: 
*       For each 3D control, creates a flat control of the same style
****************************************************************************/

void C3D::makeEvilTwins(CStatic * flat[], CStatic * c3D[])
    {
     for(int i = 0; i < ControlCount; i++)
        evilClone(flat[i], c3D[i]);
    }

BOOL C3D::OnInitDialog() 
{
        CPropertyPage::OnInitDialog();
        
        controlsToArray(Array3D, &Controls_3D);
        controlsToArray(ArrayFlat, &Controls_Flat);
        makeEvilTwins(ArrayFlat, Array3D);
        c_3D.SetCheck(BST_CHECKED);
        
        if(Is3())
           c_Etched.EnableWindow(FALSE);

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}
