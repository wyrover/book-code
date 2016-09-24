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
// $Id: ev_recv.c,v 1.1 2009/04/28 12:53:27 tanwen Exp $

/*
 * This file contains TDI_EVENT_RECEIVE and TDI_EVENT_CHAINED_RECEIVE handlers
 */

#include <ntddk.h>
#include <tdikrnl.h>
#include "sock.h"

#include "events.h"
#include "memtrack.h"
#include "obj_tbl.h"
#include "tdi_fw.h"

struct tdi_client_irp_ctx {
    PIO_COMPLETION_ROUTINE	completion;
    PVOID					context;
	UCHAR					old_control;
    PFILE_OBJECT            connobj;
};

static NTSTATUS		tdi_client_irp_complete(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp, IN PVOID Context);

//----------------------------------------------------------------------------

/*
 * TDI_EVENT_RECEIVE handler
 */

NTSTATUS
tdi_event_receive(
    IN PVOID TdiEventContext,
    IN CONNECTION_CONTEXT ConnectionContext,
    IN ULONG ReceiveFlags,
    IN ULONG BytesIndicated,
    IN ULONG BytesAvailable,
    OUT ULONG *BytesTaken,
    IN PVOID Tsdu,
    OUT PIRP *IoRequestPacket)
{
	TDI_EVENT_CONTEXT *ctx = (TDI_EVENT_CONTEXT *)TdiEventContext;
	PFILE_OBJECT connobj = ot_find_conn_ctx(ctx->fileobj, ConnectionContext);
	NTSTATUS status;

	KdPrint(("[tdi_fw] tdi_event_receive: addrobj 0x%x; connobj: 0x%x; %u/%u; flags: 0x%x\n",
		ctx->fileobj, connobj, BytesIndicated, BytesAvailable, ReceiveFlags));

	status = ((PTDI_IND_RECEIVE)(ctx->old_handler))
		(ctx->old_context, ConnectionContext, ReceiveFlags, BytesIndicated,
		BytesAvailable, BytesTaken, Tsdu, IoRequestPacket);

	KdPrint(("[tdi_fw] tdi_event_receive: status 0x%x; BytesTaken: %u; Irp: 0x%x\n",
		status, *BytesTaken, *IoRequestPacket));

	if (*BytesTaken != 0) {
		struct ot_entry *ote_conn;
		KIRQL irql;

		ote_conn = ot_find_fileobj(connobj, &irql);
		if (ote_conn != NULL) {
			ULONG bytes = *BytesTaken;

			ote_conn->bytes_in += bytes;

			// traffic stats
			KeAcquireSpinLockAtDpcLevel(&g_traffic_guard);
			
			g_traffic[TRAFFIC_TOTAL_IN] += bytes;
			
			if (ote_conn->log_disconnect)
				g_traffic[TRAFFIC_COUNTED_IN] += bytes;
			
			KeReleaseSpinLockFromDpcLevel(&g_traffic_guard);

			KeReleaseSpinLock(&g_ot_hash_guard, irql);
		}
	}

	if (*IoRequestPacket != NULL) {
	    // got IRP. replace completion.
	    struct tdi_client_irp_ctx *new_ctx;
		PIO_STACK_LOCATION irps = IoGetCurrentIrpStackLocation(*IoRequestPacket);

		new_ctx = (struct tdi_client_irp_ctx *)malloc_np(sizeof(*new_ctx));
		if (new_ctx != NULL) {

            new_ctx->connobj = connobj;

			if (irps->CompletionRoutine != NULL) {
				new_ctx->completion = irps->CompletionRoutine;
				new_ctx->context = irps->Context;
				new_ctx->old_control = irps->Control;

			} else {

				// we don't use IoSetCompletionRoutine because it uses next not current location

				new_ctx->completion = NULL;
				new_ctx->context = NULL;

			}

			irps->CompletionRoutine = tdi_client_irp_complete;
			irps->Context = new_ctx;
			irps->Control = SL_INVOKE_ON_SUCCESS | SL_INVOKE_ON_ERROR | SL_INVOKE_ON_CANCEL;
		}
	}

	return status;
}

