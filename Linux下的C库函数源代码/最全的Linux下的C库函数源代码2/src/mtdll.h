/***
*mtdll.h - DLL/Multi-thread include
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*
*       [Internal]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#ifndef _INC_MTDLL
#define _INC_MTDLL

#include <crtdefs.h>
#include <limits.h>

#ifdef _MSC_VER
#pragma pack(push,_CRT_PACKING)
#endif  /* _MSC_VER */

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include <windows.h>


/*
 * Define the number of supported handles and streams. The definitions
 * here must exactly match those in internal.h (for _NHANDLE_) and stdio.h
 * (for _NSTREAM_).
 */

#define _IOB_ENTRIES    20

/* Lock symbols */

#define _SIGNAL_LOCK    0       /* lock for signal()                */
#define _IOB_SCAN_LOCK  1       /* _iob[] table lock                */
#define _TMPNAM_LOCK    2       /* lock global tempnam variables    */
#define _CONIO_LOCK     3       /* lock for conio routines          */
#define _HEAP_LOCK      4       /* lock for heap allocator routines */
#define _UNDNAME_LOCK   5       /* lock for unDName() routine       */
#define _TIME_LOCK      6       /* lock for time functions          */
#define _ENV_LOCK       7       /* lock for environment variables   */
#define _EXIT_LOCK1     8       /* lock #1 for exit code            */
#define _POPEN_LOCK     9       /* lock for _popen/_pclose database */
#define _LOCKTAB_LOCK   10      /* lock to protect semaphore lock table */
#define _OSFHND_LOCK    11      /* lock to protect _osfhnd array    */
#define _SETLOCALE_LOCK 12      /* lock for locale handles, etc.    */
#define _MB_CP_LOCK     13      /* lock for multibyte code page     */
#define _TYPEINFO_LOCK  14      /* lock for type_info access        */
#define _DEBUG_LOCK     15      /* lock for debug global structs    */

#define _STREAM_LOCKS   16      /* Table of stream locks            */

#define _LAST_STREAM_LOCK  (_STREAM_LOCKS+_IOB_ENTRIES-1) /* Last stream lock */

#define _TOTAL_LOCKS        (_LAST_STREAM_LOCK+1)

#define _LOCK_BIT_INTS     (_TOTAL_LOCKS/(sizeof(unsigned)*8))+1   /* # of ints to hold lock bits */

#ifndef __assembler

/* Multi-thread macros and prototypes */


#define __TRY __try{
#define __FINALLY   }__finally{
#define __END_TRY_FINALLY }


#ifndef _THREADMBCINFO
typedef struct threadmbcinfostruct threadmbcinfo;
#endif  /* _THREADMBCINFO */

#ifndef _TAGLC_ID_DEFINED
typedef struct tagLC_ID {
        unsigned short wLanguage;
        unsigned short wCountry;
        unsigned short wCodePage;
} LC_ID, *LPLC_ID;
#define _TAGLC_ID_DEFINED
#endif  /* _TAGLC_ID_DEFINED */

#define MAX_LANG_LEN        64  /* max language name length */
#define MAX_CTRY_LEN        64  /* max country name length */
#define MAX_MODIFIER_LEN    0   /* max modifier name length - n/a */
#define MAX_LC_LEN          (MAX_LANG_LEN+MAX_CTRY_LEN+MAX_MODIFIER_LEN+3)
                                /* max entire locale string length */
#define MAX_CP_LEN          16  /* max code page name length */
#define CATNAMES_LEN        57  /* "LC_COLLATE=;LC_CTYPE=;..." length */

#define LC_INT_TYPE         0
#define LC_STR_TYPE         1

#ifndef _SETLOC_STRUCT_DEFINED
struct _is_ctype_compatible {
        unsigned long id;
        int is_clike;
};
typedef struct setloc_struct {
    /* getqloc static variables */
    char *pchLanguage;
    char *pchCountry;
    int iLcidState;
    int iPrimaryLen;
    BOOL bAbbrevLanguage;
    BOOL bAbbrevCountry;
    LCID lcidLanguage;
    LCID lcidCountry;
    /* expand_locale static variables */
    LC_ID       _cacheid;
    UINT        _cachecp;
    char        _cachein[MAX_LC_LEN];
    char        _cacheout[MAX_LC_LEN];
    /* _setlocale_set_cat (LC_CTYPE) static variable */
    struct _is_ctype_compatible _Lcid_c[5];
} _setloc_struct, *_psetloc_struct;
#define _SETLOC_STRUCT_DEFINED
#endif  /* _SETLOC_STRUCT_DEFINED */

_CRTIMP extern unsigned long __cdecl __threadid(void);
#define _threadid   (__threadid())
_CRTIMP extern uintptr_t __cdecl __threadhandle(void);
#define _threadhandle   (__threadhandle())

/* Structure for each thread's data */

struct _tiddata {
    unsigned long   _tid;       /* thread ID */


    uintptr_t _thandle;         /* thread handle */

