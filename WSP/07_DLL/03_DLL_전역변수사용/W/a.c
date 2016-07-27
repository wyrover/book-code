// a.c
#include <windows.h>
#include <stdio.h>

char* buf = 0;
BOOL WINAPI DllMain( HANDLE hDll,	// DLL 핸들(Load 된 주소)
					 DWORD  r,		// DllMain이 호출된 이유(4가지 중 1개)
					 LPVOID how)	// DLL을 Load한 방법(명시적또는 암시적)
{
	switch( r )
	{
	case DLL_PROCESS_ATTACH:  
		printf("DLL이 프로세스에 Load됩니다. 주소는 : %p\n", hDll);
		printf("Load방법은 %s\n", how == 0 ? "명시적":"암시적");
		buf = (char*)malloc( 1000 );
		break;

	case DLL_PROCESS_DETACH: free(buf); printf("DLL이 프로세스에서 해지 됩니다.\n");break;
	case DLL_THREAD_ATTACH: printf("새로운 스레드가 생성됩니다.\n");	break;
	case DLL_THREAD_DETACH: printf("스레드가 소멸됩니다.\n");break;
	}
	return TRUE; // FALSE를 리턴하면 DLL이 Load되지 않는다.
}

// 이제 모든 DLL 내의 함수에서 buf를 사용해도 된다.!!!








