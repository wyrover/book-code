

/****************************** Module Header *******************************
* Module Name: SCANDIR.C
*
* Scan a directory tree and build a sorted list of filenames within that
* tree.
*
* Functions:
*
* dir_buildlist()
* dir_delete()
* dir_isfile()
* dir_firstitem()
* dir_nextitem()
* dir_findnextfile()
* dir_getrelname()
* dir_getfullname()
* dir_getroot_list()
* dir_getroot_item()
* dir_freerelname()
* dir_freefullname()
* dir_freeroot_list()
* dir_freerootitem()
* dir_getopenname()
* dir_freeopenname()
* dir_openfile()
* dir_closefile()
* dir_filesize()
* dir_startcopy()
* dir_endcopy()
* dir_copy()
* dir_finalelem()
* dir_cleardirect()
* dir_adddirect()
* dir_addfile()
* dir_scan()
* dir_isvaliddir()
* dir_isvalidfile()
* dir_fileinit()
* dir_dirinit()
* dir_getpathsize()
* dir_findnextfile()
*
* Comments:
*
* The call dir_buildlist takes a pathname and returns a handle. Subsequent
* calls to dir_firstitem and dir_nextitem return handles to
* items within the list, from which you can get the name of the
* file (relative to the original pathname, or complete), and filesize.
*
* The list can be either built entirely during the build call, or
* built one directory at a time as required by dir_nextitem calls. This
* option affects only relative performance, and is taken as a
* recommendation only (ie some of the time we will ignore the flag).
*
* The list is ordered alphabetically (case-insensitive using lstrcmpi).
* within any one directory, we list filenames before going on
* to subdirectory contents.
*
* All memory is allocated from a gmem_* heap hHeap declared
* and initialised elsewhere.
*
* The caller gets handles to two things: a DIRLIST, representing the
* entire list of filenames, and a DIRITEM: one item within the list.
*
* From the DIRITEM he can get the filename (including or excluding the
* tree root passed to dir_build*) - and also he can get to the next
* DIRITEM.
*
* We permit lazy building of the tree (usually so the caller can keep
* the user-interface up-to-date as we go along). In this case,
* we need to store information about how far we have scanned and
* what is next to do. We need to scan an entire directory at a time and then
* sort it so we can return files in the correct order.
*
* We scan an entire directory and store it in a DIRECT struct. This contains
* a list of DIRITEMs for the files in the current directory, and a list of
* DIRECTs for the subdirectories (possible un-scanned).
*
* dir_nextitem will use the list functions to get the next DIRITEM on the list.
* When the end of the list is reached, it will use the backpointer back to the
* DIRECT struct to find the next directory to scan.
*
****************************************************************************/

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <direct.h>

#include "gutils.h"
#include "list.h"
#include "scandir.h"
#include "windiff.h"
#include "wdiffrc.h"

/*
 * Hold name and information about a given file (one ITEM in a DIRectory)
 * caller's DIRITEM handle is a pointer to one of these structures
 */
struct diritem {
        LPSTR name;             /* ptr to filename (final element only) */
        long size;              /* filesize */
        struct direct FAR * direct; /* containing directory */
        LPSTR localname;        /* name of temp copy of file */
        BOOL bLocalIsTemp;      /* true if localname is tempfile.
                                 */
};


/* DIRECT: Hold state about directory and current position in list of filenames.
 */
typedef struct direct {
        LPSTR relname;          /* name of dir relative to DIRLIST root */
        DIRLIST head;           /* back ptr (to get fullname) */
        struct direct FAR * parent; /* parent directory (NULL if above tree root)*/

        BOOL bScanned;          /* TRUE if scanned */
        LIST diritems;          /* list of DIRITEMs for files in cur. dir */
        LIST directs;           /* list of DIRECTs for child dirs */

        int pos;                /* where are we begin, files, dirs */
        struct direct FAR * curdir; /* subdir being scanned (ptr to list element)*/
} FAR * DIRECT;

/* Values for direct.pos */
#define DL_FILES        1       /* reading files from the diritems */
#define DL_DIRS         2       /* in the dirs: List_Next on curdir */


/*
 * The DIRLIST handle returned from a build function is in fact
 * a pointer to one of these
 */
struct dirlist {

