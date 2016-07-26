/***
*crtexe.c - Initialization for console EXE using CRT DLL
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This is the actual startup routine for apps linking to the CRT DLL.
*       It calls the user's main routine [w]main() or [w]WinMain after
*       performing C Run-Time Library initialization.
*
*       With ifdefs, this source file also provides the source code for:
*       wcrtexe.c   the startup routine for console apps with wide chars
*       crtexew.c   the startup routine for Windows apps
*       wcrtexew.c  the startup routine for Windows apps with wide chars
*
*******************************************************************************/

#define _CPP_MSTARTUP

#using <mscorlib.dll>
#if defined (_DEBUG)
#using <system.dll>
#endif  /* defined (_DEBUG) */
/*
 * SPECIAL BUILD MACROS! Note that crtexe.c (and crtexew.c) is linked in with
 * the client's code. It does not go into crtdll.dll! Therefore, it must be
 * built under the _DLL switch (like user code) and CRTDLL must be undefined.
 * The symbol SPECIAL_CRTEXE is turned on to suppress the normal CRT DLL
 * definition of _fmode and _commode using __declspec(dllexport).  Otherwise
 * this module would not be able to refer to both the local and DLL versions
 * of these two variables.
 */

#undef MRTDLL

#define SPECIAL_CRTEXE

#include <crtdefs.h>
#include <cruntime.h>

#if defined (_M_CEE_MIXED)

/* Some inline functions (like NtCurrentTeb) need to be unmanaged */
#pragma managed(push, off)
#include <oscalls.h>
#pragma managed(pop)

#else  /* defined (_M_CEE_MIXED) */

#include <oscalls.h>

#endif  /* defined (_M_CEE_MIXED) */

#include <internal.h>
#include <process.h>
#include <math.h>
#include <sect_attribs.h>
#include <rterr.h>
#include <conio.h>
#include <gcroot.h>
#include <vcclr.h>
#include <eh.h>
#include <time.h>

#pragma warning(disable:4483)   // disable warning/error about __identifier(<string>)

extern "C" {

// Note that managed items held in these arrays are really metadata tokens
// not clrcall function pointers so it's OK for these to be _PGLOBAL.

#if defined (_M_CEE_MIXED)

#pragma const_seg(".CRTMP$XCA")
_PGLOBAL const _CRTALLOC(".CRTMP$XCA") _PVFVM __xc_mp_a[1] = { NULL };

#pragma const_seg(".CRTMP$XCZ")
_PGLOBAL const _CRTALLOC(".CRTMP$XCZ") _PVFVM __xc_mp_z[1] = { NULL };

#endif  /* defined (_M_CEE_MIXED) */

#pragma const_seg(".CRTMA$XCA")
_PGLOBAL const _CRTALLOC(".CRTMA$XCA") _PVFVM __xc_ma_a[1] = { NULL };

#pragma const_seg(".CRTMA$XCZ")
_PGLOBAL const _CRTALLOC(".CRTMA$XCZ") _PVFVM __xc_ma_z[1] = { NULL };

#pragma const_seg(".CRTVT$XCA")
_PGLOBAL const _CRTALLOC(".CRTVT$XCA") _PVFVM __xi_vt_a[1] = { NULL };

#pragma const_seg(".CRTVT$XCZ")
_PGLOBAL const _CRTALLOC(".CRTVT$XCZ") _PVFVM __xi_vt_z[1] = { NULL };

#pragma const_seg()  /* reset */

#pragma comment(linker, "/merge:.CRTMP=.rdata")
#pragma comment(linker, "/merge:.CRTMA=.rdata")
#pragma comment(linker, "/merge:.CRTVT=.rdata")

#pragma comment(linker, "/defaultlib:kernel32.lib")

#if defined (_M_CEE_MIXED)
extern "C" _PGLOBAL _CRTALLOC(".CRT$XIA") _PIFV __xi_a[];
extern "C" _PGLOBAL _CRTALLOC(".CRT$XIZ") _PIFV __xi_z[];    /* C initializers */
extern "C" _PGLOBAL _CRTALLOC(".CRT$XCA") _PVFV __xc_a[];
extern "C" _PGLOBAL _CRTALLOC(".CRT$XCZ") _PVFV __xc_z[];    /* C++ initializers */
#endif  /* defined (_M_CEE_MIXED) */
}

