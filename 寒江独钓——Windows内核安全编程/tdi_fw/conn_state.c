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
// $Id: conn_state.c,v 1.1 2009/04/28 12:53:27 tanwen Exp $

#include <ntddk.h>
#include <tdikrnl.h>
#include "sock.h"

#include "conn_state.h"
#include "ipc.h"
#include "memtrack.h"
#include "obj_tbl.h"
#include "tdi_fw.h"

// how much entry in connection will live in "CLOSED" state? (sec)
#define MAX_CLOSED_TIME		20

struct listen_entry {
	struct			listen_entry *next;
	struct			listen_entry *prev;		/* using double-linked list */
	int				ipproto;
	ULONG			addr;		// IPv4 only (yet)
	USHORT			port;
	PFILE_OBJECT	addrobj;
};

static struct listen_entry **g_listen = NULL;

static KSPIN_LOCK g_listen_guard;
// !!! to avoid deadlocks with g_ot_hash_guard this spinlock MUST be acquired _after_ g_conn_guard !!!

#define LISTEN_HASH_SIZE	0x1000
#define CALC_LISTEN_HASH(ipproto, port)	((ULONG)((ipproto) + (port)) % LISTEN_HASH_SIZE)

struct conn_entry {
	struct			conn_entry *next;
	struct			conn_entry *prev;		/* using double-linked list */
	int				state;
	ULONG			laddr;		// IPv4 only (yet)
	USHORT			lport;
	ULONG			raddr;
	USHORT			rport;
	PFILE_OBJECT	connobj;

	struct			conn_entry *next_to_del;
	LARGE_INTEGER	ticks;
};

static struct conn_entry **g_conn = NULL;
static struct conn_entry *g_conn_to_del = NULL;
static KSPIN_LOCK g_conn_guard;
static KEVENT g_conn_new_to_del;
static HANDLE g_conn_thread;

#define CONN_HASH_SIZE	0x1000
#define CALC_CONN_HASH(laddr, lport, raddr, rport)	((ULONG)((laddr) + (lport) + (raddr) + (rport)) % CONN_HASH_SIZE)

static void		conn_thread(PVOID param);

//----------------------------------------------------------------------------

NTSTATUS
conn_state_init(void)
{
	NTSTATUS status;

	KeInitializeSpinLock(&g_listen_guard);

	g_listen = (struct listen_entry **)malloc_np(sizeof(struct listen_entry *) * LISTEN_HASH_SIZE);
	if (g_listen == NULL) {
		status = STATUS_INSUFFICIENT_RESOURCES;
		goto done;
	}

	memset(g_listen, 0, sizeof(struct listen_entry *) * LISTEN_HASH_SIZE);

	KeInitializeSpinLock(&g_conn_guard);

	g_conn = (struct conn_entry **)malloc_np(sizeof(struct conn_entry *) * CONN_HASH_SIZE);
	if (g_conn == NULL) {
		status = STATUS_INSUFFICIENT_RESOURCES;
		goto done;
	}

	memset(g_conn, 0, sizeof(struct conn_entry *) * CONN_HASH_SIZE);

	KeInitializeEvent(&g_conn_new_to_del, SynchronizationEvent, FALSE);

	// create thread to delete g_conn_to_del entries on timeout
	status = PsCreateSystemThread(&g_conn_thread, THREAD_ALL_ACCESS, NULL, NULL, NULL, conn_thread, NULL);

done:
	if (status != STATUS_SUCCESS) {
		if (g_listen != NULL)
			free(g_listen);
		if (g_conn != NULL)
			free(g_conn);
	}
	return status;
}

// this function MUST be executed after ot_free()
void
conn_state_free(void)
{
	HANDLE thread = g_conn_thread;

	g_conn_thread = NULL;	// signal for thread to cleanup & exit
	KeSetEvent(&g_conn_new_to_del, 0, FALSE);
	ZwWaitForSingleObject(thread, FALSE, NULL);
	ZwClose(thread);

	if (g_listen != NULL) {
		free(g_listen);
		g_listen = NULL;
	}

	if (g_conn != NULL) {
		free(g_conn);
		g_conn = NULL;
	}
}

