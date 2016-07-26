/***
*wstrdate.c - contains the function "_wstrdate()" (wchar_t version)
*             and "_wstrdate_s" (wchar_t secure version)
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       contains the function _wstrdate() and _wstrdate_s
*
*******************************************************************************/


#ifndef _UNICODE
#define _UNICODE 1
#endif  /* _UNICODE */

#ifndef UNICODE
#define UNICODE 1
#endif  /* UNICODE */

#undef _MBCS /* UNICODE not _MBCS */

#include "strdate.c"

