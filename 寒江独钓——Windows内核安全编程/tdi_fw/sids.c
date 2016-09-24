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
// $Id: sids.c,v 1.1 2009/04/28 12:53:28 tanwen Exp $

/*
 * Working with SID bitmasks
 */

#include <ntddk.h>
#include "sock.h"

#include "ipc.h"
#include "memtrack.h"
#include "sids.h"
#include "tdi_fw.h"

struct sid_nfo {
	ULONG	sid_len;
	char	sid_data[];
};

static struct {
	char		*buf;
	struct		sid_nfo *list[MAX_SIDS_COUNT];
	int			count;
	KSPIN_LOCK	guard;
} g_sids;

void
sids_init(void)
{
	KeInitializeSpinLock(&g_sids.guard);
}

NTSTATUS
set_sid_list(char *buf, ULONG size)
{
	KIRQL irql;
	NTSTATUS status;
	ULONG pos;
	int i;

	KeAcquireSpinLock(&g_sids.guard, &irql);

	// first, free information
	if (g_sids.buf != NULL) {
		free(g_sids.buf);
		g_sids.buf = NULL;
	}
	memset(g_sids.list, 0, sizeof(g_sids.list));
	g_sids.count = 0;

	if (size != 0) {
		// copy buffer
		g_sids.buf = (char *)malloc_np(size);
		if (g_sids.buf == NULL) {
			KdPrint(("[tdi_fw] set_sid_list: malloc_np!\n"));
			status = STATUS_INSUFFICIENT_RESOURCES;
			goto done;
		}
		memcpy(g_sids.buf, buf, size);

		// parse buffer and find struct sid_nfo
		for (pos = 0, i = 0; pos + sizeof(struct sid_nfo) < size && i < MAX_SIDS_COUNT; i++) {
			struct sid_nfo *nfo = (struct sid_nfo *)&g_sids.buf[pos];

			if (pos + sizeof(*nfo) + nfo->sid_len > size)
				break;

			g_sids.list[i] = nfo;

			pos += sizeof(*nfo) + nfo->sid_len;
		}

		g_sids.count = i;

		if (pos == size)
			status = STATUS_SUCCESS;
		else
			status = STATUS_INVALID_PARAMETER;
	} else
		status = STATUS_SUCCESS;

done:
	KeReleaseSpinLock(&g_sids.guard, irql);
	return status;
}

#define SeLengthSid(sid)    (8 + 4 * ((unsigned char *)(sid))[1])

int
get_sid_id(struct _SID_AND_ATTRIBUTES *sid_a, ULONG sid_a_size)
{
	// doing linear search (optimize?)
	KIRQL irql;
	int i, result = 0;

    KdPrint(("[tdi_fw] get_sid_id: (sid_size = %u/%u)\n",
        SeLengthSid(sid_a->Sid), sid_a_size));

	KeAcquireSpinLock(&g_sids.guard, &irql);

    for (i = 1; i < g_sids.count; i++) {
        // comparing sids byte by byte (can't call RtlEqualSid() due to DISPATCH_LEVEL)

        KdPrint(("[tdi_fw] get_sid_id: sid #%d size %u\n", i, SeLengthSid((PSID)g_sids.list[i]->sid_data)));

		if (SeLengthSid(sid_a->Sid) == SeLengthSid((PSID)g_sids.list[i]->sid_data) &&
            memcmp(sid_a->Sid, (PSID)g_sids.list[i]->sid_data, SeLengthSid(sid_a->Sid)) == 0) {
			result = i;
			break;
		}
    }

	KeReleaseSpinLock(&g_sids.guard, irql);

	KdPrint(("[tdi_fw] get_sid_id: %d\n", result));
	return result;
}

#define CURRENT_THREAD	(HANDLE)-2
#define CURRENT_PROCESS	(HANDLE)-1

#define TOKEN_QUERY		0x0008

struct _SID_AND_ATTRIBUTES *
get_current_sid_a(ULONG *sid_a_size)		// must be called at PASSIVE_LEVEL!
{
	NTSTATUS status;
	HANDLE token;
	ULONG size;
	SID_AND_ATTRIBUTES *sid_a;

	*sid_a_size = 0;

	// open thread token
	status = ZwOpenThreadToken(CURRENT_THREAD, TOKEN_QUERY, FALSE, &token);
	if (status == STATUS_NO_TOKEN) {
		// open process token
		status = ZwOpenProcessToken(CURRENT_PROCESS, TOKEN_QUERY, &token);
	}
	if (status != STATUS_SUCCESS) {
		KdPrint(("[tdi_fw] get_current_sid_a: ZwOpen{Thread|Process}Token: 0x%x!\n"));
		return NULL;
	}

	size = sizeof(*sid_a) + 100;		// default size
	
	sid_a = (SID_AND_ATTRIBUTES *)malloc_np(size);
	if (sid_a == NULL) {
		KdPrint(("[tdi_fw] get_current_sid_a: malloc_np!\n"));
		goto done;
	}

	status = ZwQueryInformationToken(token, TokenUser, sid_a, size, &size);
	if (status == STATUS_BUFFER_TOO_SMALL) {
		free(sid_a);
		
		sid_a = (SID_AND_ATTRIBUTES *)malloc_np(size);
		if (sid_a == NULL) {
			KdPrint(("[tdi_fw] get_current_sid_a: malloc_np!\n"));
			goto done;
		}

		status = ZwQueryInformationToken(token, TokenUser, sid_a, size, &size);
	}
	if (status != STATUS_SUCCESS) {
		KdPrint(("[tdi_fw] get_current_sid_a: ZwQueryInformationToken: 0x%x!\n"));

		free(sid_a);
		sid_a = NULL;
		goto done;
	}

	// got sid & attributes!

	*sid_a_size = size;

done:
	ZwClose(token);
	return sid_a;
}

struct _SID_AND_ATTRIBUTES *
copy_sid_a(SID_AND_ATTRIBUTES *sid_a, ULONG sid_a_size)
{
	SID_AND_ATTRIBUTES *result;
	
	if (sid_a == NULL)
		return NULL;

	result = (SID_AND_ATTRIBUTES *)malloc_np(sid_a_size);
	if (result == NULL)
		return NULL;

	memcpy(result, sid_a, sid_a_size);

	result->Sid = (char *)result + ((char *)(sid_a->Sid) - (char *)sid_a);

	return result;
}
