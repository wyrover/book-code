// Declarations for filter driver

// Copyright (C) 1999 by Walter Oney

// All rights reserved



#ifndef DRIVER_H

#define DRIVER_H 



#define DRIVERNAME "MyFileFilter(zhangfan!)"					// for use in messages



///////////////////////////////////////////////////////////////////////////////

// Device extension structure



typedef struct tagDEVICE_EXTENSION {

	PDEVICE_OBJECT DeviceObject;			// device object this extension belongs to

	PDEVICE_OBJECT LowerDeviceObject;		// next lower driver in same stack

	PDEVICE_OBJECT Pdo;						// the PDO

	IO_REMOVE_LOCK RemoveLock;

	} DEVICE_EXTENSION, *PDEVICE_EXTENSION;



///////////////////////////////////////////////////////////////////////////////

// Global functions



VOID RemoveDevice(IN PDEVICE_OBJECT fdo);

NTSTATUS CompleteRequest(IN PIRP Irp, IN NTSTATUS status, IN ULONG_PTR info);

NTSTATUS DispatchForSCSI(IN PDEVICE_OBJECT fido, IN PIRP Irp);

#endif // DRIVER_H

