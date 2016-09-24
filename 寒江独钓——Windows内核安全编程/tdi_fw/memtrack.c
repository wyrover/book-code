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
// $Id: memtrack.c,v 1.1 2009/04/28 12:53:28 tanwen Exp $

#if DBG

/*
 * Debug NonPaged pool routines (helpers to find memory leaks and pool corruption)
 */

#include <ntddk.h>

#include "memtrack.h"

#define MAGIC	'TMEM'
#define INT_3	__asm int 3

struct prefix {
	ULONG		magic;
	struct		prefix *next;
	struct		prefix *prev;
	ULONG		size;
	const char	*file;
	ULONG		line;
	char		data[];
};

struct postfix {
	ULONG	size;
	ULONG	magic;
};

static KSPIN_LOCK guard;
static struct prefix *first, *last;
static ULONG count;

static struct postfix	*check(struct prefix *p);

void
memtrack_init()
{
	KeInitializeSpinLock(&guard);
}

void
memtrack_free()
{
	KIRQL irql;
	ULONG total = 0;

	KeAcquireSpinLock(&guard, &irql);

	if (first != NULL) {
		struct prefix *p;
		for (p = first; p; p = p->next) {
			check(p);

			KdPrint(("memtrack: memory leak detected! %s:%u (%u bytes)\n",
				p->file, p->line, p->size));

			total += p->size;
		}
	}

	KeReleaseSpinLock(&guard, irql);
	KdPrint(("memtrack: Total memory leakage: %u bytes (%u blocks)\n", total, count));

	if (total) INT_3;
}

void *
mt_malloc(ULONG size, const char *file, ULONG line)
{
	KIRQL irql;
	struct prefix *data;
	struct postfix *pd;

#if 1
	// check pool integrity
	KeAcquireSpinLock(&guard, &irql);
	
	for (data = first; data; data = data->next)
		check(data);
	
	for (data = last; data; data = data->prev)
		check(data);
	
	KeReleaseSpinLock(&guard, irql);
#endif

	if (size == 0) {
		KdPrint(("memtrack: mt_malloc: size == 0!\n"));
		INT_3;
		return NULL;
	}

	data = (struct prefix *)ExAllocatePool(NonPagedPool,
		sizeof(struct prefix) + size + sizeof(struct postfix));
	if (data == NULL)
		return NULL;

	data->magic = MAGIC;
	data->next = NULL;
	data->prev = NULL;
	data->size = size;
	data->file = file;
	data->line = line;

	memset(data->data, 0xcc, size);		// fill by 0xcc: new

	pd = (struct postfix *)(data->data + data->size);

	pd->size = size;
	pd->magic = MAGIC;

	KeAcquireSpinLock(&guard, &irql);
	
	if (last) {
		last->next = data;
		data->prev = last;
		last = data;
	}
	else {
		data->prev = NULL;
		first = last = data;
	}
	count++;

	KeReleaseSpinLock(&guard, irql);
	return data->data;
}

void
free(void *ptr)
{
	KIRQL irql;
	struct prefix *data = (struct prefix *)((char *)ptr - sizeof(struct prefix));
	struct postfix *pd = check(data);

	if (pd == NULL)
		return;

	KeAcquireSpinLock(&guard, &irql);

	if (data->next != NULL)
		data->next->prev = data->prev;
	else
		last = data->prev;
	if (data->prev != NULL)
		data->prev->next = data->next;
	else
		first = data->next;

	memset(data->data, 0xc9, data->size);	// fill by 0xc9: free

	data->size = (ULONG)-1;
	pd->size = (ULONG)-1;

	count--;
	KeReleaseSpinLock(&guard, irql);

	ExFreePool(data);
}

struct postfix *
check(struct prefix *p)
{
	struct postfix *pd;

	if (p->magic != MAGIC) {
		KdPrint(("memtrack: check: invalid pre-magic! 0x%x\n", p));
		INT_3;
		return NULL;
	}

	pd = (struct postfix *)(p->data + p->size);

	if (pd->magic != MAGIC) {
		KdPrint(("memtrack: memtrack_free: invalid post-magic! 0x%x\n", pd));
		INT_3;
		return NULL;
	}

	if (p->size != pd->size) {
		KdPrint(("memtrack: memtracl_free: invalid post-size! 0x%x 0x%x\n", p, pd));
		INT_3;
		return NULL;
	}

	return pd;
}


#endif /* DBG */
