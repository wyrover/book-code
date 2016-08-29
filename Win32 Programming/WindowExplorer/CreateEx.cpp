// CreateEx.cpp : implementation file
//

#include "stdafx.h"
#include "afxtempl.h"

#include "mainfrm.h"
#include "WndExp.h"
#include "idcombo.h"
#include "NumericEdit.h"
#include "message.h"
#include "winlist.h"
#include "winmgr.h"
#include "CreateEx.h"
#include "ctlmap.h"
#include "is95.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static IDData WS_Styles[] = {
	{ IDS_WS_OVERLAPPED,   WS_OVERLAPPED },
        { IDS_WS_POPUP,        WS_POPUP },
        { IDS_WS_CHILD,        WS_CHILD },
        { IDS_WS_MINIMIZE,     WS_MINIMIZE },
        { IDS_WS_VISIBLE,      WS_VISIBLE },
        { IDS_WS_DISABLED,     WS_DISABLED },
        { IDS_WS_CLIPSIBLINGS, WS_CLIPSIBLINGS },
        { IDS_WS_CLIPCHILDREN, WS_CLIPCHILDREN },
        { IDS_WS_MAXIMIZE,     WS_MAXIMIZE },
        { IDS_WS_CAPTION,      WS_CAPTION },
        { IDS_WS_BORDER,       WS_BORDER },
        { IDS_WS_DLGFRAME,     WS_DLGFRAME },
        { IDS_WS_VSCROLL,      WS_VSCROLL },
        { IDS_WS_HSCROLL,      WS_HSCROLL },
        { IDS_WS_SYSMENU,      WS_SYSMENU },
        { IDS_WS_THICKFRAME,   WS_THICKFRAME },
        { IDS_WS_GROUP,        WS_GROUP },
        { IDS_WS_TABSTOP,      WS_TABSTOP },
        { IDS_WS_MINIMIZEBOX,  WS_MINIMIZEBOX},
        { IDS_WS_MAXIMIZEBOX,  WS_MAXIMIZEBOX},
        { 0, 0} // EOT
			    };

/////////////////////////////////////////////////////////////////////////////
// CCreateWindowEx property page

IMPLEMENT_DYNCREATE(CCreateWindowEx, CPropertyPage)

