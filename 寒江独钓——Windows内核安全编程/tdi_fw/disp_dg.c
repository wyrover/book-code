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
// $Id: disp_dg.c,v 1.1 2009/04/28 12:53:27 tanwen Exp $

/*
 * This file contains TDI_SEND_DATAGRAM and TDI_RECEIVE_DATAGRAM handlers
 */

#include <ntddk.h>
#include <tdikrnl.h>
#include "sock.h"

#include "dispatch.h"
#include "filter.h"
#include "memtrack.h"
#include "obj_tbl.h"
#include "sids.h"
#include "tdi_fw.h"

static NTSTATUS tdi_receive_datagram_complete(
	IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp, IN PVOID Context);


//----------------------------------------------------------------------------

/*
 * TDI_SEND_DATAGRAM handler
 */

int
tdi_send_datagram(PIRP irp, PIO_STACK_LOCATION irps, struct completion *completion)
{
	TDI_REQUEST_KERNEL_SENDDG *param = (TDI_REQUEST_KERNEL_SENDDG *)(&irps->Parameters);
	TA_ADDRESS *local_addr, *remote_addr;
	NTSTATUS status;
	struct ot_entry *ote_addr = NULL;
	KIRQL irql;
	int result = FILTER_DENY, ipproto;
	struct flt_request request;
	struct flt_rule rule;

	memset(&request, 0, sizeof(request));

	// check device object: UDP or RawIP
	if (get_original_devobj(irps->DeviceObject, &ipproto) == NULL ||
		(ipproto != IPPROTO_UDP && ipproto != IPPROTO_IP)) {
		// unknown device object!
		KdPrint(("[tdi_fw] tdi_send_datagram: unknown DeviceObject 0x%x!\n",
			irps->DeviceObject));
		goto done;
	}

	// get local address of address object

	ote_addr = ot_find_fileobj(irps->FileObject, &irql);
	if (ote_addr == NULL) {
		KdPrint(("[tdi_fw] tdi_send_datagram: ot_find_fileobj(0x%x)!\n", irps->FileObject));
#if DBG
		// address object was created before driver was started
		result = FILTER_ALLOW;
#endif
		goto done;
	}

	KdPrint(("[tdi_fw] tdi_send_datagram: addrobj 0x%x (size: %u)\n", irps->FileObject,
		param->SendLength));

	local_addr = (TA_ADDRESS *)(ote_addr->local_addr);
	remote_addr = ((TRANSPORT_ADDRESS *)(param->SendDatagramInformation->RemoteAddress))->Address;

	KdPrint(("[tdi_fw] tdi_send_datagram(pid:%u/%u): %x:%u -> %x:%u\n",
		ote_addr->pid, PsGetCurrentProcessId(),
		ntohl(((TDI_ADDRESS_IP *)(local_addr->Address))->in_addr),
		ntohs(((TDI_ADDRESS_IP *)(local_addr->Address))->sin_port),
		ntohl(((TDI_ADDRESS_IP *)(remote_addr->Address))->in_addr),
		ntohs(((TDI_ADDRESS_IP *)(remote_addr->Address))->sin_port)));

	request.struct_size = sizeof(request);

	request.type = TYPE_DATAGRAM;
	request.direction = DIRECTION_OUT;
	request.proto = ipproto;

	// don't use ote_addr->pid because one process can create address object
	// but another one can send datagram on it
	request.pid = (ULONG)PsGetCurrentProcessId();
	if (request.pid == 0) {
		// some NetBT datagrams are sent in context of idle process: avoid it
		request.pid = ote_addr->pid;
	}
	
	// get user SID & attributes (can't call get_current_sid_a at DISPATCH_LEVEL)
	if ((request.sid_a = copy_sid_a(ote_addr->sid_a, ote_addr->sid_a_size)) != NULL)
		request.sid_a_size = ote_addr->sid_a_size;
	
	memcpy(&request.addr.from, &local_addr->AddressType, sizeof(struct sockaddr));
	memcpy(&request.addr.to, &remote_addr->AddressType, sizeof(struct sockaddr));
	request.addr.len = sizeof(struct sockaddr_in);

	memset(&rule, 0, sizeof(rule));

	result = quick_filter(&request, &rule);
	
	memcpy(request.log_rule_id, rule.rule_id, RULE_ID_SIZE);

	if (rule.log >= RULE_LOG_LOG) {
		ULONG bytes = param->SendLength;

		// traffic stats
		KeAcquireSpinLockAtDpcLevel(&g_traffic_guard);
		
		g_traffic[TRAFFIC_TOTAL_OUT] += bytes;
		
		if (rule.log >= RULE_LOG_COUNT) {
			request.log_bytes_out = bytes;

			g_traffic[TRAFFIC_COUNTED_OUT] += bytes;

		} else
			request.log_bytes_out = (ULONG)-1;

		KeReleaseSpinLockFromDpcLevel(&g_traffic_guard);

		log_request(&request);
	}

done:

	// cleanup
	if (ote_addr != NULL)
		KeReleaseSpinLock(&g_ot_hash_guard, irql);
	if (request.sid_a != NULL)
		free(request.sid_a);

	if (result == FILTER_DENY)
		irp->IoStatus.Status = STATUS_INVALID_ADDRESS;	// set fake status

	return result;
}

