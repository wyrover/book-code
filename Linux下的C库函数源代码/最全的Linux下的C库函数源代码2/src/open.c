/***
*open.c - file open
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _open() and _sopen() - open or create a file
*
*******************************************************************************/

#include <sect_attribs.h>
#include <cruntime.h>
#include <oscalls.h>
#include <msdos.h>
#include <errno.h>
#include <fcntl.h>
#include <internal.h>
#include <io.h>
#include <share.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <mtdll.h>
#include <stdarg.h>
#include <tchar.h>

#define UTF16LE_BOM     0xFEFF      /* UTF16 Little Endian Byte Order Mark */
#define UTF16BE_BOM     0xFFFE      /* UTF16 Big Endian Byte Order Mark */
#define BOM_MASK                0xFFFF      /* Mask for testing Byte Order Mark */
#define UTF8_BOM        0xBFBBEF    /* UTF8 Byte Order Mark */
#define UTF16_BOMLEN    2           /* No of Bytes in a UTF16 BOM */
#define UTF8_BOMLEN     3           /* No of Bytes in a UTF8 BOM */

static errno_t __cdecl _tsopen_nolock ( int *,
                                int *,
                                const _TSCHAR *,
                                int,
                                int,
                                int,
                                int );

/***
*int _open(path, flag, pmode) - open or create a file
*
*Purpose:
*       Opens the file and prepares for subsequent reading or writing.
*       the flag argument specifies how to open the file:
*         _O_APPEND -   reposition file ptr to end before every write
*         _O_BINARY -   open in binary mode
*         _O_CREAT -    create a new file* no effect if file already exists
*         _O_EXCL -     return error if file exists, only use with O_CREAT
*         _O_RDONLY -   open for reading only
*         _O_RDWR -     open for reading and writing
*         _O_TEXT -     open in text mode
*         _O_TRUNC -    open and truncate to 0 length (must have write permission)
*         _O_WRONLY -   open for writing only
*         _O_NOINHERIT -handle will not be inherited by child processes.
*       exactly one of _O_RDONLY, _O_WRONLY, _O_RDWR must be given
*
*       The pmode argument is only required when _O_CREAT is specified.  Its
*       flag settings:
*         _S_IWRITE -   writing permitted
*         _S_IREAD -    reading permitted
*         _S_IREAD | _S_IWRITE - both reading and writing permitted
*       The current file-permission maks is applied to pmode before
*       setting the permission (see umask).
*
*       The oflag and mode parameter have different meanings under DOS. See
*       the A_xxx attributes in msdos.inc
*
*       The file mode is opened in _SH_DENYNO share mode
*
*       Note, the _creat() function also uses this function but setting up the
*       correct arguments and calling _open(). _creat() sets the __creat_flag
*       to 1 prior to calling _open() so _open() can return correctly. _open()
*       returns the file handle in eax in this case.
*
*Entry:
*       _TSCHAR *path - file name
*       int flag - flags for _open()
*       int pmode - permission mode for new files
*
*Exit:
*       returns file handle of open file if successful
*       returns -1 (and sets errno) if fails
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _topen (
        const _TSCHAR *path,
        int oflag,
        ...
        )
{
        va_list ap;
        int pmode = 0;
        int fh = -1;
        errno_t retval;
        int unlock_flag = 0;

        _VALIDATE_RETURN( (path != NULL), EINVAL, -1);

        va_start(ap, oflag);
        pmode = va_arg(ap, int);
        va_end(ap);


        __try {
/* Last parameter passed as 0 because we don't want to
validate pmode from open */
            retval = _tsopen_nolock( &unlock_flag,
                                 &fh,
                                 path,
                                 oflag,
                                 _SH_DENYNO,
                                 pmode,
                                 0 );
        }
        __finally {
            if ( unlock_flag )
            {
                if (retval)
                {
                    _osfile(fh) &= ~FOPEN;
                }
                _unlock_fh(fh);
            }
        }

        if (retval)
        {
            errno = retval;
            return -1;
        }

        return fh;

}

