#include "StdSDK.h"         // Standard application includes
#include "resource.h"       // For resource identifiers
#include "pizza.h"
#include <tchar.h>          // For _tcsicmp

//
// Function prototypes for static functions
//

static DWORD formatMessageFromString (LPCTSTR szFormat, LPTSTR  szBuffer, DWORD nSize, ...) ;
static DWORD getToppings(HWND hwnd) ;
static int getSize(HWND hwnd);

//
// Function prototypes for callback functions
//

//
// Function prototypes for message handlers
//

static BOOL pizzaDlg_OnContextMenu (HWND hwnd, HWND hwndCtl, int xPos, int yPos) ;
static void pizzaDlg_OnHelp (HWND hwnd, LPHELPINFO lphi) ;
static BOOL pizzaDlg_OnInitDialog (HWND hwnd, HWND hwndFocus, LPARAM lParam) ;
static void pizzaDlg_OnCommand (HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) ;
static void pizzaDlg_OnEnterIdle(HWND hwnd, UINT source, HWND hwndSource) ;

#define ALLTOPPINGS(x) (x) = 0; ToppingMap[(x)].id != 0; (x)++


//
//  BOOL CALLBACK
//  pizzaDlgProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
//
//  PURPOSE:  
// 		Dialog function for the Pizza dialog box.
// 		Display version information for the application,
//      the operating system, and the processor.
//
//  MESSAGES:
//
//
//

BOOL CALLBACK
pizzaDlgProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
     switch (message) 
	{ /* message */
	 case WM_CONTEXTMENU:           // User clicked the right mouse button
		 return HANDLE_WM_CONTEXTMENU (hwnd, wParam, lParam, 
		 			pizzaDlg_OnContextMenu) ;

	 case WM_HELP:                       // User pressed the F1 key
		 return HANDLE_WM_HELP (hwnd, wParam, lParam, 
		 			pizzaDlg_OnHelp) ;

	 case WM_INITDIALOG:            // Initialization of controls complete
		 return HANDLE_WM_INITDIALOG (hwnd, wParam, lParam, 
		 			pizzaDlg_OnInitDialog) ;

	 case WM_COMMAND:                    // Notification from a control
		 return HANDLE_WM_COMMAND (hwnd, wParam, lParam, 
		 			pizzaDlg_OnCommand) ;
	 case WM_ENTERIDLE:
		 return HANDLE_WM_ENTERIDLE (hwnd, wParam, lParam,
		 			pizzaDlg_OnEnterIdle) ;
	} /* message */
    return FALSE ;
}

//  Array of control identifiers and help context identifier pairs. 

#define IDH_PIZZA_SMALL             IDC_SMALL
#define IDH_PIZZA_MEDIUM            IDC_MEDIUM
#define IDH_PIZZA_LARGE             IDC_LARGE

#define IDH_TOPPING_XCHEESE         IDC_XCHEESE
#define IDH_TOPPING_PEPPERONI       IDC_PEPPERONI
#define IDH_TOPPING_SAUSAGE         IDC_SAUSAGE
#define IDH_TOPPING_ONIONS          IDC_ONIONS
#define IDH_TOPPING_MUSHROOMS       IDC_MUSHROOMS
#define IDH_TOPPING_ANCHOVIES       IDC_ANCHOVIES
#define IDH_TOPPING_PINEAPPLE       IDC_PINEAPPLE
#define IDH_TOPPING_HOTPEPPERS      IDC_HOTPEPPERS
#define IDH_TOPPING_BLACKOLIVES     IDC_BOLIVES
#define IDH_TOPPING_GREENOLIVES     IDC_GOLIVES
#define IDH_TOPPING_BACON           IDC_BACON
#define IDH_TOPPING_SALAMI          IDC_SALAMI

#define IDH_SET_DEFAULT             IDC_SET_DEFAULT
#define IDH_VEGGIE                  IDC_VEGGIE
#define IDH_DELUXE                  IDC_DELUXE
#define IDH_ORDER                   IDOK
 
