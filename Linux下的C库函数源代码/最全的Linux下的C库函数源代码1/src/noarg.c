/***
*noarg.c - stub out CRT's processing of command line arguments
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Stub out the processing of the command line into argv[], normally
*       carried out at during startup. Note, the argc and argv arguments to
*       main are not meaningful if this object is used. Nor are __argc and
*       __argv.
*
*******************************************************************************/

#include <tchar.h>

#ifdef _M_CEE_PURE
#define _CALLING __clrcall
#else  /* _M_CEE_PURE */
#define _CALLING __cdecl
#endif  /* _M_CEE_PURE */

int _CALLING _setargv() { return 0; }

int _CALLING _wsetargv() { return 0; }

_TUCHAR * _CALLING _wincmdln() { return NULL; }

_TUCHAR * _CALLING _wwincmdln() { return NULL; }
