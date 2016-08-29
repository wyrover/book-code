/**************************************/
/* Public section of the header file. */
/**************************************/
// These definitions provide the external interface 
// to the ScrollingWindow class.                  
//****************************************************
// SM_GETMARGINS
// Inputs;
//     wParam: unused, 0
//     lParam:: unused, 0
// Result:
//     LOWORD: Left margin, in pixels
//     HIWORD: Top margin, in pixels
#define SM_GETMARGINS           (WM_USER+1)

//****************************************************
// SM_GETDOCSIZE
// Inputs;
//     wParam: unused, 0
//     lParam:: unused, 0
// Result:
//     LOWORD: Number of horizontal pixels in document
//     HIWORD: Number of lines in document (not pixels)
#define SM_GETDOCSIZE           (WM_USER+2)

//****************************************************
// SM_SETDOCSIZE
// Inputs:
//	wParam: Width, in pixels
//	lParam: Height, in lines
// Result: 
//	0, always
// Effect:
//	Stores the current document size, in pixels
//	which is returned by SM_GETDOCSIZE
#define SM_SETDOCSIZE		(WM_USER+3)

//****************************************************
// SM_GETSCROLLPOS
// Inputs;
//     wParam: unused, 0
//     lParam:: unused, 0
// Result:
//     LOWORD: Horizontal scrolling position, in pixels
//     HIWORD: Vertical scrolling position, in lines
#define SM_GETSCROLLPOS         (WM_USER+4)


LRESULT CALLBACK 
ScrollingWndProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND createChildWindow(HINSTANCE hinst, HWND hwnd, UINT classid, UINT titleid);
BOOL registerChildClass(HINSTANCE hinst, UINT classid, UINT icon, WNDPROC proc);


/***************************************/
/* Private section of the header file. */
/***************************************/
/* These definitions are used only by the ScrollingWindow class. */
/* These macros encapsulate the details of window extra area accesses       */
/* making it much easier to later rearrange the fields, if necessary.       */
/* They also provide the proper type casts for the data involved.           */
/* Window extra area field(s) */
#define UGWL_DOCSIZE                 0
#define UGW_SWMAXUSED                (UGWL_DOCSIZE + sizeof(LONG))

#define getDocSize(w)      (GetWindowLong (w, UGWL_DOCSIZE))
#define setDocSize(wnd, w, h) (SetWindowLong (wnd, UGWL_DOCSIZE, MAKELONG(w,h)))
