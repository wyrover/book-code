/***
*wstat64.c - get file status (wchar_t version)
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _wstat64() - get file status
*
*******************************************************************************/


#define WPRFLAG 1

#ifndef _UNICODE
#define _UNICODE 1
#endif  /* _UNICODE */

#ifndef UNICODE
#define UNICODE 1
#endif  /* UNICODE */

#undef  _MBCS       /* UNICODE not _MBCS */

#include "stat64.c"

