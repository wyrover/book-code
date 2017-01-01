#pragma once
#ifndef SCREENBRIGHTCONTROLLER_H
#define SCREENBRIGHTCONTROLLER_CPP

// Comment this line to disable debug output
//#define DEBUG

#include <string>
#include <Windows.h>
#include <Tchar.h>
#include <iostream>
#include <wbemidl.h> 
#include <comdef.h>
#include <sstream>
#include <iostream>

#define _WIN32_DCOM
#pragma comment(lib, "wbemuuid.lib")

class ScreenBrightController {
public:
	ScreenBrightController();
	~ScreenBrightController();
	
	void setBrightness(int value, int timeout = 0);

private:
	HRESULT hr;
	IWbemClassObject *pInInst;
	IWbemClassObject *pClass;
	IWbemClassObject *pInClass;
	IWbemLocator *pLocator;

	BSTR ArgName0;
	BSTR ArgName1;
	BSTR MethodName;
	BSTR path;
	BSTR ClassPath;
	BSTR bstrQuery;

	IWbemServices *pNamespace;
	IWbemClassObject *pObj;

	void cleanUp();
};

#endif