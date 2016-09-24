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
// $Id: ipc.c,v 1.1 2009/04/28 12:53:28 tanwen Exp $

/*
 * This file contain replaced TDI_EVENT_CONNECT handler
 */

#include <ntddk.h>
#include <tdikrnl.h>
#include "sock.h"

#include "conn_state.h"
#include "filter.h"
#include "ipc.h"
#include "sids.h"
#include "tdi_fw.h"

static void		get_traffic_counters(unsigned __int64 *counters);

NTSTATUS
process_nfo_request(ULONG code, char *buf, ULONG *buf_len, ULONG buf_size)
{
	NTSTATUS status;
	ULONG len = *buf_len;
	*buf_len = 0;

	switch (code) {
	
	case IOCTL_CMD_ENUM_LISTEN:
		// enum listening endpoints

		if (buf_size < sizeof(struct listen_nfo) || buf == NULL) {
			status = STATUS_INFO_LENGTH_MISMATCH;
			break;
		}

		status = enum_listen((struct listen_nfo *)buf, buf_len, buf_size);
		break;

	case IOCTL_CMD_ENUM_TCP_CONN:
		// enum TCP connections

		if (buf_size < sizeof(struct tcp_conn_nfo) || buf == NULL) {
			status = STATUS_INFO_LENGTH_MISMATCH;
			break;
		}

		status = enum_tcp_conn((struct tcp_conn_nfo *)buf, buf_len, buf_size);
		break;

	case IOCTL_CMD_GET_COUNTERS: 
		// get traffic counters

		if (buf_size < sizeof(g_traffic)) {
			status = STATUS_INFO_LENGTH_MISMATCH;
			break;
		}

		get_traffic_counters((unsigned __int64 *)buf);
		
		*buf_len = sizeof(g_traffic);
		status = STATUS_SUCCESS;
		break;

	default:
		status = STATUS_NOT_SUPPORTED;
	}

	return status;
}

NTSTATUS
process_request(ULONG code, char *buf, ULONG *buf_len, ULONG buf_size)
{
	NTSTATUS status;
	ULONG len = *buf_len;
	*buf_len = 0;

	switch (code) {
	
	case IOCTL_CMD_GETREQUEST:
		// get data for logging

		if (buf_size < sizeof(struct flt_request) || buf == NULL) {
			status = STATUS_INFO_LENGTH_MISMATCH;
			break;
		}

		*buf_len = get_request(buf, buf_size);
		status = STATUS_SUCCESS;
		break;

	case IOCTL_CMD_CLEARCHAIN:
		// clear rules chain #i

		if (len != sizeof(int) || buf == NULL) {
			status = STATUS_INFO_LENGTH_MISMATCH;
			break;
		}

		status = clear_flt_chain(*(int *)buf);
		break;

	case IOCTL_CMD_APPENDRULE:
		// append rule to chain #i

		if (len != sizeof(struct flt_rule) || buf == NULL) {
			status = STATUS_INFO_LENGTH_MISMATCH;
			break;
		}

		status = add_flt_rule(((struct flt_rule *)buf)->chain, (struct flt_rule *)buf);
		break;

	case IOCTL_CMD_SETCHAINPNAME:
		// set chain #i process name

		if (len < sizeof(int) + sizeof(char) || buf == NULL) {
			status = STATUS_INFO_LENGTH_MISMATCH;
			break;
		}
		if (buf[len - 1] != '\0') {
			status = STATUS_INVALID_PARAMETER;	// string must be zero-terminated
			break;
		}

		status = set_chain_pname(*(int *)buf, buf + sizeof(int));
		break;

	case IOCTL_CMD_SETPNAME:
		// set process name for pid

		if (len < sizeof(ULONG) + sizeof(char) || buf == NULL) {
			status = STATUS_INFO_LENGTH_MISMATCH;
			break;
		}
		if (buf[len - 1] != '\0') {
			status = STATUS_INVALID_PARAMETER;	// string must be zero-terminated
			break;
		}

		status = set_pid_pname(*(ULONG *)buf, buf + sizeof(ULONG));
		break;

	case IOCTL_CMD_ACTIVATECHAIN:
		// active rules chain #i

		if (len != sizeof(int) || buf == NULL) {
			status = STATUS_INFO_LENGTH_MISMATCH;
			break;
		}

		status = activate_flt_chain(*(int *)buf);
		break;

	case IOCTL_CMD_SET_SIDS:
		// set SIDs array

		if (len < sizeof(ULONG)) {
			status = STATUS_INFO_LENGTH_MISMATCH;
			break;
		}

		status = set_sid_list(buf, len);
		break;

	case IOCTL_CMD_GET_COUNTERS: 
		// get traffic counters

		if (buf_size < sizeof(g_traffic)) {
			status = STATUS_INFO_LENGTH_MISMATCH;
			break;
		}

		get_traffic_counters((unsigned __int64 *)buf);
		
		*buf_len = sizeof(g_traffic);
		status = STATUS_SUCCESS;
		break;

	default:
		status = STATUS_NOT_SUPPORTED;
	}

	return status;
}

void
get_traffic_counters(unsigned __int64 *counters)
{
	KIRQL irql;

	KeAcquireSpinLock(&g_traffic_guard, &irql);
	memcpy(counters, g_traffic, sizeof(g_traffic));
	KeReleaseSpinLock(&g_traffic_guard, irql);
}