NTSTATUS
tdi_client_irp_complete(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp, IN PVOID Context)
{
	struct tdi_client_irp_ctx *ctx = (struct tdi_client_irp_ctx *)Context;
	NTSTATUS status;

	KdPrint(("[tdi_fw] tdi_client_irp_complete: status: 0x%x; len: %u\n",
		Irp->IoStatus.Status, Irp->IoStatus.Information));

	if (Irp->IoStatus.Status == STATUS_SUCCESS) {

		struct ot_entry *ote_conn;
		KIRQL irql;

		ote_conn = ot_find_fileobj(ctx->connobj, &irql);
		if (ote_conn != NULL) {
			ULONG bytes = Irp->IoStatus.Information;

			ote_conn->bytes_in += bytes;

			// traffic stats
			KeAcquireSpinLockAtDpcLevel(&g_traffic_guard);
			
			g_traffic[TRAFFIC_TOTAL_IN] += bytes;
			
			if (ote_conn->log_disconnect)
				g_traffic[TRAFFIC_COUNTED_IN] += bytes;
			
			KeReleaseSpinLockFromDpcLevel(&g_traffic_guard);

			KeReleaseSpinLock(&g_ot_hash_guard, irql);
		}
	}

	// call original completion
	if (ctx->completion != NULL) {
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

		if (b_call) {
			status = (ctx->completion)(DeviceObject, Irp, ctx->context);

			KdPrint(("[tdi_flt] tdi_client_irp_complete: original handler: 0x%x; status: 0x%x\n",
				ctx->completion, status));

		} else
			status = STATUS_SUCCESS;

	}

	free(ctx);
	return status;
}
  
//----------------------------------------------------------------------------

/*
 * TDI_EVENT_CHAINED_RECEIVE handler
 */

NTSTATUS
tdi_event_chained_receive(
    IN PVOID TdiEventContext,
    IN CONNECTION_CONTEXT ConnectionContext,
    IN ULONG ReceiveFlags,
    IN ULONG ReceiveLength,
    IN ULONG StartingOffset,
    IN PMDL  Tsdu,
    IN PVOID TsduDescriptor)
{
	TDI_EVENT_CONTEXT *ctx = (TDI_EVENT_CONTEXT *)TdiEventContext;
	PFILE_OBJECT connobj = ot_find_conn_ctx(ctx->fileobj, ConnectionContext);
	NTSTATUS status;

	KdPrint(("[tdi_fw] tdi_event_chained_receive: addrobj 0x%x; connobj: 0x%x; %u; flags: 0x%x\n",
		ctx->fileobj, connobj, ReceiveLength, ReceiveFlags));

	status = ((PTDI_IND_CHAINED_RECEIVE)(ctx->old_handler))
		(ctx->old_context, ConnectionContext, ReceiveFlags,ReceiveLength ,
		StartingOffset, Tsdu, TsduDescriptor);

	KdPrint(("[tdi_fw] tdi_event_chained_receive: status 0x%x\n", status));

	if (status == STATUS_SUCCESS || status == STATUS_PENDING) {
		struct ot_entry *ote_conn;
		KIRQL irql;

		ote_conn = ot_find_fileobj(connobj, &irql);
		if (ote_conn != NULL) {
			ULONG bytes = ReceiveLength;

			ote_conn->bytes_in += bytes;

			// traffic stats
			KeAcquireSpinLockAtDpcLevel(&g_traffic_guard);
			
			g_traffic[TRAFFIC_TOTAL_IN] += bytes;
			
			if (ote_conn->log_disconnect)
				g_traffic[TRAFFIC_COUNTED_IN] += bytes;
			
			KeReleaseSpinLockFromDpcLevel(&g_traffic_guard);

			KeReleaseSpinLock(&g_ot_hash_guard, irql);
		}
	}

	return status;
}
