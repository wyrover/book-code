// [!output WINX_MAINDLG_FILE].cpp : implementation of the [!output WINX_MAINDLG_CLASS] class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
[!if !WINX_APPTYPE_DLG_MODAL]
#include "AboutDlg.h"
[!endif]
[!if WINX_USE_VIEW]
#include "[!output WINX_VIEW_FILE].h"
[!endif]
#include "[!output WINX_MAINDLG_FILE].h"

[!if WINX_APPTYPE_DLG_FRAME]
BOOL [!output WINX_MAINDLG_CLASS]::PreTranslateMessage(MSG* lpMsg)
{
	if(WM_KEYDOWN == lpMsg->message &&
		(WPARAM)VK_ESCAPE ==  lpMsg->wParam )
	{   
		return  TRUE;
	}   
	
	return WindowBase::PreTranslateMessage(lpMsg);
}

BOOL [!output WINX_MAINDLG_CLASS]::OnInitDialog(HWND hDlg, HWND hWndDefaultFocus)
{
[!if WINX_USE_CMDBAR]
	HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, ATL::CWindow::rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
	// attach menu
	//m_CmdBar.LoadMenu(IDR_MAINFRAME);
	m_CmdBar.AttachMenu(GetMenu());
	// load command bar images
	m_CmdBar.LoadImages(IDR_MAINFRAME);

	SetMenu(NULL);
[!endif]

[!if WINX_USE_REBAR]
[!if WINX_USE_TOOLBAR]
	HWND hWndToolBar = CreateToolBarCtrl(IDR_MAINFRAME);
[!endif]
	CreateSimpleReBar();
[!if WINX_USE_CMDBAR]
	AddSimpleReBarBand(hWndCmdBar);
[!endif]
[!if WINX_USE_TOOLBAR]
	AddSimpleReBarBand(hWndToolBar,NULL,TRUE);
[!endif]
[!endif]

[!if WINX_USE_TOOLBAR && !WINX_USE_REBAR]
	CreateSimpleToolBar(IDR_MAINFRAME);
[!endif]

[!if WINX_USE_STATUSBAR]
	CreateSimpleStatusBar();
[!endif]
	RepositionDialog();
		
	CenterWindow();
[!if WINX_VIEWTYPE_HTML]
	m_ie.DlgItem(hDlg, IDC_[!output UPPERCASE_SAFE_PROJECT_NAME]_VIEW);
		
	CComPtr<IWebBrowser2> spBrowser;
	m_ie.QueryControl(&spBrowser);

	CComVariant v;
	spBrowser->Navigate(CComBSTR("http://www.winxgui.com"), &v, &v, &v, &v);
[!endif]
[!if WINX_VIEWTYPE_EDIT || WINX_VIEWTYPE_LISTBOX || WINX_VIEWTYPE_LISTVIEW || WINX_VIEWTYPE_RICHEDIT || WINX_VIEWTYPE_TREE]
	winx::SubclassDlgItem(&m_pView,hDlg,IDC_[!output UPPERCASE_SAFE_PROJECT_NAME]_VIEW);
[!endif]

[!if WINX_USE_REBAR]
	UIAddToolBar(hWndToolBar);
[!endif]

[!if WINX_USE_REBAR || WINX_USE_TOOLBAR]
	UISetCheck(ID_VIEW_TOOLBAR, 1);
[!endif]
[!if WINX_USE_STATUSBAR]
	UISetCheck(ID_VIEW_STATUS_BAR, 1);
[!endif]
[!if WINX_USE_DDX]
	DoDataExchange();
[!endif]
	return TRUE;
}

LRESULT [!output WINX_MAINDLG_CLASS]::HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT lr = 0;
	if (FrameDialogClass::ProcessFrameMessage(hWnd,message,wParam,lParam,lr))
	{
		return lr;
	}
	
	return WindowBase::HandleMessage(hWnd, message, wParam, lParam);
}
[!if WINX_USE_STATUSBAR]

