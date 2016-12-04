// myprogram_load.cpp : 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include <malloc.h>
#include <memory.h>

#define RELOCATION_ITEM_SIZE sizeof(int)
#define RELOCATION_ITEMS_SIZE RELOCATION_ITEM_SIZE * 1 //只有一个重定位项
#define DATA_SEGMENT_LEN 1 * sizeof(int)//只有一个整数
#define CODE_SEGMENT_LEN MOV_CODE_LEN + RET_LEN //mov指令为1条，一条ret指令
#define MOV_CODE_LEN 10
#define RET_LEN 1
#define RET_CODE 0xc3
#define PROGRAM_LEN sizeof(PrgHdr) + RELOCATION_ITEMS_SIZE + DATA_SEGMENT_LEN + CODE_SEGMENT_LEN

struct PrgHdr{
	int imageBase;//程序希望被加载的地址
	int mainEntryOffset;//主函数相对程序头部的偏移量
	int relocationCount;//重定位项数目  
};


void buildCode(char * codeStart, int varAddress)
{
	//mov [00403374h],12345678h的机器码 C7 05 74 33 50 00 78 56 34 12，其中c7 05表示mov，第3~6自己为被赋值地址，后4自己为被赋的值。
	//构建mov指令码---
	codeStart[0] = 0xc7;
	codeStart[1] = 0x05;
	//----
	int * pVarAddr;
	pVarAddr = (int *)&codeStart[2];
	*pVarAddr = varAddress;//构建地址部分
	int * pVar;
	pVar = (int *)&codeStart[6];
	*pVar = 0x12345678;//构建被赋值
    *(codeStart + MOV_CODE_LEN) = RET_CODE;//构建ret指令
}

//构造一个程序块，包括头部、重定位表、数据段、代码段
void * makeProgram(int expectedBase)
{
	int relocationTbLen;
	struct PrgHdr * prg = (struct PrgHdr *)malloc(PROGRAM_LEN);
	prg->imageBase = expectedBase;
	prg->relocationCount = 1;
	relocationTbLen = (prg->relocationCount * RELOCATION_ITEM_SIZE);
	prg->mainEntryOffset = sizeof(struct PrgHdr) + relocationTbLen + DATA_SEGMENT_LEN;
	
	*(int *)(prg + 1) = prg->mainEntryOffset + 2;//填写重定位项，mov指令的码为c7 05长两字节所以加2
	char * codeStart = (char *)prg + prg->mainEntryOffset;	
	int expectedGlobalVarAddress = expectedBase + sizeof(PrgHdr) + relocationTbLen;
	buildCode(codeStart, expectedGlobalVarAddress);    
	int * pGlobalVar = (int *)((char *)prg +  sizeof(PrgHdr) + relocationTbLen);
    * pGlobalVar = 1;//给全局变量赋初值为1， 相当于c语言中int gi = 1;即程序加载后，执行main之前的值
	return (void *)prg;
}

char gProgramLocation[PROGRAM_LEN];

void * makeProgramWillRelocate()
{
	return makeProgram(0x00400000);
}

void * makeProgramWithoutRelocate()
{
	return makeProgram((int)gProgramLocation);
}

//prgLoaded代表被加载的程序模块的起始地址，prgInMem代表被加载到内存的起始地址
void loadProgram(void * prgLoaded, void * prgInMem)
{
	memcpy(prgInMem, prgLoaded, PROGRAM_LEN);
	struct PrgHdr * prgHdr = (struct PrgHdr *)prgInMem;
	int expectedBase = prgHdr->imageBase;
	if (expectedBase == (int)prgInMem)
	{
		printf("need not relocate\n");
		return;
	}
	else
	{
		printf("need relocate, expected address is 0x%x, but actualAdress is 0x%x\n", expectedBase, prgInMem);
		printf("relocation count is %d\n", prgHdr->relocationCount);
		int *pRelocationItem = (int *)(prgHdr + 1);
		int offsetLoad = (int)prgInMem - expectedBase;
		for (int i = 0; i < prgHdr->relocationCount; i++)
		{
			int * pRelocateAddress = (int *)((char *)prgInMem + *pRelocationItem);
			int oldValue = * pRelocateAddress;
			* pRelocateAddress = oldValue + offsetLoad;
			pRelocationItem ++;
			printf("make a relocation, the value from 0x%x to 0x%x\n", oldValue, * pRelocateAddress);
		}
		return;
	}
}

void runProgram(void * prgInMem)
{
	struct PrgHdr * prgHdr = (struct PrgHdr *)prgInMem;
	void * mainFunctionAddress = (void *)((char *)prgInMem + prgHdr->mainEntryOffset);
	_asm call mainFunctionAddress;
}

void printTheGlobalVar(void * prgInMem)
{
	int * pGlobalVar = (int *)((char *)prgInMem + sizeof(struct PrgHdr) + RELOCATION_ITEMS_SIZE);
	printf("global varable in the loaded program is 0x%x\n", * pGlobalVar);
}

void main()
{
	//这个例子将展示加载以及重定位的效果
	void * prgLoaded = makeProgramWillRelocate();
	char prgLocation[PROGRAM_LEN];
	loadProgram(prgLoaded, prgLocation);
	printTheGlobalVar(prgLocation);
	runProgram(prgLocation);
	printTheGlobalVar(prgLocation);
	free(prgLoaded);
	//-------------

	//这个例子将展示加载以及不重定位的效果
	prgLoaded = makeProgramWithoutRelocate();
	loadProgram(prgLoaded, gProgramLocation);
	printTheGlobalVar(gProgramLocation);
	runProgram(gProgramLocation);
	printTheGlobalVar(gProgramLocation);
	free(prgLoaded);
	//-------------
}

