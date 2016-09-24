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
// $Id: ev_conn.c,v 1.1 2009/04/28 12:53:27 tanwen Exp $

/*
 * This file contain TDI_EVENT_CONNECT & TDI_EVENT_DISCONNECT handlers
 */

#include <ntddk.h>
#include <tdikrnl.h>
#include "sock.h"

#include "conn_state.h"
#include "dispatch.h"
#include "events.h"
#include "memtrack.h"
#include "obj_tbl.h"
#include "sids.h"
#include "tdi_fw.h"

struct accept_param {
    PIO_COMPLETION_ROUTINE	old_cr;
    PVOID					old_context;
	PFILE_OBJECT			fileobj;
	UCHAR					old_control;
};

static NTSTATUS		tdi_evconn_accept_complete(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp, IN PVOID Context);

//----------------------------------------------------------------------------

/*
 * TDI_EVENT_CONNECT handler
 */

NTSTATUS
tdi_event_connect(
    IN PVOID TdiEventContext,
    IN LONG RemoteAddressLength,
    IN PVOID RemoteAddress,
    IN LONG UserDataLength,
    IN PVOID UserData,
    IN LONG OptionsLength,
    IN PVOID Options,
    OUT CONNECTION_CONTEXT *ConnectionContext,
    OUT PIRP *AcceptIrp)
{
	TDI_EVENT_CONTEXT *ctx = (TDI_EVENT_CONTEXT *)TdiEventContext;
	TA_ADDRESS *remote_addr = ((TRANSPORT_ADDRESS *)RemoteAddress)->Address, *local_addr;
	struct ot_entry *ote_addr = NULL, *ote_conn = NULL;
	KIRQL irql;
	struct flt_request request;
	struct flt_rule rule;
	int result = FILTER_DENY;
	NTSTATUS status;
	PIO_STACK_LOCATION irps = NULL;
	struct accept_param *param = NULL;

	memset(&request, 0, sizeof(request));

	KdPrint(("[tdi_fw] tdi_event_connect: addrobj 0x%x\n", ctx->fileobj));
	
	ote_addr = ot_find_fileobj(ctx->fileobj, &irql);
	if (ote_addr == NULL) {
		KdPrint(("[tdi_fw] tdi_event_connect: ot_find_fileobj(0x%x)\n", ctx->fileobj));
		goto done;
	}

	local_addr = (TA_ADDRESS *)(ote_addr->local_addr);

	KdPrint(("[tdi_fw] tdi_event_connect(pid:%u): %x:%u -> %x:%u\n",
		ote_addr->pid,
		ntohl(((TDI_ADDRESS_IP *)(remote_addr->Address))->in_addr),
		ntohs(((TDI_ADDRESS_IP *)(remote_addr->Address))->sin_port),
		ntohl(((TDI_ADDRESS_IP *)(local_addr->Address))->in_addr),
		ntohs(((TDI_ADDRESS_IP *)(local_addr->Address))->sin_port)));

	/*
	 * request quick filter
	 */

	request.struct_size = sizeof(request);

	request.type = TYPE_CONNECT;
	request.direction = DIRECTION_IN;
	request.proto = IPPROTO_TCP;
	request.pid = ote_addr->pid;

	// get user SID & attributes!
	if ((request.sid_a = copy_sid_a(ote_addr->sid_a, ote_addr->sid_a_size)) != NULL)
		request.sid_a_size = ote_addr->sid_a_size;
	
	memcpy(&request.addr.from, &remote_addr->AddressType, sizeof(struct sockaddr));
	memcpy(&request.addr.to, &local_addr->AddressType, sizeof(struct sockaddr));
	request.addr.len = sizeof(struct sockaddr_in);

	result = quick_filter(&request, &rule);

	memcpy(request.log_rule_id, rule.rule_id, RULE_ID_SIZE);

	// log request later

	if (result == FILTER_DENY)
		goto done;

	result = FILTER_DENY;

	// leave spinlock before calling original handler
	KeReleaseSpinLock(&g_ot_hash_guard, irql);
	ote_addr = NULL;

	/*
	 * run original handler
	 */

	status = ((PTDI_IND_CONNECT)(ctx->old_handler))
		(ctx->old_context, RemoteAddressLength, RemoteAddress,
		UserDataLength, UserData, OptionsLength, Options, ConnectionContext,
		AcceptIrp);

    if (status != STATUS_MORE_PROCESSING_REQUIRED || *AcceptIrp == NULL) {
        KdPrint(("[tdi_fw] tdi_event_connect: status from original handler: 0x%x\n", status));
		goto done;
    }

	/*
	 * reinitialize connobj
	 */

	irps = IoGetCurrentIrpStackLocation(*AcceptIrp);
	KdPrint(("[tdi_fw] tdi_event_connect: connobj 0x%x\n", irps->FileObject));

	// patch *AcceptIrp to change completion routine

	param = (struct accept_param *)malloc_np(sizeof(*param));
	if (param == NULL) {
		KdPrint(("[tdi_fw] tdi_event_connect: malloc_np!\n"));
        status = STATUS_INSUFFICIENT_RESOURCES;
		goto done;
	}

	param->old_cr = irps->CompletionRoutine;
	param->old_context = irps->Context;
	param->fileobj = irps->FileObject;

	param->old_control = irps->Control;

	// can't use IoSetCompletionRoutine because it uses next not current stack location
	irps->Control = SL_INVOKE_ON_SUCCESS | SL_INVOKE_ON_ERROR | SL_INVOKE_ON_CANCEL;
	irps->CompletionRoutine = tdi_evconn_accept_complete;
	irps->Context = param;

	param = NULL;

	// find connobj for changing

	ote_conn = ot_find_fileobj(irps->FileObject, &irql);
	if (ote_conn == NULL) {
		KdPrint(("[tdi_fw] tdi_event_connect: ot_find_fileobj(0x%x)\n", irps->FileObject));
        status = STATUS_OBJECT_NAME_NOT_FOUND;
		goto done;
	}

    ASSERT(ote_conn->type == FILEOBJ_CONNOBJ);

    // connobj must be associated with addrobj!
    if (ote_conn->associated_fileobj != ctx->fileobj) {
        KdPrint(("[tdi_fw] tdi_event_connect: 0x%x != 0x%x\n", ote_conn->associated_fileobj, ctx->fileobj));
        status = STATUS_INVALID_PARAMETER;
        goto done;
    }

    // change conn_ctx (if needed)
    if (ote_conn->conn_ctx != *ConnectionContext) {
        // update (conn_ctx, addrobj)->connobj

	    status = ot_del_conn_ctx(ote_conn->associated_fileobj, ote_conn->conn_ctx);
	    if (status != STATUS_SUCCESS) {
		    KdPrint(("[tdi_fw] tdi_event_connect: ot_del_conn_ctx: 0x%x\n", status));
            goto done;
        }

        ote_conn->conn_ctx = *ConnectionContext;

	    status = ot_add_conn_ctx(ote_conn->associated_fileobj, ote_conn->conn_ctx, irps->FileObject);
	    if (status != STATUS_SUCCESS) {
		    KdPrint(("[tdi_fw] tdi_event_connect: ot_add_conn_ctx: 0x%x\n", status));
		    goto done;
	    }
    
    }

    // clear listen & conn entries in connobj (fileobject can be reused)

    ASSERT(ote_conn->listen_entry == NULL);
    if (ote_conn->listen_entry != NULL)
		del_listen_obj(ote_conn->listen_entry, FALSE);       // free build case
    
    if (ote_conn->conn_entry != NULL) {

		if (ote_conn->ipproto == IPPROTO_TCP && ote_conn->log_disconnect)
			log_disconnect(ote_conn);
    	
		del_tcp_conn_obj(ote_conn->conn_entry, FALSE);
	}

    // clear bytes count
    ote_conn->bytes_in = ote_conn->bytes_out = 0;

    // setup log_disconnect flag from rule
    ote_conn->log_disconnect = (rule.log >= RULE_LOG_COUNT);

	// sanity check
	if (local_addr->AddressLength != remote_addr->AddressLength) {
		KdPrint(("[tdi_fw] tdi_event_connect: different addr lengths! (%u != %u)\n",
			local_addr->AddressLength,
			remote_addr->AddressLength));
        status = STATUS_INFO_LENGTH_MISMATCH;
		goto done;
	}

	// associate remote address with connobj
	
	if (remote_addr->AddressLength > sizeof(ote_conn->remote_addr)) {
		KdPrint(("[tdi_fw] tdi_event_connect: address too long! (%u)\n",
			remote_addr->AddressLength));
        status = STATUS_BUFFER_TOO_SMALL;
		goto done;
	}
	memcpy(ote_conn->remote_addr, remote_addr, remote_addr->AddressLength);

	// associate local address with connobj

	if (local_addr->AddressLength > sizeof(ote_conn->local_addr)) {
		KdPrint(("[tdi_fw] tdi_event_connect: address too long! (%u)\n",
			local_addr->AddressLength));
        status = STATUS_BUFFER_TOO_SMALL;
		goto done;
	}
	memcpy(ote_conn->local_addr, local_addr, local_addr->AddressLength);

	// create connection with "SYN_RCVD" state
	status = add_tcp_conn(ote_conn, TCP_STATE_SYN_RCVD);
	if (status != STATUS_SUCCESS) {
		KdPrint(("[tdi_fw] tdi_event_connect: add_tcp_conn: 0x%x\n", status));
		goto done;
	}

	result = FILTER_ALLOW;

done:
	// if logging is needed log request
	if (rule.log >= RULE_LOG_LOG) {
        if (result != FILTER_ALLOW && rule.result == FILTER_ALLOW) {
			request.type = TYPE_CONNECT_ERROR;		// error has been occured
            request.status = status;
        }

		log_request(&request);
	}

	if (result != FILTER_ALLOW) {
		// deny incoming connection

        KdPrint(("[tdi_fw] tdi_event_connect: deny on reason 0x%x\n", status));

		if (irps != NULL) {
			// delete connection
			if (ote_conn != NULL && ote_conn->conn_entry != NULL) {
				del_tcp_conn_obj(ote_conn->conn_entry, FALSE);
				ote_conn->conn_entry = NULL;
			}

	        // release spinlock before IoCompleteRequest to avoid completion call inside spinlock
            if (ote_addr != NULL || ote_conn != NULL) {
		        KeReleaseSpinLock(&g_ot_hash_guard, irql);
                
                ote_addr = NULL;
                ote_conn = NULL;
            }

			// destroy accepted IRP
			(*AcceptIrp)->IoStatus.Status = STATUS_UNSUCCESSFUL;
			IoCompleteRequest(*AcceptIrp, IO_NO_INCREMENT);
		}

		*AcceptIrp = NULL;
		status = STATUS_CONNECTION_REFUSED;
	} else
		status = STATUS_MORE_PROCESSING_REQUIRED;

	// cleanup
	if (ote_addr != NULL || ote_conn != NULL)
		KeReleaseSpinLock(&g_ot_hash_guard, irql);
	if (param != NULL)
		free(param);
	if (request.sid_a != NULL)
		free(request.sid_a);

	return status;
}

