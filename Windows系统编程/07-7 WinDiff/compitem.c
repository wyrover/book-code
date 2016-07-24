

/****************************** Module Header *******************************
* Module Name: COMPITEM.C
*
* Module which does the comparison between two files. 
*
* Functions:
*
* ci_copytext()
* ci_makecomposite()
* ci_compare()
* ci_onesection()
* compitem_new()
* compitem_delete()
* compitem_discardsections()
* compitem_getcomposite()
* compitem_getleftsections()
* compitem_getrightsections()
* compitem_getleftfile()
* compitem_getrightfile()
* compitem_getstate()
* compitem_gettext_tag()
* compitem_gettext_result()
* compitem_getfilename()
* compitem_frefilename()
*
* Comments:
*
* This module uses the structure compitem which is a data type that knows
* about two files, and can compare them. The result of the comparison
* is a list of sections for each file, and a composite list of sections
* representing the comparison of the two files.
*
* A compitem has a state (one of the integer values defined in state.h)
* representing the result of the comparison. It can also be
* queried for the text result (text equivalent of the state) as well
* as the tag - or title for this compitem (usually a text string containing
* the name(s) of the files being compared).
*
* A compitem will supply a composite section list even if the files are
* the same, or if there is only one file. The composite section list will
* only be built (and the files read in) when the compitem_getcomposite()
* call is made (and not at compitem_new time).
* 
*  
****************************************************************************/

#include <windows.h>
#include <stdlib.h>
#include <string.h>

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


struct compitem {

        FILEDATA left;          /* handle for left-hand file */
        FILEDATA right;         /* handle for right-hand file */

        LIST secs_composite;    /* list of sections (composite file)*/
        LIST secs_left;         /* list of sections (left file) */
        LIST secs_right;        /* list of sections (right file) */

        int state;              /* compitem state - result of compare */
        BOOL bDiscard;          /* true if not alloc-ed on list */
        LPSTR tag;              /* text for tag (title of compitem) */
        LPSTR result;           /* text equivalent of state */

};


LPSTR ci_copytext(LPSTR in);
void ci_makecomposite(COMPITEM ci);
void ci_compare(COMPITEM ci);


/***************************************************************************
 * Function: compitem_new
 *
 * Purpose:
 *
 * Returns a handle to a new compitem - given the filenames for the
 * left and right files to be compared. Either left or right or neither
 * (but not both) may be null. In this case we set the state accordingly.
 *
 * The parameters are handles to DIRITEM objects: these allow us to get the
 * the name of the file relative to the compare roots (needed for the tag)
 * and the absolute name of the file (needed for opening the file).
 *
 * Comments:
 *
 * If the list parameter is not null, the List_New* functions are used to
 * allocate memory for the compitem. We remember this (in the bDiscard flag)
 * so we do not delete the compitem if it was allocated on the list.
 *
 * If the list parameter is null, the memory
 * for the compitem is allocated from the gmem_* heap initialised by the app.
 *
 ****************************************************************************/
