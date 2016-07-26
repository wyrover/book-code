/***
*gmtime.c - breaks down a time value into GMT date/time info
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _gmtime32() - breaks the clock value down into GMT time/date
*       information; returns pointer to structure with the data.
*
*******************************************************************************/

#include <cruntime.h>
#include <time.h>
#include <ctime.h>
#include <stddef.h>
#include <memory.h>
#include <internal.h>
#include <mtdll.h>
#include <malloc.h>
#include <stddef.h>
#include <dbgint.h>


/***
*errno_t _gmtime32_s(ptm, timp) - convert *timp to a structure (UTC)
*
*Purpose:
*       Converts the calendar time value, in 32 bit internal format, to
*       broken-down time (tm structure) with the corresponding UTC time.
*
*Entry:
*       const time_t *timp - pointer to time_t value to convert
*
*Exit:
*       errno_t = 0 success
*                 tm members filled-in
*       errno_t = non zero
*                 tm members initialized to -1 if ptm != NULL
*
*Exceptions:
*
*******************************************************************************/

errno_t __cdecl _gmtime32_s (
        struct tm *ptm,
        const __time32_t *timp
        )
{
        __time32_t caltim;/* = *timp; *//* calendar time to convert */
        int islpyr = 0;                 /* is-current-year-a-leap-year flag */
        REG1 int tmptim;
        REG3 int *mdays;                /* pointer to days or lpdays */
        struct tm *ptb = ptm;

        _VALIDATE_RETURN_ERRCODE( ( ptm != NULL ), EINVAL )
        memset( ptm, 0xff, sizeof( struct tm ) );

        _VALIDATE_RETURN_ERRCODE( ( timp != NULL ), EINVAL )

        caltim = *timp;
        _VALIDATE_RETURN_ERRCODE_NOEXC( ( caltim >= 0 ), EINVAL )

        /*
         * Determine years since 1970. First, identify the four-year interval
         * since this makes handling leap-years easy (note that 2000 IS a
         * leap year and 2100 is out-of-range).
         */
        tmptim = (int)(caltim / _FOUR_YEAR_SEC);
        caltim -= ((__time32_t)tmptim * _FOUR_YEAR_SEC);

        /*
         * Determine which year of the interval
         */
        tmptim = (tmptim * 4) + 70;         /* 1970, 1974, 1978,...,etc. */

        if ( caltim >= _YEAR_SEC ) {

            tmptim++;                       /* 1971, 1975, 1979,...,etc. */
            caltim -= _YEAR_SEC;

            if ( caltim >= _YEAR_SEC ) {

                tmptim++;                   /* 1972, 1976, 1980,...,etc. */
                caltim -= _YEAR_SEC;

                /*
                 * Note, it takes 366 days-worth of seconds to get past a leap
                 * year.
                 */
                if ( caltim >= (_YEAR_SEC + _DAY_SEC) ) {

                        tmptim++;           /* 1973, 1977, 1981,...,etc. */
                        caltim -= (_YEAR_SEC + _DAY_SEC);
                }
                else {
                        /*
                         * In a leap year after all, set the flag.
                         */
                        islpyr++;
                }
            }
        }

        /*
         * tmptim now holds the value for tm_year. caltim now holds the
         * number of elapsed seconds since the beginning of that year.
         */
        ptb->tm_year = tmptim;

        /*
         * Determine days since January 1 (0 - 365). This is the tm_yday value.
         * Leave caltim with number of elapsed seconds in that day.
         */
        ptb->tm_yday = (int)(caltim / _DAY_SEC);
        caltim -= (__time32_t)(ptb->tm_yday) * _DAY_SEC;

        /*
         * Determine months since January (0 - 11) and day of month (1 - 31)
         */
        if ( islpyr )
            mdays = _lpdays;
        else
            mdays = _days;


        for ( tmptim = 1 ; mdays[tmptim] < ptb->tm_yday ; tmptim++ ) ;

        ptb->tm_mon = --tmptim;

        ptb->tm_mday = ptb->tm_yday - mdays[tmptim];

        /*
         * Determine days since Sunday (0 - 6)
         */
        ptb->tm_wday = ((int)(*timp / _DAY_SEC) + _BASE_DOW) % 7;

        /*
         *  Determine hours since midnight (0 - 23), minutes after the hour
         *  (0 - 59), and seconds after the minute (0 - 59).
         */
        ptb->tm_hour = (int)(caltim / 3600);
        caltim -= (__time32_t)ptb->tm_hour * 3600L;

        ptb->tm_min = (int)(caltim / 60);
        ptb->tm_sec = (int)(caltim - (ptb->tm_min) * 60);

        ptb->tm_isdst = 0;
        return 0;

}


/***
*struct tm *__getgmtimebuf() - get the static/allocated buffer used by gmtime
*
*Purpose:
*       get the buffer used by gmtime
*
*Entry:
*
*Exit:
*       returns pointer to tm structure or NULL when allocation fails
*
*Exceptions:
*
*******************************************************************************/
struct tm * __cdecl __getgmtimebuf ()
{

        REG2 struct tm *ptb;            /* will point to gmtime buffer */
        _ptiddata ptd = _getptd_noexit();
        if (!ptd) {
            errno = ENOMEM;
            return (NULL);
        }

        /* Use per thread buffer area (malloc space, if necessary) */

        if ( (ptd->_gmtimebuf != NULL) || ((ptd->_gmtimebuf =
            _malloc_crt(sizeof(struct tm))) != NULL) )
                ptb = ptd->_gmtimebuf;
        else
        {
            errno = ENOMEM;
        }
        return ptb;
}

/***
*struct tm *_gmtime32(timp) - convert *timp to a structure (UTC)
*
*Purpose:
*       Converts the calendar time value, in 32-bit internal format, to
*       broken-down time (tm structure) with the corresponding UTC time.
*
*Entry:
*       const __time32_t *timp - pointer to time_t value to convert
*
*Exit:
*       returns pointer to filled-in tm structure.
*       returns NULL if *timp < 0L
*
*Exceptions:
*
*******************************************************************************/

struct tm * __cdecl _gmtime32 (
        const __time32_t *timp
        )
{
        errno_t e;
        struct tm *ptm = __getgmtimebuf();                 /* will point to gmtime buffer */
        if ( ptm == NULL )
        {
            return NULL;
        }

        e = _gmtime32_s( ptm, timp );
        if ( e != 0 )
        {
            return NULL;
        }
        return ptm;
}
