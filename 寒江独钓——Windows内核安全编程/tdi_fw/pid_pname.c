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
// $Id: pid_pname.c,v 1.1 2009/04/28 12:53:28 tanwen Exp $

/*
 * Get process name by its pid and all related routines
 */

#include <ntddk.h>

#include "memtrack.h"
#include "pid_pname.h"
#include "sock.h"		// add by tan wen	
#include "ipc.h"		// add by tan wen

/* process list entry */
struct plist_entry {
	struct	plist_entry *next;
	
	// id & name
	ULONG	pid;
	char	*pname;
	KEVENT	*pname_event;

	int		context;
};

/* process list */
static struct {
	struct		plist_entry *head;
	struct		plist_entry *tail;
	KSPIN_LOCK	guard;
} g_plist;

static struct plist_entry *add_ple(ULONG pid, KIRQL *irql);
static struct plist_entry *find_ple(ULONG pid, KIRQL *irql, struct plist_entry **prev);

static VOID ProcessNotifyProc(IN HANDLE ParentId, IN HANDLE ProcessId, IN BOOLEAN Create);

void
pid_pname_init(void)
{
	KeInitializeSpinLock(&g_plist.guard);
	g_plist.head = g_plist.tail = NULL;

	PsSetCreateProcessNotifyRoutine(ProcessNotifyProc, FALSE);
}

// free plist
void
pid_pname_free(void)
{
	KIRQL irql;
	struct plist_entry *ple;

	PsSetCreateProcessNotifyRoutine(ProcessNotifyProc, TRUE);

	KeAcquireSpinLock(&g_plist.guard, &irql);
	for (ple = g_plist.head; ple != NULL;) {
		struct plist_entry *ple2 = ple->next;
		if (ple->pname != NULL)
			free(ple->pname);
		free(ple);
		ple = ple2;
	}
	g_plist.head = g_plist.tail = NULL;
	KeReleaseSpinLock(&g_plist.guard, irql);
}

// try to get pname by pid
BOOLEAN
pid_pname_resolve(ULONG pid, char *buf, int buf_size)
{
	BOOLEAN result;
	KIRQL irql;
	struct plist_entry *ple = find_ple(pid, &irql, NULL);

	if (ple == NULL)
		return FALSE;

	if (ple->pname != NULL) {
		if (buf_size > 0) {
			strncpy(buf, ple->pname, buf_size);
			buf[buf_size - 1] = '\0';
		}
		result = TRUE;
	} else
		result = FALSE;
	
	KeReleaseSpinLock(&g_plist.guard, irql);
	return result;
}

// set pname_event by pid
NTSTATUS
pid_pname_set_event(ULONG pid, KEVENT *event)
{
	KIRQL irql;
	struct plist_entry *ple = find_ple(pid, &irql, NULL);

	if (ple == NULL) {
		// try to add
		ple = add_ple(pid, &irql);
		if (ple == NULL)
			return STATUS_INSUFFICIENT_RESOURCES;
	}

	ple->pname_event = event;

	KeReleaseSpinLock(&g_plist.guard, irql);
	return STATUS_SUCCESS;
}

NTSTATUS
pid_pname_set(ULONG pid, const char *pname, int context)
{
	KIRQL irql;
	struct plist_entry *ple = find_ple(pid, &irql, NULL);
	NTSTATUS status;

	if (ple == NULL) {
		// try to add
		ple = add_ple(pid, &irql);
		if (ple == NULL)
			return STATUS_INSUFFICIENT_RESOURCES;
	}
	
	if (ple->pname == NULL) {
		ple->pname = (char *)malloc_np(strlen(pname) + 1);
		if (ple->pname != NULL) {
			strcpy(ple->pname, pname);
			status = STATUS_SUCCESS;
		} else {
			KdPrint(("[tdi_fw] set_pid_name: malloc_np!\n"));
			status = STATUS_INSUFFICIENT_RESOURCES;
		}

		// and signal event we have name! (almost)
		if (ple->pname_event != NULL)
			KeSetEvent(ple->pname_event, IO_NO_INCREMENT, FALSE);
	
	} else
		status = STATUS_SUCCESS;	// already got pname


	ple->context = context;

	KeReleaseSpinLock(&g_plist.guard, irql);
	return status;
}

int
pid_pname_get_context(ULONG pid)
{
	KIRQL irql;
	int context;
	struct plist_entry *ple = find_ple(pid, &irql, NULL);
	if (ple == NULL)
		return 0;

	context = ple->context;
	
	KeReleaseSpinLock(&g_plist.guard, irql);
	return context;
}


struct plist_entry *
add_ple(ULONG pid, KIRQL *irql)
{
	struct plist_entry *ple;

	if (irql != NULL)
		KeAcquireSpinLock(&g_plist.guard, irql);

	// add new entry to g_plist
	ple = (struct plist_entry *)malloc_np(sizeof(*ple));
	if (ple != NULL) {
		memset(ple, 0, sizeof(*ple));
		ple->pid = pid;

		// append
		if (g_plist.tail != NULL) {
			g_plist.tail->next = ple;
			g_plist.tail = ple;
		} else
			g_plist.head = g_plist.tail = ple;

	} else {
		KdPrint(("[tdi_fw] add_ple: malloc_np!\n"));
	
		if (irql != NULL)
			KeReleaseSpinLock(&g_plist.guard, *irql);
	}

	return ple;
}

struct plist_entry *
find_ple(ULONG pid, KIRQL *irql, struct plist_entry **prev)
{
	struct plist_entry *ple, *prev_ple;

	if (irql != NULL)
		KeAcquireSpinLock(&g_plist.guard, irql);

	prev_ple = NULL;
	for (ple = g_plist.head; ple != NULL; ple = ple->next) {
		if (ple->pid == pid) {
			if (prev != NULL)
				*prev = prev_ple;
			return ple;
		}
		prev_ple = ple;
	}

	if (irql != NULL)
		KeReleaseSpinLock(&g_plist.guard, *irql);
	
	return NULL;
}


// notify routine on process creation or removing
extern int tdifw_filter(struct flt_request *request);

VOID
ProcessNotifyProc(IN HANDLE ParentId, IN HANDLE ProcessId, IN BOOLEAN Create)
{
	KIRQL irql;
	struct plist_entry *ple, *prev_ple;
	struct flt_request request = { 0 }; //< add by tan wen

	request.pid = (ULONG)ProcessId;

	if (Create) {

		// add by tan wen
		request.type = TYPE_PROCESS_CREATE;
		tdifw_filter(&request);

		KdPrint(("[tdi_fw] ProcessNotifyProc: create process with pid:%u\n", ProcessId));

		add_ple((ULONG)ProcessId, &irql);
		KeReleaseSpinLock(&g_plist.guard, irql);

	} else {

		request.type = TYPE_PROCESS_TERMINATE;
		tdifw_filter(&request);

		// remove entry from plist

		KdPrint(("[tdi_fw] ProcessNotifyProc: remove process with pid:%u\n", ProcessId));

		ple = find_ple((ULONG)ProcessId, &irql, &prev_ple);
		if (ple == NULL)
			return;

		if (prev_ple != NULL)
			prev_ple->next = ple->next;
		else
			g_plist.head = ple->next;

		if (ple->next == NULL)
			g_plist.tail = prev_ple;

		if (ple->pname != NULL) {
			KdPrint(("[tdi_fw] ProcessNotifyProc: pname was %s\n", ple->pname));
			free(ple->pname);
		}

		free(ple);
		KeReleaseSpinLock(&g_plist.guard, irql);
	}
}