COMPITEM
compitem_new(DIRITEM leftname, DIRITEM rightname, LIST list, BOOL fExact)
{
        COMPITEM ci;
        LPSTR str1, str2;
        char buf[2*MAX_PATH+20];


        /*
         * Allocate the memory for the compitem, either at the end of the
         * list or in the gmem_* heap.
         */
        if (list == NULL) {
                /* no list passed */
                ci = (COMPITEM) gmem_get(hHeap, sizeof(struct compitem));
                memset(ci, 0, sizeof(struct compitem));
                ci->bDiscard = TRUE;
        } else {
                /* add to end of list */
                ci = (COMPITEM) List_NewLast(list, sizeof(struct compitem));
                ci->bDiscard = FALSE;
        }

        ci->secs_composite = NULL;
        ci->secs_left = NULL;
        ci->secs_right = NULL;

        /*
         * Make a filedata for each of the files that are non-null.
         * Filedata objects are responsible for reading the file and
         * accessing the lines in it. Don't read in the files until we need to.
         */
        if (leftname != NULL) {
                ci->left = file_new(leftname, FALSE);
                if (ci->left == NULL) {
                        return(NULL);
                }
        } else {
                ci->left = NULL;
        }
        if ( rightname != NULL) {
                ci->right = file_new(rightname, FALSE);
                if (ci->right == NULL) {
                        return(NULL);
                }
        } else {
                ci->right = NULL;
        }


        /*
         * See if we have one or two files, and set the state accordingly
         */
        if ( ! ci->left && !ci->right) {
                /* two NULL files - this is wrong */
                return(NULL);
        }


        /* Set the tag (title field) for this item. If the
         * two files have names that match, we use just that name -
         * otherwise we use both names separated by a colon 'left : right'.
         *
         * In both cases, use the names relative to compare root (the
         * names will certainly be different if we compare the abs paths)
         */
        str1 = dir_getrelname(leftname);
        str2 = dir_getrelname(rightname);

        /* If only one file - set name to that */
        if (ci->left == NULL) {
                ci->tag = ci_copytext(str2);
        } else if (ci->right == NULL) {
                ci->tag = ci_copytext(str1);
        } else {
                if (lstrcmpi(str1, str2) == 0) {
                        ci->tag = ci_copytext(str2);
                } else {
                        wsprintf(buf, "%s : %s", str1, str2);
                        ci->tag = ci_copytext(buf);
                }
        }

        dir_freerelname(leftname, str1);
        dir_freerelname(leftname, str2);


        if (ci->left == NULL) {
                str1 = dir_getroot_item(rightname);
                wsprintf(buf, LoadRcString(IDS_ONLY_IN), str1);
                dir_freeroot_item(rightname, str1);

                ci->result = ci_copytext(buf);
                ci->state = STATE_FILERIGHTONLY;
        } else if (ci->right == NULL) {
                str1 = dir_getroot_item(leftname);
                wsprintf(buf, LoadRcString(IDS_ONLY_IN), str1);
                dir_freeroot_item(leftname, str1);

                ci->result = ci_copytext(buf);
                ci->state = STATE_FILELEFTONLY;
        } else {
                /* two files - are they the same ? compare
                 * the file sizes
                 */


                if (dir_getfilesize(leftname) != dir_getfilesize(rightname)) 
                {
                    ci->state = STATE_DIFFER;
                    ci->result = ci_copytext(LoadRcString(IDS_DIFFERENT_SIZES));
                } else if (!fExact){
                    ci->result = ci_copytext(LoadRcString(IDS_SAME_SIZE));
                    ci->state = STATE_SAME;
                } else {
                    ci->result =  ci_copytext(LoadRcString(IDS_IDENTICAL));
                    ci->state = STATE_SAME;
                }
        }


#if FALSE
                if (dir_getfilesize(leftname) == dir_getfilesize(rightname)) {
                    if (  !fExact )
                       {
                        ci->result = ci_copytext("same size etc.");
                        ci->state = STATE_SAME;
                    } else {
                        ci->result = ci_copytext("files differ");
                        ci->state = STATE_DIFFER;
                        ci->result = ci_copytext("files differ");
                     }
                } else {
                        ci->result = ci_copytext("files differ");
                        ci->state = STATE_DIFFER;
                }
        }
#endif

        /*
         * Building the section lists and composite lists can wait
         * until needed.
         */
        return(ci);
} /* compitem_new */

/***************************************************************************
 * Function: compitem_delete
 *
 * Purpose:
 *
 * Deletes a compitem and free all associated data.
 *
 * Comments:
 *
 * If the ci->bDiscard flag is set, the compitem was alloc-ed on a list,
 * and should not be discarded (the list itself will be deleted).
 *
 * The DIRDATA we were passed are not deleted. the filedata, lines
 * and sections are.
 ***************************************************************************/
