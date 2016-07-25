// Multi1.cpp : 多线程例子
//

#include "stdafx.h"

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <process.h>

typedef struct
{
	HWND hwnd ;
	int  cxClient ;
	int  cyClient ;
	int  cyChar ;
	BOOL bKill ;
}PARAMS, *PPARAMS;

HANDLE hThread[4];
unsigned long __stdcall Thread1 (PVOID pvoid);
unsigned long __stdcall Thread2 (PVOID pvoid);
unsigned long __stdcall Thread3 (PVOID pvoid);
unsigned long __stdcall Thread4 (PVOID pvoid);

LRESULT APIENTRY WndProc (HWND, UINT, WPARAM, LPARAM) ;



int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
	static char szAppName[] = "MultiThread" ;
	HWND        hwnd ;
	MSG         msg ;
	WNDCLASSEX  wndclass ;
	
	wndclass.cbSize        = sizeof (wndclass) ;
	wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc   = WndProc ;
	wndclass.cbClsExtra    = 0 ;
	wndclass.cbWndExtra    = 0 ;
	wndclass.hInstance     = hInstance ;
	wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
	wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
	wndclass.lpszMenuName  = NULL ;
	wndclass.lpszClassName = szAppName ;
	wndclass.hIconSm       = LoadIcon (NULL, IDI_APPLICATION) ;
	
	RegisterClassEx (&wndclass) ;
	
	hwnd = CreateWindow (szAppName, "多线程例子",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL) ;
	
	ShowWindow (hwnd, iCmdShow) ;
	UpdateWindow (hwnd) ;
	
	while (GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage (&msg) ;
		DispatchMessage (&msg) ;
	}
	return msg.wParam ;
}

int CheckBottom (HWND hwnd, int cyClient, int cyChar, int iLine)
{
	if (iLine * cyChar + cyChar > cyClient)
	{
		InvalidateRect (hwnd, NULL, TRUE) ;
		UpdateWindow (hwnd) ;
		iLine = 0 ;
	}
	return iLine ;
}

unsigned long __stdcall Thread1 (PVOID pvoid)
{
	char    szBuffer[16] ;
	int     iNum = 1, iLine = 0, i, iSqrt ;
	HDC     hdc ;
	PPARAMS pparams ;
	
	pparams = (PPARAMS) pvoid ;
	
	while (!pparams->bKill)
	{
		do
		{
			if (++iNum < 0)
				iNum = 0 ;
			
			iSqrt = (int) sqrt (iNum) ;
			
			for (i = 2 ; i <= iSqrt ; i++)
				if (iNum % i == 0)
					break ;
		}
		while (i <= iSqrt) ;
		
		iLine = CheckBottom (pparams->hwnd,   pparams->cyClient,
			pparams->cyChar, iLine) ;
		
		wsprintf (szBuffer, "%d", iNum) ;
		
		hdc = GetDC (pparams->hwnd) ;
		
		TextOut (hdc, 0, iLine * pparams->cyChar,
			szBuffer, strlen (szBuffer)) ;
		
		ReleaseDC (pparams->hwnd, hdc) ;
		
		iLine++ ;
	}

	return 0;
}

// Window 1: 显示逐渐增加的素数

LRESULT APIENTRY WndProc1 (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static PARAMS params ;
	
	switch (iMsg)
	{
	case WM_CREATE :
		params.hwnd = hwnd ;
		params.cyChar = HIWORD (GetDialogBaseUnits ()) ;//系统中基于对话框字体的高度
	
		hThread[0] = CreateThread(NULL,0,Thread1,&params,0,NULL);
		SetThreadPriority(hThread[0],THREAD_PRIORITY_BELOW_NORMAL);
		return 0 ;
		
	case WM_SIZE :
		params.cyClient = HIWORD (lParam) ;
		return 0 ;
		
	case WM_DESTROY :
		params.bKill = TRUE ;
		return 0 ;
	}
	return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
}


