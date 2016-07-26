/***
*minternal.h - contains declarations of managed internal routines and variables
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Declares routines and variables used internally by the C run-time.
*
*       [Internal]
*
****/

//
//  _CPP_MSTARTUP indicates that the file is included from mstartup.cpp
//      _PARTIAL_TRUST_OBJ indicates that ptrust*.obj is being built
// !_CPP_MSTARTUP building msvcm*
//

#pragma once

#if defined (_PARTIAL_TRUST_OBJ) || !defined (_CPP_MSTARTUP)

#include <unknwn.h>
// Don't pull have IID defns in the obj
#ifdef EXTERN_GUID
#undef EXTERN_GUID
#define EXTERN_GUID(itf,l1,s1,s2,c1,c2,c3,c4,c5,c6,c7,c8) EXTERN_C const IID itf
#endif  /* EXTERN_GUID */
#include <mscoree.h>

#if defined (_DEBUG)
#using <system.dll>
#endif  /* defined (_DEBUG) */

#endif  /* defined (_PARTIAL_TRUST_OBJ) || !defined (_CPP_MSTARTUP) */

#define _INC_MINTERNAL

#pragma warning(disable:4439)   // C4439: function with a managed parameter must have a __clrcall calling convention
#pragma warning(disable:4483)   // disable warning/error about __identifier(<string>)

namespace __identifier("<CrtImplementationDetails>")
{
    using System::AppDomain;
    using System::Object;
    using System::String;
    using System::EventHandler;
    using System::EventArgs;
    using System::Threading::Interlocked;
    using System::SerializableAttribute;
    using System::CrossAppDomainDelegate;
    using System::Collections::Stack;
    using System::Threading::Interlocked;
    using System::Runtime::Serialization::SerializationInfo;
    using System::Runtime::Serialization::StreamingContext;

#ifdef _CPP_MSTARTUP
#ifndef _PARTIAL_TRUST_OBJ
    // Declarations for MSVCM80.dll exports
#if defined (_M_CEE_MIXED)
        typedef HRESULT (__stdcall* MixedFunctionType)(void*);
        _MRTIMP void __cdecl DoCallBackInDefaultDomain(MixedFunctionType function, void* cookie);
#endif  /* defined (_M_CEE_MIXED) */
    _MRTIMP void __cdecl DoDllLanguageSupportValidation();
    _MRTIMP void __cdecl RegisterModuleUninitializer(EventHandler^ handler);
    _MRTIMP void __cdecl ThrowModuleLoadException(String^ errorMessage);
    _MRTIMP void __cdecl ThrowModuleLoadException(String^ errorMessage, System::Exception^ innerException);
    _MRTIMP void __cdecl ThrowNestedModuleLoadException(System::Exception^ innerException, System::Exception^ nestedException);
#else  /* _PARTIAL_TRUST_OBJ */
#if defined (_M_CEE_MIXED)
        typedef HRESULT (__stdcall* MixedFunctionType)(void*);
        void __cdecl DoCallBackInDefaultDomain(MixedFunctionType function, void* cookie);
#endif  /* defined (_M_CEE_MIXED) */
    void RegisterModuleUninitializer(EventHandler^ handler);
    void ThrowModuleLoadException(String^ errorMessage);
    void ThrowModuleLoadException(String^ errorMessage, System::Exception^ innerException);
    void ThrowNestedModuleLoadException(System::Exception^ innerException, System::Exception^ nestedException);
#endif  /* _PARTIAL_TRUST_OBJ */
#else  /* _CPP_MSTARTUP */
    typedef HRESULT (__stdcall* MixedFunctionType)(void*);
#endif  /* _CPP_MSTARTUP */

#if defined (_DEBUG)
    #define MANAGED_ASSERT( test, message) System::Diagnostics::Debug::Assert(test, message)
#else  /* defined (_DEBUG) */
    #define MANAGED_ASSERT( test, message )
#endif  /* defined (_DEBUG) */

}   // namespace __identifier("<CrtImplementationDetails>")

///////////////////////////////////////////////////////////////////////////////
//
// building msvcm or ptrust*.obj
//

#if defined (_PARTIAL_TRUST_OBJ) || !defined (_CPP_MSTARTUP)

