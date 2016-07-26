/***
*labs.c - find absolute value of a long integer
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines labs() - find absolute value of a long integer.
*
*******************************************************************************/

#include <cruntime.h>
#include <stdlib.h>

#ifdef _MSC_VER
#pragma function(labs)
#endif  /* _MSC_VER */

/***
*long labs(lnumber) - find absolute value of long.
*
*Purpose:
*       Find the absolute value of a long integer (lnumber if lnumber >= 0),
*       -lnumber if lnumber < 0).
*
*Entry:
*       long lnumber - number to find absolute value of
*
*Exit:
*       returns the absolute value of lnumber
*
*Exceptions:
*
*******************************************************************************/

long __cdecl labs (
        long lnumber
        )
{
        return( lnumber>=0L ? lnumber : -lnumber );
}
