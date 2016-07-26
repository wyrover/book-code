/***
*mbtohira.c - Convert character from katakana to hiragana (Japanese).
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _jtohira() - convert character to hiragana.
*
*******************************************************************************/

#ifdef _MBCS

#include <mtdll.h>
#include <cruntime.h>
#include <mbdata.h>
#include <mbstring.h>
#include <mbctype.h>
#include <stdlib.h>


/***
*unsigned int _mbctohira(c) - Converts character to hiragana.
*
*Purpose:
*       Converts the character c from katakana to hiragana, if possible.
*
*Entry:
*       unsigned int c - Character to convert.
*
*Exit:
*       Returns the converted character.
*
*Exceptions:
*
*******************************************************************************/

unsigned int __cdecl _mbctohira_l(
        unsigned int c,
        _locale_t plocinfo
        )
{
        if (_ismbckata_l(c, plocinfo) && c <= 0x8393) {
                if (c < 0x837f)
                        c -= 0xa1;
                else
                        c -= 0xa2;
        }
        return(c);
}

unsigned int (__cdecl _mbctohira)(
        unsigned int c
        )
{
    return _mbctohira_l(c, NULL);
}

#endif  /* _MBCS */
