#include "StdSDK.h"     // Standard application includes
#include <tchar.h>
#include <windowsx.h>

#include "mdi.h"
#include "edit.h"
#include "resource.h"
#include "error.h"

#define setProc(hwnd, proc) SetWindowLong(hwnd, GWL_USERDATA, (LPARAM)proc)
#define getProc(hwnd)       (WNDPROC)GetWindowLong(hwnd, GWL_USERDATA)

#define getRGB(hwnd)        (COLORREF)GetProp(hwnd, MAKEINTATOM(ATOM_TEXTCOLOR))
#define setRGB(hwnd, rgb)   SetProp(hwnd, MAKEINTATOM(ATOM_TEXTCOLOR), (HANDLE)rgb)

//=============================================================================
// These atoms are used to attach properties to the edit window
#define ATOM_LASTSEL   0x100 // good random number
#define ATOM_BKG       0x101 // different random number
#define ATOM_TEXTCOLOR 0x102

/****************************************************************************
*			    editsubclass_OnCtlColor
* Inputs:
*       HWND hwnd: Window handle
*	HDC hdc: DC to use
*	HWND hchild: Child window handle (same as hwnd)
*	int type: Control color type
* Result: HBRUSH
*       Brush handle to use, or NULL
* Effect: 
*       Sets the background brush, text color, and background color
****************************************************************************/

static HBRUSH 
editsubclass_OnCtlColor(HWND hwnd, HDC hdc, HWND hchild, int type)
    {
     HBRUSH EditBackgroundBrush = GetProp(hwnd, MAKEINTATOM(ATOM_BKG));
     LOGBRUSH lbr;

     if(type != CTLCOLOR_EDIT)
        return NULL;

     if(EditBackgroundBrush != NULL)
        { /* get color */
	 GetObject(EditBackgroundBrush, sizeof(lbr), &lbr);
	} /* get color */

     SetTextColor(hdc, getRGB(hwnd));
     SetBkColor(hdc, lbr.lbColor);
     return EditBackgroundBrush;
     
    }

/****************************************************************************
*                            editsubclass_OnSetSel
* Inputs:
*       HWND hwnd: Window handle of edit control
*       int message: MEssage ID
*       WPARAM wParam:
*       LPARAM lParam:
* Result: LRESULT
*       The value of executing EM_SETSEL, 
* Effect: 
*       If the selection changed, update the menus
****************************************************************************/

static LRESULT 
editsubclass_OnSetSel(HWND hwnd, int message, WPARAM wParam, LPARAM lParam)
    {
     LRESULT result;

     PostMessage(GetParent(hwnd), UWM_UPDATE_MENU, 0, 0);
     result = CallWindowProc(getProc(hwnd), hwnd, message, wParam, lParam);
     SetProp(hwnd, MAKEINTATOM(ATOM_LASTSEL), 
                             (HANDLE)Edit_GetSel(hwnd));
     return result;
    }

/****************************************************************************
*                         editsubclass_CheckSelChange
* Inputs:
*       HWND hwnd: Edit control handle
* Result: void
*       
* Effect: 
*       Checks to see if the selection changed
****************************************************************************/

static void 
editsubclass_CheckSelChange(HWND hwnd)
    {
     DWORD lastsel;
     DWORD newsel;

     // check to see if we changed the selection
     newsel   = Edit_GetSel(hwnd);
     lastsel  = (DWORD)GetProp(hwnd, MAKEINTATOM(ATOM_LASTSEL));
     if(lastsel != newsel)
        { /* selection changed */
         SetProp(hwnd, MAKEINTATOM(ATOM_LASTSEL), (HANDLE)newsel);
         PostMessage(GetParent(hwnd), UWM_UPDATE_MENU, 0, 0);
        } /* selection changed */
     
    }

/****************************************************************************
*                         editsubclass_OnRButtonUp
* Inputs:
*       HWND hwnd: Window handle
*	BOOL dblclk: FALSE, always
*	int x: Mouse position
*	int y: Mouse position
*	UINT keyflags: Key status flags
* Result: void
*       
* Effect: 
*       Forces the button-down message to the parent
****************************************************************************/

static void
editsubclass_OnRButtonUp(HWND hwnd, int x, int y, UINT keyflags)
    {
     FORWARD_WM_RBUTTONUP(GetParent(hwnd), x, y, keyflags, SendMessage);
    }

/****************************************************************************
*                           editsubclass_OnDestroy
* Inputs:
*       HWND hwnd: Window handle
* Result: void
*       
* Effect: 
*       Deletes the brush attached to the edit control and the brush property
*	from the window.  Deletes the selection property from the window.
****************************************************************************/

static void
editsubclass_OnDestroy(HWND hwnd)
    {
     HBRUSH br;

     br = GetProp(hwnd, MAKEINTATOM(ATOM_BKG));
     if(br != NULL)
	DeleteBrush(br);

     RemoveProp(hwnd, MAKEINTATOM(ATOM_BKG));
     RemoveProp(hwnd, MAKEINTATOM(ATOM_LASTSEL));
    }

/****************************************************************************
*                              editSubclassProc
* Inputs:
*       HWND hwnd: Edit control
*       int message: Message ID
*       WPARAM wParam:
*       LPARAM lParam:
* Result: LRESULT
*       Per-message defined
* Effect: 
*       Intercepts EM_SETSEL messages and forces a recomputation of the
*       menu state
****************************************************************************/

