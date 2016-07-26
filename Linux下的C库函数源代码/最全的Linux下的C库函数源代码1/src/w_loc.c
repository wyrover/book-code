/***
*w_loc.c - W version of GetLocaleInfo.
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
*int __cdecl __crtGetLocaleInfoW - Get locale info and return it as a wide
*       string
*
*Purpose:
*       Internal support function. Assumes info in wide string format. Tries
*       to use NLS API call GetLocaleInfoW if available (NT) and uses
*       GetLocaleInfoA if it must (Chicago). If neither are available it fails
*       and returns 0.
*
*Entry:
*       LCID     Locale      - locale context for the comparison.
*       LCTYPE   LCType      - see NT\Chicago docs
*       LPWSTR   lpLCData    - pointer to memory to return data
*       int      cchData     - wide char (word) count of buffer (including
*                              NULL) (if 0, lpLCData is not referenced, size
*                              needed is returned)
*       int      code_page   - for MB/WC conversion. If 0, use __lc_codepage
*
*Exit:
*       Success: the number of characters copied (including NULL).
*       Failure: 0
*
*Exceptions:
*
*******************************************************************************/

static int __cdecl __crtGetLocaleInfoW_stat(
        _locale_t plocinfo,
        LCID    Locale,
        LCTYPE  LCType,
        LPWSTR  lpLCData,
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

    /* Use "W" version */

    if (USE_W == f_use)
    {
        return GetLocaleInfoW(Locale, LCType, lpLCData, cchData);
    }

    /* Use "A" version */

    if (USE_A == f_use || f_use == 0)
    {
        int retval = 0;
        int buff_size;
        unsigned char *buffer;

        /*
         * Use __lc_codepage for conversion if code_page not specified
         */

        if (0 == code_page)
            code_page = plocinfo->locinfo->lc_codepage;

        /* find out how big buffer needs to be */
        if (0 == (buff_size = GetLocaleInfoA(Locale, LCType, NULL, 0)))
            return 0;

        /* allocate buffer */
        buffer = (unsigned char *)_calloca( buff_size, sizeof(char) );
        if ( buffer == NULL ) {
            return 0;
        }

        /* get the info in ANSI format */
        if (0 == GetLocaleInfoA(Locale, LCType, (char *)buffer, buff_size))
            goto error_cleanup;

        if (0 == cchData)
        {
            /* find out how much space needed */
            retval = MultiByteToWideChar( code_page,
                                          MB_PRECOMPOSED,
                                          (const char *)buffer,
                                          -1,
                                          NULL,
                                          0 );
        }
        else {
            /* convert into user buffer */
            retval = MultiByteToWideChar( code_page,
                                          MB_PRECOMPOSED,
                                          (const char *)buffer,
                                          -1,
                                          lpLCData,
                                          cchData );
        }

error_cleanup:
        _freea(buffer);

        return retval;

    }
    else   /* f_use is neither USE_A nor USE_W */
        return 0;
}

extern "C" int __cdecl __crtGetLocaleInfoW(
        _locale_t plocinfo,
        LCID    Locale,
        LCTYPE  LCType,
        LPWSTR  lpLCData,
        int     cchData,
        int     code_page
        )
{
    _LocaleUpdate _loc_update(plocinfo);

    return __crtGetLocaleInfoW_stat(
            _loc_update.GetLocaleT(),
            Locale,
            LCType,
            lpLCData,
            cchData,
            code_page
            );
}
