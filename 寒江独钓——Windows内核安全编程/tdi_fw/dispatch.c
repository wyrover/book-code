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
// $Id: dispatch.c,v 1.1 2009/04/28 12:53:27 tanwen Exp $

/*
 * Dispatch routines for TDI ioctls
 */

#include <ntddk.h>
#include <tdikrnl.h>
#include "sock.h"

#include "dispatch.h"

#if DBG
#	define ENTRY(code, fn)	{code, fn, #code}
#	define LAST_ENTRY		{0, NULL, NULL}
#else
#	define ENTRY(code, fn)	{code, fn}
#	define LAST_ENTRY		{0, NULL}
#endif

struct tdi_ioctl g_tdi_ioctls[] = {
	ENTRY(TDI_ASSOCIATE_ADDRESS,	tdi_associate_address),
	ENTRY(TDI_CONNECT,				tdi_connect),
	ENTRY(TDI_DISASSOCIATE_ADDRESS,	tdi_disassociate_address),
	ENTRY(TDI_SET_EVENT_HANDLER,	tdi_set_event_handler),
	ENTRY(TDI_SEND_DATAGRAM,		tdi_send_datagram),
	ENTRY(TDI_RECEIVE_DATAGRAM,		tdi_receive_datagram),
	ENTRY(TDI_DISCONNECT,			tdi_disconnect),
	ENTRY(TDI_SEND,					tdi_send),
	ENTRY(TDI_RECEIVE,				tdi_receive),
#if 1		// for now only deny stubs for security reasons
	ENTRY(TDI_ACCEPT,				tdi_deny_stub),
	ENTRY(TDI_LISTEN,				tdi_deny_stub),
#endif
	LAST_ENTRY
};
