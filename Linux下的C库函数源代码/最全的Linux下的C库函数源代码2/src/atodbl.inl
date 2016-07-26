/***
*atodbl.inl - convert a string to a floating point value
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*   Converts a string to a floating point value.
*
*******************************************************************************/

#ifdef _SAFECRT_IMPL
#define _LOCALE_DATA dec_point
#else  /* _SAFECRT_IMPL */
#define _LOCALE_DATA _loc_update.GetLocaleT()
#endif  /* _SAFECRT_IMPL */

/***
*int _atodbl(_CRT_DOUBLE *, char *) - char string to convert floating point
*
*Purpose:
*   _atodbl like atod converts string to floating point. but this function
*   tries to avoid floating point exceptions by using _CRT_DOUBLE struct
*   which keeps the value in normal register.
*
*Entry:
*   _CRT_DOUBLE * - number to be converted to.
*   char * - string to be converted.
*
*Exit:
*   returns 0, when sucessful or _UNDERFLOW, _OVERFLOW when applicable.
*
*Exceptions:
*
*******************************************************************************/
#ifdef _SAFECRT_IMPL
int  _safecrt_atodbl(_CRT_DOUBLE *d, char *str, char dec_point)
#else  /* _SAFECRT_IMPL */
int  _atodbl_l(_CRT_DOUBLE *d, char *str, _locale_t plocinfo)
#endif  /* _SAFECRT_IMPL */
{
    const char *EndPtr;
    _LDBL12 ld12;
    unsigned int retval1=0;
    INTRNCVT_STATUS retval2=INTRNCVT_OK;

#ifndef _SAFECRT_IMPL
    _LocaleUpdate _loc_update(plocinfo);
#endif  /* _SAFECRT_IMPL */

    /*
     * The return valued of this return is 0, _OVERFLOW or _UNDERFLOW
     * The reason behind this is once we make these routines public,
     * we need some return value that are defined in shiped headers.
     */
    retval1 = __strgtold12_l(&ld12, &EndPtr, str, 0, 0, 0, 0, _LOCALE_DATA);
    retval2 = _ld12tod(&ld12, d);
    if ((retval1 & (SLD_OVERFLOW|SLD_UNDERFLOW))==0) {
        if (retval2 == INTRNCVT_OVERFLOW) {
            return _OVERFLOW;
        } else if (retval2 == INTRNCVT_UNDERFLOW) {
            return _UNDERFLOW;
        }
    } else if (retval1 & SLD_UNDERFLOW){
        return _UNDERFLOW;
    } else if (retval1 & SLD_OVERFLOW) {
        return _OVERFLOW;
    }
    return 0;
}

#ifndef _SAFECRT_IMPL
int  _atodbl(_CRT_DOUBLE *d, char *str)
{
    return _atodbl_l(d, str, NULL);
}
#endif  /* _SAFECRT_IMPL */

#ifdef _SAFECRT_IMPL
int _safecrt_atoldbl(_LDOUBLE *d, char *str, char dec_point)
#else  /* _SAFECRT_IMPL */
int _atoldbl_l(_LDOUBLE *d, char *str, _locale_t plocinfo)
#endif  /* _SAFECRT_IMPL */
{
    const char *EndPtr;
    _LDBL12 ld12;
    unsigned int retval1=0;
    INTRNCVT_STATUS retval2=INTRNCVT_OK;

#ifndef _SAFECRT_IMPL
    _LocaleUpdate _loc_update(plocinfo);
#endif  /* _SAFECRT_IMPL */
    /*
     * The return valued of this return is 0, _OVERFLOW or _UNDERFLOW
     * The reason behind this is once we make these routines public,
     * we need some return value that are defined in shiped headers.
     */
    retval1 = __strgtold12_l(&ld12, &EndPtr, str, 1, 0, 0, 0, _LOCALE_DATA);
    retval2 = _ld12told(&ld12, d);
    if ((retval1 & (SLD_OVERFLOW|SLD_UNDERFLOW))==0) {
        if (retval2 == INTRNCVT_OVERFLOW) {
            return _OVERFLOW;
        } else if (retval2 == INTRNCVT_UNDERFLOW) {
            return _UNDERFLOW;
        }
    } else if (retval1 & SLD_UNDERFLOW){
        return _UNDERFLOW;
    } else if (retval1 & SLD_OVERFLOW) {
        return _OVERFLOW;
    }
    return 0;
}

#ifndef _SAFECRT_IMPL
int _atoldbl(_LDOUBLE *d, char *str)
{
    return _atoldbl_l(d, str, NULL);
}
#endif  /* _SAFECRT_IMPL */

#ifdef _SAFECRT_IMPL
int _safecrt_atoflt(_CRT_FLOAT *d, char *str, char dec_point)
#else  /* _SAFECRT_IMPL */
int _atoflt_l(_CRT_FLOAT *d, char *str, _locale_t plocinfo)
#endif  /* _SAFECRT_IMPL */
{
    const char *EndPtr;
    _LDBL12 ld12;
    unsigned int retval1=0;
    INTRNCVT_STATUS retval2=INTRNCVT_OK;

#ifndef _SAFECRT_IMPL
    _LocaleUpdate _loc_update(plocinfo);
#endif  /* _SAFECRT_IMPL */
    /*
     * The return valued of this return is 0, _OVERFLOW or _UNDERFLOW
     * The reason behind this is once we make these routines public,
     * we need some return value that are defined in shiped headers.
     */
    retval1 = __strgtold12_l(&ld12, &EndPtr, str, 0, 0, 0, 0, _LOCALE_DATA);
    retval2 = _ld12tof(&ld12, d);
    if ((retval1 & (SLD_OVERFLOW|SLD_UNDERFLOW))==0) {
        if (retval2 == INTRNCVT_OVERFLOW) {
            return _OVERFLOW;
        } else if (retval2 == INTRNCVT_UNDERFLOW) {
            return _UNDERFLOW;
        }
    } else if (retval1 & SLD_UNDERFLOW){
        return _UNDERFLOW;
    } else if (retval1 & SLD_OVERFLOW) {
        return _OVERFLOW;
    }
    return 0;
}

#ifndef _SAFECRT_IMPL
int _atoflt(_CRT_FLOAT *d, char *str)
{
    return _atoflt_l(d, str, NULL);
}
#endif  /* _SAFECRT_IMPL */