        char rootname[256];        /* name of root of tree */
        BOOL bFile;             /* TRUE if root of tree is file, not dir */
        DIRECT dot;             /* dir  for '.' - for tree root dir */
};

extern BOOL bAbort;             /* from windiff.c (read only here). */


/* ------ memory allocation ---------------------------------------------*/

/* All memory is allocated from a heap created by the application */
extern HANDLE hHeap;

/*-- forward declaration of internal functions ---------------------------*/

LPSTR dir_finalelem(LPSTR path);
void dir_cleardirect(DIRECT dir);
void dir_adddirect(DIRECT dir, LPSTR path);
void dir_addfile(DIRECT dir, LPSTR path, DWORD size);
void dir_scan(DIRECT dir, BOOL bRecurse);
BOOL dir_isvaliddir(LPSTR path);
BOOL dir_isvalidfile(LPSTR path);
void dir_fileinit(DIRITEM pfile, DIRECT dir, LPSTR path, long size);
void dir_dirinit(DIRECT dir, DIRLIST head, DIRECT parent, LPSTR name);
long dir_getpathsize(LPSTR path);
DIRITEM dir_findnextfile(DIRLIST dl, DIRECT curdir);



/***************************************************************************
 * Function: dir_buildlist
 *
 * Purpose:
 *
 * Build a list of filenames
 *
 * Optionally build the list on demand, in which case we scan the
 * entire directory but don't recurse into subdirs until needed
 *
 */

DIRLIST
dir_buildlist(LPSTR path, BOOL bOnDemand)
{
        DIRLIST dl;
        BOOL bFile;
        
        /* first check if the path is valid */
        if (dir_isvaliddir(path)) {
                bFile = FALSE;
        } else if (dir_isvalidfile(path)) {
                bFile = TRUE;
        } else {
                /* not valid */
                return(NULL);
        }


        /* alloc and init the DIRLIST head */

        dl = (DIRLIST) gmem_get(hHeap, sizeof(struct dirlist));
        memset(dl, 0, sizeof(struct dirlist));

        /* convert the pathname to an absolute path */

        _fullpath(dl->rootname, path, sizeof(dl->rootname));

        dl->bFile = bFile;
        /* make a '.' directory for the current directory -
         * all files and subdirs will be listed from here
         */
        dl->dot = (DIRECT) gmem_get(hHeap, sizeof(struct direct));
        dir_dirinit(dl->dot, dl, NULL, ".");

        /* were we given a file or a directory ? */
        if (bFile) {
                /* its a file. create a single file entry
                 * and set the state accordingly
                 */
                dl->dot->bScanned = TRUE;

                dir_addfile(dl->dot, dir_finalelem(path),
                                dir_getpathsize(path));

                return(dl);
        }

        /* scan the root directory and return. if we are asked
         * to scan the whole thing, this will cause a recursive
         * scan all the way down the tree
         */
        dir_scan(dl->dot, (!bOnDemand) );

        return(dl);
} /* dir_buildlist */

/***************************************************************************
 * Function: dir_delete
 *
 * Purpose:
 *
 * Free up the DIRLIST and all associated memory 
 */
void
dir_delete(DIRLIST dl)
{
        if (dl == NULL) {
                return;
        }
        dir_cleardirect(dl->dot);
        gmem_free(hHeap, (LPSTR) dl->dot, sizeof(struct direct));


        gmem_free(hHeap, (LPSTR) dl, sizeof(struct dirlist));
}



/***************************************************************************
 * Function: dir_isfile
 *
 * Purpose:
 *
 * Was the original build request a file or a directory ? 
 */
BOOL
dir_isfile(DIRLIST dl)
{
        if (dl == NULL) {
                return(FALSE);
        }

        return(dl->bFile);
}

/***************************************************************************
 * Function: dir_firstitem
 *
 * Purpose:
 *
 * Return the first file in the list, or NULL if no files found.
 * Returns a DIRITEM. This can be used to get filename, size and chcksum.
 * If there are no files in the root, we recurse down until we find a file.
 */
