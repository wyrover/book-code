/***
*mbsset_s_l.c - Sets all charcaters of string to given character (MBCS)
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Sets all charcaters of string to given character (MBCS)
*
*******************************************************************************/

#ifdef _MBCS

#include <mbstring.h>
#include <internal_securecrt.h>
#include <mtdll.h>
#include <setlocal.h>

#define _FUNC_PROLOGUE
#define _FUNC_NAME _mbsset_s_l
#define _SB_FUNC_NAME _strset_s
#define _DEST _Dst

#include <mbsset_s.inl>

#endif  /* _MBCS */
