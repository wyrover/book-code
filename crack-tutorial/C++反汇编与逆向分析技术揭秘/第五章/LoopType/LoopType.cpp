// LoopType.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// goto模拟do循环完成正数累加
int GoToDo(int nCount)
{
	int nSum = 0;
	int nIndex = 0;
GOTO_DO:
	nSum += nIndex;
	nIndex++;
	if (nIndex <= nCount)
	{
		goto GOTO_DO;
	}
	return nSum;
}
int LoopDO(int nCount)
{
	
	int nSum = 0;
	int nIndex = 0;
	do 
	{
		nSum += nIndex;
		nIndex++;
	} while(nIndex <= nCount);
	return nSum;
}
int LoopWhile(int nCount)
{
	int nSum = 0;
	int nIndex = 0;
	while (nIndex <= nCount)
	{
		nSum += nIndex;
		nIndex++;
	}
	return nSum;
}

int LoopFor(int nCount)
{
	int nSum = 0;
	for (int nIndex = 0; nIndex <= nCount; ++nIndex)
	{
		nSum += nIndex;
	}
	return nSum;
}

// 代码外提
int CodePick(int nCount)
{
	int nSum = 0;
	int nIndex = 0;
	do 
	{
		nSum += nIndex;
		nIndex++;
	} while(nIndex < nCount - 1);
	return nSum;
}

// 强度降低
void DoRate(int argc)
{
  int t = 0;
  int i = 0;
  while (t < argc)
  {
    t = i * 99; 
    i++;
  }
  printf("%d", t);

}

void main(int argc, char *argv[ ], char *envp[ ])
{
//	printf("%d \r\n", GoToDo(5));
//	printf("%d \r\n", LoopDO(5));
//	printf("%d \r\n", LoopWhile(5));
//	printf("%d \r\n", LoopFor(5));
	printf("%d \r\n", CodePick(5));
	DoRate(argc);
	
}