void
compitem_delete(COMPITEM ci)
{
        if (ci == NULL) {
                return;
        }

        compitem_discardsections(ci);

        /* Delete the two filedatas (and associated line lists) */
        file_delete(ci->left);
        file_delete(ci->right);

        /* text we allocated */
        gmem_free(hHeap, ci->tag, lstrlen(ci->tag) + 1);
        gmem_free(hHeap, ci->result, lstrlen(ci->result) + 1);

        /* Free the compitem struct itself if not alloced on a list */
        if (ci->bDiscard) {
                gmem_free(hHeap, (LPSTR) ci, sizeof(struct compitem));
        }
}


/*
/***************************************************************************
 * Function: compitem_discardsections
 *
 * Purpose:
 *
 * To discard sections - throw away cached information relating to the
 * comparison (but not the files if they are read into memory). This
 * is used to force a re-compare if changes in the comparison options
 * are made
 *
 ***************************************************************************/
void
compitem_discardsections(COMPITEM ci)
{
        /* delete the lists of sections we built */
        if (ci == NULL) {
                return;
        }
        if (ci->secs_composite) {
                section_deletelist(ci->secs_composite);
                ci->secs_composite = NULL;
        }
        if (ci->secs_left) {
                section_deletelist(ci->secs_left);
                ci->secs_left = NULL;
        }
        if (ci->secs_right) {
                section_deletelist(ci->secs_right);
                ci->secs_right = NULL;
        }

        /* reset the line lists to throw away cached hash codes and links */
        if (ci->left != NULL) {
                file_reset(ci->left);
        }
        if (ci->right != NULL) {
                file_reset(ci->right);
        }

}

/***************************************************************************
 * Function: compitem_getcomposite
 *
 * Purpose:
 *
 * To get the handle for the composite section list 
 *
 ***************************************************************************/
LIST
compitem_getcomposite(COMPITEM ci)
{
        if (ci == NULL) {
                return NULL;
        }
        /*
         * do the comparison if we haven't already done it
         */
        if (ci->secs_composite == NULL) {
                ci_makecomposite(ci);
        }

        return(ci->secs_composite);
}

/***************************************************************************
 * Function: compitem_getleftsections
 *
 * Purpose:
 *
 * To get the handle for the list of sections in the left file 
 *
 ***************************************************************************/
LIST
compitem_getleftsections(COMPITEM ci)
{
        if (ci == NULL) {
                return NULL;
        }
        /*
         * do the comparison if we haven't already done it
         */
        if (ci->secs_composite == NULL) {
                ci_makecomposite(ci);
        }

        return(ci->secs_left);
}

/***************************************************************************
 * Function: compitem_getrightsections
 *
 * Purpose:
 *
 * To get the handle for the list of sections in the right file 
 *
 ***************************************************************************/
LIST
compitem_getrightsections(COMPITEM ci)
{
        if (ci == NULL) {
                return NULL;
        }
        /*
         * do the comparison if we haven't already done it
         */
        if (ci->secs_composite == NULL) {
                ci_makecomposite(ci);
        }

        return(ci->secs_right);
}

/***************************************************************************
 * Function: compitem_getleftfile
 *
 * Purpose:
 *
 * To get the handle to the left file itself
 *
 ***************************************************************************/
FILEDATA
compitem_getleftfile(COMPITEM ci)
{
        if (ci == NULL) {
                return(NULL);
        }
        return(ci->left);
}

/***************************************************************************
 * Function: compitem_getrightfile
 *
 * Purpose:
 *
 * To get the handle to the right file itself 
 *
 ***************************************************************************/
FILEDATA
compitem_getrightfile(COMPITEM ci)
{
        if (ci == NULL) {
                return(NULL);
        }
        return(ci->right);
}

/***************************************************************************
 * Function: compitem_getstate
 *
 * Purpose:
 *
 * To get the state (compare result) of this compitem 
 *
 ***************************************************************************/