    int     _terrno;            /* errno value */
    unsigned long   _tdoserrno; /* _doserrno value */
    unsigned int    _fpds;      /* Floating Point data segment */
    unsigned long   _holdrand;  /* rand() seed value */
    char *      _token;         /* ptr to strtok() token */
    wchar_t *   _wtoken;        /* ptr to wcstok() token */
    unsigned char * _mtoken;    /* ptr to _mbstok() token */

    /* following pointers get malloc'd at runtime */
    char *      _errmsg;        /* ptr to strerror()/_strerror() buff */
    wchar_t *   _werrmsg;       /* ptr to _wcserror()/__wcserror() buff */
    char *      _namebuf0;      /* ptr to tmpnam() buffer */
    wchar_t *   _wnamebuf0;     /* ptr to _wtmpnam() buffer */
    char *      _namebuf1;      /* ptr to tmpfile() buffer */
    wchar_t *   _wnamebuf1;     /* ptr to _wtmpfile() buffer */
    char *      _asctimebuf;    /* ptr to asctime() buffer */
    wchar_t *   _wasctimebuf;   /* ptr to _wasctime() buffer */
    void *      _gmtimebuf;     /* ptr to gmtime() structure */
    char *      _cvtbuf;        /* ptr to ecvt()/fcvt buffer */
    unsigned char _con_ch_buf[MB_LEN_MAX];
                                /* ptr to putch() buffer */
    unsigned short _ch_buf_used;   /* if the _con_ch_buf is used */

    /* following fields are needed by _beginthread code */
    void *      _initaddr;      /* initial user thread address */
    void *      _initarg;       /* initial user thread argument */

    /* following three fields are needed to support signal handling and
     * runtime errors */
    void *      _pxcptacttab;   /* ptr to exception-action table */
    void *      _tpxcptinfoptrs; /* ptr to exception info pointers */
    int         _tfpecode;      /* float point exception code */

    /* pointer to the copy of the multibyte character information used by
     * the thread */
    pthreadmbcinfo  ptmbcinfo;

    /* pointer to the copy of the locale informaton used by the thead */
    pthreadlocinfo  ptlocinfo;
    int         _ownlocale;     /* if 1, this thread owns its own locale */

    /* following field is needed by NLG routines */
    unsigned long   _NLG_dwCode;

    /*
     * Per-Thread data needed by C++ Exception Handling
     */
    void *      _terminate;     /* terminate() routine */
    void *      _unexpected;    /* unexpected() routine */
    void *      _translator;    /* S.E. translator */
    void *      _purecall;      /* called when pure virtual happens */
    void *      _curexception;  /* current exception */
    void *      _curcontext;    /* current exception context */
    int         _ProcessingThrow; /* for uncaught_exception */
    void *              _curexcspec;    /* for handling exceptions thrown from std::unexpected */
#if defined (_M_IA64) || defined (_M_AMD64)
    void *      _pExitContext;
    void *      _pUnwindContext;
    void *      _pFrameInfoChain;
    unsigned __int64    _ImageBase;
#if defined (_M_IA64)
    unsigned __int64    _TargetGp;
#endif  /* defined (_M_IA64) */
    unsigned __int64    _ThrowImageBase;
    void *      _pForeignException;
#elif defined (_M_IX86)
    void *      _pFrameInfoChain;
#endif  /* defined (_M_IX86) */
    _setloc_struct _setloc_data;

    void *      _encode_ptr;    /* EncodePointer() routine */
    void *      _decode_ptr;    /* DecodePointer() routine */

    void *      _reserved1;     /* nothing */
    void *      _reserved2;     /* nothing */
    void *      _reserved3;     /* nothing */

    int _cxxReThrow;        /* Set to True if it's a rethrown C++ Exception */

    unsigned long __initDomain;     /* initial domain used by _beginthread[ex] for managed function */
};

typedef struct _tiddata * _ptiddata;

/*
 * Declaration of TLS index used in storing pointers to per-thread data
 * structures.
 */
extern unsigned long __flsindex;
extern unsigned long __getvalueindex;

/* macros */

#define _lock_fh(fh)            __lock_fhandle(fh)
#define _lock_str(s)            _lock_file(s)
#define _lock_str2(i,s)         _lock_file2(i,s)
#define _mlock(l)               _lock(l)
#define _munlock(l)             _unlock(l)
#define _unlock_fh(fh)          _unlock_fhandle(fh)
#define _unlock_str(s)          _unlock_file(s)
#define _unlock_str2(i,s)       _unlock_file2(i,s)

/* multi-thread routines */

#ifdef _M_CEE
#ifndef _CRT_MSVCR80
#ifdef _DEBUG
   #define _CRT_MSVCR80 "MSVCR80D.dll"
#else  /* _DEBUG */
   #define _CRT_MSVCR80 "MSVCR80.dll"
