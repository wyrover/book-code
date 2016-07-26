/***
*iswctype.c - support isw* wctype functions/macros for wide characters
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines iswctype - support isw* wctype functions/macros for
*       wide characters (esp. > 255).
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <locale.h>
#include <awint.h>
#include <ctype.h>
#include <mtdll.h>
#include <setlocal.h>

/*
 *  Use GetStringTypeW() API so check that character type masks agree between
 *  ctype.h and winnls.h
 */
#if _UPPER != C1_UPPER  || \
        _LOWER != C1_LOWER  || \
        _DIGIT != C1_DIGIT  || \
        _SPACE != C1_SPACE  || \
        _PUNCT != C1_PUNCT  || \
        _CONTROL != C1_CNTRL
#error Character type masks do not agree in ctype and winnls
#endif  /* _UPPER != C1_UPPER  || \ */

/***
*iswctype - support isw* wctype functions/macros.
*
*Purpose:
*       This function is called by the isw* wctype functions/macros
*       (e.g. iswalpha()) when their argument is a wide character > 255.
*       It is also a standard ITSCJ (proposed) ISO routine and can be called
*       by the user, even for characters < 256.
*       Returns true or false depending on whether the argument satisfies
*       the character class property encoded by the mask.  Returns 0 if the
*       argument is WEOF.
*
*       NOTE: The isw* functions are neither locale nor codepage dependent.
*
*Entry:
*       wchar_t c    - the wide character whose type is to be tested
*       wchar_t mask - the mask used by the isw* functions/macros
*                       corresponding to each character class property
*
*Exit:
*       Returns non-zero if c is of the character class.
*       Returns 0 if c is not of the character class.
*
*Exceptions:
*       Returns 0 on any error.
*
*******************************************************************************/

extern "C" int __cdecl _iswctype_l (
        wint_t c,
        wctype_t mask,
        _locale_t plocinfo
        )
{
    wint_t d;

    if ( c == WEOF )
        d = 0;
    else if ( c < 256 )
        d = (int)(_pwctype[c] & mask);
    else
    {
        _LocaleUpdate _loc_update(plocinfo);

        if ( __crtGetStringTypeW(
                    _loc_update.GetLocaleT(),
                    CT_CTYPE1,
                    (LPCWSTR)&c,
                    1,
                    &d,
                    _loc_update.GetLocaleT()->locinfo->lc_codepage,
                    _loc_update.GetLocaleT()->locinfo->lc_handle[LC_CTYPE]
                    ) == 0)
            d = 0;
    }

    return (int)(d & mask);
}

extern "C" int __cdecl iswctype (
        wint_t c,
        wctype_t mask
        )
{
    if (c == WEOF)
    {
        return 0;
    }
    if (c < 256)
    {
        return (int)(_pwctype[c] & mask);
    }

    if (__locale_changed == 0)
    {
        wint_t d;
        if ( __crtGetStringTypeW(
                    &__initiallocalestructinfo,
                    CT_CTYPE1,
                    (LPCWSTR)&c,
                    1,
                    &d,
                    __initiallocinfo.lc_codepage,
                    __initiallocinfo.lc_handle[LC_CTYPE]
                    ) == 0)
            d = 0;
    }

    return _iswctype_l(c, mask, NULL);
}


/***
 *is_wctype - support obsolete name
 *
 *Purpose:
 *       Name changed from is_wctype to iswctype. is_wctype must be supported.
 *
 *Entry:
 *       wchar_t c    - the wide character whose type is to be tested
 *       wchar_t mask - the mask used by the isw* functions/macros
 *                       corresponding to each character class property
 *
 *Exit:
 *       Returns non-zero if c is of the character class.
 *       Returns 0 if c is not of the character class.
 *
 *Exceptions:
 *       Returns 0 on any error.
 *
 *******************************************************************************/
extern "C" int __cdecl is_wctype (
        wint_t c,
        wctype_t mask
        )
{
    return iswctype(c, mask);
}
