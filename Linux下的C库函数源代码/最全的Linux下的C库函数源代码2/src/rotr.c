/***
*rotr.c - rotate an unsigned integer right
*
*   Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*   defines _rotr() - performs a rotate right on an unsigned integer.
*
*******************************************************************************/


#include <cruntime.h>
#include <stdlib.h>
#include <limits.h>

#ifdef _MSC_VER
#pragma function(_lrotr,_rotr, _rotr64)
#endif  /* _MSC_VER */

#if UINT_MAX != 0xffffffff
#error This module assumes 32-bit integers
#endif  /* UINT_MAX != 0xffffffff */

#if UINT_MAX != ULONG_MAX
#error This module assumes sizeof(int) == sizeof(long)
#endif  /* UINT_MAX != ULONG_MAX */

/***
*unsigned _rotr(val, shift) - int rotate right
*
*Purpose:
*   Performs a rotate right on an unsigned integer.
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

unsigned long __cdecl _lrotr (
    unsigned long val,
    int shift
    )
{
    shift &= 0x1f;
    val = (val<<(0x20 - shift)) | (val >> shift);
    return val;
}

unsigned __cdecl _rotr (
    unsigned val,
    int shift
    )
{
    shift &= 0x1f;
    val = (val<<(0x20 - shift)) | (val >> shift);
    return val;
}

unsigned __int64 __cdecl _rotr64 (
    unsigned __int64 val,
    int shift
    )
{
    shift &= 0x3f;
    val = (val<<(0x40 - shift)) | (val >> shift);
    return val;
}
