#include "GlobalResource.h"

#include <map>
#include <malloc.h>

HINSTANCE applicationHandle;

std::map<HWND, void *> windowHandles;

void initApp(HINSTANCE instance)
{
	applicationHandle = instance;	
}

__declspec(naked) LRESULT CALLBACK WndProcWrapper(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	_asm{
		pop eax;
		mov ecx, [eax];
		mov eax, [eax + 4];
		jmp eax;
	}
}

void * makeWndProcStub(void * objPtr, void * msgHandler)
{	
	char * code = (char *)malloc(13);//call 5 bytes, objPtr and msgHandler 8 bytes
	code[0] = 0xe8;
	*((long*)&code[1]) = (long)((char *)WndProcWrapper - (code + 5));//offset to WndProcWrapper
	*((void **)&code[5]) = objPtr;
	*((void **)&code[9]) = msgHandler;
	return code;
}