static LRESULT CALLBACK 
editSubclassProc(HWND hwnd, int message, WPARAM wParam, LPARAM lParam)
    {
     switch(message)
        { /* message */
	 HANDLE_MSG(hwnd, WM_RBUTTONUP, editsubclass_OnRButtonUp);
	 HANDLE_MSG(hwnd, WM_DESTROY, editsubclass_OnDestroy);
	 HANDLE_MSG(hwnd, WM_CTLCOLOREDIT, editsubclass_OnCtlColor);

         case EM_SETSEL:
                 return editsubclass_OnSetSel(hwnd, message, wParam, lParam);

         // We intercept these messages, any one of which could have caused
         // a change in the selection status.
         case WM_KEYUP:
         case WM_SYSKEYUP:
         case WM_LBUTTONUP:
                 editsubclass_CheckSelChange(hwnd);
                 break;
        } /* message */

     return CallWindowProc(getProc(hwnd), hwnd, message, wParam, lParam);
    }

/****************************************************************************
*                                  edit_New
* Inputs:
*       HWND parent: Parent window of edit control
*	DWORD styles: Style flags
* Result: HWND
*       New edit handle, or NULL if failure
* Effect: 
*       Creates and subclasses an edit window
****************************************************************************/

HWND edit_New(HWND parent, DWORD styles, BOOL subclassing)
    {
     RECT r;
     HWND hedit;

     GetClientRect(parent, &r);
     
     hedit = CreateWindow(_T("EDIT"), NULL,
     			  styles,
			  r.left, r.top,
			  r.right - r.left,
			  r.bottom - r.top,
			  parent,
			  (HMENU) ID_EDIT_CONTROL,
			  GetWindowInstance(parent),
			  (LPVOID)NULL);

     if(hedit != NULL)
	{ /* have edit control */
	 SetFocus(hedit);

	 if(subclassing)
	    setProc(hedit, SubclassWindow(hedit, editSubclassProc));

	 SetProp(hedit, MAKEINTATOM(ATOM_BKG), 
		       CreateSolidBrush(GetSysColor(COLOR_WINDOW)));

	 if(styles & ES_READONLY)
	    { /* read-only */
	     TCHAR rodata[256];
	     LoadString(GetWindowInstance(parent), IDS_RO_TEXT,
				    rodata, DIM(rodata));
	     SetWindowText(hedit, rodata);
	    } /* read-only */
	} /* have edit control */

     return hedit;
    }

/****************************************************************************
*                               edit_ChooseFont
* Inputs:
*       HWND hwnd: Window handle of edit control
* Result: void
*       
* Effect: 
*       Changes the font and font color of the window
****************************************************************************/

void edit_ChooseFont(HWND hwnd)
    {
     LOGFONT lf;
     HFONT hf;
     CHOOSEFONT cf;
     
     hf = GetWindowFont(hwnd);
     
     cf.lStructSize = sizeof(cf);
     cf.hwndOwner = hwnd;
     cf.lpLogFont = &lf;
     cf.Flags = CF_NOVECTORFONTS | CF_SCREENFONTS | CF_EFFECTS;
     cf.rgbColors = getRGB(hwnd);

     if(hf != NULL)
        { /* has font */
         GetObject(hf, sizeof(lf), &lf);
         cf.Flags |= CF_INITTOLOGFONTSTRUCT;
        } /* has font */

     if(ChooseFont(&cf) != 0)
        { /* successful choosefont */
         HFONT nf = CreateFontIndirect(&lf);
         if(nf != NULL)
            { /* created successfully */
             DeleteFont(hf);
             SetWindowFont(hwnd, nf, TRUE);
             setRGB(hwnd, cf.rgbColors);
            } /* created successfully */
        } /* successful choosefont */
     
    }

/****************************************************************************
*                               edit_UpdateMenu
* Inputs:
*       HWND hedit: Edit control
*	HMENU hmenu: Menu handle of main window menu
* Result: void
*       
* Effect: 
*       Updates the menu based on the edit state
****************************************************************************/

void edit_UpdateMenu(HWND hedit, HMENU hmenu)
    {
     int first;
     int last;
     BOOL hassel;

     SendMessage(hedit, EM_GETSEL, 
                        (WPARAM)&first, (LPARAM)&last);
     hassel = (first != last);

     EnableMenuItem(hmenu, ID_EDIT_CUT, 
                    MF_BYCOMMAND | (!(GetWindowStyle(hedit) & ES_READONLY) 
		    			&& hassel 
                                           ? MF_ENABLED 
					   : MF_GRAYED));
     EnableMenuItem(hmenu, ID_EDIT_COPY, 
                    MF_BYCOMMAND | (hassel ? MF_ENABLED : MF_GRAYED));
     EnableMenuItem(hmenu, ID_EDIT_PASTE,
                    MF_BYCOMMAND | 
                        (IsClipboardFormatAvailable(CF_TEXT) ? MF_ENABLED
                                                             : MF_GRAYED));
     EnableMenuItem(hmenu, ID_EDIT_UNDO,
                    MF_BYCOMMAND | (Edit_CanUndo(hedit) 
                                                        ? MF_ENABLED
                                                        : MF_GRAYED));
     EnableMenuItem(hmenu, ID_EDIT_SELECT_ALL,
                        (Edit_GetTextLength(hedit) > 0
                                                        ? MF_ENABLED
                                                        : MF_GRAYED));
     EnableMenuItem(hmenu, ID_EDIT_CLEAR,
                    MF_BYCOMMAND | (hassel ? MF_ENABLED : MF_GRAYED));

    }
