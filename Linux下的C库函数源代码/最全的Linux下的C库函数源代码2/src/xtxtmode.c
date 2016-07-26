/***
*xtxtmode.c - set global text mode flag
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Sets the global file mode to text.  This is the default.
*
*       This is a special version for the DLL model.  This object goes into
*       MSVCRT.LIB (and therefore into the client EXE) and not into the
*       CRTL in a DLL (MSVCRTXX.DLL).  It is identical to txtmode.obj
*       except that the latter has a DLL export definition in the DLL model.
*
*******************************************************************************/

#define SPECIAL_CRTEXE  /* turn off _CRTIMP for decl. of _fmode */

#include <cruntime.h>
#include <stdlib.h>

int _fmode = 0;                 /* set text mode */
