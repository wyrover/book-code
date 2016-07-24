// 
// EnumProt.c - Display WSAPROTOCOL_INFO information
//			    for all available protocols


#define ID_TREEVIEW     1000

// Internal functions
BOOL InitApplication(HANDLE hInstance);
BOOL InitInstance(HANDLE hInstance, int nCmdShow);
HWND CreateTreeControl(HWND hWndParent);
void AddAllProtocols(HWND hWndTree);
HTREEITEM AddTreeItem(HWND hWndTree, HTREEITEM hParent, LPSTR lpszFormat, ...);
LONG APIENTRY MainWndProc(HWND hWnd, UINT message, UINT wParam, LONG lParam);
BOOL APIENTRY About(HWND hDlg, UINT message, UINT wParam, LONG lParam);
void ShowWinsockError(int);
LPCTSTR GUIDtoString(GUID *pGUID);
LPCTSTR AFtoSTR(int iAddressFamily);
LPCTSTR PROTOtoSTR(int iProtocol);

