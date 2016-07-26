/***
*w_env.c - W version of GetEnvironmentStrings.
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Use GetEnvironmentStringsW if available, otherwise use A version.
*
*******************************************************************************/

#include <cruntime.h>
#include <internal.h>
#include <stdlib.h>
#include <setlocal.h>
#include <awint.h>
#include <dbgint.h>

#define USE_W   1
#define USE_A   2

/***
*LPVOID __cdecl __crtGetEnvironmentStringsW - Get wide environment.
*
*Purpose:
*       Internal support function. Tries to use NLS API call
*       GetEnvironmentStringsW if available and uses GetEnvironmentStringsA
*       if it must. If neither are available it fails and returns 0.
*
*Entry:
*       VOID
*
*Exit:
*       LPVOID - pointer to environment block
*
*Exceptions:
*
*******************************************************************************/

LPVOID __cdecl __crtGetEnvironmentStringsW(
        VOID
        )
{
        static int f_use = 0;
        void *penv = NULL;
        char *pch;
        wchar_t *pwch;
        wchar_t *wbuffer;
        int total_size = 0;
        int str_size;

        /*
         * Look for unstubbed 'preferred' flavor. Otherwise use available flavor.
         * Must actually call the function to ensure it's not a stub.
         */

        if ( 0 == f_use )
        {
            if ( NULL != (penv = GetEnvironmentStringsW()) )
                f_use = USE_W;

            else if (GetLastError() == ERROR_CALL_NOT_IMPLEMENTED)
                f_use = USE_A;
        }

        /* Use "W" version */

        if ( USE_W == f_use )
        {
            if ( NULL == penv )
                if ( NULL == (penv = GetEnvironmentStringsW()) )
                    return NULL;

            /* find out how big a buffer is needed */

            pwch = penv;
            while ( *pwch != L'\0' ) {
                if ( *++pwch == L'\0' )
                    pwch++;
            }

            total_size = (int)((char *)pwch - (char *)penv) +
                         (int)sizeof( wchar_t );

            /* allocate the buffer */

            if ( NULL == (wbuffer = _malloc_crt( total_size )) ) {
                FreeEnvironmentStringsW( penv );
                return NULL;
            }

            /* copy environment strings to buffer */

            memcpy( wbuffer, penv, total_size );

            FreeEnvironmentStringsW( penv );

            return (LPVOID)wbuffer;
        }

        /* Use "A" version */

        if (USE_A == f_use || f_use == 0)
        {
            /*
             * Convert strings and return the requested information.
             */
            if ( NULL == penv )
                if ( NULL == (penv = GetEnvironmentStringsA()) )
                    return NULL;

            pch = penv;

            /* find out how big a buffer we need */
            while ( *pch != '\0' )
            {
                if ( 0 == (str_size =
                      MultiByteToWideChar( CP_ACP,
                                           MB_PRECOMPOSED,
                                           pch,
                                           -1,
                                           NULL,
                                           0 )) )
                    return 0;

                total_size += str_size;
                pch += strlen(pch) + 1;
            }

            /* room for final NULL */
            total_size++;

            /* allocate enough space for chars */
            if ( NULL == (wbuffer = (wchar_t *)
                 _calloc_crt( total_size, sizeof( wchar_t ) )) )
            {
                FreeEnvironmentStringsA( penv );
                return NULL;
            }

            /* do the conversion */
            pch = penv;
            pwch = wbuffer;
            while (*pch != '\0')
            {
                if ( 0 == MultiByteToWideChar( CP_ACP,
                                               MB_PRECOMPOSED,
                                               pch,
                                               -1,
                                               pwch,
                                               total_size - (int)(pwch -
                                                 wbuffer) ) )
                {
                    _free_crt( wbuffer );
                    FreeEnvironmentStringsA( penv );
                    return NULL;
                }

                pch += strlen(pch) + 1;
                pwch += wcslen(pwch) + 1;
            }
            *pwch = L'\0';

            FreeEnvironmentStringsA( penv );

            return (LPVOID)wbuffer;

        }
        else   /* f_use is neither USE_A nor USE_W */
            return NULL;
}