//----------------------------------------------------------------------------

NTSTATUS
add_listen(struct ot_entry *ote_addr)
{
	TA_ADDRESS *address = (TA_ADDRESS *)(ote_addr->local_addr);
	struct listen_entry *le;
	KIRQL irql;
	ULONG hash;

	if (address->AddressType != TDI_ADDRESS_TYPE_IP)
		return STATUS_INVALID_PARAMETER;

	le = (struct listen_entry *)malloc_np(sizeof(*le));
	if (le == NULL)
		return STATUS_INSUFFICIENT_RESOURCES;

	memset(le, 0, sizeof(*le));

	le->addrobj = ote_addr->fileobj;
	le->addr = ((TDI_ADDRESS_IP *)(address->Address))->in_addr;
	le->port = ((TDI_ADDRESS_IP *)(address->Address))->sin_port;
	le->ipproto = ote_addr->ipproto;

	KdPrint(("[tdi_fw] add_list: got LISTEN %x:%u (ipproto=%d)\n", le->addr, ntohs(le->port), le->ipproto));

	// save le in ote

	if (ote_addr->listen_entry != NULL) {
		KdPrint(("[tdi_fw] add_listen: duplicate listen for addrobj!\n"));

		free(le);
		return STATUS_OBJECT_NAME_EXISTS;
	}

	ote_addr->listen_entry = le;

	// add to our hash

	hash = CALC_LISTEN_HASH(le->ipproto, le->port);

	KeAcquireSpinLock(&g_listen_guard, &irql);
	
	le->next = g_listen[hash];
	if (g_listen[hash] != NULL)
		g_listen[hash]->prev = le;
	g_listen[hash] = le;

	KeReleaseSpinLock(&g_listen_guard, irql);

	return STATUS_SUCCESS;
}

void
del_listen_obj(struct listen_entry *le, BOOLEAN no_guard)
{
	KIRQL irql;

	KdPrint(("[tdi_fw] del_listen_obj: NOT_LISTEN %x:%u (ipproto=%d)\n", le->addr, ntohs(le->port), le->ipproto));

	if (!no_guard)
		KeAcquireSpinLock(&g_listen_guard, &irql);	// lock our hash

	// delete le from our hash
	
	if (le->prev != NULL)
		le->prev->next = le->next;
	else {
		ULONG hash = CALC_LISTEN_HASH(le->ipproto, le->port);
		g_listen[hash] = le->next;
	}
	
	if (le->next != NULL)
		le->next->prev = le->prev;

	free(le);

	if (!no_guard)
		KeReleaseSpinLock(&g_listen_guard, irql);	// unlock our hash
}

BOOLEAN
is_listen(ULONG addr, USHORT port, int ipproto)
{
	ULONG hash = CALC_LISTEN_HASH(ipproto, port);
	struct listen_entry *le;
	KIRQL irql;
	BOOLEAN result = FALSE;

	KeAcquireSpinLock(&g_listen_guard, &irql);

	for (le = g_listen[hash]; le != NULL; le = le->next) {
		if (le->ipproto == ipproto && le->addr == addr && le->port == port) {
			result = TRUE;
			break;
		}
	}

	KeReleaseSpinLock(&g_listen_guard, irql);
	return result;
}

