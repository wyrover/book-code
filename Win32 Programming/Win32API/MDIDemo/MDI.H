#define IDM_FIRSTDOCUMENT 3000  // special IDs for documents start here
#define IDM_LASTDOCUMENT  3999  // no more than 1000 windows
//=============================================================================
// UMW_MDI_DESTROY
//	WPARAM: unused
//	LPARAM: unused
//
// Sent to main frame to notify it that an MDI child window has been destroyed
// so that it can determine which menu to post in case it was the last MDI
// child
#define UWM_MDI_DESTROY    (WM_USER + 100) 

//=============================================================================
// UWM_UPDATE_TOOLBAR
//	WPARAM: unused
//	LPARAM: unused
//
// Sent to the main frame window to indicate that it should update the
// tool bar icons
#define UWM_UPDATE_TOOLBAR (WM_USER + 101)

//=============================================================================
// UWM_UPDATE_MENU
//	WPARAM: unused
//	LPARAM: unused
//
// Sent to an MDI child to indicate that there has been a change in status
// that requires updating the menu (not relevant, since this is usually 
// handled in the OnInitMenuPopup handler) and the toolbar (which requires
// the menu be up-to-date).
#define UWM_UPDATE_MENU    (WM_USER + 102) 

//=============================================================================
// UWM_SET_ACCELERATOR
//	WPARAM: unused
//	LPARAM: handle of new accelerator table to use
// Result: HACCEL
//	Handle of previous accelerator table
// Sent to the main frame to change the accelerator table being used
#define UWM_SET_ACCELERATOR (WM_USER + 103)

#define HANDLE_UWM_SET_ACCELERATOR(hwnd, wParam, lParam, fn) \
   ((LRESULT)(fn)(hwnd, (HACCEL)lParam))
	
//=============================================================================
// UWM_CONTEXTMENU
//	WPARAM: handle of control window
//	LPARAM: LOWORD x-position
//		HIWORD y-position
//
// This is sent to the MDI child window, but is handled as a separate
// message.  Otherwise the DefMDIChildProc will route it to the main frame
// window, which, because of our dispatch, gives us an infinite recursion if
// the child window is not prepared to deal with it.  To give us robustness,
// we define a separate message which will be ignored if the child routes it
// back to the parent.
//
#define UWM_CONTEXTMENU (WM_USER + 104)

#define FORWARD_UWM_CONTEXTMENU(hwnd, hwndCtl, xPos, yPos, fn) \
    (BOOL)(DWORD)(fn)((hwnd), UWM_CONTEXTMENU, (WPARAM)(HWND)hwndCtl, MAKELPARAM((xPos), (yPos)))

BOOL 
mdi_OnContextMenu(HWND hwnd, HWND hwndCtl, int xPos, int yPos, HMENU context);
void mdi_ConformMenus(HMENU hMaster, HMENU hPopup);
HWND mdi_Create(HWND hwnd, DWORD styles, int classid, int titleid);

extern HWND MDIClientWnd;
