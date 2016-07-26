/***
*wcserror.c - Contains the _wcserror C runtime.
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       The _wcserror runtime accepts an error number as input
*       and returns the corresponding error string.
*       (wide char version of strerror)
*
*******************************************************************************/


#ifndef _UNICODE
#define _UNICODE 1
#endif  /* _UNICODE */

#ifndef UNICODE
#define UNICODE 1
#endif  /* UNICODE */

#undef _MBCS /* UNICODE not _MBCS */

#include "strerror.c"

