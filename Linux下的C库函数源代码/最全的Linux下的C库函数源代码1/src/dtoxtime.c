/***
*dtoxtime.c - convert OS local time to __time32_t
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines __loctotime32_t() - convert OS local time to internal format
*       (__time32_t).
*
*******************************************************************************/


#include <cruntime.h>
#include <time.h>
#include <ctime.h>
#include <internal.h>

/***
*__time32_t __loctotime32_t(yr, mo, dy, hr, mn, sc, dstflag) - converts OS local
*       time to internal time format (i.e., a __time32_t value)
*
*Purpose:
*       Converts a local time value, obtained in a broken down format from
*       the host OS, to __time32_t format (i.e., the number elapsed seconds since
*       01-01-70, 00:00:00, UTC).
*
*Entry:
*       int yr, mo, dy -    date
*       int hr, mn, sc -    time
*       int dstflag    -    1 if Daylight Time, 0 if Standard Time, -1 if
*                           not specified.
*
*Exit:
*       Returns calendar time value.
*
*Exceptions:
*
*******************************************************************************/

__time32_t __cdecl __loctotime32_t (
        int yr,         /* 0 based */
        int mo,         /* 1 based */
        int dy,         /* 1 based */
        int hr,
        int mn,
        int sc,
        int dstflag )
{
        int tmpdays;
        __time32_t tmptim;
        struct tm tb;
        int daylight = 0;
        long dstbias = 0;
        long timezone = 0;

        /*
         * Do a range check on the year and convert it to a delta
         * off of 1900.
         */

        _VALIDATE_RETURN(
            ( ( ( yr - 1900 ) >= _BASE_YEAR ) && ( ( yr - 1900 ) <= _MAX_YEAR ) ),
            EINVAL,
            ( ( __time32_t )( -1 ) )
        )
        yr -= 1900;

        _VALIDATE_RETURN(
            ( ( mo >= 1 ) && ( mo <= 12 ) ),
            EINVAL,
            ( ( __time32_t )( -1 ) )
        )
        _VALIDATE_RETURN(
            ( ( hr >= 0 ) && ( hr <= 23 ) ),
            EINVAL,
            ( ( __time32_t )( -1 ) )
        )
        _VALIDATE_RETURN(
            ( ( mn >= 0 ) && ( mn <= 59 ) ),
            EINVAL,
            ( ( __time32_t )( -1 ) )
        )
        _VALIDATE_RETURN(
            ( ( sc >= 0 ) && ( sc <= 59 ) ),
            EINVAL,
            ( ( __time32_t )( -1 ) )
        )
        _VALIDATE_RETURN(
            ( ( dy >= 1 ) && (
                (
                    (_days[mo] - _days[mo - 1]) >= dy) ||               // Make sure that the # of days is in valid range for the month
                    (_IS_LEAP_YEAR(yr) && mo == 2 && dy <= 29)  // Special case for Feb in a leap year
                )
            ),
            EINVAL,
            ( ( __time32_t )( -1 ) )
        )

        /*
         * Compute the number of elapsed days in the current year. Note the
         * test for a leap year would fail in the year 2100, if this was in
         * range (which it isn't).
         */
        tmpdays = dy + _days[mo - 1];
        if ( _IS_LEAP_YEAR(yr) && (mo > 2) )
            tmpdays++;

        /*
         * Compute the number of elapsed seconds since the Epoch. Note the
         * computation of elapsed leap years would break down after 2100
         * if such values were in range (fortunately, they aren't).
         */
        tmptim = /* 365 days for each year */
                 (((__time32_t)yr - _BASE_YEAR) * 365

                 /* one day for each elapsed leap year */
                 + (__time32_t)_ELAPSED_LEAP_YEARS(yr)

                 /* number of elapsed days in yr */
                 + tmpdays)

                 /* convert to hours and add in hr */
                 * 24 + hr;

        tmptim = /* convert to minutes and add in mn */
                 (tmptim * 60 + mn)

                 /* convert to seconds and add in sec */
                 * 60 + sc;

        /*
         * Account for time zone.
         */
        __tzset();

        _ERRCHECK(_get_daylight(&daylight));
        _ERRCHECK(_get_dstbias(&dstbias));
        _ERRCHECK(_get_timezone(&timezone));

        tmptim += timezone;

        /*
         * Fill in enough fields of tb for _isindst(), then call it to
         * determine DST.
         */
        tb.tm_yday = tmpdays;
        tb.tm_year = yr;
        tb.tm_mon  = mo - 1;
        tb.tm_hour = hr;
        tb.tm_min  = mn;
        tb.tm_sec  = sc;
        if ( (dstflag == 1) || ((dstflag == -1) && daylight &&
                                _isindst(&tb)) )
            tmptim += dstbias;
        return(tmptim);
}



