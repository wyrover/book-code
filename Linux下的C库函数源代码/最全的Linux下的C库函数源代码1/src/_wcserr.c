/***
*_wcserr.c - routine for indexing into system error list
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Returns system error message index by errno; conforms to the
*       XENIX standard, much compatibility with 1983 uniforum draft standard.
*       (wide char version of _strerror())
*
*******************************************************************************/


#ifndef _UNICODE
#define _UNICODE 1
#endif  /* _UNICODE */

#ifndef UNICODE
#define UNICODE 1
#endif  /* UNICODE */

#undef _MBCS /* UNICODE not _MBCS */

#include "_strerr.c"


