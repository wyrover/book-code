#include "stdafx.h"
#include "subclass.h"

#define CTLID_BASE 0 // in case we need to map the integers elsewhere
#define ATOM_PROC  0x103

/****************************************************************************
*                                  setEvent
* Inputs:
*       HWND hwnd: Window whose control ID is to be set
*       UINT CtlID: Control ID code to set
*       UINT event: Event code
* Result: void
*       
* Effect: 
*       Sets the Control ID property (see changeProc)
* Notes:
*       We add 1 to the event so that the values are 1..n instead of 0..n-1
*       We do this because no events are negative numbers, so if the
*       attribute is not present, we get back a 0.  0 is never a valid
*       return value under this scenario.
****************************************************************************/

static void setEvent(HWND hwnd, UINT CtlID, UINT event)
    {
     ::SetProp(hwnd, MAKEINTATOM(CTLID_BASE + CtlID), (HANDLE)(event + 1));
    }

/****************************************************************************
*                                  getEvent
* Inputs:
*       HWND hwnd: Window handle
*       UINT CtlID: Control ID
* Result: UINT
*       Event code.  If (UINT)-1 is returned there was no setting for
*       this control ID
****************************************************************************/

static UINT getEvent(HWND hwnd, UINT CtlID)
    {
     return (UINT)(((int)::GetProp(hwnd, 
                                       MAKEINTATOM(CTLID_BASE + CtlID))) - 1);
    }

/****************************************************************************
*                                  setProc
* Inputs:
*       HWND hwnd: Window whose parent procedure is to be set
*       WNDPROC Proc: Procedure pointer to parent
* Result: void
*       
* Effect: 
*       Sets the superclass procedure property (see changeProc)
****************************************************************************/

static void setProc(HWND hwnd, WNDPROC Proc)
    {
     ::SetProp(hwnd, MAKEINTATOM(ATOM_PROC), (HANDLE)Proc);
    }

/****************************************************************************
*                                  getProc
* Inputs:
*       HWND hwnd: Window handle
* Result: WNDPROC
*        Superclass procedure stored with window (0 if not explicitly set)
****************************************************************************/

static WNDPROC getProc(HWND hwnd)
    {
     return (WNDPROC)::GetProp(hwnd, MAKEINTATOM(ATOM_PROC));
    }


/****************************************************************************
*                                 changeProc
* Inputs:
*       HWND hwnd: Window handle
*       UINT msg: message ID
*       WPARAM wParam: 
*       LPARAM lParam:
* Result: LRESULT
*       The result of calling the superclass function
* Effect: 
*       Posts a change message to the parent
* Notes:
*       This horrific kludge is required in order to circumvent a fundamental
*       design flaw in MFC: that MFC cannot possibly be used to actually
*       subclass a common dialog.  The reason is that a common dialog subclass
*       function is supposed to return 0 if it did not handle the message
*       and non-zero if it does, and MFC is incapable of returning 0 for
*       a WM_COMMAND message; if it appears in the dispatch table at all,
*       it WILL return 1, making the common dialog think that the message
*       has been handled in its entirety.  This is not true; we want to post
*       the change message, but let the common dialog do its common dialog
*       thing.  
*
*       We could have used a GetMessage hook, but this imposes serious
*       performance penalties.  Therefore, we subclass the controls and
*       intercept the message here.
*
*       The subclassing consists of using properties to store critical
*       information.  The properties we store are:
*               ATOM_SUPERPROC: the address of the superclass function
*               MAKEINTATOM(CTLID_BASE + id): the event number for id
*
*       If the message that comes in matches the <CTLID, EVENT> pair then
*       we post a change message.
****************************************************************************/

static LRESULT CALLBACK changeProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {

     if(msg == WM_COMMAND)
        { /* WM_COMMAND */
         UINT event = getEvent(hwnd, LOWORD(wParam));
         
         if(event != (UINT)-1)
            if(HIWORD(wParam) == event)
              ::PostMessage(hwnd, UWM_UPDATE_FONTS, 0, 0);
        } /* WM_COMMAND */

     return ::CallWindowProc(getProc(hwnd), hwnd, msg, wParam, lParam);
    }

/****************************************************************************
*                               subclassCommand
* Inputs:
*       CWnd * ctl: Window to subclass
*       UINT id: ID of control we are looking for
*       UINT event: Event code we are looking for
* Result: void
*       
* Effect: 
*       Subclasses the control to the special changeProc handler
*       If the window has already been subclassed, just adds the 
*       control ID to the window.
****************************************************************************/

void subclassCommand(CWnd * ctl, UINT id, UINT event)
    {
     setEvent(ctl->m_hWnd, id, event);
     
     WNDPROC oldproc = (WNDPROC)::GetWindowLong(ctl->m_hWnd, GWL_WNDPROC);
     if(oldproc != changeProc)
        setProc(ctl->m_hWnd, (WNDPROC)::SetWindowLong(ctl->m_hWnd, GWL_WNDPROC, (LPARAM)changeProc));
    }

/****************************************************************************
*                              unsubclassControl
* Inputs:
*       HWND hctl: Window to unsubclass
*       UINT ctlId: ID to unsubclass
* Result: void
*       
* Effect: 
*       Changes the procedure pointer back to the original one and removes
*       all other properties
****************************************************************************/

void unsubclassCommand(CWnd * ctl, UINT ctlID)
    {
     WNDPROC p = getProc(ctl->m_hWnd);
     if(p != NULL)
        { /* restore */
         ::SetWindowLong(ctl->m_hWnd, GWL_WNDPROC, (LPARAM)p);
        } /* restore */
     ::RemoveProp(ctl->m_hWnd, MAKEINTATOM(CTLID_BASE + ctlID));
     ::RemoveProp(ctl->m_hWnd, MAKEINTATOM(ATOM_PROC));
    }
