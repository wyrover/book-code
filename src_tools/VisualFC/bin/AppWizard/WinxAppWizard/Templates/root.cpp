// [!output PROJECT_NAME].cpp : main source file for [!output PROJECT_NAME].exe
//

#include "stdafx.h"

#include "resource.h"

[!if WINX_USE_VIEW]
#include "[!output WINX_VIEW_FILE].h"
[!endif]

[!if !WINX_APPTYPE_DLG_MODAL]
#include "aboutdlg.h"
[!endif]

[!if WINX_APPTYPE_DLG || WINX_APPTYPE_DLG_MODAL]
#include "[!output WINX_MAINDLG_FILE].h"
[!endif]

CComModule _Module;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
	InitCommonControls(ICC_WIN95_CLASSES);
[!if WINX_VIEWTYPE_RICHEDIT]

	InitRichEditControl();
[!endif]
[!if WINX_USE_LOOKNFEEL]

	WINX_APP_LOOKNFEEL(hInstance, IDR_MAINFRAME);
[!endif]
[!if WINX_VIEWTYPE_GENERIC || WINX_VIEWTYPE_SCROLL]

	[!output WINX_VIEW_CLASS]::RegisterClass();
[!endif]
[!if WINX_USE_GDIPLUS]

	GdiplusAppInit gdiplus;
[!endif]

	CComModuleInit module;

[!if WINX_APPTYPE_DLG]
	CMainDlg dlg;
	dlg.Create(NULL);
	dlg.ShowWindow(SW_SHOW);

	return RunMsgLoop();
[!endif]
[!if WINX_APPTYPE_DLG_MODAL]
	CMainDlg dlg;
	return dlg.DoModal();
[!endif]
}

