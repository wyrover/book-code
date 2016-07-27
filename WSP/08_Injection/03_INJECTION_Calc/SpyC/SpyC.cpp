// SpyC.cp

#include <windows.h>

#define MB(x) MessageBox(NULL, x, "SpyC", MB_OK);

WNDPROC	 oldProc = (WNDPROC)0x01006118;
LRESULT CALLBACK WndProc( HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam)
{
	switch( imsg)
	{
	case WM_COMMAND:
		if( LOWORD(wParam) == 50000 )
		{ 
			MB("추가된 메뉴"); 
			return 0; 
		}
		if( LOWORD (wParam) >= 0x7c &&
						LOWORD( wParam) <= 0x85)
		{
			MB("숫자 변경");
			wParam = wParam +1;
			lParam = (LPARAM)GetDlgItem(hwnd, LOWORD(wParam));
			return oldProc(hwnd, imsg, wParam, lParam);
		}
	}
	return oldProc(hwnd, imsg, wParam, lParam);
}

DWORD WINAPI foo( PVOID p)		// 스레드 함수
{
	HWND hwnd	= FindWindow("SciCalc", "계산기");
	HMENU hmenu = GetMenu(hwnd);
	HMENU hSub  = GetSubMenu(hmenu, 0);
	MB("메뉴 추가");

	AppendMenu(hSub, MF_SEPARATOR, 0, 0);
	AppendMenu(hSub, MF_STRING, 50000, "추가된 메뉴");

	// 메시지 처리 함수 변경
	SetWindowLong(hwnd, GWL_WNDPROC, (LONG)WndProc);
	return 0;
}
BOOL WINAPI DllMain( HANDLE, DWORD dwReason, PVOID)
{
	if( dwReason == DLL_PROCESS_ATTACH)
	{
		MB("DLL 성공... 스레드를 생성한다.");

		HANDLE hThread	= CreateThread(NULL, 0, foo, 0, 0, 0);
		if( hThread != 0 )
		{
			MB("스레드 생성 성공");
			CloseHandle(hThread);
			
		}
	}
	return TRUE;
}