static DWORD aContextIds [] = { 
          IDC_SMALL,           IDH_PIZZA_SMALL,
          IDC_MEDIUM,          IDH_PIZZA_MEDIUM,
          IDC_LARGE,           IDH_PIZZA_LARGE,

          IDC_XCHEESE,         IDH_TOPPING_XCHEESE,
          IDC_PEPPERONI,       IDH_TOPPING_PEPPERONI,
          IDC_SAUSAGE,         IDH_TOPPING_SAUSAGE,
          IDC_ONIONS,          IDH_TOPPING_ONIONS,
          IDC_MUSHROOMS,       IDH_TOPPING_MUSHROOMS,
          IDC_ANCHOVIES,       IDH_TOPPING_ANCHOVIES,
          IDC_PINEAPPLE,       IDH_TOPPING_PINEAPPLE,
          IDC_HOTPEPPERS,      IDH_TOPPING_HOTPEPPERS,
          IDC_BOLIVES,         IDH_TOPPING_BLACKOLIVES,
          IDC_GOLIVES,         IDH_TOPPING_GREENOLIVES,
          IDC_BACON,           IDH_TOPPING_BACON,
          IDC_SALAMI,          IDH_TOPPING_SALAMI,

          IDC_SET_DEFAULT,     IDH_SET_DEFAULT,
          IDC_VEGGIE,          IDH_VEGGIE,
          IDC_DELUXE,          IDH_DELUXE,
          IDOK,                IDH_ORDER,
          IDCANCEL,            IDH_CANCEL,
    0,                          0
} ;
 
static const struct {int id; DWORD mask;} ToppingMap[] = {
  { IDC_XCHEESE,    TOPPING_XCHEESE      },
  { IDC_PEPPERONI,  TOPPING_PEPPERONI    },
  { IDC_SAUSAGE,    TOPPING_SAUSAGE      },
  { IDC_ONIONS,     TOPPING_ONIONS       },
  { IDC_MUSHROOMS,  TOPPING_MUSHROOMS    },
  { IDC_ANCHOVIES,  TOPPING_ANCHOVIES    },
  { IDC_PINEAPPLE,  TOPPING_PINEAPPLE    },
  { IDC_HOTPEPPERS, TOPPING_HOTPEPPERS   },
  { IDC_BOLIVES,    TOPPING_BLACKOLIVES  },
  { IDC_GOLIVES,    TOPPING_GREENOLIVES  },
  { IDC_BACON,      TOPPING_BACON        },
  { IDC_SALAMI,     TOPPING_SALAMI       },
  { 0, 0 }  // end of table
				 };

//
//  static __inline LPPIZZA getPizza ( HWND hwnd )
//
//  hwnd            Handle of window to which this message applies
//
//  Returns:
//	LPPIZZA, pointer to a pizza structure, or NULL if it hasn't been
//		 set yet
//
static __inline LPPIZZA getPizza( HWND hwnd )
{
    return (LPPIZZA)GetWindowLong(hwnd, GWL_USERDATA) ;
}

// static void pizzaDlg_OnEnterIdle(HWND hwnd, UINT source, HWND hwndSource)

static void
pizzaDlg_OnEnterIdle(HWND hwnd, UINT source, HWND hwndSource)
    {
     LPPIZZA pizza = getPizza(hwnd);
     int size;
     BOOL enable = FALSE;
     DWORD toppings;
     
     if(!pizza->standard)
	return; // nothing to do for 'other'


     // See if the size changed

     size = getSize(hwnd);

     enable |= (size != pizza->size);

     // See if any of the toppings changed

     toppings = getToppings(hwnd);
     
     enable |= (toppings != pizza->toppings);

     // See if the veggie state changed

     enable |= ((BOOL)(IsDlgButtonChecked(hwnd, IDC_VEGGIE)) != ((BOOL)pizza->veggie));

     if(enable != (BOOL)IsWindowEnabled(GetDlgItem(hwnd, IDC_SET_DEFAULT)))
	EnableWindow(GetDlgItem(hwnd, IDC_SET_DEFAULT), enable);
     if(!enable && IsDlgButtonChecked(hwnd, IDC_SET_DEFAULT))
	CheckDlgButton(hwnd, IDC_SET_DEFAULT, FALSE);
    }

