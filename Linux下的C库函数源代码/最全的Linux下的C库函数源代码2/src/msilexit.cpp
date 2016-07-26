/***
*msilexit.cpp - Exit routines for mixdll.
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file provides at_exit support for clrcall functions. Here you will
*       find implementation for both managed process exit and appdomain exit.
*
*******************************************************************************/

#ifdef MRTDLL
#undef MRTDLL
#endif  /* MRTDLL */

#include <internal.h>
#include <windows.h>
#include <malloc.h>
#include <mtdll.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <gcroot.h>

#using <mscorlib.dll>
#if defined (_DEBUG)
#using <system.dll>
#endif  /* defined (_DEBUG) */

#pragma warning(disable:4439)   // C4439: function with a managed parameter must have a __clrcall calling convention
#pragma warning(disable:4483)   // disable warning/error about __identifier(<string>)

using System::AppDomain;

#if defined (_DEBUG)
    #define MANAGED_ASSERT( test, message) System::Diagnostics::Debug::Assert(test, message)
#else  /* defined (_DEBUG) */
    #define MANAGED_ASSERT( test, message )
#endif  /* defined (_DEBUG) */

/*
 * match these macros with the ones in onexit.c
 */
#define MAXINCR     512
#define MININCR     4

extern "C" {
    typedef void (__clrcall *_CPVFV)(void);
}

#if defined (_M_CEE_MIXED)
static _CPVFV *__onexitbegin  = NULL;
static _CPVFV *__onexitend = NULL;
static size_t __exit_list_size = 0;
#endif  /* defined (_M_CEE_MIXED) */

namespace __identifier("<CrtImplementationDetails>")
{
using System::Threading::Monitor;
using System::Object;
using System::Runtime::InteropServices::GCHandle;
using System::IntPtr;

// Note the major differences between gcroot and Handle:
// 1. Handle does not provide any constructors
//    Call the Construct method to initialize.
// 2. Handle does not provide any implicit casts.
//    Use Get to obtain the underlying object.
template <class T>
class Handle
{
private:
        void* _data;

        [System::Diagnostics::DebuggerStepThroughAttribute]
        GCHandle^ _handle()
        {
                if (_data != nullptr)
                {
                        return GCHandle::FromIntPtr(IntPtr(_data));
                }

                return nullptr;
        }

public:
        // Do NOT add constructors to this class
        // We want to have objects that are not initialized
        // so that we can create per-appdomain instances that are
        // used *before* per-appdomain initialization takes place.

        void Construct()
        {
                _data = 0;
        }

        [System::Diagnostics::DebuggerStepThroughAttribute]
        void Construct(T value)
        {
                _data = 0;
                Set(value);
        }

        [System::Diagnostics::DebuggerStepThroughAttribute]
        void Set(T value)
        {
                GCHandle^ handle = _handle();
                if (handle == nullptr)
                {
                        handle = GCHandle::Alloc(value);
                        _data = GCHandle::ToIntPtr(*handle).ToPointer();
                }
                else
                {
                        handle->Target = value;
                }
        }

        [System::Diagnostics::DebuggerStepThroughAttribute]
        T Get()
        {
                GCHandle^ handle = _handle();
                if (handle != nullptr)
                {
                        return (T)handle->Target;
                }
                return nullptr;
        }

        ~Handle()
        {
                GCHandle^ handle = _handle();
                if (handle != nullptr)
                {
                        handle->Free();
                        _data = 0;
                }
        }
};

class AtExitLock
{
private:
    _AGLOBAL static Handle<Object^> _lock;

public:
    [System::Diagnostics::DebuggerStepThroughAttribute]
    static void Initialize()
    {
        _lock.Construct(gcnew Object());
    }

    [System::Diagnostics::DebuggerStepThroughAttribute]
    static bool IsInitialized()
    {
        return (_lock.Get() != nullptr);
    }

    [System::Diagnostics::DebuggerStepThroughAttribute]
    static void Enter()
    {
        Monitor::Enter(_lock.Get());
    }

    [System::Diagnostics::DebuggerStepThroughAttribute]
    static void Exit()
    {
        Monitor::Exit(_lock.Get());
    }
};
_AGLOBAL Handle<Object^> AtExitLock::_lock; /* Do NOT initialize */
}

/***
* __global_lock() - lock access for __global_lock_object
*
* Purpose:
*       This funciton is used by atexit funcitons to avoid parllel
*       access to critical data.
*
* Entry:
*
* Exit:
*       returns true if sucessful.
*
*******************************************************************************/

