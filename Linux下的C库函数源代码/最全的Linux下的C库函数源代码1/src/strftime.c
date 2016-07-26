/***
*strftime.c - String Format Time
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*
*******************************************************************************/

#include <cruntime.h>
#include <internal.h>
#include <mtdll.h>
#include <time.h>
#include <locale.h>
#include <setlocal.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <dbgint.h>
#include <malloc.h>
#include <errno.h>

/* Prototypes for local routines */
static BOOL __cdecl _expandtime(
        _locale_t plocinfo,
        char specifier,
        const struct tm *tmptr,
        char **out,
        size_t *count,
        struct __lc_time_data *lc_time,
        unsigned alternate_form);

static void __cdecl _store_str (char *in, char **out, size_t *count);

static void __cdecl _store_num (int num, int digits, char **out, size_t *count,
        unsigned no_lead_zeros);

static void __cdecl _store_number (int num, char **out, size_t *count);

static BOOL __cdecl _store_winword (
        _locale_t plocinfo,
        int field_code,
        const struct tm *tmptr,
        char **out,
        size_t *count,
        struct __lc_time_data *lc_time);

extern "C" size_t __cdecl _Strftime (
        char *string,
        size_t maxsize,
        const char *format,
        const struct tm *timeptr,
        void *lc_time_arg
        );

extern "C" size_t __cdecl _Strftime_l (
        char *string,
        size_t maxsize,
        const char *format,
        const struct tm *timeptr,
        void *lc_time_arg,
        _locale_t plocinfo
        );

/* LC_TIME data for local "C" */

