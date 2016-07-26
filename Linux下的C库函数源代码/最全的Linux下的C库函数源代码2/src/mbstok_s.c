/***
*mbstok_s.c - Break string into tokens (MBCS)
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Break string into tokens (MBCS)
*
*******************************************************************************/

#ifdef _MBCS

#include <mbstring.h>
#include <internal_securecrt.h>
#include <mtdll.h>
#include <setlocal.h>

#define _FUNC_PROLOGUE
#define _FUNC_NAME _mbstok_s_l
#define _SB_FUNC_NAME strtok_s

#include <mbstok_s.inl>

_REDIRECT_TO_L_VERSION_3(unsigned char *, _mbstok_s, unsigned char *, const unsigned char *, unsigned char **)

#endif  /* _MBCS */
