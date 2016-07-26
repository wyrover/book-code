/***
*fcvt.c - convert floating point value to string
*
*   Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*   Converts a floating point value to a string.
*
*******************************************************************************/

#include <cruntime.h>
#include <fltintrn.h>
#include <cvt.h>
#include <mtdll.h>
#include <stdlib.h>
#include <dbgint.h>
#include <internal.h>
#include <internal_securecrt.h>
#include <setlocal.h>

/*
 * The static character array buf[_CVTBUFSIZE] is used by the _fpcvt routine
 * (the workhorse for _ecvt and _fcvt) for storage of its output.  The routine
 * gcvt expects the user to have set up their own storage.  _CVTBUFSIZE is set
 * large enough to accomodate the largest double precision number plus 40
 * decimal places (even though you only have 16 digits of accuracy in a
 * double precision IEEE number, the user may ask for more to effect 0
 * padding; but there has to be a limit somewhere).
 */

/*
 * define a maximum size for the conversion buffer.  It should be at least
 * as long as the number of digits in the largest double precision value
 * (?.?e308 in IEEE arithmetic).  We will use the same size buffer as is
 * used in the printf support routine (_output)
 */

static errno_t __cdecl _fpcvt(char*, size_t, STRFLT, int, int *, int *);


/***
*errocde _fcvt_s(result, sizeInChars, value, ndec, decpr, sign) - convert floating point to char string
*
*Purpose:
*   _fcvt like _ecvt converts the value to a null terminated
*   string of ASCII digits, and returns a pointer to the
*   result.  The routine prepares data for Fortran F-format
*   output with the number of digits following the decimal
*   point specified by ndec.  The position of the decimal
*   point relative to the beginning of the string is returned
*   indirectly through decpt.  The correct digit for Fortran
*   F-format is rounded.
*   NOTE - to avoid the possibility of generating floating
*   point instructions in this code we fool the compiler
*   about the type of the 'value' parameter using a struct.
*   This is OK since all we do is pass it off as a
*   parameter.
*
*Entry:
*   char * result - pointer to the string that will receive the output
*   size_t sizeInChars - size of the output string
*   double value - number to be converted
*   int ndec - number of digits after decimal point
*
*Exit:
*   returns errno_t != 0 if something went wrong (check the validation section below).
*   char * result - the output is written into the given result string
*   int *decpt - pointer to int with pos. of dec. point
*   int *sign - pointer to int with sign (0 = pos, non-0 = neg)
*
*Exceptions:
*   Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

errno_t __cdecl _fcvt_s (
    char *result,
    size_t sizeInChars,
    double value,
    int ndec,
    int *decpt,
    int *sign
    )
{

    REG1 STRFLT pflt;
    _CRT_DOUBLE *pdvalue = (_CRT_DOUBLE *)&value;
    REG4 int digits = 0;

    struct _strflt strfltstruct;
    char resultstring[22 /* MAX_MAN_DIGITS+1 */];

    /* validation section */
    _VALIDATE_RETURN_ERRCODE(result != NULL, EINVAL);
    _VALIDATE_RETURN_ERRCODE(sizeInChars > 0, EINVAL);
    _RESET_STRING(result, sizeInChars);
    _VALIDATE_RETURN_ERRCODE(decpt != NULL, EINVAL);
    _VALIDATE_RETURN_ERRCODE(sign != NULL, EINVAL);

    /* ok to take address of stack struct here; fltout2 knows to use ss */
    pflt = _fltout2( *pdvalue, &strfltstruct, resultstring, _countof(resultstring) );

    digits = pflt->decpt + ndec;

    /* if we detect overflow then set number of digits to the largest possible */
    if (ndec > 0 && pflt->decpt > 0 && digits < ndec)
    {
        digits = INT_MAX;
    }

    return( _fpcvt( result, sizeInChars, pflt, digits, decpt, sign ) );
}

