/***
*systime.c - _getsystime and _setsystime
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _getsystime() and _setsystime()
*
*******************************************************************************/


#include <cruntime.h>
#include <oscalls.h>
#include <time.h>
#include <internal.h>

/***
*unsigned _getsystime(timestruc) - Get current system time
*
*Purpose:
*
*Entry:
*       struct tm * ptm - time structure
*
*Exit:
*       milliseconds of current time
*
*Exceptions:
*
*******************************************************************************/

unsigned __cdecl _getsystime(struct tm * ptm)
{
    SYSTEMTIME  st;

    _VALIDATE_RETURN( ( ptm != NULL ), EINVAL, 0 )

    GetLocalTime(&st);

    ptm->tm_isdst       = -1;   /* mktime() computes whether this is */
                                /* during Standard or Daylight time. */
    ptm->tm_sec         = (int)st.wSecond;
    ptm->tm_min         = (int)st.wMinute;
    ptm->tm_hour        = (int)st.wHour;
    ptm->tm_mday        = (int)st.wDay;
    ptm->tm_mon         = (int)st.wMonth - 1;
    ptm->tm_year        = (int)st.wYear - 1900;
    ptm->tm_wday        = (int)st.wDayOfWeek;

    /* Normalize uninitialized fields */
    _mktime32(ptm);

    return (st.wMilliseconds);
}

/***
*unsigned _setsystime(timestruc, milliseconds) - Set new system time
*
*Purpose:
*
*Entry:
*       struct tm * ptm - time structure
*       unsigned milliseconds - milliseconds of current time
*
*Exit:
*       0 if succeeds
*       system error if fails
*
*Exceptions:
*
*******************************************************************************/

unsigned __cdecl _setsystime(struct tm * ptm, unsigned uMilliseconds)
{
    SYSTEMTIME  st;

    _ASSERTE( ptm != NULL );
    if ( !( ptm != NULL ) )
    {
        return ERROR_INVALID_PARAMETER;
    }

    /* Normalize uninitialized fields */
    _mktime32(ptm);

    st.wYear            = (WORD)(ptm->tm_year + 1900);
    st.wMonth           = (WORD)(ptm->tm_mon + 1);
    st.wDay             = (WORD)ptm->tm_mday;
    st.wHour            = (WORD)(ptm->tm_hour);
    st.wMinute          = (WORD)ptm->tm_min;
    st.wSecond          = (WORD)ptm->tm_sec;
    st.wMilliseconds    = (WORD)uMilliseconds;

    if (!SetLocalTime(&st)) {
        return ((int)GetLastError());
    }

    return (0);
}

