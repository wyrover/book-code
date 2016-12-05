// VC80Fun.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

void AsmStack(int argc)
{
	__asm push eax
		if (argc > 0)
		{		// 此处虽然为永真，但由于是变量，编译器无法假定其永真
			__asm pop eax
		}
		int nVar = 0;
		scanf("%d", &nVar);
		printf("AsmStack %\r\n", nVar);
}
int _tmain(int argc, _TCHAR* argv[]){
	// 此程序中参数argc必然为1
	AsmStack(argc);
	return 0;
}

