/*!
* \file cactus_types.h
* \brief 常用类型声明
*
* 常用类型声明
*
* \author wangyang
* \date 2015/01/30
* \version 1.0
*/

#ifndef __CACTUS_CACTUS_TYPES_H__
#define __CACTUS_CACTUS_TYPES_H__

#if defined(WIN32)
    #ifdef _MSC_VER
        #if _MSC_VER <= 1400        // vc8.0 vs2005
            #define CACTUS_VC80
        #elif _MSC_VER <= 1500      // vc9.0 vs2008
            #define CACTUS_VC90
        #endif
    #endif
#endif


#ifndef WINVER                      // Allow use of features specific to Windows XP or later.
    #define WINVER 0x0501
#endif

#ifndef _WIN32_WINNT                // Allow use of features specific to Windows XP or later.
    #define _WIN32_WINNT 0x0501
#endif

#ifndef _WIN32_WINDOWS              // Allow use of features specific to Windows 98 or later.
    #define _WIN32_WINDOWS 0x0410
#endif

#ifndef _WIN32_IE                   // Allow use of features specific to IE 6.0 or later.
    #define _WIN32_IE 0x0600
#endif

#ifndef _RICHEDIT_VER
    #define _RICHEDIT_VER   0x0200
#endif

#include <atlbase.h>
#include <iostream>
#include <vector>
#include <map>
#include <string>

#if _MSC_VER >= 1400
//  Following 8 lines: workaround for a bug in some older SDKs
#   pragma push_macro("_interlockedbittestandset")
#   pragma push_macro("_interlockedbittestandreset")
#   pragma push_macro("_interlockedbittestandset64")
#   pragma push_macro("_interlockedbittestandreset64")
#   define _interlockedbittestandset _local_interlockedbittestandset
#   define _interlockedbittestandreset _local_interlockedbittestandreset
#   define _interlockedbittestandset64 _local_interlockedbittestandset64
#   define _interlockedbittestandreset64 _local_interlockedbittestandreset64
#   include <intrin.h> // to force the header not to be included elsewhere
#   pragma pop_macro("_interlockedbittestandreset64")
#   pragma pop_macro("_interlockedbittestandset64")
#   pragma pop_macro("_interlockedbittestandreset")
#   pragma pop_macro("_interlockedbittestandset")
#endif

#if (_MSC_VER < 1300)
    typedef signed char       int8_t;
    typedef signed short      int16_t;
    typedef signed int        int32_t;
    typedef unsigned char     uint8_t;
    typedef unsigned short    uint16_t;
    typedef unsigned int      uint32_t;
#else
    typedef signed __int8     int8_t;
    typedef signed __int16    int16_t;
    typedef signed __int32    int32_t;
    typedef unsigned __int8   uint8_t;
    typedef unsigned __int16  uint16_t;
    typedef unsigned __int32  uint32_t;
#endif

typedef signed __int64       int64_t;
typedef unsigned __int64     uint64_t;

#ifndef _INT8
    #define _INT8
    typedef signed char                 int8;
#endif

#ifndef _INT16
    #define _INT16
    typedef short                       int16;
#endif

#ifndef _INT32
    #define _INT32
    typedef int                         int32;
#endif

#ifndef _UINT8
    #define _UINT8
    typedef unsigned char               uint8;
#endif

#ifndef _UINT16
    #define _UINT16
    typedef unsigned short              uint16;
#endif

#ifndef _UINT32
    #define _UINT32
    typedef unsigned int                uint32;
#endif

#ifndef _UINT
    #define _UINT
    typedef unsigned int                uint;
#endif

#ifndef BYTE
    typedef unsigned char BYTE;
#endif


#if defined(WIN32) && (defined(__MINGW32__) == 0)
    typedef __int8             byte8;
    typedef unsigned __int8    ubyte8;
    typedef __int16            byte16;
    typedef unsigned __int16   ubyte16;
    typedef __int32            byte32;
    typedef unsigned __int32   ubyte32;
    typedef __int64            byte64;
    typedef unsigned __int64   ubyte64;
#else
    typedef char               byte8;
    typedef unsigned char      ubyte8;
    typedef short              byte16;
    typedef unsigned short     ubyte16;
    typedef int                byte32;
    typedef unsigned int       ubyte32;
    typedef long long          byte64;
    typedef unsigned long long ubyte64;
#endif


#ifndef _INT64
    #define _INT64
    #if __LP64__
        typedef long                int64;
    #else
        typedef long long           int64;
    #endif
#endif