extern "C"
{
__declspec(selectany) struct __lc_time_data __lc_time_c = {

        {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"},

        {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday",
                "Friday", "Saturday", },

        {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug",
                "Sep", "Oct", "Nov", "Dec"},

        {"January", "February", "March", "April", "May", "June",
                "July", "August", "September", "October",
                "November", "December"},

        {"AM", "PM"},

        { "MM/dd/yy" },
        { "dddd, MMMM dd, yyyy" },
        { "HH:mm:ss" },

        0x0409,
        1,

        0
        };

/* Pointer to the current LC_TIME data structure. */
struct __lc_time_data *__lc_time_curr = &__lc_time_c;
}

/* Codes for __lc_time_data ww_* fields for _store_winword */

#define WW_SDATEFMT     0
#define WW_LDATEFMT     1
#define WW_TIMEFMT      2

#define TIME_SEP        ':'

/*      get a copy of the current day names */
extern "C" char * __cdecl _Getdays_l (
        _locale_t plocinfo
        )
{
    const struct __lc_time_data *pt;
    size_t n, len = 0;
    char *p;
    _LocaleUpdate _loc_update(plocinfo);

    pt = __LC_TIME_CURR(_loc_update.GetLocaleT()->locinfo);

    for (n = 0; n < 7; ++n)
        len += strlen(pt->wday_abbr[n]) + strlen(pt->wday[n]) + 2;
    p = (char *)_malloc_crt(len + 1);

    if (p != 0) {
        char *s = p;

        for (n = 0; n < 7; ++n) {
            *s++ = TIME_SEP;
            _ERRCHECK(strcpy_s(s, (len + 1) - (s - p), pt->wday_abbr[n]));
            s += strlen(s);
            *s++ = TIME_SEP;
            _ERRCHECK(strcpy_s(s, (len + 1) - (s - p), pt->wday[n]));
            s += strlen(s);
        }
        *s++ = '\0';
    }

    return (p);
}
extern "C" char * __cdecl _Getdays (
        void
        )
{
    return _Getdays_l(NULL);
}

/*      get a copy of the current month names */
extern "C" char * __cdecl _Getmonths_l (
        _locale_t plocinfo
        )
{
    const struct __lc_time_data *pt;
    size_t n, len = 0;
    char *p;
    _LocaleUpdate _loc_update(plocinfo);

    pt = __LC_TIME_CURR(_loc_update.GetLocaleT()->locinfo);

    for (n = 0; n < 12; ++n)
        len += strlen(pt->month_abbr[n]) + strlen(pt->month[n]) + 2;
    p = (char *)_malloc_crt(len + 1);

    if (p != 0) {
        char *s = p;

        for (n = 0; n < 12; ++n) {
            *s++ = TIME_SEP;
            _ERRCHECK(strcpy_s(s, (len + 1) - (s - p), pt->month_abbr[n]));
            s += strlen(s);
            *s++ = TIME_SEP;
            _ERRCHECK(strcpy_s(s, (len + 1) - (s - p), pt->month[n]));
            s += strlen(s);
        }
        *s++ = '\0';
    }

    return (p);
}
extern "C" char * __cdecl _Getmonths (
        void
        )
{
    return _Getmonths_l(NULL);
}

/*      get a copy of the current time locale information */
extern "C" void * __cdecl _Gettnames_l (
        _locale_t plocinfo
        )
{
    const struct __lc_time_data *pt;
    size_t n, len = 0;
    char *p;
    _LocaleUpdate _loc_update(plocinfo);

    pt = __LC_TIME_CURR(_loc_update.GetLocaleT()->locinfo);

    for (n = 0; n < 7; ++n)
        len += strlen(pt->wday_abbr[n]) + strlen(pt->wday[n]) + 2;
    for (n = 0; n < 12; ++n)
        len += strlen(pt->month_abbr[n]) + strlen(pt->month[n]) + 2;
    len += strlen(pt->ampm[0]) + strlen(pt->ampm[1]) + 2;
    len += strlen(pt->ww_sdatefmt) + 1;
    len += strlen(pt->ww_ldatefmt) + 1;
    len += strlen(pt->ww_timefmt) + 1;
    len += sizeof (*pt);
    p = (char *)_malloc_crt(len);

    if (p != 0) {
        struct __lc_time_data *pn = (struct __lc_time_data *)p;
        char *s = (char *)p + sizeof (*pt);

        memcpy(p, pt, sizeof (*pt));
        for (n = 0; n < 7; ++n) {
            pn->wday_abbr[n] = s;
            _ERRCHECK(strcpy_s(s, len - (s - p), pt->wday_abbr[n]));
            s += strlen(s) + 1;
            pn->wday[n] = s;
            _ERRCHECK(strcpy_s(s, len - (s - p), pt->wday[n]));
            s += strlen(s) + 1;
        }
        for (n = 0; n < 12; ++n) {
            pn->month_abbr[n] = s;
            _ERRCHECK(strcpy_s(s, len - (s - p), pt->month_abbr[n]));
            s += strlen(s) + 1;
            pn->month[n] = s;
            _ERRCHECK(strcpy_s(s, len - (s - p), pt->month[n]));
            s += strlen(s) + 1;
        }
        pn->ampm[0] = s;
        _ERRCHECK(strcpy_s(s, len - (s - p), pt->ampm[0]));
        s += strlen(s) + 1;
        pn->ampm[1] = s;
        _ERRCHECK(strcpy_s(s, len - (s - p), pt->ampm[1]));
        s += strlen(s) + 1;
        pn->ww_sdatefmt = s;
        _ERRCHECK(strcpy_s(s, len - (s - p), pt->ww_sdatefmt));
        s += strlen(s) + 1;
        pn->ww_ldatefmt = s;
        _ERRCHECK(strcpy_s(s, len - (s - p), pt->ww_ldatefmt));
        s += strlen(s) + 1;
        pn->ww_timefmt = s;
        _ERRCHECK(strcpy_s(s, len - (s - p), pt->ww_timefmt));
    }

    return (p);
}
extern "C" void * __cdecl _Gettnames (
        void
        )
{
    return _Gettnames_l(NULL);
}


/***
*size_t strftime(string, maxsize, format, timeptr) - Format a time string
*
*Purpose:
*       Place characters into the user's output buffer expanding time
*       format directives as described in the user's control string.
*       Use the supplied 'tm' structure for time data when expanding
*       the format directives.
*       [ANSI]
*
*Entry:
*       char *string = pointer to output string
*       size_t maxsize = max length of string
*       const char *format = format control string
*       const struct tm *timeptr = pointer to tb data structure
*
*Exit:
*       !0 = If the total number of resulting characters including the
*       terminating null is not more than 'maxsize', then return the
*       number of chars placed in the 'string' array (not including the
*       null terminator).
*
*       0 = Otherwise, return 0 and the contents of the string are
*       indeterminate.
*
*Exceptions:
*
*******************************************************************************/

extern "C" size_t __cdecl _strftime_l (
        char *string,
        size_t maxsize,
        const char *format,
        const struct tm *timeptr,
        _locale_t plocinfo
        )
{
        return (_Strftime_l(string, maxsize, format, timeptr, 0, plocinfo));
}
extern "C" size_t __cdecl strftime (
        char *string,
        size_t maxsize,
        const char *format,
        const struct tm *timeptr
        )
{
        return (_Strftime_l(string, maxsize, format, timeptr, 0, NULL));
}

/***
*size_t _Strftime(string, maxsize, format,
*       timeptr, lc_time) - Format a time string for a given locale
*
*Purpose:
*       Place characters into the user's output buffer expanding time
*       format directives as described in the user's control string.
*       Use the supplied 'tm' structure for time data when expanding
*       the format directives. use the locale information at lc_time.
*       [ANSI]
*
*Entry:
*       char *string = pointer to output string
*       size_t maxsize = max length of string
*       const char *format = format control string
*       const struct tm *timeptr = pointer to tb data structure
*               struct __lc_time_data *lc_time = pointer to locale-specific info
*                       (passed as void * to avoid type mismatch with C++)
*
*Exit:
*       !0 = If the total number of resulting characters including the
*       terminating null is not more than 'maxsize', then return the
*       number of chars placed in the 'string' array (not including the
*       null terminator).
*
*       0 = Otherwise, return 0 and the contents of the string are
*       indeterminate.
*
*Exceptions:
*
*******************************************************************************/

extern "C" size_t __cdecl _Strftime (
        char *string,
        size_t maxsize,
        const char *format,
        const struct tm *timeptr,
        void *lc_time_arg
        )
{
    return _Strftime_l(string, maxsize, format, timeptr,
                        lc_time_arg, NULL);
}

extern "C" size_t __cdecl _Strftime_l (
        char *string,
        size_t maxsize,
        const char *format,
        const struct tm *timeptr,
        void *lc_time_arg,
        _locale_t plocinfo
        )
{
        unsigned alternate_form;
        struct __lc_time_data *lc_time;
        BOOL failed=FALSE;              /* true if a failure was reported to us */
        size_t left;                    /* space left in output string */
        char* strstart = string;
        _LocaleUpdate _loc_update(plocinfo);

        _VALIDATE_RETURN( ( string != NULL ), EINVAL, 0)
        _VALIDATE_RETURN( ( maxsize != 0 ), EINVAL, 0)
        *string = '\0';

        _VALIDATE_RETURN( ( format != NULL ), EINVAL, 0)

// Validated below
//        _VALIDATE_RETURN( ( timeptr != NULL ), EINVAL, 0)


        lc_time = lc_time_arg == 0 ? _loc_update.GetLocaleT()->locinfo->lc_time_curr :
                  (struct __lc_time_data *)lc_time_arg;

        /* Copy maxsize into temp. */
        left = maxsize;

        /* Copy the input string to the output string expanding the format
        designations appropriately.  Stop copying when one of the following
        is true: (1) we hit a null char in the input stream, or (2) there's
        no room left in the output stream. */

        while (left > 0)
        {
            switch(*format)
            {

            case('\0'):

                /* end of format input string */
                goto done;

            case('%'):

                /* Format directive.  Take appropriate action based
                on format control character. */

                if (!timeptr) {
                    _ASSERTE(timeptr!=0);
                    errno = EINVAL;
                    return 0;
                }
                format++;                       /* skip over % char */

                /* process flags */
                alternate_form = 0;
                if (*format == '#')
                {
                    alternate_form = 1;
                    format++;
                }
                if(!_expandtime (_loc_update.GetLocaleT(), *format, timeptr, &string,
                             &left,lc_time, alternate_form))
                {
                    /* if we don't have any space left, do not set the failure flag:
                     * we will simply return ERANGE and do not call _invalid_parameter_handler
                     * (see below)
                     */
                    if (left > 0)
                    {
                        failed=TRUE;
                    }
                    goto done;
                }

                format++;                       /* skip format char */
                break;


            default:

                /* store character, bump pointers, dec the char count */
                if( _isleadbyte_l((int)(*format), _loc_update.GetLocaleT()) && left > 1)
                {
                    /* catch \0 directly following leadbyte - invalid MBCS sequence, and do not copy to output */
                    if(format[1]=='\0')
                    {
                        _ASSERTE(("Invalid MBCS character sequence passed to strftime",0));
                        failed=TRUE;
                        goto done;
                    }
                    else
                    {
                        *string++ = *format++;
                        left--;
                    }
                }


                *string++ = *format++;
                left--;
                break;
            }
        }


        /* All done.  See if we terminated because we hit a null char or because
        we ran out of space */

        done:

        if (!failed && left > 0) {

            /* Store a terminating null char and return the number of chars
            we stored in the output string. */

            *string = '\0';
            return(maxsize-left);
        }
        else
        {
            /* error - return an empty string */
            *(strstart)='\0';

            /* now return our error/insufficient buffer indication */
            if ( !failed && left <= 0 )
            {
                /* do not report this as an error to allow the caller to resize */
                errno=ERANGE;
            }
            else
            {
                _VALIDATE_RETURN( FALSE, EINVAL, 0);
            }
            /* unused but compiler can't tell */
            return 0;
        }
}


/***
*_expandtime() - Expand the conversion specifier
*
*Purpose:
*       Expand the given strftime conversion specifier using the time struct
*       and store it in the supplied buffer.
*
*       The expansion is locale-dependent.
*
*       *** For internal use with strftime() only ***
*
*Entry:
*       char specifier = strftime conversion specifier to expand
*       const struct tm *tmptr = pointer to time/date structure
*       char **string = address of pointer to output string
*       size_t *count = address of char count (space in output area)
*       struct __lc_time_data *lc_time = pointer to locale-specific info
*
*Exit:
*       BOOL true for success, false for failure
*
*Exceptions:
*
*******************************************************************************/

static BOOL __cdecl _expandtime (
        _locale_t plocinfo,
        char specifier,
        const struct tm *timeptr,
        char **string,
        size_t *left,
        struct __lc_time_data *lc_time,
        unsigned alternate_form
        )
{
        unsigned temp;                  /* temps */
        int wdaytemp;

        /* Use a copy of the appropriate __lc_time_data pointer.  This
        should prevent the necessity of locking/unlocking in mthread
        code (if we can guarantee that the various __lc_time data
        structures are always in the same segment). contents of time
        strings structure can now change, so thus we do use locking */

        switch(specifier) {             /* switch on specifier */

        case('a'):              /* abbreviated weekday name */
        {
            _VALIDATE_RETURN( ( ( timeptr->tm_wday >=0 ) && ( timeptr->tm_wday <= 6 ) ), EINVAL, FALSE)
            _store_str((char *)(lc_time->wday_abbr[timeptr->tm_wday]),
                     string, left);
            break;
        }


        case('A'):              /* full weekday name */
        {
            _VALIDATE_RETURN( ( ( timeptr->tm_wday >=0 ) && ( timeptr->tm_wday <= 6 ) ), EINVAL, FALSE)
            _store_str((char *)(lc_time->wday[timeptr->tm_wday]),
                     string, left);
            break;
        }

        case('b'):              /* abbreviated month name */
        {
            _VALIDATE_RETURN( ( ( timeptr->tm_mon >=0 ) && ( timeptr->tm_mon <= 11 ) ), EINVAL, FALSE)
            _store_str((char *)(lc_time->month_abbr[timeptr->tm_mon]),
                     string, left);
            break;
        }

        case('B'):              /* full month name */
        {
            _VALIDATE_RETURN( ( ( timeptr->tm_mon >=0 ) && ( timeptr->tm_mon <= 11 ) ), EINVAL, FALSE)
            _store_str((char *)(lc_time->month[timeptr->tm_mon]),
                     string, left);
            break;
        }

        case('c'):              /* date and time display */
            if (alternate_form)
            {
                if(!_store_winword( plocinfo,
                                WW_LDATEFMT,
                                timeptr,
                                string,
                                left,
                                lc_time))
                {
                    return FALSE;
                }

                if (*left == 0)
                    return FALSE;
                *(*string)++=' ';
                (*left)--;
                if(!_store_winword( plocinfo,
                                WW_TIMEFMT,
                                timeptr,
                                string,
                                left,
                                lc_time))
                {
                    return FALSE;
                }

            }
            else {
                if(!_store_winword( plocinfo,
                                WW_SDATEFMT,
                                timeptr,
                                string,
                                left,
                                lc_time))
                {
                    return FALSE;
                }
                if (*left == 0)
                    return FALSE;
                *(*string)++=' ';
                (*left)--;
                if(!_store_winword( plocinfo,
                                WW_TIMEFMT,
                                timeptr,
                                string,
                                left,
                                lc_time))
                {
                    return FALSE;
                }
            }
            break;

        case('d'):              /* mday in decimal (01-31) */
            /* pass alternate_form as the no leading zeros flag */
        {
            _VALIDATE_RETURN( ( ( timeptr->tm_mday >=1 ) && ( timeptr->tm_mday <= 31 ) ), EINVAL, FALSE)
            _store_num(timeptr->tm_mday, 2, string, left,
                       alternate_form);
            break;
        }

        case('H'):              /* 24-hour decimal (00-23) */
            /* pass alternate_form as the no leading zeros flag */
        {
            _VALIDATE_RETURN( ( ( timeptr->tm_hour >=0 ) && ( timeptr->tm_hour <= 23 ) ), EINVAL, FALSE)
            _store_num(timeptr->tm_hour, 2, string, left,
                       alternate_form);
            break;
        }

        case('I'):              /* 12-hour decimal (01-12) */
        {
            _VALIDATE_RETURN( ( ( timeptr->tm_hour >=0 ) && ( timeptr->tm_hour <= 23 ) ), EINVAL, FALSE)
            if (!(temp = timeptr->tm_hour%12))
                temp=12;
            /* pass alternate_form as the no leading zeros flag */
            _store_num(temp, 2, string, left, alternate_form);
            break;
        }

        case('j'):              /* yday in decimal (001-366) */
            /* pass alternate_form as the no leading zeros flag */
        {
            _VALIDATE_RETURN( ( ( timeptr->tm_yday >=0 ) && ( timeptr->tm_yday <= 365 ) ), EINVAL, FALSE)
            _store_num(timeptr->tm_yday+1, 3, string, left,
                       alternate_form);
            break;
        }

        case('m'):              /* month in decimal (01-12) */
            /* pass alternate_form as the no leading zeros flag */
        {
            _VALIDATE_RETURN( ( ( timeptr->tm_mon >=0 ) && ( timeptr->tm_mon <= 11 ) ), EINVAL, FALSE)
            _store_num(timeptr->tm_mon+1, 2, string, left,
                       alternate_form);
            break;
        }

        case('M'):              /* minute in decimal (00-59) */
            /* pass alternate_form as the no leading zeros flag */
        {
            _VALIDATE_RETURN( ( ( timeptr->tm_min >=0 ) && ( timeptr->tm_min <= 59 ) ), EINVAL, FALSE)
            _store_num(timeptr->tm_min, 2, string, left,
                       alternate_form);
            break;
        }

        case('p'):              /* AM/PM designation */
        {
            _VALIDATE_RETURN( ( ( timeptr->tm_hour >=0 ) && ( timeptr->tm_hour <= 23 ) ), EINVAL, FALSE)
            if (timeptr->tm_hour <= 11)
                _store_str((char *)(lc_time->ampm[0]), string, left);
            else
                _store_str((char *)(lc_time->ampm[1]), string, left);
            break;
        }

        case('S'):              /* secs in decimal (00-59) */
            /* pass alternate_form as the no leading zeros flag */
        {
            _VALIDATE_RETURN( ( ( timeptr->tm_sec >=0 ) && ( timeptr->tm_sec <= 59 ) ), EINVAL, FALSE)
            _store_num(timeptr->tm_sec, 2, string, left,
                       alternate_form);
            break;
        }

        case('U'):              /* sunday week number (00-53) */
            _VALIDATE_RETURN( ( ( timeptr->tm_wday >=0 ) && ( timeptr->tm_wday <= 6 ) ), EINVAL, FALSE)
            wdaytemp = timeptr->tm_wday;
            goto weeknum;   /* join common code */

        case('w'):              /* week day in decimal (0-6) */
            /* pass alternate_form as the no leading zeros flag */
        {
            _VALIDATE_RETURN( ( ( timeptr->tm_wday >=0 ) && ( timeptr->tm_wday <= 6 ) ), EINVAL, FALSE)
            _store_num(timeptr->tm_wday, 1, string, left,
                       alternate_form);
            break;
        }

        case('W'):              /* monday week number (00-53) */
            _VALIDATE_RETURN( ( ( timeptr->tm_wday >=0 ) && ( timeptr->tm_wday <= 6 ) ), EINVAL, FALSE)
            if (timeptr->tm_wday == 0)  /* monday based */
                wdaytemp = 6;
            else
                wdaytemp = timeptr->tm_wday-1;
        weeknum:
            _VALIDATE_RETURN( ( ( timeptr->tm_yday >=0 ) && ( timeptr->tm_yday <= 365 ) ), EINVAL, FALSE)
            if (timeptr->tm_yday < wdaytemp)
                temp = 0;
            else {
                temp = timeptr->tm_yday/7;
                if ((timeptr->tm_yday%7) >= wdaytemp)
                    temp++;
            }
            /* pass alternate_form as the no leading zeros flag */
            _store_num(temp, 2, string, left, alternate_form);
            break;

        case('x'):              /* date display */
            if (alternate_form)
            {
                if(!_store_winword( plocinfo,
                                WW_LDATEFMT,
                                timeptr,
                                string,
                                left,
                                lc_time))
                {
                    return FALSE;
                }
            }
            else
            {
                if(!_store_winword( plocinfo,
                                WW_SDATEFMT,
                                timeptr,
                                string,
                                left,
                                lc_time))
                {
                    return FALSE;
                }
            }
            break;

        case('X'):              /* time display */
            if(!_store_winword( plocinfo,
                            WW_TIMEFMT,
                            timeptr,
                            string,
                            left,
                            lc_time))
                {
                    return FALSE;
                }
            break;

        case('y'):              /* year w/o century (00-99) */
        {
            _VALIDATE_RETURN( ( timeptr->tm_year >=0 ), EINVAL, FALSE)
            temp = timeptr->tm_year%100;
            /* pass alternate_form as the no leading zeros flag */
            _store_num(temp, 2, string, left, alternate_form);
            break;
        }

        case('Y'):              /* year w/ century */
        {
            _VALIDATE_RETURN( ( timeptr->tm_year >=0 ), EINVAL, FALSE)
            temp = (((timeptr->tm_year/100)+19)*100) +
                   (timeptr->tm_year%100);
            /* pass alternate_form as the no leading zeros flag */
            _store_num(temp, 4, string, left, alternate_form);
            break;
        }

        case('Z'):              /* time zone name, if any */
        case('z'):              /* time zone name, if any */
            __tzset();      /* Set time zone info */
_BEGIN_SECURE_CRT_DEPRECATION_DISABLE
            _store_str(_tzname[((timeptr->tm_isdst)?1:0)],
                     string, left);
_END_SECURE_CRT_DEPRECATION_DISABLE
            break;

        case('%'):              /* percent sign */
            *(*string)++ = '%';
            (*left)--;
            break;

        case('\004'):           /* Workaround issue in older RogueWave libraries */
        case('\015'):
            break;

        default:                /* unknown format directive */
            /* ignore the directive and continue */
            /* [ANSI: Behavior is undefined.]    */
            _ASSERTE( ( "Invalid format directive" , 0 ) );
            return FALSE;
            break;

        }       /* end % switch */

        return TRUE;
}


/***
*_store_str() - Copy a time string
*
*Purpose:
*       Copy the supplied time string into the output string until
*       (1) we hit a null in the time string, or (2) the given count
*       goes to 0.
*
*       *** For internal use with strftime() only ***
*
*Entry:
*       char *in = pointer to null terminated time string
*       char **out = address of pointer to output string
*       size_t *count = address of char count (space in output area)
*
*Exit:
*       none
*Exceptions:
*
*******************************************************************************/

static void __cdecl _store_str (
        char *in,
        char **out,
        size_t *count
        )
{

        while ((*count != 0) && (*in != '\0')) {
            *(*out)++ = *in++;
            (*count)--;
        }
}


/***
*_store_num() - Convert a number to ascii and copy it
*
*Purpose:
*       Convert the supplied number to decimal and store
*       in the output buffer.  Update both the count and
*       buffer pointers.
*
*       *** For internal use with strftime() only ***
*
*Entry:
*       int num                 = pointer to integer value
*       int digits              = # of ascii digits to put into string
*       char **out              = address of pointer to output string
*       size_t *count           = address of char count (space in output area)
*       unsigned no_lead_zeros  = flag indicating that padding by leading
*                                 zeros is not necessary
*
*Exit:
*       none
*Exceptions:
*
*******************************************************************************/

static void __cdecl _store_num (
        int num,
        int digits,
        char **out,
        size_t *count,
        unsigned no_lead_zeros
        )
{
        int temp = 0;

        if (no_lead_zeros) {
            _store_number (num, out, count);
            return;
        }

        if ((size_t)digits < *count)  {
            for (digits--; (digits+1); digits--) {
                (*out)[digits] = (char)('0' + num % 10);
                num /= 10;
                temp++;
            }
            *out += temp;
            *count -= temp;
        }
        else
            *count = 0;
}

/***
*_store_number() - Convert positive integer to string
*
*Purpose:
*       Convert positive integer to a string and store it in the output
*       buffer with no null terminator.  Update both the count and
*       buffer pointers.
*
*       Differs from _store_num in that the precision is not specified,
*       and no leading zeros are added.
*
*       *** For internal use with strftime() only ***
*
*       Created from xtoi.c
*
*Entry:
*       int num = pointer to integer value
*       char **out = address of pointer to output string
*       size_t *count = address of char count (space in output area)
*
*Exit:
*       none
*
*Exceptions:
*       The buffer is filled until it is out of space.  There is no
*       way to tell beforehand (as in _store_num) if the buffer will
*       run out of space.
*
*******************************************************************************/

static void __cdecl _store_number (
        int num,
        char **out,
        size_t *count
        )
{
        char *p;                /* pointer to traverse string */
        char *firstdig;         /* pointer to first digit */
        char temp;              /* temp char */

        p = *out;

        /* put the digits in the buffer in reverse order */
        if (*count > 1)
        {
            do {
                *p++ = (char) (num % 10 + '0');
                (*count)--;
            } while ((num/=10) > 0 && *count > 1);
        }

        firstdig = *out;                /* firstdig points to first digit */
        *out = p;                       /* return pointer to next space */
        p--;                            /* p points to last digit */

        /* reverse the buffer */
        do {
            temp = *p;
            *p-- = *firstdig;
            *firstdig++ = temp;     /* swap *p and *firstdig */
        } while (firstdig < p);         /* repeat until halfway */
}


/***
*_store_winword() - Store date/time in WinWord format
*
*Purpose:
*       Format the date/time in the supplied WinWord format
*       and store it in the supplied buffer.
*
*       *** For internal use with strftime() only ***
*
*       For simple localized Gregorian calendars (calendar type 1), the WinWord
*       format is converted token by token to strftime conversion specifiers.
*       _expandtime is then called to do the work.  The WinWord format is
*       expected to be a character string (not wide-chars).
*
*       For other calendar types, the Win32 APIs GetDateFormat/GetTimeFormat
*       are instead used to do all formatting, so that this routine doesn't
*       have to know about era/period strings, year offsets, etc.
*
*
*Entry:
*       int field_code = code for ww_* field with format
*       const struct tm *tmptr = pointer to time/date structure
*       char **out = address of pointer to output string
*       size_t *count = address of char count (space in output area)
*       struct __lc_time_data *lc_time = pointer to locale-specific info
*
*Exit:
*       BOOL true for success, false for failure
*
*Exceptions:
*
*******************************************************************************/

static BOOL __cdecl _store_winword (
        _locale_t plocinfo,
        int field_code,
        const struct tm *tmptr,
        char **out,
        size_t *count,
        struct __lc_time_data *lc_time
        )
{
        const char *format;
        char specifier;
        const char *p;
        int repeat;
        char *ampmstr;
        unsigned no_lead_zeros;

        switch (field_code)
        {
        case WW_SDATEFMT:
            format = lc_time->ww_sdatefmt;
            break;
        case WW_LDATEFMT:
            format = lc_time->ww_ldatefmt;
            break;
        case WW_TIMEFMT:
        default:
            format = lc_time->ww_timefmt;
            break;
        }

        if (lc_time->ww_caltype != 1)
        {
            /* We have something other than the basic Gregorian calendar */

            SYSTEMTIME SystemTime;
            int cch;
            int (WINAPI * FormatFunc)(LCID, DWORD, const SYSTEMTIME *,
                                      LPCSTR, LPSTR, int);

            if (field_code != WW_TIMEFMT)
                FormatFunc = GetDateFormat;
            else
                FormatFunc = GetTimeFormat;

            _VALIDATE_RETURN(
                ( tmptr->tm_year >= 0 ),
                EINVAL,
                FALSE
            )
            // month 0 based
            _VALIDATE_RETURN(
                ( ( tmptr->tm_mon  >= 0 ) && ( tmptr->tm_mon  <= 11 ) ),
                EINVAL,
                FALSE
            )
            // hour/min/sec 0 based
            _VALIDATE_RETURN(
                ( ( tmptr->tm_hour >= 0 ) && ( tmptr->tm_hour <= 23 ) ),
                EINVAL,
                FALSE
            )
            _VALIDATE_RETURN(
                ( ( tmptr->tm_min  >= 0 ) && ( tmptr->tm_min  <= 59 ) ),
                EINVAL,
                FALSE
            )
            _VALIDATE_RETURN(
                ( ( tmptr->tm_sec  >= 0 ) && ( tmptr->tm_sec  <= 59 ) ),
                EINVAL,
                FALSE
            )
            // day 1 based
            _VALIDATE_RETURN(
                (
                    ( tmptr->tm_mday >= 1 ) &&
                    (
                        // Day is in valid range for the month
                        ( ( _days[ tmptr->tm_mon + 1 ] - _days[ tmptr->tm_mon ] ) >=
                                tmptr->tm_mday ) ||
                        // Special case for Feb in a leap year
                        (
                            ( IS_LEAP_YEAR( tmptr->tm_year ) ) &&
                            ( tmptr->tm_mon == 2 ) &&
                            ( tmptr->tm_mday <= 29 )
                        )
                    )
                ),
                EINVAL,
                FALSE
            )

            SystemTime.wYear   = (WORD)(tmptr->tm_year + 1900);
            SystemTime.wMonth  = (WORD)(tmptr->tm_mon + 1);
            SystemTime.wDay    = (WORD)(tmptr->tm_mday);
            SystemTime.wHour   = (WORD)(tmptr->tm_hour);
            SystemTime.wMinute = (WORD)(tmptr->tm_min);
            SystemTime.wSecond = (WORD)(tmptr->tm_sec);
            SystemTime.wMilliseconds = 0;

            /* Find buffer size required */
            cch = FormatFunc(lc_time->ww_lcid, 0, &SystemTime,
                             format, NULL, 0);

            if (cch != 0)
            {
                char *buffer;

                /* Allocate buffer, first try stack, then heap */
                buffer = (char *)_malloca(cch);
                if (buffer != NULL)
                {
                    /* Do actual date/time formatting */
                    cch = FormatFunc(lc_time->ww_lcid, 0, &SystemTime,
                                     format, buffer, cch);

                    /* Copy to output buffer */
                    p = buffer;
                    while (--cch > 0 && *count > 0) {
                        *(*out)++ = *p++;
                        (*count)--;
                    }

                    _freea(buffer);
                    return TRUE;
                }
            }

            /* In case of error, just fall through to localized Gregorian */
        }

        while (*format && *count != 0)
        {
            specifier = 0;          /* indicate no match */
            no_lead_zeros = 0;      /* default is print leading zeros */

            /* count the number of repetitions of this character */
            for (repeat=0, p=format; *p++ == *format; repeat++);
            /* leave p pointing to the beginning of the next token */
            p--;

            /* switch on ascii format character and determine specifier */
            switch (*format)
            {
            case 'M':
                switch (repeat)
                {
                case 1: no_lead_zeros = 1;  /* fall thru */
                case 2: specifier = 'm'; break;
                case 3: specifier = 'b'; break;
                case 4: specifier = 'B'; break;
                } break;
            case 'd':
                switch (repeat)
                {
                case 1: no_lead_zeros = 1;  /* fall thru */
                case 2: specifier = 'd'; break;
                case 3: specifier = 'a'; break;
                case 4: specifier = 'A'; break;
                } break;
            case 'y':
                switch (repeat)
                {
                case 2: specifier = 'y'; break;
                case 4: specifier = 'Y'; break;
                } break;
            case 'h':
                switch (repeat)
                {
                case 1: no_lead_zeros = 1;  /* fall thru */
                case 2: specifier = 'I'; break;
                } break;
            case 'H':
                switch (repeat)
                {
                case 1: no_lead_zeros = 1;  /* fall thru */
                case 2: specifier = 'H'; break;
                } break;
            case 'm':
                switch (repeat)
                {
                case 1: no_lead_zeros = 1;  /* fall thru */
                case 2: specifier = 'M'; break;
                } break;
            case 's': /* for compatibility; not strictly WinWord */
                switch (repeat)
                {
                case 1: no_lead_zeros = 1;  /* fall thru */
                case 2: specifier = 'S'; break;
                } break;
            case 'A':
            case 'a':
                if (!__ascii_stricmp(format, "am/pm"))
                    p = format + 5;
                else if (!__ascii_stricmp(format, "a/p"))
                    p = format + 3;
                specifier = 'p';
                break;
            case 't': /* t or tt time marker suffix */
                if ( tmptr->tm_hour <= 11 )
                    ampmstr = lc_time->ampm[0];
                else
                    ampmstr = lc_time->ampm[1];

                if ( (repeat == 1) && (*count > 0) ) {
                    if ( _isleadbyte_l((int)*ampmstr, plocinfo) && (*count > 1) )
                    {
                        /* catch \0 directly following leadbyte - invalid MBCS sequence, and do not copy to output */
                        if(ampmstr[1]=='\0')
                        {
                            _ASSERTE(("Invalid MBCS character sequence found in locale AMPM string",0));
                            return FALSE;
                        }

                        *(*out)++ = *ampmstr++;
                        (*count)--;
                    }

                    *(*out)++ = *ampmstr++;
                    (*count)--;
                } else {
                    while (*ampmstr != 0 && *count > 0) {
                        if (_isleadbyte_l((int)*ampmstr, plocinfo) && *count > 1) {
                            /* handle dud string leadbyte, EOS */
                            if(ampmstr[1]=='\0')
                            {
                                _ASSERTE(("Invalid MBCS character sequence found in locale AMPM string",0));
                                return FALSE;
                            }
                            *(*out)++ = *ampmstr++;
                            (*count)--;
                        }
                        *(*out)++ = *ampmstr++;
                        (*count)--;
                    }
                }
                format = p;
                continue;

            case '\'': /* literal string */
                if (repeat & 1) /* odd number */
                {
                    format += repeat;
                    while (*format && *count != 0)
                    {
                        if (*format == '\'')
                        {
                            format++;
                            break;
                        }
                        if ( _isleadbyte_l((int)*format, plocinfo) && (*count > 1) )
                        {
                            /* catch \0 directly following leadbyte - invalid MBCS sequence, and do not copy to output */
                            if(format[1]=='\0')
                            {
                                _ASSERTE(("Invalid MBCS character sequence passed into strftime",0));
                                return FALSE;
                            }

                            *(*out)++ = *format++;
                            (*count)--;
                        }

                        *(*out)++ = *format++;
                        (*count)--;
                    }
                }
                else { /* even number */
                    format += repeat;
                }
                continue;

            default: /* non-control char, print it */
                break;
            } /* switch */

            /* expand specifier, or copy literal if specifier not found */
            if (specifier)
            {
                if (!_expandtime( plocinfo,
                             specifier,
                             tmptr,
                             out,
                             count,
                             lc_time,
                             no_lead_zeros))
                {
                    return FALSE;
                }
                format = p; /* bump format up to the next token */
            } else {
                if ( _isleadbyte_l((int)*format, plocinfo) && (*count > 1))
                {
                    /* catch \0 directly following leadbyte - invalid MBCS sequence, and do not copy to output */
                    if(format[1]=='\0')
                    {
                        _ASSERTE(("Invalid MBCS character sequence passed into strftime",0));
                        return FALSE;
                    }

                    *(*out)++ = *format++;
                    (*count)--;
                }
                *(*out)++ = *format++;
                (*count)--;
            }
        } /* while */

        return TRUE;
}
