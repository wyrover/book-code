// [!output WTL_MAINDLG_FILE].h : interface of the [!output WTL_MAINDLG_CLASS] class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined([!output FILE_NAME_SYMBOL]_INCLUDED_)
#define [!output FILE_NAME_SYMBOL]_INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

[!if WTL_APPTYPE_DLG && WTL_APPTYPE_DLG_FRAME]
class [!output WTL_MAINDLG_CLASS] : public [!output WTL_MAINDLG_BASE_CLASS]<[!output WTL_MAINDLG_CLASS]>, public CUpdateUI<[!output WTL_MAINDLG_CLASS]>,
		public CMessageFilter, public CIdleHandler, 
		public CFrameDialog<[!output WTL_MAINDLG_CLASS]>, public CDialogResize<[!output WTL_MAINDLG_CLASS]>
{
public:
	enum { IDD = IDD_MAINDLG };

[!if WTL_USE_CMDBAR]
	CCommandBarCtrl m_CmdBar;
[!endif]

[!if WTL_USE_CPP_FILES]
	virtual BOOL PreTranslateMessage(MSG* pMsg);
[!else]
	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
[!if WTL_HOST_AX]
		if((pMsg->message < WM_KEYFIRST || pMsg->message > WM_KEYLAST) &&
		   (pMsg->message < WM_MOUSEFIRST || pMsg->message > WM_MOUSELAST))
			return FALSE;

		HWND hWndCtl = ::GetFocus();
		if(IsChild(hWndCtl))
		{
			// find a direct child of the dialog from the window that has focus
			while(::GetParent(hWndCtl) != m_hWnd)
				hWndCtl = ::GetParent(hWndCtl);

			// give control a chance to translate this message
			if(::SendMessage(hWndCtl, WM_FORWARDMSG, 0, (LPARAM)pMsg) != 0)
				return TRUE;
		}

[!endif]
		return CFrameDialog<[!output WTL_MAINDLG_CLASS]>::PreTranslateMessage(pMsg);
	}

[!endif]
[!if WTL_USE_CPP_FILES]
	virtual BOOL OnIdle();
[!else]
	virtual BOOL OnIdle()
	{
[!if WTL_USE_TOOLBAR]
		UIUpdateToolBar();
[!endif]
		return FALSE;
	}
[!endif]

	BEGIN_DLGRESIZE_MAP([!output WTL_MAINDLG_CLASS])
	END_DLGRESIZE_MAP()

	BEGIN_UPDATE_UI_MAP([!output WTL_MAINDLG_CLASS])
[!if WTL_USE_TOOLBAR]
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
[!endif]
[!if WTL_USE_STATUSBAR]
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
[!endif]
[!if WTL_APPTYPE_EXPLORER]
		UPDATE_ELEMENT(ID_VIEW_TREEPANE, UPDUI_MENUPOPUP)
[!endif]
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP([!output WTL_MAINDLG_CLASS])
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnAppExit)
[!if WTL_USE_TOOLBAR]
		COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
[!endif]
[!if WTL_USE_STATUSBAR]
		COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
