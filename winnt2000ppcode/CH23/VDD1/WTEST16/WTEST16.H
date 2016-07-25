// WTEST16.H - Include file for WTEST16.EXE

#define IDM_CREATESUBKEY	100
#define IDM_DELETESUBKEY    101
#define IDM_WRITEVALUE      102
#define IDM_READVALUE       103
#define IDM_DELETEVALUE     104

int PASCAL WinMain(HANDLE, HANDLE, LPSTR, int);
BOOL InitApplication(HANDLE);
BOOL InitInstance(HANDLE, int);
long CALLBACK __export MainWndProc(HWND, UINT, WPARAM, LPARAM);

// end of file
