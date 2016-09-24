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
// $Id: filter.h,v 1.1 2009/04/28 12:53:28 tanwen Exp $

#ifndef _filter_h_
#define _filter_h_

#include "ipc.h"

NTSTATUS	filter_init(void);
NTSTATUS	filter_init_2(void);

void		filter_free(void);
void		filter_free_2(void);

NTSTATUS	add_flt_rule(int chain, const struct flt_rule *rule);
NTSTATUS	clear_flt_chain(int chain);
NTSTATUS	activate_flt_chain(int chain);
NTSTATUS	set_chain_pname(int chain, char *pname);
NTSTATUS	set_pid_pname(ULONG pid, char *pname);

BOOLEAN		default_chain_only(void);

int			quick_filter(struct flt_request *request, struct flt_rule *rule);

BOOLEAN		log_request(struct flt_request *request);
ULONG		get_request(char *buf, ULONG buf_size);

#endif
