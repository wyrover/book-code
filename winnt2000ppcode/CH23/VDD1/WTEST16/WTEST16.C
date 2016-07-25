// WTEST16.C - Source file for WTEST16.EXE, a VDD Test App
//

// Include Files
//  
#include "windows.h"
#include "wtest16.h"
#include "regvdd.h"
#include "isvbopx.h"

// Global variables
//

REGVDD_INFO regInfo;
REGVDD_INFO far *p = &regInfo;
int iSize = sizeof(REGVDD_INFO);            
WORD VDDHandle;
char szVDDName[128] = "REGVDD.DLL";
char szVDDInit[128] = "RegVDDRegisterInit";
char szVDDDisp[128] = "RegVDDDispatch";
	
HANDLE hInst;
BOOL bWinNT;
char szNotNT[] = "Sorry, only available on Windows NT!";
char szCap[] = "WTEST16";


/**-----------------------------------------------------**/
int PASCAL WinMain(HANDLE hInstance, HANDLE hPrevInstance,
   LPSTR lpCmdLine, int nCmdShow)
{
   MSG msg;

   // check GetWinFlags for a 16-bit app, NOT GetVersion!
   bWinNT = (BOOL)(GetWinFlags() & 0x4000);
   
   if (!hPrevInstance)
      if (!InitApplication(hInstance))
         return FALSE;
 
   if (!InitInstance(hInstance, nCmdShow))
      return FALSE;

   while (GetMessage(&msg, NULL, NULL, NULL))
   {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }
   return msg.wParam;
   
} // WinMain

/**-----------------------------------------------------**/
BOOL InitApplication(HANDLE hInstance)
{
   WNDCLASS wc;

   wc.style = NULL;
   wc.lpfnWndProc = MainWndProc;
   wc.cbClsExtra = 0;
   wc.cbWndExtra = 0;
   wc.hInstance = hInstance;
   wc.hCursor = LoadCursor(NULL, IDC_ARROW);
   wc.hbrBackground = GetStockObject(WHITE_BRUSH);
   wc.lpszMenuName =  "WTEST16_Menu";
   wc.lpszClassName = "WTEST16_WClass";

   return RegisterClass(&wc);
    
} // InitApplication

/**-----------------------------------------------------**/
BOOL InitInstance(HANDLE hInstance, int nCmdShow)
{
   HWND hWnd;
   
   hInst = hInstance;

   hWnd = CreateWindow("WTEST16_WClass",
      "WTEST16 VDD Test Application", WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
      CW_USEDEFAULT, NULL, NULL, hInstance, NULL);     

   if (!hWnd) return FALSE;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   return TRUE;

} // InitInstance

/**-----------------------------------------------------**/
long CALLBACK __export MainWndProc(HWND hWnd, UINT msg,
   WPARAM wParam, LPARAM lParam)
{
   switch (msg)
   {
      case WM_CREATE:
         // initialize REGVDD_INFO structure, etc.
         regInfo.dwCmd = REGVDD_CMD_CREATESUBKEY;
         regInfo.dwRootKey = 0x80000001;   //current user
         regInfo.dwVolatility = 0x00000001;
         regInfo.dwAccess = 0x000f003f;   //all access|write
         lstrcpy(regInfo.szSubkey, 
            (LPSTR)"Software\\Microsoft\\RegVDD");
         regInfo.dwValueType = 1;         //REG_SZ;
         lstrcpy(regInfo.szValueName,
            (LPSTR)"DefaultSetting");
         lstrcpy(regInfo.szValueData, (LPSTR)"Stuff");
         regInfo.dwValueDataSize = 
            lstrlen(regInfo.szValueData);
         regInfo.dwStatus = 0;
         
         if (bWinNT) {
            _asm  mov si, offset szVDDName
            _asm  mov di, offset szVDDInit
            _asm  mov bx, offset szVDDDisp
            _asm  RegisterModule
            _asm  mov VDDHandle, ax
         }
         break;

      case WM_COMMAND:
         switch(wParam) 
         {
            case IDM_CREATESUBKEY:
               regInfo.dwCmd = REGVDD_CMD_CREATESUBKEY;
               if (bWinNT) {
                  _asm  mov   ax, VDDHandle
                  _asm  les   bx, p        // regInfo
                  _asm  mov   cx, iSize    // sizeof regInfo
                  _asm  mov   dx, TRUE     // selector
                  _asm  DispatchCall
               } else MessageBox(hWnd, szNotNT, szCap, MB_OK);
               break;
             
            case IDM_DELETESUBKEY:
               regInfo.dwCmd = REGVDD_CMD_DELETESUBKEY;
               if (bWinNT) {
                  _asm  mov   ax, VDDHandle
                  _asm  les   bx, p        // regInfo
                  _asm  mov   cx, iSize    // sizeof regInfo
                  _asm  mov   dx, TRUE	   // selector
                  _asm  DispatchCall
               } else MessageBox(hWnd, szNotNT, szCap, MB_OK);
               break;
               
            case IDM_WRITEVALUE:
               regInfo.dwCmd = REGVDD_CMD_WRITEVALUE;
               if (bWinNT) {
                  _asm  mov   ax, VDDHandle
                  _asm  les   bx, p        // regInfo
                  _asm  mov   cx, iSize    // sizeof regInfo
                  _asm  mov   dx, TRUE     // selector
                  _asm  DispatchCall
               } else MessageBox(hWnd, szNotNT, szCap, MB_OK);
               break;
               
            case IDM_READVALUE:
               regInfo.dwCmd = REGVDD_CMD_READVALUE;
               if (bWinNT) {
                  _asm  mov   ax, VDDHandle
                  _asm  les   bx, p        // regInfo
                  _asm  mov   cx, iSize    // sizeof regInfo
                  _asm  mov   dx, TRUE	    // selector
                  _asm  DispatchCall
                  MessageBox(hWnd, regInfo.szValueData, 
                     szCap, MB_OK);
               } else MessageBox(hWnd, szNotNT, szCap, MB_OK);
               break;
               
            case IDM_DELETEVALUE:
               regInfo.dwCmd = REGVDD_CMD_DELETEVALUE;
               if (bWinNT) {
                  _asm  mov   ax, VDDHandle
                  _asm  les   bx, p        // regInfo
                  _asm  mov   cx, iSize    // sizeof regInfo
                  _asm  mov   dx, TRUE     // selector
                  _asm  DispatchCall
               } else MessageBox(hWnd, szNotNT, szCap, MB_OK);
               break;
            
            default:
               return DefWindowProc(hWnd,msg,wParam,lParam);
         }
         break;

      case WM_DESTROY:
         if (bWinNT) {
            _asm  mov ax, VDDHandle
            _asm  UnregisterModule
         }         
         PostQuitMessage(0);
         break;

      default:
         return DefWindowProc(hWnd, msg, wParam, lParam);
   }
   return NULL;

} // MainWndProc

// end of file