// ------------------------------------------------------



unsigned long __stdcall Thread2(PVOID pvoid)
{
	char    szBuffer[16] ;
	int     iNum = 0, iNext = 1, iLine = 0, iTemp ;
	HDC     hdc ;
	PPARAMS pparams ;
	
	pparams = (PPARAMS) pvoid ;
	
	while (!pparams->bKill)
	{
		if (iNum < 0)
		{
			iNum  = 0 ;
			iNext = 1 ;
		}
		
		iLine = CheckBottom (pparams->hwnd,   pparams->cyClient,
			pparams->cyChar, iLine) ;
		
		wsprintf (szBuffer, "%d", iNum) ;
		
		hdc = GetDC (pparams->hwnd) ;
		
		TextOut (hdc, 0, iLine * pparams->cyChar,
			szBuffer, strlen (szBuffer)) ;
		
		ReleaseDC (pparams->hwnd, hdc) ;
		
		iTemp  = iNum ;
		iNum   = iNext ;
		iNext += iTemp ;
		
		iLine++ ;
	}

	return 0;
}

// Window : 显示逐渐增加的复数
LRESULT APIENTRY WndProc2 (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static PARAMS params ;
	
	switch (iMsg)
	{
	case WM_CREATE :
		params.hwnd = hwnd ;
		params.cyChar = HIWORD (GetDialogBaseUnits ()) ;
		hThread[1] = CreateThread(NULL,0,Thread2,&params,0,NULL);
		SetThreadPriority(hThread[1],THREAD_PRIORITY_BELOW_NORMAL);
		return 0 ;
		
	case WM_SIZE :
		params.cyClient = HIWORD (lParam) ;
		return 0 ;
		
	case WM_DESTROY :
		params.bKill = TRUE ;
		return 0 ;
	}
	return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
}


unsigned long __stdcall Thread3 (PVOID pvoid)
{
	HDC     hdc ;
	int     iDiameter ;
	PPARAMS pparams ;
	
	pparams = (PPARAMS) pvoid ;
	
	while (!pparams->bKill)
	{
		InvalidateRect (pparams->hwnd, NULL, TRUE) ;
		UpdateWindow (pparams->hwnd) ;
		 
		iDiameter = rand() % (max (1,
			min (pparams->cxClient, pparams->cyClient))) ;
		
		hdc = GetDC (pparams->hwnd) ;
		
		Ellipse (hdc, (pparams->cxClient - iDiameter) / 2,
			(pparams->cyClient - iDiameter) / 2,
			(pparams->cxClient + iDiameter) / 2,
			(pparams->cyClient + iDiameter) / 2) ;
		
		ReleaseDC (pparams->hwnd, hdc) ;
	}

	return 0;
}

// Window 3: 显示任意半径的圆
LRESULT APIENTRY WndProc3 (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static PARAMS params ;
	
	switch (iMsg)
	{
	case WM_CREATE :
		params.hwnd = hwnd ;
		params.cyChar = HIWORD (GetDialogBaseUnits ()) ;
		hThread[2] = CreateThread(NULL,0,Thread3,&params,0,NULL);
		SetThreadPriority(hThread[2],THREAD_PRIORITY_BELOW_NORMAL);
		return 0 ;
		
	case WM_SIZE :
		params.cxClient = LOWORD (lParam) ;
		params.cyClient = HIWORD (lParam) ;
		return 0 ;
		
	case WM_DESTROY :
		params.bKill = TRUE ;
		return 0 ;
	}
	return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
}


