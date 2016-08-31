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

LRESULT [!output CODE_CLASS_NAME]::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());
	return TRUE;
}

LRESULT [!output CODE_CLASS_NAME]::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}
