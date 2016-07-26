/***
*tlsdtor.c - Thread Local Storage dynamic destruction run-time support module
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Handle tear-down of any destructable __declspec(thread) variables
*       when a thread exits.  For destructable __declspec(thread) variables,
*       the compiler will generate a callback run by __dyn_tls_init, and this
*       callback will register a second callback with __tlregdtor, which
*       is the __declspec(thread) equivalent of atexit().  Upon
*       DLL_THREAD_DETACH or DLL_PROCESS_DETACH, walk the list of registered
*       callback to call the destructors.
*
****/

#ifdef _MSC_VER

#ifndef MRTDLL

#ifdef CRTDLL
#undef CRTDLL
#endif  /* CRTDLL */

#include <cruntime.h>
#include <internal.h>
#include <sect_attribs.h>
#include <windows.h>
#include <malloc.h>
#include <dbgint.h>

#define FUNCS_PER_NODE  30

typedef struct TlsDtorNode {
    int count;
    struct TlsDtorNode *next;
    _PVFV funcs[FUNCS_PER_NODE];
} TlsDtorNode;

static __declspec(thread) TlsDtorNode *dtor_list;
static __declspec(thread) TlsDtorNode dtor_list_head;

/*
 * __tlregdtor - register a destructor for a __declspec(thread) variable
 *
 * Purpose:
 *      This is a compiler helper called by the compiler-generated initializer
 *      for a destructable __declspec(thread) variable.  It registers a
 *      function pointer to be called when a thread exits to destroy a specific
 *      thread instance of a __declspec(thread) variable.
 *
 * Entry:
 *      func - pointer to a function returning void and taking no arguments
 *
 * Exit:
 *      Returns non-zero on memory allocation failure, though the compiler
 *      generated code will ignore the error result.  Note that the first
 *      batch of pointers doesn't require heap memory allocation.
 */

int __cdecl __tlregdtor(
    _PVFV func
    )
{
    if (dtor_list == NULL) {
        dtor_list = &dtor_list_head;
        dtor_list_head.count = 0;
    }
    else if (dtor_list->count == FUNCS_PER_NODE) {
        TlsDtorNode *pnode = (TlsDtorNode *)_malloc_crt(sizeof(TlsDtorNode));
        if (pnode == NULL) {
            return -1;
        }
        pnode->count = 0;
        pnode->next = dtor_list;
        dtor_list = pnode;
        /* this helps prefast make sure dtor_list->count is 0 */
        dtor_list->count = 0;
    }
    dtor_list->funcs[dtor_list->count++] = func;
    return 0;
}

/*
 * __dyn_tls_init - handle destruction of __declspec(thread) variables
 *
 * Purpose:
 *      When a thread exits, call each of the function pointers registered
 *      with __tlregdtor.
 *
 * Entry:
 *      This is called directly from the Windows loader code, with dwReason
 *      equal to one of DLL_PROCESS_ATTACH/DETACH or DLL_THREAD_ATTACH_DETACH.
 *
 * Exit:
 *      Returns TRUE always, though the loader code ignores the result.
 *
 * Notes:
 *      Call the registered destructor callbacks upon DLL_THREAD_DETACH (when
 *      a thread exits but the process remains) or DLL_PROCESS_DETACH (when
 *      the process terminates).  Note that we won't get a callback on any
 *      threads still alive at process termination except for the thread which
 *      actually called ExitProcess, so we can't guarantee destruction of all
 *      destructable __declspec(thread) variables.
 */

static BOOL WINAPI __dyn_tls_dtor(
    HANDLE  hDllHandle,
    DWORD   dwReason,
    LPVOID  lpreserved
    )
{
    TlsDtorNode *pnode, *pnext;
    int i;

    if (dwReason != DLL_THREAD_DETACH && dwReason != DLL_PROCESS_DETACH) {
        return TRUE;
    }

    for (pnode = dtor_list; pnode != NULL; pnode = pnext) {
        for (i = pnode->count - 1; i >= 0; --i) {
            if (pnode->funcs[i] != NULL) {
                (*pnode->funcs[i])();
            }
        }
        /*
         * Free every TlsDtorNode except the original one, which is statically
         * allocated.
         */
        pnext = pnode->next;
        if (pnext != NULL) {
            _free_crt((void *)pnode);
        }
    }

    return TRUE;
}

/*
 * Enter a callback function pointer into the .CRT$XL* array, which is the
 * callback array pointed to by the IMAGE_TLS_DIRECTORY in the PE header, so
 * the OS knows we want to be notified on each thread startup/shutdown.
 */

static _CRTALLOC(".CRT$XLD") PIMAGE_TLS_CALLBACK __xl_d = __dyn_tls_dtor;

#endif  /* MRTDLL */

#endif  /* _MSC_VER */