CCreateWindowEx::CCreateWindowEx() : CPropertyPage(CCreateWindowEx::IDD)
{
 initialized = FALSE;
	//{{AFX_DATA_INIT(CCreateWindowEx)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCreateWindowEx::~CCreateWindowEx()
{
}

void CCreateWindowEx::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCreateWindowEx)
	DDX_Control(pDX, IDC_SPINID, c_SpinID);
	DDX_Control(pDX, IDC_MENU, c_Menu);
	DDX_Control(pDX, IDC_ID, c_ID);
	DDX_Control(pDX, IDC_API3_ONLY, c_API3Only);
	DDX_Control(pDX, IDC_WINDOWS, c_Windows);
	DDX_Control(pDX, IDC_SHOW, c_Show);
	DDX_Control(pDX, IDC_MESSAGE, c_Message);
	DDX_Control(pDX, IDC_CLASSNAME, c_Classname);
	DDX_Control(pDX, IDC_HSPIN, c_HSpin);
	DDX_Control(pDX, IDC_PARENT, c_Parent);
	DDX_Control(pDX, IDC_WINDOWNAME, c_WindowName);
	DDX_Control(pDX, IDC_YSPIN, c_YSpin);
	DDX_Control(pDX, IDC_XSPIN, c_XSpin);
	DDX_Control(pDX, IDC_WSPIN, c_WSpin);
	DDX_Control(pDX, IDC_WS_EX_WINDOWEDGE, c_WS_EX_WINDOWEDGE);
	DDX_Control(pDX, IDC_WS_EX_TRANSPARENT, c_WS_EX_TRANSPARENT);
	DDX_Control(pDX, IDC_WS_EX_TOPMOST, c_WS_EX_TOPMOST);
	DDX_Control(pDX, IDC_WS_EX_TOOLWINDOW, c_WS_EX_TOOLWINDOW);
	DDX_Control(pDX, IDC_WS_EX_STATICEDGE, c_WS_EX_STATICEDGE);
	DDX_Control(pDX, IDC_WS_EX_RTLREADING, c_WS_EX_RTLREADING);
	DDX_Control(pDX, IDC_WS_EX_RIGHTSCROLLBAR, c_WS_EX_RIGHTSCROLLBAR);
	DDX_Control(pDX, IDC_WS_EX_RIGHT, c_WS_EX_RIGHT);
	DDX_Control(pDX, IDC_WS_EX_PALETTEWINDOW, c_WS_EX_PALETTEWINDOW);
	DDX_Control(pDX, IDC_WS_EX_OVERLAPPEDWINDOW, c_WS_EX_OVERLAPPEDWINDOW);
	DDX_Control(pDX, IDC_WS_EX_ACCEPTFILES, c_WS_EX_ACCEPTFILES);
	DDX_Control(pDX, IDC_WS_EX_NOPARENTNOTIFY, c_WS_EX_NOPARENTNOTIFY);
	DDX_Control(pDX, IDC_WS_EX_MDICHILD, c_WS_EX_MDICHILD);
	DDX_Control(pDX, IDC_WS_EX_LTRREADING, c_WS_EX_LTRREADING);
	DDX_Control(pDX, IDC_WS_EX_DLGMODALFRAME, c_WS_EX_DLGMODALFRAME);
	DDX_Control(pDX, IDC_WS_EX_CONTROLPARENT, c_WS_EX_CONTROLPARENT);
	DDX_Control(pDX, IDC_WS_EX_CONTEXTHELP, c_WS_EX_CONTEXTHELP);
	DDX_Control(pDX, IDC_WS_EX_CLIENTEDGE, c_WS_EX_CLIENTEDGE);
	DDX_Control(pDX, IDC_WS_EX_APPWINDOW, c_WS_EX_APPWINDOW);
	DDX_Control(pDX, IDC_WS_EX_LEFTSCROLLBAR, c_WS_EX_LEFTSCROLLBAR);
	DDX_Control(pDX, IDC_WS_EX_LEFT, c_WS_EX_LEFT);
	DDX_Control(pDX, IDC_Y, c_Y);
	DDX_Control(pDX, IDC_X, c_X);
	DDX_Control(pDX, IDC_W, c_W);
	DDX_Control(pDX, IDC_H, c_H);
	DDX_Control(pDX, IDC_DESTROY, c_Destroy);
	DDX_Control(pDX, IDC_CREATE, c_Create);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCreateWindowEx, CPropertyPage)
	//{{AFX_MSG_MAP(CCreateWindowEx)
	ON_BN_CLICKED(IDC_CREATE, OnCreateEx)
	ON_BN_CLICKED(IDC_DESTROY, OnDoDestroy)
	ON_BN_CLICKED(IDC_WS_EX_ACCEPTFILES, OnWsExAcceptfiles)
	ON_BN_CLICKED(IDC_WS_EX_APPWINDOW, OnWsExAppwindow)
	ON_BN_CLICKED(IDC_WS_EX_CLIENTEDGE, OnWsExClientedge)
	ON_BN_CLICKED(IDC_WS_EX_CONTEXTHELP, OnWsExContexthelp)
	ON_BN_CLICKED(IDC_WS_EX_CONTROLPARENT, OnWsExControlparent)
	ON_BN_CLICKED(IDC_WS_EX_DLGMODALFRAME, OnWsExDlgmodalframe)
	ON_BN_CLICKED(IDC_WS_EX_LEFT, OnWsExLeft)
	ON_BN_CLICKED(IDC_WS_EX_LEFTSCROLLBAR, OnWsExLeftscrollbar)
	ON_BN_CLICKED(IDC_WS_EX_LTRREADING, OnWsExLtrreading)
	ON_BN_CLICKED(IDC_WS_EX_MDICHILD, OnWsExMdichild)
	ON_BN_CLICKED(IDC_WS_EX_NOPARENTNOTIFY, OnWsExNoparentnotify)
	ON_BN_CLICKED(IDC_WS_EX_OVERLAPPEDWINDOW, OnWsExOverlappedwindow)
	ON_BN_CLICKED(IDC_WS_EX_PALETTEWINDOW, OnWsExPalettewindow)
	ON_BN_CLICKED(IDC_WS_EX_RIGHT, OnWsExRight)
	ON_BN_CLICKED(IDC_WS_EX_RIGHTSCROLLBAR, OnWsExRightscrollbar)
	ON_BN_CLICKED(IDC_WS_EX_RTLREADING, OnWsExRtlreading)
	ON_BN_CLICKED(IDC_WS_EX_STATICEDGE, OnWsExStaticedge)
	ON_BN_CLICKED(IDC_WS_EX_TOOLWINDOW, OnWsExToolwindow)
	ON_BN_CLICKED(IDC_WS_EX_TOPMOST, OnWsExTopmost)
	ON_BN_CLICKED(IDC_WS_EX_TRANSPARENT, OnWsExTransparent)
	ON_BN_CLICKED(IDC_WS_EX_WINDOWEDGE, OnWsExWindowedge)
	ON_EN_CHANGE(IDC_H, OnChangeH)
	ON_EN_CHANGE(IDC_W, OnChangeW)
	ON_EN_CHANGE(IDC_X, OnChangeX)
	ON_EN_CHANGE(IDC_Y, OnChangeY)
	ON_CBN_SELENDOK(IDC_CLASSNAME, OnSelendokClassname)
	ON_EN_CHANGE(IDC_WINDOWNAME, OnChangeWindowname)
	ON_NOTIFY(NM_DBLCLK, IDC_WINDOWS, OnDblclkWindows)
	ON_NOTIFY(NM_KILLFOCUS, IDC_WINDOWS, OnKillfocusWindows)
	ON_NOTIFY(NM_SETFOCUS, IDC_WINDOWS, OnSetfocusWindows)
	ON_NOTIFY(NM_CLICK, IDC_WINDOWS, OnClickWindows)
	ON_BN_CLICKED(IDC_API3_ONLY, OnApi3Only)
	ON_CBN_SELENDOK(IDC_PARENT, OnSelendokParent)
	ON_WM_DESTROY()
	ON_CBN_SELENDOK(IDC_MENU, OnSelendokMenu)
	ON_EN_CHANGE(IDC_ID, OnChangeId)
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_WINDOWNAME, OnChangeWindowname)
	ON_REGISTERED_MESSAGE(CMainFrame::UWM_ACTIVATE, OnMyActivate)
	ON_REGISTERED_MESSAGE(CWndExpApp::UWM_KILL, OnMyKill)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreateWindowEx message handlers

