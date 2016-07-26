/***
*dllargv.c - Dummy _setargv() routine for use with C Run-Time as a DLL (CRTDLL)
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This object goes into CRTDLL.LIB, which is linked with user programs
*       to use CRTDLL.DLL for C run-time library functions.  If the user
*       program links explicitly with SETARGV.OBJ, this object will not be
*       linked in, and the _setargv() that does get called with set the flag
*       that will enable wildcard expansion.  If SETARGV.OBJ is not linked
*       into the EXE, this object will get called by the CRT start-up stub
*       and the flag to enable wildcard expansion will not be set.
*
*******************************************************************************/


#if defined (CRTDLL) || defined (MRTDLL)

#ifdef CRTDLL
#undef CRTDLL
#endif  /* CRTDLL */

#ifdef MRTDLL
#undef MRTDLL
#endif  /* MRTDLL */

#include <cruntime.h>
#include <internal.h>

/***
*_setargv - dummy version for CRTDLL.DLL model only
*
*Purpose:
*       This routine gets called by the C Run-Time start-up code in CRTEXE.C
*       which gets linked into an EXE file linked with CRTDLL.LIB.  It does
*       nothing, but if the user links the EXE with SETARGV.OBJ, this routine
*       will not get called but instead __setargv() will be called.  (In the
*       CRTDLL model, it will set the variable that is passed to _GetMainArgs
*       and enable wildcard expansion in the command line arguments.)
*
*Entry:
*
*Exit:
*       Always return 0 (full version in DLL code returns -1 on error)
*
*Exceptions:
*
*******************************************************************************/

#ifdef WPRFLAG
int __CRTDECL _wsetargv ( void )
#else  /* WPRFLAG */
int __CRTDECL _setargv ( void )
#endif  /* WPRFLAG */
{
        return 0;
}

#endif  /* defined (CRTDLL) || defined (MRTDLL) */

