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
// $Id: filter.c,v 1.1 2009/04/28 12:53:28 tanwen Exp $

/*
 * Filtering related routines
 */

#include <ntddk.h>
#include <tdikrnl.h>
#include "sock.h"

#include "filter.h"
#include "memtrack.h"
#include "packet.h"
#include "pid_pname.h"
#include "sids.h"
#include "tdi_fw.h"

// size of cyclic queue for logging
#define REQUEST_QUEUE_SIZE	1024

/* rules chains (main (first entry) and process-related) */
static struct {
	struct {
		struct		flt_rule *head;
		struct		flt_rule *tail;
		char		*pname;				// name of process
		BOOLEAN		active;				// filter chain is active
	} chain[MAX_CHAINS_COUNT];
	KSPIN_LOCK	guard;
} g_rules;

/* "ALLOW * * FROM ANY TO ANY" rule */
static struct flt_rule g_allow_all = {
	{0},
	FILTER_ALLOW,
	IPPROTO_ANY,
	DIRECTION_ANY,
	0,	// from
	0,
	0,
	0,
	0,	// to
	0,
	0,
	0,
	RULE_LOG_LOG,
	"",	// setup mask before using it!
	"startup"		// rule for startup only
};

/* logging request queue */
static struct {
	struct		flt_request *data;
	KSPIN_LOCK	guard;
	ULONG		head;	/* write to head */
	ULONG		tail;	/* read from tail */
	HANDLE		event_handle;
	PKEVENT		event;
} g_queue;

