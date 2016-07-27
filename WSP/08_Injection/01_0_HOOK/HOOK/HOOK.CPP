// HOOK
#include <windows.h>

WNDPROC  OldProc;

HWND WINAPI GetFirstChild( HWND hwnd);

BOOL CALLBACK MyNewProc( HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain( HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCMdline, 
						int nShowCmd)
{
	HWND hwnd;

	// 1. 메모장 핸들 얻기 
	hwnd = FindWindow("notepad", NULL);

	// 2. SetWIndowLong 함수를 통해 Procedure를 변경
	OldProc = (WNDPROC)SetWindowLong(hwnd, GWL_WNDPROC, (LONG)MyNewProc);
	return 0;
}

BOOL CALLBACK MyNewProc( HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

/*
	안된다.
	
	서브클래싱의 하나의 예

    메모리가 다르다..

    이런 것을 가능케 하는게 HOOK , ( dllInjection ) 
*/
















