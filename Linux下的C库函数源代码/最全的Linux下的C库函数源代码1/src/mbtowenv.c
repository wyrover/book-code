/***
*mbtowenv.c - convert multibyte environment block to wide
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines __mbtow_environ(). Create a wide character equivalent of
*       an existing multibyte environment block.
*
*******************************************************************************/


#include <windows.h>
#include <cruntime.h>
#include <internal.h>
#include <stdlib.h>
#include <dbgint.h>

/***
*__mbtow_environ - copy multibyte environment block to wide environment block
*
*Purpose:
*       Create a wide character equivalent of an existing multibyte
*       environment block.
*
*Entry:
*       Assume _environ (global pointer) points to existing multibyte
*       environment block.
*
*Exit:
*       If success, every multibyte environment variable has been added to
*       the wide environment block and returns 0.
*       If failure, returns -1.
*
*Exceptions:
*       If space cannot be allocated, returns -1.
*
*******************************************************************************/

int __cdecl __mbtow_environ (
        void
        )
{
        int size;
        wchar_t *wenvp=NULL;
        char **envp = _environ;

        /*
         * For every environment variable in the multibyte environment,
         * convert it and add it to the wide environment.
         */

        while (*envp)
        {
            /* find out how much space is needed */
            if ((size = MultiByteToWideChar(CP_ACP, 0, *envp, -1, NULL, 0)) == 0)
                return -1;

            /* allocate space for variable */
            if ((wenvp = (wchar_t *) _calloc_crt(size, sizeof(wchar_t))) == NULL)
                return -1;

            /* convert it */
            if ((size = MultiByteToWideChar(CP_ACP, 0, *envp, -1, wenvp, size)) == 0)
            {
                _free_crt(wenvp);
                return -1;
            }

            /* set it - this is not primary call, so set primary == 0 */
            if(__crtwsetenv(&wenvp, 0)<0)
            {
                /* If the set fails, free the duplicated env string and bail */
                if(wenvp)
                {
                    _free_crt(wenvp);
                    wenvp=NULL;
                }
                return -1;
            }

            envp++;
        }

        return 0;
}