//----------------------------------------------------------------------------

/*
 * TDI_RECEIVE_DATAGRAM handler
 */

int
tdi_receive_datagram(PIRP irp, PIO_STACK_LOCATION irps, struct completion *completion)
{
	KdPrint(("[tdi_fw] tdi_receive_datagram: addrobj 0x%x\n", irps->FileObject));

	completion->routine = tdi_receive_datagram_complete;

	return FILTER_ALLOW;
}

NTSTATUS
tdi_receive_datagram_complete(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp, IN PVOID Context)
{
	PIO_STACK_LOCATION irps = IoGetCurrentIrpStackLocation(Irp);
	TDI_REQUEST_KERNEL_RECEIVEDG *param = (TDI_REQUEST_KERNEL_RECEIVEDG *)(&irps->Parameters);
	PFILE_OBJECT addrobj = irps->FileObject;
	struct ot_entry *ote_addr = NULL;
	KIRQL irql;
	int result = FILTER_DENY, ipproto;
	NTSTATUS status = STATUS_SUCCESS;
	struct flt_request request;
	struct flt_rule rule;
	TA_ADDRESS *local_addr, *remote_addr;

	memset(&request, 0, sizeof(request));

	// check device object: UDP or RawIP
	if (get_original_devobj(DeviceObject, &ipproto) == NULL ||
		(ipproto != IPPROTO_UDP && ipproto != IPPROTO_IP)) {
		// unknown device object!
		KdPrint(("[tdi_fw] tdi_receive_datagram_complete: unknown DeviceObject 0x%x!\n",
			DeviceObject));
		status = STATUS_UNSUCCESSFUL;
		goto done;
	}

	KdPrint(("[tdi_fw] tdi_receive_datagram_complete: addrobj 0x%x; status 0x%x; information %u\n",
		addrobj, Irp->IoStatus.Status, Irp->IoStatus.Information));

	if (Irp->IoStatus.Status != STATUS_SUCCESS) {
		KdPrint(("[tdi_fw] tdi_receive_datagram_complete: status 0x%x\n",
			Irp->IoStatus.Status));
		status = Irp->IoStatus.Status;
		goto done;
	}

	ote_addr = ot_find_fileobj(addrobj, &irql);
	if (ote_addr == NULL) {
		KdPrint(("[tdi_fw] tdi_receive_datagram_complete: ot_find_fileobj(0x%x)!\n",
			addrobj));
		status = STATUS_UNSUCCESSFUL;
		goto done;
	}

	request.struct_size = sizeof(request);

	request.type = TYPE_DATAGRAM;
	request.direction = DIRECTION_IN;
	request.proto = ipproto;
	request.pid = ote_addr->pid;
	
	// get user SID & attributes!
	if ((request.sid_a = copy_sid_a(ote_addr->sid_a, ote_addr->sid_a_size)) != NULL)
		request.sid_a_size = ote_addr->sid_a_size;

	local_addr = (TA_ADDRESS *)(ote_addr->local_addr);
	remote_addr = ((TRANSPORT_ADDRESS *)(param->ReceiveDatagramInformation->RemoteAddress))->Address;

	KdPrint(("[tdi_fw] tdi_receive_datagram_complete(pid:%u): %x:%u -> %x:%u\n",
		ote_addr->pid,
		ntohl(((TDI_ADDRESS_IP *)(remote_addr->Address))->in_addr),
		ntohs(((TDI_ADDRESS_IP *)(remote_addr->Address))->sin_port),
		ntohl(((TDI_ADDRESS_IP *)(local_addr->Address))->in_addr),
		ntohs(((TDI_ADDRESS_IP *)(local_addr->Address))->sin_port)));

	memcpy(&request.addr.from, &remote_addr->AddressType, sizeof(struct sockaddr));
	memcpy(&request.addr.to, &local_addr->AddressType, sizeof(struct sockaddr));
	request.addr.len = sizeof(struct sockaddr_in);

	memset(&rule, 0, sizeof(rule));

	result = quick_filter(&request, &rule);

	memcpy(request.log_rule_id, rule.rule_id, RULE_ID_SIZE);

	if (rule.log >= RULE_LOG_LOG) {
		ULONG bytes = Irp->IoStatus.Information;

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
	// convert result to NTSTATUS
	if (result == FILTER_ALLOW)
		status = STATUS_SUCCESS;
	else {		/* FILTER_DENY */

		if (status == STATUS_SUCCESS)
			status = Irp->IoStatus.Status = STATUS_ACCESS_DENIED;	// good status?

	}

	// cleanup
	if (ote_addr != NULL)
		KeReleaseSpinLock(&g_ot_hash_guard, irql);
	if (request.sid_a != NULL)
		free(request.sid_a);
	
	return tdi_generic_complete(DeviceObject, Irp, Context);
}
