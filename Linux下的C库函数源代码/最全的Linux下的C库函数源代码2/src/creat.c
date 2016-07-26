/***
*creat.c - create a new file or truncate existing file
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _creat() - create new file
*
*******************************************************************************/

#include <cruntime.h>
#include <io.h>
#include <fcntl.h>
#include <share.h>
#include <tchar.h>

/***
*int _creat(path, pmode) - create a new file
*
*Purpose:
*       If file specified does not exist, _creat creates a new file
*       with the given permission setting and opens it for writing.
*       If the file already exists and its permission allows writing,
*       _creat truncates it to 0 length and open it for writing.
*       The only Xenix mode bit supprted by DOS is user write (S_IWRITE).
*
*Entry:
*       _TSCHAR *path - filename to create
*       int pmode - permission mode setting for new file
*
*Exit:
*       returns handle for created file
*       returns -1 and sets errno if fails.
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _tcreat (
        const _TSCHAR *path,
        int pmode
        )
{
        /* creat is just the same as open... */
        int fh = -1;
        errno_t e = _tsopen_s(&fh, path, _O_CREAT + _O_TRUNC + _O_RDWR, _SH_DENYNO, pmode);
        return (e == 0 ? fh : -1);
}
