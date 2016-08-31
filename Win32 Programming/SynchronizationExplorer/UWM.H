//*****************************************************************************
//                                 UWM_STATE
// Notification of current state
//      wParam: State id, one of
//              STATE_LOCK
//              STATE_FETCH
//              STATE_INCREMENT
//              STATE_STORE
//              STATE_UNLOCK
//      lParam: depends on state:
//              STATE_LOCK: (BOOL)locking flag
//              STATE_FETCH: old value of n
//              STATE_INCREMENT: new value of n
//              STATE_STORE: unused, 0
//              STATE_UNLOCK: (BOOL)locking flag
//*****************************************************************************

#define UWM_STATE (WM_USER + 100)

//*****************************************************************************
//                                UWM_WAITING
//      wParam: unused, 0
//      lParam: unused, 0
//      Effect: The thread which sends this is immediately suspended and 
//              waits for the GUI to resume it; this is usually effected by
//              the "step" button or putting the system into free-run mode, 
//              at which point the GUI thread should ResumeThread the waiting
//              thread
//*****************************************************************************

#define UWM_WAITING (WM_USER + 101)

//*****************************************************************************
//                                UWM_RUNNING
//      wParam: unused, 0
//      lParam: unused, 0
//      Effect: A thread which resumes immediately sends this to notify
//              the GUI that it is now running.
//*****************************************************************************

#define UWM_RUNNING (WM_USER + 102)

//*****************************************************************************
//                                   UWM_N
//      wParam: current data value
//      lParam: unused, 0
//      Effect: Sent to to the panel to record the current value of the
//              counter
//*****************************************************************************

#define UWM_N (WM_USER + 103)

//*****************************************************************************
//                                 UWM_LOOP
//      wParam: unused, 0
//      lParam: unused, 0
//      Effect: Sent to the panel to increment the loop count
//*****************************************************************************

#define UWM_LOOP (WM_USER + 104)
