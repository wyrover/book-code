/**-------------------------------------------------------**
** SCANAPLT.H: Defines and Prototypes for SCANAPLT.CPL
** Environment: Windows NT.
** (C) Hewlett-Packard Company 1993.  PLT.
**-------------------------------------------------------**/

/**---------------- General Defines ---------------------**/
#define IDNULL                  -1
#define MAX_STR_LEN             128
#define NUM_SCANNER_APPS        1
#define ICO_SCANNER             999

/**-------------- Vendor Capability Flags ---------------**/
#define CPL_SCANNER_SUPPORT_CONFIGURE   0x01
#define CPL_SCANNER_SUPPORT_INSTALL     0x02
#define CPL_SCANNER_SUPPORT_UNINSTALL   0x04

/**------------------ SCANNER_DLG -----------------------**/
#define SCANNER_DLG             4000
#define ID_SCANNER_LIST         4001
#define ID_SCANNER_ADD          4002
#define ID_SCANNER_REMOVE       4003
#define ID_SCANNER_CONFIGURE    4004
#define ID_SCANNER_HELP         4005

/**-------------------- ADD_DLG -------------------------**/
#define ADD_DLG                 4100
#define ID_EDIT_DRIVESRC        4101
#define ID_ADD_HELP             4102

/**-------------- Prototypes for SCANAPLT.C -------------**/
LONG CALLBACK CPlApplet(HWND, UINT, LPARAM, LPARAM);
BOOL APIENTRY ScannerDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL APIENTRY AddDlgProc(HWND, UINT, WPARAM, LPARAM);

/**------ Entry-point for Scanner Specific Options ------**/
ULONG APIENTRY CPL_ScannerConfigure(HWND, ULONG, LPTSTR);
ULONG APIENTRY CPL_ScannerInstall(HWND, ULONG, LPTSTR);
ULONG APIENTRY CPL_ScannerUninstall(HWND, ULONG, LPTSTR);