[System::Diagnostics::DebuggerStepThroughAttribute]
static bool __global_lock()
{
    using namespace __identifier("<CrtImplementationDetails>");

    bool retval = false;
    if (AtExitLock::IsInitialized())
    {
        AtExitLock::Enter();
        retval = true;
    }
    return retval;
}

/***
* __global_unlock() - releases the lock held by calling __global_lock
*
* Purpose:
*       Release the lock held by calling __global_lock.
*
* Entry:
*
* Exit:
*       returns true if sucessful.
*
*******************************************************************************/

[System::Diagnostics::DebuggerStepThroughAttribute]
static bool __global_unlock()
{
    using namespace __identifier("<CrtImplementationDetails>");

    bool retval = false;
    if (AtExitLock::IsInitialized())
    {
        AtExitLock::Exit();
        retval = true;
    }
    return retval;
}

/***
* __alloc_global_lock() - allocates CLR lock for access to lockClass::__global_lock_object
*
* Purpose:
*       This lock object is used by atexit routines to have single threaded
*       access to critical data.
*
* Entry:
*
* Exit:
*       return true if sucessful.
*
*******************************************************************************/

[System::Diagnostics::DebuggerStepThroughAttribute]
static bool __alloc_global_lock()
{
    using namespace __identifier("<CrtImplementationDetails>");

    if (!AtExitLock::IsInitialized())
    {
        AtExitLock::Initialize();
    }
    return AtExitLock::IsInitialized();
}

/***
* _atexit_helper(). registers function to be called during process exit.
*
* Purpose:
*       This function registers __clrcall function to be called at processexit.
*       The callback are in LIFO, i.e. last call registered will be first one
*       to be called at the time of exit.
*
* Entry:
*       _CPVFV,
*       size_t *,
*       _CPVFV **,
*       _CPVFV **;
*
* Exit:
*       return -1 if the the function was not registered. Else returns 0.
*
* Notes:
*       This routine assumes that _initatexit_m was called before this function
*       is called.
*
*******************************************************************************/

extern "C" int __clrcall _atexit_helper( _CPVFV func,
        size_t *__pexit_list_size,
        _CPVFV **__ponexitend,
        _CPVFV **__ponexitbegin)
{
    _CPVFV retval = NULL;

    /*
     * Get the lock for CRT exit function calls.
     */
    if (func == NULL) return -1;
    if (__global_lock() == true) {
        try {
            /*
             * check if we have space of one more entry.
             */
            if ( (*__pexit_list_size) - 1<
                    ((size_t)((char *)(*__ponexitend) - (char *)(*__ponexitbegin)))/sizeof(_CPVFV))
            {
                /*
                 * Try to increment by max increment or twice the current size, if fails,
                 * then try to increment by min increment. If that too fails then return NULL.
                 */
                try {
                    System::IntPtr tmp_ptr =
                        System::Runtime::InteropServices::Marshal::ReAllocHGlobal(
                            System::IntPtr((*__ponexitbegin)),
                            System::IntPtr((long)((*__pexit_list_size)*sizeof(_CPVFV)+
                                __min((*__pexit_list_size)*sizeof(_CPVFV),
                                    (MAXINCR*sizeof(_CPVFV))))));

                    (*__ponexitend) = (_CPVFV *)((char *)tmp_ptr.ToPointer()+
                            (size_t)(*__ponexitend)-
                            (size_t)(*__ponexitbegin));
                    (*__ponexitbegin) = (_CPVFV *)tmp_ptr.ToPointer();
                    (*__pexit_list_size) += __min(MAXINCR, (*__pexit_list_size));

                } catch (System::OutOfMemoryException ^) {

                    System::IntPtr tmp_ptr =
                        System::Runtime::InteropServices::Marshal::ReAllocHGlobal(
                            System::IntPtr((*__ponexitbegin)),
                            System::IntPtr((long)((*__pexit_list_size)*sizeof(_CPVFV)+
                                MININCR+sizeof(_CPVFV))));

                    (*__ponexitend) = (_CPVFV *)((char *)tmp_ptr.ToPointer()+
                            (size_t)(*__ponexitend)-
                            (size_t)(*__ponexitbegin));
                    (*__ponexitbegin) = (_CPVFV *)tmp_ptr.ToPointer();
                    (*__pexit_list_size) += MININCR;
                }
            }
            *((*__ponexitend)++) = (_CPVFV)func;
            retval = func;
        } catch(System::OutOfMemoryException ^) {
        } __finally{
            __global_unlock();
        }
    }
    return retval==NULL?-1:0;
}

