/***
*strdate.c - contains the function "_strdate()" and "_strdate_s()"
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       contains the function _strdate() and _strdate_s()
*
*******************************************************************************/


#include <cruntime.h>
#include <tchar.h>
#include <time.h>
#include <oscalls.h>
#include <internal.h>
#include <internal_securecrt.h>

/***
*errno_t _strdate_s(buffer, sizeInChars) - return date in string form
*
*Purpose:
*       _strdate_s() returns a string containing the date in "MM/DD/YY" form
*
*Entry:
*       _TSCHAR *buffer = the address of a 9-byte user buffer
*       size_t  sizeInChars = size of the buffer in characters.
*                         should include space for the terminating NULL
*                         Should be >= 9
*
*Exit:
*       errno_t = 0 on success
*                 buffer contains the date in "MM/DD/YY" form
*       errno_t = correct error code on failure
*                 buffer empty NULL terminated if it is at least 1 character
*                 in size.
*
*Exceptions:
*
*******************************************************************************/

errno_t __cdecl _tstrdate_s (
        _TSCHAR *buffer,
        size_t sizeInChars
        )
{
        int month, day, year;
        SYSTEMTIME dt;                  /* Win32 time structure */

        _VALIDATE_RETURN_ERRCODE( ( buffer != NULL && sizeInChars > 0 ), EINVAL )
        _RESET_STRING(buffer, sizeInChars);
        _VALIDATE_RETURN_ERRCODE( (sizeInChars >= 9), ERANGE )

        GetLocalTime(&dt);
        month = dt.wMonth;
        day = dt.wDay;
        year = dt.wYear % 100;          /* change year into 0-99 value */

        /* store the components of the date into the string */
        /* store seperators */
        buffer[2] = buffer[5] = _T('/');
        /* store end of string */
        buffer[8] = _T('\0');
        /* store tens of month */
        buffer[0] = (_TSCHAR) (month / 10 + _T('0'));
        /* store units of month */
        buffer[1] = (_TSCHAR) (month % 10 + _T('0'));
        /* store tens of day */
        buffer[3] = (_TSCHAR) (day   / 10 + _T('0'));
        /* store units of day */
        buffer[4] = (_TSCHAR) (day   % 10 + _T('0'));
        /* store tens of year */
        buffer[6] = (_TSCHAR) (year  / 10 + _T('0'));
        /* store units of year */
        buffer[7] = (_TSCHAR) (year  % 10 + _T('0'));

        return 0;
}


/***
*_TSCHAR *_strdate(buffer) - return date in string form
*
*Purpose:
*       _strdate() returns a string containing the date in "MM/DD/YY" form
*
*Entry:
*       _TSCHAR *buffer = the address of a 9-byte user buffer
*
*Exit:
*       returns buffer, which contains the date in "MM/DD/YY" form
*
*Exceptions:
*
*******************************************************************************/

_TSCHAR * __cdecl _tstrdate (
        _TSCHAR *buffer
        )
{
    // This function assumes that buffer is 9 characters in size
    errno_t e = _tstrdate_s( buffer, 9 );
    if ( e != 0 )
    {
        return NULL;
    }
    return buffer;
}

