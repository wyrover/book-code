// hk2.c

#include <windows.h>

HMODULE g_hDll;  // 현재 DLL의 핸들(주소)을 보관.
HHOOK   g_hook;  //훅 핸들..

BOOL WINAPI DllMain(HANDLE hDll, DWORD r, LPVOID H)
{
	if( r == DLL_PROCESS_ATTACH )
	{
		g_hDll = (HMODULE)hDll;
	}
	return TRUE;
}

// GetMessage()로 가져가는 모든 메세지가 아래 함수를 지나서 가게 된다..
LRESULT CALLBACK foo(int nCode, WPARAM wParam, LPARAM lParam)
{
	HWND hwnd;
	char name[256];

	if( nCode == HCBT_CREATEWND ) // 메세지를 가져가고 있는 중... !~!!!!
	{							
		hwnd = (HWND)wParam;  // 만들어 지고 있는 윈도우 핸들..
		// hwnd 가 어떤 윈도우인지 어떻게 알수 있을까??
		GetClassName(hwnd, name, 256);

		if(strcmpi(name, "WMP Skin Host") == 0)
		{
			MessageBox(0, "영화 보지 마세요...~!!!", "", MB_OK);

			return 1;  // WH_CBT에서 다음 훅으로 보내지 않고 1을 리턴하면 
					   // 생성되고 있던 윈도우가 파괴 된다..
					   // 실패하게 된다..
		}
	}
	return CallNextHookEx(0, nCode, wParam, lParam);
}

__declspec(dllexport) void Install( DWORD tid)
{
	g_hook = SetWindowsHookEx(WH_CBT, foo, g_hDll, tid);
}

__declspec(dllexport) void Uninstall()
{
	UnhookWindowsHookEx(g_hook);
}

