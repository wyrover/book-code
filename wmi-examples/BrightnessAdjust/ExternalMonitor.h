#pragma once

#include <string>
#include <Windows.h>

class ExternalMonitor
{
public:
	ExternalMonitor(const std::wstring &monitor_name);
	~ExternalMonitor();

	void set_brightness(const int brightness_percent);

	const int get_brightness();

private:
	ExternalMonitor();

	std::wstring name;
	HANDLE hPhysicalMonitor;

	static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM pData);
	static const std::wstring GetLastErrorAsString();
};

