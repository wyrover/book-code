// ProgramEntry.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <malloc.h>

class COne
{
public:
	COne()
	{
		printf("COne \r\n");
	}
	~COne()
	{
		printf("~COne \r\n");
	}
};

COne g_One;

int main()
{
	printf("main函数识别 \r\n");
	return 0;
}

void MyEntry()
{
	// 产生错误代码
	// int *p = (int*)malloc(16);
	main();
}