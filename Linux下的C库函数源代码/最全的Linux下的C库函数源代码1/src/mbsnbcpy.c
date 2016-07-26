/***
*mbsnbcpy.c - Copy one string to another, n bytes only (MBCS)
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Copy one string to another, n bytes only (MBCS)
*
*******************************************************************************/

#ifdef _MBCS

#include <mtdll.h>
#include <cruntime.h>
#include <string.h>
#include <mbdata.h>
#include <mbctype.h>
#include <mbstring.h>
#include <internal.h>
#include <locale.h>
#include <setlocal.h>

/***
* _mbsnbcpy - Copy one string to another, n bytes only (MBCS)
*
*Purpose:
*       Copies exactly cnt bytes from src to dst.  If strlen(src) < cnt, the
*       remaining character are padded with null bytes.  If strlen >= cnt, no
*       terminating null byte is added.  2-byte MBCS characters are handled
*       correctly.
*
*Entry:
*       unsigned char *dst = destination for copy
*       unsigned char *src = source for copy
*       int cnt = number of bytes to copy
*
*Exit:
*       returns dst = destination of copy
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

unsigned char * __cdecl _mbsnbcpy_l(
        unsigned char *dst,
        const unsigned char *src,
        size_t cnt,
        _locale_t plocinfo
        )
{

        unsigned char *start = dst;
        _LocaleUpdate _loc_update(plocinfo);

        /* validation section */
        _VALIDATE_RETURN(dst != NULL || cnt == 0, EINVAL, NULL);
        _VALIDATE_RETURN(src != NULL || cnt == 0, EINVAL, NULL);

        _BEGIN_SECURE_CRT_DEPRECATION_DISABLE
        if (_loc_update.GetLocaleT()->mbcinfo->ismbcodepage == 0)
            return (unsigned char *)strncpy((char *)dst, (const char *)src, cnt);
        _END_SECURE_CRT_DEPRECATION_DISABLE

        while (cnt) {

            cnt--;
            if ( _ismbblead_l(*src, _loc_update.GetLocaleT()) ) {
                *dst++ = *src++;
                if (!cnt) {
                    dst[-1] = '\0';
                    break;
                }
                cnt--;
                if ((*dst++ = *src++) == '\0') {
                    dst[-2] = '\0';
                    break;
                }
            }

            else
                if ((*dst++ = *src++) == '\0')
                    break;

        }

        /* pad with nulls as needed */

        while (cnt--)
            *dst++ = '\0';

        return start;
}
unsigned char * (__cdecl _mbsnbcpy)(
        unsigned char *dst,
        const unsigned char *src,
        size_t cnt
        )
{
    _BEGIN_SECURE_CRT_DEPRECATION_DISABLE
    return _mbsnbcpy_l(dst, src, cnt, NULL);
    _END_SECURE_CRT_DEPRECATION_DISABLE
}

#endif  /* _MBCS */
