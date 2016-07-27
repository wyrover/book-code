#include <stdio.h>
#include <windows.h>

// PE화일의 내용을 검색하려면 DbgHelp.dll 이 필요하다.
#include "Dbghelp.h"
#pragma comment(lib, "Dbghelp.lib")

void Replace( HMODULE hModule, // .idata를 가진 모듈의 주소
			  PCSTR	  dllname, // Hooking 할 함수를 가진 DLL이름
			  PROC	  oldfunc, // 훅킹할 API 함수.
			  PROC    newfunc) // 바꿀 함수 
{
	// 1. module 에서 .idata section 의주소를 찾는다.
	ULONG sz = 0;
	PIMAGE_IMPORT_DESCRIPTOR pImage = (PIMAGE_IMPORT_DESCRIPTOR)

		ImageDirectoryEntryToData( hModule, TRUE, // <<----- FALSE 를 TRUE로 

									IMAGE_DIRECTORY_ENTRY_IMPORT, & sz);










	printf(".idata section 의 주소 : %p\n", pImage );

	// 2. .idata section 에서 원하는 Dll의 위치를 찾는다. 
	for ( ; pImage->Name; ++pImage )
	{
		char* p = ((char*)hModule + pImage->Name);

		if ( strcmpi( p, dllname) == 0 ) break; // 찾은 경우
	}
	if ( pImage->Name == 0 ) 
		printf("해당 모듈은 %s 를 import 하지 않습니다.\n", dllname);

	printf("%s 모듈의 import section 주소 : %p\n", dllname, pImage );





	//--------------------------------------------------------------
	// 3. Thunk Table 에서 원하는 주소를 얻는다.
	PIMAGE_THUNK_DATA pThunk = 
				(PIMAGE_THUNK_DATA)( (char*)hModule + pImage->FirstThunk);

	for ( ; pThunk->u1.Function; ++pThunk )
	{
		if ( (PROC)(pThunk->u1.Function) == oldfunc ) // 찾은 경우.
		{
			PROC* p = (PROC*)&(pThunk->u1.Function);

			*p = newfunc; // 덮어 쓴다.
		}
	}
}
// user.chol.com/~downboard/dbghelp.zip  받아서 압축 푸세요..
// dbghelp.dll 은 C:\\windows\\system32 안에 두세요..
// .h 와 .lib 현재 작업 폴더에 복사해 오세요..


// MessageBox를 대체할 함수.
typedef UINT (WINAPI *F)(HWND, PCSTR, PCSTR, UINT);

UINT WINAPI foo( HWND hwnd, PCSTR s, PCSTR title, UINT btn)
{
	printf("foo is called\n");
	printf("Message : %s\n", s);
	printf("TitleBar : %s\n", title );
	
	// 다시 원래의 MessageBox로 전달하려면 ?
	//MessageBoxA(); // 재귀 호출..
	// 직접 함수의 주소를 구한다.
	HMODULE hDll = GetModuleHandle("User32.dll");

	F f = (F)GetProcAddress( hDll, "MessageBoxA");

	return f( hwnd, s, title, btn);  // <<!!!
}


void main()
{
	Replace( GetModuleHandle( 0 ), // exe 주소
			 "User32.dll", (PROC)MessageBoxA, (PROC)foo );



	void* p = GetModuleHandle( "User32.dll");

	printf("User32.dll 의 주소 : %p\n", p );
	printf("MessageBoxA의 주소 : %p\n", MessageBoxA );

	MessageBoxA( 0, "Hello", "AAA", MB_OK);
}












// MessageBox 는 결국 매크로이고 실제 함수는 MessageBoxA 또는 MessageBoxW임.