//
//  void pizzaDlg_OnContextMenu (HWND hwnd, HWND hwndCtl. int xPos, int yPos)
//
//  hwnd            Handle of window to which this message applies
//  hwndCtl         Handle of the window in which the user right clicked the mouse
//  xPos            Horizontal position of the cursor, in screen coordinates
//  yPos            Vertical position of the cursor, in screen coordinates
//
//  PURPOSE:        Notification that the user clicked the right
//                  mouse button in the window.
//
//  COMMENTS:       Normally a window processes this message by
//                  displaying a context menu using the TrackPopupMenu
//                  or TrackPopupMenuEx functions.
//
//                  However it's convenient to allow the user to right
//                  click on a control in a dialog box to display a popup
//                  help topic about the control. This function implements
//                  the latter technique.
//

static BOOL
pizzaDlg_OnContextMenu (HWND hwnd, HWND hwndCtl, int xPos, int yPos)
{
    // Display popup help for the control
    WinHelp (hwndCtl, getHelpFileName (), HELP_CONTEXTMENU, (DWORD) (LPVOID) aContextIds) ;
    return TRUE ;
}


//
//  void pizzaDlg_OnHelp (HWND hwnd, LPHELPINFO lphi)
//
//  PURPOSE:        Notification that the user pressed the F1 key
//
//  COMMENTS:       Normally a window processes this message by
//                  displaying a context menu using the TrackPopupMenu
//                  or TrackPopupMenuEx functions. If a window does not
//                  display a context menu it should pass this message
//                  to the DefWindowProc function. 
//

static void
pizzaDlg_OnHelp (HWND hwnd, LPHELPINFO lphi)
{
    ASSERT (HELPINFO_WINDOW == lphi->iContextType) ;

    if (HELPINFO_WINDOW == lphi->iContextType) {    // Must be for a control!

        int             nID ;
        int             nIndex ;

        // Get the control's window handle
        HWND hwndCtl = lphi->hItemHandle ;

        ASSERT (NULL != hwndCtl) ;
        ASSERT (IsWindow (hwndCtl)) ;

        // Get this control's ID
        nID = GetWindowID (hwndCtl) ;

        // Don't bother running WinHelp unless we have help for the control
        for (nIndex = 0 ; nIndex < DIM (aContextIds) - 2; nIndex += 2) {
            if (aContextIds [nIndex] == (DWORD) nID) {
                WinHelp (hwndCtl, getHelpFileName (), 
                         HELP_WM_HELP, (DWORD) (LPVOID) aContextIds) ;
                return ;
            }
        }
    }
}

// 
// static DWORD getToppings(HWND hwnd)
//
static DWORD getToppings(HWND hwnd)
{
 int i;
 DWORD toppings = 0;

 for( ALLTOPPINGS(i) )
    { /* read controls */
     if(IsDlgButtonChecked(hwnd, ToppingMap[i].id))
	toppings |= ToppingMap[i].mask;
    } /* read controls */

 return toppings;
}

//
//  static void setVeggieMode (HWND hwnd)
//
//  hwnd            Window handle for the dialog box window

static void setVeggieMode(HWND hwnd)
{
    int i;
    BOOL veggie = IsDlgButtonChecked(hwnd, IDC_VEGGIE);

    for( ALLTOPPINGS(i) )
       { /* set veggie enables */
	BOOL  meat = (ToppingMap[i].mask & TOPPING_MEATS) ;
	EnableWindow(GetDlgItem(hwnd, ToppingMap[i].id), (meat && veggie 
								? 0 : 1)) ;
	if(meat && veggie)
	   CheckDlgButton(hwnd, ToppingMap[i].id, FALSE);
       } /* set veggie enables */
}
	    

//
// static void setToppings(HWND hwnd, DWORD toppings)
//
static void setToppings(HWND hwnd, DWORD toppings)
{
    int i;

    // Set the current buttons
    for( ALLTOPPINGS(i) )
        { /* set each button */
	 CheckDlgButton(hwnd, ToppingMap[i].id, 
	 	(toppings & ToppingMap[i].mask) ? 1 : 0);
	} /* set each button */ 

}