/***
*char *_fcvt(value, ndec, decpr, sign) - convert floating point to char string
*
*Purpose:
*   _fcvt like _ecvt converts the value to a null terminated
*   string of ASCII digits, and returns a pointer to the
*   result.  The routine prepares data for Fortran F-format
*   output with the number of digits following the decimal
*   point specified by ndec.  The position of the decimal
*   point relative to the beginning of the string is returned
*   indirectly through decpt.  The correct digit for Fortran
*   F-format is rounded.
*   NOTE - to avoid the possibility of generating floating
*   point instructions in this code we fool the compiler
*   about the type of the 'value' parameter using a struct.
*   This is OK since all we do is pass it off as a
*   parameter.
*
*Entry:
*   double value - number to be converted
*   int ndec - number of digits after decimal point
*
*Exit:
*   returns pointer to the character string representation of value.
*   also, the output is written into the static char array buf.
*   int *decpt - pointer to int with pos. of dec. point
*   int *sign - pointer to int with sign (0 = pos, non-0 = neg)
*
*Exceptions:
*   Input parameters are validated like in _fcvt_s. Returns NULL if something goes wrong.
*   The function is deprecated, unless the user defines _DO_NOT_DEPRECATE_UNSAFE_FUNCTIONS.
*
*******************************************************************************/

char * __cdecl _fcvt (
    double value,
    int ndec,
    int *decpt,
    int *sign
    )
{
    errno_t e = 0;
    REG1 STRFLT pflt;
    _CRT_DOUBLE *pdvalue = (_CRT_DOUBLE *)&value;


    /* use a per-thread buffer */
    char *buf;
    _ptiddata ptd;
    struct _strflt strfltstruct;
    char resultstring[22 /* MAX_MAN_DIGITS+1 */];

    ptd = _getptd_noexit();
    if (!ptd)
        return NULL;
    if ( ptd->_cvtbuf == NULL )
        if ( (ptd->_cvtbuf = (char *)_malloc_crt(_CVTBUFSIZE)) == NULL )
            return(NULL);
    buf = ptd->_cvtbuf;

    /* ok to take address of stack struct here; fltout2 knows to use ss */
    pflt = _fltout2( *pdvalue, &strfltstruct, resultstring, _countof(resultstring) );

    /* make sure we don't overflow the buffer size.  If the user asks for
     * more digits than the buffer can handle, truncate it to the maximum
     * size allowed in the buffer.
     */
    ndec = min(ndec, _CVTBUFSIZE - 2 - pflt->decpt);

    e = _fcvt_s( buf, _CVTBUFSIZE, value, ndec, decpt, sign );
    if ( e != 0 )
    {
        return NULL;
    }
    return buf;
}


/***
*char *_ecvt_s(result, sizeInChars, value, ndigit, decpt, sign) - convert floating point to string
*
*Purpose:
*   _ecvt converts value to a null terminated string of
*   ASCII digits, and returns a pointer to the result.
*   The position of the decimal point relative to the
*   begining of the string is stored indirectly through
*   decpt, where negative means to the left of the returned
*   digits.  If the sign of the result is negative, the
*   word pointed to by sign is non zero, otherwise it is
*   zero.  The low order digit is rounded.
*
*Entry:
*   char * result - pointer to the string that will receive the output
*   size_t sizeInChars - size of the output string
*   double value - number to be converted
*   int ndec - number of digits after decimal point
*
*Exit:
*   returns errno_t != 0 if something went wrong (check the validation section below).
*   char * result - the output is written into the given result string
*   int *decpt - pointer to int with pos. of dec. point
*   int *sign - pointer to int with sign (0 = pos, non-0 = neg)
*
*Exceptions:
*   Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

errno_t __cdecl _ecvt_s (
    char *result,
    size_t sizeInChars,
    double value,
    int ndigit,
    int *decpt,
    int *sign
    )
{
    _CRT_DOUBLE *pdvalue = (_CRT_DOUBLE *)&value;
    REG1 STRFLT pflt;

    struct _strflt strfltstruct;        /* temporary buffers */
    char resultstring[22 /* MAX_MAN_DIGITS+1 */];

    errno_t e;

    /* validation section */
    _VALIDATE_RETURN_ERRCODE(result != NULL, EINVAL);
    _VALIDATE_RETURN_ERRCODE(sizeInChars > 0, EINVAL);
    _RESET_STRING(result, sizeInChars);
    _VALIDATE_RETURN_ERRCODE(decpt != NULL, EINVAL);
    _VALIDATE_RETURN_ERRCODE(sign != NULL, EINVAL);

    /* ok to take address of stack struct here; fltout2 knows to use ss */
    pflt = _fltout2( *pdvalue, &strfltstruct, resultstring, _countof(resultstring) );

    e = _fpcvt( result, sizeInChars, pflt, ndigit, decpt, sign );

    /* make sure we don't overflow the buffer size.  If the user asks for
     * more digits than the buffer can handle, truncate it to the maximum
     * size allowed in the buffer.  The maximum size is sizeInChars - 2
     * since we use one character for overflow and one for the terminating
     * null character.
     */
    if (ndigit > (int)(sizeInChars - 2))
    {
        ndigit = (int)(sizeInChars - 2);
    }

    /* _fptostr() occasionally returns an extra character in the buffer ... */
    if (ndigit >= 0 && result[ndigit])
        result[ndigit] = '\0';

    return e;
}

