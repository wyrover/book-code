/***
*noenv.c - stub out CRT's environment string processing
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Stub out the environment string processing normally carried out at
*       during startup. Note, getenv, _putenv and _environ are not supported
*       if this object is used. Nor is the third argument to main.
*
*******************************************************************************/

#include <stdlib.h>


#ifdef _M_CEE_PURE
#define _CALLING __clrcall
#else  /* _M_CEE_PURE */
#define _CALLING __cdecl
#endif  /* _M_CEE_PURE */

int _CALLING _setenvp(void) { return 0; }

void * _CALLING __crtGetEnvironmentStringsA(void) { return NULL; }

int _CALLING _wsetenvp(void) { return 0; }

void * _CALLING __crtGetEnvironmentStringsW(void) { return NULL; }
