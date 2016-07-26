/***
*mbscat_s_l.c - Concatenate one string to another (MBCS)
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Concatenate one string to another (MBCS)
*
*******************************************************************************/

#ifdef _MBCS

#include <mbstring.h>
#include <internal_securecrt.h>
#include <mtdll.h>
#include <setlocal.h>

#define _FUNC_PROLOGUE
#define _FUNC_NAME _mbscat_s_l
#define _SB_FUNC_NAME strcat_s
#define _DEST _Dst
#define _SRC _Src

#include <mbscat_s.inl>

#endif  /* _MBCS */
