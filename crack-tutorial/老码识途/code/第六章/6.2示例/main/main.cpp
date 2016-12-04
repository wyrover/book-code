
#include <windows.h> 
#include <stdio.h> 
#include "PluginEnumerator.h"

typedef void (__cdecl *PROC_PRINT)(void); 

int main(int argc, char **argv)
{
	std::vector<std::string> vstrPluginNames;

	CPluginEnumerator enumerator;
	if(!enumerator.GetPluginNames(vstrPluginNames))
		return 0;

	for(unsigned int i=0 ; i<vstrPluginNames.size(); i++)
	{
		PROC_PRINT DllProc; 

		HINSTANCE hinstLib = LoadLibrary(vstrPluginNames[i].c_str()); 

		if(hinstLib != NULL) 
		{ 
			DllProc = (PROC_PRINT)::GetProcAddress(hinstLib, "Print"); 

			if(NULL != DllProc) 
			{
				(DllProc)();
			}
			::FreeLibrary(hinstLib);
		}
	}

	getchar();
}
