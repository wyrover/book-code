/***
*mbsncat_s.c - concatenate string2 onto string1, max length n
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       defines mbsncat_s() - concatenate maximum of n characters
*
*******************************************************************************/

#ifdef _MBCS

#include <mbstring.h>

#ifdef _SYSCRT

#include <internal_safecrt.h>

#define _FUNC_PROLOGUE
#define _FUNC_NAME _mbsncat_s
#define _DEST _Dst
#define _SRC _Src
#define _COUNT _CountInChars
#define _COUNT_IN_BYTES 0

#include <mbsncat_s.inl>

#else  /* _SYSCRT */

#include <internal.h>

_REDIRECT_TO_L_VERSION_4(errno_t, _mbsncat_s, unsigned char *, size_t, const unsigned char *, size_t)

#endif  /* _SYSCRT */

#endif  /* _MBCS */
