/***
*swab.c - block copy, while swapping even/odd bytes
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This module contains the routine _swab() which swaps the odd/even
*       bytes of words during a block copy.
*
*******************************************************************************/

#include <cruntime.h>
#include <stdlib.h>
#include <internal.h>

/***
*void _swab(srcptr, dstptr, nbytes) - swap ODD/EVEN bytes during word move
*
*Purpose:
*       This routine copys a block of words and swaps the odd and even
*       bytes.  nbytes must be > 0, otherwise nothing is copied.  If
*       nbytes is odd, then only (nbytes-1) bytes are copied.
*
*Entry:
*       srcptr = pointer to the source block
*       dstptr = pointer to the destination block
*       nbytes = number of bytes to swap
*
*Returns:
*       None.
*
*Exceptions:
*
*******************************************************************************/

void __cdecl _swab
(
        char *src,
        char *dest,
        int nbytes
)
{
        char b1='\0';
        char b2='\0';

        _VALIDATE_RETURN_VOID(src!=NULL, EINVAL);
        _VALIDATE_RETURN_VOID(dest!=NULL, EINVAL);
        _VALIDATE_RETURN_VOID(nbytes>=0, EINVAL);

        while (nbytes > 1) {
                b1 = *src++;
                b2 = *src++;
                *dest++ = b2;
                *dest++ = b1;
                nbytes -= 2;
        }
}
