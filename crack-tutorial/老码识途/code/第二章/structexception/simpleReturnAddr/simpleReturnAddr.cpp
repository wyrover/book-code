#include <stdio.h>
#include <windows.h>

int gi = 0;

DWORD normalAddr;

EXCEPTION_DISPOSITION __stdcall ErrorHandler(PEXCEPTION_RECORD PtrExceptionRecord, void * PtrSEH, PCONTEXT PtrContext, void * PtrDispatcherContext)
{
	printf("hello error\n");
	PtrContext->Eip = normalAddr;
	return ExceptionContinueExecution;//让程序继续运行，不停止
}

void main()
{
	_asm mov normalAddr, offset Normal
	__asm
	{  //在栈上生成异常处理节点*************************
		xor eax, eax
		push ErrorHandler;
		push fs:[eax]
		mov fs:[eax], esp//******************************
	};
	char input;

	input = getchar();
	if (input != '0')
		printf("ok\n");
	else
		__asm
	{
		xor eax, eax
			mov [eax], 0;//写空地址，触发异常！！！！程序将转向这里
	}

Normal:
	__asm
	{
		pop ecx //将前一个节点的地址弹入ecx
			xor eax , eax
			mov fs:[eax], ecx //将前一个节点的地址作为首节点地址存入fs:[0]
			pop ecx		
			//****************************
	};
	getchar();
}

