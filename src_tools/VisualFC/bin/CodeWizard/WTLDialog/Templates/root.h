// [!output CODE_FILE_NAME].h : interface of the [!output CODE_CLASS_NAME] class
//
/////////////////////////////////////////////////////////////////////////////
[!if CODE_USE_GUIDSYMBOL]
#if !defined([!output FILE_NAME_SYMBOL])
#define [!output FILE_NAME_SYMBOL]
[!else]
#ifndef [!output FILE_NAME_SYMBOL]
#define [!output FILE_NAME_SYMBOL]
[!endif]

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

[!if CODE_USE_UPUI || CODE_USE_DDX || CODE_USE_RESIZE || CODE_USE_MSGFILTER || CODE_USE_IDLE]
class [!output CODE_CLASS_NAME] : public [!output CODE_BASE_CLASS]<[!output CODE_CLASS_NAME]>,
[!else]
class [!output CODE_CLASS_NAME] : public [!output CODE_BASE_CLASS]<[!output CODE_CLASS_NAME]>
[!endif]
[!if CODE_USE_UPUI]
[!if CODE_USE_DDX || CODE_USE_RESIZE || CODE_USE_MSGFILTER || CODE_USE_IDLE]
	public CUpdateUI<[!output CODE_CLASS_NAME]>,
[!else]
	public CUpdateUI<[!output CODE_CLASS_NAME]>
[!endif]
[!endif]
[!if CODE_USE_DDX]
[!if CODE_USE_RESIZE || CODE_USE_MSGFILTER || CODE_USE_IDLE]
	public CWinDataExchange<[!output CODE_CLASS_NAME]>,
[!else]
	public CWinDataExchange<[!output CODE_CLASS_NAME]>
[!endif]
[!endif]
[!if CODE_USE_RESIZE]
[!if CODE_USE_MSGFILTER || CODE_USE_IDLE]
	public CDialogResize<[!output CODE_CLASS_NAME]>,
[!else]
	public CDialogResize<[!output CODE_CLASS_NAME]>
[!endif]
[!endif]
[!if CODE_USE_MSGFILTER]
[!if CODE_USE_IDLE]
	public CMessageFilter,
[!else]
	public CMessageFilter
[!endif]
[!endif]
[!if CODE_USE_IDLE]
	public CIdleHandler
[!endif]
{
public:
[!if CODE_USE_CONSTRUCTION]
[!if CODE_USE_CPP_FILES]
	[!output CODE_CLASS_NAME]();
[!else]
	[!output CODE_CLASS_NAME]()
	{
	}
[!endif]
[!endif]
[!if CODE_USE_DESTRUCTION]
[!if CODE_USE_CPP_FILES]
	virtual ~[!output CODE_CLASS_NAME]();
[!else]
	virtual ~[!output CODE_CLASS_NAME]()
	{
	}
[!endif]

[!endif]
	enum { IDD = [!output CODE_DIALOG_ID] };

[!if CODE_USE_CPP_FILES]
	virtual BOOL PreTranslateMessage(MSG* pMsg);
[!else]
	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}
[!endif]
[!if CODE_USE_IDLE]
[!if CODE_USE_CPP_FILES]
	virtual BOOL OnIdle();
[!else]
	virtual BOOL OnIdle()
	{
		return FALSE;
	}
[!endif]
[!endif]
[!if CODE_USE_UPUI]

	BEGIN_UPDATE_UI_MAP([!output CODE_CLASS_NAME])
	END_UPDATE_UI_MAP()
[!endif]
[!if CODE_USE_DDX]

	BEGIN_DDX_MAP([!output CODE_CLASS_NAME])
	END_DDX_MAP()
[!endif]
[!if CODE_USE_RESIZE]

	BEGIN_DLGRESIZE_MAP([!output CODE_CLASS_NAME])
	END_DLGRESIZE_MAP()
[!endif]

	BEGIN_MSG_MAP([!output CODE_CLASS_NAME])
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
[!if CODE_USE_MSGFILTER || CODE_USE_IDLE]
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
[!endif]
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
[!if CODE_USE_UPUI]
		CHAIN_MSG_MAP(CUpdateUI<[!output CODE_CLASS_NAME]>)
[!endif]
[!if CODE_USE_RESIZE]
		CHAIN_MSG_MAP(CDialogResize<[!output CODE_CLASS_NAME]>)
[!endif]
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

[!if CODE_USE_CPP_FILES]
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
[!else]
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow(GetParent());

[!if CODE_USE_MSGFILTER || CODE_USE_IDLE]
		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
[!endif]
[!if CODE_USE_MSGFILTER]
		pLoop->AddMessageFilter(this);
[!endif]
[!if CODE_USE_IDLE]
		pLoop->AddIdleHandler(this);
[!endif]

[!if CODE_USE_DDX]
		DoDataExchange(false);
[!endif]
[!if CODE_USE_RESIZE]
		DlgResize_Init(true,false);
[!endif]
[!if CODE_USE_UPUI]
		UIAddChildWindowContainer(m_hWnd);
[!endif]
		return TRUE;
	}

[!endif]
[!if CODE_USE_MSGFILTER || CODE_USE_IDLE]
[!if CODE_USE_CPP_FILES]
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
[!else]
	LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// unregister message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
[!if CODE_USE_MSGFILTER]
		pLoop->RemoveMessageFilter(this);
[!endif]
[!if CODE_USE_IDLE]
		pLoop->RemoveIdleHandler(this);
[!endif]
		return 0;
	}
[!endif]
[!endif]
[!if CODE_USE_CPP_FILES]
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
[!else]
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CloseDialog(wID);
		return 0;
	}
[!endif]
[!if CODE_USE_CPP_FILES]
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
[!else]
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CloseDialog(wID);
		return 0;
	}
[!endif]
[!if CODE_USE_CPP_FILES]
	void CloseDialog(int nVal);
[!else]
	void CloseDialog(int nVal)
	{
		DestroyWindow();
		//if class is maindlg
		//::PostQuitMessage(nVal);
	}
[!endif]
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// [!output INSERT_LOCATION_COMMENT]
[!if CODE_USE_GUIDSYMBOL]
#endif // !defined([!output FILE_NAME_SYMBOL])
[!else]
#endif // [!output FILE_NAME_SYMBOL]
[!endif]