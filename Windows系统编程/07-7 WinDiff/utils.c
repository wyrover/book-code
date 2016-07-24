

/****************************** Module Header *******************************
* Module Name: UTILS.C
*
* standard file-reading utilities.
*
* Functions:
*
* readfile_new()
* readfile_next()
* readfile_delete()
* utils_CompPath()
* has_string()
* utils_isblank()
* StringInput()
* dodlg_stringin()
*
* Comments:
*
****************************************************************************/

#include <windows.h>
#include <stdlib.h>
#include <string.h>

#include "gutils.h"
#include "gutilsrc.h"


/*
 * we need an instance handle. this should be the dll instance
 */
extern HANDLE hLibInst;



/*
 * -- forward declaration of procedures -----------------------------------
 */
int FAR PASCAL dodlg_stringin(HWND hDlg, UINT message, UINT wParam, LONG lParam);




/*-- readfile: buffered line input ------------------------------*/

/*
 * set of functions to read a line at a time from a file, using
 * a buffer to read a block at a time from the file
 *
 */

/*
 * a FILEBUFFER handle is a pointer to a struct filebuffer
 */
struct filebuffer {
        int fh;         /* open file handle */
        PSTR start;     /* offset within buffer of next character */
        PSTR last;      /* offset within buffer of last valid char read in */

        char buffer[512];
};

/***************************************************************************
 * Function: readfile_new
 *
 * Purpose:
 *
 * Initialise a filebuffer and return a handle to it
 */
FILEBUFFER APIENTRY
readfile_new(int fh)
{
        FILEBUFFER fbuf;

        fbuf = (FILEBUFFER) LocalLock(LocalAlloc(LHND, sizeof(struct filebuffer)));
        if (fbuf == NULL) {
                return(NULL);
        }

        fbuf->fh = fh;
        fbuf->start = fbuf->buffer;
        fbuf->last = fbuf->buffer;
        /* return file pointer to beginning of file */
        _llseek(fh, 0, 0);

        return(fbuf);
}

/***************************************************************************
 * Function: readfile_next
 *
 * Purpose:
 *
 * Get the next line from a file. Returns a pointer to the line
 * in the buffer - so copy it before changing it.
 *
 * The line is *not* null-terminated. *plen is set to the length of the
 * line.
 */
LPSTR APIENTRY
readfile_next(FILEBUFFER fbuf, int FAR * plen)
{
        PSTR cstart;

        /* look for an end of line in the buffer we have*/
        for (cstart = fbuf->start; cstart < fbuf->last; cstart++) {

                if (*cstart == '\n') {
                        *plen = (cstart - fbuf->start) + 1;
                        cstart = fbuf->start;
                        fbuf->start += *plen;
                        return(cstart);
                }

        }

        /* no cr in this buffer - this buffer contains a partial line.
         * copy the partial up to the beginning of the buffer, and
         * adjust the pointers to reflect this move
         */
        Old_strncpy(fbuf->buffer, fbuf->start, fbuf->last - fbuf->start);
        fbuf->last = &fbuf->buffer[fbuf->last - fbuf->start];
        fbuf->start = fbuf->buffer;

        /* read in to fill the block */
        fbuf->last += _lread(fbuf->fh, fbuf->last,
                        &fbuf->buffer[sizeof(fbuf->buffer)] - fbuf->last);

        /* look for an end of line in the newly filled buffer */
        for (cstart = fbuf->start; cstart < fbuf->last; cstart++) {

                if (*cstart == '\n') {
                        *plen = (cstart - fbuf->start) + 1;
                        cstart = fbuf->start;
                        fbuf->start += *plen;
                        return(cstart);
                }
        }


        /* still no end of line. either the buffer is empty -
         * because of end of file - or the line is longer than
         * the buffer. in either case, return all that we have
         */
        *plen = fbuf->last - fbuf->start;
        { // for JAPAN (nChars != nBytes)
            PSTR ptr;
            for(ptr=fbuf->start;ptr<fbuf->last;ptr++) ;
            if(ptr!=fbuf->last && *plen) {
                --(*plen);
                --(fbuf->last);
                _llseek(fbuf->fh,-1,1);
            }
        }
        cstart = fbuf->start;
        fbuf->start += *plen;
        if (*plen == 0) {
                return(NULL);
        } else {
                return(cstart);
        }
}


/***************************************************************************
 * Function: readfile_delete
 *
 * Purpose:
 *
 * Delete a FILEBUFFER - close the file handle and free the buffer
 */
