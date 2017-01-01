#include "ExternalMonitor.h"

#include <iostream>
#include <string>

#include <Windows.h>
#include <HighLevelMonitorConfigurationAPI.h>

const std::wstring ExternalMonitor::GetLastErrorAsString()
{
	//Get the error message, if any.
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
		return std::wstring(); //No error message has been recorded

	LPWSTR messageBuffer = nullptr;
	size_t size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&messageBuffer, 0, NULL);

	std::wstring message(messageBuffer, size);

	//Free the buffer.
	LocalFree(messageBuffer);

	return message;
}


ExternalMonitor::ExternalMonitor(const std::wstring &monitor_name) :
	name(monitor_name)
{
	EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM) this);
}

ExternalMonitor::~ExternalMonitor()
{
	DestroyPhysicalMonitor(hPhysicalMonitor);
}

void ExternalMonitor::set_brightness(const int brightness_percent)
{
	DWORD dwNewBrightness = brightness_percent; // This assumes that 0 is min, 100 is max
	BOOL bSuccess;
	int retry_count = 10;
	do {
		bSuccess = SetMonitorBrightness(hPhysicalMonitor, dwNewBrightness);
		if (bSuccess == FALSE)
		{
			std::wcerr << "SetMonitorBrightness: " << GetLastErrorAsString() << std::endl;
		}
	} while (!bSuccess && --retry_count);
	std::wcout << "Set " << name << "'s brightness to " << brightness_percent << std::endl;
}

const int ExternalMonitor::get_brightness()
{
	DWORD pdwMinimumBrightness;
	DWORD pdwCurrentBrightness;
	DWORD pdwMaximumBrightness;
	BOOL bSuccess;
	int retry_count = 10;
	do {
		bSuccess = GetMonitorBrightness(hPhysicalMonitor, &pdwMinimumBrightness, &pdwCurrentBrightness, &pdwMaximumBrightness);
		if (bSuccess == FALSE)
		{
			std::wcerr << "SetMonitorBrightness: " << GetLastErrorAsString() << std::endl;
		}
	} while (!bSuccess && --retry_count);

	double normalized_brightness = 100 * (pdwCurrentBrightness - pdwMinimumBrightness) / (pdwMaximumBrightness - pdwMinimumBrightness);
	std::wcout << "Got " << name << "'s brightness as " << normalized_brightness << std::endl;

	return static_cast<int>(std::round(normalized_brightness));
}

BOOL CALLBACK ExternalMonitor::MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM pData)
{
	ExternalMonitor* pThis = reinterpret_cast<ExternalMonitor*>(pData);

	DWORD cPhysicalMonitors;
	LPPHYSICAL_MONITOR pPhysicalMonitors = NULL;

	BOOL bSuccess = GetNumberOfPhysicalMonitorsFromHMONITOR(hMonitor, &cPhysicalMonitors);
	if (!bSuccess)
	{
		std::wcerr << "GetNumberOfPhysicalMonitorsFromHMONITOR failed: " << GetLastErrorAsString() << std::endl;
		return false;
	}

	// Allocate the array of PHYSICAL_MONITOR structures.
	pPhysicalMonitors = (LPPHYSICAL_MONITOR)malloc( cPhysicalMonitors * sizeof(PHYSICAL_MONITOR));
	if (pPhysicalMonitors == NULL)
	{
		std::wcout << "Could not allocate PHYSICAL_MONITOR structure" << std::endl;
		return false;
	}

	// Get the array.
	bSuccess = GetPhysicalMonitorsFromHMONITOR(hMonitor, cPhysicalMonitors, pPhysicalMonitors);

	for (int i = 0; i < (int)cPhysicalMonitors; i++)
	{
		// Compare for monitor name
		if (pThis->name.compare(pPhysicalMonitors[i].szPhysicalMonitorDescription) != 0)
		{
			std::wcout << "Ignoring monitor \"" << pPhysicalMonitors[i].szPhysicalMonitorDescription << "\"" << std::endl;
			DestroyPhysicalMonitor(pPhysicalMonitors[i].hPhysicalMonitor);
			continue;
		}

		// Store physical monitor handle
		pThis->hPhysicalMonitor = pPhysicalMonitors[i].hPhysicalMonitor;

		return true;
	}
	return false;
}
