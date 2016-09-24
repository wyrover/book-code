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
// $Id: obj_tbl.h,v 1.1 2009/04/28 12:53:28 tanwen Exp $

#ifndef _obj_tbl_h_
#define _obj_tbl_h_

#include "filter.h"

NTSTATUS	ot_init(void);
void		ot_free(void);

#define FILEOBJ_CONTROLOBJ	0
#define FILEOBJ_ADDROBJ		1
#define FILEOBJ_CONNOBJ		2

NTSTATUS	ot_add_fileobj(
	PDEVICE_OBJECT devobj, PFILE_OBJECT fileobj, int fileobj_type, int ipproto,
	CONNECTION_CONTEXT conn_ctx);

NTSTATUS	ot_del_fileobj(
	PFILE_OBJECT fileobj, int *fileobj_type);

// maximum length of TDI_ADDRESS_TYPE_*
#define TDI_ADDRESS_MAX_LENGTH	TDI_ADDRESS_LENGTH_OSI_TSAP
#define TA_ADDRESS_MAX			(sizeof(TA_ADDRESS) - 1 + TDI_ADDRESS_MAX_LENGTH)
#define TDI_ADDRESS_INFO_MAX	(sizeof(TDI_ADDRESS_INFO) - 1 + TDI_ADDRESS_MAX_LENGTH)

// max event index
#ifdef TDI_EVENT_ERROR_EX
// 2k
#	define MAX_EVENT	(TDI_EVENT_ERROR_EX + 1)
#else
// NT4
#	define MAX_EVENT	(TDI_EVENT_CHAINED_RECEIVE_EXPEDITED + 1)
#endif

/* replaced context */
typedef struct {
	PFILE_OBJECT	fileobj;		/* address object */
    PVOID			old_handler;	/* old event handler */
    PVOID			old_context;	/* old event handler context */
} TDI_EVENT_CONTEXT;

struct ot_entry {
	ULONG signature;
	struct ot_entry		*next;
	
	ULONG				pid;
	
	struct				_SID_AND_ATTRIBUTES *sid_a;
	ULONG				sid_a_size;
	
	PDEVICE_OBJECT		devobj;
	PFILE_OBJECT		fileobj;
	PFILE_OBJECT		associated_fileobj;
	
	int					type;
	int					ipproto;
	
	TDI_EVENT_CONTEXT	ctx[MAX_EVENT];
	UCHAR				local_addr[TA_ADDRESS_MAX];
	UCHAR				remote_addr[TA_ADDRESS_MAX];

	CONNECTION_CONTEXT	conn_ctx;
	
	struct listen_entry	*listen_entry;	// for address object
	struct conn_entry	*conn_entry;	// for connection object

	// traffic count for connection object
	ULONG				bytes_out;
	ULONG				bytes_in;
	
	BOOLEAN				log_disconnect;
};

struct ot_entry	*ot_find_fileobj(PFILE_OBJECT fileobj, KIRQL *irql);
// Note: don't forget KeReleaseSpinLock(&g_ot_hash_guard, irql);

extern KSPIN_LOCK g_ot_hash_guard;

void    ot_cleanup_ote(struct ot_entry *ote);


NTSTATUS	ot_add_conn_ctx(
	PFILE_OBJECT addrobj, CONNECTION_CONTEXT conn_ctx, PFILE_OBJECT connobj);

NTSTATUS	ot_del_conn_ctx(
	PFILE_OBJECT addrobj, CONNECTION_CONTEXT conn_ctx);

PFILE_OBJECT ot_find_conn_ctx(
	PFILE_OBJECT addrobj, CONNECTION_CONTEXT conn_ctx);


struct _SID_AND_ATTRIBUTES *get_current_sid_a(ULONG *sid_a_size);
struct _SID_AND_ATTRIBUTES *copy_sid_a(struct _SID_AND_ATTRIBUTES *sid_a, ULONG sid_a_size);

#endif
