#include <windows.h>
#include "resource.h"

const char g_szClassName[] = "myWindowClass";

HBITMAP g_hbmBall = NULL;
HBITMAP g_hbmMask = NULL;

HBITMAP CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent)
{
	HDC hdcMem, hdcMem2;
	HBITMAP hbmMask;
	BITMAP bm;

	GetObject(hbmColour, sizeof(BITMAP), &bm);
	hbmMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

	hdcMem = CreateCompatibleDC(0);
	hdcMem2 = CreateCompatibleDC(0);

	SelectObject(hdcMem, hbmColour);
	SelectObject(hdcMem2, hbmMask);

	SetBkColor(hdcMem, crTransparent);

	BitBlt(hdcMem2, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

	BitBlt(hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem2, 0, 0, SRCINVERT);

	DeleteDC(hdcMem);
	DeleteDC(hdcMem2);

	return hbmMask;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_CREATE:
			g_hbmBall = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BALL));
			if(g_hbmBall == NULL)
				MessageBox(hwnd, "Could not load IDB_BALL!", "Error", MB_OK | MB_ICONEXCLAMATION);

			g_hbmMask = CreateBitmapMask(g_hbmBall, RGB(0, 0, 0));
			if(g_hbmMask == NULL)
				MessageBox(hwnd, "Could not create mask!", "Error", MB_OK | MB_ICONEXCLAMATION);
		break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
		break;
		case WM_PAINT:
		{
			BITMAP bm;
			RECT rcClient;
			PAINTSTRUCT ps;

			HDC hdc = BeginPaint(hwnd, &ps);

			HDC hdcMem = CreateCompatibleDC(hdc);
			HBITMAP hbmOld = SelectObject(hdcMem, g_hbmMask);

			GetObject(g_hbmBall, sizeof(bm), &bm);

			GetClientRect(hwnd, &rcClient);
			FillRect(hdc, &rcClient, GetStockObject(LTGRAY_BRUSH));

			BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
			BitBlt(hdc, bm.bmWidth, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCAND);
			BitBlt(hdc, bm.bmWidth * 2, bm.bmHeight * 2, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCAND);

			SelectObject(hdcMem, g_hbmBall);
			BitBlt(hdc, 0, bm.bmHeight, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
			BitBlt(hdc, bm.bmWidth, bm.bmHeight, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCPAINT);
			BitBlt(hdc, bm.bmWidth * 2, bm.bmHeight * 2, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCPAINT);

			SelectObject(hdcMem, hbmOld);
			DeleteDC(hdcMem);

			EndPaint(hwnd, &ps);
		}
		break;
		case WM_DESTROY:
			DeleteObject(g_hbmBall);
			DeleteObject(g_hbmMask);

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
	wc.style		 = 0;
	wc.lpfnWndProc	 = WndProc;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = hInstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName  = NULL;
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
		"Another Bitmap Program",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 240, 160,
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
