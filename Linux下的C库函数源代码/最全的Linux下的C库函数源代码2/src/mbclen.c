/***
*mbclen.c - Find length of MBCS character
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Find length of MBCS character
*
*******************************************************************************/

#include <mtdll.h>
#include <cruntime.h>
#include <mbdata.h>
#include <mbctype.h>
#include <mbstring.h>
#include <stddef.h>


/***
* _mbclen - Find length of MBCS character
*
*Purpose:
*       Find the length of the MBCS character (in bytes).
*
*Entry:
*       unsigned char *c = MBCS character
*
*Exit:
*       Returns the number of bytes in the MBCS character
*
*Exceptions:
*
*******************************************************************************/

size_t __cdecl _mbclen_l(
        const unsigned char *c,
        _locale_t plocinfo
        )

{
        /*  Don't return two if we have leadbyte, EOS.
            Don't assert here; too low level
        */
        return ((_ismbblead_l)(*c, plocinfo) && c[1]!='\0')  ? 2 : 1;
}

size_t (__cdecl _mbclen)(
        const unsigned char *c
        )

{
        /*  Don't return two if we have leadbyte, EOS.
            Don't assert here; too low level
        */
        return (_ismbblead(*c) && c[1]!='\0')  ? 2 : 1;
}
