// Styles.cpp : implementation file
//

#include "stdafx.h"
#include "WndExp.h"
#include "numericedit.h"
#include "Styles.h"
#include "ctlmap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static controlmap stylemap[] = {
                { IDC_WS_BORDER,       WS_BORDER },
                { IDC_WS_CAPTION,      WS_CAPTION },
                { IDC_WS_CHILD,        WS_CHILD },
                { IDC_WS_CLIPCHILDREN, WS_CLIPCHILDREN },
                { IDC_WS_CLIPSIBLINGS, WS_CLIPSIBLINGS },
                { IDC_WS_DLGFRAME,     WS_DLGFRAME },
                { IDC_WS_GROUP,        WS_GROUP },
                { IDC_WS_HSCROLL,      WS_HSCROLL },
                { IDC_WS_MAXIMIZE,     WS_MAXIMIZE },
                { IDC_WS_MAXIMIZEBOX,  WS_MAXIMIZEBOX },
                { IDC_WS_MINIMIZE,     WS_MINIMIZE },
                { IDC_WS_MINIMIZEBOX,  WS_MINIMIZEBOX },
                { IDC_WS_OVERLAPPED,   WS_OVERLAPPED },
                { IDC_WS_POPUP,        WS_POPUP },
                { IDC_WS_SYSMENU,      WS_SYSMENU },
                { IDC_WS_TABSTOP,      WS_TABSTOP },
                { IDC_WS_THICKFRAME,   WS_THICKFRAME },
                { IDC_WS_VISIBLE,      WS_VISIBLE },
                { IDC_WS_VSCROLL,      WS_VSCROLL },
		{ 0, 0 } // EOT
			       };

/////////////////////////////////////////////////////////////////////////////
// CStyles property page

IMPLEMENT_DYNCREATE(CStyles, CPropertyPage)

CStyles::CStyles() : CPropertyPage(CStyles::IDD)
{
	//{{AFX_DATA_INIT(CStyles)
	//}}AFX_DATA_INIT
}

CStyles::~CStyles()
{
}

