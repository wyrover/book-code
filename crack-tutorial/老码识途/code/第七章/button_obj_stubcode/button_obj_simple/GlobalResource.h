#pragma once

#include <windows.h>

extern HINSTANCE applicationHandle;

void initApp(HINSTANCE instance);

void addWindowHandle(HWND handle, void * objPtr);

void delWindowHandle(HWND handle);

void * getObject(HWND handle);

void * makeWndProcStub(void * objPtr, void * msgHandler);

void log(char * msg);