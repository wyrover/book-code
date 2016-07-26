// fiopen -- _Fiopen(const char */const wchar_t *, ios_base::openmode)
#include <fstream>
#include <internal.h>
#include <windows.h>

 #if _FN_WIDE
_STD_BEGIN

_MRTIMP2_NCEEPURE FILE *__CLRCALL_PURE_OR_CDECL _Fiopen(const char *filename,
	ios_base::openmode mode, int prot)
	{	// open wide-named file with byte name
	wchar_t wc_name[FILENAME_MAX];


	if (mbstowcs_s(NULL, wc_name, FILENAME_MAX, filename, FILENAME_MAX - 1) != 0)
		return (0);
	return _Fiopen(wc_name, mode, prot);
	}

_MRTIMP2_NCEEPURE FILE *__CLRCALL_PURE_OR_CDECL _Xfsopen(__in_z const wchar_t *filename, __in_z const wchar_t *mode, __in int prot)
	{	// use ANSI version if under Win9x
	unsigned int osplatform = 0;

	_ERRCHECK(_get_osplatform(&osplatform));
	if (osplatform != VER_PLATFORM_WIN32_WINDOWS)
		return _wfsopen(filename, mode, prot);
	else
		{
		char c_name[FILENAME_MAX];
		char c_mode[FOPEN_MAX];

		if (wcstombs_s(NULL, c_name, FILENAME_MAX, filename, FILENAME_MAX - 1) != 0)
			return (0);
		else if (wcstombs_s(NULL, c_mode, FOPEN_MAX, mode, FOPEN_MAX - 1) != 0)
			return (0);
		return _fsopen(c_name, c_mode, prot);
		}
	}

  #ifndef _SYSCH
   #define _SYSCH(x)	L ## x
typedef wchar_t _Sysch_t;
  #endif /* _SYSCH */

 #else /* _FN_WIDE */
  #define _Xfsopen	_fsopen

  #ifndef _SYSCH
   #define _SYSCH(x)	x
typedef char _Sysch_t;
  #endif /* _SYSCH */

_STD_BEGIN

_MRTIMP2_NCEEPURE FILE *__CLRCALL_PURE_OR_CDECL _Fiopen(const wchar_t *filename,
	ios_base::openmode mode, int prot)
	{	// open byte-named file with wide name
	char c_name[FILENAME_MAX];

	if (wcstombs_s(NULL, c_name, FILENAME_MAX, filename, FILENAME_MAX - 1) != 0)
		return (0);
	return _Fiopen(c_name, mode, prot);
	}

 #endif /* _FN_WIDE */

_MRTIMP2_NCEEPURE FILE *__CLRCALL_PURE_OR_CDECL _Fiopen(const _Sysch_t *filename,
	ios_base::openmode mode, int prot)
	{	// open a file with native name
	static const _Sysch_t *mods[] =
		{	// fopen mode strings corresponding to valid[i]
		_SYSCH("r"), _SYSCH("w"), _SYSCH("w"), _SYSCH("a"),
		_SYSCH("rb"), _SYSCH("wb"), _SYSCH("wb"), _SYSCH("ab"),
		_SYSCH("r+"), _SYSCH("w+"), _SYSCH("a+"),
		_SYSCH("r+b"), _SYSCH("w+b"), _SYSCH("a+b"),
		0};

	static const int valid[] =
		{	// valid combinations of open flags
		ios_base::in,
		ios_base::out,
		ios_base::out | ios_base::trunc,
		ios_base::out | ios_base::app,
		ios_base::in | ios_base::binary,
		ios_base::out | ios_base::binary,
		ios_base::out | ios_base::trunc | ios_base::binary,
		ios_base::out | ios_base::app | ios_base::binary,
		ios_base::in | ios_base::out,
		ios_base::in | ios_base::out | ios_base::trunc,
		ios_base::in | ios_base::out | ios_base::app,
		ios_base::in | ios_base::out | ios_base::binary,
		ios_base::in | ios_base::out | ios_base::trunc
			| ios_base::binary,
		ios_base::in | ios_base::out | ios_base::app
			| ios_base::binary,
		0};

	FILE *fp = 0;
	int n;
	ios_base::openmode atendflag = mode & ios_base::ate;
	ios_base::openmode norepflag = mode & ios_base::_Noreplace;

	if (mode & ios_base::_Nocreate)
		mode |= ios_base::in;	// file must exist
	if (mode & ios_base::app)
		mode |= ios_base::out;	// extension -- app implies out

	mode &= ~(ios_base::ate | ios_base::_Nocreate | ios_base::_Noreplace);
	for (n = 0; valid[n] != 0 && valid[n] != mode; ++n)
		;	// look for a valid mode

	if (valid[n] == 0)
		return (0);	// no valid mode
	else if (norepflag && mode & (ios_base::out || ios_base::app)
		&& (fp = _Xfsopen(filename, _SYSCH("r"), prot)) != 0)
		{	// file must not exist, close and fail
		fclose(fp);
		return (0);
		}
	else if (fp != 0 && fclose(fp) != 0)
		return (0);	// can't close after test open
	else if ((fp = _Xfsopen(filename, mods[n], prot)) == 0)
		return (0);	// open failed

	if (!atendflag || fseek(fp, 0, SEEK_END) == 0)
		return (fp);	// no need to seek to end, or seek succeeded

	fclose(fp);	// can't position at end
	return (0);
		}

 #ifdef _NATIVE_WCHAR_T_DEFINED
_MRTIMP2_NCEEPURE _Filet *__CLRCALL_PURE_OR_CDECL _Fiopen(const unsigned short *_Filename,
	ios_base::openmode _Mode, int _Prot)
	{	// open file with wide name
	return (_Fiopen((wchar_t*)(_Filename), _Mode, _Prot));
	}
 #endif

_STD_END

/*
 * Copyright (c) 1992-2005 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V4.05:0009 */