int
compitem_getstate(COMPITEM ci)
{
        if (ci == NULL) {
                return(0);
        }
        return(ci->state);
}

/***************************************************************************
 * Function: compitem_gettext_tag
 *
 * Purpose:
 *
 * To get the tag (text for the compitem title) 
 *
 ***************************************************************************/
LPSTR
compitem_gettext_tag(COMPITEM ci)
{
        if (ci == NULL) {
                return(NULL);
        }
        return(ci->tag);
}

/***************************************************************************
 * Function: compitem_gettext_result
 *
 * Purpose:
 *
 * To get the result text (text equiv of state) 
 *
 ***************************************************************************/
LPSTR
compitem_gettext_result(COMPITEM ci)
{
        if (ci == NULL) {
                return(NULL);
        }
        return(ci->result);
}

/***************************************************************************
 * Function: compitem_getfilename
 *
 * Purpose:
 *
 * To return the name of the file associated with this compitem. The option
 * argument (one of CI_LEFT, CI_RIGHT, CI_COMP) indicates which file
 * is required.
 *
 * Comments:
 *
 * CI_LEFT and CI_RIGHT just result in calls to dir_getopenname to get
 * an open-able filename.
 *
 * For CI_COMP, we create a temporary file, write out all the text in the
 * composite section list to this file, and then pass the name of the
 * temporary file to the caller. This file will be deleted on
 * the call to compitem_freefilename().
 * 
 ***************************************************************************/
LPSTR
compitem_getfilename(COMPITEM item, int option)
{
        LPSTR fname;
        LINE line;
        LPSTR tag, text;
        SECTION sec;
        OFSTRUCT os;
        int fh;

        if (item == NULL) {
                return(NULL);
        }

        switch(option) {
        case CI_LEFT:
                if (item->left != NULL) {
                        return(dir_getopenname(file_getdiritem(item->left)));
                } else {
                        return(NULL);
                }

        case CI_RIGHT:
                if (item->right != NULL) {
                        return(dir_getopenname(file_getdiritem(item->right)));
                } else {
                        return(NULL);
                }

        case CI_COMP:

                /* caller has asked for the filename of the composite file.
                 * we need to create a temporary file and write the
                 * lines in the composite section list out to it.
                 */
                fname = gmem_get(hHeap, MAX_PATH);
                GetTempPath(MAX_PATH, fname);
                GetTempFileName(fname, "wdf", 0, fname);

                fh = OpenFile(fname, &os, OF_READWRITE|OF_SHARE_DENY_NONE);
                if (fh < 0) {
                        MessageBox(NULL, LoadRcString(IDS_CANT_OPEN_TMP_FILE), NULL, MB_OK | MB_ICONSTOP);
                        return(NULL);
                }

                /* make sure the composite list has been built */

                if (item->secs_composite == NULL) {
                        ci_makecomposite(item);
                }

                /* write out every line in every section on the composite
                 * list to the temp file.
                 */
                List_TRAVERSE(item->secs_composite, sec) {

                        /* get the tag field based on the section state*/
                        switch(section_getstate(sec)) {
                        case STATE_SAME:
                                tag = "    ";
                                break;

                        case STATE_LEFTONLY:
                                tag = " <! ";
                                break;
                        case STATE_RIGHTONLY:
                                tag = " !> ";
                                break;

                        case STATE_MOVEDLEFT:
                                tag = " <- ";
                                break;

                        case STATE_MOVEDRIGHT:
                                tag = " -> ";
                                break;
                        }

                        /* write out each line in this section.
                         * non-standard traverse of list as we only
                         * want to go from section first to section last
                         * inclusive.
                         */
                        for (line = section_getfirstline(sec);
                             line != NULL;
                             line = List_Next(line) ) {

                                text = line_gettext(line);

                                /* write out to file */
                                _lwrite(fh, tag, lstrlen(tag));
                                _lwrite(fh, text, lstrlen(text));

                                if (line == section_getlastline(sec)) {
                                        break;
                                }
                        }
                }

                /* now close the file and return its name */
                _lclose(fh);
                return(fname);


        default:
                MessageBox(NULL, LoadRcString(IDS_BAD_ARGUMENT), NULL, MB_OK | MB_ICONSTOP);
                return(NULL);
        }
}

