/* yvals.h values header for Microsoft C/C++ */
#pragma once
#ifndef _YVALS
#define _YVALS

#include <crtdefs.h>

#ifdef  _MSC_VER
#pragma pack(push,_CRT_PACKING)
#endif  /* _MSC_VER */

#define _CPPLIB_VER	405

#if defined(MRTDLL)
/*
process-global is the default for code built with /clr or /clr:oldSyntax.
appdomain-global is the default for code built with /clr:pure.
Code in MSVCM is built with /clr, but is used by user code built with /clr:pure
so it must conform to the expectations of /clr:pure clients.
Use __PURE_APPDOMAIN_GLOBAL when a global needs to be appdomain-global for pure
clients and process-global for mixed clients.
*/
#define __PURE_APPDOMAIN_GLOBAL   __declspec(appdomain)
#else
#define __PURE_APPDOMAIN_GLOBAL
#endif

#ifndef __CRTDECL
#if defined(_M_CEE_PURE) || defined(MRTDLL)
#define __CRTDECL   __clrcall
#else
#define __CRTDECL   __cdecl
#endif
#endif

#ifndef __CLR_OR_THIS_CALL
#if defined(MRTDLL) || defined(_M_CEE_PURE)
#define __CLR_OR_THIS_CALL  __clrcall
#else
#define __CLR_OR_THIS_CALL
#endif
#endif

#ifndef __CLRCALL_OR_CDECL
#if defined(MRTDLL) || defined(_M_CEE_PURE)
#define __CLRCALL_OR_CDECL __clrcall
#else
#define __CLRCALL_OR_CDECL __cdecl
#endif
#endif

#ifndef __CLRCALL_PURE_OR_CDECL
#if defined(_M_CEE_PURE)
#define __CLRCALL_PURE_OR_CDECL __clrcall
#else
#define __CLRCALL_PURE_OR_CDECL __cdecl
#endif
#endif

		/* NAMING PROPERTIES */
#define _WIN32_C_LIB	1

		/* THREAD AND LOCALE CONTROL */
#define _MULTI_THREAD	1	/* nontrivial locks if multithreaded */
#define _IOSTREAM_OP_LOCKS	1	/* lock iostream operations */
#define _GLOBAL_LOCALE	0	/* 0 for per-thread locales, 1 for shared */

		/* THREAD-LOCAL STORAGE */
#define _COMPILER_TLS	1	/* 1 if compiler supports TLS directly */
 #if _MULTI_THREAD
  #define _TLS_QUAL	__declspec(thread)	/* TLS qualifier, if any */

 #else /* _MULTI_THREAD */
  #define _TLS_QUAL
 #endif /* _MULTI_THREAD */

 #ifndef _HAS_EXCEPTIONS
  #define  _HAS_EXCEPTIONS  1	/* predefine as 0 to disable exceptions */
 #endif /* _HAS_EXCEPTIONS */

 #ifndef _HAS_NAMESPACE
  #define  _HAS_NAMESPACE  1	/* predefine as 0 to disable exceptions */
 #endif /* _HAS_NAMESPACE */

#ifndef _HAS_IMMUTABLE_SETS
 #define _HAS_IMMUTABLE_SETS 0
#endif /* _HAS_IMMUTABLE_SETS */

#ifndef _HAS_STRICT_CONFORMANCE
 #define _HAS_STRICT_CONFORMANCE 0
#endif /* _HAS_STRICT_CONFORMANCE */

#define _GLOBAL_USING	1

#if !defined (_HAS_ITERATOR_DEBUGGING)
	#if defined (_DEBUG)
		#define _HAS_ITERATOR_DEBUGGING	1	/* for range checks, etc. */
	#else
		#define _HAS_ITERATOR_DEBUGGING	0
	#endif	/* defined (_DEBUG) */
#else
	#if !defined (_DEBUG) && _HAS_ITERATOR_DEBUGGING != 0
		#include <crtwrn.h>
		#pragma _CRT_WARNING( _NO_ITERATOR_DEBUGGING )
		#undef _HAS_ITERATOR_DEBUGGING
		#define _HAS_ITERATOR_DEBUGGING 0
	#endif
#endif /* !defined (_HAS_ITERATOR_DEBUGGING) */

/* _SECURE_CRT definitions */

