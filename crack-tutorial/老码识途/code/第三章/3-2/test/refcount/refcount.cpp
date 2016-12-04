// refcount.cpp : 定义控制台应用程序的入口点。
//
#include <stdio.h>
#include <malloc.h>
#include <memory.h>

void * mallocRef(int size)
{
	void * p = malloc(size + sizeof(int));
	if (p == NULL)
	{
		return NULL;
	}
	*(int *)p = 0;
	return (int *)p + 1;
}

void addRef(void * p)
{
	int * pCount = (int *)p - 1;
	*pCount = *pCount + 1;
	printf("refcount=%d\n", *pCount);
}

void releaseRef(void * p)
{
	int * pCount = (int *)p - 1;
	printf("refcount=%d\n", *pCount);
	if (*pCount - 1 <= 0)
	{
		printf("free memory\n");
		free(pCount);
		return;
	}
	*pCount = *pCount - 1;
}

void * gp;

void useGlobalP(void * p)
{
	addRef(p);
	if (gp != NULL)
	{
		releaseRef(gp);
	}	
	gp = p;
}

void freeGlobalP()
{
	if (gp != NULL)
	{
		releaseRef(gp);
	}	
}

void main()
{
	char * p = (char *)mallocRef(10);
	addRef(p);
	useGlobalP(p);
	memset(p, 0, 10);
	releaseRef(p);
	freeGlobalP();
}
