/***
*mbsnbcpy_s.c - Copy one string to another, n bytes only (MBCS)
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Copy one string to another, n bytes only (MBCS)
*
*******************************************************************************/

#ifdef _MBCS

#include <mbstring.h>

#ifdef _SYSCRT

#include <internal_safecrt.h>

#define _FUNC_PROLOGUE
#define _FUNC_NAME _mbsnbcpy_s
#define _DEST _Dst
#define _SRC _Src
#define _COUNT _CountInBytes
#define _COUNT_IN_BYTES 1

#include <mbsncpy_s.inl>

#else  /* _SYSCRT */

#include <internal.h>

_REDIRECT_TO_L_VERSION_4(errno_t, _mbsnbcpy_s, unsigned char *, size_t, const unsigned char *, size_t)

#endif  /* _SYSCRT */

#endif  /* _MBCS */
