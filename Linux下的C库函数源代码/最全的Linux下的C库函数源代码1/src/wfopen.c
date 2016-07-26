/***
*wfopen.c - open a file (wchar_t version)
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _wfopen() and _wfsopen() - open a file as a stream and open a file
*       with a specified sharing mode as a stream
*
*******************************************************************************/


#ifndef _UNICODE
#define _UNICODE 1
#endif  /* _UNICODE */

#ifndef UNICODE
#define UNICODE 1
#endif  /* UNICODE */

#undef _MBCS /* UNICODE not _MBCS */

#include "fopen.c"

