//handler.h

DWORD OnInitDialog(HWND hDlg, WPARAM wParam, LPARAM lParam);
DWORD OnCommand(HWND hDlg, WPARAM wParam, LPARAM lParam);

void OnLogOut(HWND hDlg);
void OnExit(HWND hDlg);
void OnLogin(HWND hDlg);
void OnSend(HWND hDlg);
void OnRecv(char* data, int length);
