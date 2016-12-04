#pragma once

#include <windows.h>

extern HINSTANCE applicationHandle;

void * makeWndProcStub(void * objPtr, void * msgHandler);

class Application
{
public:
	static void initApp(HINSTANCE instance);
	static int handleMessage();
};