/*
 * testfdi.c
 *
 * Demonstrates how to use the FDI library APIs
 */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <dos.h>
#include <sys/stat.h>

#include "..\..\include\fdi.h"


/*
 * Function prototypes
 */
BOOL    test_fdi(char *cabinet_file);
int     get_percentage(unsigned long a, unsigned long b);
char   *return_fdi_error_string(FDIERROR err);


/*
 * Destination directory for extracted files
 */
char    dest_dir[256];


void main(int argc, char **argv)
{
    if (argc != 3) {
        printf(
            "TESTFDI - Demonstrates how to use the FDI library API calls\n"
            "\n"
            "Usage: TESTFDI cabinet dest_dir\n"
            "\n"
            "Where <cabinet> is the name of a cabinet file, and <dest_dir>\n"
            "is the destination for the files extracted\n"
            "\n"
            "  e.g. testfdi c:\\test1.cab c:\\\n"
            "\n"
        );
        exit(0);
    }

    strcpy(dest_dir, argv[2]);

    if (test_fdi(argv[1]) == TRUE)
        printf("TESTFDI was successful\n");
    else
        printf("TESTFDI failed\n");

    exit(0);
}


/*
 * Memory allocation function
 */
FNALLOC(mem_alloc)
{
    return malloc(cb);
}


/*
 * Memory free function
 */
FNFREE(mem_free)
{
    free(pv);
}


FNOPEN(file_open)
{
    return _open(pszFile, oflag, pmode);
}


FNREAD(file_read)
{
    return _read(hf, pv, cb);
}


FNWRITE(file_write)
{
    return _write(hf, pv, cb);
}


FNCLOSE(file_close)
{
    return _close(hf);
}


FNSEEK(file_seek)
{
    return _lseek(hf, dist, seektype);
}


FNFDINOTIFY(notification_function)
{
    switch (fdint) {
    case fdintCABINET_INFO: // general information about the cabinet
        printf(
            "fdintCABINET_INFO\n"
            "  next cabinet     = %s\n"
            "  next disk        = %s\n"
            "  cabinet path     = %s\n"
            "  cabinet set ID   = %d\n"
            "  cabinet # in set = %d (zero based)\n"
            "\n",
            pfdin->psz1,
            pfdin->psz2,
            pfdin->psz3,
            pfdin->setID,
            pfdin->iCabinet
        );
        return 0;

    case fdintPARTIAL_FILE: // first file in cabinet is continuation
        printf(
            "fdintPARTIAL_FILE\n"
            "   name of continued file            = %s\n"
            "   name of cabinet where file starts = %s\n"
            "   name of disk where file starts    = %s\n",
            pfdin->psz1,
            pfdin->psz2,
            pfdin->psz3
        );
        return 0;

    case fdintCOPY_FILE: {  // file to be copied
        int     response;
        int     handle;
        char    destination[256];
        printf(
            "fdintCOPY_FILE\n"
            "  file name in cabinet = %s\n"
            "  uncompressed file size = %d\n"
            "  copy this file? (y/n): ",
            pfdin->psz1,
            pfdin->cb
        );

        do {
            response = getc(stdin);
            response = toupper(response);
        } while (response != 'Y' && response != 'N');

        printf("\n");

        if (response == 'Y') {
            sprintf(
                destination,
                "%s%s",
                dest_dir,
                pfdin->psz1
            );
            handle = file_open(
                         destination,
                         _O_BINARY | _O_CREAT | _O_WRONLY | _O_SEQUENTIAL,
                         _S_IREAD | _S_IWRITE
                     );
            return handle;
        } else {
            return 0; /* skip file */
        }
    }

    case fdintCLOSE_FILE_INFO: { // close the file, set relevant info
        HANDLE  handle;
        DWORD   attrs;
        char    destination[256];
        printf(
            "fdintCLOSE_FILE_INFO\n"
            "   file name in cabinet = %s\n"
            "\n",
            pfdin->psz1
        );
        sprintf(
            destination,
            "%s%s",
            dest_dir,
            pfdin->psz1
        );
        file_close(pfdin->hf);
        /*
         * Set date/time
         *
         * Need Win32 type handle for to set date/time
         */
        handle = CreateFile(
                     destination,
                     GENERIC_READ | GENERIC_WRITE,
                     FILE_SHARE_READ,
                     NULL,
                     OPEN_EXISTING,
                     FILE_ATTRIBUTE_NORMAL,
                     NULL
                 );

        if (handle != INVALID_HANDLE_VALUE) {
            FILETIME    datetime;

            if (TRUE == DosDateTimeToFileTime(
                    pfdin->date,
                    pfdin->time,
                    &datetime)) {
                FILETIME    local_filetime;

                if (TRUE == LocalFileTimeToFileTime(
                        &datetime,
                        &local_filetime)) {
                    (void) SetFileTime(
                        handle,
                        &local_filetime,
                        NULL,
                        &local_filetime
                    );
                }
            }

            CloseHandle(handle);
        }

        /*
         * Mask out attribute bits other than readonly,
         * hidden, system, and archive, since the other
         * attribute bits are reserved for use by
         * the cabinet format.
         */
        attrs = pfdin->attribs;
        attrs &= (_A_RDONLY | _A_HIDDEN | _A_SYSTEM | _A_ARCH);
        (void) SetFileAttributes(
            destination,
            attrs
        );
        return TRUE;
    }

    case fdintNEXT_CABINET: // file continued to next cabinet
        printf(
            "fdintNEXT_CABINET\n"
            "   name of next cabinet where file continued = %s\n"
            "   name of next disk where file continued    = %s\n"
            "   cabinet path name                         = %s\n"
            "\n",
            pfdin->psz1,
            pfdin->psz2,
            pfdin->psz3
        );
        return 0;
    }

    return 0;
}


