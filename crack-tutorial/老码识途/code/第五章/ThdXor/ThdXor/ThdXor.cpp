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
	
	printf("xor result: \n");
	for (int i = 0; i < ARRAY_LEN; i++)
	{
		printf("%d ", pInfo->f2[i]);
	}
	printf("\n");
	ExitThread(0); 
	return 0;
};


void main()
{
	Info info;
	for (int i = 0; i < ARRAY_LEN; i++)
	{
		info.f1[i] = i;
	}
	info.key = 10;
    unsigned long threadID;
	HANDLE threadHandle;	
	threadHandle = CreateThread(NULL, 0, threadProcedure, &info, 0, &threadID);
	if (threadHandle != NULL) 
	{
		WaitForSingleObject(threadHandle, INFINITE);
		CloseHandle(threadHandle);
	};
}

