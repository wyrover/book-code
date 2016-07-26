/***
*_mbslen_s.c - Return number of multibyte characters in a multibyte string
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
#include <ctype.h>
#include <locale.h>
#include <dbgint.h>
#include <internal.h>
#include <mtdll.h>
#include <setlocal.h>

/***
*_mbstrnlen - Return number of multibyte characters in a multibyte string
*
*Purpose:
*       Return number of multibyte characters in a multibyte string
*       excluding the terminal null.  Locale-dependent.
*
*Entry:
*       char *s = string
*       size_t maxsize
*
*Exit:
*       Returns the number of multibyte characters in the string, or
*       (size_t)-1 if the string contains an invalid multibyte character and errno
*       is set to EILSEQ.
*       Only the first sizeInBytes bytes of the string are inspected: if the null
*       terminator is not found, sizeInBytes is returned.
*       If the string is null terminated in sizeInBytes bytes, the return value
*       will always be less than sizeInBytes.
*       If something goes wrong, (size_t)-1 is returned and errno is set to EINVAL.
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

size_t __cdecl _mbstrnlen_l(
        const char *s,
        size_t sizeInBytes,
        _locale_t plocinfo
        )
{
    size_t n, size;


    /* validation section */
    _VALIDATE_RETURN(s != NULL, EINVAL, (size_t)-1);
    _VALIDATE_RETURN(sizeInBytes <= INT_MAX, EINVAL, (size_t)-1);


    _LocaleUpdate _loc_update(plocinfo);

    _ASSERTE (_loc_update.GetLocaleT()->locinfo->mb_cur_max == 1 || _loc_update.GetLocaleT()->locinfo->mb_cur_max == 2);

    if ( _loc_update.GetLocaleT()->locinfo->mb_cur_max == 1 )
        /* handle single byte character sets */
        return (int)strnlen(s, sizeInBytes);


    /* verify all valid MB chars */
    if ( MultiByteToWideChar( _loc_update.GetLocaleT()->locinfo->lc_codepage,
                              MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
                              s,
                              (int)sizeInBytes,
                              NULL,
                              0 ) == 0 )
    {
        /* bad MB char */
        errno = EILSEQ;
        return (size_t)-1;
    }

    /* count MB chars */
    /* Note that sizeInBytes here is the number of bytes, not mb characters! */
    for (n = 0, size = 0; size < sizeInBytes && *s; n++, s++, size++)
    {
        if ( _isleadbyte_l((unsigned char)*s, _loc_update.GetLocaleT()) )
        {
                        size++;
                        if (size >= sizeInBytes)
                        {
                                break;
                        }
            if (*++s == '\0')
                        {
                break;
                        }
        }
    }


        return (size >= sizeInBytes ? sizeInBytes : n);
}

size_t __cdecl _mbstrnlen(
        const char *s,
        size_t maxsize
        )
{
    return _mbstrnlen_l(s, maxsize, NULL);
}
