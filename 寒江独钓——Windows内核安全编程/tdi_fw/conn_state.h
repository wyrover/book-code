// -*- mode: C++; tab-width: 4; indent-tabs-mode: nil -*- (for GNU Emacs)
//
// $Id: conn_state.h,v 1.1 2009/04/28 12:53:27 tanwen Exp $

#ifndef _conn_state_h_
#define _conn_state_h_

#include "ipc.h"

NTSTATUS conn_state_init(void);
void conn_state_free(void);

/* work with listening ports for all protocols */

NTSTATUS add_listen(struct ot_entry *ote_addr);

void del_listen_obj(struct listen_entry *le, BOOLEAN no_guard);

BOOLEAN is_listen(ULONG addr, USHORT port, int ipproto);

BOOLEAN is_bcast_listen(ULONG addr, USHORT port, int ipproto);

NTSTATUS	enum_listen(struct listen_nfo *buf, ULONG *buf_len, ULONG buf_size);

/* work with TCP only connections */

NTSTATUS add_tcp_conn(struct ot_entry *ote_conn, int tcp_state);
void del_tcp_conn(PFILE_OBJECT connobj, BOOLEAN is_disconnect);

void del_tcp_conn_obj(struct conn_entry *ce, BOOLEAN no_guard);
void log_disconnect(struct ot_entry *ote_conn);

NTSTATUS set_tcp_conn_state(PFILE_OBJECT connobj, int state);
NTSTATUS set_tcp_conn_local(PFILE_OBJECT connobj, TA_ADDRESS *local);

int get_tcp_conn_state(ULONG laddr, USHORT lport, ULONG raddr, USHORT rport);
int get_tcp_conn_state_by_obj(PFILE_OBJECT connobj);

NTSTATUS	enum_tcp_conn(struct tcp_conn_nfo *buf, ULONG *buf_len, ULONG buf_size);

#endif