namespace __identifier("<CrtImplementationDetails>")
{

///////////////////////////////////////////////////////////////////////////////
//
// Exception
//

// This is the base class for all managed exceptions thrown by the CRT
[System::SerializableAttribute]
ref class Exception : public System::Exception
{
public:
    Exception(System::String^ message)
        : System::Exception(message)
    {
    }

    Exception(System::String^ message, System::Exception^ innerException)
        : System::Exception(message, innerException)
    {
    }
protected:
    Exception(SerializationInfo^ info, StreamingContext context)
        : System::Exception(info, context)
    {
    }
};

// This is thrown by the CRT during module construction (errors thrown
// by the cctor)
[System::SerializableAttribute]
ref class ModuleLoadException : public System::Exception
{
public:
    ModuleLoadException(System::String^ message)
        : Exception(message)
    {
    }

    ModuleLoadException(System::String^ message, System::Exception^ innerException)
        : Exception(message, innerException)
    {
    }

    literal System::String^ Nested = "A nested exception occurred after the primary exception that caused the C++ module to fail to load.\n";

protected:
    ModuleLoadException(SerializationInfo^ info, StreamingContext context)
        : Exception(info, context)
    {
    }
};

// This is thrown when an exception occurs in the cleanup code executed while handling
// an exception thrown by the module constructor.
[System::SerializableAttribute]
ref class ModuleLoadExceptionHandlerException : public ModuleLoadException
{
private:
    typedef ModuleLoadException base;
    literal System::String^ formatString = "\n{0}: {1}\n--- Start of primary exception ---\n{2}\n--- End of primary exception ---\n\n--- Start of nested exception ---\n{3}\n--- End of nested exception ---\n";

public:

    ModuleLoadExceptionHandlerException(System::String^ message, System::Exception^ innerException, System::Exception^ nestedException)
        : base(message, innerException)
    {
        NestedException = nestedException;
    }

    property System::Exception^ NestedException;

    virtual System::String^ ToString() override
    {
        System::String^ inner = InnerException ? InnerException->ToString() : System::String::Empty;
        System::String^ nested = NestedException ? NestedException->ToString() : System::String::Empty;
        System::String^ result = System::String::Format(
            formatString,
            this->GetType(),
            Message ? Message : System::String::Empty,
            inner ? inner : System::String::Empty,
            nested ? nested : System::String::Empty
            );
        return result;
    }

    virtual void GetObjectData(SerializationInfo^ info, StreamingContext context) override
    {
        base::GetObjectData(info, context);
        info->AddValue("NestedException", NestedException, System::Exception::typeid);
    }

protected:
    ModuleLoadExceptionHandlerException(SerializationInfo^ info, StreamingContext context)
        : base(info, context)
    {
        NestedException = (System::Exception^)info->GetValue("NestedException", System::Exception::typeid);
    }

};

#ifdef _PARTIAL_TRUST_OBJ
inline void ThrowNestedModuleLoadException(System::Exception^ innerException, System::Exception^ nestedException)
#else  /* _PARTIAL_TRUST_OBJ */
_MRTIMP void __cdecl ThrowNestedModuleLoadException(System::Exception^ innerException, System::Exception^ nestedException)
#endif  /* _PARTIAL_TRUST_OBJ */
{
    throw gcnew ModuleLoadExceptionHandlerException(ModuleLoadException::Nested, innerException, nestedException);
}

#ifdef _PARTIAL_TRUST_OBJ
inline void ThrowModuleLoadException(String^ errorMessage)
#else  /* _PARTIAL_TRUST_OBJ */
_MRTIMP void __cdecl ThrowModuleLoadException(String^ errorMessage)
#endif  /* _PARTIAL_TRUST_OBJ */
{
    throw gcnew ModuleLoadException(errorMessage);
}

#ifdef _PARTIAL_TRUST_OBJ
inline void ThrowModuleLoadException(String^ errorMessage, System::Exception^ innerException)
#else  /* _PARTIAL_TRUST_OBJ */
_MRTIMP void __cdecl ThrowModuleLoadException(String^ errorMessage, System::Exception^ innerException)
#endif  /* _PARTIAL_TRUST_OBJ */
{
    throw gcnew ModuleLoadException(errorMessage, innerException);
}

///////////////////////////////////////////////////////////////////////////////
//
// UnInitialize
//

/*
// A single instance of this class is stored in each AppDomain
// It registers a handler for the AppDomain::DomainUnload and AppDomain::ProcessExit events
// and maintains a list of managed C++ modules loaded in that domain. When an unload
// event occurs, it calls back on the event handler registered by each of the modules
// in the reverse order to which they were added to the list.
*/
ref class ModuleUninitializer : public Stack
{
private:
    static Object^ lock = gcnew Object();

internal:
        // Static members are AppDomain specific.
        // Instance for a new AppDomain is created when msvcm's .cctor tries to add its DomainUnload handler
    static ModuleUninitializer^ _ModuleUninitializer = gcnew ModuleUninitializer();
    void AddHandler(EventHandler^ handler)
    {
#ifndef _PARTIAL_TRUST_OBJ
        bool mustReleaseLock = false;
        System::Runtime::CompilerServices::RuntimeHelpers::PrepareConstrainedRegions();
        try
        {
            System::Runtime::CompilerServices::RuntimeHelpers::PrepareConstrainedRegions();
            try { } finally
            {
                System::Threading::Monitor::Enter(lock);
                mustReleaseLock = true;
            }
#endif  /* _PARTIAL_TRUST_OBJ */
        System::Runtime::CompilerServices::RuntimeHelpers::PrepareDelegate(handler);
        Push(handler);
#ifndef _PARTIAL_TRUST_OBJ
        }
        finally
        {
            if (mustReleaseLock)
            {
                System::Threading::Monitor::Exit(lock);
            }
        }
#endif  /* _PARTIAL_TRUST_OBJ */
    }

private:
    ModuleUninitializer()
    {
        // Protected by lock in Initialize
        // singletonHandler will be Prepared as a CER call when it's added to the DomainUnload event:
        // this makes it a CER root
        EventHandler^ singletonHandler = gcnew EventHandler(this, &ModuleUninitializer::SingletonDomainUnload);
        AppDomain::CurrentDomain->DomainUnload += singletonHandler;
        AppDomain::CurrentDomain->ProcessExit  += singletonHandler;
    }

    [System::Runtime::ConstrainedExecution::PrePrepareMethod]
    void SingletonDomainUnload(Object^ source, EventArgs^ arguments)
    {
#ifndef _PARTIAL_TRUST_OBJ
        bool mustReleaseLock = false;
        System::Runtime::CompilerServices::RuntimeHelpers::PrepareConstrainedRegions();
        try
        {
            System::Runtime::CompilerServices::RuntimeHelpers::PrepareConstrainedRegions();
            try { } finally
            {
                System::Threading::Monitor::Enter(lock);
                mustReleaseLock = true;
            }
#endif  /* _PARTIAL_TRUST_OBJ */
        // Stack iterator starts at top and moves down through stack
        for each (EventHandler^ handler in this)
        {
            handler(source, arguments);
        }
#ifndef _PARTIAL_TRUST_OBJ
        }
        finally
        {
            if (mustReleaseLock)
            {
                System::Threading::Monitor::Exit(lock);
            }
        }
#endif  /* _PARTIAL_TRUST_OBJ */
    }
};

#ifdef _PARTIAL_TRUST_OBJ
inline void RegisterModuleUninitializer(EventHandler^ handler)
#else  /* _PARTIAL_TRUST_OBJ */
_MRTIMP void __cdecl RegisterModuleUninitializer(EventHandler^ handler)
#endif  /* _PARTIAL_TRUST_OBJ */
{
    ModuleUninitializer::_ModuleUninitializer->AddHandler(handler);
}

}   // namespace __identifier("<CrtImplementationDetails>")

