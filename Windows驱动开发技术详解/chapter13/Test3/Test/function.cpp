#include <windows.h>
#include <stdio.h>
#include <winioctl.h>
#include "function.h"

HANDLE GetDeviceViaInterface( GUID* pGuid, DWORD instance)
{
	// Get handle to relevant device information set
	HDEVINFO info = SetupDiGetClassDevs(pGuid, NULL, NULL, DIGCF_PRESENT | DIGCF_INTERFACEDEVICE);
	if(info==INVALID_HANDLE_VALUE)
	{
		printf("No HDEVINFO available for this GUID\n");
		return NULL;
	}

	// Get interface data for the requested instance
	SP_INTERFACE_DEVICE_DATA ifdata;
	ifdata.cbSize = sizeof(ifdata);
	if(!SetupDiEnumDeviceInterfaces(info, NULL, pGuid, instance, &ifdata))
	{
		printf("No SP_INTERFACE_DEVICE_DATA available for this GUID instance\n");
		SetupDiDestroyDeviceInfoList(info);
		return NULL;
	}

	// Get size of symbolic link name
	DWORD ReqLen;
	SetupDiGetDeviceInterfaceDetail(info, &ifdata, NULL, 0, &ReqLen, NULL);
	PSP_INTERFACE_DEVICE_DETAIL_DATA ifDetail = (PSP_INTERFACE_DEVICE_DETAIL_DATA)(new char[ReqLen]);
	if( ifDetail==NULL)
	{
		SetupDiDestroyDeviceInfoList(info);
		return NULL;
	}

	// Get symbolic link name
	ifDetail->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);
	if( !SetupDiGetDeviceInterfaceDetail(info, &ifdata, ifDetail, ReqLen, NULL, NULL))
	{
		SetupDiDestroyDeviceInfoList(info);
		delete ifDetail;
		return NULL;
	}

	printf("Symbolic link is %s\n",ifDetail->DevicePath);
	// Open file
	HANDLE rv = CreateFile( ifDetail->DevicePath, 
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if( rv==INVALID_HANDLE_VALUE) rv = NULL;

	delete ifDetail;
	SetupDiDestroyDeviceInfoList(info);
	return rv;
}

DWORD ReadPort(HANDLE hDevice,DWORD port)
{
	DWORD dwOutput;
	DWORD dwRead;
	DeviceIoControl(hDevice, IOCTL_READ_PORT_ULONG, &port, 4, &dwOutput, 4, &dwRead, NULL);
	return dwOutput;
}
VOID WritePort(HANDLE hDevice,DWORD port,DWORD value)
{
	PVOID buffer[2];
	buffer[0] = (PVOID)port;
	buffer[1] = (PVOID)value;
	DWORD dwWrite;
	DeviceIoControl(hDevice, IOCTL_WRITE_PORT_ULONG, &port, 8, NULL, 0, &dwWrite, NULL);
}
VOID TestDriver(HANDLE hDevice)
{
	DWORD dwOutput;
	DeviceIoControl(hDevice, IOCTL_TEST, NULL, 0, NULL, 0, &dwOutput, NULL);
}

