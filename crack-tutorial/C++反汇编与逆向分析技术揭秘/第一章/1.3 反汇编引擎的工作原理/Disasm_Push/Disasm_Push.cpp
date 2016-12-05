// Disasm_Push.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string.h>
#include "Decode2Asm.h"

void main()
{
	// 假设此字符数组为机器指令编码
	unsigned char szAsmData[] = {
		0x6A, 0x00,					// PUSH 00
		0x68,0x00,0x30,0x40,0x00,	// PUSH 00403000
		0x50,						// PUSH EAX
		0x51,						// PUSH ECX
		0x52,						// PUSH EDX
		0x53						// PUSH EBX
	};

	char szCode[256] = {0};			// 存放汇编指令信息
	unsigned int nIndex = 0;		// 每条机器指令长度，用于地址偏移
	unsigned int nLen = 0;			// 分析数据总长度
	unsigned char *pCode = szAsmData;		

	nLen = sizeof(szAsmData);
	while (nLen)
	{
		if (nLen < nIndex)
		{
			break;
		}
		int nAddr = 0 ;
		// 修改pCode偏移
		pCode +=  nIndex;
		// 解析机器码
		Decode2Asm(pCode, szCode, &nIndex, 0);
		// 显示汇编指令
		puts(szCode);
		memset(szCode, 0, sizeof(szCode));
	}

}

