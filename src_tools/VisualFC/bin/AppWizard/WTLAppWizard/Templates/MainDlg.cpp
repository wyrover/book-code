// [!output WTL_MAINDLG_FILE].cpp : implementation of the [!output WTL_MAINDLG_CLASS] class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

[!if WTL_APPTYPE_DLG && !WTL_APPTYPE_DLG_MODAL]
#include "aboutdlg.h"
[!endif]
#include "[!output WTL_MAINDLG_FILE].h"

[!if WTL_APPTYPE_DLG && WTL_APPTYPE_DLG_FRAME]
BOOL [!output WTL_MAINDLG_CLASS]::PreTranslateMessage(MSG* pMsg)
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

BOOL [!output WTL_MAINDLG_CLASS]::OnIdle()
{
[!if WTL_USE_TOOLBAR]
	UIUpdateToolBar();
[!endif]
	return FALSE;
}

LRESULT [!output WTL_MAINDLG_CLASS]::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
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

LRESULT [!output WTL_MAINDLG_CLASS]::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
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

LRESULT [!output WTL_MAINDLG_CLASS]::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT [!output WTL_MAINDLG_CLASS]::OnAppExit(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	CloseDialog(wID);
	return 0;
}

LRESULT [!output WTL_MAINDLG_CLASS]::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void [!output WTL_MAINDLG_CLASS]::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}

LRESULT [!output WTL_MAINDLG_CLASS]::OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
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

LRESULT [!output WTL_MAINDLG_CLASS]::OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	static BOOL bVisible = TRUE;
	bVisible = !bVisible;
	ViewStatusBar(bVisible);
	UISetCheck(ID_VIEW_STATUS_BAR,bVisible);
	return 0;
}
[!endif]


[!if WTL_APPTYPE_DLG && !WTL_APPTYPE_DLG_MODAL && !WTL_APPTYPE_DLG_FRAME]
BOOL [!output WTL_MAINDLG_CLASS]::PreTranslateMessage(MSG* pMsg)
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

BOOL [!output WTL_MAINDLG_CLASS]::OnIdle()
{
	return FALSE;
}

LRESULT [!output WTL_MAINDLG_CLASS]::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
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

LRESULT [!output WTL_MAINDLG_CLASS]::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
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

LRESULT [!output WTL_MAINDLG_CLASS]::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT [!output WTL_MAINDLG_CLASS]::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	CloseDialog(wID);
	return 0;
}

LRESULT [!output WTL_MAINDLG_CLASS]::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void [!output WTL_MAINDLG_CLASS]::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}
[!endif]
[!if WTL_APPTYPE_DLG && WTL_APPTYPE_DLG_MODAL]
LRESULT [!output WTL_MAINDLG_CLASS]::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
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

[!if WTL_COM_SERVER]
LRESULT [!output WTL_MAINDLG_CLASS]::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
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
LRESULT [!output WTL_MAINDLG_CLASS]::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
	dlg.DoModal();
	return 0;
}

LRESULT [!output WTL_MAINDLG_CLASS]::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	EndDialog(wID);
	return 0;
}

LRESULT [!output WTL_MAINDLG_CLASS]::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}
[!endif]
