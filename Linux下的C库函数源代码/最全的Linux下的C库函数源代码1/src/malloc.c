/***
*malloc.c - Get a block of memory from the heap
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines the malloc() function.
*
*******************************************************************************/

#include <cruntime.h>
#include <malloc.h>
#include <internal.h>
#include <mtdll.h>
#include <dbgint.h>
#include <rterr.h>

#include <windows.h>
#include <winheap.h>
#include <rtcsup.h>

extern int _newmode;    /* malloc new() handler mode */

#ifndef _WIN64

void *V6_HeapAlloc(size_t size)
{
    void *pvReturn = NULL;
    if ( size <= __sbh_threshold )
    {
        _mlock( _HEAP_LOCK );
        __try {
            pvReturn = __sbh_alloc_block((int)size);
        }
        __finally {
            _munlock( _HEAP_LOCK );
        }
    }
    return pvReturn;
}

#ifdef CRTDLL
void *V5_HeapAlloc(size_t size)
{
    void * pvReturn = NULL;

    /* round up to the nearest paragraph */
    if ( size ) {
        size = (size + _OLD_PARASIZE - 1) & ~(_OLD_PARASIZE - 1);
    } else {
        size = _OLD_PARASIZE;
    }

    if ( size  <= __old_sbh_threshold ) {
        _mlock(_HEAP_LOCK);
        __try {
            pvReturn = __old_sbh_alloc_block(size >> _OLD_PARASHIFT);
        }
        __finally {
            _munlock(_HEAP_LOCK);
        }
    }
    return pvReturn;
}
#endif  /* CRTDLL */
#endif  /* _WIN64 */

#ifdef _DEBUG
#define _heap_alloc _heap_alloc_base
#endif  /* _DEBUG */

/***
*void *_heap_alloc_base(size_t size) - does actual allocation
*
*Purpose:
*       Same as malloc() except the new handler is not called.
*
*Entry:
*       See malloc
*
*Exit:
*       See malloc
*
*Exceptions:
*
*******************************************************************************/

__forceinline void * __cdecl _heap_alloc (size_t size)

{
#ifndef _WIN64
    void *pvReturn;
#endif  /* _WIN64 */

    if (_crtheap == 0) {
        _FF_MSGBANNER();    /* write run-time error banner */
        _NMSG_WRITE(_RT_CRT_NOTINIT);  /* write message */
        __crtExitProcess(255);  /* normally _exit(255) */
    }

#ifdef _WIN64
    return HeapAlloc(_crtheap, 0, size ? size : 1);
#else  /* _WIN64 */
    if (__active_heap == __SYSTEM_HEAP) {
        return HeapAlloc(_crtheap, 0, size ? size : 1);
    } else
    if ( __active_heap == __V6_HEAP ) {
        if (pvReturn = V6_HeapAlloc(size)) {
            return pvReturn;
        }
    }
#ifdef CRTDLL
    else if ( __active_heap == __V5_HEAP )
    {
        if (pvReturn = V5_HeapAlloc(size)) {
            return pvReturn;
        }
    }
#endif  /* CRTDLL */

    if (size == 0)
        size = 1;

    size = (size + BYTES_PER_PARA - 1) & ~(BYTES_PER_PARA - 1);

    return HeapAlloc(_crtheap, 0, size);

#endif  /* _WIN64 */
}


/***
*void *malloc(size_t size) - Get a block of memory from the heap
*
*Purpose:
*       Allocate of block of memory of at least size bytes from the heap and
*       return a pointer to it.
*
*       Calls the new appropriate new handler (if installed).
*
*Entry:
*       size_t size - size of block requested
*
*Exit:
*       Success:  Pointer to memory block
*       Failure:  NULL (or some error value)
*
*Uses:
*
*Exceptions:
*
*******************************************************************************/

void * __cdecl _malloc_base (size_t size)
{
    void *res = NULL;

    //  validate size
    if (size <= _HEAP_MAXREQ) {
        for (;;) {

            //  allocate memory block
            res = _heap_alloc(size);

            //  if successful allocation, return pointer to memory
            //  if new handling turned off altogether, return NULL

            if (res != NULL)
            {
                break;
            }
            if (_newmode == 0)
            {
                errno = ENOMEM;
                break;
            }

            //  call installed new handler
            if (!_callnewh(size))
                break;

            //  new handler was successful -- try to allocate again
        }
    } else {
        _callnewh(size);
        errno = ENOMEM;
        return NULL;
    }

    RTCCALLBACK(_RTC_Allocate_hook, (res, size, 0));
    if (res == NULL)
    {
        errno = ENOMEM;
    }
    return res;
}