//
// static void setSize(HWND hwnd)
//
// EFFECT:
//	Checks the appropriate control for the pizza size

static void setSize(HWND hwnd)
    {
     int ctl = 0;
     LPPIZZA pizza = getPizza(hwnd);

     if(pizza == NULL)
	return;

     switch(pizza->size)
	{ /* size */
	 case SIZE_SMALL:
		 ctl = IDC_SMALL;
		 break;
	 case SIZE_MEDIUM:
		 ctl = IDC_MEDIUM;
		 break;
	 case SIZE_LARGE:
		 ctl = IDC_LARGE;
		 break;
	} /* size */

     if(ctl != 0)
	CheckRadioButton(hwnd, IDC_SMALL, IDC_LARGE, ctl);

    }

// static int getSize(HWND hwnd)
//
// RESULT:
//	The size code as derived from the radio buttons
// Notes:
//	If none of the radio buttons are checked, returns SIZE_ERR

static int getSize(HWND hwnd)
    {
     if(IsDlgButtonChecked(hwnd, IDC_SMALL))
	return SIZE_SMALL;
     if(IsDlgButtonChecked(hwnd, IDC_MEDIUM))
	return SIZE_MEDIUM;
     if(IsDlgButtonChecked(hwnd, IDC_LARGE))
	return SIZE_LARGE;

     return SIZE_ERR;  // this should never occur
    }

//
//  void pizzaDlg_OnInitDialog (HWND hwnd, HWND hwndFocus, LPARAM lParam)
//
//  hwnd            Window handle for the dialog box window
//  id              Specifies the identifier of the menu item, control, or accelerator.
//  hwndCtl         Handle of the control sending the message if the message
//                  is from a control, otherwise, this parameter is NULL. 
//  codeNotify      Specifies the notification code if the message is from a control.
//                  This parameter is 1 when the message is from an accelerator.
//                  This parameter is 0 when the message is from a menu.
//                  
//
//  PURPOSE:        
//                  
//                  
//
//  COMMENTS:
//

static BOOL
pizzaDlg_OnInitDialog (HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    LPPIZZA pizza = (LPPIZZA)lParam;

    // Set a reference to the pizza structure in the dialog

    SetWindowLong(hwnd, GWL_USERDATA, lParam);

    // Center the dialog window
    centerWindow (hwnd, NULL) ;

    setToppings(hwnd, pizza->toppings);

    setSize(hwnd);

    CheckDlgButton(hwnd, IDC_VEGGIE, pizza->veggie);

    setVeggieMode(hwnd);

    if(!pizza->standard)
       { /* not standard pizza */
	ShowWindow(GetDlgItem(hwnd, IDC_SET_DEFAULT), SW_HIDE);
       } /* not standard pizza */
    else
       { /* standard pizza */
	EnableWindow(GetDlgItem(hwnd, IDC_SET_DEFAULT), FALSE);
       } /* standard pizza */
        

#define SEP _T(": ")
    if(pizza->name != NULL)
       { /* has name */
	int captionlen = GetWindowTextLength(hwnd);
	int newlen;
	LPTSTR newname;

	newlen = captionlen + lstrlen(pizza->name);

	newname = (LPTSTR)malloc( (newlen + lstrlen(SEP) + 1) * sizeof(TCHAR));
	if(newname != NULL)
	   { /* allocated buffer */
	    GetWindowText(hwnd, newname, captionlen + 1);
	    lstrcat(newname, SEP);
	    lstrcat(newname, pizza->name);
	    SetWindowText(hwnd, newname);
	    free(newname);
	   } /* allocated buffer */
       } /* has name */

    return TRUE ;
}


//
//  DWORD formatMessageFromString (LPCTSTR szFormat, LPTSTR  szBuffer, DWORD nSize, ...)
//
//  szFormat        Format string containing message insertion markers
//  szBuffer        Output string buffer
//  nSize           Size of output string buffer
//  ...             Variable number of optional parameter
//                  
//
//  PURPOSE:        
//                  Convenient helper function for calling formatMessage
//
//  COMMENTS:
//

