/***
*drive.c - get and change current drive
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file has the _getdrive() and _chdrive() functions
*
*******************************************************************************/

#include <cruntime.h>
#include <oscalls.h>
#include <mtdll.h>
#include <internal.h>
#include <msdos.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <dbgint.h>


/***
*int _getdrive() - get current drive (1=A:, 2=B:, etc.)
*
*Purpose:
*       Returns the current disk drive
*
*Entry:
*       No parameters.
*
*Exit:
*       returns 1 for A:, 2 for B:, 3 for C:, etc.
*       returns 0 if current drive cannot be determined.
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _getdrive (
        void
        )
{
        ULONG drivenum=0;
        UCHAR curdirstr[_MAX_PATH+1];
        UCHAR *cdirstr = curdirstr;
        int memfree=0,r=0;

        r = GetCurrentDirectory(MAX_PATH+1,(LPTSTR)cdirstr);
        if (r> MAX_PATH) {
            if ((cdirstr= (UCHAR *)_calloc_crt((r+1),sizeof(UCHAR))) == NULL) {
                errno = ENOMEM;
                r = 0;
            } else {
                memfree = 1;
            }
            if (r)
            {
                r = GetCurrentDirectory(r+1,(LPTSTR)cdirstr);
            }
        }
        drivenum = 0;
        if (r)
        {
                if (cdirstr[1] == ':')
                {
                        drivenum = toupper(cdirstr[0]) - 64;
                }
        }
        else
        {
            errno=ENOMEM;
        }

        if (memfree)
        {
            _free_crt(cdirstr);
        }
        return drivenum;
}


/***
*int _chdrive(int drive) - set the current drive (1=A:, 2=B:, etc.)
*
*Purpose:
*       Allows the user to change the current disk drive
*
*Entry:
*       drive - the number of drive which should become the current drive
*
*Exit:
*       returns 0 if successful, else -1
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _chdrive (
        int drive
        )
{
        int retval;
        char  newdrive[3];

        if (drive < 1 || drive > 31) {
            _doserrno = ERROR_INVALID_DRIVE;
            _VALIDATE_RETURN(("Invalid Drive Index",0), EACCES, -1);
        }

        _mlock( _ENV_LOCK );
        __try {

        newdrive[0] = (char)('A' + (char)drive - (char)1);
        newdrive[1] = ':';
        newdrive[2] = '\0';

        /*
         * Set new drive. If current directory on new drive exists, it
         * will become the cwd. Otherwise defaults to root directory.
         */

        if ( SetCurrentDirectory((LPSTR)newdrive) )
            retval = 0;
        else {
            _dosmaperr(GetLastError());
            retval = -1;
        }

        }
        __finally {
            _munlock( _ENV_LOCK );
        }

        return retval;
}
