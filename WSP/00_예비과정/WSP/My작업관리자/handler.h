// handler.h

int OnCreate(HWND hwnd, WPARAM wParam, LPARAM lParam);

int OnCtrlColorEdit(HWND hwnd, WPARAM wParam, LPARAM lParam);

int OnCommand(HWND hwnd, WPARAM wParam, LPARAM lParam);

int OnSysComand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int OnTrayMessage(HWND hwnd, WPARAM wParam, LPARAM lParam);


int OnDestory();