static DWORD
formatMessageFromString (LPCTSTR szFormat, LPTSTR  szBuffer, DWORD nSize, ...)
{
    DWORD               dwRet ;
    va_list             marker ;

    va_start (marker, nSize) ;              // Initialize variable arguments

    dwRet = FormatMessage (FORMAT_MESSAGE_FROM_STRING,
                           szFormat, 0, 0,
                           szBuffer, nSize,
                           &marker) ;

    va_end (marker) ;                       // Reset variable arguments

    return dwRet ;
}


// static void setDeluxePizza(HWND hwnd)
//
//  hwnd            Window handle for the dialog box window

static void setDeluxePizza(HWND hwnd)
{
 BOOL veggie;
 DWORD toppings;

 veggie = IsDlgButtonChecked(hwnd, IDC_VEGGIE);

 toppings = TOPPING_ALL;
 if(veggie)
    toppings &= ~TOPPING_MEATS;

 setToppings(hwnd, toppings);
}


//
// void savePizzaState(HWND hwnd)
//
// Effect:
//	Reads the set of controls back.  The values are stored in the
//	pizza structure

static void savePizzaState(HWND hwnd)
{
 LPPIZZA pizza = getPizza(hwnd);
 
 
 if(pizza == NULL)
    return;

 pizza->toppings = getToppings(hwnd);
 pizza->veggie = IsDlgButtonChecked(hwnd, IDC_VEGGIE);
 pizza->size = getSize(hwnd);
 if(IsWindowEnabled(GetDlgItem(hwnd, IDC_SET_DEFAULT)))
    pizza->update_standard = IsDlgButtonChecked(hwnd, IDC_SET_DEFAULT);
}

// copyWindow: Makes a copy of the window in the clipboard, for getting
// the illustration for the book.
//
// Notes:
//	This hides the little copy button at the bottom before doing the
//	screen snapshot.

static void copyWindow(HWND hwnd)
{
 HDC wdc;
 HDC memDC;
 HBITMAP bm;
 RECT r;
 SIZE sz;
 HBITMAP oldbm;

 ShowWindow(GetDlgItem(hwnd, IDC_COPY), SW_HIDE);

 wdc = GetWindowDC(hwnd);

 memDC = CreateCompatibleDC(wdc);

 GetWindowRect(hwnd, &r);
 sz.cx = r.right - r.left;
 sz.cy = r.bottom - r.top;

 bm = CreateCompatibleBitmap(wdc, sz.cx, sz.cy);

 oldbm = SelectObject(memDC, bm);

 BitBlt(memDC, 0, 0, sz.cx, sz.cy, wdc, 0, 0, SRCCOPY);

 OpenClipboard(hwnd);
 EmptyClipboard();
 SetClipboardData(CF_BITMAP, bm);
 CloseClipboard();

 SelectObject(memDC, oldbm);

 ReleaseDC(hwnd, wdc);

 ShowWindow(GetDlgItem(hwnd, IDC_COPY), SW_SHOW);
}

static void pizzaDlg_OnOK(HWND hwnd)
    {
     savePizzaState(hwnd);
     EndDialog (hwnd, TRUE) ;
    }

//
//  void pizzaDlg_OnCommand (HWND hwnd, int id, hwnd hwndCtl, UINT codeNotify)
//
//  hwnd            Window handle for the dialog box window
//  id              Specifies the identifier of the menu item, control, or accelerator.
//  hwndCtl         Handle of the control sending the message if the message
//                  is from a control, otherwise, this parameter is NULL. 
//  codeNotify      Specifies the notification code if the message is from a control.
//                  This parameter is 1 when the message is from an accelerator.
//                  This parameter is 0 when the message is from a menu.
//                  
//
//  PURPOSE:        
//                  Handle the keyboard and control notifications.
//                  An OK button press, or Enter/Esc keypress
//                  all dismiss the Pizza dialog box.
//                  
//
//  COMMENTS:
//

