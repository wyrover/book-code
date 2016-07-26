/***
*mbsset_s.c - Sets all charcaters of string to given character (MBCS)
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Sets all charcaters of string to given character (MBCS)
*
*******************************************************************************/

#ifdef _MBCS

#include <mbstring.h>

#ifdef _SYSCRT

#include <internal_safecrt.h>

#define _FUNC_PROLOGUE
#define _FUNC_NAME _mbsset_s
#define _DEST _Dst

#include <mbsset_s.inl>

#else  /* _SYSCRT */

#include <internal.h>

_REDIRECT_TO_L_VERSION_3(errno_t, _mbsset_s, unsigned char *, size_t, unsigned int)

#endif  /* _SYSCRT */

#endif  /* _MBCS */
