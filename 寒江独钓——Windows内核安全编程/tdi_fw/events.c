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
// $Id: events.c,v 1.1 2009/04/28 12:53:27 tanwen Exp $

/*
 * Replaced event handlers for TDI events
 */

#include <ntddk.h>
#include <tdikrnl.h>
#include "sock.h"

#include "events.h"
#include "memtrack.h"

// to simplify processing don't use chained handlers for datagrams
struct tdi_event_handler g_tdi_event_handlers[]= {
	{TDI_EVENT_CONNECT,						tdi_event_connect},
	{TDI_EVENT_DISCONNECT,					tdi_event_disconnect},
	{TDI_EVENT_RECEIVE_DATAGRAM,			tdi_event_receive_datagram},
	{TDI_EVENT_CHAINED_RECEIVE_DATAGRAM,	NULL},
	{TDI_EVENT_RECEIVE,						tdi_event_receive},
	{TDI_EVENT_RECEIVE_EXPEDITED,			tdi_event_receive},
	{TDI_EVENT_CHAINED_RECEIVE,				tdi_event_chained_receive},
	{TDI_EVENT_CHAINED_RECEIVE_EXPEDITED,	tdi_event_chained_receive},
	{(ULONG)-1,								NULL}
};
