/***
*wwincrt0.c - C runtime Windows EXE start-up routine, wchar_t version
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This is the actual startup routine for Windows apps using wide
*       characters.  It calls the user's main routine wWinMain() after
*       performing C Run-Time Library initialization.
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

#define _WINMAIN_
#include "crt0.c"

