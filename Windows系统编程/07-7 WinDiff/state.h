

/*
 * STATE.H
 *
 * Definition of the results of comparisons for files and for lines
 * within files.
 *
 * These need to be globally declared so that the UI code in windiff.c can
 * map states to the colour scheme (to correctly highlight changed lines).
 *
 * They apply to files (compitem_getstate() ) and to sections in the
 * composite list (section_getstate). All lines within a section have the
 * same state. The UI code will use the view_getstate() function to find the
 * state for a given line on the screen.
 *
 */

/* Applies to both lines or files: they are the same */
#define STATE_SAME              1

/* Applies to files.  Same size, date, time */
#define STATE_COMPARABLE        2

/* Applies to files.  Different, but only in blanks
 * This state only turns up after the file has been expanded.
 */
#define STATE_SIMILAR           3

/* Applies only to files */

/* - Files differ (and can be expanded) */
#define STATE_DIFFER            4

/* They are only in the left or right tree */
#define STATE_FILELEFTONLY      5
#define STATE_FILERIGHTONLY     6


/* Applies to lines only */

/* the line only exists in one of the lists */
#define STATE_LEFTONLY          7       /* line only in left file */
#define STATE_RIGHTONLY         8       /* line only in right file */


/* The line is the same in both files, but in
 * different places (thus the line will appear twice in the composite list,
 * once with each of these two states
 */
#define STATE_MOVEDLEFT         9       /* this is the left file version */
#define STATE_MOVEDRIGHT        10      /* this is the right file version*/

/* In processing the sections to build the composite list, we need to
 * track which sections have been processed.  After this the left and
 * right lists of sections are of no further interest
 */
#define STATE_MARKED            99
