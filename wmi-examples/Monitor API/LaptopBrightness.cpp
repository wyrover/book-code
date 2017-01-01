#include "LaptopBrightness.h"

#define _WIN32_DCOM
#include <iostream>
#include <Windows.h>
#include <wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")


LaptopBrightness::LaptopBrightness()
{
	HRESULT hr;
	hr = CoInitializeEx(0, COINIT_MULTITHREADED);

	if (FAILED(hr))
	{
		std::cout << "Failed to initialize COM library. Error code = 0x"
			<< std::hex << hr << std::endl;
		return;
	}
	

	hr = CoInitializeSecurity(
		NULL,                        // Security descriptor    
		-1,                          // COM negotiates authentication service
		NULL,                        // Authentication services
		NULL,                        // Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication level for proxies
		RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation level for proxies
		NULL,                        // Authentication info
		EOAC_NONE,                   // Additional capabilities of the client or server
		NULL);                       // Reserved

	if (FAILED(hr))
	{
		std::cout << "Failed to initialize security. Error code = 0x"
			<< std::hex << hr << std::endl;
		CoUninitialize();
		return;
	}


	IWbemLocator *pLoc = 0;

	hr = CoCreateInstance(CLSID_WbemLocator, 0,
		CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *)&pLoc);

	if (FAILED(hr))
	{
		std::cout << "Failed to create IWbemLocator object. Err code = 0x"
			<< std::hex << hr << std::endl;
		CoUninitialize();
		return;
	}


	IWbemServices *pSvc = 0;

	// Connect to the root\default namespace with the current user.
	hr = pLoc->ConnectServer(
		BSTR(L"Root\\wmi"),  //namespace
		NULL,       // User name 
		NULL,       // User password
		0,         // Locale 
		NULL,     // Security flags
		0,         // Authority 
		0,        // Context object 
		&pSvc);   // IWbemServices proxy


	if (FAILED(hr))
	{
		std::cout << "Could not connect. Error code = 0x"
			<< std::hex << hr << std::endl;
		pLoc->Release();
		CoUninitialize();
		return;
	}

	std::cout << "Connected to WMI" << std::endl;


	// Set the proxy so that impersonation of the client occurs.
	hr = CoSetProxyBlanket(pSvc,
		RPC_C_AUTHN_WINNT,
		RPC_C_AUTHZ_NONE,
		NULL,
		RPC_C_AUTHN_LEVEL_CALL,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		EOAC_NONE
	);

	if (FAILED(hr))
	{
		std::cout << "Could not set proxy blanket. Error code = 0x"
			<< std::hex << hr << std::endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return;
	}

	BSTR ClassName = SysAllocString(L"WmiMonitorBrightnessMethods");
	BSTR MethodName = SysAllocString(L"WmiSetBrightness");
	BSTR bstrQuery = SysAllocString(L"Select * from WmiMonitorBrightnessMethods");

	IEnumWbemClassObject *pEnum = NULL;

	hr = pSvc->ExecQuery(BSTR(L"WQL"), //Query Language  
		bstrQuery, //Query to Execute  
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, //Make a semi-synchronous call  
		NULL, //Context  
		&pEnum /*Enumeration Interface*/);

	pSvc->Release();
	pLoc->Release();
	CoUninitialize;
}

LaptopBrightness::~LaptopBrightness()
{
	
}