#include <windows.h>
#include <stdio.h>

void main()
{
	// 표준 C함수 사용 
//	system("freecell.exe");

	//API함수..
	WinExec("freecell.exe", SW_SHOW); // non blocking,

	printf("Bye~~\n");
}