BOOL test_fdi(char *cabinet_fullpath)
{
    HFDI            hfdi;
    ERF             erf;
    FDICABINETINFO  fdici;
    int             hf;
    char            *p;
    char            cabinet_name[256];
    char            cabinet_path[256];
    hfdi = FDICreate(
               mem_alloc,
               mem_free,
               file_open,
               file_read,
               file_write,
               file_close,
               file_seek,
               cpu80386,
               &erf
           );

    if (hfdi == NULL) {
        printf("FDICreate() failed: code %d [%s]\n",
               erf.erfOper, return_fdi_error_string(erf.erfOper)
              );
        return FALSE;
    }

    /*
     * Is this file really a cabinet?
     */
    hf = file_open(
             cabinet_fullpath,
             _O_BINARY | _O_RDONLY | _O_SEQUENTIAL,
             0
         );

    if (hf == -1) {
        (void) FDIDestroy(hfdi);
        printf("Unable to open '%s' for input\n", cabinet_fullpath);
        return FALSE;
    }

    if (FALSE == FDIIsCabinet(
            hfdi,
            hf,
            &fdici)) {
        /*
         * No, it's not a cabinet!
         */
        _close(hf);
        printf(
            "FDIIsCabinet() failed: '%s' is not a cabinet\n",
            cabinet_fullpath
        );
        (void) FDIDestroy(hfdi);
        return FALSE;
    } else {
        _close(hf);
        printf(
            "Information on cabinet file '%s'\n"
            "   Total length of cabinet file : %d\n"
            "   Number of folders in cabinet : %d\n"
            "   Number of files in cabinet   : %d\n"
            "   Cabinet set ID               : %d\n"
            "   Cabinet number in set        : %d\n"
            "   RESERVE area in cabinet?     : %s\n"
            "   Chained to prev cabinet?     : %s\n"
            "   Chained to next cabinet?     : %s\n"
            "\n",
            cabinet_fullpath,
            fdici.cbCabinet,
            fdici.cFolders,
            fdici.cFiles,
            fdici.setID,
            fdici.iCabinet,
            fdici.fReserve == TRUE ? "yes" : "no",
            fdici.hasprev == TRUE ? "yes" : "no",
            fdici.hasnext == TRUE ? "yes" : "no"
        );
    }

    p = strrchr(cabinet_fullpath, '\\');

    if (p == NULL) {
        strcpy(cabinet_name, cabinet_fullpath);
        strcpy(cabinet_path, "");
    } else {
        strcpy(cabinet_name, p + 1);
        strncpy(cabinet_path, cabinet_fullpath, (int)(p - cabinet_fullpath) + 1);
        cabinet_path[(int)(p - cabinet_fullpath) + 1 ] = 0;
    }

    if (TRUE != FDICopy(
            hfdi,
            cabinet_name,
            cabinet_path,
            0,
            notification_function,
            NULL,
            NULL)) {
        printf(
            "FDICopy() failed: code %d [%s]\n",
            erf.erfOper, return_fdi_error_string(erf.erfOper)
        );
        (void) FDIDestroy(hfdi);
        return FALSE;
    }

    if (FDIDestroy(hfdi) != TRUE) {
        printf(
            "FDIDestroy() failed: code %d [%s]\n",
            erf.erfOper, return_fdi_error_string(erf.erfOper)
        );
        return FALSE;
    }

    return TRUE;
}


char *return_fdi_error_string(FDIERROR err)
{
    switch (err) {
    case FDIERROR_NONE:
        return "No error";

    case FDIERROR_CABINET_NOT_FOUND:
        return "Cabinet not found";

    case FDIERROR_NOT_A_CABINET:
        return "Not a cabinet";

    case FDIERROR_UNKNOWN_CABINET_VERSION:
        return "Unknown cabinet version";

    case FDIERROR_CORRUPT_CABINET:
        return "Corrupt cabinet";

    case FDIERROR_ALLOC_FAIL:
        return "Memory allocation failed";

    case FDIERROR_BAD_COMPR_TYPE:
        return "Unknown compression type";

    case FDIERROR_MDI_FAIL:
        return "Failure decompressing data";

    case FDIERROR_TARGET_FILE:
        return "Failure writing to target file";

    case FDIERROR_RESERVE_MISMATCH:
        return "Cabinets in set have different RESERVE sizes";

    case FDIERROR_WRONG_CABINET:
        return "Cabinet returned on fdintNEXT_CABINET is incorrect";

    case FDIERROR_USER_ABORT:
        return "User aborted";

    default:
        return "Unknown error";
    }
}