// -----------------------------------------
unsigned long __stdcall Thread4 (PVOID pvoid)
{


	HDC     hdc ;
	PPARAMS pparams ;

	HBRUSH hBrush ;

    int    xLeft, xRight, yTop, yBottom, iRed, iGreen, iBlue ;
	
	pparams = (PPARAMS) pvoid ;
	
	
	while (!pparams->bKill)
          {
          if (pparams->cxClient != 0 || pparams->cyClient != 0)
              {
              xLeft  = rand () %  pparams->cxClient ;
              xRight  = rand () % pparams->cxClient ;
              yTop    = rand () % pparams->cyClient ;
              yBottom = rand () % pparams->cyClient ;
              iRed    = rand () & 255 ;
              iGreen  = rand () & 255 ;
              iBlue  = rand () & 255 ;

              hdc = GetDC (pparams->hwnd) ;
              hBrush = CreateSolidBrush (RGB (iRed, iGreen, iBlue)) ;
              SelectObject (hdc, hBrush) ;

              Rectangle (hdc, min (xLeft, xRight), min (yTop, yBottom),
                              max (xLeft, xRight), max (yTop, yBottom)) ;

              ReleaseDC (pparams->hwnd, hdc) ;
              DeleteObject (hBrush) ;
              }
          }
      
	return 0;
}



LRESULT APIENTRY WndProc4 (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static PARAMS params ;
	
	switch (iMsg)
	{
	case WM_CREATE :
		params.hwnd = hwnd ;
		params.cyChar = HIWORD (GetDialogBaseUnits ()) ;
		hThread[3] = CreateThread(NULL,0,Thread4,&params,0,NULL);
		SetThreadPriority(hThread[3],THREAD_PRIORITY_BELOW_NORMAL);
		return 0 ;
		
	case WM_SIZE :
		params.cxClient = LOWORD (lParam) ;
		params.cyClient = HIWORD (lParam) ;
		return 0 ;
		
	case WM_DESTROY :
		params.bKill = TRUE ;
		return 0 ;
	}
	return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
}

// 窗口函数--创建各个子窗口
// -----------------------------------

LRESULT APIENTRY WndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static char   *szChildClass[] = { "Child1", "Child2", "Child3", "Child4" } ;
	static HWND    hwndChild[4] ;
	static WNDPROC ChildProc[] = { WndProc1, WndProc2, WndProc3, WndProc4 } ;
	HINSTANCE      hInstance ;
	int            i, cxClient, cyClient ;
	WNDCLASSEX     wndclass ;
	
	switch (iMsg)
	{
	case WM_CREATE :
		hInstance = (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE) ;
		
		wndclass.cbSize        = sizeof (wndclass) ;
		wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
		wndclass.cbClsExtra    = 0 ;
		wndclass.cbWndExtra    = 0 ;
		wndclass.hInstance     = hInstance ;
		wndclass.hIcon         = NULL ;
		wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
		wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
		wndclass.lpszMenuName  = NULL ;
		wndclass.hIconSm       = NULL ;
		
		for (i = 0 ; i < 4 ; i++)
		{
			wndclass.lpfnWndProc   = ChildProc[i] ;
			wndclass.lpszClassName = szChildClass[i] ;
			
			RegisterClassEx (&wndclass) ;
			
			hwndChild[i] = CreateWindow (szChildClass[i], NULL,
				WS_CHILDWINDOW | WS_BORDER | WS_VISIBLE,
				0, 0, 0, 0, hwnd, (HMENU) i, hInstance, NULL) ;
		}
		
		return 0 ;
		
	case WM_SIZE :
		cxClient = LOWORD (lParam) ;
		cyClient = HIWORD (lParam) ;
		
		for (i = 0 ; i < 4 ; i++)
			MoveWindow (hwndChild[i], (i % 2) * cxClient / 2,
			(i > 1) * cyClient / 2,
			cxClient / 2, cyClient / 2, TRUE) ;
		return 0 ;
		
	case WM_CHAR :
		if (wParam == ' ')// \x1B			 
			DestroyWindow (hwnd) ;
		return 0 ;
		
	case WM_DESTROY :
		
		PostQuitMessage(0) ;
		for(int i=0;i<4;i++)
			CloseHandle(hThread[i]);
		return 0 ;
	}
	return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
}