#ifndef _UINT64
    #define _UINT64
    #if __LP64__
        typedef unsigned long       uint64;
    #else
        typedef unsigned long long  uint64;
    #endif
#endif

#ifndef _CHAR16
    #define _CHAR16
    typedef wchar_t char16;
#endif

#ifndef _CHAR32
    #define _CHAR32
    typedef signed int         char32;
#endif


typedef std::wstring                        string16;
typedef std::string                         string8;
typedef std::string                         utf8string;
typedef std::basic_string<TCHAR>            tstring;
typedef std::vector<string16>               string16array;
typedef std::vector<string8>                string8array;
typedef std::vector<utf8string>             utf8stringarray;
typedef std::vector<std::string>            stringlist;
typedef std::vector<string8>                string8list;
typedef std::vector<string16>               string16list;
typedef std::map<std::string, std::string>  stringmap;
typedef std::map<string16, string16>        string16map;
typedef std::vector<BYTE>                   bytearray;

typedef stringlist::const_iterator          string_iter;
typedef stringmap::const_iterator           stringmap_iter;
typedef string16map::const_iterator         string16map_iter;

#ifdef _UNICODE
    typedef std::wstring tstring;
#else
    typedef std::string  tstring;
#endif

#define GG_LONGLONG(x)      x##I64
#define GG_ULONGLONG(x)     x##UI64

#define GG_INT8_C(x)        (x)
#define GG_INT16_C(x)       (x)
#define GG_INT32_C(x)       (x)
#define GG_INT64_C(x)       GG_LONGLONG(x)

#define GG_UINT8_C(x)       (x ## U)
#define GG_UINT16_C(x)      (x ## U)
#define GG_UINT32_C(x)      (x ## U)
#define GG_UINT64_C(x)      GG_ULONGLONG(x)

#ifndef _K_INT_MAX
    #define _K_INT_MAX

    const uint8_t  kuint8max  = ((uint8_t) 0xFF);
    const uint16_t kuint16max = ((uint16_t) 0xFFFF);
    const uint32_t kuint32max = ((uint32_t) 0xFFFFFFFF);
    const uint64_t kuint64max = ((uint64_t) GG_LONGLONG(0xFFFFFFFFFFFFFFFF));
    const  int8_t  kint8min   = ((int8_t) 0x80);
    const  int8_t  kint8max   = ((int8_t) 0x7F);
    const  int16_t kint16min  = ((int16_t) 0x8000);
    const  int16_t kint16max  = ((int16_t) 0x7FFF);
    const  int32_t kint32min  = ((int32_t) 0x80000000);
    const  int32_t kint32max  = ((int32_t) 0x7FFFFFFF);
    const  int64_t kint64min  = ((int64_t) GG_LONGLONG(0x8000000000000000));
    const  int64_t kint64max  = ((int64_t) GG_LONGLONG(0x7FFFFFFFFFFFFFFF));
#endif

namespace cactus
{










// This macro is used to standardize the wide character string literals between UNIX and Windows.
// Unix L"" is UTF32, and on windows it's UTF16.  Because of built-in assumptions on the size
// of string literals, it's important to match behaviour between Unix and Windows.  Unix will be defined
// as u"" (char16_t)
#ifdef PLATFORM_UNIX
    #define W(str)  u##str
#else // PLATFORM_UNIX
    #define W(str)  L##str
#endif // PLATFORM_UNIX


#define lengthof(x) (sizeof(x)/sizeof(*x))                              ///< 获取字符串长度
#define NS_WARNING(_s) printf(">>> " _s "!\n")
#define MsgBox(msg)     ::MessageBox(NULL, msg, _T("信息"), 0)              ///< 弹出消息块


#ifndef ASSERT
    #define ASSERT ATLASSERT
#endif
}

#if defined(_WIN64)
    #define nullptr 0LL
#else
    #define nullptr 0L
#endif

#define CC_DISABLE_COPY(Class) \
    private: \
    Class(const Class &); \
    Class &operator =(const Class &);


// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&);               \
    void operator=(const TypeName&)


// An older, deprecated, politically incorrect name for the above.
#define DISALLOW_EVIL_CONSTRUCTORS(TypeName) DISALLOW_COPY_AND_ASSIGN(TypeName)

// A macro to disallow all the implicit constructors, namely the
// default constructor, copy constructor and operator= functions.
//
// This should be used in the private: declarations for a class
// that wants to prevent anyone from instantiating it. This is
// especially useful for classes containing only static methods.
#define DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
    TypeName();                                    \
    DISALLOW_COPY_AND_ASSIGN(TypeName)