#if defined (_M_CEE_MIXED)
/***
* _exit_callback() - this function is called by msvcrt at exit.
*
* Purpose:
*       This function is called from msvcrt dll at process exit. This
*       functio then calls all the registered __clrcall function. This
*       function acts as layer between __cdecl atexit callback and __clrcal
*       atexit callback.
*
* Entry:
*
* Exit:
*
*******************************************************************************/

extern "C" void __clrcall _exit_callback(void)
{
    MANAGED_ASSERT(AppDomain::CurrentDomain->IsDefaultAppDomain(), "This function must be called in the default domain");
    /*
     * Note: this function is not supposed to be called from more then one
     * thread at a time. Infact this is called when the program is terminating
     * or the dll is being unloaded.
     *
     * Thus we don't need to lock anything here.
     */

    /*
     * Note that this function is called during appdomain unload.
     * Thus there should be only one thread calling this function per
     * appdomain. Thus this function does not need to be under lock.
     *
     * Note: verify that when domainUnload event is generated, there
     * are no threads executing in this appdomain. If this is not the
     * case then we will need to have lock around this function.
     */

    if ((__onexitbegin != (_CPVFV *)-1) && (__onexitbegin != NULL) && (__onexitend != NULL)) {
        while ( --__onexitend >= __onexitbegin )
        {
            /*
             * if current table entry is non-NULL,
             * call thru it.
             */
            if ( *__onexitend != NULL )
                (**__onexitend)();
        }
        /*
         * There is no real need to free memory here as the process is anyway
         * going down and CRT will free it's heap and this memory will anyway
         * be freed. But just so that user doesn't see the memory leak if he
         * wants to detect leak using CRT functionality, we free this memory.
         */
        System::Runtime::InteropServices::Marshal::FreeHGlobal(
                System::IntPtr((void *)__onexitbegin));
    }
}


/***
*_initatexit_m(). Initializes the managed process exit variables.
*
* Purpose:
*       The _initatexit_m function is called from cctor of mixdll. This function
*       is responsible for initializing managed process exit variables. The
*       initialization includes allocating memory.
*
* Entry:
*
* Exit:
*       1 if initialization was sucessful 0 otherwise.
*
*******************************************************************************/

[System::Diagnostics::DebuggerStepThroughAttribute]
extern "C" int __clrcall _initatexit_m()
{
    MANAGED_ASSERT(AppDomain::CurrentDomain->IsDefaultAppDomain(), "This function must be called in the default domain");

    int retval = 0;

    /*
     * Note: This function is called only once just during cctor call.
     */
    if (__alloc_global_lock() == true) {
        System::IntPtr tmp_int_ptr =
            System::Runtime::InteropServices::Marshal::AllocHGlobal(
                    32*sizeof(*__onexitbegin));

        __onexitend = __onexitbegin = (_CPVFV *)tmp_int_ptr.ToPointer();
        __exit_list_size = 32;
        retval = 1;
    }
    return retval;
}

/***
* _atexit_m(_CPVFV). registers function to be called during process exit.
*
* Purpose:
*       This function registers __clrcall function to be called at processexit.
*       The callback are in LIFO, i.e. last call registered will be first one
*       to be called at the time of exit.
*
* Entry:
*       void (__clrcall *func)(void);
*
* Exit:
*       return -1 if the the function was not registered. Else returns 0.
*
* Notes:
*       This routine assumes that _initatexit_m was called before this function
*       is called.
*
*******************************************************************************/

extern "C" _onexit_m_t __clrcall _onexit_m( _onexit_m_t _Function)
{
    MANAGED_ASSERT(AppDomain::CurrentDomain->IsDefaultAppDomain(), "This function must be called in the default domain");

    return _atexit_m((_CPVFV)_Function) == -1 ? NULL : _Function;
}

extern "C" int __clrcall _atexit_m( _CPVFV func)
{
    MANAGED_ASSERT(AppDomain::CurrentDomain->IsDefaultAppDomain(), "This function must be called in the default domain");

    return _atexit_helper(func, &__exit_list_size, &__onexitend, &__onexitbegin);
}
#endif  /* defined (_M_CEE_MIXED) */

