

/****************************** Module Header *******************************
* Module Name: COMPLIST.C
*
* Supports a list of compitems, where each compitem represents
* a pair of matching files, or an unmatched file.
*
* Functions:
*
* complist_filedialog()
* complist_dirdialog()
* complist_args()
* complist_getitems()
* complist_delete()
* complist_savelist()
* complist_copyfiles()
* complist_dodlg_savelist()
* complist_dodlg_copyfiles()
* complist_match()
* complist_new()
* complist_dodlg_dir()
*
* Comments:
*
* We build lists of filenames from two pathnames (using the
* scandir module) and then traverse the two lists comparing names.
* Where the names match, we create a CompItem from the matching
* names. Where there is an unmatched name, we create a compitem for it.
*
* We may also be asked to create a complist for two individual files:
* here we create a single compitem for them as a matched pair even if
* the names don't match.
*
****************************************************************************/

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <direct.h>

#include "gutils.h"
#include "state.h"
#include "windiff.h"
#include "wdiffrc.h"
#include "list.h"
#include "line.h"
#include "scandir.h"
#include "file.h"
#include "section.h"
#include "compitem.h"
#include "complist.h"
#include "view.h"


extern BOOL bAbort;             /* defined in windiff.c  Read only here */

/*
 * The COMPLIST handle is typedef-ed to be a pointer to one
 * of these struct complist
 */
struct complist {
        DIRLIST left;           /* left list of files */
        DIRLIST right;          /* right list of files */
        LIST items;             /* list of COMPITEMs */
};

/* ---- module-wide data -------------------------------------*/

/* data for communicating between the SaveList dlg and complist_savelist() */

char dlg_file[256];                /* filename to save to */

/* checkbox options */
BOOL dlg_identical, dlg_differ, dlg_left, dlg_right;
BOOL dlg_recursive = FALSE;

/* data for Directory and SaveList */
char dialog_leftname[256];
char dialog_rightname[256];

/*
 * data used by dodlg_copyfiles
 */
UINT dlg_options;
char dlg_root[256];

/*------------------------timing for performance measurements-----------------*/

static DWORD TickCount;         /* time operation started, then time taken*/


int FAR PASCAL complist_dodlg_savelist(HWND hDlg, UINT message,
        UINT wParam, long lParam);
int FAR PASCAL complist_dodlg_copyfiles(HWND hDlg, UINT message,
        UINT wParam, long lParam);
BOOL complist_match(COMPLIST cl, VIEW view, BOOL fDeep, BOOL fExact);
COMPLIST complist_new(void);
int FAR PASCAL complist_dodlg_dir(HWND hDlg, unsigned message,
        WORD wParam, LONG lParam);



/***************************************************************************
 * Function: complist_filedialog
 *
 * Purpose:
 *
 * Builds a complist by putting up two dialogs to allow the user to
 * select two files. This will build a Complist with one CompItem (even
 * if the names don't match).
 *
 ***************************************************************************/
COMPLIST
complist_filedialog(VIEW view)
{
        COMPLIST cl;
        OFSTRUCT os1, os2;
        char fname[256], FileExt[256], FileOpenSpec[256];

        /* ask for the filenames */
        lstrcpy(FileExt, ".c");
        lstrcpy(FileOpenSpec, "*.*");
        lstrcpy(fname,"");

        if (!complist_open(LoadRcString(IDS_SELECT_FIRST_FILE), FileExt, FileOpenSpec,
                        &os1, fname) )
                return(NULL);

        lstrcpy(FileExt, ".c");
        lstrcpy(FileOpenSpec, "*.*");
        lstrcpy(fname,"");

        if (!complist_open(LoadRcString(IDS_SELECT_SECOND_FILE), FileExt, FileOpenSpec,
                        &os2, fname) )
                return(NULL);

        /* alloc a new structure */
        cl = complist_new();

        cl->left = dir_buildlist(os1.szPathName, TRUE);
        cl->right = dir_buildlist(os2.szPathName, TRUE);


        /* register with the view (must be done after the list is non-null) */
        view_setcomplist(view, cl);

        complist_match(cl, view, FALSE, TRUE);

        return(cl);
}/* complist_filedialog */

/***************************************************************************
 * Function: complist_dirdialog
 *
 * Purpose:
 *
 * Builds a new complist by querying the user for two directory
 * names and scanning those in parallel.
 *
 * Names that match in the same directory will be paired - unmatched
 * names will go in a compitem on their own.
 *
 ***************************************************************************/
COMPLIST
complist_dirdialog(VIEW view)
{
        DLGPROC lpProc;
        BOOL fOK;

        /* put up a dialog for the two pathnames */
        lpProc = (DLGPROC)MakeProcInstance((WNDPROC)complist_dodlg_dir, hInst);
        windiff_UI(TRUE);
        fOK = DialogBox(hInst, "Directory", hwndClient, lpProc);
        windiff_UI(FALSE);
        FreeProcInstance(lpProc);

        if (!fOK) {
                return(NULL);
        }

        return complist_args( dialog_leftname, dialog_rightname
                            , view, dlg_recursive);
} /* complist_dirdialog */


