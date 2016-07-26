/***
*gcvt.c - convert floating point number to character string
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Converts floating point number into character string representation.
*
*******************************************************************************/

#include <cruntime.h>
#include <fltintrn.h>
#include <internal.h>
#include <internal_securecrt.h>
#include <locale.h>
#include <stdlib.h>
#include <mtdll.h>
#include <setlocal.h>

/***
*errno_t _gcvt_s(buffer, sizeInChars, value, ndec) - convert floating point value to char
*       string
*
*Purpose:
*       _gcvt_s converts the value to a null terminated ASCII string
*       buffer.  It attempts to produce ndigit significant digits
*       in Fortran F format if possible, ortherwise E format,
*       ready for printing.  Trailing zeros may be suppressed.
*       NOTE - to avoid the possibility of generating floating
*       point instructions in this code we fool the compiler
*       about the type of the 'value' parameter using a struct.
*       This is OK since all we do is pass it off as a
*       parameter.
*
*Entry:
*       value - double - number to be converted
*       ndec - int - number of significant digits
*       buf - char * - buffer to place result
*       sizeInChars - size_t - destination buffer size
*
*Exit:
*       Return errno_t != 0 if something went wrong (check the validation section below).
*       Result is written into buffer. If the bugger is too small, we return an error
*       (this is part of the validation section, too).
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

extern "C" errno_t __cdecl _gcvt_s (
        char *buf,
        size_t sizeInChars,
        double value,
        int ndec
        )
{
        STRFLT string;
        int    magnitude;
        _CRT_DOUBLE *pdvalue = (_CRT_DOUBLE *)&value;

        REG1 char *str;
        REG2 char *stop;
        errno_t e;
    _locale_t plocinfo = NULL;
    _LocaleUpdate _loc_update(plocinfo);

        struct _strflt strfltstruct;    /* temporary buffers */
        char   resultstring[22 /* MAX_MAN_DIGITS+1 */];


        /* validation section */
        _VALIDATE_RETURN_ERRCODE(buf != NULL, EINVAL);
        _VALIDATE_RETURN_ERRCODE(sizeInChars > 0, EINVAL);
        _RESET_STRING(buf, sizeInChars);
        _VALIDATE_RETURN_ERRCODE((size_t)ndec < sizeInChars, ERANGE);
        /* _cftoe and _cftof (used below) are more strict in validating sizeInChars */

        /* get the magnitude of the number */

        string = _fltout2( *pdvalue, &strfltstruct, resultstring, _countof(resultstring) );

        magnitude = string->decpt - 1;

        /* output the result according to the Fortran G format as outlined in
           Fortran language specification */

        if ( magnitude < -1  ||  magnitude > ndec-1 )
                /* then  Ew.d  d = ndec */
                e = _cftoe( &value, buf, sizeInChars, ndec-1, 0);
        else
                /* Fw.d  where d = ndec-string->decpt */
                e = _cftof( &value, buf, sizeInChars, ndec-string->decpt );

        if (e == 0)
        {
                /* remove the trailing zeroes before the exponent; we don't need to check for sizeInChars */
                str = buf;
                while (*str && *str != *__LCONV(_loc_update.GetLocaleT()->locinfo)->decimal_point)
                        str++;

                if (*str++)
                {
                        while (*str && *str != 'e')
                                str++;

                        stop = str--;

                        while (*str == '0')
                                str--;

                        while (*++str = *stop++)
                                ;
                }
        }
        else
        {
                errno = e;
        }

        return e;
}

/***
*char * _gcvt(value, ndec, buffer) - convert floating point value to char
*       string
*
*Purpose:
*       _gcvt converts the value to a null terminated ASCII string
*       buf.  It attempts to produce ndigit significant digits
*       in Fortran F format if possible, ortherwise E format,
*       ready for printing.  Trailing zeros may be suppressed.
*       No error checking or overflow protection is provided.
*       NOTE - to avoid the possibility of generating floating
*       point instructions in this code we fool the compiler
*       about the type of the 'value' parameter using a struct.
*       This is OK since all we do is pass it off as a
*       parameter.
*
*Entry:
*       value - double - number to be converted
*       ndec - int - number of significant digits
*       buf - char * - buffer to place result
*
*Exit:
*       result is written into buffer; it will be overwritten if it has
*       not been made big enough.
*
*Exceptions:
*       Input parameters are validated like in _gcvt_s. Returns NULL if something goes wrong,
*       and errno is set. The destination buffer size is assumed to be large enough, and
*       no error checking or overflow protection is provided.
*
*******************************************************************************/

char * __cdecl _gcvt (
        double value,
        int ndec,
        char *buf
        )
{
        errno_t e = _gcvt_s(buf, (size_t)-1, value, ndec);
        if (e != 0)
        {
                return NULL;
        }
        return buf;
}
