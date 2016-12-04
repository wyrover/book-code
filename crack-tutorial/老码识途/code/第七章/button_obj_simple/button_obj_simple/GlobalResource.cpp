#include "GlobalResource.h"

#include <map>

HINSTANCE applicationHandle;

std::map<HWND, void *> windowHandles;

CRITICAL_SECTION  handlesLock;

void initApp(HINSTANCE instance)
{
	applicationHandle = instance;
	InitializeCriticalSection(&handlesLock);
}

void addWindowHandle(HWND handle, void * objPtr)
{
	EnterCriticalSection(&handlesLock);
	windowHandles[handle] = objPtr;
	LeaveCriticalSection(&handlesLock);
}

void delWindowHandle(HWND handle)
{
	EnterCriticalSection(&handlesLock);
	windowHandles.erase(handle);
	LeaveCriticalSection(&handlesLock);	
}

void * getObject(HWND handle)
{
	EnterCriticalSection(&handlesLock);
	void * ret = windowHandles[handle];
	LeaveCriticalSection(&handlesLock);	
	return ret;
}