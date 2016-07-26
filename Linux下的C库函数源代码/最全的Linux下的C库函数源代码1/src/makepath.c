/***
*makepath.c - create path name from components
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       To provide support for creation of full path names from components
*
*******************************************************************************/

#include <mtdll.h>
#include <cruntime.h>
#include <stdlib.h>
#ifdef _MBCS
#include <mbdata.h>
#include <mbstring.h>
#endif  /* _MBCS */
#include <tchar.h>
#include <internal.h>
#include <limits.h>

/***
*void _makepath - build path name from components
*
*Purpose:
*       create a path name from its individual components
*
*Entry:
*       _TSCHAR *path  - pointer to buffer for constructed path
*       _TSCHAR *drive - pointer to drive component, may or may not contain
*                     trailing ':'
*       _TSCHAR *dir   - pointer to subdirectory component, may or may not include
*                     leading and/or trailing '/' or '\' characters
*       _TSCHAR *fname - pointer to file base name component
*       _TSCHAR *ext   - pointer to extension component, may or may not contain
*                     a leading '.'.
*
*Exit:
*       path - pointer to constructed path name
*
*Exceptions:
*
*******************************************************************************/

void __cdecl _tmakepath (
        register _TSCHAR *path,
        const _TSCHAR *drive,
        const _TSCHAR *dir,
        const _TSCHAR *fname,
        const _TSCHAR *ext
        )
{
        _tmakepath_s(path, (size_t)-1, drive, dir, fname, ext);
}
