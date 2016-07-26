// Main.cpp: 包含程序逻辑的可执行文件的代码
//
// 版权所有(C) Microsoft Corporation。保留所有权利。

#include "stdafx.h"
#include "Main.h"
#define MAX_LOADSTRING	100	
#define MAX_LANGNAME	255		// 最大本机语言名称长度

// 类型定义
typedef struct LANGINFO_DEF {
	int		Count;
	LANGID	LangID;
} LANGINFO;
typedef LANGINFO *PLANGINFO;

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 标题栏文本
LANGID UILanguage;								// 用户选择的用户界面语言
HMODULE hSatDLL;								// 附属 DLL 模块的句柄

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	Language(HWND, UINT, WPARAM, LPARAM);
int					PopulateLanguages(HWND);
LANGID				DetectLanguage(void);
HMODULE				LoadSatelliteDLL(LANGID);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE,
                     LPTSTR,
                     int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	UILanguage = DetectLanguage();
	hSatDLL = LoadSatelliteDLL(UILanguage);
	if( hSatDLL == NULL ) hSatDLL = hInstance;

	// 初始化全局字符串
	LoadString(hSatDLL, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hSatDLL, IDC_MAIN, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hSatDLL, (LPCTSTR)IDC_MAIN);

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册 Window 类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hSatDLL, (LPCTSTR)IDI_MAIN);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_MAIN;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(hSatDLL, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HANDLE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, unsigned, WORD, LONG)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND - 处理应用程序菜单
//  WM_PAINT - 绘制主窗口
//  WM_DESTROY - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	HFONT hfnt;
	LOGFONT lf;
	TCHAR szHello[MAX_LOADSTRING];

	// 指定要使用的字体(存储在 LOGFONT 结构中)
	lf.lfHeight = 20;
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = FW_DONTCARE;
	lf.lfItalic = FALSE;
	lf.lfUnderline = FALSE;
	lf.lfStrikeOut = FALSE;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = FF_DONTCARE;
	_tcsncpy_s(lf.lfFaceName, sizeof(lf.lfFaceName)/sizeof(TCHAR), _T("MS Shell Dlg"), 12);

	switch (message) 
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hSatDLL, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case ID_FILE_CHOOSELANGUAGE:
			DialogBox(hSatDLL, (LPCTSTR)IDD_DIALOG, hWnd, (DLGPROC)Language);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		RECT rt;
		GetClientRect(hWnd, &rt);
		hfnt = CreateFontIndirect(&lf); 
		SelectObject(hdc, hfnt);
		rt.top += 20;
		rt.left += 20;
		LoadString(hSatDLL, IDS_HELLO, szHello, MAX_LOADSTRING);
		DrawText(hdc, szHello, _tcslen(szHello), &rt, DT_LEFT);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

// 语言选择框的消息处理程序。
LRESULT CALLBACK Language(HWND hDlg, UINT message, WPARAM wParam, LPARAM)
{
	int		index;
	LRESULT	NewUILanguage;
	HWND	hwndCombo = GetDlgItem(hDlg,IDC_LANGUAGE);
	HWND	hMainWindow = GetParent(hDlg);
	HMENU	hNewMenu;


	switch (message)
	{
	case WM_INITDIALOG:
		PopulateLanguages(hwndCombo);
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
			case IDOK:
				index = SendMessage(hwndCombo, CB_GETCURSEL, 0, 0);
				if( index != CB_ERR ) {
					NewUILanguage = SendMessage(hwndCombo, CB_GETITEMDATA, index, 0);
					if( NewUILanguage != CB_ERR && NewUILanguage != UILanguage ) {
						UILanguage = (LANGID)NewUILanguage;
						hSatDLL = LoadSatelliteDLL(UILanguage);
						if( hSatDLL == NULL ) hSatDLL = hInst;
						// 重绘主窗口的工作区
						InvalidateRect(hMainWindow,NULL,TRUE);
						UpdateWindow(hMainWindow);
						// 更新主窗口的标题
						LoadString(hSatDLL, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
						SendMessage(hMainWindow,WM_SETTEXT, NULL, (LPARAM)szTitle);
						// 加载本地化菜单
						DestroyMenu(GetMenu(hMainWindow));
						hNewMenu = LoadMenu(hSatDLL,(LPCTSTR)IDC_MAIN);
						SetMenu(hMainWindow, hNewMenu);
						DrawMenuBar(hMainWindow);
					}
				}
			case IDCANCEL:
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
		}
		break;
	}
	return FALSE;
}

