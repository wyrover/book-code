//*-------------------------------------------------------------------------
//| Title:
//|    MSGCOLOR.C
//|
//| Purpose:
//|      Implements ColorMessageBox which allows the backgorund and text
//|      color of a message box to be changed
//|                                                         
//| Development :  VC, 5/27/93
//|
//| Written by Microsoft Product Support Services, Windows Developer Support
//|
//| CopyRight (c) 1992 Microsoft Corporation. All rights reserved.
//|
//| Notes:
//|    This was adapted from the Microsoft Win16 example, which required
//|    substantial rewrite to make work in Win32.
//*-------------------------------------------------------------------------

#include <windows.h>
#include <windowsx.h>
#include "msgcolor.h"
#include "error.h"

static LRESULT CALLBACK cbtProc(int nCode, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK msgBoxSubClassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

typedef struct {
                HWND hwndMsgBox;
                FARPROC lpMsgBoxSubClassProc;
                BOOL SubClass;

                HHOOK     hhookCBT;     // CBT hook identifier
                HBRUSH    hbrushBkgnd;  // Brush to paint message box background
                COLORREF  clrText;      // Color of message box text
                FARPROC   MsgBoxProc;   // Message box window procedure
                HINSTANCE hinst;        // HINSTANCE of module
               } CBTvalues;

// Globals

_declspec(thread) CBTvalues cbt;

//****************************************************************************
// Function: colorMessageBox
//
// Purpose: Creates a message box with a specified background and text color.
//
// Parameters:
//    hwndOwner    == Owner of message box. Can be NULL.
//    Text         == Text in message box.
//    Title        == Title of message box.
//    uFlags       == MessageBox flags.
// The above four parameters are standard MessageBox parameters. See the
// MessageBox documentation for more details.
//    hbrushBkgnd  == Brush to paint the background of message box.
//    clrText      == Text color of text in message box.
//    hinst        == Instance of module associated with the message box.
//
// Returns: return value of MessageBox
//
// Comments: The standard message box is subclassed to change the color of the
//    the text and background. To obtain the handle of the message box inorder
//    to subclass, a CBT hook is installed before MessageBox is called. The
//    CBT hook is called when the message box is created and provides access to
//    the window handle of the message box. 
//
//****************************************************************************

int colorMessageBox(HWND hwndOwner, LPCSTR Text, LPCSTR Title, UINT uFlags, 
                    HBRUSH hbrushBkgnd, COLORREF clrText, HINSTANCE hinst)
{
    int nResult;
    
    cbt.hbrushBkgnd = hbrushBkgnd;
    cbt.hinst = hinst;
    cbt.clrText = clrText;
    
    // Set a task specific CBT hook before calling MessageBox. The CBT 
    //    hook will be called when the message box is created and will 
    //    give us access to the window handle of the MessageBox. The 
    //    message box can then be subclassed in the CBT hook to change 
    //    the color of the text and background. Remove the hook after
    //    the MessageBox is destroyed.

    cbt.hhookCBT = SetWindowsHookEx(WH_CBT, cbtProc, hinst, GetCurrentThreadId());

    nResult = MessageBox(hwndOwner, Text, Title, uFlags);

    UnhookWindowsHookEx(cbt.hhookCBT);
    
    return nResult;
}

//****************************************************************************
// Function: cbtProc
//
// Purpose: Callback function of WH_CBT hook
//
// Parameters and return value:
//    See documentation for cbtProc. 
//
// Comments: The message box is subclassed on creation and the original
//    window procedure is restored on destruction
//
//****************************************************************************

static LRESULT CALLBACK 
cbtProc(int nCode, WPARAM wParam, LPARAM lParam)
{
   LPCBT_CREATEWND lpcbtcreate;
   
   if (nCode < 0)
       return CallNextHookEx(cbt.hhookCBT, nCode, wParam, lParam); 
   
   // Window owned by our task is being created. Since the hook is installed just 
   //   before the MessageBox call and removed after the MessageBox call, the window
   //   being created is either the message box or one of its controls. 
   if (nCode == HCBT_CREATEWND)     
   {
       lpcbtcreate = (LPCBT_CREATEWND)lParam;
       
       // Check if the window being created is a message box. The class name of
       //   a message box is WC_DIALOG since message boxes are just special dialogs.
       //   We can't subclass the message box right away because the window 
       //   procedure of the message box is not set when this hook is called. So
       //   we wait till the hook is called again when one of the message box 
       //   controls are created and then we subclass. This will happen because
       //   the message box has at least one control.

       if (WC_DIALOG == lpcbtcreate->lpcs->lpszClass) 
       {
           cbt.hwndMsgBox = (HWND)wParam;
           cbt.SubClass = TRUE;      // Remember to subclass when the hook is called next
       }
       else if (cbt.SubClass)
       {
           // Subclass the dialog to change the color of the background and text
           cbt.MsgBoxProc = (FARPROC)SetWindowLong(cbt.hwndMsgBox, GWL_WNDPROC, 
                                                  (LONG)msgBoxSubClassProc);
           cbt.SubClass = FALSE;
       }
   }
   else if (nCode == HCBT_DESTROYWND && (HWND)wParam == cbt.hwndMsgBox)
   {
       // Reset the original window procedure when the message box is about to 
       //   be destroyed.
       SetWindowLong(cbt.hwndMsgBox, GWL_WNDPROC, (LONG)cbt.MsgBoxProc);
       cbt.hwndMsgBox = NULL;      
   }   
   return 0;          
}

//****************************************************************************
// Function: MsgBoxSubClassProc
//
// Purpose: Subclass procedure for message box to change text and background color
//
// Parameters & return value:
//    Standard. See documentaion for WindowProc
//
//****************************************************************************
static LRESULT CALLBACK 
msgBoxSubClassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
     // Change the background and text color of the message box.

     switch(msg)
        { /* msg */
         case WM_CTLCOLORDLG:
                 SetBkMode((HDC)wParam, TRANSPARENT);
                 SetTextColor((HDC)wParam, cbt.clrText);
                 return (LRESULT)cbt.hbrushBkgnd;
         case WM_CTLCOLORSTATIC:
                 SetBkMode((HDC)wParam, TRANSPARENT);
                 SetTextColor((HDC)wParam, cbt.clrText);
                 return (LRESULT)GetStockObject(NULL_BRUSH);
        } /* msg */
     return  CallWindowProc(cbt.MsgBoxProc, hwnd, msg, wParam, lParam);
} 
