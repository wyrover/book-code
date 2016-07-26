/***
*mbsnbset_s.c - Sets first n bytes of string to given character (MBCS)
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Sets first n bytes of string to given character (MBCS)
*
*******************************************************************************/

#ifdef _MBCS

#include <mbstring.h>

#ifdef _SYSCRT

#include <internal_safecrt.h>

#define _FUNC_PROLOGUE
#define _FUNC_NAME _mbsnbset_s
#define _DEST _Dst
#define _COUNT _CountInBytes
#define _COUNT_IN_BYTES 1

#include <mbsnset_s.inl>

#else  /* _SYSCRT */

#include <internal.h>

_REDIRECT_TO_L_VERSION_4(errno_t, _mbsnbset_s, unsigned char *, size_t, unsigned int, size_t)

#endif  /* _SYSCRT */

#endif  /* _MBCS */