#define __STR2WSTR(str)    L##str
#define _STR2WSTR(str)     __STR2WSTR(str)

#define __FILEW__          _STR2WSTR(__FILE__)
#define __FUNCTIONW__      _STR2WSTR(__FUNCTION__)

/* _SECURE_SCL switches: default values */

#if !defined (_SECURE_SCL)
#define _SECURE_SCL 1
#endif

#if !defined (_SECURE_SCL_DEPRECATE)
#if defined(_SCL_SECURE_NO_DEPRECATE)
#define _SECURE_SCL_DEPRECATE 0
#else
#define _SECURE_SCL_DEPRECATE 1
#endif
#endif

#if !defined (_SECURE_SCL_THROWS)
#define _SECURE_SCL_THROWS 0
#endif

/* _SECURE_SCL switches: helper macros */

#if _SECURE_SCL && _SECURE_SCL_DEPRECATE
#define _SCL_INSECURE_DEPRECATE _CRT_DEPRECATE_TEXT("You have used a std:: construct that is not safe. See documentation on how to use the Safe Standard C++ Library")
#else
#define _SCL_INSECURE_DEPRECATE
#endif

/*
 * Assert in debug builds.
 * set errno and return
 *
 */
 #ifdef _DEBUG 

#define _SCL_SECURE_INVALID_PARAMETER(expr) ::_invalid_parameter(__STR2WSTR(#expr), __FUNCTIONW__, __FILEW__, __LINE__, 0)

 #else

