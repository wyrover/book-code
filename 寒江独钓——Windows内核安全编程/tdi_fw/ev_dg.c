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
// $Id: ev_dg.c,v 1.1 2009/04/28 12:53:27 tanwen Exp $

/*
 * This file contains TDI_EVENT_RECEIVE_DATAGRAM handler
 */

#include <ntddk.h>
#include <tdikrnl.h>
#include "sock.h"

#include "events.h"
#include "filter.h"
#include "memtrack.h"
#include "obj_tbl.h"
#include "sids.h"
#include "tdi_fw.h"

//----------------------------------------------------------------------------

/*
 * TDI_EVENT_RECEIVE_DATAGRAM handler
 */

NTSTATUS tdi_event_receive_datagram(
    IN PVOID TdiEventContext,
    IN LONG SourceAddressLength,
    IN PVOID SourceAddress,
    IN LONG OptionsLength,
    IN PVOID Options,
    IN ULONG ReceiveDatagramFlags,
    IN ULONG BytesIndicated,
    IN ULONG BytesAvailable,
    OUT ULONG *BytesTaken,
    IN PVOID Tsdu,
    OUT PIRP *IoRequestPacket)
{
	TDI_EVENT_CONTEXT *ctx = (TDI_EVENT_CONTEXT *)TdiEventContext;
	struct ot_entry *ote_addr = NULL;
	KIRQL irql;
	TA_ADDRESS *remote_addr, *local_addr;
	NTSTATUS status;
	int ipproto, result = FILTER_DENY;
	struct flt_request request;
	struct flt_rule rule;

	// get local address of address object

	memset(&request, 0, sizeof(request));
	ote_addr = ot_find_fileobj(ctx->fileobj, &irql);
	if (ote_addr == NULL) {
		KdPrint(("[tdi_fw] tdi_receive_datagram: ot_find_fileobj(0x%x)!\n", ctx->fileobj));
		goto done;
	}

	KdPrint(("[tdi_fw] tdi_event_receive_datagram: addrobj 0x%x\n", ctx->fileobj));

	// check device object: UDP or RawIP
	if (get_original_devobj(ote_addr->devobj, &ipproto) == NULL ||
		(ipproto != IPPROTO_UDP && ipproto != IPPROTO_IP)) {
		// unknown device object!
		KdPrint(("[tdi_fw] tdi_event_receive_datagram: unknown DeviceObject 0x%x!\n",
			ote_addr));
		goto done;
	}

	local_addr = (TA_ADDRESS *)(ote_addr->local_addr);
	remote_addr = ((TRANSPORT_ADDRESS *)SourceAddress)->Address;

	KdPrint(("[tdi_fw] tdi_event_receive_datagram(pid:%u): %x:%u -> %x:%u\n",
		ote_addr->pid,
		ntohl(((TDI_ADDRESS_IP *)(remote_addr->Address))->in_addr),
		ntohs(((TDI_ADDRESS_IP *)(remote_addr->Address))->sin_port),
		ntohl(((TDI_ADDRESS_IP *)(local_addr->Address))->in_addr),
		ntohs(((TDI_ADDRESS_IP *)(local_addr->Address))->sin_port)));

	// call quick filter for datagram
	request.struct_size = sizeof(request);

	request.type = TYPE_DATAGRAM;
	request.direction = DIRECTION_IN;
	request.proto = ipproto;
	request.pid = ote_addr->pid;

	// get user SID & attributes (can't call get_current_sid_a at DISPATCH_LEVEL)
	if ((request.sid_a = copy_sid_a(ote_addr->sid_a, ote_addr->sid_a_size)) != NULL)
		request.sid_a_size = ote_addr->sid_a_size;
	
	memcpy(&request.addr.from, &remote_addr->AddressType, sizeof(struct sockaddr));
	memcpy(&request.addr.to, &local_addr->AddressType, sizeof(struct sockaddr));
	request.addr.len = sizeof(struct sockaddr_in);

	memset(&rule, 0, sizeof(rule));

	result = quick_filter(&request, &rule);

	memcpy(request.log_rule_id, rule.rule_id, RULE_ID_SIZE);

	if (rule.log >= RULE_LOG_LOG) {
		ULONG bytes = BytesAvailable;

		// traffic stats
		KeAcquireSpinLockAtDpcLevel(&g_traffic_guard);
		
		g_traffic[TRAFFIC_TOTAL_IN] += bytes;
		
		if (rule.log >= RULE_LOG_COUNT) {
			request.log_bytes_in = bytes;

			g_traffic[TRAFFIC_COUNTED_IN] += bytes;

		} else
			request.log_bytes_in = (ULONG)-1;

		KeReleaseSpinLockFromDpcLevel(&g_traffic_guard);

		log_request(&request);
	}

done:
	// cleanup
	if (ote_addr != NULL)
		KeReleaseSpinLock(&g_ot_hash_guard, irql);
	if (request.sid_a != NULL)
		free(request.sid_a);

	if (result == FILTER_ALLOW) {

		return ((PTDI_IND_RECEIVE_DATAGRAM)(ctx->old_handler))
			(ctx->old_context, SourceAddressLength, SourceAddress, OptionsLength,
			Options, ReceiveDatagramFlags, BytesIndicated, BytesAvailable, BytesTaken,
			Tsdu, IoRequestPacket);
	
	} else
		return STATUS_DATA_NOT_ACCEPTED;
}