static controlmap exstylemap[] = {
                { IDC_WS_EX_ACCEPTFILES,      WS_EX_ACCEPTFILES },
                { IDC_WS_EX_APPWINDOW,        WS_EX_APPWINDOW },
                { IDC_WS_EX_CLIENTEDGE,       WS_EX_CLIENTEDGE },
                { IDC_WS_EX_CONTEXTHELP,      WS_EX_CONTEXTHELP },
                { IDC_WS_EX_CONTROLPARENT,    WS_EX_CONTROLPARENT },
                { IDC_WS_EX_DLGMODALFRAME,    WS_EX_DLGMODALFRAME },
                { IDC_WS_EX_LEFT,             WS_EX_LEFT},
                { IDC_WS_EX_LEFTSCROLLBAR,    WS_EX_LEFTSCROLLBAR },
                { IDC_WS_EX_LTRREADING,       WS_EX_LTRREADING },
                { IDC_WS_EX_MDICHILD,         WS_EX_MDICHILD },
                { IDC_WS_EX_NOPARENTNOTIFY,   WS_EX_NOPARENTNOTIFY },
                { IDC_WS_EX_OVERLAPPEDWINDOW, WS_EX_OVERLAPPEDWINDOW },
                { IDC_WS_EX_PALETTEWINDOW,    WS_EX_PALETTEWINDOW },
                { IDC_WS_EX_RIGHT,            WS_EX_RIGHT },
                { IDC_WS_EX_RIGHTSCROLLBAR,   WS_EX_RIGHTSCROLLBAR },
                { IDC_WS_EX_RTLREADING,       WS_EX_RTLREADING },
                { IDC_WS_EX_STATICEDGE,       WS_EX_STATICEDGE },
                { IDC_WS_EX_TOOLWINDOW,       WS_EX_TOOLWINDOW },
                { IDC_WS_EX_TOPMOST,          WS_EX_TOPMOST},
                { IDC_WS_EX_TRANSPARENT,      WS_EX_TRANSPARENT },
                { IDC_WS_EX_WINDOWEDGE,       WS_EX_WINDOWEDGE },
		{ 0, 0} // EOT
			  };