/***************************************************************************
 * Function: complist_args
 *
 * Purpose:
 *
 * Given two pathname strings, scan the directories and traverse them
 * in parallel comparing matching names.
 *
 ***************************************************************************/
COMPLIST
complist_args(LPSTR p1, LPSTR p2, VIEW view, BOOL fDeep)
{
        COMPLIST cl;
        char msg[256];


        /* alloc a new complist */
        cl = complist_new();

        cl->left = dir_buildlist(p1, TRUE);
        /* check that we could find the paths, and report if not */
        if (cl->left == NULL) {
                wsprintf((LPTSTR)msg, LoadRcString(IDS_COULDNT_FIND), p1);
                MessageBox(NULL, msg, NULL, MB_OK | MB_ICONSTOP);
                return(NULL);
        }

        cl->right = dir_buildlist(p2, TRUE);
        if (cl->right == NULL) {
                wsprintf((LPTSTR)msg, LoadRcString(IDS_COULDNT_FIND), p2);
                MessageBox(NULL, msg, NULL, MB_OK | MB_ICONSTOP);
                return(NULL);
        }

        /* register with the view (must be done after building lists) */
        view_setcomplist(view, cl);

        complist_match(cl, view, fDeep, TRUE);

        return(cl);
} /* complist_args */

/***************************************************************************
 * Function: complist_getitems
 *
 * Purpose:
 *
 * Gets the handle to the list of COMPITEMs. The list continues to be
 * owned by the COMPLIST, so don't delete except by calling complist_delete.
 *
 ***************************************************************************/
LIST
complist_getitems(COMPLIST cl)
{
        if (cl == NULL) {
                return(NULL);
        }

        return(cl->items);
}

/***************************************************************************
 * Function: complist_delete
 *
 * Purpose:
 *
 * Deletes a complist and all associated CompItems and DIRLISTs. Note this
 * does not delete any VIEW - the VIEW owns the COMPLIST and not the other
 * way around.
 *
 **************************************************************************/
void
complist_delete(COMPLIST cl)
{
        COMPITEM item;

        if (cl == NULL) {
                return;
        }

        /* delete the two directory scan lists */
        dir_delete(cl->left);
        dir_delete(cl->right);

        /* delete the compitems in the list */
        List_TRAVERSE(cl->items, item) {
                        compitem_delete(item);
        }

        /* delete the list itself */
        List_Destroy(&cl->items);

        gmem_free(hHeap, (LPSTR) cl, sizeof(struct complist));

}

/***************************************************************************
 * Function: complist_savelist
 *
 * Purpose:
 *
 * Writes out to a text file the list of compitems as relative filenames
 * one per line.
 *
 * If savename is non-null, use this as the filename for output; otherwise,
 * query the user via a dialog for the filename and include options.
 *
 **************************************************************************/
