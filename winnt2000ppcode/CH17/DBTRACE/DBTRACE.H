/*--------------------------------------------------------------------
        DBTRACE.H
  --------------------------------------------------------------------*/

/*--------------------------------------------------------------------
        FUNCTION PROTOTYPES 
  --------------------------------------------------------------------*/

int WINAPI WinMain(HINSTANCE hinstThis, HINSTANCE hinstPrev,
  LPSTR lpszCmdLine, int iCmdShow);
BOOL Init(HINSTANCE hInst, int iCmdShow);
LRESULT WINAPI DBTrace_WndProc(HWND hWnd, UINT uMessage, WPARAM wParam,
  LPARAM lParam);
void DoLaunch(HWND hWnd);   
void DoCommand(HWND hWnd, WORD wCmd);
BOOL GetFileName(HWND hWnd, LPTSTR lpszFile, LPTSTR lpszFileTitle);
void Edit_Append(HWND hEditBox, LPCTSTR lpszString);
LRESULT DebuggingThread(LPTSTR lpszFileName);
void DoAbout(HWND hWnd);
void ShowErrorMsg(HWND hParent, int iLine);

/*--------------------------------------------------------------------
        RESOURCE IDs
  --------------------------------------------------------------------*/
#define IDI_APPICON       10
#define IDM_APPMENU       10  
#define CMD_FILELAUNCH    100
#define CMD_FILEDETACH    110
#define CMD_FILEEXIT      120
#define CMD_ABOUT         130
#define CMD_EDITCUT       200
#define CMD_EDITCOPY      210
#define CMD_EDITPASTE     220
#define CMD_EDITCLEAR     230
#define CMD_EDITCLEARALL  240
#define CMD_EDITSELECTALL 250


