/***
*rotl.c - rotate an unsigned integer left
*
*   Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*   defines _rotl() - performs a rotate left on an unsigned integer.
*
*******************************************************************************/


#include <cruntime.h>
#include <stdlib.h>
#include <limits.h>

#ifdef _MSC_VER
#pragma function(_lrotl,_rotl, _rotl64)
#endif  /* _MSC_VER */

#if UINT_MAX != 0xffffffff
#error This module assumes 32-bit integers
#endif  /* UINT_MAX != 0xffffffff */

#if UINT_MAX != ULONG_MAX
#error This module assumes sizeof(int) == sizeof(long)
#endif  /* UINT_MAX != ULONG_MAX */

/***
*unsigned _rotl(val, shift) - int rotate left
*
*Purpose:
*   Performs a rotate left on an unsigned integer.
*
*   [Note:  The _lrotl entry is based on the assumption
*   that sizeof(int) == sizeof(long).]
*Entry:
*   unsigned val:   value to rotate
*   int    shift:   number of bits to shift by
*
*Exit:
*   returns rotated value
*
*Exceptions:
*   None.
*
*******************************************************************************/

unsigned long __cdecl _lrotl (
    unsigned long val,
    int shift
    )
{
    shift &= 0x1f;
    val = (val>>(0x20 - shift)) | (val << shift);
    return val;
}

unsigned __cdecl _rotl (
    unsigned val,
    int shift
    )
{
    shift &= 0x1f;
    val = (val>>(0x20 - shift)) | (val << shift);
    return val;
}

unsigned __int64 __cdecl _rotl64 (
    unsigned __int64 val,
    int shift
    )
{
    shift &= 0x3f;
    val = (val>>(0x40 - shift)) | (val << shift);
    return val;
}
