// console_template.cpp : Defines the entry point for the console application.
//


#include "targetver.h"
#include "monitor.h"
#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <iostream>
#include <tracetool/tracetool.h>

#pragma comment(lib, "wbemuuid.lib")

using namespace cactus;

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
    _tsetlocale(LC_ALL, _T(""));
    std::wcout.imbue(std::locale(""));
   
	int width, height;
	GetCurrentDesktopRes(&width, &height);
	printf("W: %d, H: %d.\n", width, height);

	//ReadAndPrintLogPixels()
	//UpdateDisplay(1600 , 900, 100);

	ListDisplayDevices();

	std::vector<DisplayMode> modes;

	getDisplayModes(modes);

	for (int i = 0; i < modes.size(); ++i) {
		TRACE_SEND_FORMAT("width:%d", modes[i].width);
		TRACE_SEND_FORMAT("height:%d", modes[i].height);
		TRACE_SEND_A("");
	}

	/*SetDisplayResolution(1280, 800);

	::Sleep(5000);

	SetDisplayDefaults();*/

	EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, 0);

	MonitorList monitorlist;
	monitorlist.update();

	std::vector<Monitor>::iterator it = monitorlist.monList->begin();
	for (; it != monitorlist.monList->end(); it++)
	{
		std::wcout << L"[ " << it->getName().c_str() << L"]\n";
		std::cout << "宽度:" << it->width << " 高度:" << it->height << std::endl;
	}

	int oldBrightness = GetBrightness();
	std::cout << "显示器亮度:" << oldBrightness << std::endl;

	if (!SetBrightness(100)) {
		TRACE_SEND_A("SetBrightness failed.");
		SetBrightness(oldBrightness);
	}


	//{

	//
	//	DWORD dpi;

	//	if (getCurrentUserDPI(&dpi)) {
	//		std::cout << "DPI:" << dpi << std::endl;
	//	}

	//	if (setCurrentUserDPI(96)) {
	//		if (getCurrentUserDPI(&dpi)) {
	//			std::cout << "DPI:" << dpi << std::endl;
	//		}
	//	}
	//}

	/*DEVMODE oldMode;	

	if (!setDisplayMode(1280, 800, 32, &oldMode))
	{
		ChangeDisplaySettings(NULL, CDS_RESET);			
	} 
	else
	{
		::Sleep(5000);
		TRACE_SEND_A("restore old mode.");
		ChangeDisplaySettings(&oldMode, CDS_RESET);	
	}*/

	/*if (!ChangeDisplaySettings(NULL,CDS_TEST)) 
	{ 		
		ChangeDisplaySettings(NULL,CDS_RESET);		 
		ChangeDisplaySettings(&savedMode,CDS_RESET);	
	}
	else 
	{
		ChangeDisplaySettings(NULL,CDS_RESET);		
	}*/

    _tsystem(_T("pause"));
    return 0;
}


BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) 
{
	MONITORINFOEX mi;
	mi.cbSize = sizeof(mi);
	if (GetMonitorInfo(hMonitor, &mi)) {
		std::cout << "[ " << mi.szDevice << " ]\n";
		std::cout << "rcMonitor: (" << mi.rcMonitor.left << "," << mi.rcMonitor.top << ")-(" << mi.rcMonitor.right << "," << mi.rcMonitor.bottom << ")\n";
		std::cout << "rcWork:    (" << mi.rcWork.left << "," << mi.rcWork.top << ")-(" << mi.rcWork.right << "," << mi.rcWork.bottom << ")\n";

		if (mi.dwFlags & MONITORINFOF_PRIMARY) {
			std::cout << "This is the primary display monitor.\n";
		}

		std::cout << "\n";
	}
	return TRUE;
}