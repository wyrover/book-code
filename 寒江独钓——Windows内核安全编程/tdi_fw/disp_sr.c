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
// $Id: disp_sr.c,v 1.1 2009/04/28 12:53:27 tanwen Exp $

/*
 * This file contains TDI_SEND and TDI_RECEIVE handlers
 */

#include <ntddk.h>
#include <tdikrnl.h>
#include "sock.h"

#include "dispatch.h"
#include "memtrack.h"
#include "obj_tbl.h"
#include "tdi_fw.h"

static NTSTATUS tdi_receive_complete(
	IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp, IN PVOID Context);

//----------------------------------------------------------------------------

/*
 * TDI_SEND handler
 */

int
tdi_send(PIRP irp, PIO_STACK_LOCATION irps, struct completion *completion)
{
	TDI_REQUEST_KERNEL_SEND *param = (TDI_REQUEST_KERNEL_SEND *)(&irps->Parameters);
	struct ot_entry *ote_conn;
	KIRQL irql;

	KdPrint(("[tdi_fw] tdi_send: connobj: 0x%x; SendLength: %u; SendFlags: 0x%x\n",
		irps->FileObject, param->SendLength, param->SendFlags));

	ote_conn = ot_find_fileobj(irps->FileObject, &irql);
	if (ote_conn != NULL) {
		ULONG bytes = param->SendLength;

		ote_conn->bytes_out += bytes;

		// traffic stats
		KeAcquireSpinLockAtDpcLevel(&g_traffic_guard);
		
		g_traffic[TRAFFIC_TOTAL_OUT] += bytes;
		
		if (ote_conn->log_disconnect)
			g_traffic[TRAFFIC_COUNTED_OUT] += bytes;
		
		KeReleaseSpinLockFromDpcLevel(&g_traffic_guard);

		KeReleaseSpinLock(&g_ot_hash_guard, irql);
	}

	// TODO: process TDI_SEND_AND_DISCONNECT flag (used by IIS for example)

	return FILTER_ALLOW;
}

//----------------------------------------------------------------------------

/*
 * TDI_RECEIVE handler
 */

int
tdi_receive(PIRP irp, PIO_STACK_LOCATION irps, struct completion *completion)
{
	TDI_REQUEST_KERNEL_RECEIVE *param = (TDI_REQUEST_KERNEL_RECEIVE *)(&irps->Parameters);

	KdPrint(("[tdi_fw] tdi_receive: connobj: 0x%x; ReceiveLength: %u; ReceiveFlags: 0x%x\n",
		irps->FileObject, param->ReceiveLength, param->ReceiveFlags));

	if (!(param->ReceiveFlags & TDI_RECEIVE_PEEK)) {
		completion->routine = tdi_receive_complete;
	}

	return FILTER_ALLOW;
}

NTSTATUS
tdi_receive_complete(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp, IN PVOID Context)
{
	PIO_STACK_LOCATION irps = IoGetCurrentIrpStackLocation(Irp);
	struct ot_entry *ote_conn;
	KIRQL irql;

	KdPrint(("[tdi_fw] tdi_receive_complete: connobj: 0x%x; status: 0x%x; received: %u\n",
		irps->FileObject, Irp->IoStatus.Status, Irp->IoStatus.Information));

	ote_conn = ot_find_fileobj(irps->FileObject, &irql);
	if (ote_conn != NULL) {
		ULONG bytes =  Irp->IoStatus.Information;

		ote_conn->bytes_in += bytes;

		// traffic stats
		KeAcquireSpinLockAtDpcLevel(&g_traffic_guard);
		
		g_traffic[TRAFFIC_TOTAL_IN] += bytes;
		
		if (ote_conn->log_disconnect)
			g_traffic[TRAFFIC_COUNTED_IN] += bytes;
		
		KeReleaseSpinLockFromDpcLevel(&g_traffic_guard);

		KeReleaseSpinLock(&g_ot_hash_guard, irql);
	}

	return tdi_generic_complete(DeviceObject, Irp, Context);
}
