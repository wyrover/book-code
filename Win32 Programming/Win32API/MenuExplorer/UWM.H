// NOTES: we choose these values as 200-based to avoid conflicts with such 
// messages as DM_REPOSITION (which produces really bizarre results!)
#define UWM_BASE 200
///////////////////////////////////////////////////////////////////////////////
//				  UWM_COMMAND
//
//	wParam: LOWORD => control ID
//		HIWORD => notification code
//	lParam: 
//	Result: void (0)
//	Effect: This is used to reflect the WM_COMMAND message for logging
//		purposes

#define UWM_COMMAND       (WM_USER + UWM_BASE + 7)

///////////////////////////////////////////////////////////////////////////////
// The following messages have the same parameters as their WM_ equivalents
// but represent reflections of the message for logging purposes
#define UWM_INITMENUPOPUP (WM_USER + UWM_BASE  + 1)
#define UWM_INITMENU      (WM_USER + UWM_BASE  + 2)
#define UWM_MENUSELECT    (WM_USER + UWM_BASE  + 3)
#define UWM_MENUCHAR	  (WM_USER + UWM_BASE  + 4)
#define UWM_ENTERMENULOOP (WM_USER + UWM_BASE  + 5)
#define UWM_EXITMENULOOP  (WM_USER + UWM_BASE  + 6)

///////////////////////////////////////////////////////////////////////////////
//			      UWM_TRACKPOPUPMENU
//	WPARAM wParam: flags to use for the TrackPopupMenu
//	LPARAM lParam: MAKELONG(point.x, point.y)
//	Result: void (0)
//	Effect: Causes the recipient to issue a TrackPopupMenu call
//
#define UWM_TRACKPOPUPMENU (WM_USER + UWM_BASE + 20)

///////////////////////////////////////////////////////////////////////////////
//				  UWM_LOGMSG
//	WPARAM: unused, 0
//	LPARAM: (LPARAM)(UWM_LOGMSG_DATA *)
//	Result: void (0)
//
//	Effect: This message is handled by all child windows that do logging
//		It is normally sent to the parent property sheet which
//		sends it to all child windows; most will ignore it.
//
#define UWM_LOGMSG	   (WM_USER + UWM_BASE + 21)

typedef struct {
		HANDLE handle;
		CString type;
		CString msg;
	       } UWM_LOGMSG_DATA;

///////////////////////////////////////////////////////////////////////////////
//				  UWM_CHANGE
//	WPARAM: ignored
//	LPARAM: (LPARAM)hwnd: Sending window
//	Result: void (0)
//	Effect: This message is sent to the property sheet by any window
//		that changes the menu structurally.  The property sheet
//		sends it to all the child windows.  Normally this is handled
//		by having the menu set a flag and forcing the menu display to
//		reload in the OnSetActive handler.  The lParam is used so
//		the sending window can detect that it has sent the message
//		and does not necessarily need to set its changed flag (since
//		it probably has updated its own display)

#define UWM_CHANGE	   (WM_USER + UWM_BASE + 22)

///////////////////////////////////////////////////////////////////////////////
//			       UWM_LOGTRACKPOPUP
//	WPARAM: ignored
//	LPARAM: (LPARAM)(LPCTSTR)
//	Result: void (0)
//	Effect: This message is handled by the trackpopup menu page to give
//		a display of the track popup command.
#define UWM_LOGTRACKPOPUP  (WM_USER + UWM_BASE + 23)

///////////////////////////////////////////////////////////////////////////////
//				 UWM_SETFLAGS
//	WPARAM flags: Flag values
//	LPARAM: ignored
//	Result: void (0)
//	Effect: Sets the flag values

#define UWM_SETFLAGS       (WM_USER + UWM_BASE + 24)

///////////////////////////////////////////////////////////////////////////////
//			       UWM_ADD_OWNERDRAW
//	WPARAM: (WPARAM)(menuinfo *)
//	LPARAM: ignored
//	Result: void (0)
//	Effect: Any window that processes this message makes a copy of the
//		menuinfo object and adds that object to its dropdown list

#define UWM_ADD_OWNERDRAW (WM_USER + UWM_BASE + 25)

///////////////////////////////////////////////////////////////////////////////
//			     UWM_DELETE_OWNERDRAW
//	WPARAM: uid of object to be deleted
//	LPARAM: ignored
//	Result: void (0)
//	Effect: Any window that processes this message should remove from
//		its dropdown any menuinfo object whose uid matches
//

#define UWM_DELETE_OWNERDRAW (WM_USER + UWM_BASE + 26)
