/***
*dbgnew.cpp - defines C++ new() routines, debug version
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Defines C++ new() routines.
*
*******************************************************************************/

#ifdef _DEBUG

#include <cruntime.h>
#include <malloc.h>
#include <mtdll.h>
#include <dbgint.h>
#include <rtcsup.h>

/***
*void * operator new() - Get a block of memory from the debug heap
*
*Purpose:
*       Allocate of block of memory of at least size bytes from the heap and
*       return a pointer to it.
*
*       Allocates any type of supported memory block.
*
*Entry:
*       size_t          cb          - count of bytes requested
*       int             nBlockUse   - block type
*       char *          szFileName  - file name
*       int             nLine       - line number
*
*Exit:
*       Success:  Pointer to memory block
*       Failure:  NULL (or some error value)
*
*Exceptions:
*
*******************************************************************************/
void * operator new(
        size_t cb,
        int nBlockUse,
        const char * szFileName,
        int nLine
        )
{
    void *res = _nh_malloc_dbg( cb, 1, nBlockUse, szFileName, nLine );

    RTCCALLBACK(_RTC_Allocate_hook, (res, cb, 0));

    return res;
}

/***
*void * operator new() - Get a block of memory from the debug heap
*
*Purpose:
*       Allocate of block of memory of at least size bytes from the heap and
*       return a pointer to it.
*
*       Allocates any type of supported memory block.
*
*Entry:
*       size_t          cb          - count of bytes requested
*       int             nBlockUse   - block type
*       char *          szFileName  - file name
*       int             nLine       - line number
*
*Exit:
*       Success:  Pointer to memory block
*       Failure:  NULL (or some error value)
*
*Exceptions:
*
*******************************************************************************/
void * operator new[](
        size_t cb,
        int nBlockUse,
        const char * szFileName,
        int nLine
        )
{
    void *res = operator new(cb, nBlockUse, szFileName, nLine );

    RTCCALLBACK(_RTC_Allocate_hook, (res, cb, 0));

    return res;
}

#endif  /* _DEBUG */
