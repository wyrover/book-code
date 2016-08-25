// [!output WINX_MAINDLG_FILE].h : interface of the [!output WINX_MAINDLG_CLASS] class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined([!output FILE_NAME_SYMBOL]_INCLUDED_)
#define [!output FILE_NAME_SYMBOL]_INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

[!if WINX_APPTYPE_DLG_FRAME]
class [!output WINX_MAINDLG_CLASS] : public [!output WINX_MAINDLG_BASE_CLASS]<[!output WINX_MAINDLG_CLASS], IDD_MAINDLG>, 
		public winx::FrameDialog<[!output WINX_MAINDLG_CLASS]>,
		public winx::UpdateUI<[!output WINX_MAINDLG_CLASS]>
{
public:
	WINX_MAINFRAME();
[!if WINX_USE_ACCEL]
	WINX_ACCELFRAME(); // enable accelerator
	WINX_ACCEL(IDR_MAINFRAME);
[!endif]

[!if WINX_USE_DDX]
	WINX_DDX_BEGIN() // data exchange
	WINX_DDX_END();
[!endif]

	WINX_UPDATEUI_BEGIN()
		WINX_UPDATEUI(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		WINX_UPDATEUI(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
	WINX_UPDATEUI_END();

	WINX_DLGRESIZE_BEGIN(TRUE)
[!if WINX_USE_VIEW || WINX_VIEWTYPE_HTML]
		WINX_DLGRESIZE(IDC_[!output UPPERCASE_SAFE_PROJECT_NAME]_VIEW, AnchorAll)
[!endif]
	WINX_DLGRESIZE_END();
	
	WINX_ICON(IDR_MAINFRAME); // icon
	
	WINX_CMDS_BEGIN_EX()
		WINX_CMD(ID_APP_EXIT, OnAppExit)
		WINX_CMD(ID_APP_ABOUT,OnAppAbout)
[!if WINX_USE_TOOLBAR]
		WINX_CMD(ID_VIEW_TOOLBAR,OnViewToolbar)
[!endif]
[!if WINX_USE_STATUSBAR]
		WINX_CMD(ID_VIEW_STATUS_BAR,OnViewStatusBar)
[!endif]
	WINX_CMDS_END_EX();
[!if WINX_USE_CMDBAR]	

	WTL::CCommandBarCtrl		m_CmdBar;
[!endif]
public:
[!if WINX_USE_CPP_FILES]
	BOOL winx_msg PreTranslateMessage(MSG* lpMsg);
[!else]
	BOOL winx_msg PreTranslateMessage(MSG* lpMsg)
	{
		if(WM_KEYDOWN == lpMsg->message &&
			(WPARAM)VK_ESCAPE ==  lpMsg->wParam )
		{   
			return  TRUE;
		}   

		return WindowBase::PreTranslateMessage(lpMsg);
	}
[!endif] //[!if WINX_USE_CPP_FILES]

[!if WINX_VIEWTYPE_HTML]
private:
	winx::AxCtrlHandle m_ie;
public:
[!endif]
[!if WINX_VIEWTYPE_EDIT || WINX_VIEWTYPE_LISTBOX || WINX_VIEWTYPE_LISTVIEW || WINX_VIEWTYPE_RICHEDIT || WINX_VIEWTYPE_TREE]
private:
	[!output WINX_VIEW_CLASS] * m_pView;
public:
[!endif]

[!if WINX_USE_CPP_FILES]
	BOOL OnInitDialog(HWND hDlg, HWND hWndDefaultFocus);
[!else]
	BOOL OnInitDialog(HWND hDlg, HWND hWndDefaultFocus)
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
[!endif] //[!if WINX_USE_CPP_FILES]
[!if WINX_USE_CPP_FILES]
	LRESULT winx_msg HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
[!else]
	LRESULT winx_msg HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lr = 0;
		if (FrameDialogClass::ProcessFrameMessage(hWnd,message,wParam,lParam,lr))
		{
			return lr;
		}

		return WindowBase::HandleMessage(hWnd, message, wParam, lParam);
	}
[!endif] //[!if WINX_USE_CPP_FILES]
[!if WINX_USE_STATUSBAR]
[!if WINX_USE_CPP_FILES]	
	void OnViewStatusBar(HWND hWnd);
[!else]
	void OnViewStatusBar(HWND hWnd)
	{
		static BOOL bVisible = TRUE;
		bVisible = !bVisible;
		ViewStatusBar(bVisible);
		UISetCheck(ID_VIEW_STATUS_BAR,bVisible);
	}
[!endif] //[!if WINX_USE_CPP_FILES]
[!endif]
[!if WINX_USE_TOOLBAR]
[!if WINX_USE_CPP_FILES]	
	void OnViewToolbar(HWND hWnd);
[!else]
	void OnViewToolbar(HWND hWnd)
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
[!endif] //[!if WINX_USE_CPP_FILES]
[!endif] 
[!if WINX_USE_CPP_FILES]	
	void OnAppAbout(HWND hWnd);
[!else]
	void OnAppAbout(HWND hWnd)
	{
		CAboutDlg dlg;
		dlg.DoModal(m_hWnd);
	}
[!endif] //[!if WINX_USE_CPP_FILES]
[!if WINX_USE_CPP_FILES]
	void OnAppExit(HWND hWnd);
[!else]
	void OnAppExit(HWND hWnd)
	{
[!if WINX_USE_DDX]
		BOOL bSuccess = DoDataExchange(TRUE);
		if (bSuccess)
			CloseDialog(hWnd);
[!else]
		CloseDialog(hWnd);
[!endif]
	}
[!endif] //[!if WINX_USE_CPP_FILES]
};
[!endif]

[!if WINX_APPTYPE_DLG || WINX_APPTYPE_DLG_MODAL]
[!if !WINX_APPTYPE_DLG_FRAME]
class [!output WINX_MAINDLG_CLASS] : public [!output WINX_MAINDLG_BASE_CLASS]<[!output WINX_MAINDLG_CLASS], IDD_MAINDLG>
{
public:
[!if !WINX_APPTYPE_DLG_MODAL]
	WINX_MAINFRAME();

[!endif]
[!if WINX_USE_ACCEL]
	WINX_ACCELFRAME(); // enable accelerator
	WINX_ACCEL(IDR_MAINFRAME);

[!endif]
	WINX_CMDS_BEGIN_EX() // command dispatcher
		WINX_CMD(ID_APP_ABOUT, OnAppAbout)
[!if WINX_USE_MENUBAR]
		WINX_CMD(ID_APP_EXIT, OnAppExit)
[!endif]
	WINX_CMDS_END_EX();

[!if WINX_USE_VIEW || WINX_VIEWTYPE_HTML]
	WINX_DLGRESIZE_BEGIN_NOGRIPPER(FALSE) // layout
		WINX_DLGRESIZE(IDC_[!output UPPERCASE_SAFE_PROJECT_NAME]_VIEW, AnchorAll)
	WINX_DLGRESIZE_END();

[!endif]
[!if WINX_USE_DDX]
	WINX_DDX_BEGIN() // data exchange
	WINX_DDX_END();

[!endif]
	WINX_ICON(IDR_MAINFRAME); // icon
public:
[!if WINX_USE_CPP_FILES]
	void OnAppAbout(HWND hWnd);
[!else]
	void OnAppAbout(HWND hWnd)
	{
[!if WINX_APPTYPE_DLG_MODAL]
		winx::SimpleDialog dlg;
		dlg.DoModal(m_hWnd,IDD_ABOUTBOX);
[!else]
		CAboutDlg dlg;
		dlg.DoModal(m_hWnd);
[!endif]
	}
[!endif] //[!if WINX_USE_CPP_FILES]
[!if WINX_USE_MENUBAR]
[!if WINX_USE_CPP_FILES]
	void OnAppExit(HWND hWnd);
[!else]
	void OnAppExit(HWND hWnd)
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
[!endif] //[!if WINX_USE_CPP_FILES]
[!endif]	
[!if WINX_USE_CPP_FILES]
	void OnOK(HWND hWnd);
[!else]
	void OnOK(HWND hWnd)
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
[!endif] //[!if WINX_USE_CPP_FILES]
[!if WINX_VIEWTYPE_HTML]
private:
	winx::AxCtrlHandle m_ie;
public:
[!endif]
[!if WINX_VIEWTYPE_EDIT || WINX_VIEWTYPE_LISTBOX || WINX_VIEWTYPE_LISTVIEW || WINX_VIEWTYPE_RICHEDIT || WINX_VIEWTYPE_TREE]
private:
	[!output WINX_VIEW_CLASS] * m_pView;
public:
[!endif]
[!if WINX_USE_CPP_FILES]
	BOOL OnInitDialog(HWND hDlg, HWND hWndDefaultFocus);
[!else]
	BOOL OnInitDialog(HWND hDlg, HWND hWndDefaultFocus)
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
[!endif] ////[!if WINX_USE_CPP_FILES]
};
[!endif]
[!endif]

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// [!output INSERT_LOCATION_COMMENT]

#endif // !defined([!output FILE_NAME_SYMBOL]_INCLUDED_)