[!endif]
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		CHAIN_MSG_MAP(CUpdateUI<CMainDlg>)
		CHAIN_MSG_MAP(CFrameDialog<CMainDlg>)
		CHAIN_MSG_MAP(CDialogResize<CMainDlg>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

[!if WTL_USE_CPP_FILES]
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
[!else]
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// center the dialog on the screen
		CenterWindow();

		// set icons
		HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
		SetIcon(hIconSmall, FALSE);
		//load accelerator
		LoadSimpleAccel(IDR_MAINFRAME);
[!if WTL_USE_TOOLBAR]
[!if WTL_USE_REBAR]
[!if WTL_USE_CMDBAR]
		// create command bar window
		HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
		// attach menu
		m_CmdBar.AttachMenu(GetMenu());
		// load command bar images
		m_CmdBar.LoadImages(IDR_MAINFRAME);
		// remove old menu
		SetMenu(NULL);

[!endif]
		HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

		CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
[!if WTL_USE_CMDBAR]
		AddSimpleReBarBand(hWndCmdBar);
		AddSimpleReBarBand(hWndToolBar, NULL, TRUE);
[!else]
		AddSimpleReBarBand(hWndToolBar);
[!endif]
[!else]
		CreateSimpleToolBar(IDR_MAINFRAME);
[!endif]
[!endif]
[!if WTL_USE_STATUSBAR]

		CreateSimpleStatusBar();
[!endif]

		//frame dialog reposition dialog
		RepositionDialog();

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		UIAddChildWindowContainer(m_hWnd);
[!if WTL_USE_TOOLBAR]
[!if WTL_USE_REBAR]

		UIAddToolBar(hWndToolBar);
[!else]

		UIAddToolBar(m_hWndToolBar);
[!endif]
		UISetCheck(ID_VIEW_TOOLBAR, 1);
[!endif]
[!if WTL_USE_STATUSBAR]
		UISetCheck(ID_VIEW_STATUS_BAR, 1);
[!endif]

		DlgResize_Init();
		return TRUE;
	}

[!endif]
[!if WTL_USE_CPP_FILES]
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
[!else]
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// unregister message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);
[!if WTL_COM_SERVER]

		// if UI is the last thread, no need to wait
		if(_Module.GetLockCount() == 1)
		{
			_Module.m_dwTimeOut = 0L;
			_Module.m_dwPause = 0L;
		}
		_Module.Unlock();
[!endif]

		return 0;
	}

[!endif]
[!if WTL_USE_CPP_FILES]
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
[!else]
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return 0;
	}

[!endif]
[!if WTL_USE_CPP_FILES]
	LRESULT OnAppExit(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
[!else]
	LRESULT OnAppExit(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: Add validation code 
		CloseDialog(wID);
		return 0;
	}

[!endif]
[!if WTL_USE_CPP_FILES]
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
[!else]
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CloseDialog(wID);
		return 0;
	}

[!endif]
[!if WTL_USE_CPP_FILES]
	void CloseDialog(int nVal);
[!else]
	void CloseDialog(int nVal)
	{
		DestroyWindow();
		::PostQuitMessage(nVal);
	}
[!endif]
[!if WTL_USE_TOOLBAR]
[!if WTL_USE_CPP_FILES]
	LRESULT OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
[!else]

	LRESULT OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		static BOOL bVisible = TRUE;
		bVisible = !bVisible;
[!if WTL_USE_REBAR]
		ViewReBarBand(1,bVisible);
[!else]
		ViewToolBar(bVisible);
[!endif]
		UISetCheck(ID_VIEW_TOOLBAR, bVisible);
		return 0;
	}
[!endif]
[!endif]
[!if WTL_USE_STATUSBAR]
[!if WTL_USE_CPP_FILES]
	LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
[!else]

	LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		static BOOL bVisible = TRUE;
		bVisible = !bVisible;
		ViewStatusBar(bVisible);
		UISetCheck(ID_VIEW_STATUS_BAR,bVisible);
		return 0;
	}
[!endif]
[!endif]
};
[!endif]


[!if WTL_APPTYPE_DLG && !WTL_APPTYPE_DLG_MODAL && !WTL_APPTYPE_DLG_FRAME]
class [!output WTL_MAINDLG_CLASS] : public [!output WTL_MAINDLG_BASE_CLASS]<[!output WTL_MAINDLG_CLASS]>, public CUpdateUI<[!output WTL_MAINDLG_CLASS]>,
		public CMessageFilter, public CIdleHandler
{
public:
	enum { IDD = IDD_MAINDLG };

[!if WTL_USE_CPP_FILES]
	virtual BOOL PreTranslateMessage(MSG* pMsg);
[!else]
	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
[!if WTL_HOST_AX]
		if((pMsg->message < WM_KEYFIRST || pMsg->message > WM_KEYLAST) &&
		   (pMsg->message < WM_MOUSEFIRST || pMsg->message > WM_MOUSELAST))
			return FALSE;

		HWND hWndCtl = ::GetFocus();
		if(IsChild(hWndCtl))
		{
			// find a direct child of the dialog from the window that has focus
			while(::GetParent(hWndCtl) != m_hWnd)
				hWndCtl = ::GetParent(hWndCtl);

			// give control a chance to translate this message
			if(::SendMessage(hWndCtl, WM_FORWARDMSG, 0, (LPARAM)pMsg) != 0)
				return TRUE;
		}

[!endif]
		return CWindow::IsDialogMessage(pMsg);
	}

