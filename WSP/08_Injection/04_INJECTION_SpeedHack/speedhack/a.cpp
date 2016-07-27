#include <windows.h>
#include "Dbghelp.h"
#pragma comment( lib, "Dbghelp.lib")


void Replace( HMODULE hModule, // .idata를 가진 모듈의 주소
			  PCSTR	  dllname, // Hooking 할 함수를 가진 DLL이름
			  PROC	  oldfunc, // 훅킹할 API 함수.
			  PROC    newfunc) // 바꿀 함수 
{
	// 1. module 에서 .idata section 의주소를 찾는다.
	ULONG sz = 0;
	PIMAGE_IMPORT_DESCRIPTOR pImage = (PIMAGE_IMPORT_DESCRIPTOR)
						ImageDirectoryEntryToData( hModule, TRUE,
									IMAGE_DIRECTORY_ENTRY_IMPORT, & sz);

	// 2. .idata section 에서 원하는 Dll의 위치를 찾는다. 
	for ( ; pImage->Name; ++pImage )
	{
		char* p = ((char*)hModule + pImage->Name);

		if ( strcmpi( p, dllname) == 0 ) break; // 찾은 경우
	}
	//--------------------------------------------------------------
	// 3. Thunk Table 에서 원하는 주소를 얻는다.
	PIMAGE_THUNK_DATA pThunk = 
				(PIMAGE_THUNK_DATA)( (char*)hModule + pImage->FirstThunk);

	for ( ; pThunk->u1.Function; ++pThunk )
	{
		if ( (PROC)(pThunk->u1.Function) == oldfunc ) // 찾은 경우.
		{
			PROC* p = (PROC*)&(pThunk->u1.Function);

			// .idata section 부분의 보호 속성을 변경한다.
			DWORD old;
			VirtualProtect( p,		// 이 주소의 부터
							4,		// 4 Byte 를 
							PAGE_READWRITE,		// 읽고 쓰기로 
							&old);				// 이전 보호속성 리턴.



			*p = newfunc; // 덮어 쓴다.
		}
	}
}
















typedef UINT (WINAPI *F)(HWND, UINT, UINT, TIMERPROC);

UINT WINAPI MySetTimer( HWND hwnd, UINT id, UINT ms, TIMERPROC f )
{
	// 원래 함수의 주소를 찾는다.
	HMODULE h = GetModuleHandle("User32.dll");
	F func = (F)GetProcAddress( h, "SetTimer");

	return func( hwnd, id, ms/20, f ); // 20배 빠르게 동작 하도록 한다.
}

BOOL WINAPI DllMain( HANDLE h, DWORD r, LPVOID how )
{
	if ( r == DLL_PROCESS_ATTACH )
	{
	

		Replace( GetModuleHandle(0), "user32.dll", (PROC)SetTimer,
													(PROC)MySetTimer);
	}
	return TRUE;
}

// DbgHelp.h 와 Dbghelp.lib 를 모두 복사해 오세요..

// 완성된 speedhack.dll 을 D:\\ 아래 복사 하세요.

// 그리고 어제 만든 소스인 DLLinject 열어 보세요.



































