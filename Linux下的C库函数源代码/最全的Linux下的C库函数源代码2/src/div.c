/***
*div.c - contains the div routine
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Performs a signed divide and returns quotient
*       and remainder.
*
*******************************************************************************/

#include <cruntime.h>
#include <stdlib.h>

/***
*div_t div(int numer, int denom) - do signed divide
*
*Purpose:
*       This routine does an divide and returns the results.
*       Since we don't know how the Intel 860 does division, we'd
*       better make sure that we have done it right.
*
*Entry:
*       int numer - Numerator passed in on stack
*       int denom - Denominator passed in on stack
*
*Exit:
*       returns quotient and remainder in structure
*
*Exceptions:
*       No validation is done on [denom]* thus, if [denom] is 0,
*       this routine will trap.
*
*******************************************************************************/

div_t __cdecl div (
        int numer,
        int denom
        )
{
        div_t result;

        result.quot = numer / denom;
        result.rem = numer % denom;

        if (numer < 0 && result.rem > 0) {
                /* did division wrong; must fix up */
                ++result.quot;
                result.rem -= denom;
        }

        return result;
}