void
complist_savelist(COMPLIST cl, LPSTR savename, UINT options)
{
        DLGPROC lpProc;
        static BOOL done_init = FALSE;
        BOOL bOK;
        int fh, state;
        OFSTRUCT os;
        char msg[256];
        HCURSOR hcurs;
        COMPITEM ci;
        LPSTR pstr, lhead, rhead;
        int nFiles = 0;

        if (!done_init) {
                /* init the options once round - but keep the same options
                 * for the rest of the session.
                 */

                /* first init default options */
                dlg_identical = FALSE;
                dlg_differ = TRUE;
                dlg_left = TRUE;
                dlg_right = FALSE;

                dlg_file[0] = '\0';

                done_init = TRUE;
        }

        if (cl == NULL) {
                return;
        }

        if (savename == NULL) {

                /* store the left and right rootnames so that dodlg_savelist
                 * can display them in the dialog.
                 */
                pstr = dir_getroot_list(cl->left);
                lstrcpy(dialog_leftname, pstr);
                dir_freeroot_list(cl->left, pstr);

                pstr = dir_getroot_list(cl->right);
                lstrcpy(dialog_rightname, pstr);
                dir_freeroot_list(cl->right, pstr);

                lpProc = (DLGPROC)MakeProcInstance((WNDPROC)complist_dodlg_savelist, hInst);
                windiff_UI(TRUE);
                bOK = DialogBox(hInst, "SaveList", hwndClient, lpProc);
                windiff_UI(FALSE);
                FreeProcInstance(lpProc);

                if (!bOK) {
                        /* user cancelled from dialog box */
                        return;
                }
                savename = dlg_file;

        } else {
                dlg_identical = (options & INCLUDE_SAME);
                dlg_differ = (options & INCLUDE_DIFFER);
                dlg_left = (options & INCLUDE_LEFTONLY);
                dlg_right = (options & INCLUDE_RIGHTONLY);
        }


        /* try to open the file */
        fh = OpenFile(savename, &os, OF_CREATE|OF_READWRITE|OF_SHARE_DENY_WRITE);
        if (fh < 0) {
                wsprintf((LPTSTR)msg, LoadRcString(IDS_CANT_OPEN), savename);
                windiff_UI(TRUE);
                MessageBox(NULL, msg, "Windiff", MB_ICONSTOP|MB_OK);
                windiff_UI(FALSE);
                return;
        }

        hcurs = SetCursor(LoadCursor(NULL, IDC_WAIT));

        /* write out the header line */
        lhead = dir_getroot_list(cl->left);
        rhead = dir_getroot_list(cl->right);
{
        TCHAR szBuf1[20],szBuf2[20],szBuf3[20],szBuf4[20];
        lstrcpy(szBuf1,(LPSTR)(dlg_identical ? LoadRcString(IDS_IDENTICAL_COMMA) : ""));
        lstrcpy(szBuf2,(LPSTR)(dlg_left ? LoadRcString(IDS_LEFT_ONLY_COMMA) : ""));
        lstrcpy(szBuf3,(LPSTR)(dlg_right ? LoadRcString(IDS_RIGHT_ONLY_COMMA) : ""));
        lstrcpy(szBuf4,(LPSTR)(dlg_differ ? LoadRcString(IDS_DIFFERING) : ""));
        wsprintf(msg, LoadRcString(IDS_HEADER_LINE_STR),
                lhead, rhead, szBuf1, szBuf2, szBuf3, szBuf4);
}
        _lwrite(fh, msg, lstrlen(msg));
        dir_freeroot_list(cl->left, lhead);
        dir_freeroot_list(cl->right, rhead);


        /* traverse the list of compitems looking for the
         * ones we are supposed to include
         */
        List_TRAVERSE(cl->items, ci) {

                /* check if files of this type are to be listed */
                state = compitem_getstate(ci);

                if ((state == STATE_SAME) && (!dlg_identical)) {
                        continue;
                } else if ((state == STATE_DIFFER) && (!dlg_differ)) {
                        continue;
                } else if ((state == STATE_FILELEFTONLY) && (!dlg_left)) {
                        continue;
                } else if ((state == STATE_FILERIGHTONLY) && (!dlg_right)) {
                        continue;
                }

                nFiles++;

                /* output the list line */
                wsprintf((LPTSTR)msg, "%s\r\n", compitem_gettext_tag(ci));
                _lwrite(fh, msg, lstrlen(msg));
        }

        /* write tail line */
        wsprintf((LPTSTR)msg, LoadRcString(IDS_FILES_LISTED), nFiles);
        _lwrite(fh, msg, lstrlen(msg));

        /* - close file and we are finished */
        _lclose(fh);

        SetCursor(hcurs);
} /* complist_savelist */

/***************************************************************************
 * Function: complist_copyfiles
 *
 * Purpose:
 *
 * To copy files to a new directory newroot. if newroot is NULL, query the user
 * via a dialog to get the new dir name and options.
 *
 * Options are either COPY_FROMLEFT or COPY_FROMRIGHT (indicating which
 * tree is to be the source of the files, plus any or all of
 * INCLUDE_SAME, INCLUDE_DIFFER and INCLUDE_LEFT (INCLUDE_LEFT
 * and INCLUDE_RIGHT are treated the same here since the COPY_FROM* option
 * indicates which side to copy from).
 *
 ***************************************************************************/
