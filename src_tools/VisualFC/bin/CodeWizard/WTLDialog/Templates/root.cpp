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
	return CWindow::IsDialogMessage(pMsg);
}

[!if CODE_USE_IDLE]
BOOL [!output CODE_CLASS_NAME]::OnIdle()
{
	return FALSE;
}
[!endif]

LRESULT [!output CODE_CLASS_NAME]::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
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

[!if CODE_USE_MSGFILTER || CODE_USE_IDLE]
LRESULT [!output CODE_CLASS_NAME]::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
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

LRESULT [!output CODE_CLASS_NAME]::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

LRESULT [!output CODE_CLASS_NAME]::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void [!output CODE_CLASS_NAME]::CloseDialog(int nVal)
{
	DestroyWindow();
	//if class is maindlg
	//::PostQuitMessage(nVal);
}