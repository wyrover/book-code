#define UWM_WINEYESHOOK          (WM_USER+1234)
__declspec(dllexport) BOOL WINAPI setWineyesHook (HWND hwnd) ;
__declspec(dllexport) BOOL WINAPI unsetWineyesHook (HWND hwnd) ;