void
complist_copyfiles(COMPLIST cl, LPSTR newroot, UINT options)
{
        int nFiles = 0;
        int nFails = 0;
        static BOOL done_init = FALSE;
        LPSTR pstr;
        char buffer[64];
        DIRITEM diritem;
        DLGPROC lpProc;
        BOOL bOK;
        COMPITEM ci;
        int state;

        if (!done_init) {
                /*
                 * one-time initialisation of dialog defaults
                 */
                dlg_options = COPY_FROMLEFT|INCLUDE_LEFTONLY|INCLUDE_DIFFER;
                dlg_root[0] = '\0';
                done_init = TRUE;
        }

        if (cl == NULL) {
                return;
        }


        if (newroot == NULL) {
                /*
                 * put up dialog to query rootname and options
                 */

                /* store the left and right rootnames so that the dlg proc
                 * can display them in the dialog.
                 */
                pstr = dir_getroot_list(cl->left);
                lstrcpy(dialog_leftname, pstr);
                dir_freeroot_list(cl->left, pstr);

                pstr = dir_getroot_list(cl->right);
                lstrcpy(dialog_rightname, pstr);
                dir_freeroot_list(cl->right, pstr);

                do {
                        lpProc = (DLGPROC)MakeProcInstance((WNDPROC)complist_dodlg_copyfiles, hInst);
                        windiff_UI(TRUE);
                        bOK = DialogBox(hInst, "CopyFiles", hwndClient, lpProc);
                        windiff_UI(FALSE);
                        FreeProcInstance(lpProc);

                        if (!bOK) {
                                /* user cancelled from dialog box */
                                return;
                        }
                        if (lstrlen(dlg_root) == 0) {
                                windiff_UI(TRUE);
                                MessageBox(NULL, LoadRcString(IDS_ENTER_DIR_NAME),
                                                "Windiff", MB_ICONSTOP|MB_OK);
                                windiff_UI(FALSE);
                        }
                } while (lstrlen(dlg_root) == 0);

        } else {
                dlg_options = options;
                lstrcpy(dlg_root, newroot);
        }

        TickCount = GetTickCount();

        if (dlg_options & COPY_FROMLEFT) {
                if (!dir_startcopy(cl->left))
                        return;
        } else {
                if (!dir_startcopy(cl->right))
                        return;
        }

        /*
         * traverse the list of compitems copying files as necessary
         */
        List_TRAVERSE(cl->items, ci) {

                if (bAbort){
                        break;  /* fall into end_copy processing */
                }
                /* check if files of this type are to be copied */
                state = compitem_getstate(ci);

                if ((state == STATE_SAME) && !(dlg_options & INCLUDE_SAME)) {
                        continue;
                } else if ((state == STATE_DIFFER) && !(dlg_options & INCLUDE_DIFFER)) {
                        continue;
                } else if (state == STATE_FILELEFTONLY) {
                        if (dlg_options & COPY_FROMRIGHT) {
                                continue;
                        }
                        if ((dlg_options & (INCLUDE_LEFTONLY | INCLUDE_RIGHTONLY)) == 0) {
                                continue;
                        }
                } else if (state == STATE_FILERIGHTONLY) {
                        if (dlg_options & COPY_FROMLEFT) {
                                continue;
                        }
                        if ((dlg_options & (INCLUDE_LEFTONLY | INCLUDE_RIGHTONLY)) == 0) {
                                continue;
                        }
                }

                if (dlg_options & COPY_FROMLEFT) {
                        diritem = file_getdiritem(compitem_getleftfile(ci));
                } else {
                        diritem = file_getdiritem(compitem_getrightfile(ci));
                }

                /*
                 * copy the file to the new root directory
                 */
                if (dir_copy(diritem, dlg_root) == FALSE) {
                        nFails++;
                        pstr = dir_getrelname(diritem);
                        wsprintf((LPTSTR)buffer, LoadRcString(IDS_FAILED_TO_COPY), pstr);
                        dir_freerelname(diritem, pstr);

                        if (MessageBox(NULL, buffer, NULL, MB_OKCANCEL | MB_ICONSTOP) == IDCANCEL)
                            /* user pressed cancel - abort current operation*/
                            /* fall through to end-copy processing */
                            break;

                } else {
                        nFiles++;
                }

                wsprintf((LPTSTR)buffer, LoadRcString(IDS_COPYING), nFiles);
                SetStatus(buffer);


                /*
                 * allow user interface to continue
                 */
                if (Poll()) {
                        /* abort requested */
                        TickCount = GetTickCount()-TickCount;
                        windiff_UI(TRUE);
                        MessageBox(hwndClient, LoadRcString(IDS_COPY_ABORTED),
                                "WinDiff", MB_OK|MB_ICONINFORMATION);
                        windiff_UI(FALSE);
                        break;
                }

        } /* traverse */
        if (dlg_options & COPY_FROMLEFT) {
                nFails = dir_endcopy(cl->left);
        } else {
                nFails = dir_endcopy(cl->right);
        }
        TickCount = GetTickCount()-TickCount;

        if (nFails<0) {
                wsprintf((LPTSTR)buffer, LoadRcString(IDS_COPY_FAILED), -nFails);
        } else {
                wsprintf((LPTSTR)buffer, LoadRcString(IDS_COPY_COMPLETE), nFails);
        }
        windiff_UI(TRUE);
        MessageBox(hwndClient, buffer, "WinDiff", MB_OK|MB_ICONINFORMATION);
        windiff_UI(FALSE);

        buffer[0] = '\0';
        SetStatus(buffer);
} /* complist_copyfiles */


/***************************************************************************
 * Function: complist_match
 *
 * Purpose:
 *
 * Matches up two lists of filenames
 *
 * Commentsz:
 *
 * We can find out from the DIRLIST handle whether the original list
 * was a file or a directory name.
 * If the user typed:
 *      two file names  - match these two item even if the names differ
 *
 *      two dirs        - match only those items whose names match
 *
 *      one file and one dir
 *                      - try to find a file of that name in the dir.
 *
 * This function returns TRUE if the complist_match was ok, or FALSE if it was
 * aborted in some way.
 *
 ***************************************************************************/
