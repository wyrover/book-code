/***
*wfullpath.c - (wchar_t version)
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose: contains the function _wfullpath which makes an absolute path out
*       of a relative path. i.e.  ..\pop\..\main.c => c:\src\main.c if the
*       current directory is c:\src\src
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

#include "fullpath.c"

