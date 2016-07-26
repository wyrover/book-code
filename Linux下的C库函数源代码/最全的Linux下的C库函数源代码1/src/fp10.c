/***
* fp10.c - Set default FP precision to 64 bits (10-byte 'long double')
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*
*******************************************************************************/
#include <float.h>
#include <internal.h>

void  _setdefaultprecision(void);

/*
 * Routine to set default FP precision to 64 bits, used to override
 * standard 53-bit precision version in fpinit.c.
 */

void _setdefaultprecision()
{
#ifndef _M_AMD64
    _ERRCHECK(_controlfp_s(NULL, _PC_64, _MCW_PC));
#endif  /* _M_AMD64 */
}