// another UGLY solution for broadcasts :-(
BOOLEAN
is_bcast_listen(ULONG addr, USHORT port, int ipproto)
{
	ULONG hash = CALC_LISTEN_HASH(ipproto, port);
	struct listen_entry *le;
	KIRQL irql;
	BOOLEAN result = FALSE;

	KeAcquireSpinLock(&g_listen_guard, &irql);

	for (le = g_listen[hash]; le != NULL; le = le->next) {
		if (le->ipproto == ipproto && le->port == port) {
			ULONG addr_l = ntohl(le->addr), addr_p = ntohl(addr);
			int i;
			
			result = TRUE;

			for (i = 31; i >= 0; i--)
				if ((addr_l & (1 << i)) != (addr_p & (1 << i)) && (addr_p & (1 << i)) == 0) {
					result = FALSE;
					break;
				}

			if (result)
				break;
		}
	}

	KeReleaseSpinLock(&g_listen_guard, irql);
	return result;
}

NTSTATUS
enum_listen(struct listen_nfo *buf, ULONG *buf_len, ULONG buf_size)
{
	NTSTATUS status = STATUS_SUCCESS;
	KIRQL irql;
	ULONG hash;
	struct listen_entry *le;

	*buf_len = 0;
	
	if (buf_size < sizeof(struct listen_nfo))
		return STATUS_INVALID_PARAMETER;

	KeAcquireSpinLock(&g_ot_hash_guard, &irql);		// lock obj_tbl (avoid deadlocks!)
	KeAcquireSpinLockAtDpcLevel(&g_listen_guard);	// lock our hash

	for (hash = 0; hash < LISTEN_HASH_SIZE; hash++) {
		for (le = g_listen[hash]; le != NULL; le = le->next) {
			struct ot_entry *ote;

			buf->addr = le->addr;
			buf->port = le->port;
			buf->ipproto = le->ipproto;
			
			// try to get pid
			ote = ot_find_fileobj(le->addrobj, NULL);	// g_ot_hash_guard already acquired
			if (ote != NULL)
				buf->pid = ote->pid;
			else
				buf->pid = 0;

			*buf_len += sizeof(struct listen_nfo);
			buf++;

			if (*buf_len + sizeof(struct listen_nfo) > buf_size) {
				status = STATUS_BUFFER_TOO_SMALL;
				break;
			}
		}

		if (status != STATUS_SUCCESS)
			break;
	}
	
	KeReleaseSpinLockFromDpcLevel(&g_listen_guard);	// unlock our hash
	KeReleaseSpinLock(&g_ot_hash_guard, irql);		// unlock obj_tbl
	
	return status;
}

//----------------------------------------------------------------------------

NTSTATUS
add_tcp_conn(struct ot_entry *ote_conn, int tcp_state)
{
	TA_ADDRESS *local_addr = (TA_ADDRESS *)(ote_conn->local_addr),
		*remote_addr = (TA_ADDRESS *)(ote_conn->remote_addr);
	struct conn_entry *ce;
	KIRQL irql;
	ULONG hash;

	if (local_addr->AddressType != TDI_ADDRESS_TYPE_IP || remote_addr->AddressType != TDI_ADDRESS_TYPE_IP)
		return STATUS_INVALID_PARAMETER;

	ce = (struct conn_entry *)malloc_np(sizeof(*ce));
	if (ce == NULL)
		return STATUS_INSUFFICIENT_RESOURCES;

	memset(ce, 0, sizeof(*ce));

	ce->connobj = ote_conn->fileobj;
	ce->laddr = ((TDI_ADDRESS_IP *)(local_addr->Address))->in_addr;
	ce->lport = ((TDI_ADDRESS_IP *)(local_addr->Address))->sin_port;
	ce->raddr = ((TDI_ADDRESS_IP *)(remote_addr->Address))->in_addr;
	ce->rport = ((TDI_ADDRESS_IP *)(remote_addr->Address))->sin_port;
	ce->state = tcp_state;

	KdPrint(("[tdi_fw] add_tcp_conn: got CONNECT %x:%u <-> %x:%u (state=%d)\n",
		ce->laddr, ntohs(ce->lport), ce->raddr, ntohs(ce->rport), tcp_state));

	// save ce in ote

	if (ote_conn->conn_entry != NULL) {
        KdPrint(("[tdi_fw] add_conn: duplicate conn (0x%x:%u -> 0x%x:%u)!\n",
            ote_conn->conn_entry->laddr, ote_conn->conn_entry->lport,
            ote_conn->conn_entry->raddr, ote_conn->conn_entry->rport));

		free(ce);
		return STATUS_OBJECT_NAME_EXISTS;
	}

	ote_conn->conn_entry = ce;

	// add to our hash

	hash = CALC_CONN_HASH(ce->laddr, ce->lport, ce->raddr, ce->rport);

	KeAcquireSpinLock(&g_conn_guard, &irql);
	
	ce->next = g_conn[hash];
	if (g_conn[hash] != NULL)
		g_conn[hash]->prev = ce;
	g_conn[hash] = ce;

	KeReleaseSpinLock(&g_conn_guard, irql);

	return STATUS_SUCCESS;
}

