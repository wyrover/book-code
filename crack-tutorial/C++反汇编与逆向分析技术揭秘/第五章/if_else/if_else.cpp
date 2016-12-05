// if_else.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// if 结构
void If(int argc)
{
	if (argc == 0)
	{
		printf("%d \r\n", argc);
	}
	if (argc > 0)
	{
		printf("argc >= 0");
	}
	
	return;
}

// if else 结构
void IfElse(int argc)
{
// 	if (argc == 0)
// 	{
// 		printf("argc == 0");
// 	}
// 	else
// 	{
// 		printf("argc != 0");
//	}

	// 模拟条件表达式 转换方式1
	if (argc == 0)
	{
		argc = 5;
	}
	else
	{
		argc = 6;
	}
	printf("%d \r\n", argc);
}

// if else if 组合
void IfElseIf(int argc)
{
	argc = 0;
	if (argc > 0)
	{
		printf("argc > 0");
	}
	else if (argc == 0)
	{
		printf("argc == 0");
	}
	else
	{
		printf("argc <= 0");
	}
}

int main(int argc, char* argv[])
{

	int a = 7;
	int b = !!a;



	printf("%f", 10);
	// If(argc);
	// IfElse(argc);
	IfElseIf(argc);
	return 0;
}

