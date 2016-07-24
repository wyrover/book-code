

/*
 * SCANDIR.H
 *
 */

/* Handle to the list of files scanned */
typedef struct dirlist FAR * DIRLIST;

/* Handle to one item within the list of files */
typedef struct diritem FAR * DIRITEM;


DIRLIST dir_buildlist(LPSTR pathname, BOOL bOnDemand);
void dir_delete(DIRLIST list);
BOOL dir_isfile(DIRLIST list);
DIRITEM dir_firstitem(DIRLIST list);
DIRITEM dir_nextitem(DIRLIST list, DIRITEM previtem, BOOL fDeep);

/* Filenames
 *
 * From a DIRITEM, you can query either the relative or the full name.
 *
 * The relative name does not include the tree root that was originally
 * passed to dir_buildlist. The full name does include this. Note however
 * that if you passed a relative name to dir_buildlist, the full
 * name you get back will not be an *absolute* pathname.
 *
 * Thus, if you call dir_buildlist with "c:\",
 * we will return:
 *      relative name:  ".\config.sys"
 *      full name:      "c:\config.sys"
 *
 * If you call dir_buildlist with ".\geraintd",
 * we will return:
 *      relative name:  ".\source\scandir.h"
 *      full name:      ".\geraintd\source\scandir.h"
 *
 * In both cases, we return a pointer to a filename string: you must
 * call dir_freefullname or dir_freerelname to free this memory when you
 * have finished with it. Depending on the implementation, one or other
 * (or possibly both) of these names will have been built specially
 * when you called the query function.
 *
 * You can also return a pointer to the tree root name. (In the above
 * examples this would be c:\ and .\geraintd). Depending on the implementation,
 * this may have been forced to an absolute path.
 *
 */

LPSTR dir_getfullname(DIRITEM item);
LPSTR dir_getrelname(DIRITEM item);
LPSTR dir_getroot_item(DIRITEM item);
LPSTR dir_getroot_list(DIRLIST dl);
void dir_freefullname(DIRITEM item, LPSTR fullname);
void dir_freerelname(DIRITEM item, LPSTR relname);
void dir_freeroot_item(DIRITEM item, LPSTR rootname);
void dir_freeroot_list(DIRLIST dl, LPSTR rootname);
LPSTR dir_getopenname(DIRITEM item);
void dir_freeopenname(DIRITEM item, LPSTR openname);
int dir_openfile(DIRITEM item);
void dir_closefile(DIRITEM item, int fh);
long dir_getfilesize(DIRITEM item);
BOOL dir_copy(DIRITEM item, LPSTR newroot);
BOOL dir_startcopy(DIRLIST dl);
int dir_endcopy(DIRLIST dl);