void CCreateWindowEx::doCreate(BOOL doit)
{
 if(!initialized)
    return;

 HWND hwnd;
 DWORD exstyle = controlsToFlags(this, exstylemap);
 DWORD style = QuerySiblings(0, 0);
 if(style == 0)
    style = WS_VISIBLE;

 HWND parent = NULL;
 HMENU hmenu = NULL;
 CString windowname;
 CString classname;
 int w = c_W.GetWindowInt();
 int h = c_H.GetWindowInt();
 int x = c_X.GetWindowInt();
 int y = c_Y.GetWindowInt();

 int index = c_Parent.GetCurSel();
 if(index != CB_ERR)
    { /* has selection */
     parent = (HWND)c_Parent.GetItemData(index);
    } /* has selection */

 c_WindowName.GetWindowText(windowname);
 c_Classname.GetWindowText(classname);

 CString parms;
 CString menu;
 int menuid;
 BOOL needsmenu = FALSE;

 // The menu value is either the control ID or a menu handle
 if(style & WS_CHILD)
    { /* child window */
     // A child window has a control ID value
     hmenu = (HMENU)c_ID.GetWindowInt();
     menu.Format(_T("(HMENU)%d"), (int)hmenu);
     menuid = 0;
    } /* child window */
 else
    { /* popup/overlapped */
     int index = c_Menu.GetCurSel();
     if(index == CB_ERR || c_Menu.GetItemData(index) == 0)
        { /* nothing */
	 hmenu = NULL;
	 menu = _T("NULL");
	 menuid = 0;
	} /* nothing */
     else
        { /* has menu */
	 CString s;
	 hmenu = NULL;
	 c_Menu.GetLBText(index, s);
	 menu.Format(_T("\"%s\""), s);
	 menuid = c_Menu.GetItemData(index);
	 needsmenu = TRUE;
	} /* has menu */
    } /* popup/overlapped */
 
 parms.Format(
    _T("(%08x, \"%s\", \"%s\", %08x, %d, %d, %d, %d, %08x, %s, %08x, NULL)"),

 			exstyle, classname, windowname, style,
			x, y, w, h,
			parent, menu, AfxGetInstanceHandle());

 c_Show.SetWindowText(parms);
 if(!doit)
    return;  // just displays the settings

 // We are actually going to create a window.  If we have a real menu,
 // load it now

 if(needsmenu && menuid != 0)
    { /* load menu */
     CMenu realmenu;
     realmenu.LoadMenu(menuid);
     hmenu = realmenu.m_hMenu;
     realmenu.Detach();
    } /* load menu */ 

 hwnd = ::CreateWindowEx(exstyle, classname, windowname, style, x, y, w, h,
 			parent, hmenu, AfxGetInstanceHandle(), NULL);
 if(hwnd == NULL)
    { /* failed */
     LPTSTR msg;
     DWORD error = GetLastError();
     if(FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		      FORMAT_MESSAGE_FROM_SYSTEM,
		      NULL,
		      error,
		      0, 
		      (LPTSTR) & msg,
		      0, NULL))
        { /* success */
	 CString m(msg); // be lazy, use TrimRight
	 m.TrimRight();
	 c_Message.SetWindowText(m);
	 LocalFree(msg);
	} /* success */
    } /* failed */
 else
    { /* success */
     ::SetWindowLong(hwnd, GWL_USERDATA, (LPARAM)this); // for possible kill
     c_Message.SetWindowText(NULL);
     windowlist.Add(hwnd);
     updateWindowList();
    } /* success */

}


/****************************************************************************
*                      CCreateWindowEx::updateWindowList
* Result: void
*       
* Effect: 
*       Updates the list of active windows
****************************************************************************/

void CCreateWindowEx::updateWindowList()
    {
     CString s;
     CString caption;
     POSITION p;
     HWND hwnd;

     c_Windows.DeleteAllItems();

     p = windowlist.getFirst();

     if(p != NULL)
	for(hwnd = windowlist.getNext(p); ; hwnd = windowlist.getNext(p))
	   { /* iterate down list */
	    // Note that a window in the list may no longer be valid
	    // because it has been destroyed by the right-button popup
	    // So we do an explicit test and don't add any window that
	    // isn't real.

	    if(::IsWindow(hwnd))
	       { /* real window */
		CWnd * wnd = CWnd::FromHandle(hwnd);
		wnd->GetWindowText(caption);  // take advantage of CString
		c_Windows.Add(hwnd);
	       } /* real window */
	    else
	       { /* not window */
	        windowlist.Remove(hwnd);
	       } /* not window */
	    if( p == NULL)
	       break;
	   } /* iterate down list */
    }


void CCreateWindowEx::OnCreateEx() 
{
 doCreate(TRUE);
	
}

void CCreateWindowEx::OnDoDestroy() 
{
 HWND hwnd = c_Windows.GetCurItem();
 if(hwnd != NULL)
    { /* destroy it */
     ::DestroyWindow(hwnd);
     
     windowlist.Remove(hwnd);
     updateWindowList();
    } /* destroy it */
	
}

void CCreateWindowEx::OnWsExAcceptfiles() 
{
	// TODO: Add your control notification handler code here
 doCreate(FALSE);
	
}

void CCreateWindowEx::OnWsExAppwindow() 
{
	// TODO: Add your control notification handler code here
 doCreate(FALSE);
	
}

void CCreateWindowEx::OnWsExClientedge() 
{
 int state = c_WS_EX_CLIENTEDGE.GetCheck();
 if(state == BST_UNCHECKED)
     c_WS_EX_OVERLAPPEDWINDOW.SetCheck(BST_UNCHECKED);
 else
 if(c_WS_EX_CLIENTEDGE.GetCheck() == BST_CHECKED)
    c_WS_EX_OVERLAPPEDWINDOW.SetCheck(BST_CHECKED);

 doCreate(FALSE);
	
}