#define NOTREACHED()

// This template function declaration is used in defining arraysize.
// Note that the function doesn't need an implementation, as we only
// use its type.
template <typename T, size_t N>
char(&ArraySizeHelper(T(&array)[N]))[N];

// That gcc wants both of these prototypes seems mysterious. VC, for
// its part, can't decide which to use (another mystery). Matching of
// template overloads: the final frontier.
#ifndef _MSC_VER
    template <typename T, size_t N>
    char(&ArraySizeHelper(const T(&array)[N]))[N];
#endif

#define arraysize(array) (sizeof(ArraySizeHelper(array)))
#define SAFE_DELETE(p)              do {if (p) {delete p; p = NULL;}} while (0)                 ///< 安全释放指针
#define SAFE_DELETE_ARRAY(a)        do {if (a) {delete [] a; a = NULL;}} while (0)              ///< 安全释放数组指针
#define SAFE_DELETE_BYTE_ARRAY(a)   do {if (a) {delete [] (LPBYTE)a; a = NULL;}} while (0)
#define SAFE_RELEASE(p)             do {if (p) {p->Release (); p = NULL;}} while (0)            ///< 安全释放COM对象
#define RIF(e)                      do {if (FAILED (hr=(e))) return hr;} while (0)
#define RETURN_IF(cond)             if(cond) return
#define BREAK_IF(cond)              if(cond) break
#define CONTINUE_IF(cond)           if(cond) continue

#ifdef _DEBUG
    #define CCLOG(format, ...)      cactus::CCLog(format, ##__VA_ARGS__)
#else
    #define CCLOG(...)              do {} while (0)
#endif