void APIENTRY
readfile_delete(FILEBUFFER fbuf)
{
        _lclose(fbuf->fh);

        LocalUnlock(LocalHandle( (PSTR) fbuf));
        LocalFree(LocalHandle( (PSTR) fbuf));
}


/* ----------- things for strings-------------------------------------*/


/*
 * Compare two pathnames, and if not equal, decide which should come first.
 * Both path names should be lower cased by AnsiLowerBuff before calling.
 *
 * Returns 0 if the same, -1 if left is first, and +1 if right is first.
 *
 * The comparison is such that all filenames in a directory come before any
 * file in a subdirectory of that directory.
 *
 * Given direct\thisfile v. direct\subdir\thatfile, we take
 * thisfile < thatfile   even though it is second alphabetically.
 * We do this by picking out the shorter path
 * (fewer path elements), and comparing them up till the last element of that
 * path (in the example: compare the 'dir\' in both cases.)
 * If they are the same, then the name with more path elements is
 * in a subdirectory, and should come second.
 *
 * We have had trouble with apparently multiple collating sequences and
 * the position of \ in the sequence.  To eliminate this trouble
 * a. EVERYTHING is mapped to lower case first (actually this is done
 *    before calling this routine).
 * b. All comparison is done by using lstrcmpi with two special cases.
 *    1. Subdirs come after parents as noted above
 *    2. \ must compare low so that fred2\x > fred\x in the same way
 *       that fred2 < fred.  Unfortunately in ANSI '2' < '\\'
 *
 */
int APIENTRY
utils_CompPath(LPSTR left, LPSTR right)
{
        int compval;            // provisional value of comparison

        if (left==NULL) return -1;        // empty is less than anything else
        else if (right==NULL) return 1;  // anything is greater than empty

        for (; ; ) {
                if (*left=='\0' && *right=='\0') return 0;
                if (*left=='\0')  return -1;
                if (*right=='\0')  return 1;
                if (IsDBCSLeadByte(*left) || IsDBCSLeadByte(*right)) {
                        if (*right != *left) {
                                compval = (*left - *right);
                                break;
                        }
                        ++left;
                        ++right;
                        if (*right != *left) {
                                compval = (*left - *right);
                                break;
                        }
                        ++left;
                        ++right;
                } else {
                if (*right==*left)  {++left; ++right; continue;}
                if (*left=='\\') {compval = -1; break;}
                if (*right=='\\') {compval = 1; break;}
                compval = (*left - *right);
                break;
                }
        }

        /* We have detected a difference.  If the rest of one
           of the strings (including the current character) contains
           some \ characters, but the other one does not, then all
           elements up to the last element of the one with the fewer
           elements are equal and so the other one lies in a subdir
           and so compares greater i.e. x\y\f > x\f
           Otherwise compval tells the truth.
        */

        left = strchr(left, '\\');
        right = strchr(right, '\\');
        if (left && !right) return 1;
        if (right && !left) return -1;

        return compval;

} /* utils_CompPath */


/***************************************************************************
 * Function: hash_string
 *
 * Purpose:
 *
 * Generate a hashcode for a null-terminated ascii string.
 *
 * If bIgnoreBlanks is set, then ignore all spaces and tabs in calculating
 * the hashcode.
 *
 * Multiply each character by a function of its position and sum these.
 * The function chosen is to multiply the position by successive
 * powers of a large number.
 * The large multiple ensures that anagrams generate different hash
 * codes.
 */
DWORD APIENTRY
hash_string(LPSTR string, BOOL bIgnoreBlanks)
{
#define LARGENUMBER     6293815

        DWORD sum = 0;
        DWORD multiple = LARGENUMBER;
        int index = 1;

        while (*string != '\0') {

                if (bIgnoreBlanks) {
                        while ( (*string == ' ') || (*string == '\t')) {
                                string = CharNext(string);
                        }
                }

                sum += multiple * index++ * (*string++);
                multiple *= LARGENUMBER;
        }
        return(sum);
}


/***************************************************************************
 * Function: utils_isblank
 *
 * Purpose:
 *
 * Return TRUE iff the string is blank.  Blank means the same as
 * the characters which are ignored in hash_string when ignore_blanks is set
 */
