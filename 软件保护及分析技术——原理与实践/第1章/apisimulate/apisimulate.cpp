// apisimulate.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <windows.h>
int _tmain(int argc, _TCHAR* argv[])
{
	//这里利用一条汇编指令使程序进入死循环状态方便我们利用调试器定位代码对应的汇编指令位置
	__asm jmp $
	//在这个程式当中我们仅仅直接打印出进程ID,这里使用GetCurrentProcessID系统函数
	//我们的目的在于观察Asprotect保护系统对该函数进行的函数模拟
	printf("Current ProcessID:%d\n", GetCurrentProcessId());
	return 0;
}

