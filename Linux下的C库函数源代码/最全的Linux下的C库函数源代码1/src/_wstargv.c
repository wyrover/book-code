/***
*_wstargv.c - Wildcard argv[] expansion (wchar_t version)
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       _wsetargv performs wildcard argv[] expansion
*
*       NOTE:  This stub module scheme is compatible with NT build
*       procedure.
*
*******************************************************************************/


#define WPRFLAG 1

#ifndef _UNICODE
#define _UNICODE 1
#endif  /* _UNICODE */

#ifndef UNICODE
#define UNICODE 1
#endif  /* UNICODE */

#undef _MBCS /* UNICODE not _MBCS */

#define WILDCARD 1

#include "stdargv.c"