void CCreateWindowEx::OnWsExContexthelp() 
{
	// TODO: Add your control notification handler code here
 doCreate(FALSE);
	
}

void CCreateWindowEx::OnWsExControlparent() 
{
	// TODO: Add your control notification handler code here
 doCreate(FALSE);
	
}

void CCreateWindowEx::OnWsExDlgmodalframe() 
{
	// TODO: Add your control notification handler code here
 doCreate(FALSE);
	
}

void CCreateWindowEx::OnWsExLeft() 
{
 if(c_WS_EX_LEFT.GetCheck())
    c_WS_EX_RIGHT.SetCheck(BST_UNCHECKED);
 doCreate(FALSE);
	
}

void CCreateWindowEx::OnWsExLeftscrollbar() 
{
 if(c_WS_EX_LEFTSCROLLBAR.GetCheck())
    c_WS_EX_RIGHTSCROLLBAR.SetCheck(BST_UNCHECKED);
 doCreate(FALSE);
 
	
}

void CCreateWindowEx::OnWsExLtrreading() 
{
 if(c_WS_EX_LTRREADING.GetCheck())
    c_WS_EX_RTLREADING.SetCheck(BST_UNCHECKED);
 doCreate(FALSE);
	
}

void CCreateWindowEx::OnWsExMdichild() 
{
	// TODO: Add your control notification handler code here
 doCreate(FALSE);
	
}

void CCreateWindowEx::OnWsExNoparentnotify() 
{
	// TODO: Add your control notification handler code here
 doCreate(FALSE);
	
}

void CCreateWindowEx::OnWsExOverlappedwindow() 
{
 int state = c_WS_EX_OVERLAPPEDWINDOW.GetCheck();
 c_WS_EX_WINDOWEDGE.SetCheck(state);
 c_WS_EX_CLIENTEDGE.SetCheck(state);
 doCreate(FALSE);
	
}

void CCreateWindowEx::OnWsExPalettewindow() 
{
 int state = c_WS_EX_PALETTEWINDOW.GetCheck();
 c_WS_EX_WINDOWEDGE.SetCheck(state);
 c_WS_EX_TOOLWINDOW.SetCheck(state);
 c_WS_EX_TOPMOST.SetCheck(state);
 doCreate(FALSE);
	
}

void CCreateWindowEx::OnWsExRight() 
{
 if(c_WS_EX_RIGHT.GetCheck())
    c_WS_EX_LEFT.SetCheck(BST_UNCHECKED);
 doCreate(FALSE);
	
}

void CCreateWindowEx::OnWsExRightscrollbar() 
{
 if(c_WS_EX_RIGHTSCROLLBAR.GetCheck())
    c_WS_EX_LEFTSCROLLBAR.SetCheck(BST_UNCHECKED);
 doCreate(FALSE);
	
}

void CCreateWindowEx::OnWsExRtlreading() 
{
 if(c_WS_EX_RTLREADING.GetCheck())
    c_WS_EX_LTRREADING.SetCheck(BST_UNCHECKED);
 doCreate(FALSE);
	
}

void CCreateWindowEx::OnWsExStaticedge() 
{
	// TODO: Add your control notification handler code here
 doCreate(FALSE);
	
}

void CCreateWindowEx::OnWsExToolwindow() 
{
 int state = c_WS_EX_TOOLWINDOW.GetCheck();

 if(state == BST_UNCHECKED)
    c_WS_EX_PALETTEWINDOW.SetCheck(BST_UNCHECKED);
 else
 if(c_WS_EX_WINDOWEDGE.GetCheck() == BST_CHECKED &&
    c_WS_EX_TOPMOST.GetCheck() == BST_CHECKED)
        c_WS_EX_PALETTEWINDOW.SetCheck(BST_CHECKED);

 doCreate(FALSE);
	
}

void CCreateWindowEx::OnWsExTopmost() 
{
 int state = c_WS_EX_TOPMOST.GetCheck();

 if(state == BST_UNCHECKED)
    c_WS_EX_PALETTEWINDOW.SetCheck(BST_UNCHECKED);
 else
 if(c_WS_EX_WINDOWEDGE.GetCheck() == BST_CHECKED &&
    c_WS_EX_TOOLWINDOW.GetCheck() == BST_CHECKED)
        c_WS_EX_PALETTEWINDOW.SetCheck(BST_CHECKED);

 doCreate(FALSE);
	
}

