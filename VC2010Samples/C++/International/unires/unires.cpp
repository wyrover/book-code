// unires.cpp: 定义应用程序的入口点。
//
// 版权所有(C) Microsoft Corporation。保留所有权利。
//
// 备注: 此示例仅在 Windows 2000 或更高版本上运行，并需要
// 对所有要安装的语言的语言支持(在“控制面板”中
// “区域选项”的“系统的语言设置”之下查看)。
//

#include "stdafx.h"
#include "resource.h"

#define MAX_LOADSTRING 100
#define MAX_LANGUAGES 50

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 标题栏文本

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(HINSTANCE hInstance,
                      HINSTANCE ,
                      LPTSTR     ,
                      int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_UNIRES, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_UNIRES);

	// 主消息循环:

	BOOL bRet;
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) 
	{
			if(bRet == -1) {
					return -1;
			}

		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
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
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_UNIRES);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_UNIRES);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

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

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_VSCROLL,
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
	TCHAR szUnicode[MAX_LANGUAGES][MAX_LOADSTRING];
	LOGFONT lf;
	HFONT hfnt;
	TEXTMETRIC tm;
	int i;
	const int nLang = 41;
	SCROLLINFO si; 
	static int yClient;     // 工作区的高度 
	static int yChar;       // 垂直滚动单位 
	static int yPos = 0;        // 当前垂直滚动位置 
	static int yMax;        // 最大垂直滚动位置 
	int yInc;               // 垂直滚动增量 
	int FirstLine, LastLine;
 
	// 加载所有字符串资源	 
	// 阿拉伯语
	LoadString(hInst, IDS_UNICODE_ARA, szUnicode[0], MAX_LOADSTRING);
	// 巴斯克语
	LoadString(hInst, IDS_UNICODE_EUQ, szUnicode[1], MAX_LOADSTRING);
	// 保加利亚语
	LoadString(hInst, IDS_UNICODE_BGR, szUnicode[2], MAX_LOADSTRING);
	// 加泰罗尼亚语
	LoadString(hInst, IDS_UNICODE_CAT, szUnicode[3], MAX_LOADSTRING);
	// 中文(简体)
	LoadString(hInst, IDS_UNICODE_CHS, szUnicode[4], MAX_LOADSTRING);
	// 中文(繁体)
	LoadString(hInst, IDS_UNICODE_CHT, szUnicode[5], MAX_LOADSTRING);
	// 克罗地亚语
	LoadString(hInst, IDS_UNICODE_HRV, szUnicode[6], MAX_LOADSTRING);
	// 捷克语
	LoadString(hInst, IDS_UNICODE_CSY, szUnicode[7], MAX_LOADSTRING);
	// 丹麦语
	LoadString(hInst, IDS_UNICODE_DAN, szUnicode[8], MAX_LOADSTRING);
	// 荷兰语
	LoadString(hInst, IDS_UNICODE_NLD, szUnicode[9], MAX_LOADSTRING);
	// 英语
	LoadString(hInst, IDS_UNICODE_ENU, szUnicode[10], MAX_LOADSTRING);
	// 爱沙尼亚语
	LoadString(hInst, IDS_UNICODE_ETI, szUnicode[11], MAX_LOADSTRING);
	// 波斯语
	LoadString(hInst, IDS_UNICODE_FAR, szUnicode[12], MAX_LOADSTRING);
	// 芬兰语
	LoadString(hInst, IDS_UNICODE_FIN, szUnicode[13], MAX_LOADSTRING);
	// 法语
	LoadString(hInst, IDS_UNICODE_FRA, szUnicode[14], MAX_LOADSTRING);
	// 德语
	LoadString(hInst, IDS_UNICODE_DEU, szUnicode[15], MAX_LOADSTRING);
	// 希腊语
	LoadString(hInst, IDS_UNICODE_ELL, szUnicode[16], MAX_LOADSTRING);
	// 希伯来语
	LoadString(hInst, IDS_UNICODE_HEB, szUnicode[17], MAX_LOADSTRING);
	// 印地语
	LoadString(hInst, IDS_UNICODE_HIN, szUnicode[18], MAX_LOADSTRING);
	// 匈牙利语
	LoadString(hInst, IDS_UNICODE_HUN, szUnicode[19], MAX_LOADSTRING);
	// 冰岛语
	LoadString(hInst, IDS_UNICODE_ISL, szUnicode[20], MAX_LOADSTRING);
	// 爱尔兰语
	LoadString(hInst, IDS_UNICODE_IRL, szUnicode[21], MAX_LOADSTRING);
	// 意大利语
	LoadString(hInst, IDS_UNICODE_ITA, szUnicode[22], MAX_LOADSTRING);
	// 日语
	LoadString(hInst, IDS_UNICODE_JPN, szUnicode[23], MAX_LOADSTRING);
	// 朝鲜语
	LoadString(hInst, IDS_UNICODE_KOR, szUnicode[24], MAX_LOADSTRING);
	// 马拉地语
	LoadString(hInst, IDS_UNICODE_MAR, szUnicode[25], MAX_LOADSTRING);
	// 挪威语
	LoadString(hInst, IDS_UNICODE_NOR, szUnicode[26], MAX_LOADSTRING);
	// 奥克语
	LoadString(hInst, IDS_UNICODE_OCT, szUnicode[27], MAX_LOADSTRING);
	// 波兰语
	LoadString(hInst, IDS_UNICODE_PLK, szUnicode[28], MAX_LOADSTRING);
	// 葡萄牙语
	LoadString(hInst, IDS_UNICODE_PTG, szUnicode[29], MAX_LOADSTRING);
	// 罗马尼亚语
	LoadString(hInst, IDS_UNICODE_ROM, szUnicode[30], MAX_LOADSTRING);
	// 俄语
	LoadString(hInst, IDS_UNICODE_RUS, szUnicode[31], MAX_LOADSTRING);
	// 塞尔维亚语
	LoadString(hInst, IDS_UNICODE_SRB, szUnicode[32], MAX_LOADSTRING);
	// 斯洛文尼亚语
	LoadString(hInst, IDS_UNICODE_SLV, szUnicode[33], MAX_LOADSTRING);
	// 西班牙语(现代风格)
	LoadString(hInst, IDS_UNICODE_ESN, szUnicode[34], MAX_LOADSTRING);
	// 瑞典语
	LoadString(hInst, IDS_UNICODE_SVE, szUnicode[35], MAX_LOADSTRING);
	// 泰米尔语
	LoadString(hInst, IDS_UNICODE_TAM, szUnicode[36], MAX_LOADSTRING);
	// 泰语
	LoadString(hInst, IDS_UNICODE_THA, szUnicode[37], MAX_LOADSTRING);
	// 土耳其语
	LoadString(hInst, IDS_UNICODE_TRK, szUnicode[38], MAX_LOADSTRING);
	// 越南语
	LoadString(hInst, IDS_UNICODE_VIT, szUnicode[39], MAX_LOADSTRING);
	// 意第绪语
	LoadString(hInst, IDS_UNICODE_JII, szUnicode[40], MAX_LOADSTRING);

	// 指定要使用的 Unicode 字体(存储在 LOGFONT 结构中)
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
	_tcsncpy_s(lf.lfFaceName, sizeof(lf.lfFaceName)/sizeof(TCHAR), L"MS Shell Dlg", 12);

	switch (message) 
	{
		
		case WM_CREATE : 
 
			// 获取指向工作区的设备上下文的句柄。 
			hdc = GetDC (hWnd); 
 
			// 从 LOGFONT 结构创建字体
			hfnt = CreateFontIndirect(&lf); 
			SelectObject(hdc, hfnt);

			// 从文本度量提取字体尺寸。 
 			GetTextMetrics (hdc, &tm); 
			yChar = tm.tmHeight + tm.tmExternalLeading;
	 
			// 释放设备上下文。 
			ReleaseDC (hWnd, hdc); 
 
			return 0; 

		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// 分析菜单选择:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
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
			
				FirstLine = max (0, yPos + ps.rcPaint.top/yChar - 1); 
			LastLine = min (nLang, yPos + ps.rcPaint.bottom/yChar); 
 
			for (i = FirstLine;i < LastLine;i++) 
			{ 
				rt.top = yChar * (1 - yPos + i); 
				rt.bottom = rt.top+yChar;
				
				TCHAR szLangStr[MAX_LOADSTRING];
				wmemcpy(szLangStr, (wchar_t *) (szUnicode+i), MAX_LOADSTRING); 
				DrawText(hdc, szLangStr, (int) _tcslen(szLangStr), &rt, DT_CENTER); 
			} 
 

			EndPaint(hWnd, &ps);
			break;

		case WM_SIZE: 
 
			// 检索工作区的高度。 
			yClient = HIWORD (lParam); 
 
			// 确定最大垂直滚动位置。 
			// 为获得文本行下面的额外空间
			// 而加二。 
			yMax = nLang+2; 

			// 确保当前的垂直滚动位置
			// 不会超过最大垂直滚动位置。 
			yPos = min (yPos, yMax); 
 
			// 调整垂直滚动范围和滚动框位置
			// 以反映新的 yMax 和 yPos 值。
			si.cbSize = sizeof(si); 
			si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS; 
			si.nMin   = 0; 
			si.nMax   = yMax; 
			si.nPage  = (yClient / yChar) + 1; 
			si.nPos   = yPos; 
			SetScrollInfo(hWnd, SB_VERT, &si, TRUE); 
			return 0;

		case WM_VSCROLL:
			switch(LOWORD (wParam)) 
			{ 
				// 用户单击了滚动框上方的轴。
				case SB_PAGEUP: 
					 yInc = -yClient/yChar; 
					 break; 
 
				// 用户单击了滚动框下方的轴。
 				case SB_PAGEDOWN: 
					 yInc = yClient/yChar; 
					 break; 
 
				// 用户单击了顶部箭头。
 				case SB_LINEUP: 
					 yInc = -1; 
					 break; 
 
				// 用户单击了底部箭头。
 				case SB_LINEDOWN: 
					 yInc = 1; 
					 break; 
 
				// 用户拖动了滚动框。
 				case SB_THUMBPOSITION: 
					 yInc = HIWORD(wParam) - yPos; 
					 break; 
 
				default: 
					 yInc = 0; 
 
			} 
 
			// 如果应用垂直滚动增量没有
			// 使滚动位置超出滚动范围，
			// 则递增滚动位置，调整滚动框的
			// 位置，并更新窗口。UpdateWindow 
			// 发送 WM_PAINT 消息。
			 
			yInc = max(-yPos, min(yInc, yMax-yPos-yClient/yChar));
			if(yInc)
			{ 
				yPos += yInc; 
				ScrollWindowEx(hWnd, 0, -yChar*yInc, 
					(CONST RECT *) NULL, (CONST RECT *) NULL, 
					(HRGN) NULL, (LPRECT) NULL, SW_INVALIDATE|SW_ERASE); 
				si.cbSize = sizeof(si); 
				si.fMask  = SIF_POS; 
				si.nPos   = yPos; 
				SetScrollInfo(hWnd, SB_VERT, &si, TRUE); 
				UpdateWindow (hWnd); 
			} 
 
			return 0; 
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// “关于”框的消息处理程序。
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM )
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