// 填充选择对话框中的语言列表
// 根据用十进制 LCID 命名的子目录
int PopulateLanguages(HWND hwndCombo) {
	TCHAR				CurrentDirectory[MAX_PATH];
	int					AvailableLangID;
	TCHAR				AvailableLangName[MAX_LANGNAME];
	WIN32_FIND_DATA		FindFileData;
	HANDLE				hDir;
	int					i = 0;
	int					j;
	int					CurrentIndex = 0;

	// 检索当前目录名
	if( GetCurrentDirectory(MAX_PATH, CurrentDirectory)) {
		if( _tcsclen(CurrentDirectory) < MAX_PATH-4 )
			_tcscat_s(CurrentDirectory, sizeof(CurrentDirectory)/sizeof(TCHAR), _T("\\*.*"));
	}
	else
		return 0;

	// 循环访问所有目录并填充语言选择的
	// 下拉列表
	hDir = FindFirstFile(CurrentDirectory, &FindFileData);
	if( hDir == INVALID_HANDLE_VALUE ) {
		FindClose(hDir);
		return 0;
	}
	SendMessage(hwndCombo, CB_RESETCONTENT, 0, 0);
	do {
		if( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
			AvailableLangID = _ttoi(FindFileData.cFileName);
			if( AvailableLangID ) {
				if( GetLocaleInfo(AvailableLangID,LOCALE_SNATIVELANGNAME,AvailableLangName,MAX_LANGNAME)) {
					CurrentIndex = SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)AvailableLangName);
					SendMessage(hwndCombo, CB_SETITEMDATA, (WPARAM)CurrentIndex, (LPARAM)AvailableLangID);
					i++;
				}
			}
		}
	} while ( FindNextFile(hDir, &FindFileData) );
	FindClose(hDir);
	// 为了设置当前选择，我们必须循环访问组合框
	// 因为添加操作按字母顺序排序组合框
	for(j=0 ; j<=i ; j++) {
		if( UILanguage == SendMessage(hwndCombo, CB_GETITEMDATA, j, 0) )
			SendMessage(hwndCombo, CB_SETCURSEL, (WPARAM)j, 0);
	}
	return i;

}

// 加载为语言 DesiredLanguage 指定的附属 DLL
HMODULE		LoadSatelliteDLL(LANGID DesiredLanguage) {
	TCHAR		BaseDirectory[MAX_PATH];
	TCHAR		SatellitePath[MAX_PATH];
	TCHAR		buffer[100];
	HMODULE		hDLL;

	// 获取附属 DLL 搜索的基目录
	if( GetCurrentDirectory(MAX_PATH, BaseDirectory)) {
		_tcscat_s(BaseDirectory, sizeof(BaseDirectory)/sizeof(TCHAR), _T("\\"));
	}
	else
		return NULL;

	// 先尝试加载具有完全指定语言的库
	_tcscpy_s(SatellitePath, sizeof(SatellitePath)/sizeof(TCHAR), BaseDirectory);
	_itot_s(DesiredLanguage,buffer, sizeof(buffer)/sizeof(TCHAR), 10);
	_tcscat_s(SatellitePath, sizeof(SatellitePath)/sizeof(TCHAR), buffer);
	_tcscat_s(SatellitePath, sizeof(SatellitePath)/sizeof(TCHAR), _T("\\Satellite.dll"));
	hDLL = LoadLibrary(SatellitePath);
	if( hDLL )
		return hDLL;
	else {   // 尝试主语言 ID
		_tcscpy_s(SatellitePath, sizeof(SatellitePath)/sizeof(TCHAR), BaseDirectory);
		DesiredLanguage = PRIMARYLANGID(DesiredLanguage);
		_itot_s(DesiredLanguage,buffer,sizeof(buffer)/sizeof(TCHAR),10);
		_tcscat_s(SatellitePath, sizeof(SatellitePath)/sizeof(TCHAR), buffer);
		_tcscat_s(SatellitePath, sizeof(SatellitePath)/sizeof(TCHAR), _T("\\Satellite.dll"));
		hDLL = LoadLibrary(SatellitePath);
		if( hDLL )
			return hDLL;
		else
			return NULL;
	}
}

// 此函数为所有平台(Windows XP、Windows Vista 等)检测
// 正确的初始 UI 语言
LANGID DetectLanguage() {
     LANGID uiLangID = GetUserDefaultUILanguage();

    if (uiLangID == 0) {
        uiLangID = GetUserDefaultLangID();
    }
    // 返回找到的语言 ID。
    return (uiLangID);
}
