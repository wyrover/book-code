// OD.cpp : implementation file
//

#include "stdafx.h"
#include "menuchk.h"
#include "MenuExplorer.h"
#include "tinybutt.h"
#include "intedit.h"
#include "idcombo.h"
#include "gcombo.h"
#include "color.h"
#include "syscolor.h"
#include "odmenu.h"
#include "OD.h"

#include "odinfo.h"
#include "uwm.h"
#include "xform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COD property page

IMPLEMENT_DYNCREATE(COD, CPropertyPage)

COD::COD() : CPropertyPage(COD::IDD)
{
	//{{AFX_DATA_INIT(COD)
	//}}AFX_DATA_INIT
}

COD::~COD()
{
}

void COD::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COD)
	DDX_Control(pDX, IDC_MENUTEXT, c_MenuText);
	DDX_Control(pDX, IDC_ADD1, c_Add1);
	DDX_Control(pDX, IDC_CHECKMARKS, c_CheckMarks);
	DDX_Control(pDX, IDC_HILITEMENUCOLOR, c_HiliteMenuColor);
	DDX_Control(pDX, IDC_ADD, c_Add);
	DDX_Control(pDX, IDC_TEXTCOLOR, c_TextColor);
	DDX_Control(pDX, IDC_REMOVE, c_Remove);
	DDX_Control(pDX, IDC_MENUCOLOR, c_MenuColor);
	DDX_Control(pDX, IDC_HILITETEXTCOLOR, c_HiliteTextColor);
	DDX_Control(pDX, IDC_GRAYCOLOR, c_GrayColor);
	DDX_Control(pDX, IDC_ACTUALCHECKWIDTH, c_ActualCheckWidth);
	DDX_Control(pDX, IDC_SPINITEMWIDTH, c_SpinItemWidth);
	DDX_Control(pDX, IDC_SPINITEMHEIGHT, c_SpinItemHeight);
	DDX_Control(pDX, IDC_ITEMWIDTH, c_ItemWidth);
	DDX_Control(pDX, IDC_ITEMHEIGHT, c_ItemHeight);
	DDX_Control(pDX, IDC_DOSUBWIDTH, c_DoSubWidth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COD, CPropertyPage)
	//{{AFX_MSG_MAP(COD)
	ON_BN_CLICKED(IDC_DOSUBWIDTH, OnDosubwidth)
	ON_BN_CLICKED(IDC_ADD1, OnAdd1)
	ON_EN_CHANGE(IDC_MENUTEXT, OnChangeMenutext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COD message handlers

void COD::OnDosubwidth() 
{
	// TODO: Add your control notification handler code here
	
}

BOOL COD::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
 	c_MenuColor.Load();
	c_MenuColor.Select(::GetSysColor(COLOR_MENU));

	c_HiliteMenuColor.Load();
	c_HiliteMenuColor.Select(::GetSysColor(COLOR_HIGHLIGHT));

	c_TextColor.Load();
	c_TextColor.Select(::GetSysColor(COLOR_MENUTEXT));

	c_HiliteTextColor.Load();
	c_HiliteTextColor.Select(::GetSysColor(COLOR_HIGHLIGHTTEXT));

	c_GrayColor.Load();
	c_GrayColor.Select(::GetSysColor(COLOR_GRAYTEXT));

	c_CheckMarks.AddString(NULL);  // start with NULL item
	c_CheckMarks.load();
	c_CheckMarks.SetCurSel(0);
	
	c_SpinItemWidth.SetRange(0, 1000);
	c_SpinItemHeight.SetRange(0, 50);
	
	c_ActualCheckWidth.SetVal(LOWORD(GetMenuCheckMarkDimensions()));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COD::OnAdd1() 
{
 odinfo * mi = new odinfo();

 mi->style = 1;  // style 1 menu
 mi->width = c_ItemWidth.GetVal();
 mi->height = c_ItemHeight.GetVal();

 mi->text = c_TextColor.GetCurItem();
 mi->background = c_MenuColor.GetCurItem();
 mi->hilitetext = c_HiliteTextColor.GetCurItem();
 mi->hilitebackground = c_HiliteMenuColor.GetCurItem();
 mi->gray = c_GrayColor.GetCurItem();
 mi->bmcheck = c_CheckMarks.GetCurItem();
 c_MenuText.GetWindowText(mi->s);
 mi->s = escToStr(mi->s);

 GetParent()->SendMessage(UWM_ADD_OWNERDRAW, (WPARAM)mi);
	
}

void COD::OnChangeMenutext() 
{
 c_Add1.EnableWindow(c_MenuText.GetWindowTextLength() > 0);	
}
