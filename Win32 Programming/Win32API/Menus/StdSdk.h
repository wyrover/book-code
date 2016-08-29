#if !defined (_StdSDK_H_)
#define _StdSDK_H_

#define STRICT					// Enable strict type-checking of Windows handles
#include <windows.h>            // Fundamental Windows header file
#include <windowsx.h>           // Useful Windows programming extensions
#include <commctrl.h>           // Common Controls declarations
#include <tchar.h>

#include "Extensions.h"         // WINDOWSX.H extensions

#include "Utility.h"            // Application-independent
                                //  debugging and utility functions

#include "Menus.h"      // Application declarations

#endif          /* _StdSDK_H_ */
