#pragma once

#include <Windows.h>
#include <vector>

typedef void (__cdecl *PROC_PRINT)(void); 
typedef void (__cdecl *PROC_HELP)(void); 
typedef int (__cdecl *PROC_GETID)(void); 

class CPluginController
{
public:
	CPluginController(void);
	virtual ~CPluginController(void);
	
	bool InitializeController(void);
	bool UninitializeController(void);

	bool ProcessHelp(void);
	bool ProcessRequest(int FunctionID);

private:
	std::vector<HINSTANCE> m_vhForPlugin;
	std::vector<PROC_PRINT> m_vPrintFunc;
	std::vector<PROC_GETID> m_vGetIDFunc;
};
