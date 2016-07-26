/***
*rand_s.c - random number generator
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines rand_s() - random number generator
*
*******************************************************************************/

#include <windows.h>
#include <cruntime.h>
#include <mtdll.h>
#include <stddef.h>
#include <stdlib.h>
#include <internal.h>

/***
*errno_t rand_s(unsigned int *_RandomValue) - returns a random number
*
*Purpose:
*       returns a random number.
*
*Entry:
*       Non NULL out parameter.
*
*Exit:
*   errno_t - 0 if sucessful
*             error value on failure
*
*       Out parameter -
*             set to random value on success
*             set to 0 on error
*
*Exceptions:
*   Works only in Win2k and above. Will call invalid parameter if RtlGenRandom is not
*   available.
*
*******************************************************************************/

/* Review : Remove following after the define is added to SDK header */
#ifndef RtlGenRandom
#define RtlGenRandom    SystemFunction036
#else  /* RtlGenRandom */
#pragma message ( __FILE__ " : Review comments in this file" )
#endif  /* RtlGenRandom */

#define __TO_STR(x) #x
#define _TO_STR(x)  __TO_STR(x)



typedef BOOL (APIENTRY *PGENRANDOM)( PVOID, ULONG );

static PGENRANDOM g_pfnRtlGenRandom;

void __cdecl _initp_misc_rand_s(void* enull)
{
    g_pfnRtlGenRandom = (PGENRANDOM) enull;
}

errno_t __cdecl rand_s
(
    unsigned int *_RandomValue
)
{
    PGENRANDOM pfnRtlGenRandom = _decode_pointer(g_pfnRtlGenRandom);
    _VALIDATE_RETURN_ERRCODE( _RandomValue != NULL, EINVAL );
    *_RandomValue = 0; // Review : better value to initialize it to?

    if ( pfnRtlGenRandom == NULL )
    {
        PGENRANDOM encoded;
        void* enull;

        // Advapi32.dll is unloaded when the App exits.
        HMODULE hAdvApi32=LoadLibrary("ADVAPI32.DLL");
                if (!hAdvApi32)
                {
            _VALIDATE_RETURN_ERRCODE(("rand_s is not available on this platform", 0), EINVAL);
                }

        pfnRtlGenRandom = ( PGENRANDOM ) GetProcAddress( hAdvApi32, _TO_STR( RtlGenRandom ) );
        if ( pfnRtlGenRandom == NULL )
        {
            _VALIDATE_RETURN_ERRCODE(("rand_s is not available on this platform", 0), _get_errno_from_oserr(GetLastError()));
        }
        encoded = (PGENRANDOM) _encode_pointer(pfnRtlGenRandom);
        enull = _encoded_null();
#ifdef _M_IX86
        if ( (void*)(LONG_PTR)InterlockedExchange(
                ( LONG* )&g_pfnRtlGenRandom,
                ( LONG )( LONG_PTR )encoded)
            != enull )
#else  /* _M_IX86 */
        if ( InterlockedExchangePointer(
                ( void** )&g_pfnRtlGenRandom,
                ( void* )encoded)
            != enull )
#endif  /* _M_IX86 */
        {
            /* A different thread has already loaded advapi32.dll. */
            FreeLibrary( hAdvApi32 );
        }
    }

    if ( !(*pfnRtlGenRandom)( _RandomValue, ( ULONG )sizeof( unsigned int ) ) )
    {
        errno = ENOMEM;
        return errno;
    }
    return 0;
}