#if defined (_M_CEE_MIXED)
extern "C" int __clrcall _initatexit_m();
extern "C" void __clrcall _exit_callback(void);
#endif  /* defined (_M_CEE_MIXED) */
extern "C" int __clrcall _initatexit_app_domain();
extern "C" void __clrcall _app_exit_callback();

int __clrcall _initterm_e ( _PIFV *, _PIFV *);
void __clrcall _initterm ( _PVFV *, _PVFV *);
void __clrcall _initterm_m ( const _PVFVM *, const _PVFVM *);

#if defined (_M_CEE_MIXED)
#pragma managed(push, off)
[System::Diagnostics::DebuggerStepThroughAttribute]
void * __cdecl _getFiberPtrId()
{
    return ((PNT_TIB)NtCurrentTeb())->StackBase;
}

extern "C"
void __cdecl __security_init_cookie(void);
#pragma managed(pop)
#endif  /* defined (_M_CEE_MIXED) */


#include <minternal.h>

namespace __identifier("<CrtImplementationDetails>")
{

class TriBool
{
public:
    enum State { False = 0, True = -1, Unknown = 2 };
private:
    TriBool();
    ~TriBool();
};

class Progress
{
public:
    enum State { NotStarted = 0, Started, Finished };
private:
    Progress();
    ~Progress();
};

#if defined (_M_CEE_MIXED)

class AllDomains
{
public:
    // The number of domains in which the module is loaded
    _PGLOBAL static int Count;
};
_PGLOBAL int AllDomains::Count = 0;

class DefaultDomain
{
private:
    _PGLOBAL static TriBool::State hasPerProcess;
    _PGLOBAL static TriBool::State hasNative;
    /*static void __cdecl DoNothing()
    {
    }*/
        static HRESULT __stdcall DoNothing(void* cookie)
    {
                cookie;
                // Execute simple managed code
                System::GC::KeepAlive(System::Int32::MaxValue);
                return S_OK;
    }

public:
    _PGLOBAL static bool Entered;
    _PGLOBAL static bool InitializedNative;
    _PGLOBAL static bool InitializedNativeFromCCTOR;
    _PGLOBAL static bool InitializedPerProcess;

    inline static bool HasPerProcess()
    {
        if (hasPerProcess == TriBool::Unknown)
        {
            void **pBeg = (void **)(void *)__xc_mp_a;
            void **pEnd = (void **)(void *)__xc_mp_z;
            while (pBeg < pEnd)
            {
                if (*pBeg != 0)
                {
                    hasPerProcess = TriBool::True;
                    return true;
                }
                pBeg++;
            }

            hasPerProcess = TriBool::False;
            return false;
        }
        return hasPerProcess == TriBool::True;
    }

    inline static bool HasNative()
    {
        if (hasNative == TriBool::Unknown)
        {
            void **pBeg = (void **)(void *)__xi_a;
            void **pEnd = (void **)(void *)__xi_z;

            while (pBeg < pEnd)
            {
                if (*pBeg != 0)
                {
                    hasNative = TriBool::True;
                    return true;
                }
                pBeg++;
            }

            pBeg = (void **)(void *)__xc_a;
            pEnd = (void **)(void *)__xc_z;
            while (pBeg < pEnd)
            {
                if (*pBeg != 0)
                {
                    hasNative = TriBool::True;
                    return true;
                }
                pBeg++;
            }

            hasNative = TriBool::False;
            return false;
        }
        return hasNative == TriBool::True;
    }

    inline static bool NeedsInitialization()
    {
        return ((HasPerProcess() && !InitializedPerProcess) || (HasNative() && !InitializedNative && (__native_startup_state == __uninitialized)));
    }

    inline static bool NeedsUninitialization()
    {
        // If the default domain has been entered then there's a chance that
        // atexit has been called and we need to transition into it to uninitialize it.
        return Entered;
    }

