/***
*ismbstrail.c - True _ismbstrail function
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Contains the function _ismbstrail, which is a true context-sensitive
*       MBCS trail-byte function.  While much less efficient than _ismbbtrail,
*       it is also much more sophisticated, in that it determines whether a
*       given sub-string pointer points to a trail byte or not, taking into
*       account the context in the string.
*
*******************************************************************************/

#ifdef _MBCS

#include <mtdll.h>
#include <cruntime.h>
#include <stddef.h>
#include <mbdata.h>
#include <mbctype.h>
#include <mbstring.h>
#include <internal.h>
#include <locale.h>
#include <setlocal.h>


/***
* int _ismbstrail(const unsigned char *string, const unsigned char *current);
*
*Purpose:
*
*       _ismbstrail - Check, in context, for MBCS trail byte
*
*Entry:
*       unsigned char *string   - ptr to start of string or previous known lead byte
*       unsigned char *current  - ptr to position in string to be tested
*
*Exit:
*       TRUE    : -1
*       FALSE   : 0
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

extern "C" int __cdecl _ismbstrail_l(
        const unsigned char *string,
        const unsigned char *current,
        _locale_t plocinfo
        )
{
        /* validation section */
        _VALIDATE_RETURN(string != NULL, EINVAL, 0);
        _VALIDATE_RETURN(current != NULL, EINVAL, 0);

        _LocaleUpdate _loc_update(plocinfo);

        if (_loc_update.GetLocaleT()->mbcinfo->ismbcodepage == 0)
            return 0;

        while ( string <= current && *string ) {
            if ( _ismbblead_l((*string), _loc_update.GetLocaleT()) ) {
                if (++string == current)        /* check trail byte */
                    return -1;
                if (!(*string))
                    return 0;
            }
            ++string;
        }

        return 0;
}
extern "C" int (__cdecl _ismbstrail)(
        const unsigned char *string,
        const unsigned char *current
        )
{
        return _ismbstrail_l(string, current, NULL);
}

#endif  /* _MBCS */