void
del_tcp_conn(PFILE_OBJECT connobj, BOOLEAN is_disconnect)
{
	KIRQL irql;
	NTSTATUS status;
	struct ot_entry *ote_conn;
	struct conn_entry *ce;

	KeAcquireSpinLock(&g_ot_hash_guard, &irql);	// lock obj_tbl first (avoid deadlocks!)
	KeAcquireSpinLockAtDpcLevel(&g_conn_guard);	// lock our hash

	ote_conn = ot_find_fileobj(connobj, NULL);		// obj_tbl already locked
	if (ote_conn == NULL)
		goto done;

	ce = ote_conn->conn_entry;
	ote_conn->conn_entry = NULL;

	if (is_disconnect && ote_conn->log_disconnect)
		log_disconnect(ote_conn);

	if (ce != NULL)
		del_tcp_conn_obj(ce, TRUE);

done:
	KeReleaseSpinLockFromDpcLevel(&g_conn_guard);	// unlock our hash
	KeReleaseSpinLock(&g_ot_hash_guard, irql);	// unlock obj_tbl
}

void
del_tcp_conn_obj(struct conn_entry *ce, BOOLEAN no_guard)
{
	KIRQL irql;

	KdPrint(("[tdi_fw] del_tcp_conn_obj: CLOSED %x:%u <-> %x:%u (state=%d)\n",
		ce->laddr, ntohs(ce->lport), ce->raddr, ntohs(ce->rport), ce->state));

	if (!no_guard)
		KeAcquireSpinLock(&g_conn_guard, &irql);	// lock our hash

	// set state to TCP_STATE_CLOSED and add to special list to queue it for deleting
	ce->state = TCP_STATE_CLOSED;
	ce->next_to_del = g_conn_to_del;
	g_conn_to_del = ce;

	KeQueryTickCount(&ce->ticks);

	KeSetEvent(&g_conn_new_to_del, IO_NO_INCREMENT, FALSE);

	ce->connobj = NULL;		// no connection object related for now!!!

	KdPrint(("[tdi_fw] del_tcp_conn_obj: state table entry scheduled for deletion!\n"));

	if (!no_guard)
		KeReleaseSpinLock(&g_conn_guard, irql);	// unlock our hash
}