    inline static void Initialize()
    {
        // Trigger call to the module constructor
        DoCallBackInDefaultDomain(&DoNothing, nullptr);
    }

private:
    DefaultDomain();
    ~DefaultDomain();
};

_PGLOBAL bool DefaultDomain::Entered = false;
_PGLOBAL bool DefaultDomain::InitializedNative = false;
_PGLOBAL bool DefaultDomain::InitializedNativeFromCCTOR = false;
_PGLOBAL bool DefaultDomain::InitializedPerProcess = false;
_PGLOBAL TriBool::State DefaultDomain::hasPerProcess = TriBool::Unknown;
_PGLOBAL TriBool::State DefaultDomain::hasNative = TriBool::Unknown;

#endif  /* defined (_M_CEE_MIXED) */

class CurrentDomain
{
public:
    _AGLOBAL static int Initialized;
    _AGLOBAL static int Uninitialized;
    _AGLOBAL static bool IsDefaultDomain;
    _AGLOBAL static Progress::State InitializedVtables;
    _AGLOBAL static Progress::State InitializedNative;
    _AGLOBAL static Progress::State InitializedPerProcess;
    _AGLOBAL static Progress::State InitializedPerAppDomain;
private:
    CurrentDomain();  // Not instantiable
    ~CurrentDomain(); // Not instantiable
};

_AGLOBAL int CurrentDomain::Initialized = 0;
_AGLOBAL int CurrentDomain::Uninitialized = 0;
_AGLOBAL bool CurrentDomain::IsDefaultDomain = false;
_AGLOBAL Progress::State CurrentDomain::InitializedVtables = Progress::NotStarted;
_AGLOBAL Progress::State CurrentDomain::InitializedNative = Progress::NotStarted;
_AGLOBAL Progress::State CurrentDomain::InitializedPerProcess = Progress::NotStarted;
_AGLOBAL Progress::State CurrentDomain::InitializedPerAppDomain = Progress::NotStarted;

#ifdef GetTempFileName
#undef GetTempFileName
#endif  /* GetTempFileName */

class ActivationContext
{
    HANDLE ActivationCtxHandle;
    ULONG_PTR ActivationCtxCookie;
    gcroot<String^> ErrorMessage;

    ActivationContext( const ActivationContext& );
public :
    ActivationContext() :
        ActivationCtxHandle( INVALID_HANDLE_VALUE ),
        ActivationCtxCookie( 0 )
    {
    }

    ~ActivationContext()
    {
        Release();
    }

