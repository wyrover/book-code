/***
*wstrtime.c - contains the function "_wstrtime()"
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       contains the function _wstrtime()
*
*******************************************************************************/


#ifndef _UNICODE
#define _UNICODE 1
#endif  /* _UNICODE */

#ifndef UNICODE
#define UNICODE 1
#endif  /* UNICODE */

#undef _MBCS /* UNICODE not _MBCS */

#include "strtime.c"

