// ptrptr.cpp : 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include <malloc.h>


void allocateInt(int * i)
{
  i = (int *) malloc(sizeof(int));
  *i = 3;
} 

void main()
{
	int * i;
	allocateInt(i);
	printf("*i = %d", *i);
}

