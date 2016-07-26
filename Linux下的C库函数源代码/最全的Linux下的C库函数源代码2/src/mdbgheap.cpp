/***
*dbgheap.c - Debug CRT Heap Functions
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines pure MSIL C++ debug heap routine.
*
*******************************************************************************/

#ifdef _DEBUG

#include <crtdbg.h>
#include <internal.h>

#pragma warning(disable:4439)   // C4439: function with a managed parameter must have a __clrcall calling convention

typedef void (__clrcall *_PHEAP_m)(void *, void *);

static _PHEAP_m __pfnHeapfunc = (_PHEAP_m) _encoded_null();

static void __cdecl CrtDoForAllClientObjects_thunk(void *pData, void *pContext)
{
    _PHEAP_m pFunc = (_PHEAP_m) _decode_pointer(__pfnHeapfunc);

    if (pFunc)
    {
        pFunc(pData, pContext);
    }
}

_MRTIMP void __cdecl _CrtDoForAllClientObjects(
            _PHEAP_m pfn,
            void *pContext)
{
    __pfnHeapfunc = (_PHEAP_m) _encode_pointer(pfn);
    return _CrtDoForAllClientObjects(CrtDoForAllClientObjects_thunk, pContext);
}

#endif  /* _DEBUG */
