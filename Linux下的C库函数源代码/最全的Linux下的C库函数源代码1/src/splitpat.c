/***
*splitpath.c - break down path name into components
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       To provide support for accessing the individual components of an
*       arbitrary path name
*
*******************************************************************************/


/* we don't want to fill up the buffers in debug to mantain back-compat */
#define _SECURECRT_FILL_BUFFER 0

#include <stdlib.h>
#ifdef _MBCS
#include <mbstring.h>
#endif  /* _MBCS */
#include <tchar.h>
#include <internal_securecrt.h>

/***
*_splitpath() - split a path name into its individual components
*
*Purpose:
*       to split a path name into its individual components
*
*Entry:
*       path  - pointer to path name to be parsed
*       drive - pointer to buffer for drive component, if any
*       dir   - pointer to buffer for subdirectory component, if any
*       fname - pointer to buffer for file base name component, if any
*       ext   - pointer to buffer for file name extension component, if any
*
*Exit:
*       drive - pointer to drive string.  Includes ':' if a drive was given.
*       dir   - pointer to subdirectory string.  Includes leading and trailing
*           '/' or '\', if any.
*       fname - pointer to file base name
*       ext   - pointer to file extension, if any.  Includes leading '.'.
*
*Exceptions:
*
*******************************************************************************/

#ifdef _UNICODE
#define _tsplitpath_helper _wsplitpath_helper
#else  /* _UNICODE */
#define _tsplitpath_helper _splitpath_helper
#endif  /* _UNICODE */

#define _FUNC_PROLOGUE static
#define _FUNC_NAME _tsplitpath_helper
#define _CHAR _TSCHAR

#ifndef _MBCS
#define _MBS_SUPPORT 0
#else  /* _MBCS */
#define _MBS_SUPPORT 1
/* _splitpath uses _ismbblead and not _ismbblead_l */
#undef _ISMBBLEAD
#define _ISMBBLEAD(_Character) \
    _ismbblead((_Character))
#endif  /* _MBCS */

/* _tsplitpath_helper does not call invalid_parameter if one of the buffer is too small */
#undef _RETURN_BUFFER_TOO_SMALL
#define _RETURN_BUFFER_TOO_SMALL(_String, _Size) \
    return (errno = ERANGE)

/* _tsplitpath_helper does not pad the string */
#define _TCSNCPY_S(_Dst, _Size, _Src, _Count) _tcsncpy_s((_Dst), ((size_t)-1), (_Src), (_Count))
#undef _RESET_STRING
#define _RESET_STRING(_String, _Size) \
    *(_String) = 0;

#include <tsplitpath_s.inl>

void __cdecl _tsplitpath (
        register const _TSCHAR *path,
        _TSCHAR *drive,
        _TSCHAR *dir,
        _TSCHAR *fname,
        _TSCHAR *ext
        )
{
    _tsplitpath_helper(
        path,
        drive, drive ? _MAX_DRIVE : 0,
        dir, dir ? _MAX_DIR : 0,
        fname, fname ? _MAX_FNAME : 0,
        ext, ext ? _MAX_EXT : 0);
}