    void Create()
    {
        if ( ActivationCtxHandle != INVALID_HANDLE_VALUE )
        {
            return ;
        }

        HMODULE hKernel = GetModuleHandleW( L"KERNEL32" );
        if ( GetProcAddress( hKernel, "CreateActCtxW" ) == NULL )
        {
            // Pre XP OS
            return;
        }

        bool ManifestInFile = false;

        System::Reflection::Assembly^ CurrentAssembly = System::Reflection::Assembly::GetCallingAssembly();
        String^ AssemblyPath = CurrentAssembly->Location;
        try
        {
                if ( AssemblyPath == "" )
                {
                    String^ TempPath = System::IO::Path::GetTempFileName();
                    System::IO::StreamWriter^ sw = System::IO::File::CreateText(TempPath);
                    sw->Write (
                        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
                        "<assembly xmlns=\"urn:schemas-microsoft-com:asm.v1\" manifestVersion=\"1.0\">\n"
                        "   <dependency>\n"
                        "       <dependentAssembly>\n"
                        "           <assemblyIdentity\n"
                        "               type='win32'\n"
#ifdef _DEBUG
                        "               name='" __LIBRARIES_ASSEMBLY_NAME_PREFIX ".DebugCRT'\n"
#else  /* _DEBUG */
                        "               name='" __LIBRARIES_ASSEMBLY_NAME_PREFIX ".CRT'\n"
#endif  /* _DEBUG */
                        "               version='" _CRT_ASSEMBLY_VERSION "'\n"
#ifdef _M_IX86
                        "               processorArchitecture='x86'\n"
#endif  /* _M_IX86 */
#ifdef _M_AMD64
                        "               processorArchitecture='amd64'\n"
#endif  /* _M_AMD64 */
#ifdef _M_IA64
                        "               processorArchitecture='ia64'\n"
#endif  /* _M_IA64 */
                        "               publicKeyToken='" _VC_ASSEMBLY_PUBLICKEYTOKEN "'/>\n"
                        "       </dependentAssembly>\n"
                        "   </dependency>\n"
                        "</assembly>\n"
                    );
                    sw->Close();
                    ManifestInFile = true;
                    AssemblyPath = TempPath;
                }

                cli::pin_ptr<const System::Char> pAssemblyPath = PtrToStringChars(AssemblyPath);

                ACTCTXW actctx;
                // Don't call memset. memset results in a call to msvcr*.dll which can be loaded from WinSXS
                // only after the activation context is activated.
                actctx.wProcessorArchitecture = 0;
                actctx.wLangId = 0;
                actctx.lpAssemblyDirectory = NULL;
                actctx.lpApplicationName = NULL;
                actctx.hModule = NULL;

                actctx.cbSize = sizeof( actctx );
                actctx.lpSource = pAssemblyPath;
                if (ManifestInFile)
                {
                    actctx.lpResourceName = 0;
                    actctx.dwFlags = 0;
                }
                else
                {
                    actctx.lpResourceName = MAKEINTRESOURCEW( 2 );
                    actctx.dwFlags = ACTCTX_FLAG_RESOURCE_NAME_VALID;
                }

                ActivationCtxHandle = CreateActCtxW( &actctx );
                if ( ActivationCtxHandle == INVALID_HANDLE_VALUE )
                {
                    if (!ManifestInFile)
                    {
                        actctx.lpResourceName = MAKEINTRESOURCEW( 1 );
                        ActivationCtxHandle = CreateActCtxW( &actctx );
                    }
                    if ( ActivationCtxHandle == INVALID_HANDLE_VALUE )
                    {
                    }
                }
            }
            finally
            {
                if (ManifestInFile)
                {
                    System::IO::File::Delete(AssemblyPath);
                }
            }
    }

    void Activate()
    {
        if ( ActivationCtxHandle == INVALID_HANDLE_VALUE )
        {
            // Could not create activation context
            // Pre XP OS or no manifest
            return ;
        }
        if ( ActivateActCtx( ActivationCtxHandle, &ActivationCtxCookie ) != TRUE )
        {
            return ;
        }
    }

    void DeActivate()
    {
        if ( ActivationCtxHandle == INVALID_HANDLE_VALUE )
        {
            return ;
        }

        if ( ActivationCtxCookie == 0 )
        {
            // Already Deactivated
            return ;
        }
        ULONG_PTR cookie = ActivationCtxCookie;
        ActivationCtxCookie = 0;
        if ( DeactivateActCtx( 0, cookie ) == FALSE )
        {
            ErrorMessage = L"The C++ module failed to Deactivate WinSXS Activation Context.\n";
            ThrowModuleLoadException(ErrorMessage);
        }
    }

    void Release()
    {
        if ( ActivationCtxHandle == INVALID_HANDLE_VALUE )
        {
            return ;
        }
        DeActivate();
        ReleaseActCtx( ActivationCtxHandle );
        ActivationCtxHandle = INVALID_HANDLE_VALUE;
    }
};

class LanguageSupport
{
private:
    // If a non-CRT exception is thrown, this member
    // holds a constant describing the current activity
    gcroot<String^> ErrorMessage;

    // Initializes vtables (which are per-appdomain)
    [System::Diagnostics::DebuggerStepThroughAttribute]
    inline void InitializeVtables()
    {
        ErrorMessage = L"The C++ module failed to load during vtable initialization.\n";
        CurrentDomain::InitializedVtables = Progress::Started;
        _initterm_m((const _PVFVM *)__xi_vt_a, (const _PVFVM *)__xi_vt_z);
        CurrentDomain::InitializedVtables = Progress::Finished;
    }

#if defined (_M_CEE_MIXED)

    // This is a blocking call which won't return until the default app domain is initialized
    inline void InitializeDefaultAppDomain()
    {
        ErrorMessage = L"The C++ module failed to load while attempting to initialize the default appdomain.\n";
        DefaultDomain::Initialize();
    }