void CStyles::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStyles)
	DDX_Control(pDX, IDC_FLAGS, c_Flags);
	DDX_Control(pDX, IDC_WS_POPUP, c_WS_POPUP);
	DDX_Control(pDX, IDC_WS_OVERLAPPED, c_WS_OVERLAPPED);
	DDX_Control(pDX, IDC_WS_CHILD, c_WS_CHILD);
	DDX_Control(pDX, IDC_WS_POPUPWINDOW, c_WS_POPUPWINDOW);
	DDX_Control(pDX, IDC_WS_OVERLAPPEDWINDOW, c_WS_OVERLAPPEDWINDOW);
	DDX_Control(pDX, IDC_WS_CHILDWINDOW, c_WS_CHILDWINDOW);
	DDX_Control(pDX, IDC_WS_VSCROLL, c_WS_VSCROLL);
	DDX_Control(pDX, IDC_WS_VISIBLE, c_WS_VISIBLE);
	DDX_Control(pDX, IDC_WS_THICKFRAME, c_WS_THICKFRAME);
	DDX_Control(pDX, IDC_WS_TABSTOP, c_WS_TABSTOP);
	DDX_Control(pDX, IDC_WS_SYSMENU, c_WS_SYSMENU);
	DDX_Control(pDX, IDC_WS_MINIMIZEBOX, c_WS_MINIMIZEBOX);
	DDX_Control(pDX, IDC_WS_MINIMIZE, c_WS_MINIMIZE);
	DDX_Control(pDX, IDC_WS_MAXIMIZEBOX, c_WS_MAXIMIZEBOX);
	DDX_Control(pDX, IDC_WS_MAXIMIZE, c_WS_MAXIMIZE);
	DDX_Control(pDX, IDC_WS_HSCROLL, c_WS_HSCROLL);
	DDX_Control(pDX, IDC_WS_GROUP, c_WS_GROUP);
	DDX_Control(pDX, IDC_WS_DLGFRAME, c_WS_DLGFRAME);
	DDX_Control(pDX, IDC_WS_CLIPSIBLINGS, c_WS_CLIPSIBLINGS);
	DDX_Control(pDX, IDC_WS_CLIPCHILDREN, c_WS_CLIPCHILDREN);
	DDX_Control(pDX, IDC_WS_CAPTION, c_WS_CAPTION);
	DDX_Control(pDX, IDC_WS_BORDER, c_WS_BORDER);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStyles, CPropertyPage)
	//{{AFX_MSG_MAP(CStyles)
	ON_MESSAGE(PSM_QUERYSIBLINGS, OnQuerySiblings)
	ON_BN_CLICKED(IDC_WS_BORDER, OnWsBorder)
	ON_BN_CLICKED(IDC_WS_CAPTION, OnWsCaption)
	ON_BN_CLICKED(IDC_WS_CLIPCHILDREN, OnWsClipchildren)
	ON_BN_CLICKED(IDC_WS_CLIPSIBLINGS, OnWsClipsiblings)
	ON_BN_CLICKED(IDC_WS_DLGFRAME, OnWsDlgrame)
	ON_BN_CLICKED(IDC_WS_GROUP, OnWsGroup)
	ON_BN_CLICKED(IDC_WS_HSCROLL, OnWsHscroll)
	ON_BN_CLICKED(IDC_WS_MAXIMIZE, OnWsMaximize)
	ON_BN_CLICKED(IDC_WS_MAXIMIZEBOX, OnWsMaximizebox)
	ON_BN_CLICKED(IDC_WS_MINIMIZE, OnWsMinimize)
	ON_BN_CLICKED(IDC_WS_MINIMIZEBOX, OnWsMinimizebox)
	ON_BN_CLICKED(IDC_WS_SYSMENU, OnWsSysmenu)
	ON_BN_CLICKED(IDC_WS_TABSTOP, OnWsTabstop)
	ON_BN_CLICKED(IDC_WS_THICKFRAME, OnWsThickframe)
	ON_BN_CLICKED(IDC_WS_VISIBLE, OnWsVisible)
	ON_BN_CLICKED(IDC_WS_VSCROLL, OnWsVscroll)
	ON_BN_CLICKED(IDC_WS_POPUPWINDOW, OnWsPopupwindow)
	ON_BN_CLICKED(IDC_WS_OVERLAPPEDWINDOW, OnWsOverlappedwindow)
	ON_BN_CLICKED(IDC_WS_CHILDWINDOW, OnWsChildwindow)
	ON_BN_CLICKED(IDC_WS_CHILD, OnWsChild)
	ON_BN_CLICKED(IDC_WS_OVERLAPPED, OnWsOverlapped)
	ON_BN_CLICKED(IDC_WS_POPUP, OnWsPopup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStyles message handlers

void CStyles::OnWsBorder() 
{
 showStyles();
 setPopupWindow();	
}

void CStyles::OnWsCaption() 
{
 showStyles();
 setOverlappedWindow();
}

void CStyles::OnWsClipchildren() 
{
 showStyles();
}

void CStyles::OnWsClipsiblings() 
{
 showStyles();
}

void CStyles::OnWsDlgrame() 
{
 showStyles();
}

void CStyles::OnWsGroup() 
{
 showStyles();
}

void CStyles::OnWsHscroll() 
{
 showStyles();
}

void CStyles::OnWsVscroll() 
{
 showStyles();
}

void CStyles::OnWsMaximize() 
{
 showStyles();
}

void CStyles::OnWsMaximizebox() 
{
 setOverlappedWindow();
 showStyles();
}

void CStyles::OnWsMinimize() 
{
 showStyles();
}

void CStyles::OnWsMinimizebox() 
{
 setOverlappedWindow();
 showStyles();
}

void CStyles::OnWsSysmenu() 
{
 setOverlappedWindow();
 setPopupWindow();	
 showStyles();
}

void CStyles::OnWsTabstop() 
{
 showStyles();
}

void CStyles::OnWsThickframe() 
{
 setOverlappedWindow();
 setPopupWindow();
 showStyles();
}

void CStyles::OnWsVisible() 
{
 showStyles();
}

BOOL CStyles::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
 	c_WS_VISIBLE.SetCheck(BST_CHECKED);

	showStyles();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/****************************************************************************
*                          CStyles::OnQuerySiblings
* Inputs:
*       WPARAM: ignored
*	LPARAM: ignored
* Result: LRESULT
*       Style bits from controls
* Notes:
*	Save in local so we can look at it with debugger
****************************************************************************/

LRESULT CStyles::OnQuerySiblings(WPARAM, LPARAM)
    {
     DWORD result = controlsToFlags(this, stylemap, TRUE);
     if(result == 0)
	result |= WS_VISIBLE;  // we always force visible
     return result;
    }

void CStyles::OnWsPopupwindow() 
{
 c_WS_POPUP.SetCheck(c_WS_POPUPWINDOW.GetCheck());
 c_WS_BORDER.SetCheck(c_WS_POPUPWINDOW.GetCheck());
 c_WS_SYSMENU.SetCheck(c_WS_POPUPWINDOW.GetCheck());
 if(c_WS_POPUPWINDOW.GetCheck() == BST_CHECKED)
    { /* popup */
     c_WS_CHILD.SetCheck(BST_UNCHECKED);
     c_WS_CHILDWINDOW.SetCheck(BST_UNCHECKED);
     c_WS_OVERLAPPED.SetCheck(BST_UNCHECKED);
    } /* popup */
 setOverlappedWindow();
 showStyles();
}

void CStyles::OnWsOverlappedwindow() 
{
 c_WS_OVERLAPPED.SetCheck(c_WS_OVERLAPPEDWINDOW.GetCheck());
 c_WS_CAPTION.SetCheck(c_WS_OVERLAPPEDWINDOW.GetCheck());
 c_WS_SYSMENU.SetCheck(c_WS_OVERLAPPEDWINDOW.GetCheck());
 c_WS_THICKFRAME.SetCheck(c_WS_OVERLAPPEDWINDOW.GetCheck());
 c_WS_MINIMIZEBOX.SetCheck(c_WS_OVERLAPPEDWINDOW.GetCheck());
 c_WS_MAXIMIZEBOX.SetCheck(c_WS_OVERLAPPEDWINDOW.GetCheck());
 if(c_WS_OVERLAPPED.GetCheck() == BST_CHECKED)
    { /* overlapped */
     c_WS_CHILD.SetCheck(BST_UNCHECKED);
     c_WS_CHILDWINDOW.SetCheck(BST_UNCHECKED);
     c_WS_POPUP.SetCheck(BST_UNCHECKED);
    } /* overlapped */
 setPopupWindow();
 showStyles();
}

void CStyles::OnWsChildwindow() 
{
 c_WS_CHILD.SetCheck(c_WS_CHILDWINDOW.GetCheck());
 OnWsChild();
 showStyles();
}

void CStyles::OnWsChild() 
{
 c_WS_CHILDWINDOW.SetCheck(c_WS_CHILD.GetCheck());
 if(c_WS_CHILD.GetCheck() == BST_CHECKED)
    { /* child */
     c_WS_OVERLAPPED.SetCheck(BST_UNCHECKED);
     c_WS_POPUP.SetCheck(BST_UNCHECKED);
     setOverlappedWindow();
     setPopupWindow();
    } /* child */
 showStyles();
}


void CStyles::setPopupWindow()
{
    if(c_WS_POPUP.GetCheck() == BST_CHECKED &&
       c_WS_BORDER.GetCheck() == BST_CHECKED &&
       c_WS_SYSMENU.GetCheck() == BST_CHECKED &&
       c_WS_OVERLAPPED.GetCheck() == BST_UNCHECKED &&
       c_WS_CHILD.GetCheck() == BST_UNCHECKED)
           c_WS_POPUPWINDOW.SetCheck(BST_CHECKED);
       else
           c_WS_POPUPWINDOW.SetCheck(BST_UNCHECKED);
 showStyles();
}

void CStyles::setOverlappedWindow()
{
    if(c_WS_CAPTION.GetCheck() == BST_CHECKED &&
       c_WS_OVERLAPPED.GetCheck() == BST_CHECKED &&
       c_WS_SYSMENU.GetCheck() == BST_CHECKED &&
       c_WS_THICKFRAME.GetCheck() == BST_CHECKED &&
       c_WS_MINIMIZEBOX.GetCheck() == BST_CHECKED &&
       c_WS_MAXIMIZEBOX.GetCheck() == BST_CHECKED &&
       c_WS_CHILD.GetCheck() == BST_UNCHECKED &&
       c_WS_POPUP.GetCheck() == BST_UNCHECKED)
	   c_WS_OVERLAPPEDWINDOW.SetCheck(BST_CHECKED);
       else
	   c_WS_OVERLAPPEDWINDOW.SetCheck(BST_UNCHECKED);
 showStyles();
}

void CStyles::OnWsOverlapped() 
{
 if(c_WS_OVERLAPPED.GetCheck() == BST_CHECKED)
    { /* overlapped */
     c_WS_CHILD.SetCheck(BST_UNCHECKED);
     c_WS_CHILDWINDOW.SetCheck(BST_UNCHECKED);
     c_WS_POPUP.SetCheck(BST_UNCHECKED);
    } /* overlapped */
	 
 setOverlappedWindow();
 setPopupWindow();
 showStyles();
}

void CStyles::OnWsPopup() 
{
 if(c_WS_POPUP.GetCheck() == BST_CHECKED)
    { /* checked */
     c_WS_CHILD.SetCheck(BST_UNCHECKED);
     c_WS_OVERLAPPED.SetCheck(BST_UNCHECKED);
     c_WS_CHILDWINDOW.SetCheck(BST_UNCHECKED);
    } /* checked */
 setPopupWindow();	
 setOverlappedWindow();
 showStyles();
}

void CStyles::showStyles()
{
 c_Flags.SetWindowText(controlsToFlags(this, stylemap, TRUE), _T("%08x"));
}
