// WTEST.C - Sample program to exerise the WVerion routine

#include "windows.h"
#include "ver.h"
#include "wtest.h"
#include "wversion.h"

int PASCAL WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
long CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE hInst;

/**------------------------------------------------**/
#ifdef __BORLANDC__
    #pragma argsused
#endif

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst,
   LPSTR lpCmdLine, int nCmdShow)
{
   WNDCLASS wc;
   MSG      msg;
   HWND     hWnd;

   hInst = hInstance;

   wc.style = 0;
   wc.lpfnWndProc = MainWndProc;
   wc.cbClsExtra = 0;   
   wc.cbWndExtra = 0;  
   wc.hInstance = hInst;    
   wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
   wc.hCursor = LoadCursor(NULL, IDC_ARROW);
   wc.hbrBackground = GetStockObject(WHITE_BRUSH);
   wc.lpszMenuName =  "WTEST_Menu";   
   wc.lpszClassName = "WTEST_Class"; 

   if (!RegisterClass(&wc)) return FALSE;

   if ((hWnd = CreateWindow("WTEST_Class", 
      "WVersion Test App", WS_OVERLAPPEDWINDOW, 
      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
      CW_USEDEFAULT, NULL, NULL, hInst, NULL)) == NULL)
         return FALSE;

   ShowWindow(hWnd, nCmdShow);  
   UpdateWindow(hWnd); 

   while (GetMessage(&msg, NULL, 0, 0)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }
   return msg.wParam;
} // WinMain


/**------------------------------------------------**/
long CALLBACK MainWndProc(HWND hWnd, UINT msg, 
   WPARAM wParam, LPARAM lParam)
{
   WORD msgID;
   DWORD dwEnviron, dwVersion;
   char szString[128], szEnviron[128];

   #if defined WIN32 || defined _WIN32
      msgID = LOWORD(wParam);
   #else
      msgID = wParam;
   #endif
   
   switch (msg) {
      case WM_COMMAND:
         switch (msgID) {
            case IDM_VERSION:
               WVersion(&dwEnviron, &dwVersion);
               LoadString(hInst, (UINT)dwEnviron, 
                  szEnviron, 128);
               wsprintf((LPSTR)szString, 
                  (LPSTR)"%s, Version %d.%d", 
                  (LPSTR)szEnviron, 
                  LOBYTE(LOWORD(dwVersion)),
                  HIBYTE(LOWORD(dwVersion)));   
               MessageBox(hWnd, (LPSTR)szString, 
                  (LPSTR)"WVersion", MB_OK);
               break;

            default:
               return DefWindowProc(hWnd, msg, 
                  wParam, lParam);
          }
          break;

      case WM_DESTROY:
         PostQuitMessage(0);
         break;

      default:
         return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
} // MainWndProc
