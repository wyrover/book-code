/***
*crtheap.c - Get a block of memory from the heap
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines the _malloc_crt(), _calloc_crt(), _realloc_crt() function.
*
*******************************************************************************/
#include <malloc.h>
#include <windows.h>

#define INCR_WAIT 1000

unsigned long _maxwait = 0;

__forceinline unsigned long wait_a_bit(unsigned long WaitTime)
{
     Sleep(WaitTime);
     WaitTime += INCR_WAIT;
     if (WaitTime > _maxwait)      // ~30 minutes total
         WaitTime = -1;
     return WaitTime;
}

#ifdef _SYSCRT
#undef _CRTIMP
#define _CRTIMP
#endif  /* _SYSCRT */

_CRTIMP unsigned long __cdecl _set_malloc_crt_max_wait(unsigned long newvalue)
{
    unsigned long oldvalue = _maxwait;
    _maxwait = newvalue;
    return oldvalue;
}

_CRTIMP void * __cdecl _malloc_crt(size_t cb)
{
    unsigned long WaitTime = 0;
    void *pv;

Retry:
    pv = malloc(cb);
    if (!pv && _maxwait > 0) {
        WaitTime = wait_a_bit(WaitTime);
        if (WaitTime != -1)
            goto Retry;
    }
    return pv;
}

_CRTIMP void * __cdecl _calloc_crt(size_t count, size_t size)
{
    unsigned long WaitTime = 0;
    void *pv;

    extern void * __cdecl _calloc_impl (size_t, size_t, int *);

Retry:
    pv = _calloc_impl(count, size, NULL);
    if (!pv && _maxwait > 0) {
        WaitTime = wait_a_bit(WaitTime);
        if (WaitTime != -1)
            goto Retry;
    }
    return pv;
}

_CRTIMP void * __cdecl _realloc_crt(void *ptr, size_t size)
{
    unsigned long WaitTime = 0;
    void *pv;

Retry:
    pv = realloc(ptr, size);
    if (!pv && size && _maxwait > 0) {
        WaitTime = wait_a_bit(WaitTime);
        if (WaitTime != -1)
            goto Retry;
    }
    return pv;
}

_CRTIMP void * __cdecl _recalloc_crt(void *ptr, size_t count, size_t size)
{
    unsigned long WaitTime = 0;
    void *pv;

Retry:
    pv = _recalloc(ptr, count, size);
    if (!pv && size && _maxwait > 0)
    {
        WaitTime = wait_a_bit(WaitTime);
        if (WaitTime != -1)
        {
            goto Retry;
        }
    }
    return pv;
}

#if defined (_M_IX86)
void * __fastcall _malloc_crt_fastcall(size_t cb)
{
        return _malloc_crt(cb);
}

void * __fastcall _calloc_crt_fastcall(size_t count, size_t size)
{
        return _calloc_crt(count, size);
}

void * __fastcall _realloc_crt_fastcall(void *ptr, size_t size)
{
        return _realloc_crt(ptr, size);
}
#endif  /* defined (_M_IX86) */
