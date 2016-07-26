/***
*mbsnbcat_s.c - concatenate string2 onto string1, max length n bytes
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       defines mbsnbcat_s() - concatenate maximum of n bytes
*
*******************************************************************************/

#ifdef _MBCS

#include <mbstring.h>

#ifdef _SYSCRT

#include <internal_safecrt.h>

#define _FUNC_PROLOGUE
#define _FUNC_NAME _mbsnbcat_s
#define _DEST _Dst
#define _SRC _Src
#define _COUNT _CountInBytes
#define _COUNT_IN_BYTES 1

#include <mbsncat_s.inl>

#else  /* _SYSCRT */

#include <internal.h>

_REDIRECT_TO_L_VERSION_4(errno_t, _mbsnbcat_s, unsigned char *, size_t, const unsigned char *, size_t)

#endif  /* _SYSCRT */

#endif  /* _MBCS */
