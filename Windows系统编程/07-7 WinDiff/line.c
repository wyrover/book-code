

/****************************** Module Header *******************************
* Module Name: LINE.C
*
* Functions that handle lines of text to be output.
*
* Functions:
*
* line_new()
* line_delete()
* line_reset()
* line_gettext()
* line_gettabbedlength()
* line_getlink()
* line_getlinenr()
* line_compare()
* line_link()
* line_isblank()
*
* Comments:
*
* LINE is a data type representing a string of ascii text along with 
* a line number.
*
* A LINE can compare itself to another line, and maintain a link if the
* lines are similar. 
*
* Comparisons between lines take note of the global option flag
* ignore_blanks, defined elsewhere. If this is true, we ignore
* differences in spaces and tabs when comparing lines, and when
* generating hashcodes.
*
* Links and are only generated once. To clear the link call line_reset.
*
* Lines can be allocated on a list. If a null list handle is passed, the
* line will be allocated using gmem_get() from the hHeap defined and
* initialised elsewhere.
*
****************************************************************************/

#include <windows.h>
#include <stdlib.h>
#include <string.h>

#include "gutils.h"
#include "windiff.h"    /* defines hHeap and ignore_blanks */
#include "list.h"
#include "line.h"

struct fileline {

        UINT flags;     /* see below */

        LPSTR text;     /* null-terminated copy of line text */
        DWORD hash;     /* hashcode for line */
        LINE link;      /* handle for linked line */
        UINT linenr;    /* line number (any arbitrary value) */
};

/* flag values (or-ed) */
#define LF_DISCARD      1       /* if true, alloced from gmem heap */
#define LF_HASHVALID    2       /* if true, hashcode need not be recalced */


/***************************************************************************
 * Function: line_new
 *
 * Purpose:
 *
 * Creates a new line and makes a copy of the text.
 *
 * If the list is non-null, allocate on the list. If null, alloc from
 * gmem_get.
 *
 ***************************************************************************/
LINE
line_new(LPSTR text, int linelength, UINT linenr, LIST list)
{
        LINE line;

        /* alloc a line. from the list if there is a list */
        if (list) {
                line = List_NewLast(list, sizeof(struct fileline));
                if (line == NULL) {
                        return(NULL);
                }
                line->flags = 0;
        } else {
                line = (LINE) gmem_get(hHeap, sizeof(struct fileline));
                if (line == NULL) {
                        return(NULL);
                }
                line->flags = LF_DISCARD;
        }

        /* alloc space for the text. remember the null character */
        line->text = gmem_get(hHeap, linelength + 1);
        strncpy(line->text, text, linelength);
        line->text[linelength] = '\0';

        line->link = NULL;
        line->linenr = linenr;

        return(line);
}

/***************************************************************************
 * Function: line_delete
 *
 * Purpose:
 *
 * Deletes a line and frees up all associated memory and if the line
 * was not alloc-ed from a list, frees up the line struct itself
 *
 ***************************************************************************/
void
line_delete(LINE line)
{
        if (line == NULL) {
                return;
        }

        /* free up text space */
        gmem_free(hHeap, line->text, lstrlen(line->text)+1);

        /* free up line itself only if not on list */
        if (line->flags & LF_DISCARD) {
                gmem_free(hHeap, (LPSTR) line, sizeof(struct fileline));
        }
}

/***************************************************************************
 * Function: line_reset
 *
 * Purpose:
 *
 * Clears the link and force recalc of the hash code.
 *
 ***************************************************************************/
void
line_reset(LINE line)
{
        if (line == NULL) {
                return;
        }

        line->link = NULL;

        line->flags &= ~LF_HASHVALID;
}


/***************************************************************************
 * Function: line_gettext
 *
 * Purpose:
 *
 * Returns a pointer to the line text
 *
 ***************************************************************************/
LPSTR
line_gettext(LINE line)
{
        if (line == NULL) {
                return(NULL);
        }

        return (line->text);
}

/***************************************************************************
 * Function: line_gettabbedlength
 *
 * Purpose:
 *
 * Returns the length of line in characters, expanding tabs. 
 *
 ***************************************************************************/
