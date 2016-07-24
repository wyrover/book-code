
#include <math.h>
//#include <commctrl.h>
#include <windows.h>
#include <windowsx.h>
#include <shlobj.h>
#include "voip.h"
#include "..\\build\\\res\\\resource.h"
#include "skin.h"

char      Name[] = "The \"VoipSample\"";
RECT      clientRect, cwindowRect;
RECT      windowRect, fullwindowRect;
COLORREF  backColor;
HINSTANCE hInstance;
HWND      hwnd, about, urlW = NULL;
HACCEL    hAccel;
Skin      *skin;
int       action  = ACTION_NONE;
DWORD     count = 0;
int       showing_cursor;
int       moveX = 0, moveY = 0;
int       compact_mode;
HMENU     popupMenu;

/**************************************************************************************
 *                                                                                    *
 *                           DlgProc for the About Dialog                             *
 *                        -----------------------------------                         *
 **************************************************************************************/

int APIENTRY AboutDlgProc (HWND hDlg, WORD wMsg, LONG wParam, LONG lParam)
{
  switch (wMsg)
    {
      case WM_INITDIALOG:
        return (0);
		break;

      case WM_SYSCOMMAND:
        if (wParam == SC_CLOSE)
          {
          EndDialog (hDlg, TRUE);
          return (TRUE);
          }
        break;

	  case WM_COMMAND:

        switch (wParam)
		{
		  case IDOK:
			
			  EndDialog(hDlg, TRUE);
			  return TRUE;
			  break;

		  default:
			  return 0;
		}
		break;
	}

  return FALSE;
}
/*
 * Needed definitions
 */

/*
 * The Main message loop functions
 *
 */

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void             UpdateMainWindow();
/**************************************************************************************
 *                                                                                    *
 *                                      QUIT                                          *
 *                                    --------                                        *
 **************************************************************************************/

void Quit() 
{
	PostQuitMessage(0);
}

//////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	POINT     pt,pt2;
    MSG       Msg;
    WNDCLASS  W;

	/*
	 * Keep a global instance
	 * for dialogs and ressources
	 *
	 */

	hInstance = hInst;
	/*
	 * Window size adjustment
	 *
	 */
	compact_mode = 0;
	windowRect.left   = 0;
	windowRect.right  = DEFAULT_SKIN_WIDTH;
	windowRect.top    = 0;
	windowRect.bottom = DEFAULT_SKIN_HEIGHT;

    AdjustWindowRect(&windowRect, WS_POPUP|WS_SIZEBOX, 0);

	/*
	 * Initialize the COM library
	 *
	 */

	CoInitialize(NULL);	

	/*
	 * Register the Window Class
	 * 
	 */

	memset(&W, 0, sizeof(WNDCLASS));
   
	W.style         = CS_HREDRAW | CS_VREDRAW | CS_PARENTDC;
	W.lpfnWndProc   = WndProc;
	W.hInstance     = hInst;
	W.hbrBackground = (HBRUSH)(0);
	W.hCursor       = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR1));
	W.hIcon         = LoadIcon(hInst, MAKEINTRESOURCE(IDB_ICON));
	W.lpszClassName = Name;
	W.lpszMenuName  = NULL;

	RegisterClass(&W);
	/*
    * Load the menu and change 
	* it for recent file list
	*
    */

	popupMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_POPUPMENU));


   /*
    * Create the main window
    *
    */

	hwnd = CreateWindow(Name, Name, WS_POPUP | WS_SIZEBOX, 
		                200, 100, 
						windowRect.right - windowRect.left, 
						windowRect.bottom - windowRect.top, 
						NULL, NULL, hInst, NULL);
   /*
    * Set the window Rgn
    *
    */

	GetClientRect(hwnd, &clientRect);
	GetWindowRect(hwnd, &windowRect);

	pt.x = clientRect.left;
	pt.y = clientRect.top;

	ClientToScreen(hwnd, &pt);

	pt2.x = clientRect.right;
	pt2.y = clientRect.bottom;

	ClientToScreen(hwnd, &pt2);

	SetWindowRgn(hwnd, CreateRectRgn( pt.x  - windowRect.left, 
		  							  pt.y  - windowRect.top,
									  (windowRect.right - windowRect.left) - (windowRect.right - pt2.x),
									  (windowRect.bottom - windowRect.top) - (windowRect.bottom - pt2.y)), TRUE); 

	/*
     * Register for Drag n' Drop
     */
 

	DragAcceptFiles(hwnd, TRUE);
	/*
	 * Load Accelerators
	 */