#if defined (_M_CEE_MIXED)

#ifdef _PARTIAL_TRUST_OBJ
inline HRESULT __get_default_appdomain(IUnknown **ppUnk)
#else  /* _PARTIAL_TRUST_OBJ */
_MRTIMP HRESULT __cdecl __get_default_appdomain(IUnknown **ppUnk)
#endif  /* _PARTIAL_TRUST_OBJ */

{
    /*
     * Don't do anything in case we are already in default appdomain.
     */
    HRESULT hresult = 0;
    IUnknown *pUnk1 = NULL;
    ICorRuntimeHost *pHost = NULL;

    __try {
        /*
         * Note: We don't initialize COM as COM is already initialized as CLR is already
         * initialized.
         *
         * Initialize CLR, rather get CLSID_CorRuntimeHost Interface pointer.
         */
        if (   !FAILED(hresult = CoCreateInstance(__uuidof(CorRuntimeHost), NULL, CLSCTX_INPROC_SERVER, __uuidof(IUnknown), (void **)&pUnk1))
            && !FAILED(hresult = pUnk1->QueryInterface(__uuidof(ICorRuntimeHost), (void **)&pHost)))
            hresult = pHost->GetDefaultDomain(ppUnk);
    } __finally {
        if (pUnk1) pUnk1->Release();
        if (pHost) pHost->Release();
    }

    return hresult;
}

