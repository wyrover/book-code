#include <windows.h>
#include <iostream>
#include "../main/IPrintPlugin.h"
using namespace std;

const int FUNC_ID = 1;

class CPrintPlugin : public IPrintPlugin
{
public:
	CPrintPlugin()
	{
	}

	virtual ~CPrintPlugin()
	{
	}

	virtual void Print()
	{
		cout << "Hello World!" << endl;
	}

	virtual void Help()
	{
		cout << "Function ID " << FUNC_ID << " : This function will print hello World." << endl;
	}

	virtual int GetID(void)
	{
		return FUNC_ID;
	}
};

extern "C" __declspec(dllexport) void CreateObj(IPrintPlugin **ppPlugin)
{
	static CPrintPlugin plugin;
	*ppPlugin = &plugin;
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, void* lpReserved) 
{
	HANDLE g_hModule;
	switch(dwReason)
	{
	case DLL_PROCESS_ATTACH:
		g_hModule = (HINSTANCE)hModule;
		break;

	case DLL_PROCESS_DETACH:
		g_hModule=NULL;
		break;

	case DLL_THREAD_ATTACH:
		g_hModule = (HINSTANCE)hModule;
		break;

	case DLL_THREAD_DETACH:
		g_hModule = NULL;
		break;
	}
	return true;
}