BOOL
complist_match(COMPLIST cl, VIEW view, BOOL fDeep, BOOL fExact)
{
        LPSTR lname;
        LPSTR rname;
        DIRITEM leftitem, rightitem;
        int cmpvalue;

        TickCount = GetTickCount();

        if (dir_isfile(cl->left) ) {

                if (dir_isfile(cl->right)) {
                        /* two files */

                        /* there should be one item in each list - make
                         * a compitem by matching these two and append it to the
                         * list
                         */
                        compitem_new(dir_firstitem(cl->left),
                                       dir_firstitem(cl->right), cl->items, fExact);

                        view_newitem(view);

                        TickCount = GetTickCount() - TickCount;
                        return TRUE;
                }
                /* left is file, right is dir */
                leftitem = dir_firstitem(cl->left);
                rightitem = dir_firstitem(cl->right);
                lname = dir_getrelname(leftitem);
                while (rightitem != NULL) {
                        rname = dir_getrelname(rightitem);
                        cmpvalue = lstrcmpi(lname, rname);
                        dir_freerelname(rightitem, rname);

                        if (cmpvalue == 0) {
                                /* this is the match */
                                compitem_new(leftitem, rightitem, cl->items, fExact);
                                view_newitem(view);

                                dir_freerelname(leftitem, lname);

                                TickCount = GetTickCount() - TickCount;
                                return(TRUE);
                        }

                        rightitem = dir_nextitem(cl->right, rightitem, fDeep);
                }
                /* not found */
                dir_freerelname(leftitem, lname);
                compitem_new(leftitem, NULL, cl->items, fExact);
                view_newitem(view);
                TickCount = GetTickCount() - TickCount;
                return(TRUE);

        } else if (dir_isfile(cl->right)) {

                /* left is dir, right is file */

                /* loop through the left dir, looking for
                 * a file that has the same name as rightitem
                 */

                leftitem = dir_firstitem(cl->left);
                rightitem = dir_firstitem(cl->right);
                rname = dir_getrelname(rightitem);
                while (leftitem != NULL) {
                        lname = dir_getrelname(leftitem);
                        cmpvalue = lstrcmpi(lname, rname);
                        dir_freerelname(leftitem, lname);

                        if (cmpvalue == 0) {
                                /* this is the match */
                                compitem_new(leftitem, rightitem, cl->items, fExact);
                                view_newitem(view);

                                dir_freerelname(rightitem, rname);

                                TickCount = GetTickCount() - TickCount;
                                return(TRUE);
                        }

                        leftitem = dir_nextitem(cl->left, leftitem, fDeep);
                }
                /* not found */
                dir_freerelname(rightitem, rname);
                compitem_new(NULL, rightitem, cl->items, fExact);
                view_newitem(view);
                TickCount = GetTickCount() - TickCount;
                return(TRUE);
        }

        /* two directories */

        /* traverse the two lists in parallel comparing the relative names*/

        leftitem = dir_firstitem(cl->left);
        rightitem = dir_firstitem(cl->right);
        while ((leftitem != NULL) && (rightitem != NULL)) {

                lname = dir_getrelname(leftitem);
                rname = dir_getrelname(rightitem);
                cmpvalue = utils_CompPath(lname, rname);
                dir_freerelname(leftitem, lname);
                dir_freerelname(rightitem, rname);

                if (cmpvalue == 0) {
                        compitem_new(leftitem, rightitem, cl->items, fExact);
                        if (view_newitem(view)) {
                                TickCount = GetTickCount() - TickCount;
                                return(FALSE);
                        }
                        leftitem = dir_nextitem(cl->left, leftitem, fDeep);
                        rightitem = dir_nextitem(cl->right, rightitem, fDeep);

                } else if (cmpvalue < 0) {
                        compitem_new(leftitem, NULL, cl->items, fExact);
                        if (view_newitem(view)) {
                                TickCount = GetTickCount() - TickCount;
                                return(FALSE);
                        }
                        leftitem = dir_nextitem(cl->left, leftitem, fDeep);
                }  else {
                        compitem_new(NULL, rightitem, cl->items, fExact);
                        if (view_newitem(view)) {
                                TickCount = GetTickCount() - TickCount;
                                return(FALSE);
                        }
                        rightitem = dir_nextitem(cl->right, rightitem, fDeep);
                }
        }


        /* any left over are unmatched */
        while (leftitem != NULL) {
                compitem_new(leftitem, NULL, cl->items, fExact);
                if (view_newitem(view)) {
                        TickCount = GetTickCount() - TickCount;
                        return(FALSE);
                }
                leftitem = dir_nextitem(cl->left, leftitem, fDeep);
        }
        while (rightitem != NULL) {
                compitem_new(NULL, rightitem, cl->items, fExact);
                if (view_newitem(view)) {
                        TickCount = GetTickCount() - TickCount;
                        return(FALSE);
                }
                rightitem = dir_nextitem(cl->right, rightitem, fDeep);
        }
        TickCount = GetTickCount() - TickCount;
        return(TRUE);
} /* complist_match */

