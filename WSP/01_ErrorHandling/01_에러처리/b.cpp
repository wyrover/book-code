#include <stdio.h>
#include <windows.h>
#include <errno.h>
#include <conio.h>

// 전통적인 C 의 에러 처리 방법 - 
// 단점 프로세스당 1개의 에러 코드.
void main()
{
	FILE* f = fopen("a.txt", "rt");

	if ( f == 0 )
	{
		printf("실패 \n");

		// 1. errno : C 언어가 미리 정의한 전역 변수.
		printf("errno : %d\n", errno ); 

		// 2. errno-> 문자열로!
		char* s = strerror( errno ); 
		printf("%s\n", s );

		// 1.2통합 fprintf(stderror, "Fail : %s\n", strerror(errno));
		perror("Fail"); 
	}
}
