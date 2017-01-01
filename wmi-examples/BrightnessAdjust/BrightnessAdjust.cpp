// BrightnessAdjust.cpp : Defines the entry point for the console application.
//

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#pragma comment(lib, "Dxva2.lib")

#include "Updater.h"


int main()
{

	Updater(L"LG ULTRA HD(DisplayPort)", 5);

	return 0;
}
