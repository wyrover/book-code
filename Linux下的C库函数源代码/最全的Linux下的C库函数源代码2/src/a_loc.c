/***
*a_loc.c - A versions of GetLocaleInfo.
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Use either GetLocaleInfoA or GetLocaleInfoW depending on which is
*       available
*
*******************************************************************************/

#include <cruntime.h>
#include <internal.h>
#include <stdlib.h>
#include <awint.h>
#include <dbgint.h>
#include <malloc.h>
#include <locale.h>
#include <mtdll.h>
#include <setlocal.h>

#define USE_W   1
#define USE_A   2

/***
*int __cdecl __crtGetLocaleInfoA - Get locale info and return it as an ASCII
*       string
*
*Purpose:
*       Internal support function. Assumes info in ANSI string format. Tries
*       to use NLS API call GetLocaleInfoA if available (Chicago) and uses
*       GetLocaleInfoA if it must (NT). If neither are available it fails and
*       returns 0.
*
*Entry:
*       LCID     Locale      - locale context for the comparison.
*       LCTYPE   LCType      - see NT\Chicago docs
*       LPSTR    lpLCData    - pointer to memory to return data
*       int      cchData     - char (byte) count of buffer (including NULL)
*                              (if 0, lpLCData is not referenced, size needed
*                              is returned)
*       int      code_page   - for MB/WC conversion. If 0, use __lc_codepage
*
*Exit:
*       Success: the number of characters copied (including NULL).
*       Failure: 0
*
*Exceptions:
*
*******************************************************************************/

static int __cdecl __crtGetLocaleInfoA_stat(
        _locale_t plocinfo,
        LCID    Locale,
        LCTYPE  LCType,
        LPSTR   lpLCData,
        int     cchData,
        int     code_page
        )
{
    static int f_use = 0;

    /*
     * Look for unstubbed 'preferred' flavor. Otherwise use available flavor.
     * Must actually call the function to ensure it's not a stub.
     */

    if (0 == f_use)
    {
        if (0 != GetLocaleInfoW(0, LOCALE_ILANGUAGE, NULL, 0))
            f_use = USE_W;

        else if (GetLastError() == ERROR_CALL_NOT_IMPLEMENTED)
            f_use = USE_A;
    }

    /* Use "A" version */

    if (USE_A == f_use || f_use == 0)
    {
        return GetLocaleInfoA(Locale, LCType, lpLCData, cchData);
    }

    /* Use "W" version */

    if (USE_W == f_use)
    {
        int retval = 0;
        int buff_size;
        wchar_t *wbuffer;

        /*
         * Use __lc_codepage for conversion if code_page not specified
         */

        if (0 == code_page)
            code_page = plocinfo->locinfo->lc_codepage;

        /* find out how big buffer needs to be */
        if (0 == (buff_size = GetLocaleInfoW(Locale, LCType, NULL, 0)))
            return 0;

        /* allocate buffer */
        wbuffer = (wchar_t *)_calloca( buff_size, sizeof(wchar_t) );
        if ( wbuffer == NULL ) {
            return 0;
        }

        /* get the info in wide format */
        if (0 == GetLocaleInfoW(Locale, LCType, wbuffer, buff_size))
            goto error_cleanup;

        /* convert from Wide Char to ANSI */
        if (0 == cchData)
        {
            /* convert into local buffer */
            retval = WideCharToMultiByte( code_page,
                                          0,
                                          wbuffer,
                                          -1,
                                          NULL,
                                          0,
                                          NULL,
                                          NULL );
        }
        else {
            /* convert into user buffer */
            retval = WideCharToMultiByte( code_page,
                                          0,
                                          wbuffer,
                                          -1,
                                          lpLCData,
                                          cchData,
                                          NULL,
                                          NULL );
        }

error_cleanup:
        _freea(wbuffer);

        return retval;
    }
    else   /* f_use is neither USE_A nor USE_W */
        return 0;
}

extern "C" int __cdecl __crtGetLocaleInfoA(
        _locale_t plocinfo,
        LCID    Locale,
        LCTYPE  LCType,
        LPSTR   lpLCData,
        int     cchData,
        int     code_page
        )
{
    _LocaleUpdate _loc_update(plocinfo);

    return __crtGetLocaleInfoA_stat(
            _loc_update.GetLocaleT(),
            Locale,
            LCType,
            lpLCData,
            cchData,
            code_page
            );
}