/***************************************************************************
 * Function: compitem_freefilename
 *
 * Purpose:
 *
 * Free memory created by a call to compitem_getfilename. If a temporary
 * file was created, this may cause it to be deleted. The option argument must
 * be the same as passed to the original compitem_getfilename call.
 *
 * If we created a temporary file for CI_COMP, then delete it; otherwise,
 * just pass the name to dir_freeopenname.
 *
 ***************************************************************************/
void
compitem_freefilename(COMPITEM item, int option, LPSTR filename)
{
        OFSTRUCT os;


        if ((item == NULL) || (filename == NULL)) {
                return;
        }

        switch(option) {

        case CI_LEFT:
                dir_freeopenname(file_getdiritem(item->left), filename);
                break;

        case CI_RIGHT:
                dir_freeopenname(file_getdiritem(item->right), filename);
                break;

        case CI_COMP:

                /* this is a temporary file we created. Delete it. */
                OpenFile(filename, &os, OF_DELETE);

                gmem_free(hHeap, filename, MAX_PATH);
                break;
        }
}


/***************************************************************************
 * Function: ci_copytext
 *
 * Purpose:
 *
 * To alloc a buffer large enough for the text string and copy the text into
 * it and return a pointer to the string.
 *
 ***************************************************************************/
LPSTR
ci_copytext(LPSTR in)
{
        LPSTR out;

        if (in == NULL) {
                out = gmem_get(hHeap, 1);
                out[0] = '\0';
        } else {
                out = gmem_get(hHeap, lstrlen(in) + 1);
                lstrcpy(out, in);
        }
        return(out);
}

/***************************************************************************
 * Function: ci_onesection
 *
 * Purpose:
 *
 * To make a list containing a single section from the whole list of lines 
 *
 ***************************************************************************/
LIST
ci_onesection(FILEDATA file)
{
        LIST lines;
        LIST sections;
        SECTION section;

        lines = file_getlinelist(file);

        /* create a null list */
        sections = List_Create();

        /* tell the section to create itself on the end of this list. */
        section = section_new(List_First(lines), List_Last(lines), sections);
        section_setstate(section, STATE_SAME);


        return(sections);
}



/***************************************************************************
 * Function: ci_makecomposite
 *
 * Purpose:
 *
 * Compare the two files and build the composite list. This function is
 * called whenever we need one of the section lists and only does the 
 * comparison if the composite list does not already exist.
 *
 ***************************************************************************/
void
ci_makecomposite(COMPITEM ci)
{
        if (ci->secs_composite != NULL) {
                return;
        }

        /* if there is only one file, make a single item list
         * of sections
         */
        if (ci->left == NULL) {
                ci->secs_left = NULL;
                ci->secs_right = ci_onesection(ci->right);

                /* make a second list, not a pointer to the first
                 * or we will get confused when deleting
                 */
                ci->secs_composite = ci_onesection(ci->right);
                return;
        } else if (ci->right == NULL) {
                ci->secs_right = NULL;
                ci->secs_left = ci_onesection(ci->left);

                /* make a second list, not a pointer to the first
                 * or we will get confused when deleting
                 */
                ci->secs_composite = ci_onesection(ci->left);
                return;
        }

        /* we have two files - we need to compare them fully */
        ci_compare(ci);
}

