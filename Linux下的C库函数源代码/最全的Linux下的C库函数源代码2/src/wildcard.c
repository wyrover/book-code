/***
*wildcard.c - define the CRT internal variable _dowildcard
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This variable is not public to users but is defined outside the
*       start-up code (CRTEXE.C) to reduce duplicate definitions.
*
*******************************************************************************/

#if (defined (CRTDLL) || defined (MRTDLL))

#ifdef CRTDLL
#undef CRTDLL
#endif  /* CRTDLL */

#ifdef MRTDLL
#undef MRTDLL
#endif  /* MRTDLL */

#include <internal.h>

int _dowildcard = 0;    /* should be in <internal.h> */

#endif  /* (defined (CRTDLL) || defined (MRTDLL)) */
