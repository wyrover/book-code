/* Copyright (c) 2002-2005 Vladislav Goncharov.
 *
 * Redistribution and use in source forms, with and without modification,
 * are permitted provided that this entire comment appears intact.
 *
 * Redistribution in binary form may occur without any restrictions.
 *
 * This software is provided ``AS IS'' without any warranties of any kind.
 */
 
// -*- mode: C++; tab-width: 4; indent-tabs-mode: nil -*- (for GNU Emacs)
//
// $Id: tdi_fw.c,v 1.1 2009/04/28 12:53:28 tanwen Exp $

/*
 * TDI-based open source personal firewall (TdiFw)
 */

#include <ntddk.h>
#include <tdikrnl.h>
#include "sock.h"

#include "conn_state.h"
#include "dispatch.h"
#include "filter.h"
#include "memtrack.h"
#include "obj_tbl.h"
#include "tdi_fw.h"

#define IOCTL_TRANSFER_TYPE(ioctl)	((ioctl) & 3)

/* context for tdi_skip_complete */
typedef struct {
    PIO_COMPLETION_ROUTINE	old_cr;			/* old (original) completion routine */
    PVOID					old_context;	/* old (original) parameter for old_cr */
    PIO_COMPLETION_ROUTINE	new_cr;			/* new (replaced) completion routine */
	PVOID					new_context;	/* new (replaced) parameter for new_cr */
	PFILE_OBJECT			fileobj;		/* FileObject from IO_STACK_LOCATION */
	PDEVICE_OBJECT			new_devobj;		/* filter device object */
	UCHAR					old_control;	/* old (original) irps->Control */
} TDI_SKIP_CTX;

/* prototypes */

static NTSTATUS	DeviceDispatch(IN PDEVICE_OBJECT DeviceObject, IN PIRP irp);
static VOID		OnUnload(IN PDRIVER_OBJECT DriverObject);

#ifndef USE_TDI_HOOKING
static NTSTATUS	c_n_a_device(PDRIVER_OBJECT DriverObject, PDEVICE_OBJECT *fltobj,
							 PDEVICE_OBJECT *oldobj, wchar_t *devname);
static void		d_n_d_device(PDRIVER_OBJECT DriverObject, PDEVICE_OBJECT oldobj,
							 PDEVICE_OBJECT fltobj);
#else
static NTSTATUS	hook_tcpip(DRIVER_OBJECT *old_DriverObject, BOOLEAN b_hook);
static NTSTATUS	get_device_object(wchar_t *name, PDEVICE_OBJECT *devobj);
#endif

static NTSTATUS	tdi_skip_complete(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp, IN PVOID Context);

/* device objects for: */
PDEVICE_OBJECT
	g_tcpfltobj = NULL,		// \Device\Tcp
	g_udpfltobj = NULL,		// \Device\Udp
	g_ipfltobj = NULL,		// \Device\RawIp
	g_devcontrol = NULL,	// control device (exclusive access only!)
	g_devnfo = NULL;		// information device 

BOOLEAN g_got_log = FALSE;	// got log app

#ifndef USE_TDI_HOOKING
// original device objects
PDEVICE_OBJECT g_tcpoldobj, g_udpoldobj, g_ipoldobj;
#else
// original driver object
DRIVER_OBJECT g_old_DriverObject;
BOOLEAN g_hooked = FALSE;
#endif

/* for IOCTL_TDI_QUERY_DIRECT_SEND_HANDLER */
typedef NTSTATUS  TCPSendData_t(IN PIRP Irp, IN PIO_STACK_LOCATION IrpSp);
static TCPSendData_t *g_TCPSendData = NULL;
static TCPSendData_t new_TCPSendData;

/* global traffic stats */
unsigned __int64 g_traffic[TRAFFIC_MAX];
KSPIN_LOCK g_traffic_guard;

/* add by tan wen */
#define TDI_USER_DEV_MAX 5
PDEVICE_OBJECT g_user_devices[TDI_USER_DEV_MAX] = { NULL };

BOOLEAN tdifw_register_user_device(PDEVICE_OBJECT dev)
{
	int i;
	for(i=0;i<TDI_USER_DEV_MAX;++i)
	{
		if(g_user_devices[i] == NULL)
		{
			g_user_devices[i] = dev;
			return TRUE;
		}
	}
	return FALSE;
}

