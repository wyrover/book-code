// test2.cpp : 定义控制台应用程序的入口点。
//


#include <stdio.h>
#include <string.h>
#include <windows.h>

#define LOOP_COUNT 20000000
#define STR_BUF_LEN 1024

 __declspec(naked) int strlen2(char * str)
 {
	 _asm{
		 MOV     EDX,EDI
		 MOV     EDI,[esp + 4]
		 MOV     ECX,0FFFFFFFFH
		 XOR     AL,AL
		 REPNE   SCASB
		 MOV     EAX,0FFFFFFFEH
		 SUB     EAX,ECX
		 MOV     EDI,EDX
		 ret
	 }
 }


void main()
{
	char str[STR_BUF_LEN];
	str[STR_BUF_LEN - 1] = 0;
	memset(str, 'a', STR_BUF_LEN - 1);
	int i;
	unsigned long start = GetTickCount();
	for (int i = 0; i < LOOP_COUNT; i++)
	{
		strlen(str);
	}	
    unsigned long end = GetTickCount();
	int u1 = end - start;

	start = GetTickCount();
	_asm push edx;
	for (int i = 0; i < LOOP_COUNT; i++)
	{		
		strlen2(str);
	}	
	_asm pop edx;
	end = GetTickCount();
	int u2 = end - start;
	float r;
	printf("strlen use %dms,  and strlen2 use %dms \n", u1, u2);
	if (u2 < u1)
	{
		r = u2 * 100 / u1;
		printf("strlen2/strlen = %f百分比\n", r);
	}
	else
	{
		r = u1 * 100 / u2;
		printf("strlen/strlen2 = %f百分比\n", r);
	}
	getchar();
}

