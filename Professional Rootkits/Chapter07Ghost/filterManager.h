// Copyright Ric Vieler, 2006
// Support header for filterManager.c

#ifndef _FILTER_MANAGER_H_
#define _FILTER_MANAGER_H_

NTSTATUS insertFileFilter(PDRIVER_OBJECT pDriverObject,
	PDEVICE_OBJECT* ppOldDevice,
	PDEVICE_OBJECT* ppNewDevice,
	wchar_t* deviceName);

NTSTATUS insertNetworkFilter(PDRIVER_OBJECT pDriverObject,
	PDEVICE_OBJECT* ppOldDevice,
	PDEVICE_OBJECT* ppNewDevice,
	wchar_t* deviceName);

void removeFilter(PDEVICE_OBJECT* ppOldDevice,
	PDEVICE_OBJECT* ppNewDevice);

#endif

