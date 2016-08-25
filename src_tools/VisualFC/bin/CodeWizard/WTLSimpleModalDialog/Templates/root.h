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

class [!output CODE_CLASS_NAME] : public CDialogImpl<[!output CODE_CLASS_NAME]>
{
public:
	enum { IDD = [!output CODE_DIALOG_ID] };

	BEGIN_MSG_MAP([!output CODE_CLASS_NAME])
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
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
		return TRUE;
	}

[!endif]
[!if CODE_USE_CPP_FILES]
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
[!else]
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
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