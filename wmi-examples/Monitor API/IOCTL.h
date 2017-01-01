//ABANDONED See Issue#1
#pragma once

#include <vector>
#include <Windows.h>
#include <string>

class IOCTL
{
public:
	IOCTL(std::string sysDeviceName);

	~IOCTL();

	bool isValid();

	
protected:
	HANDLE handle;

	bool valid;

	std::vector<int> capabilities;
	
	bool getBrightnessCapabilities();
};