#define __CCLOGWITHFUNCTION(s, ...) \
    CCLog("%s : %s",__FUNCTION__, cactus::format_string(s, ##__VA_ARGS__))


#ifdef _DEBUG
    #include <atltrace.h>
    #define TRACE ATLTRACE
    #define VERIFY(f)          ASSERT(f)
    #define DEBUG_ONLY(f)      (f)
#else // _DEBUG
    #define TRACE              __noop
    #define VERIFY(f)          ((void)(f))
    #define DEBUG_ONLY(f)      ((void)0)
#endif

namespace cactus
{
#ifdef _MSC_VER
enum { IS_COMPILER_MSVC = 1 };
#else
enum { IS_COMPILER_MSVC = 0 };
#endif
}

namespace cactus
{
__inline HRESULT HRESULTFromLastError()
{
    DWORD error_code = ::GetLastError();
    //ASSERT1(error_code != NO_ERROR);
    return (error_code != NO_ERROR) ? HRESULT_FROM_WIN32(error_code) : E_FAIL;
}
}

namespace cactus
{
class NonCopyable
{
protected:
    NonCopyable() {}

private:
    NonCopyable(const NonCopyable&);
    NonCopyable& operator =(const NonCopyable&);
};
}


//------------------------------------------------
// kis_lock.h
namespace cactus
{
class KLock
{
private:
    CRITICAL_SECTION  sec;
private:
    KLock(const KLock& locker);

public:
    KLock()
    {
        memset(&sec, 0, sizeof(CRITICAL_SECTION));
        ::InitializeCriticalSection(&sec);
    }

    ~KLock()
    {
        ::DeleteCriticalSection(&sec);
    }

    void Lock()
    {
        EnterCriticalSection(&sec);
    }

    void Unlock()
    {
        LeaveCriticalSection(&sec);
    }


    BOOL TryLock()
    {
        return ::TryEnterCriticalSection(&sec);
    }
};

template< class T>
class KLockerT
{
private:
    T& m_lock;

public:
    KLockerT(T& lock) : m_lock(lock)
    {
        lock.Lock();
    }
    ~KLockerT()
    {
        m_lock.Unlock();
    }
};

typedef KLockerT< KLock >   KLocker;

template< class T>
class KPtrLockerT
{
private:
    T* m_lock;

public:
    KPtrLockerT(T* lock) : m_lock(lock)
    {
        if (m_lock)
            m_lock->Lock();
    }
    ~KPtrLockerT()
    {
        if (m_lock)
            m_lock->Unlock();
    }
};

typedef KPtrLockerT< KLock >    KPtrLokcer;
}

//------------------------------------------------
// kis_singleton.h
namespace cactus
{
/*! @brief 定义一个线程安全的单例，避免多个线程同时初始化
 *
 */
template<class T>
class KSingleton
{
public:
    _declspec(noinline) static T& Instance()
    {
        static T* _pInst = NULL;
        static volatile LONG _lock = 0;

        while (!_pInst) {
            if (::InterlockedIncrement(&_lock) == 1) {
                _pInst = CreateInstance();
                ::InterlockedDecrement(&_lock);
            } else {
                ::InterlockedDecrement(&_lock);
                ::Sleep(10);
            }
        }

        return *_pInst;
    }

private:
    _declspec(noinline) static T* CreateInstance()
    {
        static T _inst;
        return &_inst;
    }
};
}



#ifndef _SCOPED_HANDLE
#define _SCOPED_HANDLE
//------------------------------------------------
// scoped_handle.h
//namespace cactus
//{




// Used so we always remember to close the handle.
// The class interface matches that of ScopedStdioHandle in  addition to an
// IsValid() method since invalid handles on windows can be either NULL or
// INVALID_HANDLE_VALUE (-1).
//
// Example:
//   ScopedHandle hfile(CreateFile(...));
//   if (!hfile.Get())
//     ...process error
//   ReadFile(hfile.Get(), ...);
//
// To sqirrel the handle away somewhere else:
//   secret_handle_ = hfile.Take();
//
// To explicitly close the handle:
//   hfile.Close();
class ScopedHandle
{
public:
    ScopedHandle() : handle_(NULL) { }

    explicit ScopedHandle(HANDLE h) : handle_(NULL)
    {
        Set(h);
    }

    ~ScopedHandle()
    {
        Close();
    }

    // Use this instead of comparing to INVALID_HANDLE_VALUE to pick up our NULL
    // usage for errors.
    bool IsValid() const
    {
        return handle_ != NULL;
    }

    void Set(HANDLE new_handle)
    {
        Close();

        // Windows is inconsistent about invalid handles, so we always use NULL
        if (new_handle != INVALID_HANDLE_VALUE)
            handle_ = new_handle;
    }

    HANDLE Get()
    {
        return handle_;
    }

    operator HANDLE()
    {
        return handle_;
    }

    HANDLE Take()
    {
        // transfers ownership away from this object
        HANDLE h = handle_;
        handle_ = NULL;
        return h;
    }

    void Close()
    {
        if (handle_) {
            if (!::CloseHandle(handle_)) {
                NOTREACHED();
            }

            handle_ = NULL;
        }
    }

private:
    HANDLE handle_;
    DISALLOW_EVIL_CONSTRUCTORS(ScopedHandle);
};

// Like ScopedHandle, but for HANDLEs returned from FindFile().
class ScopedFindFileHandle
{
public:
    explicit ScopedFindFileHandle(HANDLE handle) : handle_(handle)
    {
        // Windows is inconsistent about invalid handles, so we always use NULL
        if (handle_ == INVALID_HANDLE_VALUE)
            handle_ = NULL;
    }

    ~ScopedFindFileHandle()
    {
        if (handle_)
            FindClose(handle_);
    }

    // Use this instead of comparing to INVALID_HANDLE_VALUE to pick up our NULL
    // usage for errors.
    bool IsValid() const
    {
        return handle_ != NULL;
    }

    operator HANDLE()
    {
        return handle_;
    }

private:
    HANDLE handle_;

    DISALLOW_EVIL_CONSTRUCTORS(ScopedFindFileHandle);
};

// Like ScopedHandle but for HDC.  Only use this on HDCs returned from
// CreateCompatibleDC.  For an HDC returned by GetDC, use ReleaseDC instead.
class ScopedHDC
{
public:
    ScopedHDC() : hdc_(NULL) { }
    explicit ScopedHDC(HDC h) : hdc_(h) { }

    ~ScopedHDC()
    {
        Close();
    }

    HDC Get()
    {
        return hdc_;
    }

    void Set(HDC h)
    {
        Close();
        hdc_ = h;
    }

    operator HDC()
    {
        return hdc_;
    }

private:
    void Close()
    {
#ifdef NOGDI
        assert(false);
#else

        if (hdc_)
            DeleteDC(hdc_);

#endif  // NOGDI
    }

    HDC hdc_;
    DISALLOW_EVIL_CONSTRUCTORS(ScopedHDC);
};

// Like ScopedHandle but for GDI objects.
template<class T>
class ScopedGDIObject
{
public:
    ScopedGDIObject() : object_(NULL) {}
    explicit ScopedGDIObject(T object) : object_(object) {}

    ~ScopedGDIObject()
    {
        Close();
    }

    T Get()
    {
        return object_;
    }

    void Set(T object)
    {
        if (object_ && object != object_)
            Close();

        object_ = object;
    }

    ScopedGDIObject& operator=(T object)
    {
        Set(object);
        return *this;
    }

    operator T()
    {
        return object_;
    }

private:
    void Close()
    {
        if (object_)
            DeleteObject(object_);
    }

    T object_;
    DISALLOW_COPY_AND_ASSIGN(ScopedGDIObject);
};

// Typedefs for some common use cases.
typedef ScopedGDIObject<HBITMAP> ScopedBitmap;
typedef ScopedGDIObject<HRGN> ScopedHRGN;
typedef ScopedGDIObject<HFONT> ScopedHFONT;


// Like ScopedHandle except for HGLOBAL.
template<class T>
class ScopedHGlobal
{
public:
    explicit ScopedHGlobal(HGLOBAL glob) : glob_(glob)
    {
        data_ = static_cast<T*>(GlobalLock(glob_));
    }

    ~ScopedHGlobal()
    {
        GlobalUnlock(glob_);
    }

    T* get()
    {
        return data_;
    }

    size_t Size() const
    {
        return GlobalSize(glob_);
    }

    T* operator->() const
    {
        assert(data_ != 0);
        return data_;
    }

private:
    HGLOBAL glob_;

    T* data_;

    DISALLOW_EVIL_CONSTRUCTORS(ScopedHGlobal);
};


// Simple scoped memory releaser class for COM allocated memory.
// Example:
//   CoMemReleaser<ITEMIDLIST> file_item;
//   SHGetSomeInfo(&file_item, ...);
//   ...
//   return;  <-- memory released
template<typename T>
class CoMemReleaser
{
public:
    explicit CoMemReleaser() : mem_ptr_(NULL) {}

    ~CoMemReleaser()
    {
        if (mem_ptr_)
            CoTaskMemFree(mem_ptr_);
    }

    T** operator&()
    {
        return &mem_ptr_;
    }

    operator T*()
    {
        return mem_ptr_;
    }

private:
    T* mem_ptr_;

    DISALLOW_COPY_AND_ASSIGN(CoMemReleaser);
};

// Initializes COM in the constructor (STA), and uninitializes COM in the
// destructor.
class ScopedCOMInitializer
{
public:
    ScopedCOMInitializer() : hr_(CoInitialize(NULL))
    {
    }

    ScopedCOMInitializer::~ScopedCOMInitializer()
    {
        if (SUCCEEDED(hr_))
            CoUninitialize();
    }

    // Returns the error code from CoInitialize(NULL)
    // (called in constructor)
    inline HRESULT error_code() const
    {
        return hr_;
    }

protected:
    HRESULT hr_;

private:
    DISALLOW_COPY_AND_ASSIGN(ScopedCOMInitializer);
};

class ScopedStdioHandle
{
public:
    ScopedStdioHandle() : handle_(NULL)
    {
    }

    explicit ScopedStdioHandle(FILE *handle) : handle_(handle)
    {
    }

    ~ScopedStdioHandle()
    {
        Close();
    }

    void Close()
    {
        if (handle_) {
            fclose(handle_);
            handle_ = NULL;
        }
    }

    FILE *get() const
    {
        return handle_;
    }

    FILE *Take()
    {
        FILE * temp = handle_;
        handle_ = NULL;
        return temp;
    }

    void Set(FILE *newhandle)
    {
        Close();
        handle_ = newhandle;
    }

private:
    FILE * handle_;

    DISALLOW_EVIL_CONSTRUCTORS(ScopedStdioHandle);
};

//}

#endif



// Turn off some annoying warnings
#if defined(_MSC_VER)
    #pragma warning(disable:4018) // signed/unsigned comparison
    #pragma warning(disable:4251) // ... needs to have dll-interface warning
    #pragma warning(disable:4355) // 'this' : used in base member initializer list
    #pragma warning(disable:4996) // VC++ 8.0 deprecation warnings
    #pragma warning(disable:4351) // new behavior: elements of array '...' will be default initialized
    #pragma warning(disable:4675) // resolved overload was found by argument-dependent lookup
    #pragma warning(disable:4275) // non dll-interface class 'std::exception' used as base for dll-interface class 'Poco::Exception'
    #pragma warning(disable:4250) // VC++ 11.0: inheriting from std stream classes produces C4250 warning;
    // see <http://connect.microsoft.com/VisualStudio/feedback/details/733720/inheriting-from-std-fstream-produces-c4250-warning>
#endif

#endif // __CACTUS_CACTUS_TYPES_H__
