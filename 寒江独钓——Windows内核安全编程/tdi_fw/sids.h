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
// $Id: sids.h,v 1.1 2009/04/28 12:53:28 tanwen Exp $

#ifndef _sids_h_
#define _sids_h_

/* SID stuff */

struct _SID_AND_ATTRIBUTES *get_current_sid_a(ULONG *sid_a_size);
struct _SID_AND_ATTRIBUTES *copy_sid_a(struct _SID_AND_ATTRIBUTES *sid_a, ULONG sid_a_size);

void sids_init(void);

int get_sid_id(struct _SID_AND_ATTRIBUTES *sid_a, ULONG sid_a_size);

/* size can be NULL; buf is ULONG sid_len; SID ... */
NTSTATUS set_sid_list(char *buf, ULONG size);

#endif
