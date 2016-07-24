

/*
 * COMPLIST.H
 */

/* The file view.h includes the term COMPLIST: we need to use the term VIEW.
 * Due to MIPS limitations,the module which declares it first does
 * the real declares and the second one gets no-ops.
 */
#ifndef INC_VIEW_COMPLIST
#define INC_VIEW_COMPLIST
typedef struct view FAR * VIEW;                 /* handle to a VIEW     */
typedef struct complist FAR * COMPLIST;         /* handle to a complist */
#endif // INC_VIEW_COMPLIST


COMPLIST complist_filedialog(VIEW view);
COMPLIST complist_dirdialog(VIEW view);
COMPLIST complist_args(LPSTR path1, LPSTR path2, VIEW view, BOOL fDeep);
void complist_delete(COMPLIST cl);
LIST complist_getitems(COMPLIST cl);
void complist_savelist(COMPLIST cl, LPSTR savename, UINT saveopts);
void complist_copyfiles(COMPLIST cl, LPSTR newroot, UINT options);
DWORD complist_querytime(void);
BOOL APIENTRY complist_open(LPSTR prompt, LPSTR ext, LPSTR spec,
        OFSTRUCT FAR *osp, LPSTR fn);
LPSTR complist_getroot_left(COMPLIST cl);
LPSTR complist_getroot_right(COMPLIST cl);
void complist_freeroot_left(COMPLIST cl, LPSTR path);
void complist_freeroot_right(COMPLIST cl, LPSTR path);

/*
 * Copy file options are either COPY_FROMLEFT or COPY_FROMRIGHT 
 * (indicating which
 * tree is to be the source of the files, plus any or all of
 * INCLUDE_SAME, INCLUDE_DIFFER and INCLUDE_LEFT (INCLUDE_LEFT
 * and INCLUDE_RIGHT are treated the same here since the COPY_FROM* option
 * indicates which side to copy from).
 */

#define COPY_FROMLEFT   0x100           /* copy files from left tree */
#define COPY_FROMRIGHT  0x200           /* copy files from right tree */


