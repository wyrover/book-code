// Operation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream.h>

void Add(int argc)
{
	// 无效语句
	15+20;
	// 加法运算
	
// 	int nVarOne = 0;
	int nVarOne = 1 + 5 - 3 * 6;
	cout << nVarOne << endl;
//	int nVarTwo = 0;
//	int nVarOne = argc;
// 	int nVarTwo = argc;
// 
// 	nVarOne = nVarOne + 1;
// 	nVarOne = 1 + 2;
// 	nVarOne = nVarOne + nVarTwo;
// 
//	printf("nVarOne = %d \r\n", nVarOne);
}

void Sub(int argc)
{
	// 变量定义
	int nVarOne = argc;
	int nVarTwo = argc;
	scanf("%d", &nVarTwo);
	// 变量减常量的减法运算
	nVarOne = nVarOne - 100;
	// 减法与加法混合运算
	nVarOne = nVarOne + 5 - nVarTwo ;
	printf("nVarOne = %d \r\n", nVarOne);
	
}

void Mul(int argc)
{
	int nVarOne = argc;
	int nVarTwo = argc;
	// 乘法运算
	printf("nVarOne * 15 = %d", nVarOne * 15);
	printf("nVarOne * 16 = %d", nVarOne * 16);
	printf("2 * 2 = %d", 2 * 2);
	printf("nVarTwo * 4 + 5 = %d", nVarTwo * 4 + 5);
	printf("nVarTwo * 9 + 5 = %d", nVarTwo * 9 + 5);
	printf("nVarOne * nVarTwo = %d", nVarOne * nVarTwo);
}

void Div(int argc)
{
	int nVarOne = argc;
	int nVarTwo = argc;
	// 除法运算
	printf("nVarOne / nVarTwo = %d", nVarOne / nVarTwo);
	printf("nVarOne / 2 = %d", nVarOne / 2);
	printf("nVarTwo / 7 = %d", nVarTwo / 7);
	printf("nVarTwo % 7 = %d", nVarTwo % 7);
	printf("nVarOne / 8 = %d", nVarOne / 8);
	printf("nVarOne / 14 = %d", argc / 14);
}

// 假象死循环
void BreakFor()
{
 	for (int i = 1; i > 0; i++)
 	{
 		printf("%d \r\n", i);
	}
}

// 自增自减
void AddSub(int argc)
{
	int nVarOne = argc;
	int nVarTwo = argc;
	nVarTwo = 5 + (nVarOne++);
	nVarTwo = 5 + (++nVarOne);

	nVarOne = 5 + (nVarTwo--);
	nVarOne = 5 + (--nVarTwo);
}

void Jmp(int argc)
{
	if (argc == 0)
	{
		printf("%d \r\n", argc);
	}
	else if (argc)
	{
	}

}

// 表达式短路
int Accumulation(int nNumber)
{
	// nNumber && (nNumber += Accumulation(nNumber - 1));

	(nNumber == 0) || (nNumber += Accumulation(nNumber - 1));
	return  nNumber;
}

// 条件表达式
int Condition(int argc, int n)
{
//	return argc == 5 ? 5 : 6;
//	return argc == 5 ? 4 : 10;
	return argc >= 8 ? 4 : 10;
//	return argc == 8 ? n : 10;

}

// 位运算
int BitOperation(int argc)
{
	unsigned int nVar = argc;
	nVar <<= 3;
	nVar >>= 5;

	argc = argc << 3;
	argc = argc >> 5;
	argc = argc | 0xFFFF0000;
	argc = argc & 0x0000FFFF;
	argc = argc ^ 0xFFFF0000;
	argc = ~argc;

	return argc;
}

int main(int argc, char* argv[])
{
	printf("nVarOne % 2 = %d", argc % 2);
	printf("nVarOne % -2 = %d", argc % -2);
	//Add(argc);
	//Sub(argc);
	//Mul(argc);
	Div(argc);
	//AddSub(argc);
	//int nNumber = Accumulation(3);
	printf("%d \r\n", Condition(argc, argc + 5));
	BitOperation(argc);
	return 0;
}

