/***
*wdll_av.c - __wsetargv() routine for use with C Run-Time as a DLL (CRTDLL)
*            (wchar_t version)
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This object is part of the start-up code for EXE's linked with
*       CRTDLL.LIB/MSVCRT.LIB.  This object will be linked into the user
*       EXE if and only if the user explicitly links with WSETARGV.OBJ.
*       The code in this object sets the flag that is passed to the
*       C Run-Time DLL to enable wildcard expansion of the argv[] vector.
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

#include "dll_argv.c"

