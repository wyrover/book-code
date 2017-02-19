// DataType.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

float GetFloat()
{
	return 12.25f;
}

void Add(int &nVar)
{
	nVar++;
}

int main(int argc, char* argv[])
{
	// 在未使用到浮点数情况下
	// 输入小数会报错，因没有进行浮点寄存器初始化
	// int nInt = 0;
	// scanf("%f", &nInt);
	// BUG解决办法,定义浮点类型，将浮点寄存器进行初始化

	
	// float fFloat = 12.25f;	
	// fFloat IEEE转换	符号为正	指数为正
	// 0100 0001 0100 0100 0000 0000 0000 0000
	// 41  44 00 00 
	// 00 00 44 41

	// 1011 1110 00000000000000000000000
	// 1011 1110 

	// 符号为负		指数为负
	//float fFloat = -0.125f;

	// 无穷转换2进制
	// float fFloat = 1.3f;
	// 转换2进制1.01001100110011001100110

	// float fTemp = 0.0001f;	// 精确范围
	// if (fFloat >= -fTemp && fFloat <= fTemp)
	// {
		// fTemp等于0
	// }

	// double双精度类型转换
	// double dDouble = 12.25;	// 1100.01
	// 00 00 00 00 00 80 28 40 
	// 40 28 80 00 00 00 00 00
	// 0100 0000 0010 1000 1000 ...
	// 0 10000000010 10001

	// 浮点数使用
	// float fFloat = (float)argc;
	// printf("%f", fFloat);
	// argc = (int)fFloat;
	// printf("%d", argc);

	// fFloat = GetFloat();
	// printf("%f", fFloat);

	// 字符和字符串
// 	wchar_t *pwChar = L"Hello World!";
// 	char	*pcChar = "Hello World!";
// 
// 	// 汉字字符串
// 	pcChar = "逆向分析";
// 	pwChar = L"逆向分析";
// 
// 	// 布尔类型
// 	bool bBool;
// 	if (argc > 0)
// 	{
// 		bBool = true;
// 	}
// 	else
// 	{
// 		bBool = false;
// 	}
// 	
// 	
// 	if (bBool == false)
// 	{
// 		printf("布尔类型\r\n");
//	}

	// 地址、指针、引用

	// 对指针取内容
// 	int nVar = 0x12345678;
// 	int *pnVar = &nVar;
// 	char *pcVar = (char*)&nVar;
// 	short *psnVar = (short*)&nVar;
// 	printf("%08x \r\n", *pnVar);
// 	printf("%08x \r\n", *pcVar);
// 	printf("%08x \r\n", *psnVar);

	// 指针偏移
// 	char cVar[5] = {0x01, 0x23, 0x45, 0x67, 0x89}; 
// 
// 	int *pnVar = (int*)cVar;
// 	char *pcVar = (char*)cVar;
// 	short *psnVar = (short*)cVar;
// 
// 	pnVar += 1;
// 	pcVar += 1;
// 	psnVar += 1;
// 
// 	printf("%08x \r\n", *pnVar);
// 	printf("%08x \r\n", *pcVar);
// 	printf("%08x \r\n", *psnVar);

	// 引用类型
	//int nVar = 0x12345678;
	// 引用类型定义
	//int &nVarTpye = nVar;
	// 调用函数，参数为引用类型
	//Add(nVar);

	// 常量
// 	char *pString = "Hello World!";
//	printf(pString);
	
	// 宏与conset

// 	#define NUMBER_ONE	1
// 	const int nVar = NUMBER_ONE;
//	printf("const = %d   #define = %d \r\n", nVar, NUMBER_ONE);

	// const常量修改

// 	const int nConst = 5;
// 	int *pConst = (int*)&nConst;
// 	*pConst = 6;
//	int nVar = nConst;

	return 0;
}

