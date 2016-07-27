// xyz.h

// 이 헤더 한개로 DLL을 만들때와 DLL을 사용할때 모두 사용하기 위해 조건부
// 컴파일 사용

#ifdef DLL_SOURCE
	#define DLLFUNC	__declspec(dllexport)   
#else
	#define DLLFUNC __declspec(dllimport)
#endif

#include <windows.h>

EXTERN_C DLLFUNC int Add( int a, int b);

//--------------------------------------------------------