    // Initializes native globals
    [System::Diagnostics::DebuggerStepThroughAttribute]
    inline void InitializeNative()
    {
        MANAGED_ASSERT(CurrentDomain::IsDefaultDomain, "Native globals must be initialized in the default domain");

        ErrorMessage = L"The C++ module failed to load during native initialization.\n";

        __security_init_cookie();

        DefaultDomain::InitializedNative = true;

        using namespace _NATIVE_STARTUP_NAMESPACE;
        if (!NativeDll::IsSafeForManagedCode())
        {
            _amsg_exit( _RT_CRT_INIT_MANAGED_CONFLICT);
        }

        /*
        * In mixed code we make sure to initialize native globals as well.
        */
        if (__native_startup_state == __initializing)
        {
            _amsg_exit( _RT_CRT_INIT_MANAGED_CONFLICT);
        }
        else if (__native_startup_state == __uninitialized)
        {
            CurrentDomain::InitializedNative = Progress::Started;
            __native_startup_state = __initializing;
            if (_initterm_e(__xi_a, __xi_z) != 0)
            {
                ThrowModuleLoadException(ErrorMessage);
            }
            _initterm(__xc_a, __xc_z);
            __native_startup_state = __initialized;
            DefaultDomain::InitializedNativeFromCCTOR = true;
            CurrentDomain::InitializedNative = Progress::Finished;
        }
    }

    // Initializes per-process managed globals
    [System::Diagnostics::DebuggerStepThroughAttribute]
    inline void InitializePerProcess()
    {
        MANAGED_ASSERT(CurrentDomain::IsDefaultDomain, "Per-process globals must be initialized in the default domain");

        ErrorMessage = L"The C++ module failed to load during process initialization.\n";

        CurrentDomain::InitializedPerProcess = Progress::Started;

        _initatexit_m();

        _initterm_m((const _PVFVM *) __xc_mp_a, (const _PVFVM *)__xc_mp_z );

        CurrentDomain::InitializedPerProcess = Progress::Finished;

        DefaultDomain::InitializedPerProcess = true;
    }

#endif  /* defined (_M_CEE_MIXED) */

    // Initializes per-appdomain managed globals
    [System::Diagnostics::DebuggerStepThroughAttribute]
    inline void InitializePerAppDomain()
    {
        ErrorMessage = L"The C++ module failed to load during appdomain initialization.\n";

        CurrentDomain::InitializedPerAppDomain = Progress::Started;

        _initatexit_app_domain();

        _initterm_m( __xc_ma_a, __xc_ma_z );

        CurrentDomain::InitializedPerAppDomain = Progress::Finished;
    }

    // Registers for appdomain unload event
    [System::Diagnostics::DebuggerStepThroughAttribute]
    inline void InitializeUninitializer()
    {
        ErrorMessage = L"The C++ module failed to load during registration for the unload events.\n";
        EventHandler^ handler = gcnew EventHandler(&DomainUnload);
        RegisterModuleUninitializer(handler);
    }

