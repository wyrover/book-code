// ABC.h
// ABC.lib에 있는 모든 함수의 선언부를 제공한다.
// 모든 C++ 컴파일러는 __cplusplus 라는 매크로를 정의 하고 있다.(산업 표준)

/*
#ifdef __cplusplus
	#define EXTERN_C extern "C"
#else
	#define EXTERN_C 
#endif
*/

#include <windows.h>   // EXTERN_C 가 내부적으로 정의 되어 있다.

EXTERN_C int Add( int , int );

// 위 처럼 헤더를 만들면 C, C++ 모두에서 사용할수 있다.
// 규칙.
// 1. 라이브러리 자체를 만들때는 .c 를 사용해서 mangling을 막는다.
// 2. 헤더를 만들때는 매크로를 사용 C/C++ 모두에서 사용할수 있게 한다.







