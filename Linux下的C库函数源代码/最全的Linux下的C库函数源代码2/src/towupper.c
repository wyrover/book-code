/***
*towupper.c - convert wide character to upper case
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines towupper().
*
*******************************************************************************/


#include <cruntime.h>
#include <stdio.h>
#include <locale.h>
#include <awint.h>
#include <ctype.h>
#include <mtdll.h>
#include <setlocal.h>

/***
*wint_t _towupper_l(c, ptloci) - convert wide character to upper case
*
*Purpose:
*       Multi-thread function only! Non-locking version of towupper.
*
*Entry:
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

extern "C" wint_t __cdecl _towupper_l (
        wint_t c,
        _locale_t plocinfo
        )
{
    wint_t widechar;

    if (c == WEOF)
        return c;

    _LocaleUpdate _loc_update(plocinfo);

    if ( _loc_update.GetLocaleT()->locinfo->lc_handle[LC_CTYPE] == _CLOCALEHANDLE )
        return __ascii_towupper(c);

    /* if checking case of c does not require API call, do it */
    if ( c < 256 ) {
        if ( !_iswlower_l(c, _loc_update.GetLocaleT()) ) {
            return c;
        } else {
            return _loc_update.GetLocaleT()->locinfo->pcumap[c];
        }
    }

    /* convert wide char to uppercase */
    if ( 0 == __crtLCMapStringW(
                _loc_update.GetLocaleT(),
                _loc_update.GetLocaleT()->locinfo->lc_handle[LC_CTYPE],
                LCMAP_UPPERCASE,
                (LPCWSTR)&c,
                1,
                (LPWSTR)&widechar,
                1,
                _loc_update.GetLocaleT()->locinfo->lc_codepage ) )
    {
        return c;
    }

    return widechar;

}

/***
*wint_t towupper(c) - convert wide character to upper case
*
*Purpose:
*       towupper() returns the uppercase equivalent of its argument
*
*Entry:
*       c - wint_t value of character to be converted
*
*Exit:
*       if c is a lower case letter, returns wint_t value of upper case
*       representation of c. otherwise, it returns c.
*
*Exceptions:
*
*******************************************************************************/

extern "C" wint_t __cdecl towupper (
        wint_t c
        )
{

    return _towupper_l(c, NULL);
}
