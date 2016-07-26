/***
*thrownew.cpp - explicit replacement operator new that throws std::bad_alloc
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Provide an explicit operator new that throws std::bad_alloc on
*       memory allocation failure.
*
*       Link with this object to get ANSI C++ new handler behavior.  This is
*       provided for those circumstances where the normal throwing new found
*       in the C++ Standard Library (libcp, libcpmt, or msvcprt.lib) isn't
*       being found by the linker before the legacy non-throwing new in the
*       main C Runtime (libc, libcmt, or msvcrt.lib).
*
*
*******************************************************************************/


/* Suppress any linker directives for the C++ Standard Library */
#define _USE_ANSI_CPP

#include <stddef.h>
#include <internal.h>
#include <new>
#include <stdlib.h>

extern "C" int __cdecl _callnewh(size_t size) _THROW1(_STD bad_alloc);

void *__CRTDECL operator new(size_t size) _THROW1(_STD bad_alloc)
{       // try to allocate size bytes
        void *p;
        while ((p = malloc(size)) == 0)
                if (_callnewh(size) == 0)
               {        // report no memory
                   static const std::bad_alloc nomem;
                   _RAISE(nomem);
               }

        return (p);
}