void [!output WINX_MAINDLG_CLASS]::OnViewStatusBar(HWND hWnd)
{
	static BOOL bVisible = TRUE;
	bVisible = !bVisible;
	ViewStatusBar(bVisible);
	UISetCheck(ID_VIEW_STATUS_BAR,bVisible);
}

[!endif]
[!if WINX_USE_TOOLBAR]

void [!output WINX_MAINDLG_CLASS]::OnViewToolbar(HWND hWnd)
{
	static BOOL bVisible = TRUE;
	bVisible = !bVisible;
[!if WINX_USE_REBAR]
	ViewReBarBand(1,bVisible);
[!else]
	ViewToolBar(bVisible);
[!endif]
	UISetCheck(ID_VIEW_TOOLBAR,bVisible);
}

[!endif]
void [!output WINX_MAINDLG_CLASS]::OnAppAbout(HWND hWnd)
{
	CAboutDlg dlg;
	dlg.DoModal(m_hWnd);
}

void [!output WINX_MAINDLG_CLASS]::OnAppExit(HWND hWnd)
{
[!if WINX_USE_DDX]
	BOOL bSuccess = DoDataExchange(TRUE);
if (bSuccess)
	CloseDialog(hWnd);
[!else]
	CloseDialog(hWnd);
[!endif]
}
[!endif]

[!if WINX_APPTYPE_DLG || WINX_APPTYPE_DLG_MODAL]
[!if !WINX_APPTYPE_DLG_FRAME]

void [!output WINX_MAINDLG_CLASS]::OnAppAbout(HWND hWnd)
{
[!if WINX_APPTYPE_DLG_MODAL]
		winx::SimpleDialog dlg;
		dlg.DoModal(m_hWnd,IDD_ABOUTBOX);
[!else]
		CAboutDlg dlg;
		dlg.DoModal(m_hWnd);
[!endif]
}

[!if WINX_USE_MENUBAR]
void [!output WINX_MAINDLG_CLASS]::OnAppExit(HWND hWnd)
{
[!if WINX_USE_DDX]
	// TODO: Add validation code 
	BOOL bSuccess = DoDataExchange(TRUE);
	if (bSuccess)
		CloseDialog(hWnd);
[!else]
	CloseDialog(hWnd);
[!endif]		
}
[!endif] //[!if WINX_USE_MENUBAR]

void [!output WINX_MAINDLG_CLASS]::OnOK(HWND hWnd)
{
[!if WINX_USE_DDX]
	// TODO: Add validation code 
	BOOL bSuccess = DoDataExchange(TRUE);
	if (bSuccess)
		CloseDialog(hWnd);
[!else]
	CloseDialog(hWnd);
[!endif]
}

BOOL [!output WINX_MAINDLG_CLASS]::OnInitDialog(HWND hDlg, HWND hWndDefaultFocus)
{
	CenterWindow();
[!if WINX_VIEWTYPE_HTML]
	m_ie.DlgItem(hDlg, IDC_[!output UPPERCASE_SAFE_PROJECT_NAME]_VIEW);
		
	CComPtr<IWebBrowser2> spBrowser;
	m_ie.QueryControl(&spBrowser);

	CComVariant v;
	spBrowser->Navigate(CComBSTR("http://www.winxgui.com"), &v, &v, &v, &v);
[!endif]
[!if WINX_VIEWTYPE_EDIT || WINX_VIEWTYPE_LISTBOX || WINX_VIEWTYPE_LISTVIEW || WINX_VIEWTYPE_RICHEDIT || WINX_VIEWTYPE_TREE]
	winx::SubclassDlgItem(&m_pView,hDlg,IDC_[!output UPPERCASE_SAFE_PROJECT_NAME]_VIEW);
[!endif]
[!if WINX_USE_DDX]
	DoDataExchange();
[!endif]
	return TRUE;
}

[!endif]
[!endif]