
/*
 * FILE.H
 */

/* handle to filedata */
typedef struct filedata FAR * FILEDATA;

FILEDATA file_new(DIRITEM fiName, BOOL bRead);
DIRITEM file_getdiritem(FILEDATA fi);
void file_delete(FILEDATA fi);
LIST file_getlinelist(FILEDATA fi);
void file_discardlines(FILEDATA fi);
void file_reset(FILEDATA fi);

