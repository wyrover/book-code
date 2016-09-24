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
// $Id: disp_ev.c,v 1.1 2009/04/28 12:53:27 tanwen Exp $

/*
 * This file contains TDI_SET_EVENT_HANDLER handler
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

int
tdi_set_event_handler(PIRP irp, PIO_STACK_LOCATION irps, struct completion *completion)
{
	PTDI_REQUEST_KERNEL_SET_EVENT r = (PTDI_REQUEST_KERNEL_SET_EVENT)&irps->Parameters;
	NTSTATUS status;
	struct ot_entry *ote_addr = NULL;
	KIRQL irql;
	int result = FILTER_DENY;
	TDI_EVENT_CONTEXT *ctx;

	KdPrint(("[tdi_fw] tdi_set_event_handler: [%s] devobj 0x%x; addrobj 0x%x; EventType: %d\n",
		r->EventHandler ? "(+)ADD" : "(-)REMOVE",
		irps->DeviceObject,
		irps->FileObject,
		r->EventType));

	ote_addr = ot_find_fileobj(irps->FileObject, &irql);
	if (ote_addr == NULL) {
		KdPrint(("[tdi_fw] tdi_set_event_handler: ot_find_fileobj(0x%x)\n", irps->FileObject));
		if (r->EventHandler == NULL) {
			 // for fileobjects loaded earlier than our driver allow removing
			result = FILTER_ALLOW;
		}
		goto done;
	}

	if (r->EventType < 0 || r->EventType >= MAX_EVENT) {
		KdPrint(("[tdi_fw] tdi_set_event_handler: unknown EventType %d!\n", r->EventType));
		result = FILTER_ALLOW;
		goto done;
	}

	ctx = &ote_addr->ctx[r->EventType];

	if (r->EventHandler != NULL) {
		/* add EventHandler */
		int i;

		for (i = 0; g_tdi_event_handlers[i].event != (ULONG)-1; i++)
			if (g_tdi_event_handlers[i].event == r->EventType)
				break;

		if (g_tdi_event_handlers[i].event == (ULONG)-1) {
			KdPrint(("[tdi_fw] tdi_set_event_handler: unknown EventType %d!\n", r->EventType));
			result = FILTER_ALLOW;
			goto done;
		}

		ctx->old_handler = r->EventHandler;
		ctx->old_context = r->EventContext;

		if (g_tdi_event_handlers[i].handler != NULL) {
			r->EventHandler = g_tdi_event_handlers[i].handler;
			r->EventContext = ctx;
		} else {
			r->EventHandler = NULL;
			r->EventContext = NULL;
		}

		KdPrint(("[tdi_fw] tdi_set_event_handler: old_handler 0x%x; old_context 0x%x\n",
			r->EventHandler, r->EventContext));

	} else {
		/* remove EventHandler */
		ctx->old_handler = NULL;
		ctx->old_context = NULL;
	}

	// change LISTEN state
	if (r->EventType == TDI_EVENT_CONNECT) {
		TA_ADDRESS *local_addr;

		if (r->EventHandler != NULL) {
			// add "LISTEN" info
			status = add_listen(ote_addr);
			if (status != STATUS_SUCCESS) {
				KdPrint(("[tdi_fw] tdi_set_event_handler: add_listen: 0x%x!\n", status));
				goto done;
			}
		} else if (ote_addr->listen_entry != NULL) {
			// remove "LISTEN" info
			del_listen_obj(ote_addr->listen_entry, FALSE);

			ote_addr->listen_entry = NULL;
		}
	
		// log it if address is not 127.0.0.1
		local_addr = (TA_ADDRESS *)(ote_addr->local_addr);
		if (ntohl(((TDI_ADDRESS_IP *)(local_addr->Address))->in_addr) != 0x7f000001) {
			struct flt_request request;
		
			memset(&request, 0, sizeof(request));

			request.struct_size = sizeof(request);

			request.type = (r->EventHandler != NULL) ? TYPE_LISTEN : TYPE_NOT_LISTEN;
			request.proto = IPPROTO_TCP;	// correct?

			if (r->EventHandler != NULL) {
				// for removing event handler ProcessNotifyProc can be already called
				request.pid = (ULONG)PsGetCurrentProcessId();
				if (request.pid == 0) {
					// avoid idle process pid (XXX do we need this?)
					request.pid = ote_addr->pid;
				}
			} else
				request.pid = (ULONG)-1;

			// get user SID & attributes (can't call get_current_sid_a at DISPATCH_LEVEL)
			if ((request.sid_a = copy_sid_a(ote_addr->sid_a, ote_addr->sid_a_size)) != NULL)
				request.sid_a_size = ote_addr->sid_a_size;
			
			memcpy(&request.addr.from, &local_addr->AddressType, sizeof(struct sockaddr));
			request.addr.len = sizeof(struct sockaddr_in);

			log_request(&request);

			if (request.sid_a != NULL)
				free(request.sid_a);
		}
	}

	result = FILTER_ALLOW;
done:
	// cleanup
	if (ote_addr != NULL)
		KeReleaseSpinLock(&g_ot_hash_guard, irql);

	return result;
}
