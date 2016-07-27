#include <windows.h>
#include <windowsx.h>

#define IDC_EDIT  1
#define IDC_COMBO 2
#define IDC_CHECK 3

HWND hCombo;
HWND hEdit;
HWND hCheck;

int cxChar;
int cyChar;

BOOL bBold			= FALSE;
TCHAR strMsg[256]	= "Sample";
TCHAR fontName[256] = "궁서";

///////////////////////
int CALLBACK AddFontName( ENUMLOGFONT *lpelf, NEWTEXTMETRIC *lpntm,  DWORD FontType,  LPARAM lParam )
{
	// 찾은 Font이름을 콤보에 넣는다.
	SendMessage(hCombo, CB_ADDSTRING, 0, 
		        (LPARAM)lpelf->elfLogFont.lfFaceName);
	return 1;
}

///////////////////////////////////////////////////////////////////
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	HDC hdc = GetDC(hwnd);

	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm); // 글꼴의 크기.
	
	cxChar = tm.tmAveCharWidth;
	cyChar = tm.tmHeight;

	HINSTANCE hInstance = lpCreateStruct->hInstance;

	// 도형의 크기를 글꼴 크기의 배수로 설정.
	hEdit = CreateWindow("Edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 
		cxChar*2, cyChar* 6,cxChar* 20, cyChar * 1.5, hwnd, (HMENU)IDC_EDIT, hInstance, 0);

	hCombo = CreateWindow("COMBOBOX", "", WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWNLIST | WS_VSCROLL | CBS_SORT,	
		cxChar * 2, cyChar * 8, cxChar * 20, cyChar * 10, hwnd, (HMENU)IDC_COMBO, hInstance, 0);

	hCheck = CreateWindow("button", "Bold" ,WS_CHILD |  WS_VISIBLE | BS_AUTOCHECKBOX, 
		cxChar * 2, cyChar * 10, cxChar* 20, cyChar, hwnd, (HMENU)IDC_CHECK, hInstance, 0);

	// EditBox 초기화 
//	SendMessage(hEdit, EM_SETLIMITTEXT, 10, 0);

	// ComboBox 초기화.
	EnumFontFamilies(hdc, NULL,(FONTENUMPROC)AddFontName, 0);

	ReleaseDC(hwnd, hdc);
	return TRUE;
}


void OnCommand(HWND hwnd, int id, HWND hChild, int nNotify)
{
	switch(id)
	{
	case IDC_CHECK:
		bBold = (SendMessage(hCheck, BM_GETCHECK, 0,0) == BST_CHECKED);
		break;
	case IDC_COMBO:
		if (nNotify == CBN_SELCHANGE)
			GetWindowText(hCombo, fontName, 256);
		break;
	case IDC_EDIT:
		if (nNotify == EN_CHANGE)
			GetWindowText(hEdit, strMsg, 256);
		break;
	default:
		return;
	}
	RECT r = {cxChar*2, cyChar * 2, cxChar*22, cyChar*5};
	InvalidateRect(hwnd, &r, TRUE);

}
void OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	
	RECT r = {cxChar*2, cyChar * 2, cxChar*22, cyChar*5};

	Rectangle(hdc, r.left, r.top, r.right, r.bottom);

	HFONT hFont = CreateFont(24, 0,0,0, bBold ? FW_BOLD : 0,
						0,0,0,DEFAULT_CHARSET, 0,0,0,0,fontName);

	HFONT OldFont = (HFONT)SelectObject(hdc, hFont);

	DrawText(hdc,  strMsg, -1, &r, DT_SINGLELINE | DT_VCENTER | DT_CENTER  );
	
	SelectObject(hdc, OldFont);
	DeleteObject(hFont);

	EndPaint(hwnd, &ps);
}

void OnDestroy(HWND hwnd) 
{ 
	PostQuitMessage(0);
}
////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		HANDLE_MSG(hwnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hwnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hwnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR lpCmdLine, int nShowCmd)
{
	TCHAR szAppName[] = TEXT("FontView");
	HWND hwnd;
	MSG msg;

	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = szAppName;
	wc.lpszMenuName = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

	RegisterClass(&wc);

	hwnd = CreateWindow(szAppName, szAppName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0,200, 210, 0, 0,hInstance, 0);

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	while( GetMessage(&msg, 0, 0, 0) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