errno_t __cdecl _tsopen_helper (
        const _TSCHAR *path,
        int oflag,
        int shflag,
        int pmode,
        int * pfh,
        int bSecure
        )
{

        errno_t retval;
        int unlock_flag = 0;

        _VALIDATE_RETURN_ERRCODE( (pfh != NULL), EINVAL);
        *pfh = -1;
        _VALIDATE_RETURN_ERRCODE( (path != NULL), EINVAL);

        if(bSecure)
            _VALIDATE_RETURN_ERRCODE(((pmode & (~(_S_IREAD | _S_IWRITE))) == 0), EINVAL);

        __try {
            retval = _tsopen_nolock( &unlock_flag,
                                 pfh,
                                 path,
                                 oflag,
                                 shflag,
                                 pmode,
                                 bSecure );
        }
        __finally {
            if ( unlock_flag )
            {
                if (retval)
                {
                    _osfile(*pfh) &= ~FOPEN;
                }
                _unlock_fh(*pfh);
            }
        }

        /* in error case, ensure *pfh is -1 */
        if (retval != 0)
        {
            *pfh = -1;
        }

        return retval;
}

static errno_t __cdecl _tsopen_nolock (
        int *punlock_flag,
        int *pfh,
        const _TSCHAR *path,
        int oflag,
        int shflag,
        int pmode,
        int bSecure
        )
{


        int filepos;                    /* length of file - 1 */
        _TSCHAR ch;                     /* character at end of file */
        char fileflags;                 /* _osfile flags */
        int fmode = 0;
        unsigned int osplatform = 0;

        HANDLE osfh;                    /* OS handle of opened file */
        DWORD fileaccess;               /* OS file access (requested) */
        DWORD fileshare;                /* OS file sharing mode */
        DWORD filecreate;               /* OS method of opening/creating */
        DWORD fileattrib;               /* OS file attribute flags */
        DWORD isdev;                    /* device indicator in low byte */
        SECURITY_ATTRIBUTES SecurityAttributes;
        char tmode = __IOINFO_TM_ANSI;  /* textmode - ANSI/UTF-8/UTF-16 */
        errno_t retvalue = 0;


        SecurityAttributes.nLength = sizeof( SecurityAttributes );
        SecurityAttributes.lpSecurityDescriptor = NULL;

        if (oflag & _O_NOINHERIT) {
            SecurityAttributes.bInheritHandle = FALSE;
            fileflags = FNOINHERIT;
        }
        else {
            SecurityAttributes.bInheritHandle = TRUE;
            fileflags = 0;
        }

        _ERRCHECK(_get_fmode(&fmode));
        _ERRCHECK(_get_osplatform(&osplatform));

        /* figure out binary/text mode */
        if ((oflag & _O_BINARY) == 0)
            if (oflag & (_O_TEXT | _O_WTEXT | _O_U16TEXT | _O_U8TEXT))
                fileflags |= FTEXT;
            else if (fmode != _O_BINARY)   /* check default mode */
                fileflags |= FTEXT;

        /*
         * decode the access flags
         */
        switch( oflag & (_O_RDONLY | _O_WRONLY | _O_RDWR) ) {

            case _O_RDONLY:         /* read access */
                    fileaccess = GENERIC_READ;
                    break;
            case _O_WRONLY:         /* write access */
                    fileaccess = GENERIC_WRITE;
                    break;
            case _O_RDWR:           /* read and write access */
                    fileaccess = GENERIC_READ | GENERIC_WRITE;
                    break;
            default:                /* error, bad oflag */
                    _doserrno = 0L; /* not an OS error */
                    *pfh = -1;
                    _VALIDATE_RETURN_ERRCODE(( "Invalid open flag" , 0 ), EINVAL);

        }

        /*
         * decode sharing flags
         */
        switch ( shflag ) {

            case _SH_DENYRW:        /* exclusive access */
                fileshare = 0L;
                break;

            case _SH_DENYWR:        /* share read access */
                fileshare = FILE_SHARE_READ;
                break;

            case _SH_DENYRD:        /* share write access */
                fileshare = FILE_SHARE_WRITE;
                break;

            case _SH_DENYNO:        /* share read and write access */
                fileshare = FILE_SHARE_READ | FILE_SHARE_WRITE;
                break;

            case _SH_SECURE:       /* share read access only if read-only */
                if (fileaccess == GENERIC_READ)
                    fileshare = FILE_SHARE_READ;
                else
                    fileshare = 0L;
                break;

            default:                /* error, bad shflag */
                _doserrno = 0L; /* not an OS error */
                *pfh = -1;
                _VALIDATE_RETURN_ERRCODE(( "Invalid sharing flag" , 0 ), EINVAL);
        }

        /*
         * decode open/create method flags
         */
        switch ( oflag & (_O_CREAT | _O_EXCL | _O_TRUNC) ) {
            case 0:
            case _O_EXCL:                   // ignore EXCL w/o CREAT
                filecreate = OPEN_EXISTING;
                break;

            case _O_CREAT:
                filecreate = OPEN_ALWAYS;
                break;

            case _O_CREAT | _O_EXCL:
            case _O_CREAT | _O_TRUNC | _O_EXCL:
                filecreate = CREATE_NEW;
                break;

            case _O_TRUNC:
            case _O_TRUNC | _O_EXCL:        // ignore EXCL w/o CREAT
                filecreate = TRUNCATE_EXISTING;
                break;

            case _O_CREAT | _O_TRUNC:
                filecreate = CREATE_ALWAYS;
                break;

            default:
                // this can't happen ... all cases are covered
                _doserrno = 0L;
                *pfh = -1;
                _VALIDATE_RETURN_ERRCODE(( "Invalid open flag" , 0 ), EINVAL);
        }

        /*
         * decode file attribute flags if _O_CREAT was specified
         */
        fileattrib = FILE_ATTRIBUTE_NORMAL;     /* default */

        if ( oflag & _O_CREAT ) {

            if ( !((pmode & ~_umaskval) & _S_IWRITE) )
                fileattrib = FILE_ATTRIBUTE_READONLY;
        }

        /*
         * Set temporary file (delete-on-close) attribute if requested.
         */
        if ( oflag & _O_TEMPORARY ) {
            fileattrib |= FILE_FLAG_DELETE_ON_CLOSE;
            fileaccess |= DELETE;
            if (osplatform == VER_PLATFORM_WIN32_NT )
                fileshare  |= FILE_SHARE_DELETE;
        }

        /*
         * Set temporary file (delay-flush-to-disk) attribute if requested.
         */
        if ( oflag & _O_SHORT_LIVED )
            fileattrib |= FILE_ATTRIBUTE_TEMPORARY;

        /*
         * Set sequential or random access attribute if requested.
         */
        if ( oflag & _O_SEQUENTIAL )
            fileattrib |= FILE_FLAG_SEQUENTIAL_SCAN;
        else if ( oflag & _O_RANDOM )
            fileattrib |= FILE_FLAG_RANDOM_ACCESS;

        /*
         * get an available handle.
         *
         * multi-thread note: the returned handle is locked!
         */
        if ( (*pfh = _alloc_osfhnd()) == -1 ) {
            _doserrno = 0L;         /* not an OS error */
            *pfh = -1;
            errno = EMFILE;
            return errno;          /* return error to caller */
        }

        /* Beyond this do not set *pfh = -1 on errors for MT.
            Because the caller needs to release the lock on the
            handle */

        *punlock_flag = 1;

        /*
         * try to open/create the file
         */
        if ( (osfh = CreateFile( (LPTSTR)path,
                                 fileaccess,
                                 fileshare,
                                 &SecurityAttributes,
                                 filecreate,
                                 fileattrib,
                                 NULL ))
             == (HANDLE)(-1) )
        {
            /*
             * OS call to open/create file failed! map the error, release
             * the lock, and return -1. note that it's not necessary to
             * call _free_osfhnd (it hasn't been used yet), but we do need
             * to clear the FOPEN that was set by _alloc_osfhnd.
             */
            _osfile(*pfh) &= ~FOPEN;
            _dosmaperr(GetLastError());
            retvalue = errno;
            goto exit;
        }

        /* find out what type of file (file/device/pipe) */
        if ( (isdev = GetFileType(osfh)) == FILE_TYPE_UNKNOWN ) {
            _osfile(*pfh) &= ~FOPEN;
            CloseHandle(osfh);
            _dosmaperr(GetLastError());
            retvalue = errno;
            goto exit;
        }

        /* use isdev value to set flags */
        if (isdev == FILE_TYPE_CHAR)
            fileflags |= FDEV;
        else if (isdev == FILE_TYPE_PIPE) {
            fileflags |= FPIPE;
        }

        /*
         * the file is open. now, set the info in _osfhnd array
         */
        _set_osfhnd(*pfh, (intptr_t)osfh);

        /*
         * mark the handle as open. store flags gathered so far in _osfile
         * array.
         */
        fileflags |= FOPEN;
        _osfile(*pfh) = fileflags;

        /* TextMode set to ANSI by default, if we find a BOM, then
        we reset it to the appropriate type below */
        _textmode(*pfh) = __IOINFO_TM_ANSI;

        if ( !(fileflags & (FDEV|FPIPE)) && (fileflags & FTEXT) &&
             (oflag & _O_RDWR) )
        {
            /* We have a text mode file.  If it ends in CTRL-Z, we wish to
               remove the CTRL-Z character, so that appending will work.
               We do this by seeking to the end of file, reading the last
               byte, and shortening the file if it is a CTRL-Z. */

            if ((filepos = _lseek_nolock(*pfh, -1, SEEK_END)) == -1) {
                /* OS error -- should ignore negative seek error,
                   since that means we had a zero-length file. */
                if (_doserrno != ERROR_NEGATIVE_SEEK) {
                    _close_nolock(*pfh);
                    retvalue = errno;
                    goto exit;
                }
            }
            else {
                /* Seek was OK, read the last char in file. The last
                   char is a CTRL-Z if and only if _read returns 0
                   and ch ends up with a CTRL-Z. */
                ch = 0;
                if (_read_nolock(*pfh, &ch, 1) == 0 && ch == 26) {
                    /* read was OK and we got CTRL-Z! Wipe it
                       out! */
                    if (_chsize_nolock(*pfh,filepos) == -1)
                    {
                        _close_nolock(*pfh);
                        retvalue = errno;
                        goto exit;
                    }
                }

                /* now rewind the file to the beginning */
                if ((filepos = _lseek_nolock(*pfh, 0, SEEK_SET)) == -1) {
                    _close_nolock(*pfh);
                    retvalue = errno;
                    goto exit;
                }
            }
        }

        if ( fileflags & FTEXT) {

            int count;

            /* Check for FTEXT makes sure that _O_BINARY is not set in
               oflag and _fmode. ASSERT is for internal validation to
               catch any changes in future that may invalidate this
               assumption which will then require a change to the condition
               below.
            */
            _ASSERTE( (oflag & (_O_BINARY) ) == 0 && fmode != _O_BINARY);

            /* Set default tmode per oflag. BOM will change the defualt. */
            /* If oflag does not specify file type get type from _fmode  */
            if ( (oflag & (_O_TEXT | _O_WTEXT | _O_U16TEXT | _O_U8TEXT) ) == 0 )
            {
                /* Type specified in default mode? */
                if ( (fmode & (_O_TEXT | _O_WTEXT | _O_U16TEXT | _O_U8TEXT) ) == 0)
                    oflag |= _O_TEXT; /* default to ANSI */
                else
                    oflag |= fmode & (_O_TEXT | _O_WTEXT | _O_U16TEXT | _O_U8TEXT);
            }

            switch ( oflag & (_O_TEXT | _O_WTEXT | _O_U16TEXT | _O_U8TEXT) )
            {
            case _O_TEXT :
                tmode = __IOINFO_TM_ANSI;
                break;
            case _O_WTEXT :
            case _O_WTEXT | _O_TEXT :
                if ( (oflag & (_O_WRONLY | _O_CREAT | _O_TRUNC)) ==
                    (_O_WRONLY | _O_CREAT | _O_TRUNC) )
                    tmode = __IOINFO_TM_UTF16LE;
                break;
            case _O_U16TEXT :
            case _O_U16TEXT | _O_TEXT :
                tmode = __IOINFO_TM_UTF16LE;
                break;

            case _O_U8TEXT :
            case _O_U8TEXT | _O_TEXT :
                tmode = __IOINFO_TM_UTF8;
                break;
            }

            /* If the file hasn't been opened with the UNICODE flags then we
            have nothing to do - textmode's already set to default specified in oflag */
            if((oflag & (_O_WTEXT | _O_U16TEXT | _O_U8TEXT)) != 0) {

                int bom = 0;
                int bWriteBom = 0;
                int bCheckBom = 0;

                if ( !(fileflags & FDEV) ) {
                        switch ( fileaccess & (GENERIC_READ | GENERIC_WRITE) ) {

                        case GENERIC_READ :
                                {
                                        bCheckBom = 1;
                                        break;
                                }

                        case GENERIC_WRITE :
                                {
                                        switch ( filecreate ) {
                                            /* Write BOM if empty file */
                                        case OPEN_EXISTING :
                                        case OPEN_ALWAYS :
                                        {
                                            /* Check if the file contains at least one byte */
                                            /* Fall through otherwise */
                                            if( _lseeki64_nolock(*pfh, 0, SEEK_END) != 0) {
                                                if(_lseeki64_nolock(*pfh, 0, SEEK_SET) == -1) {
                                                    _close_nolock(*pfh);
                                                    retvalue = errno;
                                                    goto exit;
                                                }
                                                break;
                                            }
                                        }

                                        /* New or truncated file. Always write BOM */
                                        case CREATE_NEW :
                                        case CREATE_ALWAYS :
                                        case TRUNCATE_EXISTING :
                                        {
                                            bWriteBom = 1;
                                            break;
                                        }
                                        }
                                        break;
                                }

                        case GENERIC_READ | GENERIC_WRITE :
                                {
                                        switch ( filecreate ) {
                                            /* Check for existing BOM, Write BOM if empty file */
                                        case OPEN_EXISTING :
                                        case OPEN_ALWAYS :
                                        {
                                            /* Check if the file contains at least one byte */
                                            /* Fall through otherwise */
                                            if( _lseeki64_nolock(*pfh, 0, SEEK_END) != 0) {
                                                if(_lseeki64_nolock(*pfh, 0, SEEK_SET) == -1) {
                                                    _close_nolock(*pfh);
                                                    retvalue = errno;
                                                    goto exit;
                                                }
                                                bCheckBom = 1;
                                            }
                                            else {
                                                bWriteBom = 1; /* reset if file is not zero size */
                                            }
                                            break;
                                        }

                                        /* New or truncated file. Always write BOM */
                                        case CREATE_NEW :
                                        case TRUNCATE_EXISTING :
                                        case CREATE_ALWAYS :
                                        {
                                            bWriteBom = 1;
                                            break;
                                        }
                                        }
                                        break;
                                }
                        }
                }

                if ( bCheckBom ) {
                    count = _read_nolock(*pfh, &bom, UTF8_BOMLEN);

                    /*
                       Internal Validation.
                       This branch should never be taken if bWriteBom is 1 and count > 0
                    */
                    if (count > 0 && bWriteBom == 1) {
                        _ASSERTE(0 && "Internal Error");
                        bWriteBom = 0;
                    }

                    switch(count) {

                        case -1 :
                            _close_nolock(*pfh);
                            retvalue = errno;
                            goto exit;

                        case UTF8_BOMLEN:
                            if(bom == UTF8_BOM) {
                                tmode = __IOINFO_TM_UTF8;
                                break;
                            }

                        case UTF16_BOMLEN:
                            if((bom & BOM_MASK) == UTF16BE_BOM) {
                                _close_nolock(*pfh);
                                _ASSERTE(0 && "Only UTF-16 little endian & UTF-8 is supported for reads");
                                retvalue = errno = EINVAL;
                                goto exit;
                            }

                            if((bom & BOM_MASK) == UTF16LE_BOM) {
                                /* We have read 3 bytes, so we should seek back
                                1 byte */
                                if((filepos = _lseek_nolock(*pfh, UTF16_BOMLEN, SEEK_SET)) == -1) {
                                    _close_nolock(*pfh);
                                    retvalue = errno;
                                    goto exit;
                                }
                                tmode = __IOINFO_TM_UTF16LE;
                                break;
                            }

                            /* Fall through to default case to lseek to
                            beginning of file */

                        default:
                            if((filepos = _lseek_nolock(*pfh, 0, SEEK_SET)) == -1) {
                                /* No BOM, so we should seek back to the beginning
                                of the file */
                                _close_nolock(*pfh);
                                retvalue = errno;
                                goto exit;
                            }
                            break;
                    }
                }
                if( bWriteBom ) {
                    int written = 0;
                    int bomlen = 0;
                    bom = 0;

                    /* If we are creating a new file, we write a UTF-16LE or UTF8 BOM */
                    switch (tmode)
                    {
                    case __IOINFO_TM_UTF16LE :
                        {
                            bom = UTF16LE_BOM;
                            bomlen = UTF16_BOMLEN;
                            break;
                        }
                    case __IOINFO_TM_UTF8 :
                        {
                            bom = UTF8_BOM;
                            bomlen = UTF8_BOMLEN;
                            break;
                        }
                    }

                    while (bomlen > written) {
                        int tmp = 0;
                        /*
                         * Note that write may write less than bomlen characters, but not really fail.
                         * Retry till write fails or till we wrote all the characters.
                         */
                        if ((tmp = _write(*pfh, ((char *)(&bom)) + written, bomlen - written)) == -1) {
                            _close_nolock(*pfh);
                            retvalue = errno;
                            goto exit;
                        }
                        written += tmp;
                    }
                }
            }
        }

        _textmode(*pfh) = tmode;
        _tm_unicode(*pfh) = ((oflag & _O_WTEXT) == 0) ? 0 : 1;

        /*
         * Set FAPPEND flag if appropriate. Don't do this for devices or pipes.
         */
        if ( !(fileflags & (FDEV|FPIPE)) && (oflag & _O_APPEND) )
            _osfile(*pfh) |= FAPPEND;

exit:
        return retvalue;
}

