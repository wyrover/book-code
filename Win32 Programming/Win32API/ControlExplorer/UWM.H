//*****************************************************************************
//			     UWM_SIMULATE_WM_SIZE
// Same parameters as WM_SIZE
//	WPARAM: nType
//	LPARAM: MAKELONG(cx, cy);
// 	Result: void (0)

#define UWM_SIMULATE_WM_SIZE (WM_USER + 100)

//*****************************************************************************
//				 UWM_SETFOCUS
// 	WPARAM: Window to pass focus to
//	LPARAM: unused, 0
//	Result: void (0)
// Effect:
//	Causes focus to be set to the window whose handle is provided
//	This is so we can change the focus from within an OnSetFocus handler

#define UWM_SETFOCUS (WM_USER + 101)