BOOL APIENTRY
utils_isblank(LPSTR string)
{
        while ( (*string == ' ') || (*string == '\t')) {
                string = CharNext(string);
        }

        /* having skipped all the blanks, do we see the end delimiter? */
        return (*string == '\0' || *string == '\r' || *string == '\n');
}



/* --- simple string input -------------------------------------- */

/*
 * static variables for communication between function and dialog
 */
LPSTR dlg_result;
int dlg_size;
LPSTR dlg_prompt, dlg_default, dlg_caption;

/***************************************************************************
 * Function: StringInput
 *
 * Purpose:
 *
 * Input of a single text string, using a simple dialog.
 *
 * Returns TRUE if ok, or FALSE if error or user canceled. If TRUE,
 * puts the string entered into result (up to resultsize characters).
 *
 * Prompt is used as the prompt string, caption as the dialog caption and
 * default as the default input. All of these can be null.
 */

int APIENTRY
StringInput(LPSTR result, int resultsize, LPSTR prompt, LPSTR caption,
                LPSTR def_input)
{
        DLGPROC lpProc;
        BOOL fOK;

        /* copy args to static variable so that winproc can see them */

        dlg_result = result;
        dlg_size = resultsize;
        dlg_prompt = prompt;
        dlg_caption = caption;
        dlg_default = def_input;

        lpProc = (DLGPROC)MakeProcInstance((WNDPROC)dodlg_stringin, hLibInst);
        fOK = DialogBox(hLibInst, "StringInput", GetFocus(), lpProc);
        FreeProcInstance((WNDPROC)lpProc);

        return(fOK);
}

/***************************************************************************
 * Function: dodlg_stringin
 *
 */
int FAR PASCAL
dodlg_stringin(HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
        switch(message) {

        case WM_INITDIALOG:
                if (dlg_caption != NULL) {
                        SendMessage(hDlg, WM_SETTEXT, 0, (LONG) dlg_caption);
                }
                if (dlg_prompt != NULL) {
                        SetDlgItemText(hDlg, IDD_LABEL, dlg_prompt);
                }
                if (dlg_default) {
                        SetDlgItemText(hDlg, IDD_FILE, dlg_default);
                }
                return(TRUE);

        case WM_COMMAND:
                switch(GET_WM_COMMAND_ID(wParam, lParam)) {

                case IDCANCEL:
                        EndDialog(hDlg, FALSE);
                        return(TRUE);

                case IDOK:
                        GetDlgItemText(hDlg, IDD_FILE, dlg_result, dlg_size);
                        EndDialog(hDlg, TRUE);
                        return(TRUE);
                }
        }
        return (FALSE);
}

/***************************************************************************
 * Function: My_mbschr
 *
 * Purpose:
 *
 * DBCS version of strchr
 *
 */
unsigned char * _CRTAPI1 My_mbschr(
    unsigned char *psz, unsigned short uiSep)
{
    while (*psz != '\0' && *psz != uiSep) {
        psz = CharNext(psz);
    }
    return *psz == uiSep ? psz : NULL;
}
/***************************************************************************
 * Function: My_mbsncpy
 *
 * Purpose:
 *
 * DBCS version of strncpy
 *
 */
unsigned char * _CRTAPI1 My_mbsncpy(
	unsigned char *psz1, const unsigned char *psz2, size_t Length)
{
        int nLen = (int)Length;
	unsigned char *pszSv = psz1;

	while (0 < nLen) {
		if (*psz2 == '\0') {
			*psz1++ = '\0';
			nLen--;
		} else if (IsDBCSLeadByte(*psz2)) {
			if (nLen == 1) {
				*psz1 = '\0';
			} else {
				*psz1++ = *psz2++;
				*psz1++ = *psz2++;
			}
			nLen -= 2;
		} else {
			*psz1++ = *psz2++;
			nLen--;
		}
	}
	return pszSv;
}

/***************************************************************************
 * Function: LoadRcString
 *
 * Purpose: Loads a resource string from string table and returns a pointer
 *          to the string.
 *
 * Parameters: wID - resource string id
 *
 */
LPTSTR APIENTRY LoadRcString(UINT wID)
{
    static TCHAR szBuf[512];

    LoadString((HANDLE)GetModuleHandle(NULL),wID,szBuf,sizeof(szBuf));
    return szBuf;
}

LPTSTR APIENTRY LoadRcString2(UINT wID)
{
    static TCHAR szBuf[512];

    LoadString((HANDLE)GetModuleHandle(NULL),wID,szBuf,sizeof(szBuf));
    return szBuf;
}
