#ifndef _COREDEFS_H_
#define _COREDEFS_H_

template
<
    typename T
>
class CZeroInit
{
protected:
    CZeroInit(
        VOID
        )
    {
        memset(static_cast<T*>(this), 0, sizeof(T));
    }
};

#define DISABLE_COPY(className) \
    private: \
    className(const className &src); \
    className& operator = (const className &src); \

#define DISABLE_CREATE(className) \
    protected: \
    className() {} \
    private: \

#if DBG
    #define FORCE_INLINE
#else // DBG
    #define FORCE_INLINE __forceinline
#endif // DBG

#if defined(__cplusplus)
    #define COREDEFS_INLINE inline
#else
    #define COREDEFS_INLINE __inline
#endif

#ifndef ARRAYSIZE
    #define ARRAYSIZE(x) (sizeof(x)/sizeof((x)[0]))
#endif // ARRAYSIZE

//
// Pre-Vista, use the "__debugbreak()" ("int 3") intrinsic
// for assertions. In Vista and above, use the NT_ASSERT inline
// macro which expands to "int 2c". NT_ASSERT has 
// better integration with debuggers and other error
// handling frameworks, such as WER.
//
#if (NTDDI_VERSION < NTDDI_VISTA)

#define __BREAK_ON_FALSE(expr)                      \
    ((!(expr)) ?                                    \
        (__debugbreak(), FALSE) : TRUE);            \

#else  // NTDDI_VERSION < NTDDI_VISTA

#define __BREAK_ON_FALSE(expr)                      \
    ((!(expr)) ?                                    \
        (__annotation(L"Debug", L"AssertFail", L#expr), \
         DbgRaiseAssertionFailure(), FALSE) :       \
        TRUE);

#endif // NTDDI_VERSION < NTDDI_VISTA

//
// CBreakOnFailure is a utility class to support debugging Chk* macros-based
// code. You can set a global variable (g_hResultToBreakOn) in the debugger
// to break on specific HRESULT code (in runtime). At compile time,
// a "break-in" HRESULT can also be set using CHK_MACROS_HRESULT_TO_BREAK_ON
// (which you can define either at the project level or before including
// this file).
//
#ifndef CHK_MACROS_HRESULT_TO_BREAK_ON
    #define CHK_MACROS_HRESULT_TO_BREAK_ON S_OK
#endif //CHK_MACROS_HRESULT_TO_BREAK_ON

class CBreakOnFailure
{
public:
    DECLSPEC_NOINLINE
    static
    VOID
    CheckToBreakOnFailure(
        __in_opt HRESULT hResult
        )
    {
        //
        // Break only if g_hResultToBreakOn is modified and hResult is a 
        // failure HRESULT
        //
        if (g_hResultToBreakOn != S_OK && FAILED(hResult))
        {
            //
            // -1 (0xFFFFFFFF) is a special code to cause a debugger
            // break-in for any error failure
            //
            if (g_hResultToBreakOn == hResult || 
                g_hResultToBreakOn == static_cast<HRESULT>(-1))
            {
                __debugbreak();
            }
        }
    }

private:
    static HRESULT g_hResultToBreakOn;
};

__declspec(selectany)
HRESULT
CBreakOnFailure::g_hResultToBreakOn = CHK_MACROS_HRESULT_TO_BREAK_ON;

#define CHK_MACROS_CHECK_TO_BREAK_ON_FAILURE(hr) CBreakOnFailure::CheckToBreakOnFailure(hr)

//
// Define a few useful debugging macros
//
#if (DBG) || (defined(_DEBUG))

    #ifndef ASSERT
    #define ASSERT(expr)                __BREAK_ON_FALSE(expr)
    #endif // ASSERT

    #ifndef VERIFY
    #define VERIFY(expr)                ASSERT(expr)
    #endif // VERIFY

    #define UNREACHABLE_CODE()          ASSERT(FALSE)

#else // defined(DBG) || defined(_DEBUG)

    #undef ASSERT
    #undef VERIFY

    #define ASSERT(expr)
    #define VERIFY(expr)                { if (expr) {} }

    #define UNREACHABLE_CODE()

#endif // defined(DBG) || defined(_DEBUG)

//
// Memory allocators
//
COREDEFS_INLINE
__allocator
__out_bcount_opt(cbBlockSize)
PVOID
MemoryAlloc(
    __in SIZE_T cbBlockSize
    )
{
    return HeapAlloc(GetProcessHeap(), 0, cbBlockSize);
}

COREDEFS_INLINE
VOID
MemoryFree(
    __inout PVOID pv
    )
{
    HeapFree(GetProcessHeap(), 0, pv);
}

COREDEFS_INLINE
void*
__cdecl
operator new (size_t _Size)
{
    return MemoryAlloc(_Size);
}

COREDEFS_INLINE
void*
__cdecl
operator new [] (size_t _Size)
{
    return MemoryAlloc(_Size);
}

COREDEFS_INLINE
void
__cdecl
operator delete (void* p)
{
    MemoryFree(p);
}

COREDEFS_INLINE
void
__cdecl
operator delete [] (void* p)
{
    MemoryFree(p);
}

#endif // _COREDEFS_H_
