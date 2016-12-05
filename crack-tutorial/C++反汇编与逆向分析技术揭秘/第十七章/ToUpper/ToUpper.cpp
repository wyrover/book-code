// ToUpper.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <string.h>

// void ToUpper(char * pBuff)
// {
// 	int nLen = strlen(pBuff);
// 	int nDiffer = 'a'  - 'A';
// 
// 	for (int i = 0; i < nLen; i++)
// 	{
// 		if (pBuff[i] >= 'a' && pBuff[i] <= 'z')
// 		{
// 			pBuff[i] -= nDiffer;
// 		}
// 	}
// }

extern "C" void ToUpper(char *);

int main(int argc, char* argv[])
{
	char szBuff[256] = {0};
	printf("请输入字符串：\n");
	scanf("%255s", szBuff);
	ToUpper(szBuff);
	MessageBox(NULL, szBuff, "转换结果", MB_OK);
	
	return 0;
}

