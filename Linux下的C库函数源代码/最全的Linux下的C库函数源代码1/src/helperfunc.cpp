/***
*crtexe.c - Initialization for console EXE using CRT DLL
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This is the actual startup routine for apps linking to the CRT DLL.
*       It calls the user's main routine [w]main() or [w]WinMain after
*       performing C Run-Time Library initialization.
*
*       With ifdefs, this source file also provides the source code for:
*       wcrtexe.c   the startup routine for console apps with wide chars
*       crtexew.c   the startup routine for Windows apps
*       wcrtexew.c  the startup routine for Windows apps with wide chars
*
*******************************************************************************/

#undef  CRTDLL
#ifdef MRTDLL
#undef MRTDLL
#endif  /* MRTDLL */

#define SPECIAL_CRTEXE

#include <cruntime.h>
#include <internal.h>
#include <math.h>
#include <stdlib.h>
#include <float.h>

#pragma warning(disable:4439)   // C4439: function with a managed parameter must have a __clrcall calling convention

extern "C"
{
int __defaultmatherr;
_MRTIMP void __cdecl __setusermatherr_m( int (__clrcall *)(struct _exception *) );
}

extern "C" void __clrcall __set_matherr()
{
    if ( !__defaultmatherr )
        __setusermatherr_m(_matherr);
}

#ifdef ENABLE_EHTRACE
int __ehtrace_level;
#endif  /* ENABLE_EHTRACE */
