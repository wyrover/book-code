/***
*dbghook.c - Debug CRT Hook Functions
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Allow users to override default alloc hook at link time.
*
*******************************************************************************/

#include <dbgint.h>

#ifdef _DEBUG

#include <internal.h>
#include <limits.h>
#include <mtdll.h>
#include <malloc.h>
#include <stdlib.h>

_CRT_ALLOC_HOOK _pfnAllocHook = _CrtDefaultAllocHook;

/***
*int _CrtDefaultAllocHook() - allow allocation
*
*Purpose:
*       allow allocation
*
*Entry:
*       all parameters ignored
*
*Exit:
*       returns TRUE
*
*Exceptions:
*
*******************************************************************************/
int __cdecl _CrtDefaultAllocHook(
        int nAllocType,
        void * pvData,
        size_t nSize,
        int nBlockUse,
        long lRequest,
        const unsigned char * szFileName,
        int nLine
        )
{
        return 1; /* allow all allocs/reallocs/frees */
}

#endif  /* _DEBUG */

int _debugger_hook_dummy;

#ifdef _M_IA64
#undef _CRT_DEBUGGER_HOOK
#define _CRT_DEBUGGER_HOOK __crt_debugger_hook
#endif  /* _M_IA64 */

__declspec(noinline)
void __cdecl _CRT_DEBUGGER_HOOK(int _Reserved)
{
    /* assign 0 to _debugger_hook_dummy so that the function is not folded in retail */
    (_Reserved);
    _debugger_hook_dummy = 0;
}
