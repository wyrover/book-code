// fun.h


int OnTrayIcon(HWND hwnd);

void TrayMessage(HWND hwnd, WPARAM wParam, LPARAM lParam);

//--- 컨트롤 초기화 
void CreateChildControl(HWND hwnd);


//---- 프로세스 목록 출력
void ProcessList();
void ListBoxAddData(int i, PROCESSENTRY32 ppe);

//---- 프로세스 종료 
void ProcessExit(HWND hwnd);


//---- 프로세스 감시
void WatchProcess(HWND hwnd);
DWORD WINAPI Watch( void *p);		// Thread Fun


//---- 프로세스 생성 
void NewProcess(HWND hwnd);
