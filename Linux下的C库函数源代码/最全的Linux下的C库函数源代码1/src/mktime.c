/***
*mktime.c - Convert struct tm value to __time32_t value.
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines _mktime32() and _mkgmtime32(), routines to converts a time value
*       in a tm structure (possibly incomplete) into a __time32_t value, then
*       update (all) the structure fields with "normalized" values.
*
*******************************************************************************/

#include <cruntime.h>
#include <stddef.h>
#include <ctime.h>
#include <time.h>
#include <internal.h>

/*
 * ChkAdd evaluates to TRUE if dest = src1 + src2 has overflowed
 */
#define ChkAdd(dest, src1, src2)   ( ((src1 >= 0L) && (src2 >= 0L) \
    && (dest < 0L)) || ((src1 < 0L) && (src2 < 0L) && (dest >= 0L)) )

/*
 * ChkMul evaluates to TRUE if dest = src1 * src2 has overflowed
 */
#define ChkMul(dest, src1, src2)   ( src1 ? (dest/src1 != src2) : 0 )


/*
 * Core function for both _mktime32() and _mkgmtime32()
 */
static __time32_t __cdecl _make__time32_t( struct tm *, int);


/***
*__time32_t _mktime32(tb) - Normalize user time block structure
*
*Purpose:
*       _mktime32 converts a time structure, passed in as an argument, into a
*       calendar time value in internal format (__time32_t). It also completes
*       and updates the fields the of the passed in structure with 'normalized'
*       values. There are three practical uses for this routine:
*
*       (1) Convert broken-down time to internal time format (__time32_t).
*       (2) To have _mktime32 fill in the tm_wday, tm_yday, or tm_isdst fields.
*       (3) To pass in a time structure with 'out of range' values for some
*           fields and have _mktime32 "normalize" them (e.g., pass in 1/35/87 and
*           get back 2/4/87).
*Entry:
*       struct tm *tb - pointer to a tm time structure to convert and
*                       normalize
*
*Exit:
*       If successful, _mktime32 returns the specified calender time encoded as
*       a __time32_t value. Otherwise, (__time32_t)(-1) is returned to indicate an
*       error.
*
*Exceptions:
*       None.
*
*******************************************************************************/

_CRTIMP __time32_t __cdecl _mktime32 (
        struct tm *tb
        )
{
        return( _make__time32_t(tb, 1) );
}


/***
*__time32_t _mkgmtime32(tb) - Convert broken down UTC time to __time32_t
*
*Purpose:
*       Convert a tm structure, passed in as an argument, containing a UTC
*       time value to internal format (__time32_t). It also completes and updates
*       the fields the of the passed in structure with 'normalized' values.

*Entry:
*       struct tm *tb - pointer to a tm time structure to convert and
*                       normalize
*
*Exit:
*       If successful, _mkgmtime returns the calender time encoded as __time32_t
*       Otherwise, (__time32_t)(-1) is returned to indicate an error.
*
*Exceptions:
*       None.
*
*******************************************************************************/

_CRTIMP __time32_t __cdecl _mkgmtime32 (
        struct tm *tb
        )
{
        return( _make__time32_t(tb, 0) );
}


/***
*static __time32_t _make__time32_t(tb, ultflag) -
*
*Purpose:
*       Converts a struct tm value to a __time32_t value, then updates the struct
*       tm value. Either local time or UTC is supported, based on ultflag.
*       This is the routine that actually does the work for both _mktime32() and
*       _mkgmtime32().
*
*Entry:
*       struct tm *tb - pointer to a tm time structure to convert and
*                       normalize
*       int ultflag   - use local time flag. the tb structure is assumed
*                       to represent a local date/time if ultflag > 0.
*                       otherwise, UTC is assumed.
*
*Exit:
*       If successful, _mktime32 returns the specified calender time encoded as
*       a __time32_t value. Otherwise, (__time32_t)(-1) is returned to
*       indicate an error.
*
*Exceptions:
*       None.
*
*******************************************************************************/

