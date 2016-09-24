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
// $Id: memtrack.h,v 1.1 2009/04/28 12:53:28 tanwen Exp $

#ifndef _memtrack_h_
#define _memtrack_h_

#define MEM_TAG		'1VRD'

#if DBG

void	memtrack_init(void);
void	memtrack_free(void);

void	*mt_malloc(ULONG size, const char *file, ULONG line);

// allocate memory from nonpaged pool
#define malloc_np(size)	mt_malloc((size), __FILE__, __LINE__)	

void free(void *ptr);

// debug macro
#define _TEST_ME_	__asm int 3

#else /* DBG */

#define memtrack_init()
#define memtrack_free()

#define malloc_np(size)	ExAllocatePoolWithTag(NonPagedPool, (size), MEM_TAG)
#define free(ptr)		ExFreePool(ptr)


#define _TEST_ME_

#endif /* DBG */

#endif
