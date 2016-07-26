/***
*strtime.c - contains the function "_strtime()" ans "_strtime_s()"
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       contains the function _strtime() and _strtime_s()
*
*******************************************************************************/


#include <cruntime.h>
#include <time.h>
#include <tchar.h>
#include <oscalls.h>
#include <internal.h>
#include <internal_securecrt.h>

/***
*errno_t _strtime_s(buffer, sizeInChars) - return time in string form
*
*Purpose:
*       _strtime_s() returns a string containing the time in "HH:MM:SS" form
*
*Entry:
*       _TSCHAR *buffer = the address of a 9-byte user buffer
*       size_t  sizeInChars = size of the buffer in characters.
*                         should include space for the terminating NULL
*                         Should be >= 9
*
*Exit:
*       errno_t = 0 on success
*                 buffer contains the time in "HH:MM:SS" form
*       errno_t = correct error code on failure
*                 buffer empty NULL terminated if it is at least 1 character
*                 in size.
*
*
*Exceptions:
*
*******************************************************************************/

errno_t __cdecl _tstrtime_s (
        _TSCHAR *buffer,
        size_t sizeInChars
        )
{
        int hours, minutes, seconds;
        SYSTEMTIME dt;                       /* Win32 time structure */

        _VALIDATE_RETURN_ERRCODE( ( buffer != NULL && sizeInChars > 0 ), EINVAL )
        _RESET_STRING(buffer, sizeInChars);
        _VALIDATE_RETURN_ERRCODE( ( sizeInChars >= 9 ), ERANGE )

        GetLocalTime(&dt);

        hours = dt.wHour;
        minutes = dt.wMinute;
        seconds = dt.wSecond;

        /* store the components of the time into the string */
        /* store separators */
        buffer[2] = buffer[5] = _T(':');
        /* store end of string */
        buffer[8] = _T('\0');
        /* store tens of hour */
        buffer[0] = (_TSCHAR) (hours   / 10 + _T('0'));
        /* store units of hour */
        buffer[1] = (_TSCHAR) (hours   % 10 + _T('0'));
        /* store tens of minute */
        buffer[3] = (_TSCHAR) (minutes / 10 + _T('0'));
        /* store units of minute */
        buffer[4] = (_TSCHAR) (minutes % 10 + _T('0'));
        /* store tens of second */
        buffer[6] = (_TSCHAR) (seconds / 10 + _T('0'));
        /* store units of second */
        buffer[7] = (_TSCHAR) (seconds % 10 + _T('0'));

        return 0;
}

/***
*_TSCHAR *_strtime(buffer) - return time in string form
*
*Purpose:
*       _strtime() returns a string containing the time in "HH:MM:SS" form
*
*Entry:
*       _TSCHAR *buffer = the address of a 9-byte user buffer
*
*Exit:
*       returns buffer, which contains the time in "HH:MM:SS" form
*
*Exceptions:
*
*******************************************************************************/

_TSCHAR * __cdecl _tstrtime (
        _TSCHAR *buffer
        )
{
    // This function assumes that buffer is 9 characters in size
    errno_t e = _tstrtime_s( buffer, 9 );
    if ( e != 0 )
    {
        return NULL;
    }
    return buffer;
}