static
void pizzaDlg_OnCommand (HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id) {
        case IDOK:                          // OK pushbutton/Enter keypress
	    pizzaDlg_OnOK(hwnd);
	    break;
        case IDCANCEL:                      // Esc keypress or Cancel
	    savePizzaState(hwnd);
            EndDialog (hwnd, FALSE) ;       // Dismiss the Pizza dialog box
	    break ;

	case IDC_VEGGIE:
		setVeggieMode(hwnd);
		break;

        case IDC_DELUXE:
		setDeluxePizza(hwnd);
		break;
	case IDC_COPY:
		copyWindow(hwnd);
	default:
            FORWARD_WM_COMMAND (hwnd, id, hwndCtl, codeNotify, DefWindowProc) ;
	}
}


BOOL getPizzaOrder(HWND hwnd, LPPIZZA pizza)
    {
     return DialogBoxParam(GetWindowInstance(hwnd),
     		           MAKEINTRESOURCE(IDD_PIZZA),
		           hwnd,
			   pizzaDlgProc,
			   (LPARAM)pizza) == IDOK;
		      
    }

BOOL orderPizza(HWND hwnd, LPPIZZA pizza)
    {
     return 
        (MessageBox(hwnd, "Ordering Pizza", "Confirm order", MB_OKCANCEL) == IDOK);

    }


//
// Default pizza table:  This will eventually be replaced by a different
// structure that is loaded from a database, such as the Registry.  However,
// for this example, we use a predefined array of person-pizza names.
//
PIZZA PizzaTable[] = {
  {TRUE, SIZE_MEDIUM, _T("Joe"),  FALSE, TOPPING_PEPPERONI | 
   					 TOPPING_ANCHOVIES |
  				         TOPPING_HOTPEPPERS | 
					 TOPPING_BLACKOLIVES},
  {TRUE, SIZE_MEDIUM, _T("Brent"),FALSE, TOPPING_PEPPERONI | 
   					 TOPPING_SAUSAGE |
					 TOPPING_ONIONS |
					 TOPPING_XCHEESE |
   					 TOPPING_MUSHROOMS},
  {TRUE, SIZE_MEDIUM, _T("Alan"), FALSE, TOPPING_SAUSAGE | 
   					 TOPPING_XCHEESE},
  {TRUE, SIZE_MEDIUM, _T("Mike"), FALSE, TOPPING_PEPPERONI | 
   					 TOPPING_GREENOLIVES |
					 TOPPING_MUSHROOMS |
					 TOPPING_SAUSAGE |
					 TOPPING_XCHEESE |
   					 TOPPING_ONIONS},
  {FALSE, SIZE_MEDIUM, NULL,      FALSE, TOPPING_PEPPERONI}// END OF TABLE
		     };

// 
// void select_OnInitDialg(HWND hwnd, HWND hwndFocus, LPARAM lParam)
//
// 	hwnd: The dialog window handle
//
// 	hwndFocus: The handle of the control that will receive focus 
//
// 	lParam: (LPPIZZA) A pointer to an array of standard pizzas.
//
// EFFECT:
//	Loads up a list box with the names of all the standard pizzas,
//	and adds an entry for "Other".  
// 
// RETURNS:
//	TRUE, always
//
static BOOL select_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
    {
     int i;
     int index;
     LPPIZZA pizza = (LPPIZZA)lParam;
     TCHAR otherstr[256];

     // Center the dialog window
     centerWindow (hwnd, NULL) ;


     for(i = 0; pizza[i].name != NULL; i++)
        { /* load standard pizzas */
	 index = ListBox_AddString(GetDlgItem(hwnd, IDC_SELECTIONS), 
	 				pizza[i].name);
         ListBox_SetItemData(GetDlgItem(hwnd, IDC_SELECTIONS), index,
	 				&pizza[i]);
	} /* load standard pizzas */

     // Now that we've loaded up all the standard names, load up the
     // "Other" case.  Use the specs of the last entry to set the default

     LoadString(GetWindowInstance(hwnd), IDS_OTHER, otherstr, 
     					sizeof(otherstr) / sizeof(TCHAR));
     index = ListBox_AddString(GetDlgItem(hwnd, IDC_SELECTIONS), otherstr);
     ListBox_SetItemData(GetDlgItem(hwnd, IDC_SELECTIONS), index, &pizza[i]);
     ListBox_SetCurSel(GetDlgItem(hwnd, IDC_SELECTIONS), index);


     return TRUE;
    }

