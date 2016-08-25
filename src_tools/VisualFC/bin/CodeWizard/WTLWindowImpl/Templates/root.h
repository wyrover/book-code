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

[!if WTL_VIEWTYPE_GENERIC || WTL_VIEWTYPE_FORM || WTL_VIEWTYPE_SCROLL]
class [!output CODE_CLASS_NAME] : public [!output CODE_BASE_CLASS]<[!output CODE_CLASS_NAME]>
[!else]
class [!output CODE_CLASS_NAME] : public CWindowImpl<[!output CODE_CLASS_NAME], [!output CODE_BASE_CLASS]>
[!endif]
{
public:
[!if WTL_VIEWTYPE_GENERIC || WTL_VIEWTYPE_SCROLL]
	DECLARE_WND_CLASS(NULL)
[!else]
[!if WTL_VIEWTYPE_FORM]
	enum { IDD = IDD_[!output UPPERCASE_SAFE_PROJECT_NAME]_FORM };
[!else]
	DECLARE_WND_SUPERCLASS(NULL, [!output CODE_BASE_CLASS]::GetWndClassName())
[!endif]
[!endif]
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
[!if CODE_USE_CPP_FILES]

	BOOL PreTranslateMessage(MSG* pMsg);
[!else]

	BOOL PreTranslateMessage(MSG* pMsg)
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
[!if WTL_VIEWTYPE_HTML]
		if((pMsg->message < WM_KEYFIRST || pMsg->message > WM_KEYLAST) &&
		   (pMsg->message < WM_MOUSEFIRST || pMsg->message > WM_MOUSELAST))
			return FALSE;

		// give HTML page a chance to translate this message
		return (BOOL)SendMessage(WM_FORWARDMSG, 0, (LPARAM)pMsg);
[!else]
[!if WTL_VIEWTYPE_FORM]
		return CWindow::IsDialogMessage(pMsg);
[!else]
		pMsg;
		return FALSE;
[!endif]
[!endif]
	}
[!endif]
[!if WTL_VIEWTYPE_SCROLL]
[!if CODE_USE_CPP_FILES]

	void DoPaint(CDCHandle dc);
[!else]

	void DoPaint(CDCHandle dc)
	{
		//TODO: Add your drawing code here
	}
[!endif]
[!endif]

	BEGIN_MSG_MAP([!output CODE_CLASS_NAME])
[!if WTL_VIEWTYPE_GENERIC]
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
[!endif]
[!if WTL_VIEWTYPE_SCROLL]
		CHAIN_MSG_MAP([!output CODE_BASE_CLASS]<[!output CODE_CLASS_NAME]>)
[!endif]
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
[!if WTL_VIEWTYPE_GENERIC]
[!if CODE_USE_CPP_FILES]

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
[!else]

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CPaintDC dc(m_hWnd);

		//TODO: Add your drawing code here

		return 0;
	}
[!endif]
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