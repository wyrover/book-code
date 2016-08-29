#include <windows.h>

#include "resource.h"

const char g_szClassName[] = "myWindowClass";

HFONT g_hfFont = NULL;
BOOL g_bOpaque = TRUE;
COLORREF g_rgbText = RGB(0, 0, 0);
COLORREF g_rgbBackground = RGB(255, 255, 255);

COLORREF g_rgbCustom[16] = {0};

void DoSelectFont(HWND hwnd)
{
	CHOOSEFONT cf = {sizeof(CHOOSEFONT)};
	LOGFONT lf;

	GetObject(g_hfFont, sizeof(LOGFONT), &lf);

	cf.Flags = CF_EFFECTS | CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS;
	cf.hwndOwner = hwnd;
	cf.lpLogFont = &lf;
	cf.rgbColors = g_rgbText;

	if(ChooseFont(&cf))
	{
		HFONT hf = CreateFontIndirect(&lf);
		if(hf)
		{
			g_hfFont = hf;
		}
		else
		{
			MessageBox(hwnd, "Font creation failed!", "Error", MB_OK | MB_ICONEXCLAMATION);
		}

		g_rgbText = cf.rgbColors;
	}
}

void DoSelectColour(HWND hwnd)
{
	CHOOSECOLOR cc = {sizeof(CHOOSECOLOR)};

	cc.Flags = CC_RGBINIT | CC_FULLOPEN | CC_ANYCOLOR;
	cc.hwndOwner = hwnd;
	cc.rgbResult = g_rgbBackground;
	cc.lpCustColors = g_rgbCustom;

	if(ChooseColor(&cc))
	{
		g_rgbBackground = cc.rgbResult;
	}
}

void DrawClientSize(HDC hdc, RECT* prc, HFONT hf)
{
	char szSize[100];
	char szTitle[] = "These are the dimensions of your client area:";
	HFONT hfOld = SelectObject(hdc, hf);

	SetBkColor(hdc, g_rgbBackground);
	SetTextColor(hdc, g_rgbText);

	if(g_bOpaque)
	{
		SetBkMode(hdc, OPAQUE);
	}
	else
	{
		SetBkMode(hdc, TRANSPARENT);
	}

	DrawText(hdc, szTitle, -1, prc, DT_WORDBREAK);

	wsprintf(szSize, "{%d, %d, %d, %d}", prc->left, prc->top, prc->right, prc->bottom);
	DrawText(hdc, szSize, -1, prc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	SelectObject(hdc, hfOld);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_CREATE:
			g_hfFont = GetStockObject(DEFAULT_GUI_FONT);
		break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
		break;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case ID_FILE_EXIT:
					PostMessage(hwnd, WM_CLOSE, 0, 0);
				break;
				case ID_FORMAT_FONT:
					DoSelectFont(hwnd);

					InvalidateRect(hwnd, NULL, TRUE);
					UpdateWindow(hwnd);
				break;
				case ID_FORMAT_DEFAULTGUIFONT:
					DeleteObject(g_hfFont);
					g_hfFont = GetStockObject(DEFAULT_GUI_FONT);
					
					InvalidateRect(hwnd, NULL, TRUE);
					UpdateWindow(hwnd);
				break;
				case ID_FORMAT_TEST:
				{
					HFONT hf;
					HDC hdc;
					long lfHeight;
					
					hdc = GetDC(NULL);
					lfHeight = -MulDiv(12, GetDeviceCaps(hdc, LOGPIXELSY), 72);
					ReleaseDC(NULL, hdc);

					hf = CreateFont(lfHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, "Times New Roman");

					if(hf)
					{
						DeleteObject(g_hfFont);
						g_hfFont = hf;
					}
					else
					{
						MessageBox(hwnd, "Font creation failed!", "Error", MB_OK | MB_ICONEXCLAMATION);
					}					
					InvalidateRect(hwnd, NULL, TRUE);
					UpdateWindow(hwnd);
				}
				break;
				case ID_FORMAT_BACKGROUNDCOLOUR:
					DoSelectColour(hwnd);

					InvalidateRect(hwnd, NULL, TRUE);
					UpdateWindow(hwnd);
				break;
				case ID_FORMAT_OPAQUE:
					g_bOpaque = !g_bOpaque;

					InvalidateRect(hwnd, NULL, TRUE);
					UpdateWindow(hwnd);
				break;
			}
		break;
		case WM_INITMENUPOPUP:
			CheckMenuItem((HMENU)wParam, ID_FORMAT_OPAQUE, MF_BYCOMMAND | ( g_bOpaque ? MF_CHECKED : MF_UNCHECKED));
		break;
		case WM_PAINT:
		{
			RECT rcClient;
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			GetClientRect(hwnd, &rcClient);

			DrawClientSize(hdc, &rcClient, g_hfFont);

			EndPaint(hwnd, &ps);
		}
		break;
		case WM_DESTROY:
			DeleteObject(g_hfFont);

			PostQuitMessage(0);
		break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.style		 = CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc	 = WndProc;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = hInstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU);
	wc.lpszClassName = g_szClassName;
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		"A Font Program",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
		NULL, NULL, hInstance, NULL);

	if(hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}
