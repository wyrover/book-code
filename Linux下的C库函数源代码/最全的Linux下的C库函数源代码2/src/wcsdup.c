/***
*wcsdup.c - duplicate a wide-character string in malloc'd memory
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _wcsdup() - grab new memory, and duplicate the string into it
*       (wide-character).
*
*******************************************************************************/


#include <cruntime.h>
#include <malloc.h>
#include <string.h>
#include <internal.h>

/***
*wchar_t *_wcsdup(string) - duplicate string into malloc'd memory
*
*Purpose:
*       Allocates enough storage via malloc() for a copy of the
*       string, copies the string into the new memory, and returns
*       a pointer to it (wide-character).
*
*Entry:
*       wchar_t *string - string to copy into new memory
*
*Exit:
*       returns a pointer to the newly allocated storage with the
*       string in it.
*
*       returns NULL if enough memory could not be allocated, or
*       string was NULL.
*
*Uses:
*
*Exceptions:
*
*******************************************************************************/

#ifdef _DEBUG

#include <crtdbg.h>

wchar_t * __cdecl _wcsdup (
        const wchar_t * string
        )
{
        return _wcsdup_dbg(string, _NORMAL_BLOCK, NULL, 0);
}

wchar_t * __cdecl _wcsdup_dbg (
        const wchar_t * string,
        int nBlockUse,
        const char * szFileName,
        int nLine
        )

#else  /* _DEBUG */

wchar_t * __cdecl _wcsdup (
        const wchar_t * string
        )

#endif  /* _DEBUG */

{
        wchar_t *memory;
    size_t size = 0;

        if (!string)
                return(NULL);

    size = wcslen(string) + 1;
#ifdef _DEBUG
        if (memory = (wchar_t *) _calloc_dbg(size, sizeof(wchar_t), nBlockUse, szFileName, nLine))
#else  /* _DEBUG */
        if (memory = (wchar_t *) calloc(size, sizeof(wchar_t)))
#endif  /* _DEBUG */
        {
                _ERRCHECK(wcscpy_s(memory, size, string));
        return memory;
        }

        return(NULL);
}

