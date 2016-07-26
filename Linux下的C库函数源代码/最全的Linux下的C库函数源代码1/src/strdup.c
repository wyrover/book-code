/***
*strdup.c - duplicate a string in malloc'd memory
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _strdup() - grab new memory, and duplicate the string into it.
*
*******************************************************************************/

#include <cruntime.h>
#include <malloc.h>
#include <string.h>
#include <internal.h>

/***
*char *_strdup(string) - duplicate string into malloc'd memory
*
*Purpose:
*       Allocates enough storage via malloc() for a copy of the
*       string, copies the string into the new memory, and returns
*       a pointer to it.
*
*Entry:
*       char *string - string to copy into new memory
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

char * __cdecl _strdup (
        const char * string
        )
{
        return _strdup_dbg(string, _NORMAL_BLOCK, NULL, 0);
}

char * __cdecl _strdup_dbg (
        const char * string,
        int nBlockUse,
        const char * szFileName,
        int nLine
        )

#else  /* _DEBUG */

char * __cdecl _strdup (
        const char * string
        )

#endif  /* _DEBUG */

{
        char *memory;
    size_t size = 0;

        if (!string)
                return(NULL);

    size = strlen(string) + 1;
#ifdef _DEBUG
        if (memory = _malloc_dbg(size, nBlockUse, szFileName, nLine))
#else  /* _DEBUG */
        if (memory = malloc(size))
#endif  /* _DEBUG */
        {
                _ERRCHECK(strcpy_s(memory, size, string));
        return memory;
        }

        return(NULL);
}
