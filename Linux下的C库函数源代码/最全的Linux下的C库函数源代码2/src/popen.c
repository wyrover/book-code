/***
*popen.c - initiate a pipe and a child command
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines _popen() and _pclose().
*
*******************************************************************************/


#include <cruntime.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <process.h>
#include <io.h>
#include <fcntl.h>
#include <internal.h>
#include <errno.h>
#include <msdos.h>
#include <mtdll.h>
#include <oscalls.h>
#include <tchar.h>
#include <dbgint.h>

/* size for pipe buffer
 */
#define PSIZE     1024

#define STDIN     0
#define STDOUT    1

#define SLASH _T("\\")
#define SLASHCHAR _T('\\')
#define XSLASHCHAR _T('/')
#define DELIMITER _T(";")



/* definitions for table of stream pointer - process handle pairs. the table
 * is created, maintained and accessed by the idtab function. _popen and
 * _pclose gain access to table entries only by calling idtab. Note that the
 * table is expanded as necessary (by idtab) and free table entries are reused
 * (an entry is free if its stream field is NULL), but the table is never
 * contracted.
 */

typedef struct {
        FILE *stream;
        intptr_t prochnd;
} IDpair;

/* number of entries in idpairs table
 */
#ifndef _UNICODE
unsigned __idtabsiz = 0;
#else  /* _UNICODE */
extern unsigned __idtabsiz;
#endif  /* _UNICODE */

/* pointer to first table entry
 */
#ifndef _UNICODE
IDpair *__idpairs = NULL;
#else  /* _UNICODE */
extern IDpair *__idpairs;
#endif  /* _UNICODE */

/* function to find specified table entries. also, creates and maintains
 * the table.
 */
static IDpair * __cdecl idtab(FILE *);


/***
*FILE *_popen(cmdstring,type) - initiate a pipe and a child command
*
*Purpose:
*       Creates a pipe and asynchronously executes a child copy of the command
*       processor with cmdstring (see system()). If the type string contains
*       an 'r', the calling process can read child command's standard output
*       via the returned stream. If the type string contains a 'w', the calling
*       process can write to the child command's standard input via the
*       returned stream.
*
*Entry:
*       _TSCHAR *cmdstring - command to be executed
*       _TSCHAR *type   - string of the form "r|w[b|t]", determines the mode
*                         of the returned stream (i.e., read-only vs write-only,
*                         binary vs text mode)
*
*Exit:
*       If successful, returns a stream associated with one end of the created
*       pipe (the other end of the pipe is associated with either the child
*       command's standard input or standard output).
*
*       If an error occurs, NULL is returned.
*
*Exceptions:
*
*******************************************************************************/