NTSTATUS
set_tcp_conn_state(PFILE_OBJECT connobj, int state)
{
	KIRQL irql;
	struct ot_entry *ote_conn;
	NTSTATUS status;

	KeAcquireSpinLock(&g_ot_hash_guard, &irql);	// lock obj_tbl first (avoid deadlocks!)
	KeAcquireSpinLockAtDpcLevel(&g_conn_guard);	// lock our hash

	ote_conn = ot_find_fileobj(connobj, NULL);		// obj_tbl already locked
	if (ote_conn == NULL) {
		KdPrint(("[tdi_fw] set_tcp_conn_state: ot_find_fileobj(0x%x)!\n", connobj));
		status = STATUS_OBJECT_NAME_NOT_FOUND;
		goto done;
	}

	if (ote_conn->conn_entry != NULL) {
		ote_conn->conn_entry->state = state;

		KdPrint(("[tdi_fw] set_tcp_conn_state: got CHANGE CONNECT STATE %x:%u <-> %x:%u (state=%d)\n",
			ote_conn->conn_entry->laddr, ntohs(ote_conn->conn_entry->lport),
			ote_conn->conn_entry->raddr, ntohs(ote_conn->conn_entry->rport),
			ote_conn->conn_entry->state));

		status = STATUS_SUCCESS;
	} else {
		KdPrint(("[tdi_fw] set_tcp_conn_state: no conn_entry!\n"));
		status = STATUS_INVALID_PARAMETER;
	}

done:
	KeReleaseSpinLockFromDpcLevel(&g_conn_guard);		// unlock our hash
	KeReleaseSpinLock(&g_ot_hash_guard, irql);		// unlock obj_tbl
	
	return status;
}

NTSTATUS
set_tcp_conn_local(PFILE_OBJECT connobj, TA_ADDRESS *local)
{
	KIRQL irql;
	struct ot_entry *ote_conn;
	NTSTATUS status;

	KeAcquireSpinLock(&g_ot_hash_guard, &irql);	// lock obj_tbl first (avoid deadlocks!)
	KeAcquireSpinLockAtDpcLevel(&g_conn_guard);	// lock our hash

	ote_conn = ot_find_fileobj(connobj, NULL);		// obj_tbl already locked
	if (ote_conn == NULL) {
		KdPrint(("[tdi_fw] set_tcp_conn_local: ot_find_fileobj(0x%x)!\n", connobj));
		status = STATUS_OBJECT_NAME_NOT_FOUND;
		goto done;
	}

	if (ote_conn->conn_entry != NULL) {
		struct conn_entry *ce = ote_conn->conn_entry;
		ULONG hash;

		// remove our conn_entry from list and recalculate hash

		if (ce->prev != NULL)
			ce->prev->next = ce->next;
		else {
			hash = CALC_CONN_HASH(ce->laddr, ce->lport, ce->raddr, ce->rport);
			g_conn[hash] = ce->next;
		}
		
		if (ce->next != NULL)
			ce->next->prev = ce->prev;

		ce->laddr = ((TDI_ADDRESS_IP *)(local->Address))->in_addr;
		ce->lport = ((TDI_ADDRESS_IP *)(local->Address))->sin_port;

		hash = CALC_CONN_HASH(ce->laddr, ce->lport, ce->raddr, ce->rport);

		KdPrint(("[tdi_fw] set_tcp_conn_local: got CHANGE CONNECT LOCAL %x:%u <-> %x:%u (state=%d)\n",
			ce->laddr, ntohs(ce->lport), ce->raddr, ntohs(ce->rport), ce->state));

		// and now add our conn_entry under new hash value

		ce->next = g_conn[hash];
		if (g_conn[hash] != NULL)
			g_conn[hash]->prev = ce;
		g_conn[hash] = ce;

		ce->prev = NULL;

		status = STATUS_SUCCESS;

	} else {
		KdPrint(("[tdi_fw] set_tcp_conn_local: no conn_entry!\n"));
		status = STATUS_INVALID_PARAMETER;
	}

done:
	KeReleaseSpinLockFromDpcLevel(&g_conn_guard);		// unlock our hash
	KeReleaseSpinLock(&g_ot_hash_guard, irql);		// unlock obj_tbl
	
	return status;
}