/***
*int _sopen(path, oflag, shflag, pmode) - opne a file with sharing
*
*Purpose:
*       Opens the file with possible file sharing.
*       shflag defines the sharing flags:
*         _SH_COMPAT -  set compatability mode
*         _SH_DENYRW -  deny read and write access to the file
*         _SH_DENYWR -  deny write access to the file
*         _SH_DENYRD -  deny read access to the file
*         _SH_DENYNO -  permit read and write access
*
*       Other flags are the same as _open().
*
*       SOPEN is the routine used when file sharing is desired.
*
*Entry:
*       _TSCHAR *path - file to open
*       int oflag -     open flag
*       int shflag -    sharing flag
*       int pmode -     permission mode (needed only when creating file)
*
*Exit:
*       returns file handle for the opened file
*       returns -1 and sets errno if fails.
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _tsopen (
        const _TSCHAR *path,
        int oflag,
        int shflag,
        ...
        )
{
        va_list ap;
        int pmode = 0;
        int fh = -1;
        errno_t e;

        va_start(ap, shflag);
        pmode = va_arg(ap, int);
        va_end(ap);

        /* Last parameter passed as 0 because we don't want to
        validate pmode from open */

        e = _tsopen_helper(path, oflag, shflag, pmode, &fh, 0);

        return e ? -1 : fh;
}

/***
*int _sopen_s(pfh, path, oflag, shflag, pmode) - open a file with sharing
*
*Purpose:
*       Check comments for _sopen. This works similiar to _sopen except
*       it validates the pmode.
*
*Entry:
*       _TSCHAR *path - file to open
*       int oflag -     open flag
*       int shflag -    sharing flag
*       int pmode -     permission mode (needed only when creating file)
*       int * pfh -     pointer to write the file handle to.
*
*Exit:
*       returns 0 on success & sets pfile
*       returns errno_t on failure.
*       On success, the file handle is written to the pfh parameter.
*
*Exceptions:
*
*******************************************************************************/

errno_t __cdecl _tsopen_s (
        int * pfh,
        const _TSCHAR *path,
        int oflag,
        int shflag,
        int pmode
        )
{
    /* Last parameter passed as 1 because we want to validate
     * pmode from the secure open_s */
    return _tsopen_helper(path, oflag, shflag, pmode, pfh, 1);
}
