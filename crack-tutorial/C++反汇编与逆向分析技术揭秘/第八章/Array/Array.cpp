// Array.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string.h>

int g_nArry[5] = {1, 2, 3, 4, 5};
// 参数为字符数组
void Show(char szBuff[])
{
	strcpy(szBuff, "Hello World");
	printf(szBuff);
}

// strlen函数分析
int GetLen(char szBuff[])
{
	return strlen(szBuff);
}

	
// 局部数组作为返回值
// char* RetArray()
// {
// 	char szBuff[] = {"Hello World"};
// 	return szBuff;
//}

// void __cdecl Show()
// {
// 	printf("Show\r\n");
//}

int __stdcall Show(int nShow)
{
	printf("Show : %d\r\n", nShow);
	return nShow;
}
void main(int argc, char *argv[ ], char *envp[ ] )
{
	// 局部数组的初始化
//	int nArry[5] = {1, 2, 3, 4, 5};
// 	char cChar = 'A';
// 	float fFloat = 1.0f;
// 	short sShort = 1;
// 	int nInt = 2;
//	double dDouble = 2.0f;

// 	int nOne = 1;
// 	int nTwo = 2;
// 	int nThree = 3;
// 	int nFour = 4;
//	int nFive = 5;

	// 数组作为参数
// 	char szHello[20] = {0};
//	Show(szHello);

	// strlen函数讲解
	printf("%d \r\n", GetLen("Hello"));

	// 三种下标寻址方式
	int nArry[5] = {1, 2, 3, 4, 5};
	
	// 整型常量
	printf("%d \r\n", nArry[2]);
	// 整形变量
	printf("%d \r\n", nArry[argc]);
	// 表达式
	printf("%d \r\n", nArry[argc * 2]);

	// 调用返回值为局部变量
//	printf("%s\r\n", RetArray());

	// 全局数组使用
// 	for (int i = 0; i < 5; i++)
// 	{
// 		printf("%d", g_nArry[i]);
//	}

	// 局部静态数组
// 	int nOne;
// 	int nTwo;
// 	scanf("%d%d", &nOne,&nTwo);
// 	static int g_snArry[5] = {nOne, nTwo, 0};
// 
// 	for (int i = 0; i < 5; i++)
// 	{
// 		printf("%d", g_snArry[i]);
//	}

	// 下标、指针寻址
// 
// 	char * pChar = NULL;
// 	char szBuff[10] = {0};
// 	scanf("%9s", szBuff);
// 	pChar = szBuff;
// 	printf("%c", *pChar);
//	printf("%c", szBuff[0]);

	// 二维数组与一维数组寻址比较
// 	int i = 0;
// 	int j = 0;
// 	int nArray[4] = {1, 2, 3, 4};
// 	int nTwoArray[2][2] = {{1, 2},{3, 4}};
// 	scanf("%d %d", &i, &j);
// 	printf("nArray = %d\r\n", nArray[i]);
//	printf("nTwoArray = %d\r\n", nTwoArray[i][j]);


// 	int i = 0; 
// 	int nTwoArray[2][2] = {{1, 2},{3, 4}};				// 二维数组
// 	scanf("%d", &i);
// 	printf("nTwoArray = %d\r\n", nTwoArray[1][i]);		// 

	// 三维数组
// 	int x = 0,y = 0,z = 0;
// 
// 	int nArray[3][3][3] = {{1,2,3},{4,5,6},{7,8,9}};
// 	scanf("%d %d %d", &x, &y, &z);
// 
//	printf("%d", nArray[x][y][z]);

	// 指针数组
// 	char * pBuff[3] = {
// 		"Hello ",
// 		"World ",
// 		"!\r\n"
//	};
// 	for (int i = 0; i < 3; i++)	{
// 		printf(pBuff[i]);
//	}

	// 二维字符数组
// 	char cArray[3][10] = {
// 		"Hello ",
// 		"World ",
// 		"!\r\n"
// 	};
// 		for (int i = 0; i < 3; i++)	{
// 		printf(cArray[i]);
//	}

	// 数组指针
// 	char (*pArray)[10] = cArray;
// 	for (int i = 0; i < 3; i++)
// 	{
// 		printf(*pArray);
// 		pArray++;
//	}

	// 数组指针获取命令行参数
// 	for (int i = 1; i < argc; i++)
// 	{
// 		printf(argv[i]);
// 		printf(" ");
//	}

	// 函数指针
// 	 void (__cdecl *pShow)(void) = Show;
// 	 void (__cdecl *pShow1)(void) = Show;
// 
// 	 pShow();
//	 Show();

	int (__stdcall *pShow)(int) = Show;
	int nRet = pShow(5);
	printf("ret = %d \r\n", nRet);
}