/***
*
* AppDomain Exit stuff.
*
*******************************************************************************/

/*
 * Note that the initialization values are commented out.
 * The reason here is that these variables are appdomain specific. And if
 * we try to initialize them, unlike process variables, they will be
 * dynamically initialized. This means they will be initialized by our
 * initialization routine BUT we need to initialize them before we run
 * our initialization code.
 *
 * Not initializating them here is also OK just because we can always
 * intialize them in _initatexit_app_domain function
 */

_AGLOBAL _CPVFV *__onexitbegin_app_domain /*= NULL*/;
_AGLOBAL _CPVFV *__onexitend_app_domain /*= NULL*/;
_AGLOBAL size_t __exit_list_size_app_domain /*= 0*/;


/***
*_initatexit_app_domain(). Initializes the managed per appdomain exit variables.
*
* Purpose:
*       The _initatexit_app_domain function is called from cctor of mixdll. This
*       function is responsible for initializing managed per appdomain exit
*       variables. The initialization includes allocating memory.
*
* Entry:
*
* Exit:
*       1 if initialization was sucessful 0 otherwise.
*
*******************************************************************************/

[System::Diagnostics::DebuggerStepThroughAttribute]
extern "C" int __clrcall _initatexit_app_domain()
{
    if (__alloc_global_lock() == true) {
        /*
         * Note that this function is called from the cctor during the
         * appdomain initialization. Thus there is going to be only one thread
         * per process accessing this function.
         *
         * Thus this function does not need to be under the lock.
         */

        System::IntPtr tmp_int_ptr =
            System::Runtime::InteropServices::Marshal::AllocHGlobal(
                    32*sizeof(*__onexitbegin_app_domain));

        __onexitend_app_domain = __onexitbegin_app_domain = (_CPVFV *)tmp_int_ptr.ToPointer();
        __exit_list_size_app_domain = 32;
#if defined (_M_CEE_PURE)
        extern void __clrcall __clean_type_info_names();

        _atexit_m_appdomain(__clean_type_info_names);
#endif  /* defined (_M_CEE_PURE) */
    }
    return 1;
}

/***
* _app_exit_callback() - this function is called by cctor during appdomain exit.
*
* Purpose:
*       This function is called from cctor during appdomain exit. This
*       function then calls all the registered __clrcall function. This
*       function acts as appdomain version of _exit_callback.
*
* Entry:
*
* Exit:
*
*******************************************************************************/

extern "C" void __clrcall _app_exit_callback(void)
{
    /*
     * Note that this function is called during appdomain unload.
     * Thus there should be only one thread calling this function per
     * appdomain. Thus this function does not need to be under lock.
     *
     * Note: verify that when domainUnload event is generated, there
     * are no threads executing in this appdomain. If this is not the
     * case then we will need to have lock around this function.
     */
    if ((__onexitbegin_app_domain != (_CPVFV *)-1) && (__onexitbegin_app_domain != NULL) && (__onexitend_app_domain != NULL)) {
        __try
        {
            while (--__onexitend_app_domain >= __onexitbegin_app_domain) {
                if (*__onexitend_app_domain != NULL) {
                    (**__onexitend_app_domain)();
                }
            }
        }
        __finally
        {
            System::Runtime::InteropServices::Marshal::FreeHGlobal(
                    System::IntPtr((void *)__onexitbegin_app_domain));
        }
    }
}

/***
* _atexit_app_domain(_CPVFV). registers function to be called during appdomain exit.
*
* Purpose:
*       This function registers __clrcall function to be called at appdomain exit.
*       The callback are in LIFO, i.e. last call registered will be first one
*       to be called at the time of exit. This is appdomain version of _atexit_m()
*
* Entry:
*       void (__clrcall *func)(void);
*
* Exit:
*       return -1 if the the function was not registered. Else returns 0.
*
* Notes:
*       This routine assumes that _initatexit_app_domain was called before this
*       function is called.
*
*******************************************************************************/

extern "C" _onexit_m_t __clrcall _onexit_m_appdomain(_onexit_m_t _Function)
{
    return _atexit_m_appdomain((_CPVFV)_Function) == -1 ? NULL : _Function;
}

[System::Diagnostics::DebuggerStepThroughAttribute]
extern "C" int __clrcall _atexit_m_appdomain(_CPVFV func)
{
    return _atexit_helper(func, &__exit_list_size_app_domain, &__onexitend_app_domain, &__onexitbegin_app_domain);
}
