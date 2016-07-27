#include <stdio.h>
#include <windows.h>

// STH : 구조화된 종료 처리 (Structual Termination Handling)
DWORD foo()
{
	int ret = 0;
	__try
	{
		return ret;
	}
	__finally
	{
		ret = 10;
		printf("try 블록을 벗어나기 전에 finally 구문은 반드시 실행됩니다\n");
	}
	return 0;
}
void main()
{
	printf("결과 : %d\n", foo() ); // ?
}