int
get_tcp_conn_state(ULONG laddr, USHORT lport, ULONG raddr, USHORT rport)
{
	ULONG hash = CALC_CONN_HASH(laddr, lport, raddr, rport);
	KIRQL irql;
	struct conn_entry *ce;
	int result;

	KeAcquireSpinLock(&g_conn_guard, &irql);

	result = TCP_STATE_NONE;

	for (ce = g_conn[hash]; ce != NULL; ce = ce->next)
		if (ce->laddr == laddr && ce->lport == lport &&
			ce->raddr == raddr && ce->rport == rport) {

			result = ce->state;
			break;
		}

	KeReleaseSpinLock(&g_conn_guard, irql);

	return result;
}

int
get_tcp_conn_state_by_obj(PFILE_OBJECT connobj)
{
	KIRQL irql;
	struct ot_entry *ote_conn;
	NTSTATUS status;
	int result = TCP_STATE_NONE;

	KeAcquireSpinLock(&g_ot_hash_guard, &irql);	// lock obj_tbl first (avoid deadlocks!)
	KeAcquireSpinLockAtDpcLevel(&g_conn_guard);	// lock our hash

	ote_conn = ot_find_fileobj(connobj, NULL);		// obj_tbl already locked
	if (ote_conn == NULL) {
		KdPrint(("[tdi_fw] set_tcp_conn_state: ot_find_fileobj(0x%x)!\n", connobj));
		goto done;
	}

	if (ote_conn->conn_entry != NULL)
		result = ote_conn->conn_entry->state;

done:
	KeReleaseSpinLockFromDpcLevel(&g_conn_guard);	// unlock our hash
	KeReleaseSpinLock(&g_ot_hash_guard, irql);	// unlock obj_tbl
	
	return result;
}

NTSTATUS
enum_tcp_conn(struct tcp_conn_nfo *buf, ULONG *buf_len, ULONG buf_size)
{
	NTSTATUS status = STATUS_SUCCESS;
	KIRQL irql;
	ULONG hash;
	struct conn_entry *ce;

	*buf_len = 0;
	
	if (buf_size < sizeof(struct tcp_conn_nfo))
		return STATUS_INVALID_PARAMETER;

	KeAcquireSpinLock(&g_ot_hash_guard, &irql);	// lock obj_tbl first (avoid deadlocks!)
	KeAcquireSpinLockAtDpcLevel(&g_conn_guard);	// lock our hash

	for (hash = 0; hash < CONN_HASH_SIZE; hash++) {
		for (ce = g_conn[hash]; ce != NULL; ce = ce->next) {
			struct ot_entry *ote;

			if (ce->state == TCP_STATE_CLOSED)
				continue;		// don't log "closed"

			buf->laddr = ce->laddr;
			buf->lport = ce->lport;
			buf->raddr = ce->raddr;
			buf->rport = ce->rport;
			buf->state = ce->state;
			
			// try to get pid
			ote = ot_find_fileobj(ce->connobj, NULL);		// obj_tbl already locked
			if (ote != NULL) {
				buf->pid = ote->pid;
				buf->bytes_in = ote->bytes_in;
				buf->bytes_out = ote->bytes_out;
			} else {
				buf->pid = 0;
				buf->bytes_in = 0;
				buf->bytes_out = 0;
			}

			*buf_len += sizeof(struct tcp_conn_nfo);
			buf++;

			if (*buf_len + sizeof(struct tcp_conn_nfo) > buf_size) {
				status = STATUS_BUFFER_TOO_SMALL;
				break;
			}
		}

		if (status != STATUS_SUCCESS)
			break;
	}
	
	KeReleaseSpinLockFromDpcLevel(&g_conn_guard);	// unlock our_hash
	KeReleaseSpinLock(&g_ot_hash_guard, irql);	// unlock obj_tbl
	
	return status;
}