// init
NTSTATUS
filter_init(void)
{
	NTSTATUS status;
	int i;

	pid_pname_init();
	sids_init();

	/* rules chain */
	
	KeInitializeSpinLock(&g_rules.guard);
	for (i = 0; i < MAX_CHAINS_COUNT; i++) {
		g_rules.chain[i].head = g_rules.chain[i].tail = NULL;
		g_rules.chain[i].pname = NULL;
		g_rules.chain[i].active = FALSE;
	}

	// setup the first rule "ALLOW * * FROM ANY TO ANY"

	for (i = 0; i < sizeof(g_allow_all.sid_mask); i++)
		g_allow_all.sid_mask[i] = (UCHAR)-1;

	g_rules.chain[0].head = malloc_np(sizeof(g_allow_all));
	if (g_rules.chain[0].head == NULL) {
		KdPrint(("[tdi_fw] filter_init: malloc_np!\n"));
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	memcpy(g_rules.chain[0].head, &g_allow_all, sizeof(g_allow_all));

	g_rules.chain[0].tail = g_rules.chain[0].head;
	g_rules.chain[0].active = TRUE;

	/* request queue */
	
	KeInitializeSpinLock(&g_queue.guard);

	g_queue.data = (struct flt_request *)malloc_np(sizeof(struct flt_request) * REQUEST_QUEUE_SIZE);
	if (g_queue.data == NULL) {
		KdPrint(("[tdi_fw] filter_init: malloc_np!\n"));
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	memset(g_queue.data, 0, sizeof(struct flt_request) * REQUEST_QUEUE_SIZE);

	g_queue.head = g_queue.tail = 0;

	return STATUS_SUCCESS;
}

// init for user part starting
NTSTATUS
filter_init_2(void)
{
	NTSTATUS status;

	if (g_queue.event_handle == NULL) {
		UNICODE_STRING str;
		OBJECT_ATTRIBUTES oa;

		RtlInitUnicodeString(&str, L"\\BaseNamedObjects\\tdifw_request");
		InitializeObjectAttributes(&oa, &str, 0, NULL, NULL);

		status = ZwCreateEvent(&g_queue.event_handle, EVENT_ALL_ACCESS, &oa, SynchronizationEvent, FALSE);
		if (status != STATUS_SUCCESS) {
			KdPrint(("[tdi_fw] filter_init_2: ZwCreateEvent: 0x%x\n", status));
			return status;
		}

	}

	if (g_queue.event == NULL) {
		status = ObReferenceObjectByHandle(g_queue.event_handle, EVENT_ALL_ACCESS, NULL, KernelMode,
			&g_queue.event, NULL);
		if (status != STATUS_SUCCESS) {
			KdPrint(("[tdi_fw] filter_init_2: ObReferenceObjectByHandle: 0x%x\n", status));
			return status;
		}
	
	}

	// try to communicate with packet driver
	init_packet();

	return STATUS_SUCCESS;
}

// cleanup for user part
void
filter_free_2(void)
{
	free_packet();

	if (g_queue.event != NULL) {
		ObDereferenceObject(g_queue.event);
		g_queue.event = NULL;
	}
	if (g_queue.event_handle != NULL) {
		ZwClose(g_queue.event_handle);
		g_queue.event_handle = NULL;
	}
}

// free
void
filter_free(void)
{
	KIRQL irql;
	struct plist_entry *ple;
	int i;

	// clear all chains
	for (i = 0; i < MAX_CHAINS_COUNT; i++)
		clear_flt_chain(i);

	/* clear request queue */
	KeAcquireSpinLock(&g_queue.guard, &irql);
	for (i = 0; i < REQUEST_QUEUE_SIZE; i++) {
		if (g_queue.data[i].pname != NULL)
			free(g_queue.data[i].pname);
		if (g_queue.data[i].sid_a != NULL)
			free(g_queue.data[i].sid_a);
	}
	free(g_queue.data);
	KeReleaseSpinLock(&g_queue.guard, irql);

	set_sid_list(NULL, 0);
	pid_pname_free();
}

#define CHECK_BIT(char_mask, num)	\
	((char_mask)[(num) / 8] & (1 << ((num) % 8)))

// quick filter (I mean "synchronous" (can work at DISPATCH_LEVEL))

// To work as a library, tan wen changed this function.

extern int tdifw_filter(struct flt_request *request);

int quick_filter(struct flt_request *request, struct flt_rule *rule)
{
	// In fact, I care nothing about rules. I simply call user's function
	// to deciede the result.
	struct flt_rule *myrule = rule;
	return tdifw_filter(request);
}

/*
int
quick_filter(struct flt_request *request, struct flt_rule *rule)
{
    const struct sockaddr_in *from, *to;
	struct flt_rule *r;
	struct plist_entry *ple;
	KIRQL irql;
	int chain, result, sid_id;

	// not IP
    if (request->addr.len != sizeof(struct sockaddr_in) ||
        request->addr.from.sa_family != AF_INET ||
        request->addr.to.sa_family != AF_INET)
    {
		KdPrint(("[tdi_fw] quick_filter: not ip addr!\n"));
        return FILTER_DENY;
    }

    from = (const struct sockaddr_in *)&request->addr.from;
    to = (const struct sockaddr_in *)&request->addr.to;

	// default behavior: DENY and LOG
	result = FILTER_DENY;
	if (rule != NULL) {
		memset(rule, 0, sizeof(*rule));
		rule->result = result;
		rule->log = TRUE;
		strcpy(rule->rule_id, "default");
	}

	chain = pid_pname_get_context(request->pid);
	if (!g_rules.chain[chain].active) {
		// chain is not active; don't check request
		return result;
	}

	if (request->sid_a != NULL)
		sid_id = get_sid_id(request->sid_a, request->sid_a_size);
	else
		sid_id = 0;		// default sid_id

	// quick filter
	KeAcquireSpinLock(&g_rules.guard, &irql);

#define CHECK_ADDR_PORT(r_addr_from, r_mask_from, r_port_from, r_port2_from,										\
						r_addr_to, r_mask_to, r_port_to, r_port2_to)												\
			((r_addr_from & r_mask_from) == (from->sin_addr.s_addr & r_mask_from) &&								\
			(r_addr_to & r_mask_to) == (to->sin_addr.s_addr & r_mask_to) &&											\
			(r_port_from == 0 || ((r_port2_from == 0) ? (r_port_from == from->sin_port) :							\
				(ntohs(from->sin_port) >= ntohs(r_port_from) && ntohs(from->sin_port) <= ntohs(r_port2_from)))) &&	\
			(r_port_to == 0 || ((r_port2_to == 0) ? (r_port_to == to->sin_port) :									\
				(ntohs(to->sin_port) >= ntohs(r_port_to) && ntohs(to->sin_port) <= ntohs(r_port2_to)))))			\

	// go through rules
	for (r = g_rules.chain[chain].head; r != NULL; r = r->next)
		// Can anybody understand it?
		if ((r->proto == IPPROTO_ANY || r->proto == request->proto) &&
			((r->direction != DIRECTION_ANY &&
				r->direction == request->direction &&
				CHECK_ADDR_PORT(r->addr_from, r->mask_from, r->port_from, r->port2_from,
						r->addr_to, r->mask_to, r->port_to, r->port2_to)) ||
			(r->direction == DIRECTION_ANY &&
				((request->direction == DIRECTION_OUT &&
					CHECK_ADDR_PORT(r->addr_from, r->mask_from, r->port_from, r->port2_from,
						r->addr_to, r->mask_to, r->port_to, r->port2_to)) ||
				(request->direction == DIRECTION_IN &&
					CHECK_ADDR_PORT(r->addr_to, r->mask_to, r->port_to,
						r->port2_to, r->addr_from, r->mask_from, r->port_from, r->port2_from))))) &&
			CHECK_BIT(r->sid_mask, sid_id))
		{
			result = r->result;
			KdPrint(("[tdi_fw] quick_filter: found rule with result: %d\n", result));
			
			if (rule != NULL) {
				memcpy(rule, r, sizeof(*rule));
				
				rule->next = NULL;	// useless field
			}

			break;
		}


	KeReleaseSpinLock(&g_rules.guard, irql);

	request->result = result;
	return result;
}
*/

// write request to request queue
BOOLEAN
log_request(struct flt_request *request)
{
	KIRQL irql, irql2;
	ULONG next_head;
	char pname_buf[256], *pname;
	struct plist_entry *ple;

	if (!g_got_log && request->type == TYPE_RESOLVE_PID)	// don't log - no log app
		return FALSE;

	KeAcquireSpinLock(&g_queue.guard, &irql);

	next_head = (g_queue.head + 1) % REQUEST_QUEUE_SIZE;
	
	if (next_head == g_queue.tail) {
		// queue overflow: reject one entry from tail
		KdPrint(("[tdi_fw] log_request: queue overflow!\n"));
		
		request->log_skipped = g_queue.data[g_queue.tail].log_skipped + 1;
		
		// free process name & sid!
		if (g_queue.data[g_queue.tail].pname != NULL)
			free(g_queue.data[g_queue.tail].pname);
		if (g_queue.data[g_queue.tail].sid_a != NULL)
			free(g_queue.data[g_queue.tail].sid_a);
		
		g_queue.tail = (g_queue.tail + 1) % REQUEST_QUEUE_SIZE;

	} else
		request->log_skipped = 0;

	memcpy(&g_queue.data[g_queue.head], request, sizeof(struct flt_request));

	// try to get process name
	pname = NULL;
	if (request->pid != (ULONG)-1 &&
		pid_pname_resolve(request->pid, pname_buf, sizeof(pname_buf))) {
		
		KdPrint(("[tdi_fw] log_request: pid:%u; pname:%s\n",
			request->pid, pname_buf));

		// ala strdup()
		pname = (char *)malloc_np(strlen(pname_buf) + 1);
		if (pname != NULL)
			strcpy(pname, pname_buf);
		else
			KdPrint(("[tdi_fw] log_request: malloc_np!\n"));
	}

	g_queue.data[g_queue.head].pname = pname;
	g_queue.head = next_head;

	// don't free sid & attributes
	if (request->sid_a != NULL)
		request->sid_a = NULL;

	KeReleaseSpinLock(&g_queue.guard, irql);

	// signal to user app
	if (g_queue.event != NULL)
		KeSetEvent(g_queue.event, IO_NO_INCREMENT, FALSE);
	
	return TRUE;
}

// read requests from log queue to buffer
ULONG
get_request(char *buf, ULONG buf_size)
{
	ULONG result = 0;
	KIRQL irql;

	// sanity check
	if (buf_size < sizeof(struct flt_request))
		return 0;

	KeAcquireSpinLock(&g_queue.guard, &irql);

	while (g_queue.head != g_queue.tail) {
		ULONG pname_size, sid_a_size;

		if (g_queue.data[g_queue.tail].pname != NULL)
			pname_size = strlen(g_queue.data[g_queue.tail].pname) + 1;
		else
			pname_size = 0;

		if (g_queue.data[g_queue.tail].sid_a != NULL)
			sid_a_size = g_queue.data[g_queue.tail].sid_a_size;
		else
			sid_a_size = 0;

		if (buf_size < sizeof(struct flt_request) + pname_size + sid_a_size)
			break;

		memcpy(buf, &g_queue.data[g_queue.tail], sizeof(struct flt_request));

		if (g_queue.data[g_queue.tail].pname != NULL) {
			((struct flt_request *)buf)->struct_size += pname_size;
			
			strcpy(buf + sizeof(struct flt_request), g_queue.data[g_queue.tail].pname);
			
			free(g_queue.data[g_queue.tail].pname);
			g_queue.data[g_queue.tail].pname = NULL;
		}

		if (g_queue.data[g_queue.tail].sid_a != NULL) {

			// make sid pointer relative only
			SID_AND_ATTRIBUTES *sid_a = g_queue.data[g_queue.tail].sid_a;

			sid_a->Sid = (PSID)((char *)(sid_a->Sid) - (char *)sid_a);

			// copy sid_a
			memcpy(buf + sizeof(struct flt_request) + pname_size, sid_a, sid_a_size);

			// free sid_a memory
			free(sid_a);
			g_queue.data[g_queue.tail].sid_a = NULL;

			// increase pname_size and struct size
			((struct flt_request *)buf)->struct_size += sid_a_size;
		}

		result += sizeof(struct flt_request) + pname_size + sid_a_size;
		buf += sizeof(struct flt_request) + pname_size + sid_a_size;
		buf_size -= sizeof(struct flt_request) + pname_size + sid_a_size;

		g_queue.tail = (g_queue.tail + 1) % REQUEST_QUEUE_SIZE;
	}
	
	KdPrint(("[tdi_fw] get_request: copied %u bytes\n", result));

	KeReleaseSpinLock(&g_queue.guard, irql);
	return result;
}

// add rule to rules chain
NTSTATUS
add_flt_rule(int chain, const struct flt_rule *rule)
{
	NTSTATUS status;
	struct flt_rule *new_rule;
	KIRQL irql;

	// sanity check
	if (chain < 0 || chain >= MAX_CHAINS_COUNT)
		return STATUS_INVALID_PARAMETER_1;
	
	KeAcquireSpinLock(&g_rules.guard, &irql);

	new_rule = (struct flt_rule *)malloc_np(sizeof(struct flt_rule));
	if (new_rule == NULL) {
		KdPrint(("[tdi_fw] add_flt_rule: malloc_np\n"));
		status = STATUS_INSUFFICIENT_RESOURCES;
		goto done;
	}

	memcpy(new_rule, rule, sizeof(*new_rule));

	// append
	new_rule->next = NULL;

	if (g_rules.chain[chain].tail == NULL) {
		g_rules.chain[chain].head = new_rule;
		g_rules.chain[chain].tail = new_rule;
	} else {
		g_rules.chain[chain].tail->next = new_rule;
		g_rules.chain[chain].tail = new_rule;
	}

	status = STATUS_SUCCESS;

done:
	KeReleaseSpinLock(&g_rules.guard, irql);
	return status;
}

// clear rules chain
NTSTATUS
clear_flt_chain(int chain)
{
	struct flt_rule *rule;
	KIRQL irql;

	// sanity check
	if (chain < 0 || chain >= MAX_CHAINS_COUNT)
		return STATUS_INVALID_PARAMETER_1;
	
	/* rules chain */
	KeAcquireSpinLock(&g_rules.guard, &irql);

	for (rule = g_rules.chain[chain].head; rule != NULL;) {
		struct flt_rule *rule2 = rule->next;
		free(rule);
		rule = rule2;
	}

	g_rules.chain[chain].head = NULL;
	g_rules.chain[chain].tail = NULL;

	if (g_rules.chain[chain].pname != NULL) {
		free(g_rules.chain[chain].pname);
		g_rules.chain[chain].pname = NULL;
	}

	// deactivate chain
	g_rules.chain[chain].active = FALSE;

	KeReleaseSpinLock(&g_rules.guard, irql);
	return STATUS_SUCCESS;
}

// set process name for chain
NTSTATUS
set_chain_pname(int chain, char *pname)
{
	KIRQL irql;
	NTSTATUS status;

	// sanity check
	if (chain < 0 || chain >= MAX_CHAINS_COUNT)
		return STATUS_INVALID_PARAMETER_1;

	KdPrint(("[tdi_fw] set_chain_pname: setting name %s for chain %d\n", pname, chain));

	KeAcquireSpinLock(&g_rules.guard, &irql);

	if (g_rules.chain[chain].pname != NULL)
		free(g_rules.chain[chain].pname);

	g_rules.chain[chain].pname = (char *)malloc_np(strlen(pname) + 1);
	if (g_rules.chain[chain].pname != NULL) {
		// copy pname
		strcpy(g_rules.chain[chain].pname, pname);
		status = STATUS_SUCCESS;
	} else
		status = STATUS_INSUFFICIENT_RESOURCES;

	KeReleaseSpinLock(&g_rules.guard, irql);
	return status;
}

// set result of process name by pid resolving
NTSTATUS
set_pid_pname(ULONG pid, char *pname)
{
	KIRQL irql;
	int i, chain = 0;

	KdPrint(("[tdi_fw] set_pid_pname: setting pname %s for pid %u\n", pname, pid));
	
	KeAcquireSpinLock(&g_rules.guard, &irql);
	for (i = 0; i < MAX_CHAINS_COUNT; i++)
		if (g_rules.chain[i].pname != NULL &&
			_stricmp(pname, g_rules.chain[i].pname) == 0) {
	
			KdPrint(("[tdi_fw] set_pid_pname: found chain %d\n", i));
			chain = i;

			break;
		}
	KeReleaseSpinLock(&g_rules.guard, irql);

	return pid_pname_set(pid, pname, chain);
}

// activate rules chain
NTSTATUS
activate_flt_chain(int chain)
{
	// sanity check
	if (chain < 0 || chain >= MAX_CHAINS_COUNT)
		return STATUS_INVALID_PARAMETER_1;

	g_rules.chain[chain].active = TRUE;

	return STATUS_SUCCESS;
}

BOOLEAN
default_chain_only(void)
{
	int i;

	if (!g_rules.chain[0].active)
		return FALSE;

	for (i = 1; i < MAX_CHAINS_COUNT; i++)
		if (g_rules.chain[i].active)
			return FALSE;

	return TRUE;

}
