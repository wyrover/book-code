/***
*delete.cpp - defines C++ scalar delete routine, non-debug version
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Defines C++ delete routine.
*
*******************************************************************************/

#ifndef _DEBUG

#include <cruntime.h>
#include <malloc.h>
#include <new.h>
#include <windows.h>
#include <rtcsup.h>

void operator delete( void * p )
{
    RTCCALLBACK(_RTC_Free_hook, (p, 0));

    free( p );
}

#endif  /* _DEBUG */