    // Initializes vtables and globals required for C++ language
    [System::Diagnostics::DebuggerStepThroughAttribute]
    [System::Runtime::ConstrainedExecution::ReliabilityContract(
        System::Runtime::ConstrainedExecution::Consistency::WillNotCorruptState,
        System::Runtime::ConstrainedExecution::Cer::Success)]
    inline void _Initialize()
    {
#if !defined (_M_CEE_MIXED)
        ActivationContext ActivationCtx;
        ActivationCtx.Create();
        ActivationCtx.Activate();
        // Load Msvcr80*.dll when the activation context is active.
        clock();
#endif  /* !defined (_M_CEE_MIXED) */

        CurrentDomain::IsDefaultDomain = AppDomain::CurrentDomain->IsDefaultAppDomain();
#if defined (_M_CEE_MIXED)
        if (CurrentDomain::IsDefaultDomain)
        {
            DefaultDomain::Entered = true;
        }
#endif  /* defined (_M_CEE_MIXED) */

#if !defined (_PARTIAL_TRUST_OBJ)
        DoDllLanguageSupportValidation();
#endif  /* !defined (_PARTIAL_TRUST_OBJ) */

#if defined (_M_CEE_MIXED)
        void *lock_free=0;
        void *fiberid=_getFiberPtrId();
        int nested=false;
        int done=false;
        System::Runtime::CompilerServices::RuntimeHelpers::PrepareConstrainedRegions();
        // this make sure that __native_startup_lock is released no matter what, in line with the
        // ReliabilityContract asserted above
        try
        {
            while (!done)
            {
                try { }
                finally
                {
#if defined (_WIN64)
                    lock_free = (void*)Interlocked::CompareExchange((System::Int64&)__native_startup_lock, (System::Int64)fiberid, (System::Int64)0);
#else  /* defined (_WIN64) */
                    lock_free = (void*)Interlocked::CompareExchange((System::IntPtr&)__native_startup_lock, (System::IntPtr)fiberid, (System::IntPtr)0);
#endif  /* defined (_WIN64) */
                    if (lock_free == 0)
                    {
                        done = true;
                    }
                    else if(lock_free == fiberid)
                    {
                        nested = true;
                        done = true;
                    }
                }
                if (!done)
                {
                    // Some other thread is running native startup/shutdown during a cctor/domain unload.
                    // Should only happen if this DLL was built using the Everett-compat loader lock fix in vcclrit.h.
                    // Wait for the other thread to complete init before we return
                    Sleep(1000);
                }
            }

            if (!CurrentDomain::IsDefaultDomain && DefaultDomain::NeedsInitialization())
            {
                InitializeDefaultAppDomain();
            }
        }
        finally
        {
            if(!nested)
            {
#if defined (_WIN64)
                Interlocked::Exchange((System::Int64&)__native_startup_lock, (System::Int64)0);
#else  /* defined (_WIN64) */
                Interlocked::Exchange((System::IntPtr&)__native_startup_lock, (System::IntPtr)0);
#endif  /* defined (_WIN64) */
            }
        }

#else  /* defined (_M_CEE_MIXED) */
        ActivationCtx.DeActivate();
        ActivationCtx.Release();
#endif  /* defined (_M_CEE_MIXED) */

        InitializeVtables();

#if defined (_M_CEE_MIXED)
        if (CurrentDomain::IsDefaultDomain)
        {
            InitializeNative();

            InitializePerProcess();
        }
#endif  /* defined (_M_CEE_MIXED) */

        InitializePerAppDomain();

        CurrentDomain::Initialized = 1;

        InitializeUninitializer();
    }

    // Uninitializes per-appdomain managed globals
    inline static void UninitializeAppDomain()
    {
        _app_exit_callback();
    }

#if defined (_M_CEE_MIXED)
    //static void __cdecl _UninitializeDefaultDomain()
        static HRESULT __stdcall _UninitializeDefaultDomain(void* cookie)
    {
                cookie;
        MANAGED_ASSERT(AppDomain::CurrentDomain->IsDefaultAppDomain(), "This function must be called in the default domain");

        // Uninitialize per-process
        _exit_callback();

        DefaultDomain::InitializedPerProcess = false;

        if(DefaultDomain::InitializedNativeFromCCTOR)
        {
            // Uninitialize native
            _cexit();
            __native_startup_state = __uninitialized;

            DefaultDomain::InitializedNativeFromCCTOR = false;
        }

        DefaultDomain::InitializedNative = false;

                return S_OK;
    }

    static void UninitializeDefaultDomain()
    {
        if (DefaultDomain::NeedsUninitialization())
        {
            if (AppDomain::CurrentDomain->IsDefaultAppDomain())
            {
                _UninitializeDefaultDomain(nullptr);
            }
            else
            {
                DoCallBackInDefaultDomain(&_UninitializeDefaultDomain, nullptr);
            }
        }
    }
#endif  /* defined (_M_CEE_MIXED) */