/***
*char *_ecvt(value, ndigit, decpt, sign) - convert floating point to string
*
*Purpose:
*   _ecvt converts value to a null terminated string of
*   ASCII digits, and returns a pointer to the result.
*   The position of the decimal point relative to the
*   begining of the string is stored indirectly through
*   decpt, where negative means to the left of the returned
*   digits.  If the sign of the result is negative, the
*   word pointed to by sign is non zero, otherwise it is
*   zero.  The low order digit is rounded.
*
*Entry:
*   double value - number to be converted
*   int ndigit - number of digits after decimal point
*
*Exit:
*   returns pointer to the character representation of value.
*   also the output is written into the statuc char array buf.
*   int *decpt - pointer to int with position of decimal point
*   int *sign - pointer to int with sign in it (0 = pos, non-0 = neg)
*
*Exceptions:
*   Input parameters are validated like in _ecvt_s. Returns NULL if something goes wrong.
*   The function is deprecated, unless the user defines _DO_NOT_DEPRECATE_UNSAFE_FUNCTIONS.
*
*******************************************************************************/

char * __cdecl _ecvt (
    double value,
    int ndigit,
    int *decpt,
    int *sign
    )
{
    errno_t e = 0;

    /* use a per-thread buffer */

    char *buf;

    _ptiddata ptd;

    ptd = _getptd_noexit();
    if (!ptd)
        return NULL;
    if ( ptd->_cvtbuf == NULL )
        if ( (ptd->_cvtbuf = (char *)_malloc_crt(_CVTBUFSIZE)) == NULL )
            return(NULL);
    buf = ptd->_cvtbuf;


    /* make sure we don't overflow the buffer size.  If the user asks for
     * more digits than the buffer can handle, truncate it to the maximum
     * size allowed in the buffer.  The maximum size is _CVTBUFSIZE - 2
     * since we use one character for overflow and one for the terminating
     * null character.
     */
    ndigit = min(ndigit, _CVTBUFSIZE - 2);

    e = _ecvt_s( buf, _CVTBUFSIZE, value, ndigit, decpt, sign );
    if ( e != 0 )
    {
        return NULL;
    }
    return buf;
}


/***
*char *_fpcvt() - gets final string and sets decpt and sign [STATIC]
*
*Purpose:
*   This is a small common routine used by [ef]cvt[_s].  It calls fptostr
*   to get the final string and sets the decpt and sign indicators.
*
*Entry:
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

static errno_t __cdecl _fpcvt (
    char *result,
    size_t sizeInChars,
    REG2 STRFLT pflt,
    REG3 int digits,
    int *decpt,
    int *sign
    )
{
    errno_t e = 0;

    /* make sure we don't overflow the buffer size.  If the user asks for
     * more digits than the buffer can handle, truncate it to the maximum
     * size allowed in the buffer.  The maximum size is sizeInChars - 2
     * since we use one character for overflow and one for the terminating
     * null character.
     */
    _VALIDATE_RETURN_ERRCODE(sizeInChars >= (size_t)((digits > 0 ? digits : 0) + 2), ERANGE);

    e = _fptostr(
        result,
        sizeInChars,
        (digits > (int)(sizeInChars - 2)) ? (int)(sizeInChars - 2) : digits,
        pflt);
    if (e != 0)
    {
        errno = e;
        return e;
    }

    /* set the sign flag and decimal point position */
    *sign = (pflt->sign == '-') ? 1 : 0;
    *decpt = pflt->decpt;

    return 0;
}

/* back-compat functions for non-locale users */
FLT __cdecl _fltinf(const char *str, int len, int scale, int decpt)
{
    _LocaleUpdate _loc_update(NULL);

        return _fltinf_l(str, len, scale, decpt, _loc_update.GetLocaleT());
}
