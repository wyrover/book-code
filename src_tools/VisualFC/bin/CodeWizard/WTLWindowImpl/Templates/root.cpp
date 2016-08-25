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
BOOL [!output CODE_CLASS_NAME]::PreTranslateMessage(MSG* pMsg)
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
[!if WTL_VIEWTYPE_SCROLL]

void [!output CODE_CLASS_NAME]::DoPaint(CDCHandle dc)
{
	//TODO: Add your drawing code here
}
[!endif]
[!if WTL_VIEWTYPE_GENERIC]

LRESULT [!output CODE_CLASS_NAME]::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CPaintDC dc(m_hWnd);

	//TODO: Add your drawing code here

	return 0;
}
[!endif]
