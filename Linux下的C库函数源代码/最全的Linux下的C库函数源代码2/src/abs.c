/***
*abs.c - find absolute value
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines abs() - find the absolute value of an int.
*
*******************************************************************************/

#include <cruntime.h>
#include <stdlib.h>

#pragma function(abs, _abs64)

/***
*int abs(number) - find absolute value of number
*
*Purpose:
*       Returns the absolute value of number (if number >= 0, returns number,
*       else returns -number).
*
*Entry:
*       int number - number to find absolute value of
*
*Exit:
*       returns the aboslute value of number
*
*Exceptions:
*
*******************************************************************************/

int __cdecl abs (
        int number
        )
{
        return( number>=0 ? number : -number );
}
__int64 __cdecl _abs64(
        __int64 num
        )
{
        return (num >=0 ? num : -num);
}
