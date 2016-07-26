/***
* loosefpmath.c - Set DAZ and FTZ for SSE2 architectures
*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*
*******************************************************************************/
#include <float.h>
#include <sect_attribs.h>
#include <internal.h>

int __set_loosefpmath(void);

int __set_loosefpmath(void)
{
    _ERRCHECK(_controlfp_s(NULL, _DN_FLUSH, _MCW_DN));
    return 0;
}

_CRTALLOC(".CRT$XID") static _PIFV pinit = __set_loosefpmath;
