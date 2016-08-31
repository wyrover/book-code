void logPageSetupDlg(HWND hdlg, WPARAM wParam, LPARAM lParam);
void logPageInit();
extern int UWM_LOG_PAINT;

UINT APIENTRY pagePaint(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam);
UINT APIENTRY pageSetup(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam);
