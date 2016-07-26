/***
*wtombenv.c - convert wide environment block to multibyte
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines __wtomb_environ(). Create a multibyte equivalent of
*       an existing wide character environment block.
*
*******************************************************************************/


#include <windows.h>
#include <cruntime.h>
#include <internal.h>
#include <stdlib.h>
#include <dbgint.h>

/***
*__wtomb_environ - copy wide environment block to multibyte environment block
*
*Purpose:
*       Create a multibyte equivalent of an existing wide character
*       environment block.
*
*Entry:
*       Assume _wenviron (global pointer) points to existing wide
*       environment block.
*
*Exit:
*       If success, every wide environment variable has been added to
*       the multibyte environment block and returns 0.
*       If failure, returns -1.
*
*Exceptions:
*       If space cannot be allocated, returns -1.
*
*******************************************************************************/

int __cdecl __wtomb_environ (
        void
        )
{
        char *envp=NULL;
        wchar_t **wenvp = _wenviron;

        /*
         * For every environment variable in the multibyte environment,
         * convert it and add it to the wide environment.
         */

        while (*wenvp)
        {
            int size;

            /* find out how much space is needed */
            if ((size = WideCharToMultiByte(CP_ACP, 0, *wenvp, -1, NULL, 0, NULL, NULL)) == 0)
                return -1;

            /* allocate space for variable */
            if ((envp = (char *) _calloc_crt(size, sizeof(char))) == NULL)
                return -1;

            /* convert it */
            if (WideCharToMultiByte(CP_ACP, 0, *wenvp, -1, envp, size, NULL, NULL) == 0)
            {
                _free_crt(envp);
                return -1;
            }

            /* set it - this is not primary call, so set primary == 0 */
            if(__crtsetenv(&envp, 0)<0)
            {
                if(envp)
                {
                    _free_crt(envp);
                    envp=NULL;
                }
            }

            wenvp++;
        }

        return 0;
}

