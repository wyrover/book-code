#include <stdio.h>
#include <windows.h>

int gi = 0;

EXCEPTION_DISPOSITION __stdcall ErrorHandler(PEXCEPTION_RECORD PtrExceptionRecord, void * PtrSEH, PCONTEXT PtrContext, void * PtrDispatcherContext)
{
	printf("hello error\n");
	PtrContext->Eax = (DWORD)&gi;
	return ExceptionContinueExecution;//让程序继续运行，不停止
}

void main()
{	
	char input;
	input = getchar();
	fflush(stdin);
    if (input == '1')
		goto compilerexcept;
	__asm
	{  //在栈上生成异常处理节点*************************
		xor eax, eax
		push ErrorHandler;
		push fs:[eax]
		mov fs:[eax], esp//******************************

	};
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
   return;

	

compilerexcept:	
	try
	{
		throw 1;
		int i = 0;
		int b = 4 / i;
		__asm xor eax, eax
			__asm mov [eax], 0;
	}
	catch (...)
	{
		gi = 12;
	}
	
		//  Sleep(10000);
}