#include "PluginController.h"
#include "PluginEnumerator.h"

CPluginController::CPluginController(void)
{
}

CPluginController::~CPluginController(void)
{
}

bool CPluginController::InitializeController(void)
{
	std::vector<std::string> vstrPluginNames;

	CPluginEnumerator enumerator;
	if(!enumerator.GetPluginNames(vstrPluginNames))
		return false;

	for(unsigned int i=0 ; i<vstrPluginNames.size(); i++)
	{
		HINSTANCE hinstLib = LoadLibrary(vstrPluginNames[i].c_str()); 

		if(hinstLib != NULL) 
		{ 
			m_vhForPlugin.push_back(hinstLib);

			PROC_PRINT DllPrint = (PROC_PRINT)::GetProcAddress(hinstLib, "Print");
			PROC_GETID DllGetID = (PROC_GETID)::GetProcAddress(hinstLib, "GetID");
			if((NULL != DllPrint) && (NULL != DllGetID))
			{
				m_vPrintFunc.push_back(DllPrint);
				m_vGetIDFunc.push_back(DllGetID);
			}
		}
	}

	return true;
}

bool CPluginController::ProcessRequest(int FunctionID)
{
	for(unsigned int i = 0; i < m_vGetIDFunc.size(); i++)
	{
		if((m_vGetIDFunc[i])() == FunctionID)
		{
			(m_vPrintFunc[i])();
			break;
		}
	}

	return true;
}

bool CPluginController::ProcessHelp(void)
{
	std::vector<std::string> vstrPluginNames;

	CPluginEnumerator enumerator;
	if(!enumerator.GetPluginNames(vstrPluginNames))
		return false;

	for(unsigned int i=0 ; i<vstrPluginNames.size(); i++)
	{
		PROC_HELP DllProc; 

		HINSTANCE hinstLib = LoadLibrary(vstrPluginNames[i].c_str()); 

		if(hinstLib != NULL) 
		{ 
			DllProc = (PROC_HELP)::GetProcAddress(hinstLib, "Help"); 

			if(NULL != DllProc) 
			{
				(DllProc)();
			}

			::FreeLibrary(hinstLib);
		}
	}

	return true;
}

bool CPluginController::UninitializeController()
{
	for(unsigned int i = 0; i < m_vhForPlugin.size(); i++)
	{
		::FreeLibrary(m_vhForPlugin[i]);
	}

	return true;
}