void pizzaOrderFailure(HWND hwnd, LPPIZZA pizza)
    {
     // NYI: Fix this up later:
     MessageBox(hwnd, "Order failure", "Can't order pizza", MB_OK);
    }

// void select_OnOK(HWND hwnd)
//
static void select_OnOK(HWND hwnd)
    {
     LPPIZZA pizza = NULL;
     int index;

     index = ListBox_GetCurSel(GetDlgItem(hwnd, IDC_SELECTIONS));
     if(index != LB_ERR)
	{ /* has selection */
	 pizza = (LPPIZZA)ListBox_GetItemData(GetDlgItem(hwnd, IDC_SELECTIONS), 
	 						index);
	} /* has selection */
     EndDialog(hwnd, (int)pizza);
    }

// void select_OnSelection(HWND hwnd, HWND hwndCtl, UINT notifycode)
//
// 	hwnd: The window handle of the dialog
//	hwndCtl: The control handle of the people list (ignored)
//	notifycode: The notification code
//
// EFFECT:
//	If the user double-clicks on a name, select that name as the
//	name of choice and exit as if the OK button had been clicked.

static void select_OnSelection(HWND hwnd, HWND hwndCtl, UINT notifycode)
    {
     switch(notifycode)
        { /* notifycode */
	 case LBN_DBLCLK:
		 select_OnOK(hwnd);
		 return;
	} /* notifycode */
    }

// void select_MoveButton(HWND hwnd, UINT id)
//
// 	hwnd: the dialog window
//	id: Control ID to move
//
// EFFECT:
//	Moves the button horizontally so that it is always on the right edge
// 	of the window
//
static void select_MoveButton(HWND hwnd, UINT id)
    {
     RECT client;
     RECT r;
     SIZE sz;

     GetClientRect(hwnd, &client);
     GetWindowRect(GetDlgItem(hwnd, id), &r);

     // We how have the window coordinates of the button.  Convert them
     // to client coordinates

     ScreenToClient(hwnd, (LPPOINT)&r.left);
     ScreenToClient(hwnd, (LPPOINT)&r.right);

     sz.cx = r.right - r.left;
     sz.cy = r.bottom - r.top;

     MoveWindow(GetDlgItem(hwnd, id), client.right - sz.cx, r.top,
     				      sz.cx, sz.cy, TRUE);
    }

// void select_OnSize(HWND hwnd, UINT state, int cx, int cy)
//
// 	hwnd: Window handle of dialog
//	state: SIZE_MINIMIZED will be ignored.  All other values apply
//	cx: new width of client area
//	cy: new height of client area
//
// EFFECT:
//	When the window is resized, move the OK and Cancel buttons so they
//	are still visible and resize the list box width and height

static void select_OnSize(HWND hwnd, UINT state, int cx, int cy)
    {
     RECT client;
     RECT r;
     SIZE sz;

     if(state == SIZE_MINIMIZED)
	return; // ignore iconic change

     // Move the OK and Cancel buttons so they are again flush against the
     // right edge of the window

     select_MoveButton(hwnd, IDOK);
     select_MoveButton(hwnd, IDCANCEL);

     // Next, resize the list box to fit the remaining area:

     GetClientRect(hwnd, &client);
     GetWindowRect(GetDlgItem(hwnd, IDC_SELECTIONS), &r);

     ScreenToClient(hwnd, (LPPOINT)&r.left);
     ScreenToClient(hwnd, (LPPOINT)&r.right);

     sz.cx = client.right - client.left;
     sz.cy = client.bottom - r.top;

     // If the size should be negative, the list box is invisible so don't
     // even try to resize it.

     if(sz.cy > 0)
        { /* resize list box */
	 MoveWindow(GetDlgItem(hwnd, IDC_SELECTIONS), 
	 		r.left, r.top,
			sz.cx, sz.cy, TRUE);
	} /* resize list box */
    }