/* return time last operation took in milliseconds */
DWORD complist_querytime(void)
{       return TickCount;
}


/***************************************************************************
 * Function: complist_dodlg_savelist
 *
 * Purpose:
 *
 * Dialog to query about filename and types of files. Init dlg fields from
 * the dlg_* variables, and save state to the dlg_* variables on dialog
 * close. return TRUE for OK, or FALSE for cancel (from the dialogbox()
 * using EndDialog).
 *
 **************************************************************************/
int FAR PASCAL
complist_dodlg_savelist(HWND hDlg, UINT message, UINT wParam, long lParam)
{
        static char buffer[256];

        switch(message) {


        case WM_INITDIALOG:
                SendDlgItemMessage(hDlg, IDD_IDENTICAL, BM_SETCHECK,
                        dlg_identical ? 1 : 0, 0);
                SendDlgItemMessage(hDlg, IDD_DIFFER, BM_SETCHECK,
                        dlg_differ ? 1 : 0, 0);
                SendDlgItemMessage(hDlg, IDD_LEFT, BM_SETCHECK,
                        dlg_left ? 1 : 0, 0);
                SendDlgItemMessage(hDlg, IDD_RIGHT, BM_SETCHECK,
                        dlg_right ? 1 : 0, 0);

                SetDlgItemText(hDlg, IDD_FILE, dlg_file);

                /* convert 'left tree' into the right name */
                wsprintf((LPTSTR)buffer, LoadRcString(IDS_FILES_ONLY), (LPSTR) dialog_leftname);
                SendDlgItemMessage(hDlg, IDD_LEFT, WM_SETTEXT, 0, (DWORD) (LPSTR) buffer);
                /* convert 'right tree' msg into correct path */
                wsprintf((LPTSTR)buffer, LoadRcString(IDS_FILES_ONLY), (LPSTR) dialog_rightname);
                SendDlgItemMessage(hDlg, IDD_RIGHT, WM_SETTEXT, 0, (DWORD) (LPSTR) buffer);


                return(TRUE);

        case WM_COMMAND:
                switch (GET_WM_COMMAND_ID(wParam, lParam)) {

                case IDOK:
                        dlg_identical = (SendDlgItemMessage(hDlg, IDD_IDENTICAL,
                                        BM_GETCHECK, 0, 0) == 1);
                        dlg_differ = (SendDlgItemMessage(hDlg, IDD_DIFFER,
                                        BM_GETCHECK, 0, 0) == 1);
                        dlg_left = (SendDlgItemMessage(hDlg, IDD_LEFT,
                                        BM_GETCHECK, 0, 0) == 1);
                        dlg_right = (SendDlgItemMessage(hDlg, IDD_RIGHT,
                                        BM_GETCHECK, 0, 0) == 1);
                        GetDlgItemText(hDlg, IDD_FILE, dlg_file, sizeof(dlg_file));

                        EndDialog(hDlg, TRUE);
                        break;

                case IDCANCEL:
                        EndDialog(hDlg, FALSE);
                        break;
                }
        }
        return(FALSE);
} /* complist_dodlg_savelist */

/***************************************************************************
 * Function: complist_dodlg_copyfiles
 *
 * Purpose:
 *
 * dialog to get directory name and inclusion options. Init dlg fields from
 * the dlg_* variables, and save state to the dlg_* variables on dialog
 * close. return TRUE for OK, or FALSE for cancel (from the dialogbox()
 * using EndDialog).
 * 
 **************************************************************************/
