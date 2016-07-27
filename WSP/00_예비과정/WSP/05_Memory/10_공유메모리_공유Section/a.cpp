#include <stdio.h>
#include <windows.h>
#include <conio.h>
// 초기화된 전역 변수 .data section 에 놓인다 - 기본적으로 COPY ON WRITE
int x = 0; 

#pragma data_seg("AAA")	// exe(PE) 에 새로운 data section AAA를 만든다
int y = 0;
#pragma data_seg()

// AAA 섹션의 속성을 지정한다. (Read, Write, Share)
#pragma comment(linker, "/section:AAA,RWS") 

void main()
{
	++x; ++y;

	printf("x = %d\n", x);
	printf("y = %d\n", y);

	getch();
}
// 죽이지 말고 2번이상 실행해 보세요.!!



/*  
	메모리 정리 1. Copy On Write 라는 개념 - 중요한 OS개념 이므로 꼭 알아 둘것...
	            2. 공유 메모리 만드는 기법. 

*/