    // Called when the domain unloads
    // Compare with code in Cleanup
    [System::Runtime::ConstrainedExecution::PrePrepareMethod]
    [System::Runtime::ConstrainedExecution::ReliabilityContract(
        System::Runtime::ConstrainedExecution::Consistency::WillNotCorruptState,
        System::Runtime::ConstrainedExecution::Cer::Success)]
    static void DomainUnload(Object^ source, EventArgs^ arguments)
    {
        if (CurrentDomain::Initialized && !Interlocked::Exchange(CurrentDomain::Uninitialized, (int)1))
        {
#if defined (_M_CEE_MIXED)
            bool lastDomain = (Interlocked::Decrement(AllDomains::Count) == 0);
#endif  /* defined (_M_CEE_MIXED) */

            UninitializeAppDomain();

#if defined (_M_CEE_MIXED)
            if (lastDomain)
            {
                UninitializeDefaultDomain();
            }
#endif  /* defined (_M_CEE_MIXED) */
        }
    }

    // Cleans up initialization in the event of an error
    // Compare with code in DomainUnload
    [System::Diagnostics::DebuggerStepThroughAttribute]
    [System::Runtime::ConstrainedExecution::ReliabilityContract(
        System::Runtime::ConstrainedExecution::Consistency::WillNotCorruptState,
        System::Runtime::ConstrainedExecution::Cer::Success)]
    void Cleanup(System::Exception^ innerException)
    {
        try
        {
#if defined (_M_CEE_MIXED)
            // If we end up here, DomainUnload will not be called
            bool lastDomain = (Interlocked::Decrement(AllDomains::Count) == 0);
#endif  /* defined (_M_CEE_MIXED) */

            UninitializeAppDomain();

#if defined (_M_CEE_MIXED)
            if (lastDomain)
            {
                UninitializeDefaultDomain();
            }
#endif  /* defined (_M_CEE_MIXED) */
        }
        catch (System::Exception^ exception)
        {
            ThrowNestedModuleLoadException(innerException, exception);
        }
        catch (System::Object^ exception)
        {
            exception;
            ThrowNestedModuleLoadException(innerException, nullptr);
        }
    }

public:
    [System::Diagnostics::DebuggerStepThroughAttribute]
    [System::Runtime::ConstrainedExecution::ReliabilityContract(
        System::Runtime::ConstrainedExecution::Consistency::WillNotCorruptState,
        System::Runtime::ConstrainedExecution::Cer::Success)]
    void Initialize()
    {
#if defined (_M_CEE_MIXED)
        bool allDomainsCountIncremented = false;

        // this make sure that AllDomains::Count reflects the actual count no matter what,
        // in line with the ReliabilityContract asserted above
        System::Runtime::CompilerServices::RuntimeHelpers::PrepareConstrainedRegions();
#endif  /* defined (_M_CEE_MIXED) */
        try
        {
            ErrorMessage = L"The C++ module failed to load.\n";
#if defined (_M_CEE_MIXED)
            System::Runtime::CompilerServices::RuntimeHelpers::PrepareConstrainedRegions();
            try { } finally
            {
                Interlocked::Increment(AllDomains::Count);
                allDomainsCountIncremented = true;
            }
#endif  /* defined (_M_CEE_MIXED) */
            _Initialize();
        }
        catch (System::Exception^ exception)
        {
#if defined (_M_CEE_MIXED)
            if (allDomainsCountIncremented)
#endif  /* defined (_M_CEE_MIXED) */
            {
                Cleanup(exception);
            }
            ThrowModuleLoadException(ErrorMessage, exception);
        }
        catch (System::Object^ exception)
        {
            exception;
#if defined (_M_CEE_MIXED)
            if (allDomainsCountIncremented)
#endif  /* defined (_M_CEE_MIXED) */
            {
                Cleanup(nullptr);
            }
            ThrowModuleLoadException(ErrorMessage, nullptr);
        }
    }

};

} // namespace __identifier("<CrtImplementationDetails>")

/***
* .cctor - This is global constructor for user module.
*
* Purpose:
*       This function is called during appdomain creation. Here is we
*       initialize all the vtables, and native/process/appdomain variables.
*
* Exit:
*
* Entry:
*
*******************************************************************************/
[System::Diagnostics::DebuggerStepThroughAttribute]
void __clrcall __identifier(".cctor")()
{
    using namespace __identifier("<CrtImplementationDetails>");

    LanguageSupport languageSupport;
    languageSupport.Initialize();
}
