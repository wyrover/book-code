/***
*xncommode.c - set global file commit mode flag to nocommit
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Sets the global file commit mode flag to nocommit.  This is the default.
*
*       This is a special version for the DLL model.  This object goes into
*       MSVCRT.LIB (and therefore into the client EXE) and not into the
*       CRTL in a DLL (MSVCRTXX.DLL).  It is identical to ncommode.obj
*       except that the latter has a DLL export definition in the DLL model.
*
*******************************************************************************/

#define SPECIAL_CRTEXE  /* turn off _CRTIMP for decl. of _commode */
#if !defined (_DLL)
#define _DLL            /* also necessary to turn off _CRTIMP */
#endif  /* !defined (_DLL) */

#ifdef CRTDLL
#undef CRTDLL
#endif  /* CRTDLL */

#ifdef MRTDLL
#undef MRTDLL
#endif  /* MRTDLL */

#include <cruntime.h>
#include <internal.h>

/* set default file commit mode to nocommit */
int _commode = 0;
