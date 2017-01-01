//ABANDONED See Issue#1
#include "IOCTL.h"

#include <ntddvdeo.h>
#include <iostream>

IOCTL::IOCTL(std::string sysDeviceName)
{
	valid = true;
	//sysDeviceName = "\\\\.\\Device\\0000004a";
	handle = CreateFileW(
		L"\\\\.\\LCD",
		FILE_ANY_ACCESS,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL
	);

	if (handle == INVALID_HANDLE_VALUE)
	{
		valid = false;
		std::cout << "Error creating IOCTL handle for " << sysDeviceName << ": " << GetLastError() << std::endl;

		//Maybe need this but could cause errors
		CloseHandle(handle);
	}


	getBrightnessCapabilities();
}

IOCTL::~IOCTL()
{
	if (handle != INVALID_HANDLE_VALUE && !valid)
	{
		if (CloseHandle(handle) == 0)
		{
			std::cout << "Error closing IOCTL handle: " << GetLastError() << std::endl;
		}
	}
}

bool IOCTL::isValid()
{
	return valid;
	
}

bool IOCTL::getBrightnessCapabilities()
{
	BYTE outBuffer[256];
	DWORD outBufferSize = 256;
	LPDWORD bytesReturned = new DWORD(0);

	BOOL controlSuccess = DeviceIoControl(
		handle,									 // handle to device
		IOCTL_VIDEO_QUERY_SUPPORTED_BRIGHTNESS,  // dwIoControlCode
		NULL,									 // lpInBuffer
		0,										 // nInBufferSize
		(LPVOID)outBuffer,						 // output buffer
		outBufferSize,					 // size of output buffer
		bytesReturned,				     // number of bytes returned
		NULL									 // OVERLAPPED structure
	);

	if (controlSuccess == 0)
	{
		std::cout << "Error querying supported brightness levels: " << GetLastError() << std::endl;
		valid = false;
		return false;
	}

	if (*bytesReturned <= 0)
	{
		//error here
		valid = false;
		return false;
	}

	//Loop through output buffer to get all the brightness levels
	for (unsigned int i = 0; i < *bytesReturned; i++)
	{
		capabilities.push_back((INT)outBuffer[i]);
	}

	return false;
}
