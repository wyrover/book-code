// SPY.h
/*
#ifdef DLL_SOURCE
	#define DLLAPI extern "C" __declspec( dllexport)

#else
	#define DLLAPI extern "C" __declspec(dllimport)
#endif

// 2개의 함수를 export
DLLAPI void Install();
DLLAPI void Uninstall();
*/


// SPY.cpp
#include <windows.h>
#define DLL_SOURCE

#include "SPY.h"

HHOOK		g_hook;		// hook 핸들
HMODULE		g_hDll;		// 훅 함수가 있는 DLL의 핸들

BOOL WINAPI DllMain( HANDLE h, DWORD, LPVOID )
{
	g_hDll = (HMODULE)h;		// DLL의 instance handle 보관
	return TRUE;
}

// 훅 함수
LRESULT  CALLBACK foo( int nCode, WPARAM wp, LPARAM lp)
{
	if( nCode == HC_ACTION )	// GetMessage()나 PeekMessage()로 메시지를 꺼낼 경우
	{
		MSG *pMsg  = (MSG*)lp;

		if( pMsg->message	== WM_CHAR)
		{
			HWND hwnd = FindWindow(0, "UseSPY");
			PostMessage(hwnd, WM_USER+100, pMsg->wParam, pMsg->lParam);
		}
	}
	// 다음 훅체인에게 전달
	return CallNextHookEx( g_hook, nCode, wp, lp);
}

// 훅 설치
void Install()
{
	g_hook = SetWindowsHookEx(WH_GETMESSAGE, foo, g_hDll, 0);
}

void Uninstall()
{
	UnhookWindowsHookEx(g_hook);
}