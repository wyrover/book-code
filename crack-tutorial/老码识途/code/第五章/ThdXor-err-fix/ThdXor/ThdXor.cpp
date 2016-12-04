// ThdXor.cpp : 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include <windows.h>

#define ARRAY_LEN 10

struct Info{
	char f1[ARRAY_LEN];
	char f2[ARRAY_LEN];
	char key;
};

unsigned long __stdcall threadProcedure(void * arg)
{	
	//用 xor 加密
	struct Info * pInfo = (struct Info *)arg;
	for (int i = 0; i < ARRAY_LEN; i++)
	{
		pInfo->f2[i] = pInfo->f1[i] ^ pInfo->key;
	}	
	ExitThread(0); 
	return 0;
};

void PrintResult(struct Info * pInfo)
{
	printf("xor result: \n");
	for (int i = 0; i < ARRAY_LEN; i++)
	{
		printf("%d ", pInfo->f2[i]);
	}
	printf("\n");
}

HANDLE CreateThd(char start, struct Info ** ppInfo)
{
	struct Info * info = (struct Info *)malloc(sizeof(Info));
	* ppInfo = info;
	for (int i = 0; i < ARRAY_LEN; i++)
	{
		info->f1[i] = i + start;
	}
	info->key = 10;
	unsigned long threadID;		
	return CreateThread(NULL, 0, threadProcedure, info, 0, &threadID);
}

void main()
{
	HANDLE h1, h2;
	struct Info * p1, *p2;
	h1 = CreateThd(1, &p1);
	h2 = CreateThd(0, &p2);	
	if (h1 != NULL) 
	{
		WaitForSingleObject(h1, INFINITE);
		CloseHandle(h1);
		PrintResult(p1);
		free(p1);
	};	
	
	if (h2 != NULL) 
	{
		WaitForSingleObject(h2, INFINITE);
		CloseHandle(h2);
		PrintResult(p2);
		free(p2);
	};
}

