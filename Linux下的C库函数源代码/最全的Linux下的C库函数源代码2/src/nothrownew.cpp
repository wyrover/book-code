/***
*new.cxx - defines C++ new routine
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Defines C++ new routine.
*
*******************************************************************************/


#include <cruntime.h>
#include <crtdbg.h>
#include <malloc.h>
#include <new.h>
#include <stdlib.h>
#include <winheap.h>
#include <rtcsup.h>
#include <internal.h>

void * operator new( size_t cb )
{
    void *res;

    for (;;) {

        //  allocate memory block
        res = malloc(cb);

        //  if successful allocation, return pointer to memory

        if (res)
            break;

        //  call installed new handler
        if (!_callnewh(cb))
            break;

        //  new handler was successful -- try to allocate again
    }

    RTCCALLBACK(_RTC_Allocate_hook, (res, cb, 0));

    return res;
}

void * operator new[]( size_t cb )
{
    void *res = operator new(cb);

    RTCCALLBACK(_RTC_Allocate_hook, (res, cb, 0));

    return res;
}

