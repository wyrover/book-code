/***
*new.cxx - defines C++ new routine
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Defines C++ new routine.
*
*******************************************************************************/


#ifdef _SYSCRT
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
        res = _heap_alloc(cb);

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
#else  /* _SYSCRT */

#include <cstdlib>
#include <new>

_C_LIB_DECL
int __cdecl _callnewh(size_t size) _THROW1(_STD bad_alloc);
_END_C_LIB_DECL

void *__CRTDECL operator new(size_t size) _THROW1(_STD bad_alloc)
        {       // try to allocate size bytes
        void *p;
        while ((p = malloc(size)) == 0)
                if (_callnewh(size) == 0)
                {       // report no memory
                static const std::bad_alloc nomem;
                _RAISE(nomem);
                }

        return (p);
        }

/*
 * Copyright (c) 1992-2002 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V3.13:0009 */
#endif  /* _SYSCRT */
