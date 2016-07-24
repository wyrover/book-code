

/*
 * COMPITEM.H
 */

/* handle to a CompItem */
typedef struct compitem FAR* COMPITEM;


COMPITEM compitem_new(DIRITEM left, DIRITEM right, LIST list, BOOL fExact);
void compitem_delete(COMPITEM item);
LIST compitem_getcomposite(COMPITEM item);
void compitem_discardsections(COMPITEM item);
LIST compitem_getleftsections(COMPITEM item);
LIST compitem_getrightsections(COMPITEM item);
FILEDATA compitem_getleftfile(COMPITEM item);
FILEDATA compitem_getrightfile(COMPITEM item);
int compitem_getstate(COMPITEM item);
LPSTR compitem_gettext_tag(COMPITEM item);
LPSTR compitem_gettext_result(COMPITEM item);
LPSTR compitem_getfilename(COMPITEM item, int option);
void compitem_freefilename(COMPITEM item, int option, LPSTR filename);

/*
 * options for compitem_getfilename, indicating which name is desired
 */
#define CI_LEFT         1       /* name of left file */
#define CI_RIGHT        2       /* name of right file */
#define CI_COMP         3       /* name of composite file */



