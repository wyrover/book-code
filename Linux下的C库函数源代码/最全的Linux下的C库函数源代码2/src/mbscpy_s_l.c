/***
*mbscpy_s_l.c - Copy one string to another (MBCS)
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Copy one string to another (MBCS)
*
*******************************************************************************/

#ifdef _MBCS

#include <mbstring.h>
#include <internal_securecrt.h>
#include <mtdll.h>
#include <setlocal.h>

#define _FUNC_PROLOGUE
#define _FUNC_NAME _mbscpy_s_l
#define _SB_FUNC_NAME strcpy_s
#define _DEST _Dst
#define _SRC _Src

#include <mbscpy_s.inl>

#endif  /* _MBCS */