#ifdef _PARTIAL_TRUST_OBJ
inline void __release_appdomain(IUnknown *ppUnk)
#else  /* _PARTIAL_TRUST_OBJ */
_MRTIMP void __cdecl __release_appdomain(IUnknown *ppUnk)
#endif  /* _PARTIAL_TRUST_OBJ */
{
    ppUnk->Release();
}

namespace __identifier("<CrtImplementationDetails>")
{

inline AppDomain^ GetDefaultDomain()
{
    using System::Runtime::InteropServices::Marshal;
    IUnknown* pUnk = NULL;
    HRESULT hresult = __get_default_appdomain(&pUnk);
    if (SUCCEEDED(hresult))
    {
        try
        {
            Object^ pObject = Marshal::GetObjectForIUnknown(System::IntPtr(pUnk));

            AppDomain^ pDomain = (AppDomain^)pObject;

            MANAGED_ASSERT(pDomain->IsDefaultAppDomain(), "Expecting default appdomain");

            return pDomain;
        }
        finally
        {
            __release_appdomain(pUnk);
        }
    }
    else
    {
        Marshal::ThrowExceptionForHR(hresult);
    }

    return nullptr;
}

#ifdef _PARTIAL_TRUST_OBJ

///////////////////////////////////////////////////////////////////////////////
//
// building ptmstartup.obj
//

inline void DoCallBackInDefaultDomain(MixedFunctionType function, void* cookie)

#else  /* _PARTIAL_TRUST_OBJ */

///////////////////////////////////////////////////////////////////////////////
//
// building msvcm
//

_MRTIMP void __cdecl DoCallBackInDefaultDomain(MixedFunctionType function, void* cookie)

#endif  /* _PARTIAL_TRUST_OBJ */
{
    using System::Runtime::InteropServices::Marshal;
    ICLRRuntimeHost* pClrHost = nullptr;
    try
    {
        HRESULT hr = CorBindToRuntimeEx(
                NULL,                                 // version of the runtime to request
                NULL,                                 // flavor of the runtime to request
                0,                                    // runtime startup flags
                __uuidof(CLRRuntimeHost),                 // clsid of ICLRRuntimeHost
                __uuidof(ICLRRuntimeHost),                  // IID of ICLRRuntimeHost
                reinterpret_cast<void**>(&pClrHost)); // a pointer to our punk that we get back
        if (FAILED(hr))
        {
            Marshal::ThrowExceptionForHR(hr);
        }
        AppDomain^ defaultDomain = GetDefaultDomain();

        hr = pClrHost->ExecuteInAppDomain(defaultDomain->Id, function, cookie);
        if (FAILED(hr))
        {
            Marshal::ThrowExceptionForHR(hr);
        }
    }
    finally
    {
        if (pClrHost != nullptr)
        {
            pClrHost->Release();
        }
    }
}

[System::SerializableAttribute]
ref class OpenMPWithMultipleAppdomainsException
    : public System::Exception
{
public:
    OpenMPWithMultipleAppdomainsException() { }
protected:
    OpenMPWithMultipleAppdomainsException(SerializationInfo^ info, StreamingContext context)
        : System::Exception(info, context)
    {
    }
};

// Returns true if the current application is using OpenMP
inline bool IsUsingOpenMP()
{
    HINSTANCE hMod = GetModuleHandle("vcomp.dll");

    if ( hMod )
    {
        void * fnp = GetProcAddress(hMod, "_vcomp_fork");
        if ( fnp )
        {
            return true;
        }
    }

    hMod = GetModuleHandle("vcompd.dll");
    if ( hMod )
    {
        void * fnp = GetProcAddress(hMod, "_vcomp_fork");
        if ( fnp )
        {
            return true;
        }
    }

    return false;
}

#ifdef _PARTIAL_TRUST_OBJ
inline void __cdecl DoDllLanguageSupportValidation()
#else  /* _PARTIAL_TRUST_OBJ */
_MRTIMP void __cdecl DoDllLanguageSupportValidation()
#endif  /* _PARTIAL_TRUST_OBJ */
{
    if (!AppDomain::CurrentDomain->IsDefaultAppDomain() && IsUsingOpenMP())
    {
        throw gcnew OpenMPWithMultipleAppdomainsException();
    }
}

}   // namespace __identifier("<CrtImplementationDetails>")

#endif  /* defined (_M_CEE_MIXED) */

#endif  /* defined (_PARTIAL_TRUST_OBJ) || !defined (_CPP_MSTARTUP) */
