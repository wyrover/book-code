/***
*strncnt.c - contains __strncnt() routine
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       __strncnt returns the count characters in a string, up to n.
*   (used by _strncnt)
*
*******************************************************************************/


#include <cruntime.h>
#include <string.h>

#if defined (_M_IA64)
#pragma warning(disable:4163)
#pragma function(wcslen)
#endif  /* defined (_M_IA64) */

/***
*size_t __cdecl __strncnt - count characters in a string, up to n.
*
*Purpose:
*       Internal local support function. Counts characters in string before NULL.
*       If NULL not found in n chars, then return n.
*
*Entry:
*       const char *string   - start of string
*       int n                - byte count
*
*Exit:
*       returns number of bytes from start of string to
*       NULL (exclusive), up to n.
*
*Exceptions:
*
*******************************************************************************/

size_t __cdecl __strncnt (
        const char *string,
        size_t cnt
        )
{
        size_t n = cnt;
        char *cp = (char *)string;

        while (n-- && *cp)
            cp++;

        return cnt - n - 1;
}