static __time32_t __cdecl _make__time32_t (
        struct tm *tb,
        int ultflag
        )
{
        __time32_t tmptm1, tmptm2, tmptm3;
        struct tm tbtemp;
        long dstbias = 0;
        long timezone = 0;

        _VALIDATE_RETURN( ( tb != NULL ), EINVAL, ( ( __time32_t )( -1 ) ) )

        /*
         * First, make sure tm_year is reasonably close to being in range.
         */
        if ( ((tmptm1 = tb->tm_year) < _BASE_YEAR - 1) || (tmptm1 > _MAX_YEAR
          + 1) )
            goto err_mktime;


        /*
         * Adjust month value so it is in the range 0 - 11.  This is because
         * we don't know how many days are in months 12, 13, 14, etc.
         */

        if ( (tb->tm_mon < 0) || (tb->tm_mon > 11) ) {

            /*
             * no danger of overflow because the range check above.
             */
            tmptm1 += (tb->tm_mon / 12);

            if ( (tb->tm_mon %= 12) < 0 ) {
                tb->tm_mon += 12;
                tmptm1--;
            }

            /*
             * Make sure year count is still in range.
             */
            if ( (tmptm1 < _BASE_YEAR - 1) || (tmptm1 > _MAX_YEAR + 1) )
                goto err_mktime;
        }

        /***** HERE: tmptm1 holds number of elapsed years *****/

        /*
         * Calculate days elapsed minus one, in the given year, to the given
         * month. Check for leap year and adjust if necessary.
         */
        tmptm2 = _days[tb->tm_mon];
        if ( !(tmptm1 & 3) && (tb->tm_mon > 1) )
                tmptm2++;

        /*
         * Calculate elapsed days since base date (midnight, 1/1/70, UTC)
         *
         *
         * 365 days for each elapsed year since 1970, plus one more day for
         * each elapsed leap year. no danger of overflow because of the range
         * check (above) on tmptm1.
         */
        tmptm3 = (tmptm1 - _BASE_YEAR) * 365L + ((tmptm1 - 1L) >> 2)
          - _LEAP_YEAR_ADJUST;

        /*
         * elapsed days to current month (still no possible overflow)
         */
        tmptm3 += tmptm2;

        /*
         * elapsed days to current date. overflow is now possible.
         */
        tmptm1 = tmptm3 + (tmptm2 = (__time32_t)(tb->tm_mday));
        if ( ChkAdd(tmptm1, tmptm3, tmptm2) )
            goto err_mktime;

        /***** HERE: tmptm1 holds number of elapsed days *****/

        /*
         * Calculate elapsed hours since base date
         */
        tmptm2 = tmptm1 * 24L;
        if ( ChkMul(tmptm2, tmptm1, 24L) )
            goto err_mktime;

        tmptm1 = tmptm2 + (tmptm3 = (__time32_t)tb->tm_hour);
        if ( ChkAdd(tmptm1, tmptm2, tmptm3) )
            goto err_mktime;

        /***** HERE: tmptm1 holds number of elapsed hours *****/

        /*
         * Calculate elapsed minutes since base date
         */

        tmptm2 = tmptm1 * 60L;
        if ( ChkMul(tmptm2, tmptm1, 60L) )
            goto err_mktime;

        tmptm1 = tmptm2 + (tmptm3 = (__time32_t)tb->tm_min);
        if ( ChkAdd(tmptm1, tmptm2, tmptm3) )
            goto err_mktime;

        /***** HERE: tmptm1 holds number of elapsed minutes *****/

        /*
         * Calculate elapsed seconds since base date
         */

        tmptm2 = tmptm1 * 60L;
        if ( ChkMul(tmptm2, tmptm1, 60L) )
            goto err_mktime;

        tmptm1 = tmptm2 + (tmptm3 = (__time32_t)tb->tm_sec);
        if ( ChkAdd(tmptm1, tmptm2, tmptm3) )
            goto err_mktime;

        /***** HERE: tmptm1 holds number of elapsed seconds *****/

        if  ( ultflag ) {

            /*
             * Adjust for timezone. No need to check for overflow since
             * localtime() will check its arg value
             */

            __tzset();

            _ERRCHECK(_get_dstbias(&dstbias));
            _ERRCHECK(_get_timezone(&timezone));

            tmptm1 += timezone;

            /*
             * Convert this second count back into a time block structure.
             * If localtime returns NULL, return an error.
             */
            if ( _localtime32_s(&tbtemp, &tmptm1) != 0 )
                goto err_mktime;

            /*
             * Now must compensate for DST. The ANSI rules are to use the
             * passed-in tm_isdst flag if it is non-negative. Otherwise,
             * compute if DST applies. Recall that tbtemp has the time without
             * DST compensation, but has set tm_isdst correctly.
             */
            if ( (tb->tm_isdst > 0) || ((tb->tm_isdst < 0) &&
              (tbtemp.tm_isdst > 0)) ) {
                tmptm1 += dstbias;
                if ( _localtime32_s(&tbtemp, &tmptm1) != 0 )
                    goto err_mktime;
            }

        }
        else {
            if ( _gmtime32_s(&tbtemp, &tmptm1) != 0 )
                goto err_mktime;
        }

        /***** HERE: tmptm1 holds number of elapsed seconds, adjusted *****/
        /*****       for local time if requested                      *****/

        *tb = tbtemp;
        return (__time32_t)tmptm1;

err_mktime:
        /*
         * All errors come to here
         */

        errno = EINVAL;
        return (__time32_t)(-1);
}
