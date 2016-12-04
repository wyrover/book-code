#include "PluginController.h"
#include "PluginEnumerator.h"
#include "IPrintPlugin.h"

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
		typedef int (__cdecl *PLUGIN_CREATE)(IPrintPlugin**);
		PLUGIN_CREATE CreateProc; 

		IPrintPlugin *pPlugin = NULL;

		HINSTANCE hinstLib = LoadLibrary(vstrPluginNames[i].c_str()); 

		if(hinstLib != NULL) 
		{ 
			m_vhForPlugin.push_back(hinstLib);

			CreateProc = (PLUGIN_CREATE)::GetProcAddress(hinstLib, "CreateObj"); 

			if(NULL != CreateProc) 
			{
				(CreateProc)(&pPlugin);

				if(pPlugin != NULL)
				{
					m_vpPlugin.push_back(pPlugin);
				}
			}
		}
	}

	return true;
}

bool CPluginController::ProcessRequest(int FunctionID)
{
	for(unsigned int i = 0; i < m_vpPlugin.size(); i++)
	{
		if(m_vpPlugin[i]->GetID() == FunctionID)
		{
			m_vpPlugin[i]->Print();
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
		typedef int (__cdecl *PLUGIN_CREATE)(IPrintPlugin**);
		PLUGIN_CREATE CreateProc; 

		IPrintPlugin *pPlugin = NULL;

		HINSTANCE hinstLib = LoadLibrary(vstrPluginNames[i].c_str()); 

		if(hinstLib != NULL) 
		{ 
			CreateProc = (PLUGIN_CREATE)::GetProcAddress(hinstLib, "CreateObj"); 

			if(NULL != CreateProc) 
			{
				(CreateProc)(&pPlugin);

				if(pPlugin != NULL)
				{
					pPlugin->Help();
				}
			}

			FreeLibrary(hinstLib);
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