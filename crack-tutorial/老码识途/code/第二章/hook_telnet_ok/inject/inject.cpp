// inject.cpp : 定义 DLL 应用程序的导出函数。
//
#include <windows.h>
#include "../include/detours.h"

extern "C" BOOL __declspec(dllexport) inject(HANDLE hProcess, LPCSTR lpDllName)
{
	return DetourContinueProcessWithDllA(hProcess, lpDllName);
}