DIRITEM
dir_firstitem(DIRLIST dl)
{
        if (dl == NULL) {
                return(NULL);
        }
        /*
         * reset the state to indicate that no files have been read yet
         */
        dl->dot->pos = DL_FILES;
        dl->dot->curdir = NULL;

        /* now get the next filename */
        return(dir_findnextfile(dl, dl->dot));
} /* dir_firstitem */


/***************************************************************************
 * Function:dir_nextitem
 *
 * Purpose:
 *
 * Get the next filename after the one given.
 *
 * The List_Next function can give us the next element on the list of files.
 * If this is null, we need to go back to the DIRECT and find the
 * next list of files to traverse (in the next subdir).
 *
 * After scanning all the subdirs, return to the parent to scan further
 * dirs that are peers of this, if there are any. If we have reached the end of
 * the tree (no more dirs in dl->dot to scan), return NULL.
 *
 * Don't recurse to lower levels unless fDeep is TRUE
 */
DIRITEM
dir_nextitem(DIRLIST dl, DIRITEM cur, BOOL fDeep)
{
        DIRITEM next;

        if ((dl == NULL) || (cur == NULL)) {
                return(NULL);
        }
        if (bAbort) return NULL;  /* user requested abort */

        if ( (next = List_Next(cur)) != NULL) {
                /* there was another file on this list */
                return(next);
        }
        if (!fDeep) return NULL;

        /* get the head of the next list of filenames from the directory */
        cur->direct->pos = DL_DIRS;
        cur->direct->curdir = NULL;
        return(dir_findnextfile(dl, cur->direct));
} /* dir_nextitem */

/***************************************************************************
 * Function: dir_findnextfile
 *
 * Purpose:
 *
 * Gets the next file in the directory
 */
DIRITEM
dir_findnextfile(DIRLIST dl, DIRECT curdir)
{
        DIRITEM curfile;

        if ((dl == NULL) || (curdir == NULL)) {
                return(NULL);
        }

        /* scan the subdir if necessary */
        if (!curdir->bScanned) {
                dir_scan(curdir, FALSE);
        }

        /* have we already read the files in this directory ? */
        if (curdir->pos == DL_FILES) {
                /* no - return head of file list */
                curfile = (DIRITEM) List_First(curdir->diritems);
                if (curfile != NULL) {
                        return(curfile);
                }

                /* no more files - try the subdirs */
                curdir->pos = DL_DIRS;
        }

        /* try the next subdir on the list, if any */
        /* is this the first or the next */
        if (curdir->curdir == NULL) {
                curdir->curdir = (DIRECT) List_First(curdir->directs);
        } else {
                curdir->curdir = (DIRECT) List_Next(curdir->curdir);
        }
        /* did we find a subdir ? */
        if (curdir->curdir == NULL) {

                /* no more dirs - go back to parent if there is one */
                if (curdir->parent == NULL) {
                        /* no parent - we have exhausted the tree */
                        return(NULL);
                }

                /* reset parent state to indicate this is the current
                 * directory - so that next gets the next after this.
                 * this ensures that multiple callers of dir_nextitem()
                 * to the same tree work.
                 */
                curdir->parent->pos = DL_DIRS;
                curdir->parent->curdir = curdir;

                return(dir_findnextfile(dl, curdir->parent));
        }

        /* there is a next directory - set it to the
         * beginning and get the first file from it
         */
        curdir->curdir->pos = DL_FILES;
        curdir->curdir->curdir = NULL;
        return(dir_findnextfile(dl, curdir->curdir));

} /* dir_findnextfile */


/*-- pathnames ----
 *
 * This module supports two types of pathnames, called relative and full.
 * Relative names are relative to the root passed in the initial call
 * to dir_build*, and full names include the tree root.
 *
 * Note that this is a different distinction to relative vs absolute
 * pathnames, since the tree root may still be either relative or absolute.
 *
 * Examples:
 *
 *  - if you called dir_buildlist("c:\")
 *              getrelname gives:               ".\config.sys"
 *              getfullname gives:              "c:\config.sys"
 *
 * - if you called dir_buildlist(".\geraintd")
 *              getrelname gives:               ".\source\scandir.h"
 *              getfullname gives either
 *                      ".\geraintd\source\scandir.h"
 *                    or "c:\geraintd\source\scandir.h"
 *                   (depending on the implementation).
 *
 * To support this, we maintain the tree root name in the DIRLIST head, and
 * in each directory, the name of that directory relative to tree root.
 * Files just have the filename, so we need to prepend the directory name,
 * and (for getfullname) the tree root name as well
 *
 * We store the directory name with a trailing
 * slash to make concatenation easier
 *
 * -----
 */

