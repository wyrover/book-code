#include <windows.h>

BOOL WINAPI DllMain(HANDLE handle, DWORD dword, LPVOID lpvoid)
{
	return TRUE;
}

// 노출하는 함수가 있을 경우는 Complier가 자동으로 main을 만들어
// 준다.

// 함수가 없을 경우는 사용자가 직접 만들어야 함.