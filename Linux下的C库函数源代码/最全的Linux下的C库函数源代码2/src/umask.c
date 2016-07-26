/***
*umask.c - set file permission mask
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _umask() - sets file permission mask of current process*
*       affecting files created by creat, open, or sopen.
*
*******************************************************************************/

#include <cruntime.h>
#include <internal.h>
#include <io.h>
#include <sys\stat.h>

/***
*errno_t _umask(mode, poldmode) - set the file mode mask
*
*Purpose :
*    Works similiar to umask except it validates input params.
*
*
*******************************************************************************/

errno_t __cdecl _umask_s (
        int mode, int * poldmode
        )
{
        _VALIDATE_RETURN_ERRCODE((poldmode != NULL), EINVAL);
        *poldmode = _umaskval;
        _VALIDATE_RETURN_ERRCODE(((mode & ~(_S_IREAD | _S_IWRITE)) == 0), EINVAL);

        /* only user read/write permitted */
        mode &= (_S_IREAD | _S_IWRITE);
        _umaskval = mode;
        return 0;
}

/***
*int _umask(mode) - set the file mode mask
*
*Purpose:
*       Sets the file-permission mask of the current process* which
*       modifies the permission setting of new files created by creat,
*       open, or sopen.
*
*Entry:
*       int mode - new file permission mask
*                  may contain _S_IWRITE, _S_IREAD, _S_IWRITE | _S_IREAD.
*                  The S_IREAD bit has no effect under Win32
*
*Exit:
*       returns the previous setting of the file permission mask.
*
*Exceptions:
*
*******************************************************************************/
int __cdecl _umask (
        int mode
        )
{
        int oldmode = 0;

    /* silently ignore non-Windows modes */
        mode &= (_S_IREAD | _S_IWRITE);

        _umask_s(mode, &oldmode);
        return oldmode;
}
