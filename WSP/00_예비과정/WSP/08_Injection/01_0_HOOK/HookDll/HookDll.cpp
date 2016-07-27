// HookDll.cpp

#include <windows.h>

// dll => WINNT\\SYSTEM32 디렉토리에 복사 
HHOOK		hHook;
HMODULE		hDll;

BOOL  WINAPI DllMain( HANDLE h, DWORD , LPVOID p)
{
	hDll	= (HMODULE)h;
	return TRUE;
}

// Hook Procedure....

LRESULT CALLBACK foo( int nCode, WPARAM wParam, LPARAM lParam)
{
	// 모든 훅 함수는 반드시 도입부가 이 모양을 갖는다.
	if ( nCode < 0 )
		return CallNextHookEx( hHook, nCode, wParam, lParam);

	// 만약 윈도우가 만들어지고 있다면..
	if( nCode == HCBT_CREATEWND)
	{
		HWND hwnd = (HWND)wParam;		// 만들어지고 있는 윈도우 핸들
		char buf[256];

		GetClassName(hwnd, buf, 256);	// 클래스 이름 얻기

		if( strcmp(buf, "SciCalc") == 0 )	// 계산기 라면??
		{
			MessageBox(NULL, "계산기 실행", "", MB_OK);
			::WinExec("notePad.exe", SW_SHOW);

			DWORD pid;
			DWORD tid = GetWindowThreadProcessId(hwnd, &pid);
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
			TerminateProcess(hProcess, 0);
			CloseHandle(hProcess);
		}
	}
	return CallNextHookEx( hHook, nCode, wParam, lParam);
}

// install
extern "C" __declspec( dllexport)
void Install()
{
	// 4번째 인수가 0이면 시스템 전체에 대한 훅을 얻음 
	hHook	= SetWindowsHookEx(WH_CBT, foo, hDll, 0);
}

// uninstall
extern "C" __declspec( dllexport)
void UnInstall()
{
	UnhookWindowsHookEx(hHook);
}