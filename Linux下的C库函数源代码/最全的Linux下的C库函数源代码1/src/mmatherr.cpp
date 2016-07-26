/***
*matherr.c - floating point exception handling
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*
*******************************************************************************/

#include <math.h>
#include <stddef.h>
#include <internal.h>

#pragma warning(disable:4439)   // C4439: function with a managed parameter must have a __clrcall calling convention

int _matherr_flag = 9876;


/*
 * Pointer to user-supplied _matherr routine if one has been supplied.
 */
typedef int (__clrcall * _HANDLE_MATH_ERROR_M)(struct _exception *);

_HANDLE_MATH_ERROR_M pusermatherr = (_HANDLE_MATH_ERROR_M) _encoded_null();

extern "C" _CRTIMP void __cdecl __setusermatherr( int (__cdecl *pf)(struct _exception *) );

/***
*int _matherr(struct _exception *except) - handle math errors
*
*Purpose:
*   Permits the user customize fp error handling by redefining this function.
*
*   The default matherr does nothing and returns 0
*
*Entry:
*
*Exit:
*
*Exceptions:
*******************************************************************************/
extern "C" int __cdecl _matherr(struct _exception *pexcept)
{

    /*
     * If user has supplied a _matherr implementation, pass control to
     * it and let it handle the error.
     */
    _HANDLE_MATH_ERROR_M pmatherr = (_HANDLE_MATH_ERROR_M) _decode_pointer(pusermatherr);
    if ( pmatherr != NULL )
            return pmatherr(pexcept);

    return 0;
}

/***
*void __setusermatherr ( int (__cdecl *pf)(struct exception *) )
*
*Purpose:
*       Copy pointer to user-supplied matherr routine into pusermatherr
*
*Entry:
*       pf  - pointer to an implementation of _matherr supplied by the user
*Exit:
*
*Exceptions:
*******************************************************************************/

extern "C" _MRTIMP void __cdecl __setusermatherr_m( _HANDLE_MATH_ERROR_M pf)
{
        pusermatherr = (_HANDLE_MATH_ERROR_M) _encode_pointer(pf);
        if (pf != 0) {
            __setusermatherr(_matherr);
        }
        _matherr_flag = 0;
}
