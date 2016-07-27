// b.cpp
#include <stdio.h>

// static library 사용하기.
#include "ABC.h"				// 1. 헤더 include
#pragma comment(lib, "ABC.lib")	// 2. library 추가

void main()
{
	int s = Add( 10, 20 );
	
	printf("결과 : %d\n", s );
}

// ABC.h와 ABC.lib(Debug 아래있습니다.)를 현재 작업 폴더로 복사해 오세요

// 컴파일 하고 실행해 보세요..

