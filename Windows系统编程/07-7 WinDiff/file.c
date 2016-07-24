

/****************************** Module Header *******************************
* Module Name: FILE.C
*
* An object representing a file and the lines of text it contains.
*
* Functions:
*
* file_new()
* file_getdiritem()
* file_delete()
* file_getlinelist()
* file_discardlines()
* file_reset()
* file_readlines()
*
* Comments:
*
* A FILEDATA object is initialised with a DIRITEM handle from which it
* can get a filename. It knows how to supply a list of LINE handles for the
* lines of text in the file.
*
* The file is read into memory optionally on creation of the FILEDATA object:
* otherwise, at the first call to file_getlinelist. It can be discarded
* by calling file_discardlines: in this case, it will be re-read next time
* file_getlinelist is called.
*
* Calling file_reset will cause line_reset to be called for all lines
* in the list. This clears any links.
*
* We allocate all memory from a gmem* heap hHeap, assumed to be declared and
* initialised elsewhere.
*
****************************************************************************/

#include <windows.h>
#include <stdlib.h>
#include <string.h>

#include "gutils.h"
#include "windiff.h"
#include "list.h"
#include "line.h"
#include "scandir.h"
#include "file.h"

extern HANDLE hHeap;

struct filedata {

        DIRITEM diritem;        /* handle to file name information */
        LIST lines;             /* NULL if lines not read in */
};


void file_readlines(FILEDATA fd);

/****************************************************************************
 * Function: file_new
 *
 * Purpose:
 * 
 * Creates a new FILEDATA, based on a DIRITEM. the filedata will retain
 * the diritem handle for use in fetching filenames and handles.
 *
 * If the bRead is set, the file will be read into memory. If not, this
 * will be done during the first call to file_getlines.
 *
 ***************************************************************************/
FILEDATA
file_new(DIRITEM fiName, BOOL bRead)
{
        FILEDATA fd;

        fd = (FILEDATA) gmem_get(hHeap, sizeof(struct filedata));
        if (fd == NULL) {
                return(NULL);
        }

        fd->diritem = fiName;
        fd->lines = NULL;

        if (bRead) {
                file_readlines(fd);
        }

        return(fd);
}

/****************************************************************************
 * Function: file_getdiritem
 *
 * Purpose:
 * 
 * Returns a handle to the DIRITEM used to create this FILEDATA
 *
 ***************************************************************************/
DIRITEM
file_getdiritem(FILEDATA fd)
{
        if (fd == NULL) {
                return(NULL);
        }

        return(fd->diritem);
}


/****************************************************************************
 * Function: file_delete
 *
 * Purpose:
 * 
 * Deletes a filedata and its associated list of lines. Note that the diritem
 * is not deleted (this is owned by the DIRLIST, and will be deleted
 * when the DIRLIST is deleted)
 *
 ***************************************************************************/
void
file_delete(FILEDATA fd)
{
        if (fd == NULL) {
                return;
        }

        /* throw away the line list, if there is one */
        file_discardlines(fd);

        gmem_free(hHeap, (LPSTR) fd, sizeof(struct filedata));
}

/****************************************************************************
 * Function: file_getlinelist
 *
 * Purpose:
 * 
 * Returns a handle to a list of lines in this file. The items in the
 * list are LINE handles.
 *
 * The first call to this function will cause the file to be read into
 * memory if bRead was FALSE on the call to file_new, or if file_discardlines
 * has since been called.
 *
 * The list of lines returned should not be deleted except by calls to
 * file_delete or file_discardlines.
 *
 ***************************************************************************/
LIST
file_getlinelist(FILEDATA fd)
{
        if (fd == NULL) {
                return NULL;
        }

        if (fd->lines == NULL) {
                file_readlines(fd);
        }
        return(fd->lines);
}


/****************************************************************************
 * Function: file_discardlines
 *
 * Purpose:
 * 
 * Discards the list of lines associated with a file. This will cause
 * the file to be re-read next time file_getlinelist is called.
 *
 ***************************************************************************/
void
file_discardlines(FILEDATA fd)
{
        LINE line;

        if (fd == NULL) {
                return;
        }

        if (fd->lines != NULL) {

                /* clear each line to free any memory associated
                 * with them, then discard the entire list
                 */
                List_TRAVERSE(fd->lines, line) {
                        line_delete(line);
                }
                List_Destroy(&fd->lines);
        }

        /* this is probably done in List_Destroy, but better do it anyway*/
        fd->lines = NULL;
}


/****************************************************************************
 * Function: file_reset
 *
 * Purpose:
 * 
 * Forces a reset of each line in the list. The function line_reset discards 
 * links between lines, and any hashcode information. This would be used if
 * the compare options or hashcode options have changed.
 *
 ***************************************************************************/
void
file_reset(FILEDATA fd)
{
        LINE line;

        if (fd == NULL) {
                return;
        }

        if (fd->lines != NULL) {

                List_TRAVERSE(fd->lines, line)  {
                        line_reset(line);
                }
        }
}


/****************************************************************************
 * Function: file_readlines
 *
 * Purpose:
 * 
 * Reads the file into a list of lines.
 *
 * Comments:
 *
 * We use the buffered read functions to read a block at a time, and
 * return us a pointer to a line within the block. The line we are
 * pointed to is not null terminated. from this we do a line_new: this
 * will make a copy of the text (since we want to re-use the buffer), and
 * will null-terminate its copy.
 *
 *
 ***************************************************************************/
void
file_readlines(FILEDATA fd)
{
        LPSTR textp;
        int fh;
        FILEBUFFER fbuf;
        int linelen;
        int linenr = 1;
        HCURSOR hcurs;

        hcurs = SetCursor(LoadCursor(NULL, IDC_WAIT));

        /* open the file */
        fh = dir_openfile(fd->diritem);

        if (fh < 0) {
                SetCursor(hcurs);
                return;
        }
        /* initialise the file buffering */
        fbuf = readfile_new(fh);


        /* make an empty list for the files */
        fd->lines = List_Create();

        while ( (textp = readfile_next(fbuf, &linelen)) != NULL) {

                line_new(textp, linelen, linenr++, fd->lines);

        }

        /* close filehandle and free buffer */
        readfile_delete(fbuf);

        dir_closefile(fd->diritem, fh);

        SetCursor(hcurs);
}