/***************************************************************************
 * Function: dir_getrelname
 *
 * Purpose:
 *
 * Return the name of the current file relative to tree root
 */
LPSTR
dir_getrelname(DIRITEM cur)
{
        LPSTR name;
        int size;

        /* check this is a valid item */
        if (cur == NULL) {
                return(NULL);
        }
        /* remember to include the NULL when sizing */
        size = lstrlen(cur->direct->relname) + lstrlen(cur->name) + 1;
        name = gmem_get(hHeap, size);
        lstrcpy(name, cur->direct->relname);
        lstrcat(name, cur->name);

        return(name);
} /* dir_getrelname */

/***************************************************************************
 * Function: dir_getfullname
 *
 * Purpose:
 *
 * Return the fullname of the file (including the tree root passed in) 
 */
LPSTR
dir_getfullname(DIRITEM cur)
{
        LPSTR name;
        int size;
        LPSTR head;

        /* check this is a valid item */
        if (cur == NULL)  {
                return(NULL);
        }

        if (cur->direct->head->bFile) {
                return(cur->direct->head->rootname);
        }

        /* remember to include the NULL when sizing */
        size = lstrlen(cur->name) + 1;

        size += lstrlen(cur->direct->relname);

        /* add on root name */
        head = cur->direct->head->rootname;
        size += lstrlen(head);

        /* root names may not end in a slash. we need to
         * insert one in this case. Also, relnames always begin .\, so
         * we skip the . always, and the .\ if we don't need to
         * append a slash
         *
         */
        size--;         /* omit the '.' */
        if (*CharPrev(head, head+lstrlen(head)) == '\\') {
                size--;                         /* omit the .\ */
        }

        name = gmem_get(hHeap, size);

        lstrcpy(name, cur->direct->head->rootname);

        /* add relname and then name, omiting the .\ */

                /* skip . or .\ before relname */
                if (*CharPrev(head, head+lstrlen(head)) == '\\') {
                        lstrcat(name, &cur->direct->relname[2]);
                } else {
                        lstrcat(name, &cur->direct->relname[1]);
                }
                lstrcat(name, cur->name);
        return(name);
} /* dir_getfullname */


/***************************************************************************
 * Function: dir_getroot_list
 *
 * Purpose:
 *
 * Return the name of the tree root given a handle to the DIRLIST.
 */
LPSTR
dir_getroot_list(DIRLIST dl)
{
        if (dl == NULL) 
                return(NULL);
        return(dl->rootname);
} /* dir_getroot_list */

/***************************************************************************
 * Function: dir_getroot_item
 *
 * Purpose:
 *
 * Return the root name of this tree given a handle to a DIRITEM in the
 * list.
 */
LPSTR dir_getroot_item(DIRITEM item)
{
        if (item == NULL) 
                return(NULL);

        return(dir_getroot_list(item->direct->head));
}


/***************************************************************************
 * Function: dir_freerelname
 *
 * Purpose:
 *
 * Free up a relname that we allocated. This interface allows us
 * some flexibility in how we store relative and complete names
 *
 */
void
dir_freerelname(DIRITEM cur, LPSTR name)
{
        if((cur != NULL) && (name != NULL))
                        gmem_free(hHeap, name, lstrlen(name) +1);
} /* dir_freerelname */

/***************************************************************************
 * Function: dir_freefullname
 *
 * Purpose:
 *
 */
void
dir_freefullname(DIRITEM cur, LPSTR name)
{
        if (cur->direct->head->bFile)
                return;

        if (name != NULL) 
                gmem_free(hHeap, name, lstrlen(name) + 1);
} /* dir_freefullname            */

/***************************************************************************
 * Function: dir_freeroot_list
 *
 * Purpose:
 *
 * Free up rootname allocated by dir_getroot_list.
 * We just gave a pointer to the rootname, so do nothing.
 */
void
dir_freeroot_list(DIRLIST dl, LPSTR name)
{
        if ((dl == NULL) || (name == NULL)) {
                return;
        }
        return;
} /* dir_freeroot_list */

