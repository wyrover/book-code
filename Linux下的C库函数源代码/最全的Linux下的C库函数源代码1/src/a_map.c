/***
*a_map.c - A version of LCMapString.
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Use either LCMapStringA or LCMapStringW depending on which is available
*
*******************************************************************************/

#include <cruntime.h>
#include <internal.h>
#include <stdlib.h>
#include <locale.h>
#include <awint.h>
#include <dbgint.h>
#include <malloc.h>
#include <locale.h>
#include <mtdll.h>
#include <setlocal.h>

#define USE_W   1
#define USE_A   2

/***
*int __cdecl strncnt - count characters in a string, up to n.
*
*Purpose:
*       Internal local support function. Counts characters in string before
*       null. If null is not found in n chars, then return n.
*
*Entry:
*       const char *string   - start of string
*       int n                - byte count
*
*Exit:
*       returns number of bytes from start of string to
*       null (exclusive), up to n.
*
*Exceptions:
*
*******************************************************************************/

static int __cdecl strncnt (
        const char *string,
        int cnt
        )
{
        int n = cnt;
        char *cp = (char *)string;

        while (n-- && *cp)
            cp++;

        return cnt - n - 1;
}


/***
*int __cdecl __crtLCMapStringA - Get type information about an ANSI string.
*
*Purpose:
*       Internal support function. Assumes info in ANSI string format. Tries
*       to use NLS API call LCMapStringA if available and uses LCMapStringW
*       if it must. If neither are available it fails and returns 0.
*
*Entry:
*       LCID     Locale      - locale context for the comparison.
*       DWORD    dwMapFlags  - see NT\Chicago docs
*       LPCSTR   lpSrcStr    - pointer to string to be mapped
*       int      cchSrc      - wide char (word) count of input string
*                              (including NULL if any)
*                              (-1 if NULL terminated)
*       LPSTR    lpDestStr   - pointer to memory to store mapping
*       int      cchDest     - char (byte) count of buffer (including NULL)
*       int      code_page   - for MB/WC conversion. If 0, use __lc_codepage
*       BOOL     bError      - TRUE if MB_ERR_INVALID_CHARS set on call to
*                              MultiByteToWideChar when GetStringTypeW used.
*
*Exit:
*       Success: number of chars written to lpDestStr (including NULL)
*       Failure: 0
*
*Exceptions:
*
*******************************************************************************/

