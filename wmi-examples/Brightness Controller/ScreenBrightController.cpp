#include "stdafx.h"
#include "ScreenBrightController.h"

// Comment the following line to disable debug output
#define DEBUG FALSE

#if(DEBUG == TRUE)
	#define debugOUT(s) std::cout << s << std::endl
#else
	#define debugOUT(s)
#endif

ScreenBrightController::ScreenBrightController(){
	pLocator = NULL;
	pNamespace = 0;
	pClass = NULL;
	pInClass = NULL;
	pInInst = NULL;
	IEnumWbemClassObject *pEnum = NULL;
	hr = S_OK;

	// Declaracion de nombre de métodos y argumentos
	path			= SysAllocString(L"root\\wmi");										// Namespace de la clase
	ClassPath		= SysAllocString(L"WmiMonitorBrightnessMethods");					// Nombre de la clase
	MethodName		= SysAllocString(L"WmiSetBrightness");								// Nombre del método
	ArgName0		= SysAllocString(L"Timeout");										// Primer argumento del método
	ArgName1		= SysAllocString(L"Brightness");									// Segundo argumento del método
	bstrQuery		= SysAllocString(L"Select * from WmiMonitorBrightnessMethods");		// ??
	if (!path || !ClassPath || !MethodName || !ArgName0){								// Compruebo por errores
		debugOUT("SysAllocString failed - Out of memory");
		cleanUp(); return;
	}

	// Initialize COM and connect up to CIMOM
	hr = CoInitialize(0);
	if (FAILED(hr))
	{
		debugOUT("CoInitialize returned: " << hr);
		cleanUp(); return;
	}

	//  NOTE:
	//  When using asynchronous WMI API's remotely in an environment where the "Local System" account 
	//  has no network identity (such as non-Kerberos domains), the authentication level of 
	//  RPC_C_AUTHN_LEVEL_NONE is needed. However, lowering the authentication level to 
	//  RPC_C_AUTHN_LEVEL_NONE makes your application less secure. It is wise to
	//  use semi-synchronous API's for accessing WMI data and events instead of the asynchronous ones.
	hr = CoInitializeSecurity(NULL, -1, NULL, NULL,
		RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		EOAC_SECURE_REFS, //change to EOAC_NONE if you change dwAuthnLevel to RPC_C_AUTHN_LEVEL_NONE
		NULL);
	if (FAILED(hr))
	{
		debugOUT("CoInitializeSecurity returned: " << hr);
		cleanUp(); return;
	}

	hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID *)&pLocator);
	if (FAILED(hr))
	{
		debugOUT("CoCreateInstance returned: " << hr);
		cleanUp(); return;
	}

	hr = pLocator->ConnectServer(path, NULL, NULL, NULL, 0, NULL, NULL, &pNamespace);
	debugOUT("ConnectServer returned: " << hr);
	if (hr != WBEM_S_NO_ERROR){
		cleanUp(); return;
	}

	hr = CoSetProxyBlanket(pNamespace,
		RPC_C_AUTHN_WINNT,
		RPC_C_AUTHZ_NONE,
		NULL,
		RPC_C_AUTHN_LEVEL_PKT,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		EOAC_NONE
		);
	if (hr != WBEM_S_NO_ERROR){
		cleanUp(); return;
	}

	hr = pNamespace->ExecQuery(_bstr_t(L"WQL"), //Query Language
		bstrQuery, //Query to Execute
		WBEM_FLAG_RETURN_IMMEDIATELY, //Make a semi-synchronous call
		NULL, //Context
		&pEnum //Enumeration Interface
		);
	if (hr != WBEM_S_NO_ERROR){
		cleanUp(); return;
	}

	/***************************************************************************************/
	/* ACCESO AL METODO Y PASAJE DE ARGUMENTOS											   */
	/***************************************************************************************/
	hr = WBEM_S_NO_ERROR;
	ULONG ulReturned;
	DWORD retVal = 0;

	//Get the Next Object from the collection 
	hr = pEnum->Next(	WBEM_INFINITE,			// Timeout
						1,						// No of objects requested
						&pObj,					// Returned Object 
						&ulReturned				// No of object returned
					);
	if (hr != WBEM_S_NO_ERROR){
		cleanUp(); return;
	}

	// Get the class object
	hr = pNamespace->GetObject(ClassPath, 0, NULL, &pClass, NULL);
	debugOUT("GetObject returned: " << hr);
	if (hr != WBEM_S_NO_ERROR){
		cleanUp(); return;
	}

	// Get the input argument and set the property
	hr = pClass->GetMethod(MethodName, 0, &pInClass, NULL);
	debugOUT("GetMethod returned: " << hr);
	if (hr != WBEM_S_NO_ERROR){
		cleanUp(); return;
	}

	hr = pInClass->SpawnInstance(0, &pInInst);
	debugOUT("SpawnInstance returned: " << hr);
	if (hr != WBEM_S_NO_ERROR){
		cleanUp(); return;
	}
}

ScreenBrightController::~ScreenBrightController(){
	cleanUp();
}

void ScreenBrightController::setBrightness(int value, int timeout){

	if (value < 0) value = 0;
	if (value > 255) value = 255;
	
	std::wstring a = std::to_wstring(value);
	const wchar_t* brightness = a.c_str();

	// Primer argumento del método WmiSetBrightness (TimeOut)
	VARIANT var1;
	VariantInit(&var1);
	V_VT(&var1) = VT_BSTR;
	V_BSTR(&var1) = SysAllocString(L"0");
	hr = pInInst->Put(ArgName0, 0, &var1, CIM_UINT32); //CIM_UINT64
	debugOUT("Put ArgName0 returned: " << hr);
	VariantClear(&var1);
	if (hr != WBEM_S_NO_ERROR){
		cleanUp(); return;
	}

	// Segundo argumento del método WmiSetBrightness (brightness)
	VARIANT var;
	VariantInit(&var);
	V_VT(&var) = VT_BSTR;
	V_BSTR(&var) = SysAllocString(brightness);
	hr = pInInst->Put(ArgName1, 0, &var, CIM_UINT8);
	VariantClear(&var);
	debugOUT("Put ArgName1 returned: " << hr);
	if (hr != WBEM_S_NO_ERROR){
		cleanUp(); return;
	}

	// Call the method
	VARIANT pathVariable;
	VariantInit(&pathVariable);
	hr = pObj->Get(_bstr_t(L"__PATH"), 0, &pathVariable, NULL, NULL);
	debugOUT("pObj Get returned: " << hr);
	if (hr != WBEM_S_NO_ERROR){
		cleanUp(); return;
	}

	hr = pNamespace->ExecMethod(pathVariable.bstrVal, MethodName, 0, NULL, pInInst, NULL, NULL);
	VariantClear(&pathVariable);
	debugOUT("ExecMethod returned: " << hr);
	if (hr != WBEM_S_NO_ERROR){
		cleanUp(); return;
	}
}

void ScreenBrightController::cleanUp(){
	SysFreeString(path);
	SysFreeString(ClassPath);
	SysFreeString(MethodName);
	SysFreeString(ArgName0);
	SysFreeString(ArgName1);
	SysFreeString(bstrQuery);
	if (pClass)
		pClass->Release();
	if (pInInst)
		pInInst->Release();
	if (pInClass)
		pInClass->Release();
	if (pLocator)
		pLocator->Release();
	if (pNamespace)
		pNamespace->Release();
	CoUninitialize();
}