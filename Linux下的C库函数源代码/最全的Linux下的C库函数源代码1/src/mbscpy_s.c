/***
*mbscpy_s.c - Copy one string to another (MBCS)
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Copy one string to another (MBCS)
*
*******************************************************************************/

#ifdef _MBCS

#include <mbstring.h>

#ifdef _SYSCRT

#include <internal_safecrt.h>

#define _FUNC_PROLOGUE
#define _FUNC_NAME _mbscpy_s
#define _DEST _Dst
#define _SRC _Src

#include <mbscpy_s.inl>

#else  /* _SYSCRT */

#include <internal.h>

_REDIRECT_TO_L_VERSION_3(errno_t, _mbscpy_s, unsigned char *, size_t, const unsigned char *)

#endif  /* _SYSCRT */

#endif  /* _MBCS */
