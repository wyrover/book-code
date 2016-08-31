// [!output CODE_FILE_NAME].cpp : implementation of the [!output CODE_CLASS_NAME] class
//
/////////////////////////////////////////////////////////////////////////////
[!if CODE_USE_STDAFX]
#include "stdafx.h"
[!endif]
[!if CODE_USE_RESOURCE]
#include "resource.h"
[!endif]

#include "[!output CODE_FILE_NAME].h"

[!if CODE_USE_CONSTRUCTION]
[!output CODE_CLASS_NAME]::[!output CODE_CLASS_NAME]()
{
}
[!endif]
[!if CODE_USE_DESTRUCTION]
[!output CODE_CLASS_NAME]::~[!output CODE_CLASS_NAME]()
{
}
[!endif]

LRESULT [!output CODE_CLASS_NAME]::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());
[!if CODE_USE_DDX]
	DoDataExchange(false);
[!endif]
[!if CODE_USE_RESIZE]
	DlgResize_Init(true,false);
[!endif]
	return TRUE;
}

LRESULT [!output CODE_CLASS_NAME]::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT [!output CODE_CLASS_NAME]::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}