void CCreateWindowEx::OnWsExTransparent() 
{
	// TODO: Add your control notification handler code here
 doCreate(FALSE);
	
}

void CCreateWindowEx::OnWsExWindowedge() 
{
 int state = c_WS_EX_WINDOWEDGE.GetCheck();
 if(state == BST_UNCHECKED)
    { /* off */
     c_WS_EX_OVERLAPPEDWINDOW.SetCheck(BST_UNCHECKED);
     c_WS_EX_PALETTEWINDOW.SetCheck(BST_UNCHECKED);
    } /* off */
 else
    { /* on */
     if(c_WS_EX_CLIENTEDGE.GetCheck() == BST_CHECKED)
	c_WS_EX_OVERLAPPEDWINDOW.SetCheck(BST_CHECKED);
     if(c_WS_EX_TOOLWINDOW.GetCheck() == BST_CHECKED &&
	c_WS_EX_TOPMOST.GetCheck() == BST_CHECKED)
	   c_WS_EX_PALETTEWINDOW.SetCheck(BST_CHECKED);
    } /* on */
 doCreate(FALSE);
	
}

/****************************************************************************
*                        CCreateWindowEx::addClassName
* Inputs:
*       int id: ID of string to be added
* Result: int
*       Width of string in c_Windows units
* Effect: 
*       Adds the string to the class names list
****************************************************************************/

int CCreateWindowEx::addClassName(int id)
    {
     CString s;
     s.LoadString(id);
     int width = c_Windows.GetStringWidth(s);
     int index = c_Classname.AddString(s);
     c_Classname.SetItemData(index, id);
     return width;
    }

/****************************************************************************
*                       CCreateWindowEx::initClassNames
* Result: int
*       Width of widest name in units suitable for c_Windows
* Effect: 
*       Adds the class names to 
****************************************************************************/

int CCreateWindowEx::initClassNames()
    {
     CString s;
     int width = 0;
     int n;

     n = addClassName(IDS_GENERIC_OVERLAPPED);
     width = max(width, n);

     n = addClassName(IDS_GENERIC_CHILD);
     width = max(width, n);

     n = addClassName(IDS_GENERIC_POPUP);
     width = max(width, n);

     n = addClassName(IDS_EDIT);
     width = max(width, n);

     c_Classname.SetCurSel(0);  // default selection

     for(int i = 0; i < c_Classname.GetCount(); i++)
        { /* find index */
	 if(c_Classname.GetItemData(i) == IDS_GENERIC_OVERLAPPED)
	    { /* found it */
	     c_Classname.SetCurSel(i);
	     break;
	    } /* found it */
	} /* find index */
		 
     return width;
    }


/****************************************************************************
*                       CCreateWindowEx::enableControls
* Result: void
*       
* Effect: 
*       Enables the various controls
****************************************************************************/

void CCreateWindowEx::enableControls()
    {
     BOOL enabled = !Is3() || (Is3() && !c_API3Only.GetCheck());
     c_WS_EX_WINDOWEDGE.EnableWindow(enabled);
     c_WS_EX_TOOLWINDOW.EnableWindow(enabled);
     c_WS_EX_STATICEDGE.EnableWindow(enabled);
     c_WS_EX_RTLREADING.EnableWindow(enabled);
     c_WS_EX_RIGHTSCROLLBAR.EnableWindow(enabled);
     c_WS_EX_RIGHT.EnableWindow(enabled);
     c_WS_EX_PALETTEWINDOW.EnableWindow(enabled);
     c_WS_EX_OVERLAPPEDWINDOW.EnableWindow(enabled);
     c_WS_EX_MDICHILD.EnableWindow(enabled);
     c_WS_EX_LTRREADING.EnableWindow(enabled);
     c_WS_EX_CONTROLPARENT.EnableWindow(enabled);
     // c_WS_EX_CONTEXTHELP.EnableWindow(enabled); // not speced at API3, but there anyway
     c_WS_EX_CLIENTEDGE.EnableWindow(enabled);
     c_WS_EX_APPWINDOW.EnableWindow(enabled);
     c_WS_EX_LEFTSCROLLBAR.EnableWindow(enabled);
     c_WS_EX_LEFT.EnableWindow(enabled);
    }


/****************************************************************************
*                         CCreateWindowEx::addParent
* Inputs:
*       CString s: String name of window
*	HWND hwnd: Window handle
* Result: void
*       
* Effect: 
*       Adds a potential parent to the window
****************************************************************************/

void CCreateWindowEx::addParent(CString s, HWND hwnd)
    {
     int index = c_Parent.AddString(s);
     c_Parent.SetItemData(index, (DWORD)hwnd);
    }