NTSTATUS
tdi_evconn_accept_complete(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp, IN PVOID Context)
{
	PIO_STACK_LOCATION irps = IoGetNextIrpStackLocation(Irp);
	struct accept_param *param = (struct accept_param *)Context;
	NTSTATUS status = STATUS_SUCCESS;

	KdPrint(("[tdi_fw] tdi_evconn_accept_complete: status 0x%x\n", Irp->IoStatus.Status));

    if (Irp->IoStatus.Status == STATUS_SUCCESS) {
		set_tcp_conn_state(param->fileobj, TCP_STATE_ESTABLISHED_IN);

	    // query & update connection local_addr
	    update_conn_info(irps->DeviceObject, param->fileobj);

    } else
		del_tcp_conn(param->fileobj, TRUE);		// TRUE because we logged connection in event handler

	// restore routine and context (and even control!)
	irps->CompletionRoutine = param->old_cr;
	irps->Context = param->old_context;
	irps->Control = param->old_control;

	// call original completion (I like this code :-)

	if (param->old_cr != NULL) {
		// call old completion (see the old control)
		BOOLEAN b_call = FALSE;

		if (Irp->Cancel) {
			// cancel
			if (param->old_control & SL_INVOKE_ON_CANCEL)
				b_call = TRUE;
		} else {
			if (Irp->IoStatus.Status >= STATUS_SUCCESS) {
				// success
				if (param->old_control & SL_INVOKE_ON_SUCCESS)
					b_call = TRUE;
			} else {
				// error
				if (param->old_control & SL_INVOKE_ON_ERROR)
					b_call = TRUE;
			}
		}

		if (b_call)
			status = param->old_cr(DeviceObject, Irp, param->old_context);
	}

	free(param);
	return status;
}