int FAR PASCAL
complist_dodlg_copyfiles(HWND hDlg, UINT message, UINT wParam, long lParam)
{
        static char buffer[256];

        switch(message) {


        case WM_INITDIALOG:
                /*
                 * set checkboxes and directory field to defaults
                 */
                CheckDlgButton(hDlg, IDD_IDENTICAL,
                        (dlg_options & INCLUDE_SAME) ? 1 : 0);

                CheckDlgButton(hDlg, IDD_DIFFER,
                        (dlg_options & INCLUDE_DIFFER) ? 1 : 0);

                CheckDlgButton(hDlg, IDD_LEFT,
                        (dlg_options & (INCLUDE_LEFTONLY|INCLUDE_RIGHTONLY)) ? 1 : 0);

                SetDlgItemText(hDlg, IDD_DIR1, dlg_root);

                /*
                 * set 'copy from' buttons to have the full pathname
                 */
                SetDlgItemText(hDlg, IDD_FROMLEFT, dialog_leftname);
                SetDlgItemText(hDlg, IDD_FROMRIGHT, dialog_rightname);

                /*
                 * set default radio button for copy from, and set
                 * the text on the 'files only in...' checkbox to
                 * indicate which path is being selected
                 */
                if (dlg_options & COPY_FROMLEFT) {
                        CheckRadioButton(hDlg, IDD_FROMLEFT, IDD_FROMRIGHT,
                                        IDD_FROMLEFT);

                        wsprintf((LPTSTR)buffer, LoadRcString(IDS_FILES_ONLY), (LPSTR) dialog_leftname);
                        SetDlgItemText(hDlg, IDD_LEFT, buffer);
                } else {
                        CheckRadioButton(hDlg, IDD_FROMLEFT, IDD_FROMRIGHT,
                                        IDD_FROMRIGHT);

                        wsprintf((LPTSTR)buffer, LoadRcString(IDS_FILES_ONLY), (LPSTR) dialog_rightname);
                        SetDlgItemText(hDlg, IDD_LEFT, buffer);
                }

                return(TRUE);

        case WM_COMMAND:
                switch (GET_WM_COMMAND_ID(wParam, lParam)) {

                case IDD_FROMLEFT:
                        wsprintf((LPTSTR)buffer, LoadRcString(IDS_FILES_ONLY), (LPSTR) dialog_leftname);
                        SetDlgItemText(hDlg, IDD_LEFT, buffer);

                        dlg_options &= ~(COPY_FROMRIGHT);
                        dlg_options |= COPY_FROMLEFT;
                        break;

                case IDD_FROMRIGHT:
                        wsprintf((LPTSTR)buffer, LoadRcString(IDS_FILES_ONLY), (LPSTR) dialog_rightname);
                        SetDlgItemText(hDlg, IDD_LEFT, buffer);

                        dlg_options &= ~(COPY_FROMLEFT);
                        dlg_options |= COPY_FROMRIGHT;
                        break;

                case IDOK:
                        if (SendDlgItemMessage(hDlg, IDD_IDENTICAL,
                            BM_GETCHECK, 0, 0) == 1) {
                                dlg_options |= INCLUDE_SAME;
                        } else {
                                dlg_options &= ~INCLUDE_SAME;
                        }
                        if (SendDlgItemMessage(hDlg, IDD_DIFFER,
                            BM_GETCHECK, 0, 0) == 1) {
                                dlg_options |= INCLUDE_DIFFER;
                        } else {
                                dlg_options &= ~INCLUDE_DIFFER;
                        }
                        if (SendDlgItemMessage(hDlg, IDD_LEFT,
                            BM_GETCHECK, 0, 0) == 1) {
                                dlg_options |= INCLUDE_LEFTONLY;
                        } else {
                                dlg_options &= ~INCLUDE_LEFTONLY;
                        }
                        GetDlgItemText(hDlg, IDD_DIR1, dlg_root, sizeof(dlg_root));

                        EndDialog(hDlg, TRUE);
                        break;

                case IDCANCEL:
                        EndDialog(hDlg, FALSE);
                        break;
                }
        }
        return(FALSE);
} /* complist_dodlg_copyfiles */

/***************************************************************************
 * Function: complist_new
 *
 * Purpose:
 *
 * Allocates a new complist and initialise it 
 *
 **************************************************************************/
COMPLIST
complist_new(void)
{
        COMPLIST cl;

        cl = (COMPLIST) gmem_get(hHeap, sizeof(struct complist));
        cl->left = NULL;
        cl->right = NULL;
        cl->items = List_Create();

        return(cl);
} /* complist_new */

/***************************************************************************
 * Function: complist_dodlg_dir
 *
 * Purpose:
 *
 * Dialog box function to ask for two directory names.
 * no listing of files etc - just two edit fields  in which the
 * user can type a file or a directory name.
 *
 * Initialises the names from win.ini, and stores them to win.ini first.
 *
 **************************************************************************/
int FAR PASCAL
complist_dodlg_dir(HWND hDlg, unsigned message, WORD wParam, LONG lParam)
{
        static char path[256];
        static char buffer[256];

        switch (message) {

        case WM_INITDIALOG:

                /* fill the edit fields with the current
                 * directory as a good starting point
                 */
                _getcwd(path, sizeof(path));
                AnsiLowerBuff(path, strlen(path));
                GetProfileString(APPNAME, "NameLeft", path, buffer, 256);
                SetDlgItemText(hDlg, IDD_DIR1, buffer);
                GetProfileString(APPNAME, "NameRight", path, buffer, 256);
                SetDlgItemText(hDlg, IDD_DIR2, buffer);
                /* set recursive option to most recent value */
                CheckDlgButton(hDlg, IDD_RECURSIVE, dlg_recursive);
                return(TRUE);

        case WM_COMMAND:
                switch (LOWORD(wParam)) {
                case IDCANCEL:
                        EndDialog(hDlg, FALSE);
                        return(TRUE);

                case IDOK:
                        /* fetch the text from the dialog, and remember
                         * it in win.ini
                         */

                        GetDlgItemText(hDlg, IDD_DIR1,
                                dialog_leftname, sizeof(dialog_leftname));
                        WriteProfileString(APPNAME, "NameLeft", dialog_leftname);

                        GetDlgItemText(hDlg, IDD_DIR2,
                                dialog_rightname, sizeof(dialog_rightname));
                        WriteProfileString(APPNAME, "NameRight", dialog_rightname);

                        /* fetch recursive option */
                        dlg_recursive = SendDlgItemMessage(hDlg, IDD_RECURSIVE,
                                BM_GETCHECK, 0, 0);

                        EndDialog(hDlg, TRUE);
                        return(TRUE);
                }
                break;
        }
        return(FALSE);
} /* complist_dodlg_dir */

