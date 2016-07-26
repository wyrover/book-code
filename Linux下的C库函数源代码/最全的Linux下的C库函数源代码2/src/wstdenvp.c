/***
*wstdenvp.c - standard _setenvp routine (wchar_t version)
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This module is called by the C start-up routine to set up "_wenviron".
*       Its sets up an array of pointers to strings in the environment.
*       The global symbol "_wenviron" is set to point to this array.
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

#include "stdenvp.c"