//----------------------------------------------------------------------------

/*
 * TDI_EVENT_DISCONNECT handler
 */

NTSTATUS
tdi_event_disconnect(
    IN PVOID TdiEventContext,
    IN CONNECTION_CONTEXT ConnectionContext,
    IN LONG DisconnectDataLength,
    IN PVOID DisconnectData,
    IN LONG DisconnectInformationLength,
    IN PVOID DisconnectInformation,
    IN ULONG DisconnectFlags)
{
	TDI_EVENT_CONTEXT *ctx = (TDI_EVENT_CONTEXT *)TdiEventContext;
	PFILE_OBJECT connobj = ot_find_conn_ctx(ctx->fileobj, ConnectionContext);

	KdPrint(("[tdi_fw] tdi_event_disconnect: connobj: 0x%x (flags: 0x%x)\n",
		connobj, DisconnectFlags));

	if (DisconnectFlags & TDI_DISCONNECT_RELEASE) {
		int state = get_tcp_conn_state_by_obj(connobj);
		
		if (state == TCP_STATE_ESTABLISHED_IN || state == TCP_STATE_ESTABLISHED_OUT)
			set_tcp_conn_state(connobj, TCP_STATE_CLOSE_WAIT);
		else if (state == TCP_STATE_FIN_WAIT2)
			set_tcp_conn_state(connobj, TCP_STATE_TIME_WAIT);
		else
			KdPrint(("[tdi_fw] tdi_event_disconnect: weird conn state: %d\n", state));

	} else
		del_tcp_conn(connobj, TRUE);

	return ((PTDI_IND_DISCONNECT)(ctx->old_handler))(ctx->old_context, ConnectionContext,
		DisconnectDataLength, DisconnectData, DisconnectInformationLength,
		DisconnectInformation, DisconnectFlags);
}