/***************************************************************************
 * Function: complist_open
 *
 * Purpose:
 *      
 * Puts up dialog asking the user to select an existing file to open.
 *
 * Parameters:
 *
 *      prompt - message to user indicating purpose of file
 *               (to be displayed somewhere in dialog box.
 *
 *      ext    - default file extension if user enters file without
 *               extension.
 *
 *      spec   - default file spec (eg *.*)
 *
 *      osp    - OFSTRUCT representing file, if successfully open.
 *
 *      fn     - buffer where filename (just final element) is returned.
 *
 * Returns:
 *
 * TRUE - if file selected and exists (tested with OF_EXIST).
 *
 * FALSE - if dialog cancelled. If user selects a file that we cannot
 *           open, we complain and restart the dialog.
 *
 * Comments:
 *
 *           if TRUE is returned, the file will have been successfully opened,
 *           for reading and then closed again.
 *
 **************************************************************************/

BOOL FAR PASCAL
complist_open(LPSTR prompt, LPSTR ext, LPSTR spec, OFSTRUCT FAR *osp, LPSTR fn)
{
    OPENFILENAME ofn;
    char achFilters[256];
    char achPath[256];
    LPSTR chp;
    int fh;

    /* build filter-pair buffer to contain one pair - the spec filter,
     * twice (one of the pair should be the filter, the second should be
     * the title of the filter - we don't have a title so we use the
     * filter both times. remember double null at end of list of strings.
     */
    lstrcpy(achFilters, spec);             // filter + null
    chp = &achFilters[lstrlen(achFilters)+1];      //2nd string just after null
    lstrcpy(chp, spec);                    // filter name (+null)
    chp[lstrlen(chp)+1] = '\0';            // double null at end of list
    /*
     * initialise arguments to dialog proc
     */
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = NULL;
    ofn.hInstance = NULL;
    ofn.lpstrFilter = achFilters;
    ofn.lpstrCustomFilter = (LPSTR)NULL;
    ofn.nMaxCustFilter = 0L;
    ofn.nFilterIndex = 1L;              // first filter pair in list
    achPath[0] = '\0';
    ofn.lpstrFile = achPath;            // we need to get the full path to open
    ofn.nMaxFile = sizeof(achPath);
    ofn.lpstrFileTitle = fn;            // return final elem of name here
    ofn.nMaxFileTitle = sizeof(fn);
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = prompt;            // dialog title is good place for prompt text
    ofn.Flags = OFN_FILEMUSTEXIST |
                OFN_HIDEREADONLY |
                OFN_PATHMUSTEXIST;
    ofn.lpstrDefExt = ext;
    ofn.nFileOffset = 0;
    ofn.nFileExtension = 0;
    ofn.lCustData = 0;

    /*
     * loop until the user cancels, or selects a file that we can open
     */
    do {
        if (!GetOpenFileName(&ofn)) {
            return(FALSE);
        }

        fh = OpenFile(achPath, osp, OF_READ);
        
        if (fh == HFILE_ERROR) {
            if (MessageBox(NULL, LoadRcString(IDS_COULDNT_BE_OPENED), LoadRcString2(IDS_FILEOPEN),
                            MB_OKCANCEL|MB_ICONSTOP) == IDCANCEL) {
                return(FALSE);
            }
        }
    } while (fh == HFILE_ERROR);

    _lclose(fh);

    return(TRUE);
}

/***************************************************************************
 * Function: complist_getroot_left
 *
 * Purpose:
 *
 * Gets the root names of the left tree used to build this complist.
 *
 **************************************************************************/
LPSTR
complist_getroot_left(COMPLIST cl)
{
        return( dir_getroot_list(cl->left));
}

/***************************************************************************
 * Function: complist_getroot_right
 *
 * Purpose:
 *
 * Gets the root names of the right tree used to build this complist.
 *
 **************************************************************************/
LPSTR
complist_getroot_right(COMPLIST cl)
{
        return( dir_getroot_list(cl->right));
}
/***************************************************************************
 * Function: complist_freeroot_*
 *
 * Purpose:
 *
 * Frees up memory allocated in a call to complist_getroot*() 
 *
 **************************************************************************/
void
complist_freeroot_left(COMPLIST cl, LPSTR path)
{
        dir_freeroot_list(cl->left, path);
}

void
complist_freeroot_right(COMPLIST cl, LPSTR path)
{
        dir_freeroot_list(cl->right, path);
}


