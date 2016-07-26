/***
*mbscmp.c - Compare MBCS strings
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Compare MBCS strings
*
*******************************************************************************/

#ifdef _MBCS

#include <mtdll.h>
#include <cruntime.h>
#include <mbdata.h>
#include <mbctype.h>
#include <string.h>
#include <mbstring.h>
#include <internal.h>
#include <locale.h>
#include <setlocal.h>


/***
* _mbscmp - Compare MBCS strings
*
*Purpose:
*       Compares two strings for lexical order.   Strings
*       are compared on a character basis, not a byte basis.
*
*Entry:
*       char *s1, *s2 = strings to compare
*
*Exit:
*       Returns <0 if s1 < s2
*       Returns  0 if s1 == s2
*       Returns >0 if s1 > s2
*       Returns _NLSCMPERROR is something went wrong
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

extern "C" int __cdecl _mbscmp_l(
        const unsigned char *s1,
        const unsigned char *s2,
        _locale_t plocinfo
        )
{
        unsigned short c1, c2;
        _LocaleUpdate _loc_update(plocinfo);

        /* validation section */
        _VALIDATE_RETURN(s1 != NULL, EINVAL, _NLSCMPERROR);
        _VALIDATE_RETURN(s2 != NULL, EINVAL, _NLSCMPERROR);
        if (_loc_update.GetLocaleT()->mbcinfo->ismbcodepage == 0)
            return strcmp((const char *)s1, (const char *)s2);

        for (;;) {

            c1 = *s1++;
            if ( _ismbblead_l(c1, _loc_update.GetLocaleT()) )
                c1 = ( (*s1 == '\0') ? 0 : ((c1<<8) | *s1++) );

            c2 = *s2++;
            if ( _ismbblead_l(c2, _loc_update.GetLocaleT()) )
                c2 = ( (*s2 == '\0') ? 0 : ((c2<<8) | *s2++) );

            if (c1 != c2)
                return (c1 > c2) ? 1 : -1;

            if (c1 == 0)
                return 0;

        }
}

extern "C" int (__cdecl _mbscmp)(
        const unsigned char *s1,
        const unsigned char *s2
        )
{
    return _mbscmp_l(s1, s2, NULL);
}

#endif  /* _MBCS */
