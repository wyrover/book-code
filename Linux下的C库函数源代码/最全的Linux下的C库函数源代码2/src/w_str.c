/***
*w_str.c - W version of GetStringType.
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Use either GetStringTypeA or GetStringTypeW depending on which is
*       unstubbed.
*
*******************************************************************************/

#include <cruntime.h>
#include <internal.h>
#include <stdlib.h>
#include <locale.h>
#include <awint.h>
#include <dbgint.h>
#include <malloc.h>
#include <mtdll.h>
#include <setlocal.h>
#include <sal.h>

#define USE_W   1
#define USE_A   2

/***
*int __cdecl __crtGetStringTypeW - Get type information about a wide string.
*
*Purpose:
*       Internal support function. Assumes info in wide string format. Tries
*       to use NLS API call GetStringTypeW if available and uses GetStringTypeA
*       if it must. If neither are available it fails and returns FALSE.
*
*Entry:
*       DWORD    dwInfoType  - see NT\Chicago docs
*       LPCWSTR  lpSrcStr    - wide string for which character types are
*                              requested
*       int      cchSrc      - wide char (word) count of lpSrcStr (including
*                              NULL if any)
*       LPWORD   lpCharType  - array to receive character type information
*                              (must be same size as lpSrcStr)
*       int      code_page   - for MB/WC conversion. If 0, use __lc_codepage
*       int      lcid        - for A call, specify LCID, If 0, use
*                              __lc_handle[LC_CTYPE].
*
*Exit:
*       Success: TRUE
*       Failure: FALSE
*
*Exceptions:
*
*******************************************************************************/

static BOOL __cdecl __crtGetStringTypeW_stat
(
    __in_opt _locale_t plocinfo,
    __in DWORD dwInfoType,
    __in_ecount(_CchSrc) LPCWSTR lpSrcStr,
    __in int cchSrc,
    __out LPWORD lpCharType,
    __in int code_page,
    __in int lcid
)
{
    static int f_use = 0;

    /*
     * Look for unstubbed 'preferred' flavor. Otherwise use available flavor.
     * Must actually call the function to ensure it's not a stub.
     */

    if ( cchSrc < -1 )
        return FALSE;

    if (0 == f_use)
    {
        unsigned short dummy;

        if (0 != GetStringTypeW(CT_CTYPE1, L"\0", 1, &dummy))
            f_use = USE_W;

        else if (GetLastError() == ERROR_CALL_NOT_IMPLEMENTED)
            f_use = USE_A;
    }

    /* Use "W" version */

    if (USE_W == f_use)
    {
        return GetStringTypeW(dwInfoType, lpSrcStr, cchSrc, lpCharType);
    }

    /* Use "A" version */

    if (USE_A == f_use || f_use == 0)
    {
        int buff_size;
        BOOL retbool=FALSE;
        unsigned char *buffer;
        WORD * pwCharInfo;
        int malloc_flag1 = 0;
        int malloc_flag2 = 0;
        int AnsiCP;

        /*
         * Convert string and return the requested information. Note that
         * we are converting to a multibyte string so there is not a
         * one-to-one correspondence between number of wide chars in the
         * input string and the number of *bytes* in the buffer. However,
         * there had *better be* a one-to-one correspondence between the
         * number of wide characters and the number of WORDs in the
         * return buffer.
         */

        /*
         * Use __lc_codepage for conversion if code_page not specified
         */

        if (0 == lcid)
            lcid = plocinfo->locinfo->lc_handle[LC_CTYPE];
        if (0 == code_page)
            code_page = plocinfo->locinfo->lc_codepage;

        /*
         * Always use Ansi codepage with Ansi WinAPI because they use
         * Ansi codepage
         */
        if ( code_page != (AnsiCP = __ansicp(lcid)))
        {
            if (AnsiCP != -1)
                code_page = AnsiCP;
        }

        /* find out how big a buffer we need */
        if ( 0 == (buff_size = WideCharToMultiByte( code_page,
                                                    0,
                                                    lpSrcStr,
                                                    cchSrc,
                                                    NULL,
                                                    0,
                                                    NULL,
                                                    NULL )) )
            return FALSE;

        /* allocate enough space for chars */
        buffer = (unsigned char *)_calloca( sizeof(char), buff_size );
        if ( buffer == NULL ) {
            return FALSE;
        }
        (void)memset( buffer, 0, sizeof(char) * buff_size );

        /* do the conversion */
        if ( 0 == WideCharToMultiByte( code_page,
                                       0,
                                       lpSrcStr,
                                       cchSrc,
                                       (char *)buffer,
                                       buff_size,
                                       NULL,
                                       NULL ) )
            goto error_cleanup;

        /* allocate enough space for result (+1 for sanity check) */
        pwCharInfo = (WORD *)_calloca( sizeof(WORD), (buff_size + 1) );
        if ( pwCharInfo == NULL ) {
            goto error_cleanup;
        }

        /* do we use default lcid */
        if (0 == lcid)
            lcid = plocinfo->locinfo->lc_handle[LC_CTYPE];

        /* set to known value */
        pwCharInfo[cchSrc - 1] = pwCharInfo[cchSrc] = 0xFFFF;

        /* obtain result */
        retbool = GetStringTypeA( lcid, dwInfoType, (const char *)buffer, buff_size,
                                  pwCharInfo );

        /*
         * GetStringTypeA does not reveal how many WORDs have been
         * modifed - to be safe we use another buffer and then
         * verify that EXACTLY cchSrc WORDs were modified. Note that
         * not all multibyte LCID/codepage combos are guaranteed to work.
         */
        if ( (pwCharInfo[cchSrc - 1] == 0xFFFF) || (pwCharInfo[cchSrc] != 0xFFFF) )
            retbool = FALSE;
        else
            memmove(lpCharType, pwCharInfo, cchSrc * sizeof(WORD));

        _freea(pwCharInfo);

error_cleanup:
        _freea(buffer);

        return retbool;
    }
    else   /* f_use is neither USE_A nor USE_W */
        return FALSE;
}

extern "C" BOOL __cdecl __crtGetStringTypeW(
        _locale_t plocinfo,
        DWORD    dwInfoType,
        LPCWSTR  lpSrcStr,
        int      cchSrc,
        LPWORD   lpCharType,
        int      code_page,
        int      lcid
        )
{
    _LocaleUpdate _loc_update(plocinfo);

    return __crtGetStringTypeW_stat(
            _loc_update.GetLocaleT(),
            dwInfoType,
            lpSrcStr,
            cchSrc,
            lpCharType,
            code_page,
            lcid
            );
}
