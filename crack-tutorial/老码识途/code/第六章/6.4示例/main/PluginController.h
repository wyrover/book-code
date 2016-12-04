#pragma once

#include <Windows.h>
#include <vector>

class IPrintPlugin;

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
	std::vector<IPrintPlugin*> m_vpPlugin;
};
