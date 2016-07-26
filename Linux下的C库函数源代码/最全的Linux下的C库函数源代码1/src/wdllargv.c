/***
*wdllargv.c - Dummy _wsetargv() routine for use with C Run-Time as a DLL (CRTDLL)
*             (wchar_t version)
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This object goes into CRTDLL.LIB, which is linked with user programs
*       to use CRTDLL.DLL for C run-time library functions.  If the user
*       program links explicitly with WSETARGV.OBJ, this object will not be
*       linked in, and the _wsetargv() that does get called with set the flag
*       that will enable wildcard expansion.  If WSETARGV.OBJ is not linked
*       into the EXE, this object will get called by the CRT start-up stub
*       and the flag to enable wildcard expansion will not be set.
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

#include "dllargv.c"

