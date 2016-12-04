// test.cpp : 定义控制台应用程序的入口点。
//
#include "../inc/common.h"
#include <stdio.h>
void main()
{
	Parent* p = createObj();
	p->Destroy();
}