int
line_gettabbedlength(LINE line, int tabstops)
{
        int length;
        LPSTR chp;

        if (line == NULL) {
                return(0);
        }

        for (length = 0, chp = line->text; *chp != '\0'; chp++) {
                if (*chp == '\t') {
                         length = (length + tabstops) / tabstops * tabstops;
                } else {
                        length++;
                }
        }
        return(length);
}


/***************************************************************************
 * Function: line_gethashcode
 *
 * Purpose:
 *
 * Returns the hashcode for this line 
 *
 ***************************************************************************/
DWORD
line_gethashcode(LINE line)
{
        if (line == NULL) {
                return(0);
        }

        if (! (line->flags & LF_HASHVALID)) {


                /* hashcode needs to be recalced */
                line->hash = hash_string(line->text, ignore_blanks);
                line->flags |= LF_HASHVALID;
        }
        return (line->hash);
}

/***************************************************************************
 * Function: line_getlink
 *
 * Purpose:
 *
 * Returns the handle for the line that is linked to this line (the
 * result of a successful line_link() operation). This line is
 * identical in text to the linked line (allowing for ignore_blanks).
 *
 ***************************************************************************/
LINE
line_getlink(LINE line)
{
        if (line == NULL) {
                return(NULL);
        }

        return(line->link);
}

/***************************************************************************
 * Function: line_getlinenr
 *
 * Purpose:
 *
 * Returns the line number associated with this line 
 *
 ***************************************************************************/
UINT
line_getlinenr(LINE line)
{
        if (line == NULL) {
                return(0);
        }

        return(line->linenr);
}

/***************************************************************************
 * Function: line_compare
 *
 * Purpose:
 *
 * Compares two lines and returns TRUE if they are the same.
 *
 ***************************************************************************/
BOOL
line_compare(LINE line1, LINE line2)
{
        LPSTR p1, p2;

        /* Assert: At least one of them is not null ??? */

        if ((line1 == NULL) || (line2 == NULL)) {
                /* null line handles do not compare */
                return(FALSE);
        }

        /* check that the hashcodes match */
        if (line_gethashcode(line1) != line_gethashcode(line2)) {
                return(FALSE);
        }

        /* hashcodes match - are the lines really the same ? */
        /* note that this is coupled to gutils\utils.c in definition of blank */
        p1 = line_gettext(line1);
        p2 = line_gettext(line2);

// Japanese friendy
        do {
                if (ignore_blanks) {
                        while ( (*p1 == ' ') || (*p1 == '\t')) {
                                p1 = CharNext(p1);
                        }
                        while ( (*p2 == ' ') || (*p2 == '\t')) {
                                p2 = CharNext(p2);
                        }
                }
                if (IsDBCSLeadByte(*p1) && *(p1+1) != '\0'
                &&  IsDBCSLeadByte(*p2) && *(p2+1) != '\0') {
                        if (*p1 != *p2 || *(p1+1) != *(p2+1)) {
                                return(FALSE);
                        }
                        p1 += 2;
                        p2 += 2;
                } else {
                        if (*p1 != *p2) {
                                return(FALSE);
                        }
                        p1++;
                        p2++;
                }
        } while ( (*p1 != '\0') && (*p2 != '\0'));

        return(TRUE);
}

/***************************************************************************
 * Function: line_link
 *
 * Purpose:
 *
 * Attempts to link two lines and returns TRUE if succesful.
 *
 * This will fail if either line is NULL, or already linked, or if
 * they differ.
 *
 ***************************************************************************/
BOOL
line_link(LINE line1, LINE line2)
{
        if ( (line1 == NULL) || (line2 == NULL)) {
                return(FALSE);
        }

        if ( (line1->link != NULL) || (line2->link != NULL)) {
                return(FALSE);
        }

        if (line_compare(line1, line2)) {
                line1->link = line2;
                line2->link = line1;
                return(TRUE);
        } else {
                return(FALSE);
        }
}


/***************************************************************************
 * Function: line_isblank
 *
 * Purpose:
 *
 * Returns TRUE iff line is blank.  NULL => return FALSE 
 *
 ***************************************************************************/
BOOL line_isblank(LINE line)
{
        return line!=NULL && utils_isblank(line->text);
}
