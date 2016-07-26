/***
*_mbslen.c - Return number of multibyte characters in a multibyte string
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Return number of multibyte characters in a multibyte string
*       excluding the terminal null.  Locale-dependent.
*
*******************************************************************************/

#include <cruntime.h>
#include <stdlib.h>
#include <locale.h>
#include <dbgint.h>
#include <ctype.h>
#include <mbctype.h>
#include <internal.h>
#include <mtdll.h>
#include <setlocal.h>

/***
*_mbstrlen - Return number of multibyte characters in a multibyte string
*
*Purpose:
*       Return number of multibyte characters in a multibyte string
*       excluding the terminal null.  Locale-dependent.
*
*Entry:
*       char *s = string
*
*Exit:
*       Returns the number of multibyte characters in the string, or
*       Returns (size_t)-1 if the string contains an invalid multibyte character.
*       Also, errno is set to EILSEQ.
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

size_t __cdecl _mbstrlen_l(
        const char *s,
        _locale_t plocinfo
        )
{
    size_t n;
    _LocaleUpdate _loc_update(plocinfo);

    _ASSERTE (_loc_update.GetLocaleT()->locinfo->mb_cur_max == 1 || _loc_update.GetLocaleT()->locinfo->mb_cur_max == 2);

    if ( _loc_update.GetLocaleT()->locinfo->mb_cur_max == 1 )
        /* handle single byte character sets */
        return strlen(s);


    /* verify all valid MB chars */
    if ( MultiByteToWideChar( _loc_update.GetLocaleT()->locinfo->lc_codepage,
                              MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
                              s,
                              -1,
                              NULL,
                              0 ) == 0 )
    {
        /* bad MB char */
        errno = EILSEQ;
        return (size_t)-1;
    }

    /* count MB chars */
    for (n = 0; *s; n++, s++) {
        if ( _isleadbyte_l((unsigned char)*s, _loc_update.GetLocaleT()) )
        {
            if (*++s == '\0')
                break;
        }
    }


    return(n);
}

size_t __cdecl _mbstrlen(
        const char *s
        )
{
    if (__locale_changed == 0)
    {
        return strlen(s);
    }
    else
    {
        return _mbstrlen_l(s, NULL);
    }
}
