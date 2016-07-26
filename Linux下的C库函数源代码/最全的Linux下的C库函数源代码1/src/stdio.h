/***
*stdio.h - definitions/declarations for standard I/O routines
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file defines the structures, values, macros, and functions
*       used by the level 2 I/O ("standard I/O") routines.
*       [ANSI/System V]
*
*       [Public]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#ifndef _INC_STDIO
#define _INC_STDIO

#include <crtdefs.h>
#ifdef _MSC_VER
/*
 * Currently, all MS C compilers for Win32 platforms default to 8 byte
 * alignment.
 */
#pragma pack(push,_CRT_PACKING)
#endif  /* _MSC_VER */

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


/* Buffered I/O macros */

#define BUFSIZ  512

#ifndef _INTERNAL_IFSTRIP_
/*
 * Real default size for stdio buffers
 */
#define _INTERNAL_BUFSIZ    4096
#define _SMALL_BUFSIZ       512
#endif  /* _INTERNAL_IFSTRIP_ */

/*
 * Default number of supported streams. _NFILE is confusing and obsolete, but
 * supported anyway for backwards compatibility.
 */
#define _NFILE      _NSTREAM_

#define _NSTREAM_   512

/*
 * Number of entries in _iob[] (declared below). Note that _NSTREAM_ must be
 * greater than or equal to _IOB_ENTRIES.
 */
#define _IOB_ENTRIES 20

#define EOF     (-1)


#ifndef _FILE_DEFINED
struct _iobuf {
        char *_ptr;
        int   _cnt;
        char *_base;
        int   _flag;
        int   _file;
        int   _charbuf;
        int   _bufsiz;
        char *_tmpfname;
        };
typedef struct _iobuf FILE;
#define _FILE_DEFINED
#endif  /* _FILE_DEFINED */


/* Directory where temporary files may be created. */

#define _P_tmpdir   "\\"
#define _wP_tmpdir  L"\\"

/* L_tmpnam = length of string _P_tmpdir
 *            + 1 if _P_tmpdir does not end in "/" or "\", else 0
 *            + 12 (for the filename string)
 *            + 1 (for the null terminator)
 * L_tmpnam_s = length of string _P_tmpdir
 *            + 1 if _P_tmpdir does not end in "/" or "\", else 0
 *            + 16 (for the filename string)
 *            + 1 (for the null terminator)
 */
#define L_tmpnam   (sizeof(_P_tmpdir) + 12)
#if __STDC_WANT_SECURE_LIB__
#define L_tmpnam_s (sizeof(_P_tmpdir) + 16)
#endif  /* __STDC_WANT_SECURE_LIB__ */



/* Seek method constants */

#define SEEK_CUR    1
#define SEEK_END    2
#define SEEK_SET    0


#define FILENAME_MAX    260
#define FOPEN_MAX       20
#define _SYS_OPEN       20
#define TMP_MAX         32767  /* SHRT_MAX */
#if __STDC_WANT_SECURE_LIB__
#define _TMP_MAX_S      2147483647 /* INT_MAX */
#endif  /* __STDC_WANT_SECURE_LIB__ */

/* Define NULL pointer value */
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else  /* __cplusplus */
#define NULL    ((void *)0)
#endif  /* __cplusplus */
#endif  /* NULL */

/* Declare _iob[] array */

#ifndef _STDIO_DEFINED
#ifndef _INTERNAL_IFSTRIP_
/* These functions are for enabling STATIC_CPPLIB functionality */
#if defined (_DLL) && defined (_M_IX86)
/* Retained for compatibility with VC++ 5.0 and earlier versions */
_CRTIMP extern FILE * __cdecl __p__iob(void);
#endif  /* defined (_DLL) && defined (_M_IX86) */
#ifndef _M_CEE_PURE
_CRTIMP extern FILE _iob[];
#endif  /* _M_CEE_PURE */
#endif  /* _INTERNAL_IFSTRIP_ */
_CRTIMP FILE * __cdecl __iob_func(void);
#endif  /* _STDIO_DEFINED */


/* Define file position type */

#ifndef _FPOS_T_DEFINED
#undef _FPOSOFF


#if !__STDC__ && _INTEGRAL_MAX_BITS >= 64   
/* At this point we could switch both to long long, but we won't do that till next version to avoid any potential compat issues */
typedef __int64 fpos_t;
#define _FPOSOFF(fp) ((long)(fp))
#else  /* !__STDC__ && _INTEGRAL_MAX_BITS >= 64    */
typedef long long fpos_t;
#define _FPOSOFF(fp) ((long)(fp))
#endif  /* !__STDC__ && _INTEGRAL_MAX_BITS >= 64    */

#define _FPOS_T_DEFINED
#endif  /* _FPOS_T_DEFINED */

#ifndef _STDSTREAM_DEFINED
#define stdin  (&__iob_func()[0])
#define stdout (&__iob_func()[1])
#define stderr (&__iob_func()[2])
#define _STDSTREAM_DEFINED
#endif  /* _STDSTREAM_DEFINED */

#define _IOREAD         0x0001
#define _IOWRT          0x0002

#define _IOFBF          0x0000
#define _IOLBF          0x0040
#define _IONBF          0x0004

#define _IOMYBUF        0x0008
#define _IOEOF          0x0010
#define _IOERR          0x0020
#define _IOSTRG         0x0040
#define _IORW           0x0080

/* constants used by _set_output_format */
#define _TWO_DIGIT_EXPONENT 0x1

/* Function prototypes */

#ifndef _STDIO_DEFINED

_CRTIMP __checkReturn int __cdecl _filbuf(__inout FILE * _File );
_CRTIMP __checkReturn_opt int __cdecl _flsbuf(__in int _Ch, __inout FILE * _File);

_CRTIMP __checkReturn FILE * __cdecl _fsopen(__in_z const char * _Filename, __in_z const char * _Mode, __in int _ShFlag);

_CRTIMP void __cdecl clearerr(__inout FILE * _File);
_CRTIMP __checkReturn_wat errno_t __cdecl clearerr_s(__inout FILE * _File );
_CRTIMP __checkReturn_opt int __cdecl fclose(__inout FILE * _File);
_CRTIMP __checkReturn_opt int __cdecl _fcloseall(void);

_CRTIMP __checkReturn FILE * __cdecl _fdopen(__in int _FileHandle, __in_z const char * _Mode);

_CRTIMP __checkReturn int __cdecl feof(__in FILE * _File);
_CRTIMP __checkReturn int __cdecl ferror(__in FILE * _File);
_CRTIMP __checkReturn_opt int __cdecl fflush(__inout_opt FILE * _File);
_CRTIMP __checkReturn_opt int __cdecl fgetc(__inout FILE * _File);
_CRTIMP __checkReturn_opt int __cdecl _fgetchar(void);
_CRTIMP __checkReturn_opt int __cdecl fgetpos(__inout FILE * _File , __out fpos_t * _Pos);
_CRTIMP __checkReturn_opt char * __cdecl fgets(__out_ecount_z(_MaxCount) char * _Buf, __in int _MaxCount, __inout FILE * _File);

