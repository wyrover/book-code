/***
*mbsncat_s_l.c - concatenate string2 onto string1, max length n
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       defines mbsncat_s_l() - concatenate maximum of n characters
*
*******************************************************************************/

#ifdef _MBCS

#include <mbstring.h>
#include <internal_securecrt.h>
#include <mtdll.h>
#include <setlocal.h>

#define _FUNC_PROLOGUE
#define _FUNC_NAME _mbsncat_s_l
#define _SB_FUNC_NAME strncat_s
#define _DEST _Dst
#define _SRC _Src
#define _COUNT _CountInChars
#define _COUNT_IN_BYTES 0

#include <mbsncat_s.inl>

#endif  /* _MBCS */
