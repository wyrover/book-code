/**-------------------------------------------------------**
** HPSCANAP.H:	Defines for the HP-specific scanner options.
** Environment: Windows NT.
** (C) Hewlett-Packard Company 1993.  PLT.
**-------------------------------------------------------**/

/**------------------ CONFIGURE_DLG ---------------------**/
#define CONFIGURE_DLG          3000
#define ID_CONFIG_SCANNER      3001
#define ID_CONFIG_DRV          3002
#define ID_CONFIG_TEST         3003
#define ID_CONFIG_CAL          3004
#define ID_CONFIG_HELP         3005

/**------------------- Prototypes -----------------------**/
INT APIENTRY LibMain(HANDLE, ULONG, LPVOID);
BOOL APIENTRY ConfigureDlgProc(HWND, UINT, WPARAM, LPARAM);