_CRTIMP __checkReturn int __cdecl _fileno(__in FILE * _File);

#if defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC)
#pragma push_macro("_tempnam")
#undef _tempnam
#endif  /* defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC) */

_CRTIMP __checkReturn char * __cdecl _tempnam(__in_z_opt const char * _DirName, __in_z_opt const char * _FilePrefix);

#if defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC)
#pragma pop_macro("_tempnam")
#endif  /* defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC) */

_CRTIMP __checkReturn_opt int __cdecl _flushall(void);
_CRT_INSECURE_DEPRECATE(fopen_s) _CRTIMP __checkReturn FILE * __cdecl fopen(__in_z const char * _Filename, __in_z const char * _Mode);
#if __STDC_WANT_SECURE_LIB__
_CRTIMP __checkReturn_wat errno_t __cdecl fopen_s(__deref_out_opt FILE ** _File, __in_z const char * _Filename, __in_z const char * _Mode);
#endif  /* __STDC_WANT_SECURE_LIB__ */
_CRTIMP __checkReturn_opt int __cdecl fprintf(__inout FILE * _File, __in_z __format_string const char * _Format, ...);
_CRTIMP __checkReturn_opt int __cdecl fprintf_s(__inout FILE * _File, __in_z __format_string const char * _Format, ...);
_CRTIMP __checkReturn_opt int __cdecl fputc(__in int _Ch, __inout FILE * _File);
_CRTIMP __checkReturn_opt int __cdecl _fputchar(__in int _Ch);
_CRTIMP __checkReturn_opt int __cdecl fputs(__in_z const char * _Str, __inout FILE * _File);
_CRTIMP __checkReturn_opt size_t __cdecl fread(__out_bcount(_ElementSize*_Count) void * _DstBuf, __in size_t _ElementSize, __in size_t _Count, __inout FILE * _File);
_CRTIMP __checkReturn_opt size_t __cdecl fread_s(__out_bcount(_ElementSize*_Count) void * _DstBuf, __in size_t _DstSize, __in size_t _ElementSize, __in size_t _Count, __inout FILE * _File);
_CRT_INSECURE_DEPRECATE(freopen_s) _CRTIMP __checkReturn FILE * __cdecl freopen(__in_z const char * _Filename, __in_z const char * _Mode, __inout FILE * _File);
#if __STDC_WANT_SECURE_LIB__
_CRTIMP __checkReturn_wat errno_t __cdecl freopen_s(__deref_out_opt FILE ** _File, __in_z const char * _Filename, __in_z const char * _Mode, __inout FILE * _OldFile);
#endif  /* __STDC_WANT_SECURE_LIB__ */
_CRT_INSECURE_DEPRECATE(fscanf_s) _CRTIMP __checkReturn_opt int __cdecl fscanf(__inout FILE * _File, __in_z __format_string const char * _Format, ...);
_CRT_INSECURE_DEPRECATE(_fscanf_s_l) _CRTIMP __checkReturn_opt int __cdecl _fscanf_l(__inout FILE * _File, __in_z __format_string const char * _Format, __in_opt _locale_t _Locale, ...);
#if __STDC_WANT_SECURE_LIB__
_CRTIMP __checkReturn_opt int __cdecl fscanf_s(__inout FILE * _File, __in_z __format_string const char * _Format, ...);
#endif  /* __STDC_WANT_SECURE_LIB__ */
_CRTIMP __checkReturn_opt int __cdecl _fscanf_s_l(__inout FILE * _File, __in_z __format_string const char * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP __checkReturn_opt int __cdecl fsetpos(__inout FILE * _File, __in const fpos_t * _Pos);
_CRTIMP __checkReturn_opt int __cdecl fseek(__inout FILE * _File, __in long _Offset, __in int _Origin);
_CRTIMP __checkReturn long __cdecl ftell(__inout FILE * _File);

_CRTIMP __checkReturn_opt int __cdecl _fseeki64(__inout FILE * _File, __in __int64 _Offset, __in int _Origin);
_CRTIMP __checkReturn __int64 __cdecl _ftelli64(__inout FILE * _File);

_CRTIMP __checkReturn_opt size_t __cdecl fwrite(__in_ecount(_Size*_Count) const void * _Str, __in size_t _Size, __in size_t _Count, __inout FILE * _File);
_CRTIMP __checkReturn int __cdecl getc(__inout FILE * _File);
_CRTIMP __checkReturn int __cdecl getchar(void);
_CRTIMP __checkReturn int __cdecl _getmaxstdio(void);
#if __STDC_WANT_SECURE_LIB__
_CRTIMP char * __cdecl gets_s(__out_ecount_z(_Size) char * _Buf, __in rsize_t _Size);
#endif  /* __STDC_WANT_SECURE_LIB__ */
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_0(char *, gets_s, __out_ecount(_Size) char, _Buffer)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_0(char *, __RETURN_POLICY_SAME, _CRTIMP, gets, __out_z char, _Buffer)
__checkReturn int __cdecl _getw(__inout FILE * _File);
#ifndef _CRT_PERROR_DEFINED
#define _CRT_PERROR_DEFINED
_CRTIMP void __cdecl perror(__in_z_opt const char * _ErrMsg);
#endif  /* _CRT_PERROR_DEFINED */
_CRTIMP __checkReturn_opt int __cdecl _pclose(__inout FILE * _File);
_CRTIMP __checkReturn FILE * __cdecl _popen(__in_z const char * _Command, __in_z const char * _Mode);
_CRTIMP __checkReturn_opt int __cdecl printf(__in_z __format_string const char * _Format, ...);
_CRTIMP __checkReturn_opt int __cdecl printf_s(__in_z __format_string const char * _Format, ...);
_CRTIMP __checkReturn_opt int __cdecl putc(__in int _Ch, __inout FILE * _File);
_CRTIMP __checkReturn_opt int __cdecl putchar(__in int _Ch);
_CRTIMP __checkReturn_opt int __cdecl puts(__in_z const char * _Str);
_CRTIMP __checkReturn_opt int __cdecl _putw(__in int _Word, __inout FILE * _File);
#ifndef _CRT_DIRECTORY_DEFINED
#define _CRT_DIRECTORY_DEFINED
_CRTIMP __checkReturn int __cdecl remove(__in_z const char * _Filename);
_CRTIMP __checkReturn int __cdecl rename(__in_z const char * _OldFilename, __in_z const char * _NewFilename);
_CRTIMP __checkReturn int __cdecl _unlink(__in_z const char * _Filename);
#if !__STDC__
_CRT_NONSTDC_DEPRECATE(_unlink) _CRTIMP __checkReturn int __cdecl unlink(__in_z const char * _Filename);
#endif  /* !__STDC__ */
#endif  /* _CRT_DIRECTORY_DEFINED */
_CRTIMP void __cdecl rewind(__inout FILE * _File);
_CRTIMP __checkReturn_opt int __cdecl _rmtmp(void);
_CRT_INSECURE_DEPRECATE(scanf_s) _CRTIMP __checkReturn_opt int __cdecl scanf(__in_z __format_string const char * _Format, ...);
_CRT_INSECURE_DEPRECATE(_scanf_s_l) _CRTIMP __checkReturn_opt int __cdecl _scanf_l(__in_z __format_string const char * _Format, __in_opt _locale_t _Locale, ...);
#if __STDC_WANT_SECURE_LIB__
_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl scanf_s(__in_z __format_string const char * _Format, ...);
#endif  /* __STDC_WANT_SECURE_LIB__ */
_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl _scanf_s_l(__in_z __format_string const char * _Format, __in_opt _locale_t _Locale, ...);
_CRT_INSECURE_DEPRECATE(setvbuf) _CRTIMP void __cdecl setbuf(__inout FILE * _File, __inout_ecount_part_opt(BUFSIZ, 0) char * _Buffer);
_CRTIMP __checkReturn_opt int __cdecl _setmaxstdio(__in int _Max);
_CRTIMP __checkReturn_opt unsigned int __cdecl _set_output_format(__in unsigned int _Format);
_CRTIMP __checkReturn_opt unsigned int __cdecl _get_output_format(void);
_CRTIMP __checkReturn_opt int __cdecl setvbuf(__inout FILE * _File, __inout_bcount_z_opt(_Size) char * _Buf, __in int _Mode, __in size_t _Size);
_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl _snprintf_s(__out_bcount_z(_DstSize) char * _DstBuf, __in size_t _DstSize, __in size_t _MaxCount, __in_z __format_string const char * _Format, ...);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_2_ARGLIST(int, _snprintf_s, _vsnprintf_s, __out_bcount(_Size) char, _Dest, __in size_t, _Size, __in_z __format_string const char *,_Format)
_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl sprintf_s(__out_bcount_z(_DstSize) char * _DstBuf, __in size_t _DstSize, __in_z __format_string const char * _Format, ...);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_1_ARGLIST(int, sprintf_s, vsprintf_s, __out_bcount(_Size) char, _Dest, __in_z __format_string const char *, _Format)
_CRTIMP __checkReturn int __cdecl _scprintf(__in_z __format_string const char * _Format, ...);
_CRT_INSECURE_DEPRECATE(sscanf_s) _CRTIMP __checkReturn_opt int __cdecl sscanf(__in_z const char * _Src, __in_z __format_string const char * _Format, ...);
_CRT_INSECURE_DEPRECATE(_sscanf_s_l) _CRTIMP __checkReturn_opt int __cdecl _sscanf_l(__in_z const char * _Src, __in_z __format_string const char * _Format, __in_opt _locale_t _Locale, ...);
#if __STDC_WANT_SECURE_LIB__
_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl sscanf_s(__in_z const char * _Src, __in_z __format_string const char * _Format, ...);
#endif  /* __STDC_WANT_SECURE_LIB__ */
_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl _sscanf_s_l(__in_z const char * _Src, __in_z __format_string const char * _Format, __in_opt _locale_t _Locale, ...);
_CRT_INSECURE_DEPRECATE(_snscanf_s) _CRTIMP __checkReturn_opt int __cdecl _snscanf(__in_bcount_z(_MaxCount) const char * _Src, __in size_t _MaxCount, __in_z __format_string const char * _Format, ...);
_CRT_INSECURE_DEPRECATE(_snscanf_s_l) _CRTIMP __checkReturn_opt int __cdecl _snscanf_l(__in_bcount_z(_MaxCount) const char * _Src, __in size_t _MaxCount, __in_z __format_string const char * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl _snscanf_s(__in_bcount_z(_MaxCount) const char * _Src, __in size_t _MaxCount, __in_z __format_string const char * _Format, ...);
_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl _snscanf_s_l(__in_bcount_z(_MaxCount) const char * _Src, __in size_t _MaxCount, __in_z __format_string const char * _Format, __in_opt _locale_t _Locale, ...);
_CRT_INSECURE_DEPRECATE(tmpfile_s) _CRTIMP __checkReturn FILE * __cdecl tmpfile(void);
#if __STDC_WANT_SECURE_LIB__
_CRTIMP __checkReturn_wat errno_t __cdecl tmpfile_s(__deref_opt_out FILE ** _File);
_CRTIMP __checkReturn_wat errno_t __cdecl tmpnam_s(__out_ecount_z(_Size) char * _Buf, __in rsize_t _Size);
#endif  /* __STDC_WANT_SECURE_LIB__ */
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_0(errno_t, tmpnam_s, __out_ecount_z(_Size) char, _Buf)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_0(char *, __RETURN_POLICY_DST, _CRTIMP, tmpnam, __out_z_opt char, _Buffer)
_CRTIMP __checkReturn_opt int __cdecl ungetc(__in int _Ch, __inout FILE * _File);
_CRTIMP __checkReturn_opt int __cdecl vfprintf(__inout FILE * _File, __in_z __format_string const char * _Format, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl vfprintf_s(__inout FILE * _File, __in_z __format_string const char * _Format, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl vprintf(__in_z __format_string const char * _Format, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl vprintf_s(__in_z __format_string const char * _Format, va_list _ArgList);
_CRT_INSECURE_DEPRECATE(vsnprintf_s) _CRTIMP __checkReturn_opt int __cdecl vsnprintf(__out_ecount(_MaxCount) char * _DstBuf, __in size_t _MaxCount, __in_z __format_string const char * _Format, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl vsnprintf_s(__out_ecount_z(_DstSize) char * _DstBuf, __in size_t _DstSize, __in size_t _MaxCount, __in_z __format_string const char * _Format, va_list _ArgList);
_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl _vsnprintf_s(__out_ecount_z(_DstSize) char * _DstBuf, __in size_t _DstSize, __in size_t _MaxCount, __in_z __format_string const char * _Format, va_list _ArgList);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_3(int, _vsnprintf_s, __out_ecount(_Size) char, _Dest, __in size_t, _Size, __in_z __format_string const char *, _Format, va_list, _Args)
#pragma warning(push)
#pragma warning(disable:4793)
__DEFINE_CPP_OVERLOAD_STANDARD_NFUNC_0_2_ARGLIST_EX(int, __RETURN_POLICY_SAME, _CRTIMP, _snprintf, _vsnprintf, __out_z char, __out_ecount(_Count) char, _Dest, __in size_t, _Count, __in_z __format_string const char *, _Format)
#pragma warning(pop)
_CRTIMP_ALTERNATIVE int __cdecl vsprintf_s(__out_ecount_z(_Size) char * _DstBuf, __in size_t _Size, __in_z __format_string const char * _Format, va_list _ArgList);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_2(int, vsprintf_s, __out_ecount(_Size) char, _Dest, __in_z __format_string const char *, _Format, va_list, _Args)
#pragma warning(push)
#pragma warning(disable:4793)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_1_ARGLIST(int, __RETURN_POLICY_SAME, _CRTIMP, sprintf, vsprintf, __out_z char, _Dest, __in_z __format_string const char *, _Format)
#pragma warning(pop)
_CRTIMP __checkReturn int __cdecl _vscprintf(__in_z __format_string const char * _Format, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl _snprintf_c(__out_ecount(_MaxCount) char * _DstBuf, __in size_t _MaxCount, __in_z __format_string const char * _Format, ...);
_CRTIMP __checkReturn_opt int __cdecl _vsnprintf_c(__out_ecount(_MaxCount) char *_DstBuf, __in size_t _MaxCount, __in_z __format_string const char * _Format, va_list _ArgList);

_CRTIMP __checkReturn_opt int __cdecl _fprintf_p(__inout FILE * _File, __in_z __format_string const char * _Format, ...);
_CRTIMP __checkReturn_opt int __cdecl _printf_p(__in_z __format_string const char * _Format, ...);
_CRTIMP __checkReturn_opt int __cdecl _sprintf_p(__out_ecount_z(_MaxCount) char * _Dst, __in size_t _MaxCount, __in_z __format_string const char * _Format, ...);
_CRTIMP __checkReturn_opt int __cdecl _vfprintf_p(__inout FILE * _File, __in_z __format_string const char * _Format, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl _vprintf_p(__in_z __format_string const char * _Format, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl _vsprintf_p(__out_ecount_z(_MaxCount) char * _Dst, __in size_t _MaxCount, __in_z __format_string const char * _Format, va_list _ArgList);
_CRTIMP __checkReturn int __cdecl _scprintf_p(__in_z __format_string const char * _Format, ...);
_CRTIMP __checkReturn int __cdecl _vscprintf_p(__in_z __format_string const char * _Format, va_list _ArgList);
_CRTIMP int __cdecl _set_printf_count_output(__in int _Value);
_CRTIMP int __cdecl _get_printf_count_output();

_CRTIMP __checkReturn_opt int __cdecl _printf_l(__in_z __format_string const char * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP __checkReturn_opt int __cdecl _printf_p_l(__in_z __format_string const char * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP __checkReturn_opt int __cdecl _printf_s_l(__in_z __format_string const char * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP __checkReturn_opt int __cdecl _vprintf_l(__in_z __format_string const char * _Format, __in_opt _locale_t _Locale, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl _vprintf_p_l(__in_z __format_string const char * _Format, __in_opt _locale_t _Locale, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl _vprintf_s_l(__in_z __format_string const char * _Format, __in_opt _locale_t _Locale, va_list _ArgList);

_CRTIMP __checkReturn_opt int __cdecl _fprintf_l(__inout FILE * _File, __in_z __format_string const char * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP __checkReturn_opt int __cdecl _fprintf_p_l(__inout FILE * _File, __in_z __format_string const char * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP __checkReturn_opt int __cdecl _fprintf_s_l(__inout FILE * _File, __in_z __format_string const char * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP __checkReturn_opt int __cdecl _vfprintf_l(__inout FILE * _File, __in_z const char * _Format, __in_opt _locale_t _Locale, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl _vfprintf_p_l(__inout FILE * _File, __in_z const char * _Format, __in_opt _locale_t _Locale, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl _vfprintf_s_l(__inout FILE * _File, __in_z const char * _Format, __in_opt _locale_t _Locale, va_list _ArgList);

_CRT_INSECURE_DEPRECATE(_sprintf_s_l) _CRTIMP __checkReturn_opt int __cdecl _sprintf_l(__out_z char * _DstBuf, __in_z __format_string const char * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP __checkReturn_opt int __cdecl _sprintf_p_l(__out_ecount_z(_MaxCount) char * _DstBuf, __in size_t _MaxCount, __in_z __format_string const char * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl _sprintf_s_l(__out_bcount_z(_DstSize) char * _DstBuf, __in size_t _DstSize, __in_z __format_string const char * _Format, __in_opt _locale_t _Locale, ...);
_CRT_INSECURE_DEPRECATE(_vsprintf_s_l) _CRTIMP __checkReturn_opt int __cdecl _vsprintf_l(__out_z char * _DstBuf, __in_z const char * _Format, __in_opt _locale_t, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl _vsprintf_p_l(__out_ecount_z(_MaxCount) char * _DstBuf, __in size_t _MaxCount, __in_z __format_string const char* _Format, __in_opt _locale_t _Locale,  va_list _ArgList);
_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl _vsprintf_s_l(__out_ecount_z(_DstSize) char * _DstBuf, __in size_t _DstSize, __in_z __format_string const char * _Format, __in_opt _locale_t _Locale, va_list _ArgList);

_CRTIMP __checkReturn_opt int __cdecl _scprintf_l(__in_z __format_string const char * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP __checkReturn_opt int __cdecl _scprintf_p_l(__in_z __format_string const char * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP __checkReturn_opt int __cdecl _vscprintf_l(__in_z __format_string const char * _Format, __in_opt _locale_t _Locale, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl _vscprintf_p_l(__in_z __format_string const char * _Format, __in_opt _locale_t _Locale, va_list _ArgList);

_CRT_INSECURE_DEPRECATE(_snprintf_s_l) _CRTIMP __checkReturn_opt int __cdecl _snprintf_l(__out_ecount(_MaxCount) char * _DstBuf, __in size_t _MaxCount, __in_z __format_string const char * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP __checkReturn_opt int __cdecl _snprintf_c_l(__out_ecount(_MaxCount) char * _DstBuf, __in size_t _MaxCount, __in_z __format_string const char * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl _snprintf_s_l(__out_ecount_z(_DstSize) char * _DstBuf, __in size_t _DstSize, __in size_t _MaxCount, __in_z __format_string const char * _Format, __in_opt _locale_t _Locale, ...);
_CRT_INSECURE_DEPRECATE(_vsnprintf_s_l) _CRTIMP __checkReturn_opt int __cdecl _vsnprintf_l(__out_ecount(_MaxCount) char * _DstBuf, __in size_t _MaxCount, __in_z __format_string const char * _Format, __in_opt _locale_t _Locale, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl _vsnprintf_c_l(__out_ecount(_MaxCount) char * _DstBuf, __in size_t _MaxCount, const char *, __in_opt _locale_t _Locale, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl _vsnprintf_s_l(__out_ecount_z(_DstSize) char * _DstBuf, __in size_t _DstSize, __in size_t _MaxCount, __in_z __format_string const char* _Format,__in_opt _locale_t _Locale, va_list _ArgList);

#ifndef _WSTDIO_DEFINED

/* wide function prototypes, also declared in wchar.h  */

#ifndef WEOF
#define WEOF (wint_t)(0xFFFF)
#endif  /* WEOF */

_CRTIMP __checkReturn FILE * __cdecl _wfsopen(__in_z const wchar_t * _Filename, __in_z const wchar_t * _Mode, __in int _ShFlag);

_CRTIMP __checkReturn_opt wint_t __cdecl fgetwc(__inout FILE * _File);
_CRTIMP __checkReturn_opt wint_t __cdecl _fgetwchar(void);
_CRTIMP __checkReturn_opt wint_t __cdecl fputwc(__in_z wchar_t _Ch, __inout FILE * _File);
_CRTIMP __checkReturn_opt wint_t __cdecl _fputwchar(__in_z wchar_t _Ch);
_CRTIMP __checkReturn wint_t __cdecl getwc(__inout FILE * _File);
_CRTIMP __checkReturn wint_t __cdecl getwchar(void);
_CRTIMP __checkReturn_opt wint_t __cdecl putwc(__in_z wchar_t _Ch, __inout FILE * _File);
_CRTIMP __checkReturn_opt wint_t __cdecl putwchar(__in_z wchar_t _Ch);
_CRTIMP __checkReturn_opt wint_t __cdecl ungetwc(__in wint_t _Ch, __inout FILE * _File);

_CRTIMP __checkReturn_opt wchar_t * __cdecl fgetws(__out_ecount_z(_SizeInWords) wchar_t * _Dst, __in int _SizeInWords, __inout FILE * _File);
_CRTIMP __checkReturn_opt int __cdecl fputws(__in_z const wchar_t * _Str, __inout FILE * _File);
_CRTIMP __checkReturn_opt wchar_t * __cdecl _getws_s(__out_ecount_z(_SizeInWords) wchar_t * _Str, __in size_t _SizeInWords);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_0(wchar_t *, _getws_s, __out_ecount(_Size) wchar_t, _String)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_0(wchar_t *, __RETURN_POLICY_SAME, _CRTIMP, _getws, __out_z wchar_t, _String)
_CRTIMP __checkReturn_opt int __cdecl _putws(__in_z const wchar_t * _Str);

_CRTIMP __checkReturn_opt int __cdecl fwprintf(__inout FILE * _File, __in_z __format_string const wchar_t * _Format, ...);
_CRTIMP __checkReturn_opt int __cdecl fwprintf_s(__inout FILE * _File, __in_z __format_string const wchar_t * _Format, ...);
_CRTIMP __checkReturn_opt int __cdecl wprintf(__in_z __format_string const wchar_t * _Format, ...);
_CRTIMP __checkReturn_opt int __cdecl wprintf_s(__in_z __format_string const wchar_t * _Format, ...);
_CRTIMP __checkReturn int __cdecl _scwprintf(__in_z __format_string const wchar_t * _Format, ...);
_CRTIMP __checkReturn_opt int __cdecl vfwprintf(__inout FILE * _File, __in_z __format_string const wchar_t * _Format, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl vfwprintf_s(__inout FILE * _File, __in_z __format_string const wchar_t * _Format, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl vwprintf(__in_z __format_string const wchar_t * _Format, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl vwprintf_s(__in_z __format_string const wchar_t * _Format, va_list _ArgList);

_CRTIMP_ALTERNATIVE int __cdecl swprintf_s(__out_ecount_z(_SizeInWords) wchar_t * _Dst, __in size_t _SizeInWords, __in_z __format_string const wchar_t * _Format, ...);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_1_ARGLIST(int, swprintf_s, vswprintf_s, __out_ecount(_Size) wchar_t, _Dest, __in_z __format_string const wchar_t *, _Format)
_CRTIMP_ALTERNATIVE int __cdecl vswprintf_s(__out_ecount_z(_SizeInWords) wchar_t * _Dst, __in size_t _SizeInWords, __in_z __format_string const wchar_t * _Format, va_list _ArgList);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_2(int, vswprintf_s, __out_ecount(_Size) wchar_t, _Dest, __in_z __format_string const wchar_t *, _Format, va_list, _Args)

_CRTIMP __checkReturn_opt int __cdecl _swprintf_c(__out_ecount_z(_SizeInWords) wchar_t * _DstBuf, __in size_t _SizeInWords, __in_z __format_string const wchar_t * _Format, ...);
_CRTIMP __checkReturn_opt int __cdecl _vswprintf_c(__out_ecount_z(_SizeInWords) wchar_t * _DstBuf, __in size_t _SizeInWords, __in_z __format_string const wchar_t * _Format, va_list _ArgList);

_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl _snwprintf_s(__out_ecount_z(_DstSizeInWords) wchar_t * _DstBuf, __in size_t _DstSizeInWords, __in size_t _MaxCount, __in_z __format_string const wchar_t * _Format, ...);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_2_ARGLIST(int, _snwprintf_s, _vsnwprintf_s, __out_ecount(_Size) wchar_t, _Dest, __in size_t, _Count, __in_z __format_string const wchar_t *, _Format)
_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl _vsnwprintf_s(__out_ecount_z(_DstSizeInWords) wchar_t * _DstBuf, __in size_t _DstSizeInWords, __in size_t _MaxCount, __in_z __format_string const wchar_t * _Format, va_list _ArgList);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_3(int, _vsnwprintf_s, __out_ecount(_Size) wchar_t, _Dest, __in size_t, _Count, __in_z __format_string const wchar_t *, _Format, va_list, _Args)
#pragma warning(push)
#pragma warning(disable:4793)
__DEFINE_CPP_OVERLOAD_STANDARD_NFUNC_0_2_ARGLIST_EX(int, __RETURN_POLICY_SAME, _CRTIMP, _snwprintf, _vsnwprintf, __out_z wchar_t, __out_ecount(_Count) wchar_t, _Dest, __in size_t, _Count, __in_z __format_string const wchar_t *, _Format)
#pragma warning(pop)

_CRTIMP __checkReturn_opt int __cdecl _fwprintf_p(__inout FILE * _File, __in_z __format_string const wchar_t * _Format, ...);
_CRTIMP __checkReturn_opt int __cdecl _wprintf_p(__in_z __format_string const wchar_t * _Format, ...);
_CRTIMP __checkReturn_opt int __cdecl _vfwprintf_p(__inout FILE * _File, __in_z __format_string const wchar_t * _Format, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl _vwprintf_p(__in_z __format_string const wchar_t * _Format, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl _swprintf_p(__out_ecount_z(_MaxCount) wchar_t * _DstBuf, __in size_t _MaxCount, __in_z __format_string const wchar_t * _Format, ...);
_CRTIMP __checkReturn_opt int __cdecl _vswprintf_p(__out_ecount_z(_MaxCount) wchar_t * _DstBuf, __in size_t _MaxCount, __in_z __format_string const wchar_t * _Format, va_list _ArgList);
_CRTIMP __checkReturn int __cdecl _scwprintf_p(__in_z __format_string const wchar_t * _Format, ...);
_CRTIMP __checkReturn int __cdecl _vscwprintf_p(__in_z __format_string const wchar_t * _Format, va_list _ArgList);

_CRTIMP __checkReturn_opt int __cdecl _wprintf_l(__in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP __checkReturn_opt int __cdecl _wprintf_p_l(__in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP __checkReturn_opt int __cdecl _wprintf_s_l(__in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP __checkReturn_opt int __cdecl _vwprintf_l(__in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl _vwprintf_p_l(__in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl _vwprintf_s_l(__in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, va_list _ArgList);

_CRTIMP __checkReturn_opt int __cdecl _fwprintf_l(__inout FILE * _File, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP __checkReturn_opt int __cdecl _fwprintf_p_l(__inout FILE * _File, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP __checkReturn_opt int __cdecl _fwprintf_s_l(__inout FILE * _File, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP __checkReturn_opt int __cdecl _vfwprintf_l(__inout FILE * _File, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl _vfwprintf_p_l(__inout FILE * _File, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl _vfwprintf_s_l(__inout FILE * _File, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, va_list _ArgList);

_CRTIMP __checkReturn_opt int __cdecl _swprintf_c_l(__out_ecount_z(_MaxCount) wchar_t * _DstBuf, __in size_t _MaxCount, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP __checkReturn_opt int __cdecl _swprintf_p_l(__out_ecount_z(_MaxCount) wchar_t * _DstBuf, __in size_t _MaxCount, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl _swprintf_s_l(__out_ecount_z(_DstSize) wchar_t * _DstBuf, __in size_t _DstSize, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP __checkReturn_opt int __cdecl _vswprintf_c_l(__out_ecount_z(_MaxCount) wchar_t * _DstBuf, __in size_t _MaxCount, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl _vswprintf_p_l(__out_ecount_z(_MaxCount) wchar_t * _DstBuf, __in size_t _MaxCount, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, va_list _ArgList);
_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl _vswprintf_s_l(__out_ecount_z(_DstSize) wchar_t * _DstBuf, __in size_t _DstSize, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, va_list _ArgList);

_CRTIMP __checkReturn int __cdecl _scwprintf_l(__in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP __checkReturn int __cdecl _scwprintf_p_l(__in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP __checkReturn int __cdecl _vscwprintf_p_l(__in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, va_list _ArgList);

_CRT_INSECURE_DEPRECATE(_snwprintf_s_l) _CRTIMP __checkReturn_opt int __cdecl _snwprintf_l(__out_ecount(_MaxCount) wchar_t * _DstBuf, __in size_t _MaxCount, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl _snwprintf_s_l(__out_ecount_z(_DstSize) wchar_t * _DstBuf, __in size_t _DstSize, __in size_t _MaxCount, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRT_INSECURE_DEPRECATE(_vsnwprintf_s_l) _CRTIMP __checkReturn_opt int __cdecl _vsnwprintf_l(__out_ecount(_MaxCount) wchar_t * _DstBuf, __in size_t _MaxCount, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, va_list _ArgList);
_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl _vsnwprintf_s_l(__out_ecount_z(_DstSize) wchar_t * _DstBuf, __in size_t _DstSize, __in size_t _MaxCount, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, va_list _ArgList);


#ifndef _CRT_NON_CONFORMING_SWPRINTFS

#define _SWPRINTFS_DEPRECATED _CRT_DEPRECATE_TEXT("swprintf has been changed to conform with the ISO C standard, adding an extra character count parameter. To use traditional Microsoft swprintf, set _CRT_NON_CONFORMING_SWPRINTFS.")

#else  /* _CRT_NON_CONFORMING_SWPRINTFS */

#define _SWPRINTFS_DEPRECATED

#endif  /* _CRT_NON_CONFORMING_SWPRINTFS */

/* we could end up with a double deprecation, disable warnings 4141 and 4996 */
#pragma warning(push)
#pragma warning(disable:4141 4996 4793)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_1_ARGLIST_EX(int, __RETURN_POLICY_SAME, _SWPRINTFS_DEPRECATED _CRTIMP, _swprintf, _swprintf_s, _vswprintf, vswprintf_s, __out_z wchar_t, _Dest, __in_z __format_string const wchar_t *, _Format)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_2_ARGLIST_EX(int, __RETURN_POLICY_SAME, _SWPRINTFS_DEPRECATED _CRTIMP, __swprintf_l, __vswprintf_l, _vswprintf_s_l, __out_z wchar_t, __out wchar_t, _Dest, __in_z __format_string const wchar_t *, _Format, _locale_t, _Plocinfo)
#pragma warning(pop)

#if !defined (RC_INVOKED) && !defined (__midl)
#include <swprintf.inl>
#endif  /* !defined (RC_INVOKED) && !defined (__midl) */

#ifdef _CRT_NON_CONFORMING_SWPRINTFS
#ifndef __cplusplus
#define swprintf _swprintf
#define vswprintf _vswprintf
#define _swprintf_l __swprintf_l
#define _vswprintf_l __vswprintf_l
#endif  /* __cplusplus */
#endif  /* _CRT_NON_CONFORMING_SWPRINTFS */

#if defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC)
#pragma push_macro("_wtempnam")
#undef _wtempnam
#endif  /* defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC) */

_CRTIMP __checkReturn wchar_t * __cdecl _wtempnam(__in_z_opt const wchar_t * _Directory, __in_z_opt const wchar_t * _FilePrefix);

#if defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC)
#pragma pop_macro("_wtempnam")
#endif  /* defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC) */

_CRTIMP __checkReturn int __cdecl _vscwprintf(__in_z __format_string const wchar_t * _Format, va_list _ArgList);
_CRTIMP __checkReturn int __cdecl _vscwprintf_l(__in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, va_list _ArgList);
_CRT_INSECURE_DEPRECATE(fwscanf_s) _CRTIMP __checkReturn_opt int __cdecl fwscanf(__inout FILE * _File, __in_z __format_string const wchar_t * _Format, ...);
_CRT_INSECURE_DEPRECATE(_fwscanf_s_l) _CRTIMP __checkReturn_opt int __cdecl _fwscanf_l(__inout FILE * _File, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
#if __STDC_WANT_SECURE_LIB__
_CRTIMP __checkReturn_opt int __cdecl fwscanf_s(__inout FILE * _File, __in_z __format_string const wchar_t * _Format, ...);
#endif  /* __STDC_WANT_SECURE_LIB__ */
_CRTIMP __checkReturn_opt int __cdecl _fwscanf_s_l(__inout FILE * _File, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRT_INSECURE_DEPRECATE(swscanf_s) _CRTIMP __checkReturn_opt int __cdecl swscanf(__in_z const wchar_t * _Src, __in_z __format_string const wchar_t * _Format, ...);
_CRT_INSECURE_DEPRECATE(_swscanf_s_l) _CRTIMP __checkReturn_opt int __cdecl _swscanf_l(__in_z const wchar_t * _Src, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
#if __STDC_WANT_SECURE_LIB__
_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl swscanf_s(__in_z const wchar_t *_Src, __in_z __format_string const wchar_t * _Format, ...);
#endif  /* __STDC_WANT_SECURE_LIB__ */
_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl _swscanf_s_l(__in_z const wchar_t * _Src, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRT_INSECURE_DEPRECATE(_snwscanf_s) _CRTIMP __checkReturn_opt int __cdecl _snwscanf(__in_ecount_z(_MaxCount) const wchar_t * _Src, __in size_t _MaxCount, __in_z __format_string const wchar_t * _Format, ...);
_CRT_INSECURE_DEPRECATE(_snwscanf_s_l) _CRTIMP __checkReturn_opt int __cdecl _snwscanf_l(__in_ecount_z(_MaxCount) const wchar_t * _Src, __in size_t _MaxCount, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl _snwscanf_s(__in_ecount_z(_MaxCount) const wchar_t * _Src, __in size_t _MaxCount, __in_z __format_string const wchar_t * _Format, ...);
_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl _snwscanf_s_l(__in_ecount_z(_MaxCount) const wchar_t * _Src, __in size_t _MaxCount, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRT_INSECURE_DEPRECATE(wscanf_s) _CRTIMP __checkReturn_opt int __cdecl wscanf(__in_z __format_string const wchar_t * _Format, ...);
_CRT_INSECURE_DEPRECATE(_wscanf_s_l) _CRTIMP __checkReturn_opt int __cdecl _wscanf_l(__in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
#if __STDC_WANT_SECURE_LIB__
_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl wscanf_s(__in_z __format_string const wchar_t * _Format, ...);
#endif  /* __STDC_WANT_SECURE_LIB__ */
_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl _wscanf_s_l(__in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);

_CRTIMP __checkReturn FILE * __cdecl _wfdopen(__in int _FileHandle , __in_z const wchar_t * _Mode);
_CRT_INSECURE_DEPRECATE(_wfopen_s) _CRTIMP __checkReturn FILE * __cdecl _wfopen(__in_z const wchar_t * _Filename, __in_z const wchar_t * _Mode);
_CRTIMP __checkReturn_wat errno_t __cdecl _wfopen_s(__deref_out_opt FILE ** _File, __in_z const wchar_t * _Filename, __in_z const wchar_t * _Mode);
_CRT_INSECURE_DEPRECATE(_wfreopen_s) _CRTIMP __checkReturn FILE * __cdecl _wfreopen(__in_z const wchar_t * _Filename, __in_z const wchar_t * _Mode, __inout FILE * _OldFile);
_CRTIMP __checkReturn_wat errno_t __cdecl _wfreopen_s(__deref_out_opt FILE ** _File, __in_z const wchar_t * _Filename, __in_z const wchar_t * _Mode, __inout FILE * _OldFile);

#ifndef _CRT_WPERROR_DEFINED
#define _CRT_WPERROR_DEFINED
_CRTIMP void __cdecl _wperror(__in_z_opt const wchar_t * _ErrMsg);
#endif  /* _CRT_WPERROR_DEFINED */
_CRTIMP __checkReturn FILE * __cdecl _wpopen(__in_z const wchar_t *_Command, __in_z const wchar_t * _Mode);
_CRTIMP __checkReturn int __cdecl _wremove(__in_z const wchar_t * _Filename);
_CRTIMP __checkReturn_wat errno_t __cdecl _wtmpnam_s(__out_ecount_z(_SizeInWords) wchar_t * _DstBuf, __in size_t _SizeInWords);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_0(errno_t, _wtmpnam_s, __out_ecount_z(_Size) wchar_t, _Buffer)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_0(wchar_t *, __RETURN_POLICY_DST, _CRTIMP, _wtmpnam, __out_z_opt wchar_t, _Buffer)

_CRTIMP __checkReturn_opt wint_t __cdecl _fgetwc_nolock(__inout FILE * _File);
_CRTIMP __checkReturn_opt wint_t __cdecl _fputwc_nolock(__in_z wchar_t _Ch, __inout FILE * _File);
_CRTIMP __checkReturn_opt wint_t __cdecl _ungetwc_nolock(__in wint_t _Ch, __inout FILE * _File);

#ifdef _CRTBLD
#ifndef _INTERNAL_IFSTRIP_
#define _CRT_GETPUTWCHAR_NOINLINE
#else  /* _INTERNAL_IFSTRIP_ */
#undef _CRT_GETPUTWCHAR_NOINLINE
#endif  /* _INTERNAL_IFSTRIP_ */
#endif  /* _CRTBLD */

#if !defined (__cplusplus) || defined (_M_CEE_PURE) || defined (_CRT_GETPUTWCHAR_NOINLINE)
#define getwchar()      fgetwc(stdin)
#define putwchar(_c)    fputwc((_c),stdout)
#else  /* !defined (__cplusplus) || defined (_M_CEE_PURE) || defined (_CRT_GETPUTWCHAR_NOINLINE) */
inline wint_t __CRTDECL getwchar()
        {return (fgetwc(stdin)); }   /* stdin */
inline wint_t __CRTDECL putwchar(wchar_t _C)
        {return (fputwc(_C, stdout)); }       /* stdout */
#endif  /* !defined (__cplusplus) || defined (_M_CEE_PURE) || defined (_CRT_GETPUTWCHAR_NOINLINE) */

#define getwc(_stm)             fgetwc(_stm)
#define putwc(_c,_stm)          fputwc(_c,_stm)
#define _putwc_nolock(_c,_stm)     _fputwc_nolock(_c,_stm)
#define _getwc_nolock(_stm)        _fgetwc_nolock(_stm)

#if defined (_CRT_DISABLE_PERFCRIT_LOCKS) && !defined (_DLL)
#define fgetwc(_stm)            _getwc_nolock(_stm)
#define fputwc(_c,_stm)         _putwc_nolock(_c,_stm)
#define ungetwc(_c,_stm)        _ungetwc_nolock(_c,_stm)
#endif  /* defined (_CRT_DISABLE_PERFCRIT_LOCKS) && !defined (_DLL) */

#define _WSTDIO_DEFINED
#endif  /* _WSTDIO_DEFINED */

#define _STDIO_DEFINED
#endif  /* _STDIO_DEFINED */


/* Macro definitions */

#if defined (_CRT_DISABLE_PERFCRIT_LOCKS) && !defined (_DLL)
#define feof(_stream)     ((_stream)->_flag & _IOEOF)
#define ferror(_stream)   ((_stream)->_flag & _IOERR)
#define _fileno(_stream)  ((_stream)->_file)
#define fgetc(_stream)     (--(_stream)->_cnt >= 0 \
                ? 0xff & *(_stream)->_ptr++ : _filbuf(_stream))
#define putc(_c,_stream)  (--(_stream)->_cnt >= 0 \
                ? 0xff & (*(_stream)->_ptr++ = (char)(_c)) :  _flsbuf((_c),(_stream)))
#define getc(_stream)    fgetc(_stream)
#define getchar()         getc(stdin)
#define putchar(_c)       putc((_c),stdout)
#endif  /* defined (_CRT_DISABLE_PERFCRIT_LOCKS) && !defined (_DLL) */


#define _fgetc_nolock(_stream)       (--(_stream)->_cnt >= 0 ? 0xff & *(_stream)->_ptr++ : _filbuf(_stream))
#define _fputc_nolock(_c,_stream)    (--(_stream)->_cnt >= 0 ? 0xff & (*(_stream)->_ptr++ = (char)(_c)) :  _flsbuf((_c),(_stream)))
#define _getc_nolock(_stream)       _fgetc_nolock(_stream)
#define _putc_nolock(_c, _stream)   _fputc_nolock(_c, _stream)
#define _getchar_nolock()           _getc_nolock(stdin)
#define _putchar_nolock(_c)         _putc_nolock((_c),stdout)
#define _getwchar_nolock()          _getwc_nolock(stdin)
#define _putwchar_nolock(_c)        _putwc_nolock((_c),stdout)

_CRTIMP void __cdecl _lock_file(__inout FILE * _File);
_CRTIMP void __cdecl _unlock_file(__inout FILE * _File);


_CRTIMP __checkReturn_opt int __cdecl _fclose_nolock(__inout FILE * _File);
_CRTIMP __checkReturn_opt int __cdecl _fflush_nolock(__inout_opt FILE * _File);
_CRTIMP __checkReturn_opt size_t __cdecl _fread_nolock(__out_bcount(_ElementSize*_Count) void * _DstBuf, __in size_t _ElementSize, __in size_t _Count, __inout FILE * _File);
_CRTIMP __checkReturn_opt size_t __cdecl _fread_nolock_s(__out_bcount(_ElementSize*_Count) void * _DstBuf, __in size_t _DstSize, __in size_t _ElementSize, __in size_t _Count, __inout FILE * _File);
_CRTIMP __checkReturn_opt int __cdecl _fseek_nolock(__inout FILE * _File, __in long _Offset, __in int _Origin);
_CRTIMP __checkReturn long __cdecl _ftell_nolock(__inout FILE * _File);
_CRTIMP __checkReturn_opt int __cdecl _fseeki64_nolock(__inout FILE * _File, __in __int64 _Offset, __in int _Origin);
_CRTIMP __checkReturn __int64 __cdecl _ftelli64_nolock(__inout FILE * _File);
_CRTIMP __checkReturn_opt size_t __cdecl _fwrite_nolock(__in_bcount(_Size*_Count) const void * _DstBuf, __in size_t _Size, __in size_t _Count, __inout FILE * _File);
_CRTIMP __checkReturn_opt int __cdecl _ungetc_nolock(__in int _Ch, __inout FILE * _File);

#if defined (_CRT_DISABLE_PERFCRIT_LOCKS) && !defined (_DLL)
#define fclose(_stm)                                            _fclose_nolock(_stm)
#define fflush(_stm)                                            _fflush_nolock(_stm)
#define fread(_DstBuf, _ElementSize, _Count, _File)             _fread_nolock(_DstBuf, _ElementSize, _Count, _File)
#define fread_s(_DstBuf, _DstSize, _ElementSize, _Count, _File) _fread_nolock_s(_DstBuf, _DstSize, _ElementSize, _Count, _File)
#define fseek(_stm,_offset,_origin)                             _fseek_nolock(_stm,_offset,_origin)
#define ftell(_stm)                                             _ftell_nolock(_stm)
#define _fseeki64(_stm)                                         _fseeki64_nolock(_stm)
#define _ftelli64(_stm)                                         _ftelli64_nolock(_stm)
#define fwrite(_buf,_siz,_cnt,_stm)                             _fwrite_nolock(_buf,_siz,_cnt,_stm)
#define ungetc(_c,_stm)                                         _ungetc_nolock(_c,_stm)
#endif  /* defined (_CRT_DISABLE_PERFCRIT_LOCKS) && !defined (_DLL) */

#if !__STDC__

/* Non-ANSI names for compatibility */

#define P_tmpdir  _P_tmpdir
#define SYS_OPEN  _SYS_OPEN

#if defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC)
#pragma push_macro("tempnam")
#undef tempnam
#endif  /* defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC) */

_CRT_NONSTDC_DEPRECATE(_tempnam) _CRTIMP char * __cdecl tempnam(__in_z_opt const char * _Directory, __in_z_opt const char * _FilePrefix);

#if defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC)
#pragma pop_macro("tempnam")
#endif  /* defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC) */

_CRT_NONSTDC_DEPRECATE(_fcloseall) _CRTIMP __checkReturn_opt int __cdecl fcloseall(void);
_CRT_NONSTDC_DEPRECATE(_fdopen) _CRTIMP __checkReturn FILE * __cdecl fdopen(__in int _FileHandle, __in_z __format_string const char * _Format);
_CRT_NONSTDC_DEPRECATE(_fgetchar) _CRTIMP __checkReturn_opt int __cdecl fgetchar(void);
_CRT_NONSTDC_DEPRECATE(_fileno) _CRTIMP __checkReturn int __cdecl fileno(__in FILE * _File);
_CRT_NONSTDC_DEPRECATE(_flushall) _CRTIMP __checkReturn_opt int __cdecl flushall(void);
_CRT_NONSTDC_DEPRECATE(_fputchar) _CRTIMP __checkReturn_opt int __cdecl fputchar(__in int _Ch);
_CRT_NONSTDC_DEPRECATE(_getw) _CRTIMP __checkReturn int __cdecl getw(__inout FILE * _File);
_CRT_NONSTDC_DEPRECATE(_putw) _CRTIMP __checkReturn_opt int __cdecl putw(__in int _Ch, __inout FILE * _File);
_CRT_NONSTDC_DEPRECATE(_rmtmp) _CRTIMP __checkReturn int __cdecl rmtmp(void);

#endif  /* !__STDC__ */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#ifdef _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif  /* _INC_STDIO */
