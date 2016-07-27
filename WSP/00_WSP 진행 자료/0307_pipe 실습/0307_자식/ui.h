//ui.h

void ui_init(HWND hDlg);
void ui_EnableButton(HWND hDlg, BOOL flogin, BOOL fsend);

void ui_GetName(char *name);
int ui_GetGroup();
void ui_SetName(char* name);	
void ui_SetGroup(int group);

int ui_GetSendGroup();
void ui_GetSendData(char *data);
void ui_SetSendData(char* data);

void ui_DisplayText(char *fmt, ...);
