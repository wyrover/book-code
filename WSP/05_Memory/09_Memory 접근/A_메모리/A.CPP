#include <windows.h>
#include <stdio.h>

void main()
{
	printf("Process ID : %d\n", GetCurrentProcessId() );

	char pass[256] = {0};
	printf("pass 주소 : %p\n", pass );

	while ( 1 )
	{
		printf("암호를 입력하세요 >>");
		scanf("%s", pass );
	}
}

