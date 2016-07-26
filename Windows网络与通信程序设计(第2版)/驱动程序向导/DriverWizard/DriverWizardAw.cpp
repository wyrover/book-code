// DriverWizardaw.cpp : implementation file
//

#include "stdafx.h"
#include "DriverWizard.h"
#include "DriverWizardaw.h"

#ifdef _PSEUDO_DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// This is called immediately after the custom AppWizard is loaded.  Initialize
//  the state of the custom AppWizard here.
void CDriverWizardAppWiz::InitCustomAppWiz()
{
	// There are no steps in this custom AppWizard.
	SetNumberOfSteps(0);
}

// This is called just before the custom AppWizard is unloaded.
void CDriverWizardAppWiz::ExitCustomAppWiz()
{
	// TODO: Add code here to deallocate resources used by the custom AppWizard
}

// This is called when the user clicks "Create..." on the New Project dialog
CAppWizStepDlg* CDriverWizardAppWiz::Next(CAppWizStepDlg* pDlg)
{
	return NULL; 
}

void CDriverWizardAppWiz::CustomizeProject(IBuildProject* pProject)
{
	// TODO: Add code here to customize the project.  If you don't wish
	//  to customize project, you may remove this virtual override.
	
	// This is called immediately after the default Debug and Release
	//  configurations have been created for each platform.  You may customize
	//  existing configurations on this project by using the methods
	//  of IBuildProject and IConfiguration such as AddToolSettings,
	//  RemoveToolSettings, and AddCustomBuildStep. These are documented in
	//  the Developer Studio object model documentation.

	// WARNING!!  IBuildProject and all interfaces you can get from it are OLE
	//  COM interfaces.  You must be careful to release all new interfaces
	//  you acquire.  In accordance with the standard rules of COM, you must
	//  NOT release pProject, unless you explicitly AddRef it, since pProject
	//  is passed as an "in" parameter to this function.  See the documentation
	//  on CCustomAppWiz::CustomizeProject for more information.
	CComPtr<IConfigurations> pConfigs;
	HRESULT hr=pProject->get_Configurations(&pConfigs);
	if(FAILED(hr))
	{
		AfxMessageBox("An error occurred while obtaining the IConfigurations interface pointer");
		return;
	}
	CComPtr<IConfiguration> pConfig;
	CComVariant index;
	VARIANT dummy = {0};
	CComBSTR Name;
	CString text;
	CString output;

	long Count=0;
	pConfigs->get_Count(&Count);

	// Iterate through all the configurations of the project
	for(int i=1; i <= Count; i++)
	{
		index=i;
		hr=pConfigs->Item(index, &pConfig);
		if(FAILED(hr))
		{
			AfxMessageBox("An error occurred while obtaining the IConfiguration pointer");
			return;
		}
		pConfig->get_Name(&Name);
		text = Name;
		
		if (text.Find("Debug") == -1)
			output = "Release";
		else
			output = "Debug";

		text.Format("/out:\"%s/%s.exe\"",output,m_Dictionary["Root"]);
		pConfig->RemoveToolSettings(L"link.exe", text.AllocSysString(), dummy);
		text.Format("/out:\"%s/%s.sys\"",output,m_Dictionary["Root"]);
		pConfig->AddToolSettings(L"link.exe", text.AllocSysString(), dummy);
		
		pConfig->AddToolSettings(L"mfc", L"0", dummy);	//None : 0, Static Libraries : 1, Shared Dll : 2
		pConfig->RemoveToolSettings(L"link.exe", L"/subsystem:windows", dummy);
		pConfig->RemoveToolSettings(L"link.exe", L"/incremental:yes", dummy);

		//The NATIVE subsystem applies device drivers for Windows NT.
		pConfig->AddToolSettings(L"link.exe", L"/subsystem:native", dummy);
		pConfig->AddToolSettings(L"link.exe", L"/entry:\"DriverEntry\"", dummy);
		pConfig->AddToolSettings(L"link.exe", L"/driver", dummy);
		pConfig->AddToolSettings(L"link.exe", L"/incremental:no", dummy);

		// change the preprocessor definitions
		pConfig->RemoveToolSettings(L"cl.exe", L"/D \"_WINDOWS\"", dummy);
		pConfig->RemoveToolSettings(L"cl.exe", L"/D \"_MBCS\"", dummy);
		pConfig->RemoveToolSettings(L"cl.exe", L"/Od", dummy);
		pConfig->RemoveToolSettings(L"cl.exe", L"/ZI", dummy);	// Program Database for "Edit & Continue" can not be defined when /driver option is defined
		pConfig->RemoveToolSettings(L"cl.exe", L"/GZ", dummy);	//GZ initializes all local variables not explicitly initialized by the program. It fills all memory used by these variables with 0xCC
		pConfig->AddToolSettings(L"cl.exe", L"/Zi", dummy);	// Program Database
		pConfig->AddToolSettings(L"cl.exe", L"/Oi", dummy);	// 
		pConfig->AddToolSettings(L"cl.exe", L"/Gz", dummy);	// __stdcall calling convention
		pConfig->AddToolSettings(L"cl.exe", L"/D \"_X86_\"", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/D \"i386\"", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/D \"STD_CALL\"", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/D \"CONDITION_HANDLING\"", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/D \"WIN32_LEAN_AND_MEAN\"", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/D \"NT_UP\"", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/D \"SRVDBG\"", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/D \"DBG\"", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/D \"_IDWBUILD\"", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/D \"_WIN32_WINNT=0x0400\"", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/I \"$(ddkroot)\\inc\\ddk\"", dummy);
		pConfig->AddToolSettings(L"cl.exe", L"/I \"$(ddkroot)\\inc\"", dummy);

		// Change the libraries
		pConfig->RemoveToolSettings(L"link.exe", L"kernel32.lib", dummy);
		pConfig->RemoveToolSettings(L"link.exe", L"user32.lib", dummy);
		pConfig->RemoveToolSettings(L"link.exe", L"gdi32.lib", dummy);
		pConfig->RemoveToolSettings(L"link.exe", L"winspool.lib", dummy);
		pConfig->RemoveToolSettings(L"link.exe", L"comdlg32.lib", dummy);
		pConfig->RemoveToolSettings(L"link.exe", L"advapi32.lib", dummy);
		pConfig->RemoveToolSettings(L"link.exe", L"shell32.lib", dummy);
		pConfig->RemoveToolSettings(L"link.exe", L"ole32.lib", dummy);
		pConfig->RemoveToolSettings(L"link.exe", L"oleaut32.lib", dummy);
		pConfig->RemoveToolSettings(L"link.exe", L"uuid.lib", dummy);
		pConfig->RemoveToolSettings(L"link.exe", L"odbc32.lib", dummy);
		pConfig->RemoveToolSettings(L"link.exe", L"odbccp32.lib", dummy);
		pConfig->AddToolSettings(L"link.exe", L"ntoskrnl.lib", dummy);
		pConfig->AddToolSettings(L"link.exe", L"hal.lib", dummy);
//		pConfig->AddToolSettings(L"link.exe", L"int64.lib", dummy);

		if (output == "Debug")
		{
			pConfig->AddToolSettings(L"link.exe", L"/libpath:\"$(ddkroot)\\libchk\\i386\"", dummy);
			pConfig->AddToolSettings(L"link.exe", L"/libpath:\"$(ddkroot)\\lib\\i386\\checked\"", dummy);
		}
		else
		{
			pConfig->AddToolSettings(L"link.exe", L"/libpath:\"$(ddkroot)\\libfre\\i386\"", dummy);
			pConfig->AddToolSettings(L"link.exe", L"/libpath:\"$(ddkroot)\\lib\\i386\\free\"", dummy);
		}

		pConfig=NULL;
	}
	pConfigs=NULL;
}


// Here we define one instance of the CDriverWizardAppWiz class.  You can access
//  m_Dictionary and any other public members of this class through the
//  global DriverWizardaw.
CDriverWizardAppWiz DriverWizardaw;

