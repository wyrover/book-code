/***
*system.c - pass a command line to the shell
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines system() - passes a command to the shell
*
*******************************************************************************/

#include <cruntime.h>
#include <process.h>
#include <io.h>
#include <stdlib.h>
#include <errno.h>
#include <tchar.h>
#include <dbgint.h>
#include <internal.h>

/***
*int system(command) - send the command line to a shell
*
*Purpose:
*       Executes a shell and passes the command line to it.
*       If command is NULL, determine if a command processor exists.
*       The command processor is described by the environment variable
*       COMSPEC.  If that environment variable does not exist, try the
*       name "cmd.exe" for Windows NT and "command.com" for Windows '95.
*
*Entry:
*       char *command - command to pass to the shell (if NULL, just determine
*                       if command processor exists)
*
*Exit:
*       if command != NULL  returns status of the shell
*       if command == NULL  returns non-zero if CP exists, zero if CP doesn't exist
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _tsystem (
        const _TSCHAR *command
        )
{
        _TSCHAR *argv[4];
        _TSCHAR *envbuf = NULL;
        unsigned int osver = 0;
        int retval = 0;

        _ERRCHECK_EINVAL(_tdupenv_s_crt(&envbuf, NULL, _T("COMSPEC")));
        argv[0] = envbuf;

        /*
         * If command == NULL, return true IFF %COMSPEC%
         * is set AND the file it points to exists.
         */

        if (command == NULL)
        {
            if(argv[0]==NULL)
            {
                goto cleanup;
            }
            else
            {
                /* _taccess_s does not change errno if the return value is 0*/
                errno_t e = _taccess_s(argv[0], 0);
                retval = (e == 0);
                goto cleanup;
            }
        }

        _ASSERTE(*command != _T('\0'));

        argv[1] = _T("/c");
        argv[2] = (_TSCHAR *) command;
        argv[3] = NULL;

        /* If there is a COMSPEC defined, try spawning the shell */

        /* Do not try to spawn the null string */
        if (argv[0])
        {
                errno_t save_errno = errno;
                errno = 0;

                if ((retval = (int)_tspawnve(_P_WAIT,argv[0],argv,NULL)) != -1)
                {
                    errno = save_errno;
                    goto cleanup;
                }
                if (errno != ENOENT && errno != EACCES)
                {
                    goto cleanup;
                }
                errno = save_errno;
        }

        /* No COMSPEC so set argv[0] to what COMSPEC should be. */
        _ERRCHECK(_get_osver(&osver));
        argv[0] = ( osver & 0x8000 ) ? _T("command.com") : _T("cmd.exe");

        /* Let the _spawnvpe routine do the path search and spawn. */

        retval = (int)_tspawnvpe(_P_WAIT,argv[0],argv,NULL);
        goto cleanup;

cleanup:
        _free_crt(envbuf);
        return retval;
}
