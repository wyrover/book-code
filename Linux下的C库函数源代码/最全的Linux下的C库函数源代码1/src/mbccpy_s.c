/***
*mbccpy_s.c - Copy one character  to another (MBCS)
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Copy one MBCS character to another (1 or 2 bytes)
*
*******************************************************************************/

#include <mbstring.h>

#ifdef _SYSCRT

#include <internal_safecrt.h>

#define _FUNC_PROLOGUE
#define _FUNC_NAME _mbccpy_s
#define _DEST _Dst
#define _SRC _Src

#include <mbccpy_s.inl>

#else  /* _SYSCRT */

#include <internal.h>

_REDIRECT_TO_L_VERSION_4(errno_t, _mbccpy_s, unsigned char *, size_t , int *, const unsigned char *)

#endif  /* _SYSCRT */
