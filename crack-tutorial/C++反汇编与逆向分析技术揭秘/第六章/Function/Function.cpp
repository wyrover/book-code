// Function.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

typedef void (*p)();

void __stdcall ShowStd(int nNumber)
{
	printf("%d \r\n", nNumber);
}
void __cdecl ShowCde(int nNumber)
{
	printf("%d \r\n", nNumber);
}

void __fastcall ShowFast(int nOne, int nTwo, int nThree, int nFour)
{
	printf("%d %d %d %d\r\n", nOne, nTwo, nThree, nFour);
}




// ebp 与 esp访问
void InNumber()
{
	
	// 局部变量定义
	int nInt = 1;
	scanf("%d", &nInt);
	char cChar = 2;
	scanf("%c", &cChar);

	printf("%d %c\r\n", nInt, cChar);
}

// 两数交换
void AddNumber(int nOne)
{
	nOne += 1;
	printf("%d \r\n", nOne);
}

int GetAddr(int nNumber)
{
	int nAddr = *(int*)(&nNumber - 1);
	return nAddr;
}


struct tagTEST 
{
	int m_nOne;
	int m_nTwo;
};

tagTEST RetStruct()
{
	tagTEST testRet;
	testRet.m_nOne = 1;
	testRet.m_nTwo = 2;
	return testRet;
}

void AsmStack()
{
	__asm
	{
		push eax
		pop eax
	}
	int nVar = 0;
	scanf("%d", &nVar);
	printf("AsmStack %\r\n", nVar);
}

void main()
{
	AsmStack();
// 	tagTEST test;
//	test = RetStruct();
// 	int nAddr = GetAddr(1);
// 	int nReload = (nAddr + *(int *)(nAddr - 4)) - (int)GetAddr;
// 	
// 	int nNumber = 0;
// 	scanf("%d", &nNumber);
//	AddNumber(nNumber);
	
// 	ShowStd(5);
//	ShowCde(5);
//	ShowFast(1, 2, 3, 4);
// 	printf("Hello ");
// 	printf("World");
// 	printf(" C++");
//	printf("\r\n");

	InNumber();
}