#endif  /* _DEBUG */
#endif  /* _CRT_MSVCR80 */
 #define _INTEROPSERVICES_DLLIMPORT(_DllName , _EntryPoint , _CallingConvention) \
    [System::Runtime::InteropServices::DllImport(_DllName , EntryPoint = _EntryPoint, CallingConvention = _CallingConvention)]
 #define _SUPPRESS_UNMANAGED_CODE_SECURITY [System::Security::SuppressUnmanagedCodeSecurity]
 #define _CALLING_CONVENTION_CDECL System::Runtime::InteropServices::CallingConvention::Cdecl
 #define _RELIABILITY_CONTRACT \
    [System::Runtime::ConstrainedExecution::ReliabilityContract( \
        System::Runtime::ConstrainedExecution::Consistency::WillNotCorruptState, \
        System::Runtime::ConstrainedExecution::Cer::Success)]
#else  /* _M_CEE */
 #define _INTEROPSERVICES_DLLIMPORT(_DllName , _EntryPoint , _CallingConvention)
 #define _SUPPRESS_UNMANAGED_CODE_SECURITY
 #define _CALLING_CONVENTION_CDECL
 #define _RELIABILITY_CONTRACT
#endif  /* _M_CEE */

_SUPPRESS_UNMANAGED_CODE_SECURITY
_RELIABILITY_CONTRACT
_INTEROPSERVICES_DLLIMPORT(_CRT_MSVCR80, "_lock", _CALLING_CONVENTION_CDECL)
void __cdecl _lock(__in int _File);
void __cdecl _lock_file2(__in int _Index, __inout void * _File);
void __cdecl _lockexit(void);
_SUPPRESS_UNMANAGED_CODE_SECURITY
_RELIABILITY_CONTRACT
_INTEROPSERVICES_DLLIMPORT(_CRT_MSVCR80, "_unlock", _CALLING_CONVENTION_CDECL)
void __cdecl _unlock(__inout int _File);
void __cdecl _unlock_file2(__in int _Index, __inout void * _File);
void __cdecl _unlockexit(void);
int  __cdecl _mtinitlocknum(__in int _LockNum);

_ptiddata __cdecl _getptd(void);  /* return address of per-thread CRT data */
_ptiddata __cdecl _getptd_noexit(void);  /* return address of per-thread CRT data - doesn't exit on malloc failure */
_CRTIMP void WINAPI _freefls(__inout_opt void * _PerFiberData);         /* free up per-fiber CRT data block */
void __cdecl _freeptd(__inout_opt _ptiddata _Ptd); /* free up a per-thread CRT data block */
_CRTIMP void __cdecl _initptd(__inout _ptiddata _Ptd,__in_opt pthreadlocinfo _Locale); /* initialize a per-thread CRT data block */

/*
Define Fiber Local Storage function prototypes and access macros.
*/

_CRTIMP PVOID WINAPI __fls_getvalue ( IN DWORD dwFlsIndex);
_CRTIMP BOOL WINAPI __fls_setvalue ( IN DWORD dwFlsIndex, IN PVOID lpFlsData);
_CRTIMP unsigned long __cdecl __get_flsindex(void);
_CRTIMP void __cdecl __set_flsgetvalue(void);

typedef
VOID
(WINAPI *PFLS_CALLBACK_FUNCTION) (
    IN PVOID lpFlsData
    );

typedef
DWORD
(WINAPI *PFLS_ALLOC_FUNCTION) (
    IN PFLS_CALLBACK_FUNCTION lpCallback OPTIONAL
    );

typedef
PVOID
(WINAPI *PFLS_GETVALUE_FUNCTION) (
    IN DWORD dwFlsIndex
    );

typedef
BOOL
(WINAPI *PFLS_SETVALUE_FUNCTION) (
    IN DWORD dwFlsIndex,
    IN PVOID lpFlsData
    );

typedef
BOOL
(WINAPI *PFLS_FREE_FUNCTION) (
    IN DWORD dwFlsIndex
    );

#if defined (_M_AMD64)

#define FLS_ALLOC(callback) (FlsAlloc(callback))
#define FLS_GETVALUE(index) (FlsGetValue(index))
#define FLS_SETVALUE(index, value) (FlsSetValue(index, value))
#define FLS_FREE(index)     (FlsFree(index))

#else  /* defined (_M_AMD64) */

extern PFLS_ALLOC_FUNCTION gpFlsAlloc;
extern PFLS_GETVALUE_FUNCTION gpFlsGetValue;
extern PFLS_SETVALUE_FUNCTION gpFlsSetValue;
extern PFLS_FREE_FUNCTION gpFlsFree;

#define FLS_ALLOC(callback)  (((PFLS_ALLOC_FUNCTION)_decode_pointer(gpFlsAlloc))(callback))
#define FLS_GETVALUE    ((PFLS_GETVALUE_FUNCTION)TlsGetValue(__getvalueindex))
#define FLS_SETVALUE(index, value) (((PFLS_SETVALUE_FUNCTION)_decode_pointer(gpFlsSetValue))(index, value))
#define FLS_FREE(index) (((PFLS_FREE_FUNCTION)_decode_pointer(gpFlsFree))(index))

#endif  /* defined (_M_AMD64) */


#endif  /* __assembler */


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#ifdef _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif  /* _INC_MTDLL */