FILE * __cdecl _tpopen (
        const _TSCHAR *cmdstring,
        const _TSCHAR *type
        )
{

        int phdls[2];             /* I/O handles for pipe */
        int ph_open[2];           /* flags, set if correspond phdls is open */
        int i1;                   /* index into phdls[] */
        int i2;                   /* index into phdls[] */

        int tm = 0;               /* flag indicating text or binary mode */

        int stdhdl;               /* either STDIN or STDOUT */

        HANDLE newhnd;            /* ...in calls to DuplicateHandle API */

        FILE *pstream = NULL;     /* stream to be associated with pipe */

        HANDLE prochnd;           /* handle for current process */

        _TSCHAR *cmdexe;          /* pathname for the command processor */
        _TSCHAR *envbuf = NULL;   /* buffer for the env variable */
        intptr_t childhnd;        /* handle for child process (cmd.exe) */

        IDpair *locidpair;        /* pointer to IDpair table entry */
        _TSCHAR *buf = NULL, *pfin, *env;
        _TSCHAR *CommandLine;
        size_t CommandLineSize = 0;
        _TSCHAR _type[3] = {0, 0, 0};

        /* Info for spawning the child. */
        STARTUPINFO StartupInfo;  /* Info for spawning a child */
        BOOL childstatus = 0;
        PROCESS_INFORMATION ProcessInfo; /* child process information */

        errno_t save_errno;

        int fh_lock_held = 0;
        int popen_lock_held = 0;

        /* first check for errors in the arguments
         */
        _VALIDATE_RETURN((cmdstring != NULL), EINVAL,NULL);
        _VALIDATE_RETURN((type != NULL), EINVAL,NULL);

        while (*type == _T(' '))
        {
            type++;
        }
        _VALIDATE_RETURN(((*type == _T('w')) || (*type == _T('r'))), EINVAL,NULL);
        _type[0] = *type;
        ++type;
        while (*type == _T(' '))
        {
            ++type;
        }
        _VALIDATE_RETURN(((*type == 0) || (*type == _T('t')) || (*type == _T('b'))), EINVAL, NULL);
        _type[1] = *type;

        /* do the _pipe(). note that neither of the resulting handles will
         * be inheritable.
         */

        if ( _type[1] == _T('t') )
                tm = _O_TEXT;
        else if ( _type[1] == _T('b') )
                tm = _O_BINARY;

        tm |= _O_NOINHERIT;

        if ( _pipe( phdls, PSIZE, tm ) == -1 )
                goto error1;

        /* test _type[0] and set stdhdl, i1 and i2 accordingly.
         */
        if ( _type[0] == _T('w') ) {
                stdhdl = STDIN;
                i1 = 0;
                i2 = 1;
        }
        else {
                stdhdl = STDOUT;
                i1 = 1;
                i2 = 0;
        }

        /* ASSERT LOCK FOR IDPAIRS HERE!!!!
         */
        if ( !_mtinitlocknum( _POPEN_LOCK )) {
            _close( phdls[0] );
            _close( phdls[1] );
            return NULL;
        }
        _mlock( _POPEN_LOCK );
        __try
        {

        /* set flags to indicate pipe handles are open. note, these are only
         * used for error recovery.
         */
        ph_open[ 0 ] = ph_open[ 1 ] = 1;


        /* get the process handle, it will be needed in some API calls
         */
        prochnd = GetCurrentProcess();



        if ( !DuplicateHandle( prochnd,
                               (HANDLE)_osfhnd( phdls[i1] ),
                               prochnd,
                               &newhnd,
                               0L,
                               TRUE,                    /* inheritable */
                               DUPLICATE_SAME_ACCESS )
        ) {
                goto error2;
        }
        (void)_close( phdls[i1] );
        ph_open[ i1 ] = 0;

        /* associate a stream with phdls[i2]. note that if there are no
         * errors, pstream is the return value to the caller.
         */
        if ( (pstream = _tfdopen( phdls[i2], _type )) == NULL )
                goto error2;

        /* next, set locidpair to a free entry in the idpairs table.
         */
        if ( (locidpair = idtab( NULL )) == NULL )
                goto error3;


        /* Find what to use. command.com or cmd.exe */
        if ( (_ERRCHECK_EINVAL(_tdupenv_s_crt(&envbuf, NULL, _T("COMSPEC"))) != 0) || (envbuf == NULL) )
        {
            unsigned int osver = 0;
            _get_osver(&osver);
            cmdexe = ( osver & 0x8000 ) ? _T("command.com") : _T("cmd.exe");
        }
        else
        {
            cmdexe = envbuf;
        }

        /*
         * Initialise the variable for passing to CreateProcess
         */

        memset(&StartupInfo, 0, sizeof(StartupInfo));
        StartupInfo.cb = sizeof(StartupInfo);

        /* Used by os for duplicating the Handles. */

        StartupInfo.dwFlags = STARTF_USESTDHANDLES;
        StartupInfo.hStdInput = stdhdl == STDIN ? (HANDLE) newhnd
                                                : (HANDLE) _osfhnd(0);
        StartupInfo.hStdOutput = stdhdl == STDOUT ? (HANDLE) newhnd
                                                  : (HANDLE) _osfhnd(1);
        StartupInfo.hStdError = (HANDLE) _osfhnd(2);


        CommandLineSize = _tcslen(cmdexe) + _tcslen(_T(" /c ")) + (_tcslen(cmdstring)) +1;
        if ((CommandLine = _calloc_crt( CommandLineSize, sizeof(_TSCHAR))) == NULL)
            goto error3;
        _ERRCHECK(_tcscpy_s(CommandLine, CommandLineSize, cmdexe));
        _ERRCHECK(_tcscat_s(CommandLine, CommandLineSize, _T(" /c ")));
        _ERRCHECK(_tcscat_s(CommandLine, CommandLineSize, cmdstring));

        /* Check if cmdexe can be accessed. If yes CreateProcess else try
         * searching path.
         */
        save_errno = errno;
        if (_taccess_s(cmdexe, 0) == 0) {
            childstatus = CreateProcess( (LPTSTR) cmdexe,
                                         (LPTSTR) CommandLine,
                                         NULL,
                                         NULL,
                                         TRUE,
                                         0,
                                         NULL,
                                         NULL,
                                         &StartupInfo,
                                         &ProcessInfo
                                         );
        }
        else {
            TCHAR* envPath = NULL;
            size_t envPathSize = 0;
            if ((buf = _calloc_crt(_MAX_PATH, sizeof(_TSCHAR))) == NULL)
            {
                _free_crt(buf);
                _free_crt(CommandLine);
                _free_crt(envbuf);
                cmdexe = NULL;
                errno = save_errno;
                goto error3;
            }
            if (_ERRCHECK_EINVAL(_tdupenv_s_crt(&envPath, NULL, _T("PATH"))) != 0)
            {
                _free_crt(envPath);
                _free_crt(buf);
                _free_crt(CommandLine);
                _free_crt(envbuf);
                cmdexe = NULL;
                errno = save_errno;
                goto error3;
            }
            env = envPath;

#ifdef WPRFLAG
            while ( (env = _wgetpath(env, buf, _MAX_PATH -1)) && (*buf) ) {
#else  /* WPRFLAG */
            while ( (env = _getpath(env, buf, _MAX_PATH -1)) && (*buf) ) {
#endif  /* WPRFLAG */
                pfin = buf + _tcslen(buf) -1;

#ifdef _MBCS
                if (*pfin == SLASHCHAR) {
                    if (pfin != _mbsrchr(buf, SLASHCHAR))
                        _ERRCHECK(strcat_s(buf, _MAX_PATH, SLASH));
                }
                else if (*pfin != XSLASHCHAR)
                    _ERRCHECK(strcat_s(buf, _MAX_PATH, SLASH));

#else  /* _MBCS */
                if (*pfin != SLASHCHAR && *pfin != XSLASHCHAR)
                    _ERRCHECK(_tcscat_s(buf, _MAX_PATH, SLASH));
#endif  /* _MBCS */
                /* check that the final path will be of legal size. if so,
                 * build it. otherwise, return to the caller (return value
                 * and errno rename set from initial call to _spawnve()).
                 */
                if ( (_tcslen(buf) + _tcslen(cmdexe)) < _MAX_PATH )
                    _ERRCHECK(_tcscat_s(buf, _MAX_PATH, cmdexe));
                else
                    break;

                /* Check if buf can be accessed. If yes CreateProcess else try
                 * again.
                 */
                if (_taccess_s(buf, 0) == 0) {
                    childstatus = CreateProcess( (LPTSTR) buf,
                                                 CommandLine,
                                                 NULL,
                                                 NULL,
                                                 TRUE,
                                                 0,
                                                 NULL,
                                                 NULL,
                                                 &StartupInfo,
                                                 &ProcessInfo
                                                 );
                    break;
                }
            }
            _free_crt(envPath);
            _free_crt(buf);
        }
        _free_crt(CommandLine);
        _free_crt(envbuf);
        cmdexe = NULL;
        CloseHandle((HANDLE)newhnd);
        CloseHandle((HANDLE)ProcessInfo.hThread);
                errno = save_errno;

        /* check if the CreateProcess was sucessful.
         */
        if ( childstatus)
            childhnd = (intptr_t)ProcessInfo.hProcess;
        else
            goto error4;
        locidpair->prochnd = childhnd;
        locidpair->stream = pstream;

        /* success, return the stream to the caller
         */
        goto done;

        /**
         * error handling code. all detected errors end up here, entering
         * via a goto one of the labels. note that the logic is currently
         * a straight fall-thru scheme (e.g., if entered at error4, the
         * code for error4, error3,...,error1 is all executed).
         **********************************************************************/

error4:         /* make sure locidpair is reusable
                 */
                locidpair->stream = NULL;

error3:         /* close pstream (also, clear ph_open[i2] since the stream
                 * close will also close the pipe handle)
                 */
                (void)fclose( pstream );
                ph_open[ i2 ] = 0;
                pstream = NULL;

error2:         /* close handles on pipe (if they are still open)
                 */

                if ( ph_open[i1] )
                        _close( phdls[i1] );
                if ( ph_open[i2] )
                        _close( phdls[i2] );
done:

        ;}
        __finally {
            _munlock(_POPEN_LOCK);
        }


error1:
        return pstream;
}

#ifndef _UNICODE

/***
*int _pclose(pstream) - wait on a child command and close the stream on the
*   associated pipe
*
*Purpose:
*       Closes pstream then waits on the associated child command. The
*       argument, pstream, must be the return value from a previous call to
*       _popen. _pclose first looks up the process handle of child command
*       started by that _popen and does a cwait on it. Then, it closes pstream
*       and returns the exit status of the child command to the caller.
*
*Entry:
*       FILE *pstream - file stream returned by a previous call to _popen
*
*Exit:
*       If successful, _pclose returns the exit status of the child command.
*       The format of the return value is that same as for cwait, except that
*       the low order and high order bytes are swapped.
*
*       If an error occurs, -1 is returned.
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _pclose (
        FILE *pstream
        )
{
        IDpair *locidpair;        /* pointer to entry in idpairs table */
        int termstat;             /* termination status word */
        int retval = -1;          /* return value (to caller) */
        errno_t save_errno;

        _VALIDATE_RETURN((pstream != NULL), EINVAL, -1);

        if (!_mtinitlocknum(_POPEN_LOCK))
            return -1;
        _mlock(_POPEN_LOCK);
        __try {

        if ((locidpair = idtab(pstream)) == NULL)
        {
                /* invalid pstream, exit with retval == -1
                 */
                errno = EBADF;
                goto done;
        }

        /* close pstream
         */
        (void)fclose(pstream);

        /* wait on the child (copy of the command processor) and all of its
         * children.
         */
        save_errno = errno;
        errno = 0;
        if ( (_cwait(&termstat, locidpair->prochnd, _WAIT_GRANDCHILD) != -1) ||
             (errno == EINTR) )
                retval = termstat;
        errno = save_errno;

        /* Mark the IDpairtable entry as free (note: prochnd was closed by the
         * preceding call to _cwait).
         */
        locidpair->stream = NULL;
        locidpair->prochnd = 0;

        /* only return path!
         */
        done:

        ; }
        __finally {
            _munlock(_POPEN_LOCK);
        }
        return(retval);
}