void
conn_thread(PVOID param)
{
	LARGE_INTEGER timeout;

	timeout.QuadPart = 0;

	do {
		LARGE_INTEGER ticks;
		struct conn_entry *ce, *prev_ce, *ce2;
		KIRQL irql;

		KeWaitForSingleObject(&g_conn_new_to_del, Executive, KernelMode, FALSE,
			(timeout.QuadPart != 0) ? &timeout : NULL);

		KdPrint(("[tdi_fw] conn_thread: wake up!\n"));

		KeQueryTickCount(&ticks);

		// delete all entries older than MAX_CLOSED_TIME sec
		// or all entries if (g_conn_thread == NULL)

		// and also find min timeout to next entry

		KeAcquireSpinLock(&g_conn_guard, &irql);

		prev_ce = NULL;
		timeout.QuadPart = 0;
		for (ce = g_conn_to_del; ce != NULL;) {
			__int64 delta = (ticks.QuadPart - ce->ticks.QuadPart) * KeQueryTimeIncrement();

			KdPrint(("[tdi_fw] conn_thread: delta %d msec\n", delta / 10000));

			if (delta >= MAX_CLOSED_TIME * 1000 * 10000 ||
				g_conn_thread == NULL) {
			
				KdPrint(("[tdi_fw] conn_thread: remove it!\n"));

				// remove this entry!!!

				if (prev_ce == NULL)
					g_conn_to_del = ce->next_to_del;
				else
					prev_ce->next_to_del = ce->next_to_del;

				// delete ce from our hash
				
				if (ce->prev != NULL)
					ce->prev->next = ce->next;
				else {
					ULONG hash = CALC_CONN_HASH(ce->laddr, ce->lport, ce->raddr, ce->rport);
					g_conn[hash] = ce->next;
				}
				
				if (ce->next != NULL)
					ce->next->prev = ce->prev;

				ce2 = ce->next_to_del;
				free(ce);
			
				ce = ce2;

			} else {
				delta = (MAX_CLOSED_TIME * 1000 * 10000 - delta);

				if (delta > -timeout.QuadPart || timeout.QuadPart == 0) {
					timeout.QuadPart = -delta;	// how much to wait?

					KdPrint(("[tdi_fw] conn_thread: we must wait %d ms!\n", timeout.QuadPart / 10000));
				}

				prev_ce = ce;
				ce = ce->next_to_del;
			}
		}

		KeReleaseSpinLock(&g_conn_guard, irql);

	} while(g_conn_thread != NULL);
}

void
log_disconnect(struct ot_entry *ote_conn)
{
	TA_ADDRESS *local_addr, *remote_addr;
	struct flt_request request;

	local_addr = (TA_ADDRESS *)(ote_conn->local_addr);
	remote_addr = (TA_ADDRESS *)(ote_conn->remote_addr);

	KdPrint(("[tdi_flt] del_tcp_conn: %x:%u -> %x:%u\n",
		ntohl(((TDI_ADDRESS_IP *)(local_addr->Address))->in_addr),
		ntohs(((TDI_ADDRESS_IP *)(local_addr->Address))->sin_port),
		ntohl(((TDI_ADDRESS_IP *)(remote_addr->Address))->in_addr),
		ntohs(((TDI_ADDRESS_IP *)(remote_addr->Address))->sin_port)));

	memset(&request, 0, sizeof(request));

	request.struct_size = sizeof(request);

	request.result = FILTER_DISCONNECT;
	request.proto = ote_conn->ipproto;
	request.direction = DIRECTION_ANY;

	request.pid = (ULONG)-1;		// don't use pid because on close there's no info in database

	// get user SID & attributes!
	if ((request.sid_a = copy_sid_a(ote_conn->sid_a, ote_conn->sid_a_size)) != NULL)
		request.sid_a_size = ote_conn->sid_a_size;

	memcpy(&request.addr.from, &local_addr->AddressType, sizeof(struct sockaddr));
	memcpy(&request.addr.to, &remote_addr->AddressType, sizeof(struct sockaddr));
	request.addr.len = sizeof(struct sockaddr_in);

	request.log_bytes_in = ote_conn->bytes_in;
	request.log_bytes_out = ote_conn->bytes_out;
	
	log_request(&request);
}