/****************************************************************************
*                         CCreateWindowEx::addParent
* Inputs:
*       int id: String ID of entry
*	HWND hwnd: Window handle
* Result: void
*       
* Effect: 
*       Adds a potential parent to the window
****************************************************************************/

void CCreateWindowEx::addParent(int id, HWND hwnd)
    {
     CString s;
     s.LoadString(id);
     addParent(s, hwnd);
    }

/****************************************************************************
*                        CCreateWindowEx::initParents
* Result: void
*       
* Effect: 
*       Loads the parent reference
****************************************************************************/

void CCreateWindowEx::initParents()
    {
     int index;
     HWND current = NULL;
     index = c_Parent.GetCurSel();  // get current selection
     if(index != CB_ERR)
        { /* has selection */
	 current = (HWND)c_Parent.GetItemData(index);
	} /* has selection */

     // At this point, 'current' is either NULL (if no selection was made
     // or the combo box is empty), or is the currently selected parent.

     c_Parent.ResetContent();

     addParent(IDS_NONE, NULL);
     addParent(IDS_THIS, m_hWnd);
     
     // Now iterate over all the MDI child windows
     CMDIFrameWnd * mainframe = (CMDIFrameWnd *)AfxGetMainWnd();

     CWnd * child = CWnd::FromHandle(mainframe->m_hWndMDIClient)->GetWindow(GW_CHILD);
     while(child != NULL)
        { /* scan each */
	 CString s;
	 child->GetWindowText(s);
	 if(s.GetLength() == 0)
	    { /* fake it */
	     s.Format(_T("0x%08x"), child->m_hWnd);
	    } /* fake it */

	 addParent(s, child->m_hWnd);
	 child = child->GetWindow(GW_HWNDNEXT);
	} /* scan each */

     // and finally, add any created windows we have that are not themselves
     // child windows

     POSITION p = windowlist.getFirst();
     if(p != NULL)
	for(HWND hwnd = windowlist.getNext(p);  ; hwnd = windowlist.getNext(p))
	   { /* go down list */
	    CString caption;
	    CWnd * wnd = CWnd::FromHandle(hwnd);
	    wnd->GetWindowText(caption);

	    if(caption.GetLength() == 0)
	       { /* fake it */
	        caption.Format(_T("* %08x"), hwnd);
	       } /* fake it */

	    addParent(caption, hwnd);
	    if(p == NULL)
	       break;
	   } /* go down list */
     
     // Set the default to be 'None'

     for(index = 0; index < c_Parent.GetCount(); index++)
        { /* find none */
	 if(c_Parent.GetItemData(index) == 0)
	    { /* found it */
	     c_Parent.SetCurSel(index);
	     break;
	    } /* found it */
	} /* find none */

    }

/****************************************************************************
*                          CCreateWindowEx::initMenu
* Result: void
*       
* Effect: 
*       Initializes the Menu dropdown
****************************************************************************/

void CCreateWindowEx::initMenu()
    {
     CString s;
     s.LoadString(IDR_MENU1);
     int index = c_Menu.AddString(s);
     c_Menu.SetItemData(index, IDR_MENU1);

     s.LoadString(IDS_NONE);
     index = c_Menu.AddString(s);
     c_Menu.SetCurSel(index);
    }

/****************************************************************************
*                        CCreateWindowEx::OnInitDialog
* Result: BOOL
*       TRUE, always
* Effect: 
*       Initializes the controls
****************************************************************************/

BOOL CCreateWindowEx::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	c_WSpin.SetRange(0, ::GetSystemMetrics(SM_CXSCREEN));
	c_WSpin.SetPos(::GetSystemMetrics(SM_CXSCREEN) / 4);

	c_HSpin.SetRange(0, ::GetSystemMetrics(SM_CYSCREEN));
	c_HSpin.SetPos(::GetSystemMetrics(SM_CXSCREEN) / 4);

	c_XSpin.SetRange(0, ::GetSystemMetrics(SM_CXSCREEN));
	c_YSpin.SetRange(0, ::GetSystemMetrics(SM_CYSCREEN));
	

	c_SpinID.SetRange(0, 4095);
	

	initMenu();

	int n = initClassNames();

	
	c_Windows.initialize(n);

	initParents();

	initialized = TRUE;
	doCreate(FALSE);

	c_Destroy.EnableWindow(FALSE);

	c_API3Only.ShowWindow(Is3() ? SW_SHOW : SW_HIDE);

	// We may have values in the window list from a previous
	// incarnation, but we may also have done something outside
	// to cause them to be destroyed.  Re-verify the list.

	updateWindowList();

	if(Is3())
	   c_API3Only.SetCheck(BST_CHECKED);
        enableControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCreateWindowEx::OnChangeH() 
{
	// TODO: Add your control notification handler code here
 doCreate(FALSE);
	
}

