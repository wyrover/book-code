/***
*wgetcwd.c - get current working directory (wchar_t version)
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*
*       contains functions _getcwd, _getdcwd and _getcdrv for getting the
*       current working directory.  getcwd gets the c.w.d. for the default disk
*       drive, whereas _getdcwd allows one to get the c.w.d. for whatever disk
*       drive is specified. _getcdrv gets the current drive.
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

#include "getcwd.c"