[!endif]
[!if WTL_USE_CPP_FILES]
	virtual BOOL OnIdle();
[!else]
	virtual BOOL OnIdle()
	{
		return FALSE;
	}
[!endif]

	BEGIN_UPDATE_UI_MAP([!output WTL_MAINDLG_CLASS])
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP([!output WTL_MAINDLG_CLASS])
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

[!if WTL_USE_CPP_FILES]
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
[!else]
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// center the dialog on the screen
		CenterWindow();

		// set icons
		HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
		SetIcon(hIconSmall, FALSE);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		UIAddChildWindowContainer(m_hWnd);

		return TRUE;
	}

[!endif]
[!if WTL_USE_CPP_FILES]
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
[!else]
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// unregister message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);
[!if WTL_COM_SERVER]

		// if UI is the last thread, no need to wait
		if(_Module.GetLockCount() == 1)
		{
			_Module.m_dwTimeOut = 0L;
			_Module.m_dwPause = 0L;
		}
		_Module.Unlock();
[!endif]

		return 0;
	}

[!endif]
[!if WTL_USE_CPP_FILES]
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
[!else]
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return 0;
	}

[!endif]
[!if WTL_USE_CPP_FILES]
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
[!else]
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: Add validation code 
		CloseDialog(wID);
		return 0;
	}

[!endif]
[!if WTL_USE_CPP_FILES]
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
[!else]
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CloseDialog(wID);
		return 0;
	}

[!endif]
[!if WTL_USE_CPP_FILES]

	void CloseDialog(int nVal);
[!else]
	void CloseDialog(int nVal)
	{
		DestroyWindow();
		::PostQuitMessage(nVal);
	}
[!endif]
};
[!endif]
[!if WTL_APPTYPE_DLG && WTL_APPTYPE_DLG_MODAL]
class [!output WTL_MAINDLG_CLASS] : public [!output WTL_MAINDLG_BASE_CLASS]<[!output WTL_MAINDLG_CLASS]>
{
public:
	enum { IDD = IDD_MAINDLG };

	BEGIN_MSG_MAP([!output WTL_MAINDLG_CLASS])
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
[!if WTL_COM_SERVER]
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
[!endif]
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

[!if WTL_USE_CPP_FILES]
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
[!else]
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
[!if WTL_COM_SERVER]
		_Module.Lock();

[!endif]
		// center the dialog on the screen
		CenterWindow();

		// set icons
		HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
		SetIcon(hIconSmall, FALSE);

		return TRUE;
	}

[!endif]
[!if WTL_COM_SERVER]
[!if WTL_USE_CPP_FILES]
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
[!else]
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// if UI is the last thread, no need to wait
		if(_Module.GetLockCount() == 1)
		{
			_Module.m_dwTimeOut = 0L;
			_Module.m_dwPause = 0L;
		}
		_Module.Unlock();

		return 0;
	}

[!endif]
[!endif]
[!if WTL_USE_CPP_FILES]
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
[!else]
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
		dlg.DoModal();
		return 0;
	}

[!endif]
[!if WTL_USE_CPP_FILES]
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
[!else]
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: Add validation code 
		EndDialog(wID);
		return 0;
	}

[!endif]
[!if WTL_USE_CPP_FILES]
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
[!else]
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}
[!endif]
};
[!endif]

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// [!output INSERT_LOCATION_COMMENT]

#endif // !defined([!output FILE_NAME_SYMBOL]_INCLUDED_)