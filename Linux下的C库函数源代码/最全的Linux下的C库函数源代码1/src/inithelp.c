/***
*inithelp.c - Contains the __getlocaleinfo helper routine
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*  Contains the __getlocaleinfo helper routine.
*
*******************************************************************************/

#include <stdlib.h>
#include <cruntime.h>
#include <locale.h>
#include <setlocal.h>
#include <awint.h>
#include <dbgint.h>
#include <ctype.h>
#include <mtdll.h>
#include <internal.h>

/***
*__getlocaleinfo - return locale data
*
*Purpose:
*       Return locale data appropriate for the setlocale init functions.
*       In particular, wide locale strings are converted to char strings
*       or numeric depending on the value of the first parameter.
*
*       Memory is allocated for the char version of the data, and the
*       calling function's pointer is set to it.  This pointer should later
*       be used to free the data.  The wide-char data is fetched using
*       GetLocaleInfo and converted to multibyte using WideCharToMultiByte.
*
*       *** For internal use by the __init_* functions only ***
*
*       *** Future optimization ***
*       When converting a large number of wide-strings to multibyte, do
*       not query the size of the result, but convert them one after
*       another into a large character buffer.  The entire buffer can
*       also be freed with one pointer.
*
*Entry:
*       int lc_type - LC_STR_TYPE for string data, LC_INT_TYPE for numeric data
*       LCID localehandle - LCID based on category and lang or ctry of __lc_id
*       LCTYPE fieldtype - int or string value
*       void *address - cast to either char * or char**
*
*Exit:
*        0  success
*       -1  failure
*
*Exceptions:
*
*******************************************************************************/

#if NO_ERROR == -1
#error Need to use another error return code in __getlocaleinfo
#endif  /* NO_ERROR == -1 */

#define STR_CHAR_CNT    128
#define INT_CHAR_CNT    4

int __cdecl __getlocaleinfo (
        _locale_t plocinfo,
        int lc_type,
        LCID localehandle,
        LCTYPE fieldtype,
        void *address
        )
{
        if (lc_type == LC_STR_TYPE)
        {
            char **straddress = (char **)address;
            unsigned char cbuffer[STR_CHAR_CNT];
            unsigned char *pcbuffer = cbuffer;
            int bufferused = 0; /* 1 indicates buffer points to malloc'ed memory */
            int buffersize = STR_CHAR_CNT;
            int outsize;

            if ((outsize = __crtGetLocaleInfoA(plocinfo, localehandle, fieldtype, pcbuffer, buffersize, 0))
                == 0)
            {
                if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
                    goto error;

                /* buffersize too small, get required size and malloc new buffer */

                if ((buffersize = __crtGetLocaleInfoA (plocinfo, localehandle, fieldtype, NULL, 0, 0))
                    == 0)
                    goto error;

                if ((pcbuffer = (unsigned char *) _calloc_crt (buffersize, sizeof(unsigned char)))
                    == NULL)
                    goto error;

                bufferused = 1;

                if ((outsize = __crtGetLocaleInfoA (plocinfo, localehandle, fieldtype, pcbuffer, buffersize, 0))
                    == 0)
                    goto error;
            }

            if ((*straddress = (char *) _calloc_crt (outsize, sizeof(char))) == NULL)
                goto error;

            _ERRCHECK(strncpy_s(*straddress, outsize, pcbuffer, outsize - 1));

            if (bufferused)
                _free_crt (pcbuffer);

            return 0;

error:
            if (bufferused)
                _free_crt (pcbuffer);
            return -1;

        } else if (lc_type == LC_INT_TYPE)
        {
            int i;
            char c;
            static wchar_t wcbuffer[INT_CHAR_CNT];
            const int buffersize = INT_CHAR_CNT;
            char *charaddress = (char *)address;

            if (__crtGetLocaleInfoW (plocinfo, localehandle, fieldtype, (LPWSTR)&wcbuffer, buffersize, 0) == 0)
                return -1;

            *(char *)charaddress = 0;

            /* assume GetLocaleInfoW returns valid ASCII integer in wcstr format */
            for (i = 0; i < INT_CHAR_CNT; i++)
            {
                if (isdigit(((unsigned char)c = (unsigned char)wcbuffer[i])))
                    *(unsigned char *)charaddress = (unsigned char)(10 * (int)(*charaddress) + (c - '0'));
                else
                    break;
            }
            return 0;
        }
        return -1;
}



_purecall_handler __pPurecall= NULL;

/***
*void _initp_misc_purevirt(void) -
*
*Purpose:
*       Initialize the __pPurecall function pointer
*
*Entry:
*       The per-process encoded value for the null pointer.
*
*Exit:
*       Never returns
*
*Exceptions:
*
*******************************************************************************/

//extern "C"
void __cdecl _initp_misc_purevirt(void* enull)
{
    __pPurecall = (_purecall_handler) enull;
}