static int __cdecl __crtLCMapStringA_stat(
        _locale_t plocinfo,
        LCID     Locale,
        DWORD    dwMapFlags,
        LPCSTR   lpSrcStr,
        int      cchSrc,
        LPSTR    lpDestStr,
        int      cchDest,
        int      code_page,
        BOOL     bError
        )
{
    static int f_use = 0;

    /*
     * Look for unstubbed 'preferred' flavor. Otherwise use available flavor.
     * Must actually call the function to ensure it's not a stub.
     * (Always try wide version first so WinNT can process codepage correctly.)
     */

    if (0 == f_use) {
        if (0 != LCMapStringW(0, LCMAP_LOWERCASE, L"\0", 1, NULL, 0))
            f_use = USE_W;
        else if (GetLastError() == ERROR_CALL_NOT_IMPLEMENTED)
            f_use = USE_A;
    }

    /*
     * LCMapString will map past NULL. Must find NULL if in string
     * before cchSrc characters.
     */
    if (cchSrc > 0) {
        int cchSrcCnt = strncnt(lpSrcStr, cchSrc);
        /*
         * Include NULL in cchSrc if lpSrcStr terminated within cchSrc bytes.
         */
        if (cchSrcCnt < cchSrc) {
            cchSrc = cchSrcCnt + 1;
        } else {
            cchSrc = cchSrcCnt;
        }
    }

    /* Use "A" version */

    if (USE_A == f_use || f_use == 0) {
        char *cbuffer = NULL;
        char *cbuffer1 = NULL;
        char *cbuffer2 = NULL;
        int ret;
        int AnsiCP;
        int buff_size;

        if (0 == Locale)
            Locale = plocinfo->locinfo->lc_handle[LC_CTYPE];
        if (0 == code_page)
            code_page = plocinfo->locinfo->lc_codepage;

        if ( -1 == (AnsiCP = __ansicp(Locale)))
            return FALSE;
        /* LCMapStringA uses ANSI code page to map the string. Check if
         * codepage is ansi, if not convert the input string to ansi
         * codepage then map to a temporary string and then convert temp
         * string to DestStr.
         */
        if ( AnsiCP != code_page)
        {
            cbuffer = __convertcp(code_page, AnsiCP, lpSrcStr, &cchSrc, NULL, 0);
            if (cbuffer == NULL)
                return FALSE;
            lpSrcStr = cbuffer;
            if (0 == (buff_size = LCMapStringA( Locale,
                                                dwMapFlags,
                                                lpSrcStr,
                                                cchSrc,
                                                0,
                                                0 )))
            {
                ret = FALSE;
                goto cleanupA;
            }
            cbuffer1 = (char *)_calloca(sizeof(char), buff_size);
            if ( cbuffer1 == NULL ) {
                ret = FALSE;
                goto cleanupA;
            }
            memset(cbuffer1, 0, sizeof(char) * buff_size);

            if (0 == (buff_size = LCMapStringA( Locale,
                                                dwMapFlags,
                                                lpSrcStr,
                                                cchSrc,
                                                cbuffer1,
                                                buff_size)))
            {
                ret = FALSE;
            } else {
                if ( NULL == (cbuffer2 = __convertcp(AnsiCP,
                                         code_page,
                                         cbuffer1,
                                         &buff_size,
                                         lpDestStr,
                                         cchDest)))
                    ret = FALSE;
                else
                    ret = buff_size;

            }
cleanupA:
            if ( cbuffer1 != NULL )
                _freea(cbuffer1);
        } else
        {
            ret = LCMapStringA( Locale, dwMapFlags, lpSrcStr, cchSrc,
                                lpDestStr, cchDest );
        }
        if ( cbuffer != NULL)
            _free_crt(cbuffer);
        if ( (cbuffer2 != NULL) && (lpDestStr != cbuffer2))
             _free_crt(cbuffer2);
        return ret;
    }

    /* Use "W" version */

    if (USE_W == f_use)
    {
        int retval = 0;
        int inbuff_size;
        int outbuff_size;
        wchar_t *inwbuffer = NULL;
        wchar_t *outwbuffer = NULL;

        /*
         * Convert string and return the requested information. Note that
         * we are converting to a wide string so there is not a
         * one-to-one correspondence between number of wide chars in the
         * input string and the number of *bytes* in the buffer. However,
         * there had *better be* a one-to-one correspondence between the
         * number of wide characters and the number of multibyte characters
         * or the resulting mapped string will be worthless to the user.
         */

        /*
         * Use __lc_codepage for conversion if code_page not specified
         */

        if (0 == code_page)
            code_page = plocinfo->locinfo->lc_codepage;

        /* find out how big a buffer we need (includes NULL if any) */
        if ( 0 == (inbuff_size =
                   MultiByteToWideChar( code_page,
                                        bError ? MB_PRECOMPOSED |
                                            MB_ERR_INVALID_CHARS :
                                            MB_PRECOMPOSED,
                                        lpSrcStr,
                                        cchSrc,
                                        NULL,
                                        0 )) )
            return 0;

        /* allocate enough space for wide chars */
        inwbuffer = (wchar_t *)_calloca( inbuff_size, sizeof(wchar_t) );
        if ( inwbuffer == NULL ) {
            return 0;
        }

        /* do the conversion */
        if ( 0 == MultiByteToWideChar( code_page,
                                       MB_PRECOMPOSED,
                                       lpSrcStr,
                                       cchSrc,
                                       inwbuffer,
                                       inbuff_size) )
            goto error_cleanup;

        /* get size required for string mapping */
        if ( 0 == (retval = LCMapStringW( Locale,
                                          dwMapFlags,
                                          inwbuffer,
                                          inbuff_size,
                                          NULL,
                                          0 )) )
            goto error_cleanup;

        if (dwMapFlags & LCMAP_SORTKEY) {
            /* retval is size in BYTES */

            if (0 != cchDest) {

                if (retval > cchDest)
                    goto error_cleanup;

                /* do string mapping */
                if ( 0 == LCMapStringW( Locale,
                                        dwMapFlags,
                                        inwbuffer,
                                        inbuff_size,
                                        (LPWSTR)lpDestStr,
                                        cchDest ) )
                    goto error_cleanup;
            }
        }
        else {
            /* retval is size in wide chars */

            outbuff_size = retval;

            /* allocate enough space for wide chars (includes NULL if any) */
            outwbuffer = (wchar_t *)_calloca( outbuff_size, sizeof(wchar_t) );
            if ( outwbuffer == NULL ) {
                goto error_cleanup;
            }

            /* do string mapping */
            if ( 0 == LCMapStringW( Locale,
                                    dwMapFlags,
                                    inwbuffer,
                                    inbuff_size,
                                    outwbuffer,
                                    outbuff_size ) )
                goto error_cleanup;

            if (0 == cchDest) {
                /* get size required */
                if ( 0 == (retval =
                           WideCharToMultiByte( code_page,
                                                0,
                                                outwbuffer,
                                                outbuff_size,
                                                NULL,
                                                0,
                                                NULL,
                                                NULL )) )
                    goto error_cleanup;
            }
            else {
                /* convert mapping */
                if ( 0 == (retval =
                           WideCharToMultiByte( code_page,
                                                0,
                                                outwbuffer,
                                                outbuff_size,
                                                lpDestStr,
                                                cchDest,
                                                NULL,
                                                NULL )) )
                    goto error_cleanup;
            }
        }

error_cleanup:
        if ( outwbuffer != NULL )
            _freea(outwbuffer);

        _freea(inwbuffer);

        return retval;
    }
    else   /* f_use is neither USE_A nor USE_W */
        return 0;
}

extern "C" int __cdecl __crtLCMapStringA(
        _locale_t plocinfo,
        LCID     Locale,
        DWORD    dwMapFlags,
        LPCSTR   lpSrcStr,
        int      cchSrc,
        LPSTR    lpDestStr,
        int      cchDest,
        int      code_page,
        BOOL     bError
        )
{
    _LocaleUpdate _loc_update(plocinfo);

    return __crtLCMapStringA_stat(
            _loc_update.GetLocaleT(),
            Locale,
            dwMapFlags,
            lpSrcStr,
            cchSrc,
            lpDestStr,
            cchDest,
            code_page,
            bError
            );
}
