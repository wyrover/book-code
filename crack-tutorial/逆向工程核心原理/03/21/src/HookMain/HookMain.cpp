#include "stdio.h"
#include "conio.h"
#include "windows.h"

#define	DEF_DLL_NAME		"KeyHook.dll"
#define	DEF_HOOKSTART		"HookStart"
#define	DEF_HOOKSTOP		"HookStop"

typedef void (*PFN_HOOKSTART)();
typedef void (*PFN_HOOKSTOP)();

void main()
{
	HMODULE			hDll = NULL;
	PFN_HOOKSTART	HookStart = NULL;
	PFN_HOOKSTOP	HookStop = NULL;
	char			ch = 0;

    // KeyHook.dll 로딩
	hDll = LoadLibraryA(DEF_DLL_NAME);
    if( hDll == NULL )
    {
        printf("LoadLibrary(%s) failed!!! [%d]", DEF_DLL_NAME, GetLastError());
        return;
    }

    // export 함수 주소 얻기
	HookStart = (PFN_HOOKSTART)GetProcAddress(hDll, DEF_HOOKSTART);
	HookStop = (PFN_HOOKSTOP)GetProcAddress(hDll, DEF_HOOKSTOP);

    // 후킹 시작
	HookStart();

    // 사용자가 'q' 를 입력할 때까지 대기
	printf("press 'q' to quit!\n");
	while( _getch() != 'q' )	;

    // 후킹 종료
	HookStop();
	
    // KeyHook.dll 언로딩
	FreeLibrary(hDll);
}