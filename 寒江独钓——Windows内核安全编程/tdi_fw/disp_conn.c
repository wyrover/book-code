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
// $Id: disp_conn.c,v 1.1 2009/04/28 12:53:27 tanwen Exp $

/*
 * This file contains TDI_CONNECT & TDI_DISCONNECT handlers
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

struct delayed_ucn_param {
	WORK_QUEUE_ITEM	item;
	PDEVICE_OBJECT	devobj;
	PFILE_OBJECT	fileobj;
};

struct uci_param {
	PFILE_OBJECT	connobj;
	char			address[];
};

static void		delayed_ucn(PVOID p);
static NTSTATUS	update_conn_info_complete(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp, IN PVOID Context);


static NTSTATUS	tdi_connect_complete(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp, IN PVOID Context);
static NTSTATUS	tdi_disconnect_complete(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp, IN PVOID Context);

//----------------------------------------------------------------------------

/*
 * TDI_CONNECT handler
 */

int
tdi_connect(PIRP irp, PIO_STACK_LOCATION irps, struct completion *completion)
{
	PTDI_REQUEST_KERNEL_CONNECT param = (PTDI_REQUEST_KERNEL_CONNECT)(&irps->Parameters);
	TA_ADDRESS *remote_addr = ((TRANSPORT_ADDRESS *)(param->RequestConnectionInformation->RemoteAddress))->Address;
	PFILE_OBJECT addrobj;
	NTSTATUS status;
	TA_ADDRESS *local_addr;
	int result = FILTER_DENY, ipproto;
	struct ot_entry *ote_conn = NULL, *ote_addr;
	KIRQL irql;
	struct flt_request request;
	struct flt_rule rule;

	memset(&request, 0, sizeof(request));

	KdPrint(("[tdi_fw] tdi_connect: connobj 0x%x, to address %x:%u\n",
		irps->FileObject,
		ntohl(((TDI_ADDRESS_IP *)(remote_addr->Address))->in_addr),
		ntohs(((TDI_ADDRESS_IP *)(remote_addr->Address))->sin_port)));

	// check device object: TCP or UDP
	if (irps->DeviceObject != g_tcpfltobj && irps->DeviceObject != g_udpfltobj) {
		KdPrint(("[tdi_fw] tdi_connect: unknown DeviceObject 0x%x!\n", irps->DeviceObject));
		goto done;
	}

	ote_conn = ot_find_fileobj(irps->FileObject, &irql);
	if (ote_conn == NULL) {
		KdPrint(("[tdi_fw] tdi_connect: ot_find_fileobj(0x%x)!\n", irps->FileObject));
		goto done;
	}

	if (get_original_devobj(irps->DeviceObject, &ipproto) == NULL ||
		(ipproto != IPPROTO_TCP && ipproto != IPPROTO_UDP)) {
		// invalid device object!
		KdPrint(("[tdi_fw] tdi_connect: invalid device object 0x%x!\n", irps->DeviceObject));
		goto done;
	}

	if (ipproto == IPPROTO_TCP) {
		/*
		 * For TCP: get addrobj by connobj and get local address by it
		 */

		addrobj = ote_conn->associated_fileobj;
		if (addrobj == NULL) {
			KdPrint(("[tdi_fw] tdi_connect: empty addrobj!\n"));
			goto done;
		}

		ote_addr = ot_find_fileobj(addrobj, NULL); // we're already in spinlock
		if (ote_addr == NULL) {
			KdPrint(("[tdi_fw] tdi_connect: ot_find_fileobj(0x%x)!\n", addrobj));
			goto done;
		}

	} else {
		/*
		 * For UDP: connobj and addrobj are the same
		 */
		KdPrint(("[tdi_fw] tdi_connect: connected UDP socket detected\n"));

		// for connected UDP sockets connobj and addrobj are the same
		addrobj= irps->FileObject;
		ote_addr = ote_conn;
	}

	local_addr = (TA_ADDRESS *)(ote_addr->local_addr);

	// sanity check
	if (local_addr->AddressLength != remote_addr->AddressLength) {
		KdPrint(("[tdi_fw] tdi_connect: different addr lengths! (%u != %u)\n",
			local_addr->AddressLength, remote_addr->AddressLength));
		goto done;
	}

	// set remote address with connobj
	
	if (remote_addr->AddressLength > sizeof(ote_conn->remote_addr)) {
		KdPrint(("[tdi_fw] tdi_connect: address too long! (%u)\n", remote_addr->AddressLength));
		goto done;
	}
	memcpy(ote_conn->remote_addr, remote_addr, remote_addr->AddressLength);

	// set local address with connobj

	if (local_addr->AddressLength > sizeof(ote_conn->local_addr)) {
		KdPrint(("[tdi_fw] tdi_connect: address to long! (%u)\n", local_addr->AddressLength));
		goto done;
	}
	memcpy(ote_conn->local_addr, local_addr, local_addr->AddressLength);

	KdPrint(("[tdi_fw] tdi_connect(pid:%u/%u): %x:%u -> %x:%u (ipproto = %d)\n",
		ote_conn->pid, PsGetCurrentProcessId(),
		ntohl(((TDI_ADDRESS_IP *)(local_addr->Address))->in_addr),
		ntohs(((TDI_ADDRESS_IP *)(local_addr->Address))->sin_port),
		ntohl(((TDI_ADDRESS_IP *)(remote_addr->Address))->in_addr),
		ntohs(((TDI_ADDRESS_IP *)(remote_addr->Address))->sin_port), ipproto));

	/*
	 * Call quick_filter
	 */

	request.struct_size = sizeof(request);

	request.type = TYPE_CONNECT;
	request.direction = DIRECTION_OUT;
	request.proto = ipproto;

	// don't use ote_conn->pid because one process can create connection object
	// but another one can connect
	request.pid = (ULONG)PsGetCurrentProcessId();
	if (request.pid == 0) {
		// avoid idle process pid (XXX do we need this?)
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

	if (result == FILTER_ALLOW && ipproto == IPPROTO_TCP) {
		struct flt_request *context_req = NULL;

		// add connection with state "SYN_SENT"
		status = add_tcp_conn(ote_conn, TCP_STATE_SYN_SENT);
		if (status != STATUS_SUCCESS) {
			KdPrint(("[tdi_fw] tdi_connect: add_conn: 0x%x!\n", status));
			
			result = FILTER_DENY;
			goto done;				// don't log this failure
		}

		if (rule.log >= RULE_LOG_LOG) {
			// set ote_conn->log_disconnect
			ote_conn->log_disconnect = (rule.log >= RULE_LOG_COUNT);

			// copy request for completion (LOG success or not)
			context_req = (struct flt_request *)malloc_np(sizeof(*context_req));
			if (context_req != NULL) {
				memcpy(context_req, &request, sizeof(*context_req));

				// don't free SID
				request.sid_a = NULL;

				// don't log request in this time
				rule.log = RULE_LOG_NOLOG;
			}
		}

		// set completion to add connection info to connection table
		completion->routine = tdi_connect_complete;
		completion->context = context_req;
	}

	// if logging is needed log request
	if (rule.log >= RULE_LOG_LOG)
		log_request(&request);

done:
	// cleanup
	if (ote_conn != NULL)
		KeReleaseSpinLock(&g_ot_hash_guard, irql);
	if (request.sid_a != NULL)
		free(request.sid_a);

	if (result != FILTER_ALLOW) {
		irp->IoStatus.Status = STATUS_REMOTE_NOT_LISTENING;	// set fake status
	}

	return result;
}

NTSTATUS
tdi_connect_complete(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp, IN PVOID Context)
{
	NTSTATUS status;
	struct flt_request *request = (struct flt_request *)Context;
	PIO_STACK_LOCATION irps = IoGetCurrentIrpStackLocation(Irp);

	KdPrint(("[tdi_fw] tdi_connect_complete: status 0x%x\n", Irp->IoStatus.Status));

	if (Irp->IoStatus.Status == STATUS_SUCCESS) {

		if (request != NULL)
			log_request(request);		// log successful connection
		
		// very good! set connection state to "ESTABLISHED"
		status = set_tcp_conn_state(irps->FileObject, TCP_STATE_ESTABLISHED_OUT);
		if (status != STATUS_SUCCESS) {
			KdPrint(("[tdi_fw] tdi_connect_complete: set_tcp_conn_state: 0x%x!\n", status));

			// set fake status
			Irp->IoStatus.Status = STATUS_REMOTE_NOT_LISTENING;
			// TDI client will close connection object and connection will not "hang" (maybe)
			goto done;
		}

		// and update local address for this connection in state table
		update_conn_info(DeviceObject, irps->FileObject);

	} else {

		if (request != NULL) {

			switch (Irp->IoStatus.Status) {		// are status codes correct?
			case STATUS_CONNECTION_REFUSED:
			case STATUS_CONNECTION_RESET:
				request->type = TYPE_CONNECT_RESET;
				break;
			case STATUS_CONNECTION_ABORTED:
			case STATUS_CANCELLED:
				request->type = TYPE_CONNECT_CANCELED;
				break;
			case STATUS_IO_TIMEOUT:
				request->type = TYPE_CONNECT_TIMEOUT;
				break;
			case STATUS_NETWORK_UNREACHABLE:
			case STATUS_HOST_UNREACHABLE:
			case STATUS_PROTOCOL_UNREACHABLE:
			case STATUS_PORT_UNREACHABLE:
				request->type = TYPE_CONNECT_UNREACH;
				break;
			default:
				request->type = TYPE_CONNECT_ERROR;
			}

			// anyway save status
			request->status = Irp->IoStatus.Status;

			log_request(request);
		}

		del_tcp_conn(irps->FileObject, FALSE);
	}

done:
	if (request != NULL) {
		if (request->sid_a != NULL)
			free(request->sid_a);
		free(request);
	}
	return tdi_generic_complete(DeviceObject, Irp, Context);
}

void
delayed_ucn(PVOID p)
{
	struct delayed_ucn_param *ucn_param = (struct delayed_ucn_param *)p;

	update_conn_info(ucn_param->devobj, ucn_param->fileobj);
	
	free(ucn_param);
}

/* query local address and port for connection */
void
update_conn_info(PDEVICE_OBJECT devobj, PFILE_OBJECT connobj)
{
	PIRP query_irp;
	PMDL mdl = NULL;
	struct uci_param *uci_param = NULL;

	// MUST be executed at PASSIVE_LEVEL

	if (KeGetCurrentIrql() != PASSIVE_LEVEL) {
		// do it a bit later :-)
		struct delayed_ucn_param *ucn_param = (struct delayed_ucn_param *)malloc_np(sizeof(*ucn_param));
		if (ucn_param != NULL) {

			memset(ucn_param, 0, sizeof(*ucn_param));

			ucn_param->devobj = devobj;
			ucn_param->fileobj = connobj;

			ExInitializeWorkItem(&ucn_param->item, delayed_ucn, ucn_param);
			ExQueueWorkItem(&ucn_param->item, DelayedWorkQueue);	// DelayedWorkQueue a good value?

		} else {
			KdPrint(("[ndis_hk] tdi_connect_complete: malloc_np!\n"));
			// so we'll live without known local address :-(
		}
		return;
	}

	// we're at PASSIVE_LEVEL

	query_irp = TdiBuildInternalDeviceControlIrp(TDI_QUERY_INFORMATION, devobj, connobj, NULL, NULL);
	if (query_irp == NULL) {
		KdPrint(("[tdi_fw] update_conn_info: TdiBuildInternalDeviceControlIrp!\n"));
		goto done;
	}

	uci_param = (struct uci_param *)malloc_np(sizeof(*uci_param) + TDI_ADDRESS_INFO_MAX);
	if (uci_param == NULL) {
		KdPrint(("[tdi_fw] update_conn_info: malloc_np!\n"));
		goto done;
	}

	memset(uci_param, 0, sizeof(*uci_param) + TDI_ADDRESS_INFO_MAX);
	uci_param->connobj = connobj;

	mdl = IoAllocateMdl(uci_param->address, TDI_ADDRESS_INFO_MAX, FALSE, FALSE, NULL);
	if (mdl == NULL) {
		KdPrint(("[tdi_fw] update_conn_info: IoAllocateMdl!\n"));
		goto done;
	}
	MmBuildMdlForNonPagedPool(mdl);

	TdiBuildQueryInformation(query_irp, devobj, connobj,
		update_conn_info_complete, uci_param,
		TDI_QUERY_ADDRESS_INFO, mdl);

	IoCallDriver(devobj, query_irp);

	query_irp = NULL;
	mdl = NULL;
	uci_param = NULL;

done:
	// cleanup
	if (mdl != NULL)
		IoFreeMdl(mdl);
	if (uci_param != NULL)
		ExFreePool(uci_param);
	if (query_irp != NULL)
		IoCompleteRequest(query_irp, IO_NO_INCREMENT);
}

NTSTATUS
update_conn_info_complete(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp, IN PVOID Context)
{
	struct uci_param *param = (struct uci_param *)Context;
	TA_ADDRESS *addr = ((TDI_ADDRESS_INFO *)(param->address))->Address.Address;
	NTSTATUS status;

	status = set_tcp_conn_local(param->connobj, addr);
	if (status != STATUS_SUCCESS)
		KdPrint(("[tdi_fw] update_conn_info_complete: set_tcp_conn_local: 0x%x!\n", status));

	// cleanup MDL to avoid unlocking pages from NonPaged pool
	if (Irp->MdlAddress != NULL) {
		IoFreeMdl(Irp->MdlAddress);
		Irp->MdlAddress = NULL;
	}

	free(param);
	return STATUS_SUCCESS;
}

//----------------------------------------------------------------------------

/*
 * TDI_DISCONNECT handler
 */

int
tdi_disconnect(PIRP irp, PIO_STACK_LOCATION irps, struct completion *completion)
{
	TDI_REQUEST_KERNEL_DISCONNECT *param = (TDI_REQUEST_KERNEL_DISCONNECT *)(&irps->Parameters);

	KdPrint(("[tdi_fw] tdi_disconnect: connobj 0x%x (flags: 0x%x)\n",
		irps->FileObject, param->RequestFlags));

	if (param->RequestFlags & TDI_DISCONNECT_RELEASE) {
		int state = get_tcp_conn_state_by_obj(irps->FileObject), new_state;

		if (state == TCP_STATE_ESTABLISHED_IN || state == TCP_STATE_ESTABLISHED_OUT)
			new_state = TCP_STATE_FIN_WAIT1;
		else if (state == TCP_STATE_CLOSE_WAIT)
			new_state = TCP_STATE_LAST_ACK;
		else
			KdPrint(("[tdi_fw] tdi_disconnect: weird conn state: %d\n", state));

		set_tcp_conn_state(irps->FileObject, new_state);

		completion->routine = tdi_disconnect_complete;
		completion->context = (PVOID)new_state;

	} else {

		// set TCP_STATE_CLOSED and delete object in completion

		set_tcp_conn_state(irps->FileObject, TCP_STATE_CLOSED);

		completion->routine = tdi_disconnect_complete;
		completion->context = (PVOID)TCP_STATE_CLOSED;

	}

	return FILTER_ALLOW;
}

NTSTATUS
tdi_disconnect_complete(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp, IN PVOID Context)
{
	PIO_STACK_LOCATION irps = IoGetCurrentIrpStackLocation(Irp);
	int state = (int)Context;

	KdPrint(("[tdi_fw] tdi_disconnect_complete: connobj 0x%x; status: 0x%x\n",
		irps->FileObject, Irp->IoStatus.Status));

	if (Irp->IoStatus.Status == STATUS_SUCCESS) {
		
		// update TCP state table
		
		if (state == TCP_STATE_FIN_WAIT1)
			set_tcp_conn_state(irps->FileObject, TCP_STATE_FIN_WAIT2);
		else if (state == TCP_STATE_LAST_ACK)
			del_tcp_conn(irps->FileObject, TRUE);
		else if (state == TCP_STATE_CLOSED)
			del_tcp_conn(irps->FileObject, TRUE);
		else
			KdPrint(("[tdi_fw] tdi_disconnect_complete: weird conn state: %d\n", state));
	}

	return tdi_generic_complete(DeviceObject, Irp, Context);
}
