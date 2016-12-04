// asmjmp.cpp : 定义控制台应用程序的入口点。
//
#include <stdio.h>
#include <malloc.h>

int gi;
void * address;

void * buildCode()
{
	//我们要构建的代码如下
	// mov gi, 18
	// jmp address;

	//mov gi, 被赋值整数；指令是10个字节组成C7 05 gi的地址四字节 被赋值整数四字节
	//jmp address;6字节，前两个为ff 25，后4字节为地址
	//因此我们需要分配16个字节
	char * code = (char *)malloc(16);
	char * pMov = code;
	char * pJmp = code + 10;
	char * pAddress;
	//mov gi, 18
	pMov[0] = 0xc7;
	pMov[1] = 0x05;
	pAddress = pMov + 2;
	*((int *)pAddress) = (int)&gi;
	*((int *)(pAddress + 4)) = 18;
	//jmp address
	pJmp[0] = 0xff;
	pJmp[1] = 0x25;
	*((int *)(&pJmp[2])) = (int)&address;
	return code;
}

void main()
{	
	void * code = buildCode();
	_asm {
		mov address, offset _lb1;	
	}
	gi = 12;
	printf("gi = %d\n", gi);
	_asm jmp code;//执行我们自己构建的代码
	gi = 13;
	_lb1:
	printf("gi = %d\n", gi);//打印的结果为18，而不是12
	getchar();
}

