/***
*free.c - free an entry in the heap
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines the following functions:
*           freea()     - free a memory block in the heap
*           This has been moved to inline, but we need this for backwards bin compat
*
*******************************************************************************/

#define _CRT_NOFREEA
#include <malloc.h>
#include <stdlib.h>

/***
*void _freea(pblock) -
*
*Purpose:
*       Frees only block allocated on the heap, and not the ones
*       allocated on the stack.
*       See the implementation of _malloca
*
*******************************************************************************/

void __cdecl _freea(void *ptr)
{
    if (ptr != NULL)
    {
        ptr = (char*)ptr - _ALLOCA_S_MARKER_SIZE;
        if (*((unsigned int*)ptr) == _ALLOCA_S_HEAP_MARKER)
        {
            free(ptr);
        }
    }
}

