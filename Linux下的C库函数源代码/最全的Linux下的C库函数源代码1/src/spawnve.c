/***
*spawnve.c - Low level routine eventually called by all _spawnXX routines
*       also contains all code for _execve, called by _execXX routines
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*
*       This is the low level routine which is eventually invoked by all the
*       _spawnXX routines.
*
*       This is also the low-level routine which is eventually invoked by
*       all of the _execXX routines.
*
*******************************************************************************/

#include <cruntime.h>
#include <io.h>
#include <process.h>
#include <errno.h>
#include <msdos.h>
#include <string.h>
#include <stdlib.h>
#include <internal.h>
#include <mbstring.h>
#include <tchar.h>
#include <dbgint.h>

#define SLASHCHAR   _T('\\')
#define XSLASHCHAR  _T('/')

#ifndef EXECVE
#ifdef WPRFLAG
static intptr_t __cdecl wcomexecmd(int mode, const wchar_t * name,
        const wchar_t * const * argv, const wchar_t * const * envp);
#else  /* WPRFLAG */
static intptr_t __cdecl comexecmd(int mode, const char * name,
        const char * const * argv, const char * const * envp);
#endif  /* WPRFLAG */
#else  /* EXECVE */
#ifdef WPRFLAG
static intptr_t __cdecl wcomexecmd(const wchar_t * name,
        const wchar_t * const * argv, const wchar_t * const * envp);
#else  /* WPRFLAG */
static intptr_t __cdecl comexecmd(const char * name,
        const char * const * argv, const char * const * envp);
#endif  /* WPRFLAG */
#endif  /* EXECVE */

/***
*static int comexecmd(mode, name, argv, envp) - do the exec
*       or spawn after name fixup
*
*Purpose:
*       Spawns a child process with given parameters and environment.  Either
*       overlays current process or loads in free memory while parent process
*       waits.  If the named file is a .cmd file, modifies the calling sequence
*       and prepends the /c and filename arguments into the command string
*
*       Exec doesn't take a mode; instead, the parent process goes away as
*       the child process is brought in.
*
*Entry:
*       int mode - mode to spawn (WAIT, NOWAIT, or OVERLAY)
*                   only WAIT and OVERLAY currently supported
*
*           ****  mode is only used in the spawnve() version  ****
*
*       _TSCHAR *name - pathname of file to spawn.  Includes the extension
*       _TSCHAR **argv - vector of parameter strings
*       _TSCHAR **envp - vector of environment variables
*
*Exit:
*       returns exit code of child process
*       if fails, returns -1
*
*Exceptions:
*       Returns a value of (-1) to indicate an error in exec'ing the child
*       process.  errno may be set to:
*
*       E2BIG   = failed in argument/environment processing (_cenvarg)
*                 argument list or environment too big;
*       EACCESS = locking or sharing violation on file;
*       EMFILE  = too many files open;
*       ENOENT  = failed to find program name - no such file or directory;
*       ENOEXEC = failed in exec - bad executable format;
*       ENOMEM  = failed in memory allocation (during malloc, or in
*                 setting up memory for executing child process).
*
*******************************************************************************/