void CCreateWindowEx::OnChangeW() 
{
	// TODO: Add your control notification handler code here
 doCreate(FALSE);
	
}

void CCreateWindowEx::OnChangeX() 
{
	// TODO: Add your control notification handler code here
 doCreate(FALSE);
	
}

void CCreateWindowEx::OnChangeY() 
{
	// TODO: Add your control notification handler code here
 doCreate(FALSE);
	
}

void CCreateWindowEx::OnSelendokClassname() 
{
	// TODO: Add your control notification handler code here
 doCreate(FALSE);
	
}

void CCreateWindowEx::OnChangeWindowname() 
{
 doCreate(FALSE);
}

BOOL CCreateWindowEx::OnSetActive() 
{
 	doCreate(FALSE);
	
	return CPropertyPage::OnSetActive();
}

/****************************************************************************
*                        CCreateWindowEx::selectWindow
* Inputs:
*       HWND hwnd: Window handle
* Result: void
*       
* Effect: 
*       Selects the item in the c_Windows list that represents hwnd
****************************************************************************/

void CCreateWindowEx::selectWindow(HWND hwnd)
    {
     LV_FINDINFO info;
     info.flags = LVFI_PARAM;
     info.lParam = (LPARAM)hwnd;

     int i = c_Windows.FindItem(&info);
     if(i == -1)
	return; // not found
     c_Windows.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
     c_Windows.EnsureVisible(i, FALSE);
    }

/****************************************************************************
*                        CCreateWindowEx::OnMyActivate
* Inputs:
*       WPARAM wParam: (WPARAM)HWND: Window handle of window being activated
* Result: LRESULT
*       0 (ignored)
* Effect: 
*       Select the correct item from the window list.
****************************************************************************/

LRESULT CCreateWindowEx::OnMyActivate(WPARAM wParam, LPARAM)
    {
     selectWindow((HWND)wParam);
     return 0;
    }

void CCreateWindowEx::OnDblclkWindows(NMHDR* pNMHDR, LRESULT* pResult) 
{
 	// Find which item has been selected
 	HWND hwnd = c_Windows.GetCurItem();
	if(!::IsWindow(hwnd))
	   { /* destroyed */
	    // The window has been destroyed, but we seem to have missed
	    // the notification.  Just ignore this, but regenerate the
	    // display list.

	    updateWindowList();

	    // Having updated the list, see if we have a selection.  If
	    // not, we disable the Destroy button
	    HWND hwnd = c_Windows.GetCurItem();
	    c_Destroy.EnableWindow(hwnd != NULL && ::IsWindow(hwnd));
	    return;
	   } /* destroyed */

	if(hwnd != NULL)
	   { /* has selection */
	    CWnd * wnd = CWnd::FromHandle(hwnd);
	    wnd->BringWindowToTop();
	    wnd->SetFocus();
	   } /* has selection */
	
	*pResult = 0;
}

void CCreateWindowEx::OnKillfocusWindows(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	*pResult = 0;
}

void CCreateWindowEx::OnSetfocusWindows(NMHDR* pNMHDR, LRESULT* pResult) 
{
 	HWND hwnd = c_Windows.GetCurItem();
	c_Destroy.EnableWindow(hwnd != NULL && ::IsWindow(hwnd));
	
	*pResult = 0;
}

void CCreateWindowEx::OnClickWindows(NMHDR* pNMHDR, LRESULT* pResult) 
{
 	HWND hwnd = c_Windows.GetCurItem();
	c_Destroy.EnableWindow(hwnd != NULL);
	
	*pResult = 0;
}

void CCreateWindowEx::OnApi3Only() 
{
 enableControls();
}

void CCreateWindowEx::OnSelendokParent() 
{
 doCreate(FALSE);
}

/****************************************************************************
*                          CCreateWindowEx::OnMyKill
* Inputs:
*       WPARAM: ignored
*	LPARAM: ignored
* Result: LRESULT
*       0, always (ignored)
* Effect: 
*       Redoes the window list because a window has been killed
****************************************************************************/

LRESULT CCreateWindowEx::OnMyKill(WPARAM, LPARAM)
    {
     updateWindowList();
     return 0;
    }

void CCreateWindowEx::OnDestroy() 
{
	CPropertyPage::OnDestroy();
	
 	// Since created windows may not be owned by this window, they
	// will not necessarily be destroyed when it exits.  This may
	// be what the user intends.
	
}

void CCreateWindowEx::OnSelendokMenu() 
{
 doCreate(FALSE);
}

void CCreateWindowEx::OnChangeId() 
{
 doCreate(FALSE);	
}
