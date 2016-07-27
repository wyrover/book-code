//ui.h

void ui_init(HWND hDlg);

void ui_EnableButton(HWND hDlg, BOOL flogin, BOOL fsend);

void ui_getname(HWND);
int ui_getgroup(HWND);
void ui_SetName(HWND);
void ui_SetGroup(HWND);

void ui_getmessage(HWND);
void ui_Setmessage(HWND);
void ui_SetGroupsend(HWND);
int ui_getgroupsend(HWND hDlg);

void ui_sendthread();
DWORD WINAPI ThreadFunc(LPVOID);

void myrefun(PACKET*,int,SOCKET);
void DisplayText(char *fmt, ...);

void server_run();