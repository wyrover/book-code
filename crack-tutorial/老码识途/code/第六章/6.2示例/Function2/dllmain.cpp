// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"

#include <windows.h>
#include <iostream>
using namespace std;

extern "C" __declspec(dllexport)  void Print(void)
{
	cout << "Hello China!" << endl;
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, void* lpReserved) 
{
	HANDLE g_hModule;
	switch(dwReason)
	{
	case DLL_PROCESS_ATTACH:
		g_hModule = (HINSTANCE)hModule;
		break;

	case DLL_PROCESS_DETACH:
		g_hModule=NULL;
		break;

	case DLL_THREAD_ATTACH:
		g_hModule = (HINSTANCE)hModule;
		break;

	case DLL_THREAD_DETACH:
		g_hModule = NULL;
		break;
	}
	return true;
}