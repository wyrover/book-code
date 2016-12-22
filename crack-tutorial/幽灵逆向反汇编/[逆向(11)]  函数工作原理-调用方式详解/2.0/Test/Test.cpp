// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>

void _cdecl FuncC(int i, int j, int k)
{
    printf("i %d, j %d k %d \n", i, j, k);
    printf("FuncC");
    printf("i %d", i);

}

void _stdcall FuncStd(int i, int j, int k)
{
    printf("i %d, j %d k %d \n", i, j, k);
}

void _fastcall FuncFast(int i, int j, int k)
{
    printf("i %d, j %d k %d \n", i, j, k);
}

int main(int argc, char* argv[])
{
    printf("hwllo    ");
    printf("Fun");
    printf("worl");
    printf("cccdfdfd");    
    printf("Fun");
    printf("fds");
    printf("fd");    
    printf("Fun");
    printf("worl");
    printf("fdcvf");    
    printf("Fun");
    printf("we");
    printf("sdf");    
    printf("Fun");
    printf("fdas");
    printf("afds");
	return 0;
}