#ifdef WPRFLAG
static intptr_t __cdecl wcomexecmd (
#else  /* WPRFLAG */
static intptr_t __cdecl comexecmd (
#endif  /* WPRFLAG */

#ifndef EXECVE
        REG3 int mode,
#endif  /* EXECVE */

        REG2 const _TSCHAR *name,
        const _TSCHAR * const *argv,
        const _TSCHAR * const *envp
        )
{
        _TSCHAR *argblk;
        _TSCHAR *envblk;
        REG4 intptr_t rc;

        /* validation section */
        _VALIDATE_RETURN(name != NULL, EINVAL, -1);
        _VALIDATE_RETURN(argv != NULL, EINVAL, -1);

#ifdef WPRFLAG
        if (_wcenvarg(argv, envp, &argblk, &envblk, name) == -1)
#else  /* WPRFLAG */
        if (_cenvarg(argv, envp, &argblk, &envblk, name) == -1)
#endif  /* WPRFLAG */
                return -1;

#ifndef EXECVE
#ifdef WPRFLAG
        rc = _wdospawn(mode, name, argblk, envblk);
#else  /* WPRFLAG */
        rc = _dospawn(mode, name, argblk, envblk);
#endif  /* WPRFLAG */
#else  /* EXECVE */
#ifdef WPRFLAG
        rc = _wdospawn(_P_OVERLAY, name, argblk, envblk);
#else  /* WPRFLAG */
        rc = _dospawn(_P_OVERLAY, name, argblk, envblk);
#endif  /* WPRFLAG */
#endif  /* EXECVE */
        /* free memory */

        _free_crt(argblk);
        _free_crt(envblk);

        return rc;
}

/***
*int _spawnve(mode, name, argv, envp) - low level _spawnXX library function
*int _execve(name, argv, envp) - low level _execXX library function
*
*Purpose:
*       spawns or execs a child process; takes a single pointer to an argument
*       list as well as a pointer to the environment; unlike _spawnvpe,
*       _spawnve does not search the PATH= list in processing the name
*       parameter; mode specifies the parent's execution mode.
*
*Entry:
*       int mode    - parent process's execution mode:
*                     must be one of _P_OVERLAY, _P_WAIT, _P_NOWAIT;
*                     not used for _execve
*       _TSCHAR *name  - path name of program to spawn;
*       _TSCHAR **argv - pointer to array of pointers to child's arguments;
*       _TSCHAR **envp - pointer to array of pointers to child's environment
*                     settings.
*
*Exit:
*       Returns : (int) a status value whose meaning is as follows:
*               0        = normal termination of child process;
*               positive = exit code of child upon error termination
*                          (abort or exit(nonzero));
*               -1       = child process was not spawned;
*                          errno indicates what kind of error:
*                          (E2BIG, EINVAL, ENOENT, ENOEXEC, ENOMEM).
*
*Exceptions:
*       Returns a value of (-1) to indicate an error in spawning the child
*       process.  errno may be set to:
*
*       E2BIG   = failed in argument/environment processing (_cenvarg) -
*                 argument list or environment too big;
*       EINVAL  = invalid mode argument;
*       ENOENT  = failed to find program name - no such file or directory;
*       ENOEXEC = failed in spawn - bad executable format;
*       ENOMEM  = failed in memory allocation (during malloc, or in
*                 setting up memory for spawning child process).
*
*******************************************************************************/

/* Extension array - ordered in search order from right to left.

   ext_strings  = array of extensions
*/

static _TSCHAR *ext_strings[] = { _T(".cmd"), _T(".bat"), _T(".exe"), _T(".com") };
enum {CMD, BAT, EXE, COM, EXTFIRST=CMD, EXTLAST=COM};

intptr_t __cdecl

#ifndef EXECVE

_tspawnve (
        REG3 int mode,

#else  /* EXECVE */

_texecve (

#endif  /* EXECVE */

        const _TSCHAR *name,
        const _TSCHAR * const *argv,
        const _TSCHAR * const *envp
        )
{
        _TSCHAR *ext;   /* where the extension goes if we have to add one */
        REG1 _TSCHAR *p;
        _TSCHAR *q;
        REG2 _TSCHAR *pathname = (_TSCHAR *)name;
        REG4 intptr_t rc;
        REG5 int i;
        errno_t save_errno = 0;

        /* validation section */
        _VALIDATE_RETURN(name != NULL, EINVAL, -1);
        _VALIDATE_RETURN(*name != _T('\0'), EINVAL, -1);
        _VALIDATE_RETURN(argv != NULL, EINVAL, -1);
        _VALIDATE_RETURN(*argv != NULL, EINVAL, -1);
        _VALIDATE_RETURN(**argv != _T('\0'), EINVAL, -1);

        p = _tcsrchr(pathname, SLASHCHAR);
        q = _tcsrchr(pathname, XSLASHCHAR);

        /* ensure that pathname is an absolute or relative pathname. also,
         * position p to point at the filename portion of pathname (i.e., just
         * after the last occurence of a colon, slash or backslash character */

        if (!q) {
                if (!p)
                        if (!(p = _tcschr(pathname, _T(':')))) {

                                /* pathname is a filename only, force it to be
                                 * a relative pathname. note that an extra byte
                                 * is malloc-ed just in case pathname is NULL,
                                 * to keep the heap from being trashed by
                                 * strcpy */
                                size_t pathname_size = _tcslen(pathname) + 3;
                                if (!(pathname = _calloc_crt(pathname_size, sizeof(_TSCHAR))))
                                        return(-1);

                                _ERRCHECK(_tcscpy_s(pathname, pathname_size, _T(".\\")));
                                _ERRCHECK(_tcscat_s(pathname, pathname_size, name));

                                /* set p to point to the start of the filename
                                 * (i.e., past the ".\\" prefix) */
                                p = pathname + 2;
                        }
                        /* else pathname has drive specifier prefix and p is
                         * is pointing to the ':' */
        }
        else if (!p || q > p)   /* p == NULL or q > p */
                p = q;


        rc = -1;        /* init to error value */

        if (ext = _tcsrchr(p, _T('.')))  {

                /* extension given; only do filename */

                if (_taccess_s(pathname, 0) == 0) {

#ifndef EXECVE

#ifdef WPRFLAG
                        rc = wcomexecmd(mode, pathname, argv, envp);
#else  /* WPRFLAG */
                        rc = comexecmd(mode, pathname, argv, envp);
#endif  /* WPRFLAG */

#else  /* EXECVE */

#ifdef WPRFLAG
                        rc = wcomexecmd(pathname, argv, envp);
#else  /* WPRFLAG */
                        rc = comexecmd(pathname, argv, envp);
#endif  /* WPRFLAG */

#endif  /* EXECVE */
                }

        }
        else    {

                /* no extension; try .cmd/.bat, then .com and .exe */

                size_t size = _tcslen(pathname) + 5;
                if (!(p = _calloc_crt(size, sizeof(_TSCHAR))))
                        return(-1);

                _ERRCHECK(_tcscpy_s(p, size, pathname));
                ext = p + _tcslen(pathname);

                save_errno = errno;
                for (i = EXTLAST; i >= EXTFIRST; --i) {
                        _ERRCHECK(_tcscpy_s(ext, size - (ext - p), ext_strings[i]));

                        if (_taccess_s(p, 0) == 0) {
                                errno = save_errno;

#ifndef EXECVE
#ifdef WPRFLAG
                                rc = wcomexecmd(mode, p, argv, envp);
#else  /* WPRFLAG */
                                rc = comexecmd(mode, p, argv, envp);
#endif  /* WPRFLAG */
#else  /* EXECVE */
#ifdef WPRFLAG
                                rc = wcomexecmd(p, argv, envp);
#else  /* WPRFLAG */
                                rc = comexecmd(p, argv, envp);
#endif  /* WPRFLAG */
#endif  /* EXECVE */
                                break;
                        }
                }
                _free_crt(p);
        }

        if (pathname != name)
                _free_crt(pathname);

        return rc;
}
