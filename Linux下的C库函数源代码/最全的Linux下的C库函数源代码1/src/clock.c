/***
*clock.c - Contains the clock runtime
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       The clock runtime returns the processor time used by
*       the current process.
*
*******************************************************************************/

#include <sect_attribs.h>
#include <cruntime.h>
#include <windows.h>
#include <stdio.h>
#include <time.h>

#include <internal.h>
#include <sys\timeb.h>
#include <sys\types.h>



int __cdecl __inittime(void);

#ifdef _MSC_VER

_CRTALLOC(".CRT$XIC") static _PIFV pinit = __inittime;

#endif  /* _MSC_VER */

static unsigned __int64 start_tics;

/***
*clock_t clock() - Return the processor time used by this process.
*
*Purpose:
*       This routine calculates how much time the calling process
*       has used.  At startup time, startup calls __inittime which stores
*       the initial time.  The clock routine calculates the difference
*       between the current time and the initial time.
*
*       Clock must reference _cinitime so that _cinitim.asm gets linked in.
*       That routine, in turn, puts __inittime in the startup initialization
*       routine table.
*
*Entry:
*       No parameters.
*       itime is a static structure of type timeb.
*
*Exit:
*       If successful, clock returns the number of CLK_TCKs (milliseconds)
*       that have elapsed.  If unsuccessful, clock returns -1.
*
*Exceptions:
*       None.
*
*******************************************************************************/

clock_t __cdecl clock (
        void
        )
{
        unsigned __int64 current_tics;
        FILETIME ct;

        GetSystemTimeAsFileTime( &ct );

        current_tics = (unsigned __int64)ct.dwLowDateTime +
                       (((unsigned __int64)ct.dwHighDateTime) << 32);

        /* calculate the elapsed number of 100 nanosecond units */
        current_tics -= start_tics;

        /* return number of elapsed milliseconds */
        return (clock_t)(current_tics / 10000);
}

/***
*int __inittime() - Initialize the time location
*
*Purpose:
*       This routine stores the time of the process startup.
*       It is only linked in if the user issues a clock runtime call.
*
*Entry:
*       No arguments.
*
*Exit:
*       Returns 0 to indicate no error.
*
*Exceptions:
*       None.
*
*******************************************************************************/

int __cdecl __inittime (
        void
        )
{
        FILETIME st;

        GetSystemTimeAsFileTime( &st );

        start_tics = (unsigned __int64)st.dwLowDateTime +
                     (((unsigned __int64)st.dwHighDateTime) << 32);

        return 0;
}