//	hAccel = LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_ACCELERATOR));	
 
	/*
	 * Set the icon
	 */

	SetClassLong(hwnd, GCL_HICON, (LONG) LoadIcon(hInst, MAKEINTRESOURCE(IDB_ICON))); 
    /*
    * Let the Show Begin
	*
    */
	skin = new Skin(hInst, hwnd);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
 
	GetWindowRect(hwnd, &windowRect);
	/*
	 * Message Loop
	 *
	 */

	while (TRUE)
    {

	   /*
		* Get the messages
		*/

	   if (!GetMessage(&Msg, NULL, 0, 0))
		return (int) Msg.wParam;

		TranslateMessage(&Msg);
	    DispatchMessage(&Msg);
	
   }

   return Msg.wParam;
}
/**************************************************************************************
 *                                                                                    *
 *                            WNDPROC (Main Event Func)                               *
 *                            -------------------------                               *
 **************************************************************************************
 *                                                                                    *
 *                                                                                    *
 *                                                                                    *
 **************************************************************************************/

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{ 
	signed short xPos, yPos;
	RECT         r;

	switch(Message) {

		/*
		 * Message sent by the playback engine
		 * when the playbacks en (go to next file)
		 *
		 */
	/*
	 * Key Press
	 *
	 */
	case WM_COMMAND	:
		RECT rect;
		switch(LOWORD(wParam)) 
		{
		case ID_MENU_ABOUT:
           DialogBox (hInstance, (LPCSTR)MAKEINTRESOURCE(IDD_ABOUTBOX),
                       hwnd, (DLGPROC)AboutDlgProc);
						
			break;
		case ID_MENU_EXIT:
			Quit();
			return TRUE;
			break;
		}
		break;
	case WM_KEYUP:
		break;

	case WM_MOUSEMOVE:
		count = 0;
		ShowCursor(1);
		showing_cursor = 1;

		if(action == ACTION_MOVING) {

			RECT   r;
			POINT pt;

			xPos = LOWORD(lParam); 
			yPos = HIWORD(lParam);

			GetClientRect(hwnd, &r);

			pt.x = xPos;
			pt.y = yPos;

			ClientToScreen(hwnd, &pt);

			MoveWindow( hwnd, pt.x - moveX, 
						pt.y - moveY, 
						windowRect.right - windowRect.left, 
						windowRect.bottom - windowRect.top, TRUE);
		
		
		}


		break;
	case WM_LBUTTONUP:

		switch(action) {

		case ACTION_MENU:
			HMENU menu;

			POINT pt;

			pt.x = pt.y = 0;
			ClientToScreen(hwnd, &pt);

			xPos = pt.x + LOWORD(lParam); 
			yPos = pt.y + HIWORD(lParam);

			menu = GetSubMenu(popupMenu, 0);

			TrackPopupMenu( menu, 0, xPos, yPos, 0, hwnd, NULL);
			break;
	

		case ACTION_CLOSE:

			Quit();
			break;

		case ACTION_MINIMIZE:

			ShowWindow(hwnd, SW_MINIMIZE);
			break;

		case ACTION_NONE:
		default:
			break;
		}

		if(action == ACTION_MOVING) {
			ReleaseCapture();
			RECT rect;

			GetWindowRect(hwnd, &rect);

			SetWindowPos(hwnd, HWND_TOPMOST, rect.left, rect.top, 
						 rect.right - rect.left, rect.bottom - rect.top, 
						 SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOACTIVATE);

			GetWindowRect(hwnd, &windowRect);

			}


		action = ACTION_NONE;

		break;

	case WM_LBUTTONDOWN:


			xPos = LOWORD(lParam); 
			yPos = HIWORD(lParam);

			if(compact_mode) 
			{

				action = ACTION_NONE;
			}
			else 
			{

			action = skin->GetAction(xPos, yPos);
			}

			if(action == ACTION_PLAY_AREA)
				action = ACTION_NONE;
	
	

			if(action == ACTION_NONE) 
			{

				/*
				 * We are moving the window
				 */

				action = ACTION_MOVING;

				moveX = xPos;
				moveY = yPos;

				GetWindowRect(hwnd, &windowRect);

				SetCapture(hwnd);
			}

		break;
	case WM_RBUTTONDOWN:

		HMENU menu;
		POINT pt1;
		int   x, y;

		pt1.x = pt1.y = 0;
		ClientToScreen(hwnd, &pt1);

		x = pt1.x + LOWORD(lParam); 
		y = pt1.y + HIWORD(lParam);

		menu = GetSubMenu(popupMenu, 0);

		TrackPopupMenu( menu, 0, x, y, 0, hwnd, NULL);
		break;

	case WM_PAINT:
		skin->UpdateSize(hwnd);
		skin->Display(hwnd);
		return 0;
		
		break;
	case WM_DESTROY:
		Quit();
		return TRUE;
		break;




	}
	  return DefWindowProc(hwnd, Message, wParam, lParam);

}