/***************************************************************************
 * Function: ci_compare
 *
 * Purpose:
 *
 * Compare files and build a composite list.
 *
 * Comments:
 *
 * Comparison method:
 *
 *    0   Break each file into lines and hash each line.  Lines which 
 *        don't match can be rapidly eliminated by comparing the hash code.
 *
 *        Store the hash codes in a binary search tree that
 *        will give for each hash code the number of times that it
 *        occurred in each file and one of the lines where it occurred
 *        in each file.  The tree is used to rapidly find the partner
 *        of a line which occurs exactly once in each file.
 *
 *    1   Make a section covering the whole file (for both)
 *        and link unique lines between these sections (i.e. link lines
 *        which occur exactly once in each file as they must match each other).
 *        These are referred to as anchor points.
 *
 *    2   Build section lists for both files by traversing line lists and
 *        making a section for each set of adjacent lines that are unmatched
 *        and for each set of adjacent lines that match a set of adjacent
 *        lines in the other file.  In making a section we start from a
 *        known matching line and work both forwards and backwards through
 *        the file including lines which match, whether they are unique or not.
 *
 *    3   Establish links between sections that match
 *        and also between sections that don't match but do
 *        correspond (by position in file between matching sections)
 *
 *    4   For each section pair that don't match but do correspond,
 *        link up matching lines unique within that section.  (i.e. do
 *        the whole algorithm again on just this section).
 *
 *    There may be some lines which occur many times over in each file.
 *    As these occurrences are matched up, so the number left to match
 *    reduces, and may reach one in each file.  At this point these two
 *    can be matched.  Therefore we...
 *
 *    Repeat steps 0-4 until no more new links are added, but (especially
 *    in step 0) we only bother with lines which have not yet been matched.
 *    This means that a line which has only one unmatched instance in each
 *    file gets a count of one and so is a new anchor point.
 *
 *    Finally build a composite list from the two lists of sections.
 *
 ***************************************************************************/
void
ci_compare(COMPITEM ci)
{
        LIST lines_left, lines_right;
        SECTION whole_left, whole_right;
        BOOL bChanges;

        /* get the list of lines for each file */
        lines_left = file_getlinelist(ci->left);
        lines_right = file_getlinelist(ci->right);

        if ((lines_left == NULL) || (lines_right == NULL)) {
                ci->secs_left = NULL;
                ci->secs_right = NULL;
                ci->secs_composite = NULL;
                return;
        }

        do {

                /* we have made no changes so far this time round the
                 * loop
                 */
                bChanges = FALSE;

                /* make a section covering the whole file */
                whole_left = section_new(List_First(lines_left),
                                         List_Last(lines_left), NULL);

                whole_right = section_new(List_First(lines_right),
                                         List_Last(lines_right), NULL);

                /* link up matching unique lines between these sections */
                if (section_match(whole_left, whole_right)) {
                        bChanges = TRUE;
                }

                /* delete the two temp sections */
                section_delete(whole_left);
                section_delete(whole_right);

                /* discard previous section lists if made */
                if (ci->secs_left) {
                        section_deletelist(ci->secs_left);
                        ci->secs_left = NULL;
                }
                if (ci->secs_right) {
                        section_deletelist(ci->secs_right);
                        ci->secs_right = NULL;
                }
                /* build new section lists for both files */
                ci->secs_left = section_makelist(lines_left, TRUE);
                ci->secs_right = section_makelist(lines_right, FALSE);

                /* match up sections - make links and corresponds between
                 * sections. Attempts to section_match corresponding
                 * sections that are not matched. returns true if any
                 * further links were made
                 */
                if (section_matchlists(ci->secs_left, ci->secs_right)) {
                        bChanges = TRUE;
                }

        /* repeat as long as we keep adding new links */

        } while (bChanges);

        /* all possible lines linked, and section lists made .
         * combine the two section lists to get a view of the
         * whole comparison - the composite section list. This also
         * sets the state of each section in the composite list.
         */
        ci->secs_composite = section_makecomposite(ci->secs_left, ci->secs_right);

}









