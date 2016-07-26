/***
*mbsninc.c - Increment MBCS string pointer by specified char count.
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Increment MBCS string pointer by specified char count.
*
*******************************************************************************/

#ifdef _MBCS

#include <mtdll.h>
#include <cruntime.h>
#include <mbdata.h>
#include <mbstring.h>
#include <stddef.h>

/***
*_mbsninc - Increment MBCS string pointer by specified char count.
*
*Purpose:
*       Increment the supplied string pointer by the specified number
*       of characters.  MBCS characters are handled correctly.
*
*Entry:
*       const unsigned char *string = pointer to string
*       unsigned int ccnt = number of char to advance the pointer
*
*Exit:
*       Returns pointer after advancing it.
*       Returns pointer to end of string if string is not ccnt chars long.
*       Returns NULL is supplied pointer is NULL.
*
*Exceptions:
*
*******************************************************************************/

unsigned char * __cdecl _mbsninc_l(
        const unsigned char *string,
        size_t ccnt,
        _locale_t plocinfo
        )
{
        if (string == NULL)
                return(NULL);

        return((char *)string + (unsigned int)_mbsnbcnt_l(string, ccnt, plocinfo));
}

unsigned char * (__cdecl _mbsninc)(
        const unsigned char *string,
        size_t ccnt
        )
{
    return _mbsninc_l(string, ccnt, NULL);
}

#endif  /* _MBCS */
