/***
*timeset.c - contains defaults for timezone setting
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Contains the timezone values for default timezone.
*       Also contains month and day name three letter abbreviations.
*
*******************************************************************************/

#include <cruntime.h>
#include <stdlib.h>
#include <time.h>
#include <internal.h>

#undef _daylight
#undef _dstbias
#undef _timezone
#undef _tzname


long _timezone = 8 * 3600L; /* Pacific Time Zone */
int _daylight = 1;          /* Daylight Saving Time (DST) in timezone */
long _dstbias = -3600L;     /* DST offset in seconds */

/* note that NT Posix's TZNAME_MAX is only 10 */

static char tzstd[_TZ_STRINGS_SIZE] = { "PST" };
static char tzdst[_TZ_STRINGS_SIZE] = { "PDT" };

char *_tzname[2] = { tzstd, tzdst };

_CRTIMP errno_t __cdecl _get_daylight(int * _Daylight)
{
    _VALIDATE_RETURN_ERRCODE((_Daylight != NULL), EINVAL);

    /* This variable is correctly inited at startup, so no need to check if CRT init finished */
    *_Daylight = _daylight;
    return 0;
}

_CRTIMP errno_t __cdecl _get_dstbias(long * _Daylight_savings_bias)
{
    _VALIDATE_RETURN_ERRCODE((_Daylight_savings_bias != NULL), EINVAL);

    /* This variable is correctly inited at startup, so no need to check if CRT init finished */
    *_Daylight_savings_bias = _dstbias;
    return 0;
}

_CRTIMP errno_t __cdecl _get_timezone(long * _Timezone)
{
    _VALIDATE_RETURN_ERRCODE((_Timezone != NULL), EINVAL);

    /* This variable is correctly inited at startup, so no need to check if CRT init finished */
    *_Timezone = _timezone;
    return 0;
}

_CRTIMP errno_t __cdecl _get_tzname(size_t *_ReturnValue, char *_Buffer, size_t _SizeInBytes, int _Index)
{
    _VALIDATE_RETURN_ERRCODE((_Buffer != NULL && _SizeInBytes > 0) || (_Buffer == NULL && _SizeInBytes == 0), EINVAL);
    if (_Buffer != NULL)
    {
        _Buffer[0] = '\0';
    }
    _VALIDATE_RETURN_ERRCODE(_ReturnValue != NULL, EINVAL);
    _VALIDATE_RETURN_ERRCODE(_Index == 0 || _Index == 1, EINVAL);

    /* _tzname is correctly inited at startup, so no need to check if CRT init finished */
    *_ReturnValue = strlen(_tzname[_Index]) + 1;
    if (_Buffer == NULL)
    {
        /* the user is interested only in the size of the buffer */
        return 0;
    }
    if (*_ReturnValue > _SizeInBytes)
    {
        return ERANGE;
    }
    return strcpy_s(_Buffer, _SizeInBytes, _tzname[_Index]);
}



/*  Day names must be Three character abbreviations strung together */

const char __dnames[] = {
        "SunMonTueWedThuFriSat"
};

/*  Month names must be Three character abbreviations strung together */

const char __mnames[] = {
        "JanFebMarAprMayJunJulAugSepOctNovDec"
};

/***
*int * __daylight()                                 - return pointer to _daylight
*long * __dstbias()                                 - return pointer to _dstbias
*long * __timezone()                                - return pointer to __timezone
*char ** __tzname()                                 - return _tzname
*
*Purpose:
*       Returns former global variables
*
*Entry:
*       None.
*
*Exit:
*       See above.
*
*Exceptions:
*
*******************************************************************************/
int * __cdecl __daylight(void)
{
    return &(_daylight);
}

long * __cdecl __dstbias(void)
{
    return &(_dstbias);
}

long * __cdecl __timezone(void)
{
    return &(_timezone);
}

char ** __cdecl __tzname(void)
{
    return (_tzname);
}
