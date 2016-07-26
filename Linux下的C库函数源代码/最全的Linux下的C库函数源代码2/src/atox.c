/***
*atox.c - atoi and atol conversion
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Converts a character string into an int or long.
*
*******************************************************************************/

#include <cruntime.h>
#include <mtdll.h>
#include <stdlib.h>
#include <ctype.h>
#include <mbctype.h>
#include <tchar.h>
#include <setlocal.h>
#ifdef _MBCS
#undef _MBCS
#endif  /* _MBCS */
#include <tchar.h>

/***
*long atol(char *nptr) - Convert string to long
*
*Purpose:
*       Converts ASCII string pointed to by nptr to binary.
*       It behaves exactly like strtol(nptr, NULL, 10).
*       Please refer to the strtol implementation for details.
*
*Entry:
*       nptr = ptr to string to convert
*
*       string format: [whitespace] [sign] [digits]
*
*Exit:
*       Good return:
*               result
*
*       Overflow return:
*               LONG_MAX or LONG_MIN
*               errno == ERANGE
*
*       No digits or other error condition return:
*               0
*
*Exceptions:
*       Input parameters are validated. Refer to the strtox function family.
*
*******************************************************************************/

long __cdecl _tstol(
        const _TCHAR *nptr
        )
{
    return _tcstol(nptr, NULL, 10);
}


long __cdecl _tstol_l(
        const _TCHAR *nptr,
        _locale_t plocinfo
        )
{
    return _tcstol_l(nptr, NULL, 10, plocinfo);
}


/***
*int atoi(char *nptr) - Convert string to int
*
*Purpose:
*       Converts ASCII string pointed to by nptr to binary.
*       It behaves exactly like (int)strtol(nptr, NULL, 10).
*       Please refer to the strtol implementation for details.
*
*Entry:
*       nptr = ptr to string to convert
*
*Exit:
*       Good return:
*               result
*
*       Overflow return:
*               INT_MAX or INT_MIN
*               errno == ERANGE
*
*       No digits or other error condition return:
*               0
*
*Exceptions:
*       Input parameters are validated. Refer to the strtox function family.
*
*******************************************************************************/

int __cdecl _tstoi(
        const _TCHAR *nptr
        )
{
    return (int)_tstol(nptr);
}

int __cdecl _tstoi_l(
        const _TCHAR *nptr,
        _locale_t plocinfo
        )
{
    return (int)_tstol_l(nptr, plocinfo);
}

#ifndef _NO_INT64

/***
*int atoi64(char *nptr) - Convert string to int64
*
*Purpose:
*       Converts ASCII string pointed to by nptr to binary.
*       It behaves exactly like (int)strtoi64(nptr, NULL, 10).
*       Please refer to the strtoi64 implementation for details.
*
*Entry:
*       nptr = ptr to string to convert
*
*Exit:
*       Good return:
*               result
*
*       Overflow return:
*               _I64_MAX or _I64_MIN
*               errno == ERANGE
*
*       No digits or other error condition return:
*               0
*
*Exceptions:
*       Input parameters are validated. Refer to the strtox function family.
*
*******************************************************************************/

__int64 __cdecl _tstoi64(
        const _TCHAR *nptr
        )
{
    return _tcstoi64(nptr, NULL, 10);
}

__int64 __cdecl _tstoi64_l(
        const _TCHAR *nptr,
        _locale_t plocinfo
        )
{
    return _tcstoi64_l(nptr, NULL, 10, plocinfo);
}

#endif  /* _NO_INT64 */