#define _SCL_SECURE_INVALID_PARAMETER(expr) ::_invalid_parameter_noinfo()

 #endif

 #if _SECURE_SCL_THROWS

 #define _SCL_SECURE_INVALID_ARGUMENT_NO_ASSERT		_Xinvarg()
 #define _SCL_SECURE_OUT_OF_RANGE_NO_ASSERT			_Xran()

 #else /* _SECURE_SCL_THROWS */

 #define _SCL_SECURE_INVALID_ARGUMENT_NO_ASSERT		_SCL_SECURE_INVALID_PARAMETER("invalid argument")
 #define _SCL_SECURE_OUT_OF_RANGE_NO_ASSERT			_SCL_SECURE_INVALID_PARAMETER("out of range")

 #endif /* _SECURE_SCL_THROWS */

 #define _SCL_SECURE_ALWAYS_VALIDATE(cond)				\
	{													\
		if (!(cond))									\
		{												\
			_ASSERTE((#cond, 0));						\
			_SCL_SECURE_INVALID_ARGUMENT_NO_ASSERT;		\
		}												\
	}

 #define _SCL_SECURE_ALWAYS_VALIDATE_RANGE(cond)		\
	{													\
		if (!(cond))									\
		{												\
			_ASSERTE((#cond, 0));						\
			_SCL_SECURE_OUT_OF_RANGE_NO_ASSERT;			\
		}												\
	}

// validation a la Secure CRT
 #define _SCL_SECURE_CRT_VALIDATE(cond, retvalue)		\
	{													\
		if (!(cond))									\
		{												\
			_ASSERTE((#cond, 0));						\
			_SCL_SECURE_INVALID_PARAMETER(cond);		\
			return (retvalue);							\
		}												\
	}

 #if _SECURE_SCL

 #define _SCL_SECURE_VALIDATE(cond)						\
	{													\
		if (!(cond))									\
		{												\
			_ASSERTE((#cond, 0));						\
			_SCL_SECURE_INVALID_ARGUMENT_NO_ASSERT;		\
		}												\
		__analysis_assume(cond);						\
	}

 #define _SCL_SECURE_VALIDATE_RANGE(cond)				\
	{													\
		if (!(cond))									\
		{												\
			_ASSERTE((#cond, 0));						\
			_SCL_SECURE_OUT_OF_RANGE_NO_ASSERT;			\
		}												\
		__analysis_assume(cond);						\
	}

 #define _SCL_SECURE_INVALID_ARGUMENT					\
	{													\
		_ASSERTE(("Standard C++ Libraries Invalid Argument", 0)); \
		_SCL_SECURE_INVALID_ARGUMENT_NO_ASSERT;			\
	}
 #define _SCL_SECURE_OUT_OF_RANGE						\
	{													\
		_ASSERTE(("Standard C++ Libraries Out of Range", 0)); \
		_SCL_SECURE_OUT_OF_RANGE_NO_ASSERT;				\
	}

 #define _SCL_SECURE_MOVE(func, dst, size, src, count)	func((dst), (size), (src), (count))
 #define _SCL_SECURE_COPY(func, dst, size, src, count)	func((dst), (size), (src), (count))

 #else /* _SECURE_SCL */
 
/* when users disable _SECURE_SCL to get performance, we don't want analysis warnings from SCL headers */
#if _HAS_ITERATOR_DEBUGGING
 #define _SCL_SECURE_VALIDATE(cond)			__analysis_assume(cond)
 #define _SCL_SECURE_VALIDATE_RANGE(cond)	__analysis_assume(cond)
#else
 #define _SCL_SECURE_VALIDATE(cond)
 #define _SCL_SECURE_VALIDATE_RANGE(cond)
#endif

 #define _SCL_SECURE_INVALID_ARGUMENT 
 #define _SCL_SECURE_OUT_OF_RANGE 

 #define _SCL_SECURE_MOVE(func, dst, size, src, count)	func((dst), (src), (count))
 #define _SCL_SECURE_COPY(func, dst, size, src, count)	func((dst), (src), (count))

 #endif /* _SECURE_SCL */

/* _SECURE_VALIDATION is the name of the bool template argument used to define iterators
   with secure validation selectively turned on or off (for an example, see the std::list
   iterators). Notice that all the _SCL_SECURE_TRAITS_* validation macros are almost identical
   to the _SCL_SECURE_* macros defined above. The only difference is the:
        
        if (_SECURE_VALIDATION) { }

   statement, which will statically evaluate to true or false. With optimizations on, the
   compiler will optimize the check away.
 */
 #define _SECURE_VALIDATION _Secure_validation

 #if _SECURE_SCL

 #define _SECURE_VALIDATION_DEFAULT true

 #define _SCL_SECURE_TRAITS_VALIDATE(cond)		\
	{											\
		__pragma(warning(push));				\
		__pragma(warning(disable: 4127));		\
		if (_SECURE_VALIDATION)					\
		{										\
			_SCL_SECURE_VALIDATE(cond)			\
		}										\
		__pragma(warning(pop));					\
	}

 #define _SCL_SECURE_TRAITS_VALIDATE_RANGE(cond) \
	{											\
		__pragma(warning(push));				\
		__pragma(warning(disable: 4127));		\
		if (_SECURE_VALIDATION)					\
		{										\
			_SCL_SECURE_VALIDATE_RANGE(cond)	\
		}										\
		__pragma(warning(pop));					\
	}

 #define _SCL_SECURE_TRAITS_INVALID_ARGUMENT	\
	{											\
		__pragma(warning(push));				\
		__pragma(warning(disable: 4127));		\
		if (_SECURE_VALIDATION)					\
		{										\
			_SCL_SECURE_INVALID_ARGUMENT		\
		}										\
		__pragma(warning(pop));					\
	}

 #define _SCL_SECURE_TRAITS_OUT_OF_RANGE		\
	{											\
		__pragma(warning(push));				\
		__pragma(warning(disable: 4127));		\
		if (_SECURE_VALIDATION)					\
		{										\
			_SCL_SECURE_OUT_OF_RANGE			\
		}										\
		__pragma(warning(pop));					\
	}

 #else

 #define _SECURE_VALIDATION_DEFAULT false

 #define _SCL_SECURE_TRAITS_VALIDATE(cond)
 #define _SCL_SECURE_TRAITS_VALIDATE_RANGE(cond)

 #define _SCL_SECURE_TRAITS_INVALID_ARGUMENT
 #define _SCL_SECURE_TRAITS_OUT_OF_RANGE

 #endif

#if __STDC_WANT_SECURE_LIB__
#define _CRT_SECURE_MEMCPY(dest, destsize, source, count) ::memcpy_s((dest), (destsize), (source), (count))
#define _CRT_SECURE_MEMMOVE(dest, destsize, source, count) ::memmove_s((dest), (destsize), (source), (count))
#define _CRT_SECURE_WMEMCPY(dest, destsize, source, count) ::wmemcpy_s((dest), (destsize), (source), (count))
#define _CRT_SECURE_WMEMMOVE(dest, destsize, source, count) ::wmemmove_s((dest), (destsize), (source), (count))
#else
#define _CRT_SECURE_MEMCPY(dest, destsize, source, count) ::memcpy((dest), (source), (count))
#define _CRT_SECURE_MEMMOVE(dest, destsize, source, count) ::memmove((dest), (source), (count))
#define _CRT_SECURE_WMEMCPY(dest, destsize, source, count) ::wmemcpy((dest), (source), (count))
#define _CRT_SECURE_WMEMMOVE(dest, destsize, source, count) ::wmemmove((dest), (source), (count))
#endif

#include <use_ansi.h>

#ifndef _VC6SP2
 #define _VC6SP2	0 /* define as 1 to fix linker errors with V6.0 SP2 */
#endif /* _VC6SP2 */

/* Define _CRTIMP2 */
 #ifndef _CRTIMP2
  #ifdef  CRTDLL2
   #define _CRTIMP2	__declspec(dllexport)
  #else   /* ndef CRTDLL2 */

   #if defined(_DLL) && !defined(_STATIC_CPPLIB)
    #define _CRTIMP2	__declspec(dllimport)

   #else   /* ndef _DLL && !STATIC_CPPLIB */
    #define _CRTIMP2
   #endif  /* _DLL && !STATIC_CPPLIB */

  #endif  /* CRTDLL2 */
 #endif  /* _CRTIMP2 */

/* Define _CRTIMP2_NCEEPURE */
 #ifndef _CRTIMP2_NCEEPURE
  #if defined(_M_CEE_PURE)
   #define _CRTIMP2_NCEEPURE
  #else
   #define _CRTIMP2_NCEEPURE _CRTIMP2
  #endif
 #endif

#ifndef _MRTIMP
  #ifdef MRTDLL
    #if !defined(_M_CEE_PURE)
      #define _MRTIMP __declspec(dllexport)
    #else
      #define _MRTIMP   
    #endif
  #else   /* ndef MRTDLL */
    #define _MRTIMP __declspec(dllimport)
  #endif  /* MRTDLL */
#endif  /* _MRTIMP */

/* Define _MRTIMP2 */
 #ifndef _MRTIMP2
  #if   defined(CRTDLL2)
   #define _MRTIMP2	__declspec(dllexport)
  #elif defined(MRTDLL)
    #define _MRTIMP2 _MRTIMP
  #else   /* ndef CRTDLL2 */

   #if defined(_DLL) && !defined(_STATIC_CPPLIB)
    #define _MRTIMP2	__declspec(dllimport)

   #else   /* ndef _DLL && !STATIC_CPPLIB */
    #define _MRTIMP2
   #endif  /* _DLL && !STATIC_CPPLIB */

  #endif  /* CRTDLL2 */
 #endif  /* _MRTIMP2 */

/* Define _MRTIMP2_NPURE */
 #ifndef _MRTIMP2_NPURE
  #if   defined(MRTDLL)
    #if !defined(_M_CEE_PURE)
      #define _MRTIMP2_NPURE __declspec(dllexport)
    #else
      #define _MRTIMP2_NPURE
    #endif
  #else   /* ndef MRTDLL */

   #if defined(_DLL) && defined(_M_CEE_PURE)
    #define _MRTIMP2_NPURE	__declspec(dllimport)

   #else
    #define _MRTIMP2_NPURE
   #endif

  #endif  /* MRTDLL */
 #endif  /* _MRTIMP2_NPURE */

/* Define _MRTIMP2_NCEE */
 #ifndef _MRTIMP2_NCEE
  #if defined(_M_CEE)
   #define _MRTIMP2_NCEE
  #else
   #define _MRTIMP2_NCEE _MRTIMP2
  #endif
 #endif

/* Define _MRTIMP2_NCEEPURE */
 #ifndef _MRTIMP2_NCEEPURE
  #if defined(_M_CEE_PURE)
   #define _MRTIMP2_NCEEPURE
  #else
   #define _MRTIMP2_NCEEPURE _MRTIMP2
  #endif
 #endif

/* Define _MRTIMP2_NPURE_NCEEPURE */
 #ifndef _MRTIMP2_NPURE_NCEEPURE
  #if defined(_M_CEE_PURE)
   #define _MRTIMP2_NPURE_NCEEPURE
  #else
   #define _MRTIMP2_NPURE_NCEEPURE _MRTIMP2_NPURE
  #endif
 #endif

 #if defined(_DLL) && !defined(_STATIC_CPPLIB) && !defined(_M_CEE_PURE)
  #define _DLL_CPPLIB
 #endif

 #ifndef _CRTIMP2_PURE
  #if   defined(MRTDLL)
   #define _CRTIMP2_PURE
  #else
   #ifdef  _M_CEE_PURE
     #define _CRTIMP2_PURE
   #else
     #define _CRTIMP2_PURE _CRTIMP2
   #endif
  #endif
 #endif

#if !defined(_CRTDATA2)
  #if !defined(MRTDLL)
    #define _CRTDATA2 _CRTIMP2
  #else
    #define _CRTDATA2    
  #endif
#endif

  #ifndef _INTERNAL_IFSTRIP_
/* Define _CRTBLD_NATIVE_WCHAR_T */

   #if defined(_CRTBLD) && defined(__cplusplus)
    #ifndef _NATIVE_WCHAR_T_DEFINED
     #error Native wchar_t must be defined

    #else /* _NATIVE_WCHAR_T_DEFINED */
     #define _CRTBLD_NATIVE_WCHAR_T
    #endif /* _NATIVE_WCHAR_T_DEFINED */

   #endif /* defined(_CRTBLD) && defined(__cplusplus) */
  #endif /* _INTERNAL_IFSTRIP_ */

  #define _DEPRECATED	_CRT_DEPRECATE_TEXT("This is an obsolete part of the Standard C++ Library Implementation. Do not use it.")


  #ifndef _INTERNAL_IFSTRIP_
/* These functions are for enabling STATIC_CPPLIB functionality */
   #define _cpp_stdin  (&(__iob_func())[0])
   #define _cpp_stdout (&(__iob_func())[1])
   #define _cpp_stderr (&(__iob_func())[2])
   #define _cpp_isleadbyte(c) (__pctype_func()[(unsigned char)(c)] & _LEADBYTE)
  #endif

		/* NAMESPACE */

 #if defined(__cplusplus)
  #define _STD_BEGIN	namespace std {
  #define _STD_END		}
  #define _STD	::std::

/*
We use the stdext (standard extension) namespace to contain extensions that are not part of the current standard
*/
  #define _STDEXT_BEGIN	    namespace stdext {
  #define _STDEXT_END		}
  #define _STDEXT	        ::stdext::

  #ifdef _STD_USING
   #define _C_STD_BEGIN	namespace std {	/* only if *.c compiled as C++ */
   #define _C_STD_END	}
   #define _CSTD	::std::

  #else /* _STD_USING */
/* #define _GLOBAL_USING	*.h in global namespace, c* imports to std */

   #define _C_STD_BEGIN
   #define _C_STD_END
   #define _CSTD	::
  #endif /* _STD_USING */

  #define _C_LIB_DECL		extern "C" {	/* C has extern "C" linkage */
  #define _END_C_LIB_DECL	}
  #define _EXTERN_C			extern "C" {
  #define _END_EXTERN_C		}

 #else /* __cplusplus */
  #define _STD_BEGIN
  #define _STD_END
  #define _STD

  #define _C_STD_BEGIN
  #define _C_STD_END
  #define _CSTD

  #define _C_LIB_DECL
  #define _END_C_LIB_DECL
  #define _EXTERN_C
  #define _END_EXTERN_C
 #endif /* __cplusplus */

 #define _Restrict	restrict

 #ifdef __cplusplus
_STD_BEGIN
typedef bool _Bool;
_STD_END
 #endif /* __cplusplus */

		/* VC++ COMPILER PARAMETERS */
#define _LONGLONG	__int64
#define _ULONGLONG	unsigned __int64
#define _LLONG_MAX	0x7fffffffffffffff
#define _ULLONG_MAX	0xffffffffffffffff

		/* INTEGER PROPERTIES */
#define _C2			1	/* 0 if not 2's complement */

#define _MAX_EXP_DIG	8	/* for parsing numerics */
#define _MAX_INT_DIG	32
#define _MAX_SIG_DIG	36

typedef _LONGLONG _Longlong;
typedef _ULONGLONG _ULonglong;

		/* STDIO PROPERTIES */
#define _Filet _iobuf

 #ifndef _FPOS_T_DEFINED
  #define _FPOSOFF(fp)	((long)(fp))
 #endif /* _FPOS_T_DEFINED */

#define _IOBASE	_base
#define _IOPTR	_ptr
#define _IOCNT	_cnt

		/* MULTITHREAD PROPERTIES */
		/* LOCK MACROS */
#define _LOCK_LOCALE	0
#define _LOCK_MALLOC	1
#define _LOCK_STREAM	2
#define _LOCK_DEBUG		3
#define _MAX_LOCK		4	/* one more than highest lock number */

 #ifdef __cplusplus
_STD_BEGIN
		// CLASS _Lockit

// warning 4412 is benign here
#pragma warning(push)
#pragma warning(disable:4412)
class _CRTIMP2_PURE _Lockit
	{	// lock while object in existence -- MUST NEST
public:
  #if _MULTI_THREAD

    #if defined(_M_CEE_PURE) || defined(MRTDLL)
	explicit __CLR_OR_THIS_CALL _Lockit()	// set default lock
        : _Locktype(0)
    {
        _Lockit_ctor(this);
    }
	explicit __CLR_OR_THIS_CALL _Lockit(int _Kind)	// set the lock
    {
        _Lockit_ctor(this, _Kind);
    }
	__CLR_OR_THIS_CALL ~_Lockit()	// clear the lock
    {
        _Lockit_dtor(this);
    }
    #else
    explicit __thiscall _Lockit();	// set default lock
	explicit __thiscall _Lockit(int);	// set the lock
	__thiscall ~_Lockit();	// clear the lock
    #endif

    static _MRTIMP2_NPURE void __cdecl _Lockit_ctor(int);
    static _MRTIMP2_NPURE void __cdecl _Lockit_dtor(int);

private:
    static _MRTIMP2_NPURE void __cdecl _Lockit_ctor(_Lockit *);
    static _MRTIMP2_NPURE void __cdecl _Lockit_ctor(_Lockit *, int);
    static _MRTIMP2_NPURE void __cdecl _Lockit_dtor(_Lockit *);

	__CLR_OR_THIS_CALL _Lockit(const _Lockit&);				// not defined
	_Lockit& __CLR_OR_THIS_CALL operator=(const _Lockit&);	// not defined

	int _Locktype;

  #else /* _MULTI_THREAD */

	explicit _Lockit()
		{	// do nothing
		}

	explicit _Lockit(int)
		{	// do nothing
		}

	~_Lockit()
		{	// do nothing
		}
  #endif /* _MULTI_THREAD */

	};

#ifdef _M_CEE

class _CRTIMP2_PURE _EmptyLockit
	{	// empty lock class used for bin compat
public:
  #if _MULTI_THREAD

private:
	int _Locktype;

  #else /* _MULTI_THREAD */

  #endif /* _MULTI_THREAD */
	};

#if defined(__cplusplus_cli)
 #define _M_CEE_FINALLY finally
#else
 #define _M_CEE_FINALLY __finally
#endif

#define _BEGIN_LOCK(_Kind) \
	{ \
		typedef int _TmpTestType; \
		__if_exists(_TmpTestType::ToString) \
		{ \
		bool _MustReleaseLock = false; \
		int _LockKind = _Kind; \
		System::Runtime::CompilerServices::RuntimeHelpers::PrepareConstrainedRegions(); \
		try \
		} \
		{ \
			__if_exists(_TmpTestType::ToString) \
			{ \
			System::Runtime::CompilerServices::RuntimeHelpers::PrepareConstrainedRegions(); \
			try { } _M_CEE_FINALLY \
			{ \
				_STD _Lockit::_Lockit_ctor(_LockKind); \
				_MustReleaseLock = true; \
			} \
			} \
			__if_not_exists(_TmpTestType::ToString) \
			{ \
			_STD _Lockit _Lock(_Kind); \
			}

#define _END_LOCK() \
		} \
		__if_exists(_TmpTestType::ToString) \
		{ \
		_M_CEE_FINALLY \
		{ \
			if (_MustReleaseLock) \
			{ \
				_STD _Lockit::_Lockit_dtor(_LockKind); \
			} \
		} \
		} \
	}

#define _BEGIN_LOCINFO(_VarName) \
	_BEGIN_LOCK(_LOCK_LOCALE) \
	_Locinfo _VarName;

#define _END_LOCINFO() \
	_END_LOCK() \

#define _RELIABILITY_CONTRACT \
	[System::Runtime::ConstrainedExecution::ReliabilityContract( \
		System::Runtime::ConstrainedExecution::Consistency::WillNotCorruptState, \
		System::Runtime::ConstrainedExecution::Cer::Success)]

#else

#define _BEGIN_LOCK(_Kind) \
	{ \
		_STD _Lockit _Lock(_Kind);

#define _END_LOCK() \
	}

#define _BEGIN_LOCINFO(_VarName) \
	{ \
		_Locinfo _VarName;

#define _END_LOCINFO() \
	}

#define _RELIABILITY_CONTRACT

#endif

class _CRTIMP2_PURE _Mutex
	{	// lock under program control
public:

  #if _MULTI_THREAD
    
    #if defined(_M_CEE_PURE) || defined(MRTDLL)
	__CLR_OR_THIS_CALL _Mutex()
    {
        _Mutex_ctor(this);
    }
	__CLR_OR_THIS_CALL ~_Mutex()
    {
        _Mutex_dtor(this);
    }
	void __CLR_OR_THIS_CALL _Lock()
    {
        _Mutex_Lock(this);
    }
	void __CLR_OR_THIS_CALL _Unlock()
    {
        _Mutex_Unlock(this);
    }
    #else
    __thiscall _Mutex();
	__thiscall ~_Mutex();
	void __thiscall _Lock();
	void __thiscall _Unlock();
    #endif

private:
    static _MRTIMP2_NPURE_NCEEPURE void __CLRCALL_PURE_OR_CDECL _Mutex_ctor(_Mutex *);
    static _MRTIMP2_NPURE_NCEEPURE void __CLRCALL_PURE_OR_CDECL _Mutex_dtor(_Mutex *);
    static _MRTIMP2_NPURE_NCEEPURE void __CLRCALL_PURE_OR_CDECL _Mutex_Lock(_Mutex *);
    static _MRTIMP2_NPURE_NCEEPURE void __CLRCALL_PURE_OR_CDECL _Mutex_Unlock(_Mutex *);

	__CLR_OR_THIS_CALL _Mutex(const _Mutex&);				// not defined
	_Mutex& __CLR_OR_THIS_CALL operator=(const _Mutex&);	// not defined
	void *_Mtx;

  #else /* _MULTI_THREAD */
    void _Lock()
		{	// do nothing
		}

	void _Unlock()
		{	// do nothing
		}
  #endif /* _MULTI_THREAD */

	};

class _CRTIMP2_PURE _Init_locks
	{	// initialize mutexes
public:

 #if _MULTI_THREAD
    
    #if defined(_M_CEE_PURE) || defined(MRTDLL)
	__CLR_OR_THIS_CALL _Init_locks()
    {
        _Init_locks_ctor(this);
    }
	__CLR_OR_THIS_CALL ~_Init_locks()
    {
        _Init_locks_dtor(this);
    }
    #else
    __thiscall _Init_locks();
	__thiscall ~_Init_locks();
    #endif

private:
    static _MRTIMP2_NPURE void __cdecl _Init_locks_ctor(_Init_locks *);
    static _MRTIMP2_NPURE void __cdecl _Init_locks_dtor(_Init_locks *);

 #else /* _MULTI_THREAD */
	_Init_locks()
		{	// do nothing
		}

	~_Init_locks()
		{	// do nothing
		}
 #endif /* _MULTI_THREAD */

	};
#pragma warning(pop)
_STD_END
 #endif /* __cplusplus */

#ifndef _RELIABILITY_CONTRACT
#define _RELIABILITY_CONTRACT
#endif

		/* MISCELLANEOUS MACROS AND TYPES */
_C_STD_BEGIN
_MRTIMP2 void __cdecl _Atexit(void (__cdecl *)(void));

typedef int _Mbstatet;

#define _ATEXIT_T	void
#define _Mbstinit(x)	mbstate_t x = {0}
_C_STD_END

  #define _EXTERN_TEMPLATE	template
  #define _THROW_BAD_ALLOC	_THROW1(...)

#ifdef  _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif /* _YVALS */


/*
 * Copyright (c) 1992-2005 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V4.05:0009 */
