// testscanf.cpp : 定义控制台应用程序的入口点。
//
#include<stdio.h>

void main()
{
	char cmd[30];
	while (1)
	{
		scanf("%[^\n]", cmd);
		printf("%s\n", cmd);
	}
}

