/**************************************************************************\
* install.h -- header file supporting nls table installing.
*
* Kept separate because install.* may later be used independently as
*  a control panel applet.
\**************************************************************************/


/**************************************************************************\
*  Function prototypes, window procedures first.
\**************************************************************************/

LRESULT CALLBACK InstallTableProc (HWND, UINT, WPARAM, LPARAM);

int InstallFile (TCHAR*,TCHAR*);
int ListInstalledTables (HWND, UINT, int);
int GetTableFileNames (HWND);
int myScanf (TCHAR*, int*);



/**************************************************************************\
*  Defined constants.
\**************************************************************************/

/* IDs from the Install dialog box. */
#define DID_LISTBOX  1000
#define BID_ADD      1001