/***************************************************************************
 * Function: dir_freeroot_item
 *
 * Purpose:
 *
 * Free up memory alloc-ed by a call to dir_getroot_item. 
 */
void
dir_freeroot_item(DIRITEM item, LPSTR name)
{
        if ((item == NULL) || (name == NULL)) 
                return;
        dir_freeroot_list(item->direct->head, name);
}

/***************************************************************************
 * Function: dir_getopenname
 *
 * Purpose:
 *
 * Get an open-able name for the file. This will be the same as the fullname.
 */
LPSTR
dir_getopenname(DIRITEM item)
{
        LPSTR fname;

        if (item == NULL) 
                return(NULL);

        fname = dir_getfullname(item);

                return(fname);
} /* dir_getopenname */


/***************************************************************************
 * Function: dir_freeopenname
 *
 * Purpose:
 *
 * Free up memory created by a call to dir_getopenname(). This *may*
 * cause the file to be deleted if it was a temporary copy.
 */
void
dir_freeopenname(DIRITEM item, LPSTR openname)
{
        if ((item == NULL) || (openname == NULL)) 
                return;

        dir_freefullname(item, openname);
} /* dir_freeopenname */

/***************************************************************************
 * Function: dir_openfile
 *
 * Purpose:
 *
 * Return an open file handle to the file. 
 */
int
dir_openfile(DIRITEM item)
{
        LPSTR fname;
        int fh;
        OFSTRUCT os;

        fname = dir_getfullname(item);
        fh = OpenFile(fname, &os, OF_READ|OF_SHARE_DENY_NONE);
        dir_freefullname(item, fname);
        return(fh);
} /* dir_openfile */

/***************************************************************************
 * Function: dir_closefile
 *
 * Purpose:
 *
 * Close a file opened with dir_openfile.
 */
void
dir_closefile(DIRITEM item, int fh)
{
        _lclose(fh);

} /* dir_closefile */


/***************************************************************************
 * Function: dir_getfilesize
 *
 * Purpose:
 *
 * Return the file size (set during scanning) 
 */
long
dir_getfilesize(DIRITEM cur)
{
        /* check this is a valid item */
        if (cur == NULL)
                return(0);

        return(cur->size);
} /* dir_getfilesize */



/* ss_endcopy returns a number indicating the number of files copied,
   but we may have some local copies too.  We need to count these
   ourselves and add them in
*/

int nLocalCopies;        /* cleared in startcopy, ++d in copy
                                ** inspected in endcopy
                                */

/***************************************************************************
 * Function: dir_startcopy
 *
 * Purpose:
 *
 * Start a bulk copy 
 */
BOOL dir_startcopy(DIRLIST dl)
{
        nLocalCopies = 0;
        return(TRUE);

} /* dir_startcopy */
/***************************************************************************
 * Function: dir_endcopy
 *
 */
 
int dir_endcopy(DIRLIST dl)
{
        return(nLocalCopies);

} /* dir_endcopy */

/***************************************************************************
 * Function: dir_copy
 *
 * Purpose:
 *
 * Create a copy of the file, in the new root directory. Creates sub-dirs as
 * necessary. 
 *
 * Returns TRUE for success and FALSE for failure.
 */