// void select_OnGetMinMaxInfo(HWND hwnd, LPMINMAXINFO mmi)
//
//	hwnd: Window handle of dialog
//	mmi: an LPMINMAXINFO structure.  We do not allow the dialog
//		to become smaller than the OK button (which is the same
//		size as the Cancel button) or to become shorter than is
//		necessary to allow a 1-line list box
//

static void select_OnGetMinMaxInfo(HWND hwnd, LPMINMAXINFO mmi)
    {
     POINT limit;
     RECT r;
     int lineheight;

     // We don't want the window to get any narrower than the size of
     // the OK button, plus the width of its frame

     GetWindowRect(GetDlgItem(hwnd, IDOK), &r);
     limit.x = r.right - r.left + 2 * GetSystemMetrics(SM_CYFRAME);

     // Get the position and size of the list box in client coordinates

     GetWindowRect(GetDlgItem(hwnd, IDC_SELECTIONS), &r);
     ScreenToClient(hwnd, (LPPOINT)&r.left);
     ScreenToClient(hwnd, (LPPOINT)&r.right);

     // Find out how high one line is in the list box:

     lineheight = ListBox_GetItemHeight(GetDlgItem(hwnd, IDC_SELECTIONS), 0);

     // Don't forget to add the thickness of the top and bottom borders and the
     // caption bar height!  Remember, this is the *window* height we're
     // computing!

     limit.y = r.top + lineheight + GetSystemMetrics(SM_CYCAPTION) +
     	        2 * GetSystemMetrics(SM_CYBORDER) +
     		2 * GetSystemMetrics(SM_CYFRAME);

     // Set our new limit value in the MINMAXINFO structure

     mmi->ptMinTrackSize = limit;
     
    }

// void select_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT notifycode)
//
// Handles the OK and Cancel buttons

static void select_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT notifycode)
    {
     switch(id)
        { /* id */
	 case IDOK:
		 select_OnOK(hwnd);
		 return;
	 case IDCANCEL:
		 EndDialog(hwnd, (int)NULL);
		 return;
	 case IDC_SELECTIONS:
		 select_OnSelection(hwnd, hwndCtl, notifycode);
		 return;
	} /* id */
    }

//
// This fires off a dialog that loads up the 
BOOL CALLBACK selectProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
     switch(msg)
        { /* msg */
	 case WM_INITDIALOG:
		 return (BOOL)HANDLE_WM_INITDIALOG(hwnd, wParam, lParam, 
	 				  	    select_OnInitDialog);
         case WM_SIZE:
		 HANDLE_WM_SIZE(hwnd, wParam, lParam, select_OnSize);
		 return TRUE;
         case WM_COMMAND:
		 HANDLE_WM_COMMAND(hwnd, wParam, lParam, select_OnCommand);
		 return TRUE;
         case WM_GETMINMAXINFO:
		 HANDLE_WM_GETMINMAXINFO(hwnd, wParam, lParam,
		 				select_OnGetMinMaxInfo);
	} /* msg */
     return FALSE;
    }

//
// LPPIZZA getPizzaSelection(HWND hwnd)
//
//	hwnd: The parent window for the dialog
//
// RESULT:
//	LPPIZZA
//	NULL if any error, or operation is cancelled
//	Pointer to PIZZA object selected, otherwise
//
LPPIZZA getPizzaSelection(HWND hwnd)
    {
     LPPIZZA pizza = (LPPIZZA)DialogBoxParam(GetWindowInstance(hwnd),
     					     MAKEINTRESOURCE(IDD_SELECTPIZZA),
					     hwnd,
					     selectProc,
					     (LPARAM)PizzaTable);
					

     return pizza;
    }


/****************************************************************************
*                                releasePizza
* Inputs:
*       LPPIZZA pizza: Pizza object to release
* Result: void
*       
* Effect: 
*       No effect, as right now the pizza object is statically allocated
****************************************************************************/

void releasePizza(LPPIZZA pizza)
    {
     // NYI: does not need to do anything this week
    }