extern NTSTATUS
tdifw_driver_entry(
			IN PDRIVER_OBJECT theDriverObject,
            IN PUNICODE_STRING theRegistryPath);

/* initialization */
NTSTATUS
DriverEntry(IN PDRIVER_OBJECT theDriverObject,
            IN PUNICODE_STRING theRegistryPath)
{
    NTSTATUS status = STATUS_SUCCESS;
	int i;
	UNICODE_STRING name, linkname;

	if(status != STATUS_SUCCESS)
		goto done;

	memtrack_init();
	KeInitializeSpinLock(&g_traffic_guard);

#ifdef USE_TDI_HOOKING
	KdPrint(("[tdi_fw] WARNING! Using unstable working mode: TDI hooking!\n"));
#endif

	status = ot_init();
	if (status != STATUS_SUCCESS) {
		KdPrint(("[tdi_fw] DriverEntry: ot_init: 0x%x\n", status));
		goto done;
	}

	status = filter_init();
	if (status != STATUS_SUCCESS) {
		KdPrint(("[tdi_fw] DriverEntry: filter_init: 0x%x\n", status));
		goto done;
	}

	status = conn_state_init();
	if (status != STATUS_SUCCESS) {
		KdPrint(("[tdi_fw] DriverEntry: conn_state_init: 0x%x\n", status));
		goto done;
	}
	
	for (i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
		theDriverObject->MajorFunction[i] = DeviceDispatch;

#if DBG
	// register UnLoad procedure
	theDriverObject->DriverUnload = OnUnload;
#endif

	/* create control device and symbolic link */

	RtlInitUnicodeString(&name, L"\\Device\\tdifw");

	status = IoCreateDevice(theDriverObject,
							0,
							&name,
							0,
							0,
							TRUE,		// exclusive!
							&g_devcontrol);
	if (status != STATUS_SUCCESS) {
		KdPrint(("[tdi_fw] DriverEntry: IoCreateDevice(control): 0x%x!\n", status));
		goto done;
	}

	RtlInitUnicodeString(&linkname, L"\\??\\tdifw");

	status = IoCreateSymbolicLink(&linkname, &name);
	if (status != STATUS_SUCCESS) {
		KdPrint(("[tdi_fw] DriverEntry: IoCreateSymbolicLink: 0x%x!\n", status));
		goto done;
	}

	RtlInitUnicodeString(&name, L"\\Device\\tdifw_nfo");

	status = IoCreateDevice(theDriverObject,
							0,
							&name,
							0,
							0,
							FALSE,		// not exclusive!
							&g_devnfo);
	if (status != STATUS_SUCCESS) {
		KdPrint(("[tdi_fw] DriverEntry: IoCreateDevice(nfo): 0x%x!\n", status));
		goto done;
	}

	RtlInitUnicodeString(&linkname, L"\\??\\tdifw_nfo");

	status = IoCreateSymbolicLink(&linkname, &name);
	if (status != STATUS_SUCCESS) {
		KdPrint(("[tdi_fw] DriverEntry: IoCreateSymbolicLink: 0x%x!\n", status));
		goto done;
	}

#ifndef USE_TDI_HOOKING

	// Add by tanwen.
	// Call this function before hooking! So that when tdifw_filter() happened, 
	// Our driver has been initialized.
	status = tdifw_driver_entry(theDriverObject,theRegistryPath);

	status = c_n_a_device(theDriverObject, &g_tcpfltobj, &g_tcpoldobj, L"\\Device\\Tcp");
	if (status != STATUS_SUCCESS) {
		KdPrint(("[tdi_fw] DriverEntry: c_n_a_device: 0x%x\n", status));
		goto done;
	}

	status = c_n_a_device(theDriverObject, &g_udpfltobj, &g_udpoldobj, L"\\Device\\Udp");
	if (status != STATUS_SUCCESS) {
		KdPrint(("[tdi_fw] DriverEntry: c_n_a_device: 0x%x\n", status));
		goto done;
	}

	status = c_n_a_device(theDriverObject, &g_ipfltobj, &g_ipoldobj, L"\\Device\\RawIp");
	if (status != STATUS_SUCCESS) {
		KdPrint(("[tdi_fw] DriverEntry: c_n_a_device: 0x%x\n", status));
		goto done;
	}

#else	/* USE_TDI_HOOKING */

	// Add by tanwen.
	// Call this function before hooking! So that when tdifw_filter() happened, 
	// Our driver has been initialized.
	status = tdifw_driver_entry(theDriverObject,theRegistryPath);

	/* get device objects for tcp/udp/ip */

	status = get_device_object(L"\\Device\\Tcp", &g_tcpfltobj);
	if (status != STATUS_SUCCESS) {
		KdPrint(("[tdi_fw] DriverEntry: get_device_object(tcp): 0x%x\n", status));
		goto done;
	}
	
	status = get_device_object(L"\\Device\\Udp", &g_udpfltobj);
	if (status != STATUS_SUCCESS) {
		KdPrint(("[tdi_fw] DriverEntry: get_device_object(udp): 0x%x\n", status));
		goto done;
	}
	
	status = get_device_object(L"\\Device\\RawIp", &g_ipfltobj);
	if (status != STATUS_SUCCESS) {
		KdPrint(("[tdi_fw] DriverEntry: get_device_object(ip): 0x%x\n", status));
		goto done;
	}

	/* hook tcpip */

	status = hook_tcpip(&g_old_DriverObject, TRUE);
	if (status != STATUS_SUCCESS) {
		KdPrint(("[tdi_fw] DriverEntry: hook_driver: 0x%x\n", status));
		goto done;
	}
	g_hooked = TRUE;

#endif	/* USE_TDI_HOOKING */

	status = STATUS_SUCCESS;

done:
	if (status != STATUS_SUCCESS) {
		// cleanup
		OnUnload(theDriverObject);
	}

    return status;
}

extern VOID
tdifw_driver_unload(
			IN PDRIVER_OBJECT DriverObject);

/* deinitialization */
VOID
OnUnload(IN PDRIVER_OBJECT DriverObject)
{
	// Add by tan wen. 
	tdifw_driver_unload(DriverObject);

#ifndef USE_TDI_HOOKING
	d_n_d_device(DriverObject, g_tcpoldobj, g_tcpfltobj);
	d_n_d_device(DriverObject, g_udpoldobj, g_udpfltobj);
	d_n_d_device(DriverObject, g_ipoldobj, g_ipfltobj);
#else
	if (g_hooked)
		hook_tcpip(&g_old_DriverObject, FALSE);  
#endif

	// delete control device and symbolic link
	if (g_devcontrol != NULL) {
		UNICODE_STRING linkname;
		
		RtlInitUnicodeString(&linkname, L"\\??\\tdifw");
		IoDeleteSymbolicLink(&linkname);

		IoDeleteDevice(g_devcontrol);
	}

	// delete info device and symbolic link
	if (g_devnfo != NULL) {
		UNICODE_STRING linkname;
		
		RtlInitUnicodeString(&linkname, L"\\??\\tdifw_nfo");
		IoDeleteSymbolicLink(&linkname);

		IoDeleteDevice(g_devnfo);
	}

	filter_free();
	ot_free();
	conn_state_free();		// call after ot_free()

	memtrack_free();

}

#ifndef USE_TDI_HOOKING

/* create & attach device */
NTSTATUS
c_n_a_device(PDRIVER_OBJECT DriverObject, PDEVICE_OBJECT *fltobj, PDEVICE_OBJECT *oldobj,
			 wchar_t *devname)
{
	NTSTATUS status;
	UNICODE_STRING str;

	/* create filter device */

	status = IoCreateDevice(DriverObject,
							0,
							NULL,
							FILE_DEVICE_UNKNOWN,
							0,
							TRUE,
							fltobj);
	if (status != STATUS_SUCCESS) {
		KdPrint(("[tdi_fw] c_n_a_device: IoCreateDevice(%S): 0x%x\n", devname, status));
		return status;
	}

	(*fltobj)->Flags |= DO_DIRECT_IO;

	RtlInitUnicodeString(&str, devname);
	
	status = IoAttachDevice(*fltobj, &str, oldobj);
	if (status != STATUS_SUCCESS) {
		KdPrint(("[tdi_fw] DriverEntry: IoAttachDevice(%S): 0x%x\n", devname, status));
		return status;
	}

	KdPrint(("[tdi_fw] DriverEntry: %S fileobj: 0x%x\n", devname, *fltobj));

	return STATUS_SUCCESS;
}

/* detach & delete device */
void
d_n_d_device(PDRIVER_OBJECT DriverObject, PDEVICE_OBJECT oldobj, PDEVICE_OBJECT fltobj)
{
	/*
	 * Detaching of a filter driver at runtime is a high-risk deal
	 */

#if 1
	// for extremal guys only!
	if (oldobj != NULL && fltobj != NULL) {
		int i;
		for (i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
			DriverObject->MajorFunction[i] = g_tcpoldobj->DriverObject->MajorFunction[i];
	}
#endif

	if (oldobj != NULL)
		IoDetachDevice(oldobj);

	if (fltobj != NULL)
		IoDeleteDevice(fltobj);
}

#else	/* USE_TDI_HOOKING */

/* hook/unhook driver */
NTSTATUS
hook_tcpip(DRIVER_OBJECT *old_DriverObject, BOOLEAN b_hook)
{
	UNICODE_STRING drv_name;
	NTSTATUS status;
	PDRIVER_OBJECT new_DriverObject;
	int i;

	RtlInitUnicodeString(&drv_name, L"\\Driver\\Tcpip");

	status = ObReferenceObjectByName(&drv_name, OBJ_CASE_INSENSITIVE, NULL, 0,
		IoDriverObjectType, KernelMode, NULL, &new_DriverObject);
	if (status != STATUS_SUCCESS) {
		KdPrint(("[tdi_fw] hook_driver: ObReferenceObjectByName\n"));
		return status;
	}

	for (i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++) {
		if (b_hook) {
			old_DriverObject->MajorFunction[i] = new_DriverObject->MajorFunction[i];
			new_DriverObject->MajorFunction[i] = DeviceDispatch;
		} else
			new_DriverObject->MajorFunction[i] = old_DriverObject->MajorFunction[i];
	}
	
	return STATUS_SUCCESS;	
}

/* get device object by its name */
NTSTATUS
get_device_object(wchar_t *name, PDEVICE_OBJECT *devobj)
{
	UNICODE_STRING str;
	NTSTATUS status;
	PFILE_OBJECT fileobj;

	RtlInitUnicodeString(&str, name);

	status = IoGetDeviceObjectPointer(&str, FILE_ALL_ACCESS, &fileobj, devobj);
	if (status == STATUS_SUCCESS)
		ObDereferenceObject(fileobj);

	return status;
}

#endif	/* USE_TDI_HOOKING */

extern 
NTSTATUS tdifw_user_device_dispatch(IN PDEVICE_OBJECT DeviceObject, IN PIRP irp);

/* dispatch */
NTSTATUS
DeviceDispatch(IN PDEVICE_OBJECT DeviceObject, IN PIRP irp)
{
	PIO_STACK_LOCATION irps;
	NTSTATUS status;
	int i;				// add by tanwen

	// sanity check
	if (irp == NULL) {
		KdPrint(("[tdi_fw] DeviceDispatch: !irp\n"));
		return STATUS_SUCCESS;
	}
	
	// add by tanwen
	for(i=0;i<TDI_USER_DEV_MAX;++i)
	{
		if(g_user_devices[i] == DeviceObject)
			return tdifw_user_device_dispatch(DeviceObject,irp);
	}

	irps = IoGetCurrentIrpStackLocation(irp);

	if (DeviceObject == g_tcpfltobj || DeviceObject == g_udpfltobj ||
		DeviceObject == g_ipfltobj) {

		/*
		 * This IRP is for filtered device
		 */

		int result;
		struct completion completion;

		memset(&completion, 0, sizeof(completion));

		// Analyze MajorFunction
		switch (irps->MajorFunction) {

		case IRP_MJ_CREATE:		/* create fileobject */
			
			result = tdi_create(irp, irps, &completion);

			status = tdi_dispatch_complete(DeviceObject, irp, result,
				completion.routine, completion.context);
			
			break;

		case IRP_MJ_DEVICE_CONTROL:
			
			KdPrint(("[tdi_fw] DeviceDispatch: IRP_MJ_DEVICE_CONTROL, control 0x%x for 0x%08X\n",
				irps->Parameters.DeviceIoControl.IoControlCode, irps->FileObject));

			if (KeGetCurrentIrql() == PASSIVE_LEVEL) {
				/*
				 * try to convert it to IRP_MJ_INTERNAL_DEVICE_CONTROL
				 * (works on PASSIVE_LEVEL only!)
				 */
				status = TdiMapUserRequest(DeviceObject, irp, irps);
			} else
				status = STATUS_NOT_IMPLEMENTED; // set fake status

			if (status != STATUS_SUCCESS) {
				void *buf = (irps->Parameters.DeviceIoControl.IoControlCode == IOCTL_TDI_QUERY_DIRECT_SEND_HANDLER) ?
					irps->Parameters.DeviceIoControl.Type3InputBuffer : NULL;

				// send IRP to original driver
				status = tdi_dispatch_complete(DeviceObject, irp, FILTER_ALLOW, NULL, NULL);

				if (buf != NULL && status == STATUS_SUCCESS) {

					g_TCPSendData = *(TCPSendData_t **)buf;

					KdPrint(("[tdi_fw] DeviceDispatch: IOCTL_TDI_QUERY_DIRECT_SEND_HANDLER: TCPSendData = 0x%x\n",
						g_TCPSendData));

					*(TCPSendData_t **)buf = new_TCPSendData;
				}

				break;
			}

			// don't break! go to internal device control!
		
		case IRP_MJ_INTERNAL_DEVICE_CONTROL: {
			/*
			 * Analyze ioctl for TDI driver
			 */
			int i;

			for (i = 0; g_tdi_ioctls[i].MinorFunction != 0; i++)
				if (g_tdi_ioctls[i].MinorFunction == irps->MinorFunction) {
					
#if DBG
					// print description
					KdPrint(("[tdi_fw] DeviceDispatch: %s (0x%x) for 0x%x\n",
						g_tdi_ioctls[i].desc,
						irps->MinorFunction,
						irps->FileObject));
#endif

					if (g_tdi_ioctls[i].fn == NULL) {
						// send IRP to original driver
						status = tdi_dispatch_complete(DeviceObject, irp, FILTER_ALLOW,
							NULL, NULL);
						break;
					}

					// call dispatch function

					result = g_tdi_ioctls[i].fn(irp, irps, &completion);

					// complete request
					status = tdi_dispatch_complete(DeviceObject, irp, result,
						completion.routine, completion.context);

					break;
				}
	
			// if dispatch function hasn't been found
			if (g_tdi_ioctls[i].MinorFunction == 0) {
				// send IRP to original driver
				status = tdi_dispatch_complete(DeviceObject, irp, FILTER_ALLOW, NULL, NULL);
			}

			break;
		}

		case IRP_MJ_CLEANUP:		/* cleanup fileobject */

			result = tdi_cleanup(irp, irps, &completion);

			status = tdi_dispatch_complete(DeviceObject, irp, result,
				completion.routine, completion.context);
			break;

		case IRP_MJ_CLOSE:
			KdPrint(("[tdi_fw] DeviceDispatch: IRP_MJ_CLOSE fileobj 0x%x\n", irps->FileObject));

			// passthrough IRP
			status = tdi_dispatch_complete(DeviceObject, irp, FILTER_ALLOW,
				completion.routine, completion.context);

			break;

		default:
			KdPrint(("[tdi_fw] DeviceDispatch: major 0x%x, minor 0x%x for 0x%x\n",
				irps->MajorFunction, irps->MinorFunction, irps->FileObject));

			// passthrough IRP
			status = tdi_dispatch_complete(DeviceObject, irp, FILTER_ALLOW,
				completion.routine, completion.context);
		}

	} else if (DeviceObject == g_devcontrol) {

		/*
		 * this IRP is for control device
		 */

		// set default status
		status = STATUS_SUCCESS;

		if (irps->MajorFunction == IRP_MJ_CREATE) {

			// initialize for user-mode part (exclusive access - 1 user-mode logging part)
			filter_init_2();

			g_got_log = TRUE;

		} else if (irps->MajorFunction == IRP_MJ_CLOSE) {

			// cleanup for user-mode logging part
			filter_free_2();

			g_got_log = FALSE;

		} if (irps->MajorFunction == IRP_MJ_DEVICE_CONTROL) {

			/*
			 * control request
			 */

			ULONG ioctl = irps->Parameters.DeviceIoControl.IoControlCode,
				len = irps->Parameters.DeviceIoControl.InputBufferLength,
				size = irps->Parameters.DeviceIoControl.OutputBufferLength;
			char *out_buf;

			if (IOCTL_TRANSFER_TYPE(ioctl) == METHOD_NEITHER) {
				// this type of transfer unsupported
				out_buf = NULL;
			} else
				out_buf = (char *)irp->AssociatedIrp.SystemBuffer;

			// process control request
			status = process_request(ioctl, out_buf, &len, size);

			irp->IoStatus.Information = len;

		}

		irp->IoStatus.Status = status;

		IoCompleteRequest(irp, IO_NO_INCREMENT);

	} else if (DeviceObject == g_devnfo) {

		/*
		 * this IRP is for information device
		 */

		// set default status
		status = STATUS_SUCCESS;

		if (irps->MajorFunction == IRP_MJ_DEVICE_CONTROL) {

			/*
			 * control request
			 */

			ULONG ioctl = irps->Parameters.DeviceIoControl.IoControlCode,
				len = irps->Parameters.DeviceIoControl.InputBufferLength,
				size = irps->Parameters.DeviceIoControl.OutputBufferLength;
			char *out_buf;

			if (IOCTL_TRANSFER_TYPE(ioctl) == METHOD_NEITHER) {
				// this type of transfer unsupported
				out_buf = NULL;
			} else
				out_buf = (char *)irp->AssociatedIrp.SystemBuffer;

			// process control request
			status = process_nfo_request(ioctl, out_buf, &len, size);

			irp->IoStatus.Information = len;

		}

		irp->IoStatus.Status = status;

		IoCompleteRequest(irp, IO_NO_INCREMENT);

	} else {

		KdPrint(("[tdi_fw] DeviceDispatch: ioctl for unknown DeviceObject 0x%x\n", DeviceObject));

#ifndef USE_TDI_HOOKING
		// ??? just complete IRP
		status = irp->IoStatus.Status = STATUS_SUCCESS;
		IoCompleteRequest(irp, IO_NO_INCREMENT);
#else
		// call original handler
		status = g_old_DriverObject.MajorFunction[irps->MajorFunction](
			DeviceObject, irp);
#endif
	}

	return status;
}

/*
 * Dispatch routines call this function to complete their processing.
 * They _MUST_ call this function anyway.
 */
NTSTATUS
tdi_dispatch_complete(PDEVICE_OBJECT devobj, PIRP irp, int filter,
					  PIO_COMPLETION_ROUTINE cr, PVOID context)
{
	PIO_STACK_LOCATION irps = IoGetCurrentIrpStackLocation(irp);
	NTSTATUS status;

	if (filter == FILTER_DENY) {
		
		/*
		 * DENY: complete request with status "Access violation"
		 */

		KdPrint(("[tdi_fw] tdi_dispatch_complete: [DROP!]"
			" major 0x%x, minor 0x%x for devobj 0x%x; fileobj 0x%x\n",
			irps->MajorFunction,
			irps->MinorFunction,
			devobj,
			irps->FileObject));

		if (irp->IoStatus.Status == STATUS_SUCCESS) {
			// change status
			status = irp->IoStatus.Status = STATUS_ACCESS_DENIED;
		} else {
			// set IRP status unchanged
			status = irp->IoStatus.Status;
		}

		IoCompleteRequest (irp, IO_NO_INCREMENT);
		
	} else if (filter == FILTER_ALLOW) {

		/*
		 * ALLOW: pass IRP to the next driver
		 */

#ifndef USE_TDI_HOOKING

		PDEVICE_OBJECT old_devobj = get_original_devobj(devobj, NULL);

		if (old_devobj == NULL) {
			KdPrint(("[tdi_fw] tdi_send_irp_to_old_driver: Unknown DeviceObject 0x%x!\n", devobj));
	
			status = irp->IoStatus.Status = STATUS_UNSUCCESSFUL;
			IoCompleteRequest (irp, IO_NO_INCREMENT);
			
			return status;
		}

#endif

		KdPrint(("[tdi_fw] tdi_dispatch_complete: [ALLOW.]"
			" major 0x%x, minor 0x%x for devobj 0x%x; fileobj 0x%x\n",
			irps->MajorFunction,
			irps->MinorFunction,
			devobj,
			irps->FileObject));

#ifndef USE_TDI_HOOKING

		if (cr == NULL || irp->CurrentLocation <= 1) {
			/*
			 * we use _THIS_ way of sending IRP to old driver
			 * a) to avoid NO_MORE_STACK_LOCATIONS
			 * b) and if we haven't our completions - no need to copy stack locations!
			 */

			// stay on this location after IoCallDriver
			IoSkipCurrentIrpStackLocation(irp);

#endif

			if (cr != NULL) {
				/*
				 * set completion routine (this way is slow)
				 */

				// save old completion routine and context
				TDI_SKIP_CTX *ctx = (TDI_SKIP_CTX *)malloc_np(sizeof(*ctx));
				if (ctx == NULL) {
					KdPrint(("[tdi_fw] tdi_send_irp_to_old_driver: malloc_np\n"));
					
					status = irp->IoStatus.Status = STATUS_INSUFFICIENT_RESOURCES;
					IoCompleteRequest(irp, IO_NO_INCREMENT);
					
					return status;
				}

				ctx->old_cr = irps->CompletionRoutine;
				ctx->old_context = irps->Context;
				ctx->new_cr = cr;
				ctx->new_context = context;
				ctx->fileobj = irps->FileObject;
				ctx->new_devobj = devobj;

				ctx->old_control = irps->Control;

				IoSetCompletionRoutine(irp, tdi_skip_complete, ctx, TRUE, TRUE, TRUE);
			}

#ifndef USE_TDI_HOOKING			
		} else {
			PIO_STACK_LOCATION irps = IoGetCurrentIrpStackLocation(irp),
				next_irps = IoGetNextIrpStackLocation(irp);
			
			memcpy(next_irps, irps, sizeof(*irps));

			if (cr != NULL) {
				/*
				 * this way for completion is more quicker than used above
				 */

				IoSetCompletionRoutine(irp, cr, context, TRUE, TRUE, TRUE);
			} else
				IoSetCompletionRoutine(irp, tdi_generic_complete, NULL, TRUE, TRUE, TRUE);
		}
#endif

		/* call original driver */

#ifndef USE_TDI_HOOKING
		status = IoCallDriver(old_devobj, irp);
#else
		status = g_old_DriverObject.MajorFunction[irps->MajorFunction](devobj, irp);
#endif

	} else {	/* FILTER_UNKNOWN */

		/*
		 * UNKNOWN: just complete the request
		 */

		status = irp->IoStatus.Status = STATUS_SUCCESS;	// ???
		IoCompleteRequest (irp, IO_NO_INCREMENT);
	}

	return status;
}

/*
 * completion routine for case if we use IoSkipCurrentIrpStackLocation way
 * or we USE_TDI_HOOKING
 */
NTSTATUS
tdi_skip_complete(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp, IN PVOID Context)
{
	TDI_SKIP_CTX *ctx = (TDI_SKIP_CTX *)Context;
	NTSTATUS status;
	PIO_STACK_LOCATION irps;

	if (Irp->IoStatus.Status != STATUS_SUCCESS)
		KdPrint(("[tdi_fw] tdi_skip_complete: status 0x%x\n", Irp->IoStatus.Status));

	// restore IRP for using in our completion

	Irp->CurrentLocation--;
	Irp->Tail.Overlay.CurrentStackLocation--;

	irps = IoGetCurrentIrpStackLocation(Irp);

	KdPrint(("[tdi_fw] tdi_skip_complete: DeviceObject = 0x%x; FileObject = 0x%x\n",
		DeviceObject, irps->FileObject));

	DeviceObject = irps->DeviceObject;

	if (ctx->new_cr != NULL) {
		// restore fileobject (it's NULL)
		irps->FileObject = ctx->fileobj;
		// set new device object in irps
		irps->DeviceObject = ctx->new_devobj;
		
		// call new completion 
		status = ctx->new_cr(ctx->new_devobj, Irp, ctx->new_context);

	} else
		status = STATUS_SUCCESS;

	/* patch IRP back */

	// restore routine and context (and even control!)
	irps->CompletionRoutine = ctx->old_cr;
	irps->Context = ctx->old_context;
	irps->Control = ctx->old_control;

	// restore device object
	irps->DeviceObject = DeviceObject;

	Irp->CurrentLocation++;
	Irp->Tail.Overlay.CurrentStackLocation++;

	if (ctx->old_cr != NULL) {

		if (status != STATUS_MORE_PROCESSING_REQUIRED) {
			// call old completion (see the old control)
			BOOLEAN b_call = FALSE;

			if (Irp->Cancel) {
				// cancel
				if (ctx->old_control & SL_INVOKE_ON_CANCEL)
					b_call = TRUE;
			} else {
				if (Irp->IoStatus.Status >= STATUS_SUCCESS) {
					// success
					if (ctx->old_control & SL_INVOKE_ON_SUCCESS)
						b_call = TRUE;
				} else {
					// error
					if (ctx->old_control & SL_INVOKE_ON_ERROR)
						b_call = TRUE;
				}
			}

			if (b_call)
				status = ctx->old_cr(DeviceObject, Irp, ctx->old_context);
		
		} else {

			/*
			 * patch IRP to set IoManager to call completion next time
			 */

			// restore Control
			irps->Control = ctx->old_control;

		}
	}

	free(ctx);

	return status;
}


/* get original device object by filtered */
PDEVICE_OBJECT
get_original_devobj(PDEVICE_OBJECT flt_devobj, int *proto)
{
#ifndef USE_TDI_HOOKING
	PDEVICE_OBJECT result;
	int ipproto;

	if (flt_devobj == g_tcpfltobj) {
		result = g_tcpoldobj;
		ipproto = IPPROTO_TCP;
	} else if (flt_devobj == g_udpfltobj) {
		result = g_udpoldobj;
		ipproto = IPPROTO_UDP;
	} else if (flt_devobj == g_ipfltobj) {
		result = g_ipoldobj;
		ipproto = IPPROTO_IP;
	} else {
		KdPrint(("[tdi_fw] get_original_devobj: Unknown DeviceObject 0x%x!\n",
			flt_devobj));
		ipproto = IPPROTO_IP;		// what else?
		result = NULL;
	}

	if (result != NULL && proto != NULL)
		*proto = ipproto;

	return result;

#else	/* USE_TDI_HOOKING */

	// just stub for original devobj; return proto by devobj
	int ipproto;

	if (flt_devobj == g_tcpfltobj)
		ipproto = IPPROTO_TCP;
	else if (flt_devobj == g_udpfltobj)
		ipproto = IPPROTO_UDP;
	else if (flt_devobj == g_ipfltobj)
		ipproto = IPPROTO_IP;
	else {
		KdPrint(("[tdi_fw] get_original_devobj: Unknown DeviceObject 0x%x!\n",
			flt_devobj));
		ipproto = IPPROTO_IP;		// what else?
		flt_devobj = NULL;
	}

	if (proto != NULL)
		*proto = ipproto;

	return flt_devobj;

#endif
}

/*
 * Completion routines must call this function at the end of their execution
 */
NTSTATUS
tdi_generic_complete(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp, IN PVOID Context)
{
	KdPrint(("[tdi_fw] tdi_generic_complete: STATUS = 0x%x\n", Irp->IoStatus.Status));

	if (Irp->PendingReturned) {
		KdPrint(("[tdi_fw] tdi_generic_complete: PENDING\n"));
		IoMarkIrpPending(Irp);
	}

	return STATUS_SUCCESS;
}

/*
 * for IOCTL_TDI_QUERY_DIRECT_SEND_HANDLER
 */
NTSTATUS
new_TCPSendData(IN PIRP Irp, IN PIO_STACK_LOCATION IrpSp)
{
	struct completion completion;
	int result;

	KdPrint(("[tdi_fw] new_TCPSendData\n"));

#if 1
	memset(&completion, 0, sizeof(completion));

	result = tdi_send(Irp, IrpSp, &completion);

	// complete request
	return tdi_dispatch_complete(IrpSp->DeviceObject, Irp, result,
		completion.routine, completion.context);
#else
	return g_TCPSendData(Irp, IrpSp);
#endif
}

/*
 * deny stub for dispatch table
 */
int
tdi_deny_stub(PIRP irp, PIO_STACK_LOCATION irps, struct completion *completion)
{
	KdPrint(("[tdi_fw] tdi_deny_stub!\n"));
	return FILTER_DENY;
}
