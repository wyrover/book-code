// a.c
#define DLL_SOURCE
#include "xyz.h"	// 이 안에 있는 DLLFUNC는 __declspec(dllexport)로 된다.

int Add( int a, int b )
{
	return a + b;
}
