/***
*mblen.c - length of multibyte character
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Return the number of bytes contained in a multibyte character.
*
*******************************************************************************/


#include <internal.h>
#include <locale.h>
#include <cruntime.h>
#include <stdlib.h>
#include <ctype.h>
#include <dbgint.h>
#include <mtdll.h>
#include <setlocal.h>

/***
*int mblen() - length of multibyte character
*
*Purpose:
*       Return the number of bytes contained in a multibyte character.
*       [ANSI].
*
*Entry:
*       const char *s = pointer to multibyte character
*       size_t      n = maximum length of multibyte character to consider
*
*Exit:
*       If s = NULL, returns 0, indicating we use (only) state-independent
*       character encodings.
*
*       If s != NULL, returns:   0  (if *s = null char),
*                               -1  (if the next n or fewer bytes not valid
*                                   mbc),
*                               number of bytes contained in multibyte char
*
*Exceptions:
*
*******************************************************************************/

extern "C" int __cdecl _mblen_l
        (
        const char * s,
        size_t n,
        _locale_t plocinfo
        )
{
    if ( !s || !(*s) || (n == 0) )
        /* indicate do not have state-dependent encodings,
           empty string length is 0 */
        return 0;

    _LocaleUpdate _loc_update(plocinfo);

    _ASSERTE (_loc_update.GetLocaleT()->locinfo->mb_cur_max == 1 || _loc_update.GetLocaleT()->locinfo->mb_cur_max == 2);


    if ( _isleadbyte_l((unsigned char)*s, _loc_update.GetLocaleT()) )
    {
        /* multi-byte char */

        /* verify valid MB char */
        if ( _loc_update.GetLocaleT()->locinfo->mb_cur_max <= 1 ||
             (int)n < _loc_update.GetLocaleT()->locinfo->mb_cur_max ||
             MultiByteToWideChar( _loc_update.GetLocaleT()->locinfo->lc_codepage,
                                  MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
                                  s,
                                  _loc_update.GetLocaleT()->locinfo->mb_cur_max,
                                  NULL,
                                  0 ) == 0 )
            /* bad MB char */
            return -1;
        else
            return _loc_update.GetLocaleT()->locinfo->mb_cur_max;
    }
    else {
        /* single byte char */

        /* verify valid SB char */
        if ( MultiByteToWideChar( _loc_update.GetLocaleT()->locinfo->lc_codepage,
                                  MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
                                  s,
                                  1,
                                  NULL,
                                  0 ) == 0 )
            return -1;
        return sizeof(char);
    }

}

extern "C" int __cdecl mblen
        (
        const char * s,
        size_t n
        )
{
    if (__locale_changed == 0)
    {
        return _mblen_l(s, n, &__initiallocalestructinfo);
    }
    else
    {
        return _mblen_l(s, n, NULL);
    }
}
