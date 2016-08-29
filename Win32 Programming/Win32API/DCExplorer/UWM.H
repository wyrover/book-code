//*****************************************************************************
//			       UWM_UPDATE_ERROR
// WPARAM: error message code
// LPARAM: unused, 0
// Result: void
//
// Effect: Passed to parent to suggest that error message should be
//         updated.
#define UWM_UPDATE_ERROR (WM_USER + 1)

//*****************************************************************************
//				UWM_CHANGE_VEXT
// WPARAM: viewport x value
// LPARAM: viewport y value
// Result: void
//
// Effect: The viewport ext has changed because of an update; this forces
// 	   the new value to be set back in the control
#define UWM_CHANGE_VEXT (WM_USER + 2)

//*****************************************************************************
//				UWM_CHANGE_WEXT
// WPARAM: window x value
// LPARAM: window y value
// Result: void
//
// Effect: The window ext has changed because of an update; this forces
// 	   the new value to be set back in the control
#define UWM_CHANGE_WEXT (WM_USER + 2)

//*****************************************************************************
//			       UWM_COLOR_CHANGE
//
// WPARAM: COLORREF of color value
// LPARAM: unused, 0
// Result: void
// Effect: This notifies the parent that the mouse has selected a new
//         color
#define UWM_COLOR_CHANGE (WM_USER + 3)

//*****************************************************************************
//				UWM_COLOR_CLEAR
//
// WPARAM: unused, 0
// LPARAM: unused, 0
// Result: void
// Effect: This notifies the parent that the mouse is no longer over a color
#define UWM_COLOR_CLEAR (WM_USER + 4)