#endif  /* _UNICODE */

/***
* static IDpair * idtab(FILE *pstream) - find an idpairs table entry
*
*Purpose:
*   Find an entry in the idpairs table.  This function finds the entry the
*   idpairs table entry corresponding to pstream. In the case where pstream
*   is NULL, the entry being searched for is any free entry. In this case,
*   idtab will create the idpairs table if it doesn't exist, or expand it (by
*   exactly one entry) if there are no free entries.
*
*   [MTHREAD NOTE:  This routine assumes that the caller has acquired the
*   idpairs table lock.]
*
*Entry:
*   FILE *pstream - stream corresponding to table entry to be found (if NULL
*                   then find any free table entry)
*
*Exit:
*   if successful, returns a pointer to the idpairs table entry. otherwise,
*   returns NULL.
*
*Exceptions:
*
*******************************************************************************/

static IDpair * __cdecl idtab (
        FILE *pstream
        )
{

        IDpair * pairptr;       /* ptr to entry */
        IDpair * newptr;        /* ptr to newly malloc'd memory */


        /* search the table. if table is empty, appropriate action should
         * fall out automatically.
         */
        for ( pairptr = __idpairs ; pairptr < (__idpairs+__idtabsiz) ; pairptr++ )
                if ( pairptr->stream == pstream )
                        break;

        /* if we found an entry, return it.
         */
        if ( pairptr < (__idpairs + __idtabsiz) )
                return(pairptr);

        /* did not find an entry in the table.  if pstream was NULL, then try
         * creating/expanding the table. otherwise, return NULL. note that
         * when the table is created or expanded, exactly one new entry is
         * produced. this must not be changed unless code is added to mark
         * the extra entries as being free (i.e., set their stream fields to
         * to NULL).
         */
        if ( (pstream != NULL) ||
             ((__idtabsiz + 1) < __idtabsiz) ||
             ((__idtabsiz + 1) >= (SIZE_MAX / sizeof(IDpair))) ||
             ((newptr = (IDpair *)_recalloc_crt((void *)__idpairs, (__idtabsiz + 1),sizeof(IDpair))) == NULL))
                /* either pstream was non-NULL or the attempt to create/expand
                 * the table failed. in either case, return a NULL to indicate
                 * failure.
                 */
                return( NULL );

        __idpairs = newptr;             /* new table ptr */
        pairptr = newptr + __idtabsiz;  /* first new entry */
        __idtabsiz++;                   /* new table size */

        return( pairptr );

}