BOOL dir_copy(DIRITEM item, LPSTR newroot)
{
        static char newpath[256];
        LPSTR relname, fullname;
        LPSTR pstart, pdest, pel;
        BOOL bOK;

        BY_HANDLE_FILE_INFORMATION bhfi;
        HANDLE hfile;

        /*
         * check that the newroot directory itself exists
         */
        if ((item == NULL) || !dir_isvaliddir(newroot)) {
                return(FALSE);
        }

        /*
         * name of file relative to the tree root
         */
        relname = dir_getrelname(item);

        /*
         * build the new pathname by concatenating the new root and
         * the old relative name. add one path element at a time and
         * ensure that the directory exists, creating it if necessary.
         */
        lstrcpy(newpath, newroot);

        /* add separating slash if not already there */
        if (*CharPrev(newpath, newpath+lstrlen(newpath)) != '\\') {
                lstrcat(newpath, "\\");
        }

        pstart = relname;
        while ( (pel = strchr(pstart, '\\')) != NULL) {

                /* found another element ending in slash. incr past the \\ */
                pel++;

                /*
                 * ignore .
                 */
                if (strncmp(pstart, ".\\", 2) != 0) {

                        pdest = &newpath[lstrlen(newpath)];
                        strncpy(pdest, pstart, pel - pstart);
                        pdest[pel - pstart] = '\0';

                        /* create subdir if necessary */
                        if (!dir_isvaliddir(newpath)) {
                                if (_mkdir(newpath) != 0) {
                                        return(FALSE);
                                }
                        }
                }

                pstart = pel;
        }

        /*
         * there are no more slashes, so pstart points at the final
         * element
         */
        lstrcat(newpath, pstart);

        fullname = dir_getfullname(item);

                bOK = CopyFile(fullname, newpath, FALSE);

                /* having copied the file, now copy the times, attributes */
                hfile = CreateFile(fullname, GENERIC_READ, 0, NULL,
                                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
                bhfi.dwFileAttributes = GetFileAttributes(fullname);
                GetFileTime(hfile, &bhfi.ftCreationTime,
                                &bhfi.ftLastAccessTime, &bhfi.ftLastWriteTime);
                CloseHandle(hfile);

                hfile = CreateFile(newpath, GENERIC_WRITE, 0, NULL,
                                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
                SetFileTime(hfile, &bhfi.ftCreationTime,
                                   &bhfi.ftLastAccessTime,
                                   &bhfi.ftLastWriteTime);
                CloseHandle(hfile);
                SetFileAttributes(newpath, bhfi.dwFileAttributes);


                if (bOK) ++nLocalCopies;

        dir_freerelname(item, relname);
        dir_freefullname(item, fullname);

        return(bOK);
} /* dir_copy */


/***************************************************************************
 * Function: dir_dirinit
 *
 * Purpose:
 *
 * Fill out a new DIRECT for a subdirectory (pre-allocated).
 * Init files and dirs lists to empty (List_Create). Set the relname
 * of the directory by pre-pending the parent relname if there
 * is a parent, and appending a trailing slash (if there isn't one).
 */
void
dir_dirinit(DIRECT dir, DIRLIST head, DIRECT parent, LPSTR name)
{
        int size;

        dir->head = head;
        dir->parent = parent;

        /* add on one for the null and one for the trailing slash */
        size = lstrlen(name) + 2;
        if (parent != NULL) {
                size += lstrlen(parent->relname);
        }

        /* build the relname from the parent and the current name
         * with a terminating slash
         */
        dir->relname = gmem_get(hHeap, size);
        if (parent != NULL) {
                lstrcpy(dir->relname, parent->relname);
        } else{
                dir->relname[0] = '\0';
        }

        lstrcat(dir->relname, name);

        if (*CharPrev(dir->relname,
                        dir->relname+lstrlen(dir->relname)) != '\\') {
                lstrcat(dir->relname, "\\");
        }

        /* force name to lowercase */
        AnsiLowerBuff(dir->relname, lstrlen(dir->relname));

        dir->diritems = List_Create();
        dir->directs = List_Create();
        dir->bScanned = FALSE;
        dir->pos = DL_FILES;

} /* dir_dirinit */


/***************************************************************************
 * Function: dir_fileinit
 *
 * Purpose:
 *
 * Initialise the contents of an (allocated) DIRITEM struct. 
 */
void
dir_fileinit(DIRITEM pfile, DIRECT dir, LPSTR path, long size)
{

        pfile->name = gmem_get(hHeap, lstrlen(path) + 1);
        lstrcpy(pfile->name, path);

        /* force name to lower case */
        AnsiLowerBuff(pfile->name, lstrlen(path));

        pfile->direct = dir;
        pfile->size = size;

        pfile->localname = NULL;

} /* dir_fileinit */

/***************************************************************************
 * Function: dir_isfilevalid
 *
 * Purpose:
 *
 * Is this a valid file or not 
 */
BOOL
dir_isvalidfile(LPSTR path)
{
        DWORD dwAttrib;

        dwAttrib = GetFileAttributes(path);
        if (dwAttrib == -1) {
                return(FALSE);
        }
        if (dwAttrib & FILE_ATTRIBUTE_DIRECTORY) {
                return(FALSE);
        }
        return(TRUE);
} /* dir_isvalidfile */

/***************************************************************************
 * Function: dir_isvaliddir
 *
 * Purpose:
 *
 * Is this a valid directory ? 
 */
BOOL
dir_isvaliddir(LPSTR path)
{
        DWORD dwAttrib;

        dwAttrib = GetFileAttributes(path);
        if (dwAttrib == -1) {
                return(FALSE);
        }
        if (dwAttrib & FILE_ATTRIBUTE_DIRECTORY) {
                return(TRUE);
        }
        return(FALSE);
} /* dir_isvaliddir */

/***************************************************************************
 * Function: dir_scan
 *
 * Purpose:
 *
 * Scan the directory given. Add all files to the list
 * in alphabetic order, and add all directories in alphabetic
 * order to the list of child DIRITEMs. If bRecurse is true, go on to
 * recursive call dir_scan for each of the child DIRITEMs
 */
void
dir_scan(DIRECT dir, BOOL bRecurse)
{
        PSTR path;
        int size;
        DIRECT child;
        BOOL bMore;
        long filesize;
        BOOL bIsDir;
        LPSTR name;

        HANDLE hFind;
        WIN32_FIND_DATA finddata;

        char debugmsg[200];
        wsprintf(debugmsg, "scandir: %s %s\n",
                 dir->relname, bRecurse?"recursive":"non-recursive"
                );

        /* make the complete search string including *.* */
        size = lstrlen(dir->head->rootname);
        size += lstrlen(dir->relname);

        /* add on one null and *.* */
        size += 4;

        path = LocalLock(LocalAlloc(LHND, size));

        lstrcpy(path, dir->head->rootname);

        /* omit the . at the beginning of the relname, and the
         * .\ if there is a trailing \ on the rootname
         */
        if (*CharPrev(path, path+lstrlen(path)) == '\\') {
                lstrcat(path, &dir->relname[2]);
        } else {
                lstrcat(path, &dir->relname[1]);
        }
        lstrcat(path, "*.*");

        /* read all entries in the directory */
        hFind = FindFirstFile(path, &finddata);
        bMore = (hFind != (HANDLE) -1);
        LocalUnlock(LocalHandle ( (PSTR) path));
        LocalFree(LocalHandle ( (PSTR) path));

        while (bMore) {

                bIsDir = (finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
                name = (LPSTR) &finddata.cFileName;
                filesize = finddata.nFileSizeLow;
                if (!bIsDir) {

                        dir_addfile(dir, name, filesize);

                } else if ( (lstrcmp(name, ".") != 0) &&
                           ( lstrcmp(name, "..") != 0) ) {

                        dir_adddirect(dir, name);
                }

                bMore = FindNextFile(hFind, &finddata);
        }

        FindClose(hFind);

        dir->bScanned = TRUE;
        dir->pos = DL_FILES;

        if (bRecurse) {
                List_TRAVERSE(dir->directs, child) {
                        dir_scan(child, TRUE);
                }
        }

} /* dir_scan */


/***************************************************************************
 * Function: dir_addfile
 *
 * Purpose:
 *
 * Add the file 'path' to the list of files in dir, in order.
 */
void
dir_addfile(DIRECT dir, LPSTR path, DWORD size)
{
        DIRITEM pfile;

        AnsiLowerBuff(path, lstrlen(path));  // needless?

        List_TRAVERSE(dir->diritems, pfile) {
                /////if (lstrcmpi(pfile->name, path) > 0) {
                if (utils_CompPath(pfile->name, path) > 0) {

                        /* goes before this one */
                        pfile = List_NewBefore(dir->diritems, pfile, sizeof(struct diritem));
                        dir_fileinit(pfile, dir, path, size);
                        return;
                }
        }
        /* goes at end */
        pfile = List_NewLast(dir->diritems, sizeof(struct diritem));
        dir_fileinit(pfile, dir, path, size);
} /* dir_addfile */


/***************************************************************************
 * Function: dir_addirect
 *
 * Purpose:
 *
 * Add a new directory in alphabetic order on
 * the list dir->directs
 *
 */
void
dir_adddirect(DIRECT dir, LPSTR path)
{
        DIRECT child;
        LPSTR finalel;
        char achTempName[256];

        AnsiLowerBuff(path, lstrlen(path));
        List_TRAVERSE(dir->directs, child) {

                int cmpval;

                /* we need to compare the child name with the new name.
                 * the child name is a relname with a trailing
                 * slash - so compare only the name up to but
                 * not including the final slash.
                 */
                finalel = dir_finalelem(child->relname);

                /*
                 * we cannot use strnicmp since this uses a different
                 * collating sequence to lstrcmpi. So copy the portion
                 * we are interested in to a null-term. buffer.
                 */
                strncpy(achTempName, finalel, lstrlen(finalel)-1);
                achTempName[lstrlen(finalel)-1] = '\0';

                cmpval = utils_CompPath(achTempName, path);

                if (cmpval > 0) {

                        /* goes before this one */
                        child = List_NewBefore(dir->directs, child, sizeof(struct direct));
                        dir_dirinit(child, dir->head, dir, path);
                        return;
                }
        }
        /* goes at end */
        child = List_NewLast(dir->directs, sizeof(struct direct));
        dir_dirinit(child, dir->head, dir, path);
} /* dir_adddirect */


/***************************************************************************
 * Function: dir_cleardirect
 *
 * Purpose:
 *
 * Free all memory associated with a DIRECT (including freeing
 * child lists). Don't de-alloc the direct itself (allocated on a list)
 */
void
dir_cleardirect(DIRECT dir)
{
        DIRITEM pfile;
        DIRECT child;

        /* clear contents of files list */
        List_TRAVERSE(dir->diritems, pfile) {
                gmem_free(hHeap, pfile->name, lstrlen(pfile->name));
                if ((pfile->localname) && (pfile->bLocalIsTemp)) {

                        /*
                         * the copy will have copied the attributes,
                         * including read-only. We should unset this bit
                         * so we can delete the temp file.
                         */
                        SetFileAttributes(pfile->localname,
                                GetFileAttributes(pfile->localname)
                                        & ~FILE_ATTRIBUTE_READONLY);
                        DeleteFile(pfile->localname);
                        gmem_free(hHeap, pfile->localname, 256);
                        pfile->localname = NULL;
                }

        }
        List_Destroy(&dir->diritems);

        /* clear contents of dirs list (recursively) */
        List_TRAVERSE(dir->directs, child) {
                dir_cleardirect(child);
        }
        List_Destroy(&dir->directs);

        gmem_free(hHeap, dir->relname, lstrlen(dir->relname) + 1);

} /* dir_cleardirect */

/***************************************************************************
 * Function: dir_finalelem
 *
 * Purpose:
 *
 * Return a pointer to the final element in a path. Note that
 * we may be passed relnames with a trailing final slash - ignore this
 * and return the element before that final slash.
 */
LPSTR
dir_finalelem(LPSTR path)
{
        LPSTR chp;
        int size;

        /* is the final character a slash ? */
        size = lstrlen(path) - 1;
        if (*(chp = CharPrev(path, path+lstrlen(path))) == '\\') {
                /* find the slash before this */
                while (chp > path) {
                        if (*(chp = CharPrev(path, chp)) == '\\') {
                                /* skip the slash itself */
                                chp++;
                                break;
                        }
                }
                return(chp);
        }
        /* look for final slash */
        chp = strrchr(path, '\\');
        if (chp != NULL) {
                return(chp+1);
        }

        /* no slash - is there a drive letter ? */
        chp = strrchr(path, ':');
        if (chp != NULL) {
                return(chp+1);
        }

        /* this is a final-element anyway */
        return(path);

} /* dir_finalelem */

/***************************************************************************
 * Function: dir_getpathsize
 *
 * Purpose:
 *
 * Find the size of a file given a pathname to it 
 */
long
dir_getpathsize(LPSTR path)
{
        int fh;
        OFSTRUCT os;
        long size;
        fh = OpenFile(path, &os, OF_READ|OF_SHARE_DENY_NONE);
        if (fh == -1) {
                return(0);
        }

        size = GetFileSize( (HANDLE) fh, NULL);
        _lclose(fh);
        return(size);
} /* dir_getpathsize */


