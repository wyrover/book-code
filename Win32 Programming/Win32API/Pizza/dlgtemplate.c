#define STRICT
#include <windows.h>            // Fundamental Windows header file
#include <windowsx.h>           // Useful Windows programming extensions
#include <commctrl.h>           // Common Controls declarations
//#include <string.h>
//#include <wchar.h>

#include "Extensions.h"         // WINDOWSX.H extensions

#include "Utility.h"            // Application-independent
                                //  debugging and utility functions

#include "dlgtemplate.h"

/*****************************************************************************
                                 appendString
*****************************************************************************/

__inline LPWORD appendString(LPWORD ptr, LPCWSTR text)
    {
     LPWSTR str = (LPWSTR)ptr;
     wcscpy(str, text);
     ptr = (LPWORD)(str + wcslen(str) + 1);
     return ptr;
    }

/*****************************************************************************
                                 setClassAtom
*****************************************************************************/

__inline LPWORD setClassAtom(LPDLGITEMTEMPLATE item, WORD classatom)
    {
     LPWORD ptr = (LPWORD)&item[1];
     *ptr++ = 0xFFFF;
     *ptr++ = classatom;
     return ptr;
    }

/*****************************************************************************
                                 SetClassName
*****************************************************************************/
__inline LPWORD SetClassName(LPDLGITEMTEMPLATE item, LPCWSTR classname)
    {
     LPWORD ptr = (LPWORD)&item[1];
     ptr = appendString(ptr, classname);
     return ptr;
    }

/*****************************************************************************
                                 setResourceID
*****************************************************************************/

__inline LPWORD setResourceID(LPWORD ptr, short id)
    {
     *ptr++ = 0xFFFF;
     *ptr++ = (WORD)id;
     return ptr;
    }

/*****************************************************************************
                                 noParms
*****************************************************************************/
__inline LPWORD noParms(LPDLGITEMTEMPLATE item, LPWORD ptr)
    {
     *ptr++ = 0;   // no parameters
     if( (((LPWORD)item) - ptr) & 1)
        *ptr++ = 0;

     return ptr;
    }

/*****************************************************************************
                                  setItemPos
*****************************************************************************/

__inline void setItemPos(LPDLGITEMTEMPLATE item, int x, int y, int cx, int cy)
    {
      item->x = x;
      item->y = y;
      item->cx = cx;
      item->cy = cy;
    }


/*****************************************************************************
                                  AUTO3STATE
*****************************************************************************/

LPDLGITEMTEMPLATE AUTO3STATE(LPDLGITEMTEMPLATE item, LPCWSTR text,
                        short id, int x, int y, int cx, int cy, 
                        DWORD style, DWORD exstyle)
    {
     return CONTROL(item, text, id, MAKEINTRESOURCEW(0x0080), 
                    style | BS_AUTO3STATE,
                    x, y, cx, cy, 
                    exstyle);
    }

/*****************************************************************************
                                AUTOCHECKBOX
*****************************************************************************/

LPDLGITEMTEMPLATE AUTOCHECKBOX(LPDLGITEMTEMPLATE item, LPCWSTR text,
                        short id, int x, int y, int cx, int cy, 
                        DWORD style, DWORD exstyle)
    {
     return CONTROL(item, text, id, MAKEINTRESOURCEW(0x0080),
                    style | BS_AUTOCHECKBOX,
                    x, y, cx, cy, 
                    exstyle);
    }

/*****************************************************************************
                                AUTORADIOBUTTON
*****************************************************************************/

LPDLGITEMTEMPLATE AUTORADIOBUTTON(LPDLGITEMTEMPLATE item, LPCWSTR text,
                        short id, int x, int y, int cx, int cy, 
                        DWORD style, DWORD exstyle)
    {
     return CONTROL(item, text, id, MAKEINTRESOURCEW(0x0080),
                    style | BS_AUTORADIOBUTTON,
                    x, y, cx, cy, 
                    exstyle);
    }

/*****************************************************************************
                                   CHECKBOX
*****************************************************************************/

LPDLGITEMTEMPLATE CHECKBOX(LPDLGITEMTEMPLATE item, LPCWSTR text,
                        short id, int x, int y, int cx, int cy, 
                        DWORD style, DWORD exstyle)
    {
     return CONTROL(item, text, id, MAKEINTRESOURCEW(0x0080),
                    style | BS_CHECKBOX,
                    x, y, cx, cy, 
                    exstyle);
    }

/*****************************************************************************
                                   COMBOBOX
*****************************************************************************/

LPDLGITEMTEMPLATE COMBOBOX(LPDLGITEMTEMPLATE item, short id, 
                        int x, int y, int cx, int cy, DWORD style, 
                        DWORD exstyle)
    {

     return CONTROL(item, L"", id, MAKEINTRESOURCEW(0x0085),
                       style, 
                       x, y, cx, cy, 
                       exstyle);

    }

/*****************************************************************************
                                    CONTROL
*****************************************************************************/

LPDLGITEMTEMPLATE CONTROL(LPDLGITEMTEMPLATE item, LPCWSTR text, 
                                        short id, LPCWSTR classname, 
                                        DWORD style, 
                                        int x, int y, int cx, int cy, 
                                        DWORD exstyle)
    {
     LPWORD ptr = (LPWORD) &item[1];

     item->style = WS_CHILD | style;
     item->dwExtendedStyle = exstyle;
     setItemPos(item, x, y, cx, cy);
     item->id = (WORD)id;

     if(HIWORD(classname) != 0)
        ptr = SetClassName(item, classname);
     else
        ptr = setResourceID(ptr, LOWORD(classname)); // MAKEINTRESOURCEW(class)
     
     if(text == 0)
        ptr = appendString(ptr, L"");
     else
     if(HIWORD(text) != 0)
        ptr = appendString(ptr, text);
     else
        ptr = setResourceID(ptr, LOWORD(text));  // MAKEINTRESOURCEW(id)

     ptr = noParms(item, ptr);

     return (LPDLGITEMTEMPLATE)ptr;
    }

/*****************************************************************************
                                     CTEXT
*****************************************************************************/

LPDLGITEMTEMPLATE CTEXT(LPDLGITEMTEMPLATE item, LPCWSTR text, short id, 
                        int x, int y, int cx, int cy, DWORD style, 
                        DWORD exstyle)
    {

     return CONTROL(item, text, id, MAKEINTRESOURCEW(0x0082),
                    SS_CENTER | style, 
                    x, y, cx, cy, 
                    exstyle);

    }

/*****************************************************************************
                                 DEFPUSHBUTTON
*****************************************************************************/

LPDLGITEMTEMPLATE DEFPUSHBUTTON(LPDLGITEMTEMPLATE item, LPCWSTR text,
                        short id, int x, int y, int cx, int cy, 
                        DWORD style, DWORD exstyle)
    {
     return CONTROL(item, text, id, MAKEINTRESOURCEW(0x0080),
                    style | BS_DEFPUSHBUTTON,
                    x, y, cx, cy, 
                    exstyle);
    }

/*****************************************************************************
                                     EDITTEXT
*****************************************************************************/

LPDLGITEMTEMPLATE EDITTEXT(LPDLGITEMTEMPLATE item, short id, 
                        int x, int y, int cx, int cy, DWORD style, 
                        DWORD exstyle)
    {

     return CONTROL(item, L"", id, MAKEINTRESOURCEW(0x0081),
                     style, 
                     x, y, cx, cy, 
                     exstyle);

    }

/*****************************************************************************
                                   GROUPBOX
*****************************************************************************/

LPDLGITEMTEMPLATE GROUPBOX(LPDLGITEMTEMPLATE item, LPCWSTR text,
                        short id, int x, int y, int cx, int cy, 
                        DWORD style, DWORD exstyle)
    {
     return CONTROL(item, text, id, MAKEINTRESOURCEW(0x0080),
                    style | BS_GROUPBOX,
                    x, y, cx, cy, 
                    exstyle);
    }

/*****************************************************************************
                                     ICON
*****************************************************************************/

LPDLGITEMTEMPLATE ICON(LPDLGITEMTEMPLATE item, LPCWSTR icon, 
                                        short id, int x, int y, int cx, int cy,
                                        DWORD style, DWORD exstyle)
    {
     return CONTROL(item, icon, id, MAKEINTRESOURCEW(0x0082), 
                        SS_ICON | style,
                        x, y, cx, cy,
                        exstyle);
    }

/*****************************************************************************
                                     LTEXT
*****************************************************************************/

LPDLGITEMTEMPLATE LTEXT(LPDLGITEMTEMPLATE item, LPCWSTR text, short id, 
                        int x, int y, int cx, int cy, DWORD style, 
                        DWORD exstyle)
    {

     return CONTROL(item, text, id,  MAKEINTRESOURCEW(0x0082),
                    SS_LEFT | style, 
                    x, y, cx, cy, 
                    exstyle);
    }

/*****************************************************************************
                                    LISTBOX
*****************************************************************************/

LPDLGITEMTEMPLATE LISTBOX(LPDLGITEMTEMPLATE item, short id, 
                        int x, int y, int cx, int cy, DWORD style, 
                        DWORD exstyle)
    {

     return CONTROL(item, L"", id, MAKEINTRESOURCEW(0x0083), 
                    style, 
                    x, y, cx, cy, 
                    exstyle);

    }

/*****************************************************************************
                                  PUSHBUTTON
*****************************************************************************/

LPDLGITEMTEMPLATE PUSHBUTTON(LPDLGITEMTEMPLATE item, LPCWSTR text,
                        short id, int x, int y, int cx, int cy, 
                        DWORD style, DWORD exstyle)
    {
     return CONTROL(item, text, id, MAKEINTRESOURCEW(0x0080),
                    style | BS_PUSHBUTTON,
                    x, y, cx, cy, 
                    exstyle);
    }

/*****************************************************************************
                                    PUSHBOX
*****************************************************************************/

LPDLGITEMTEMPLATE PUSHBOX(LPDLGITEMTEMPLATE item, LPCWSTR text,
                        short id, int x, int y, int cx, int cy, 
                        DWORD style, DWORD exstyle)
    {
     return CONTROL(item, text, id, MAKEINTRESOURCEW(0x0080),
                    style | BS_PUSHLIKE,
                    x, y, cx, cy, 
                    exstyle);
    }

/*****************************************************************************
                                  RADIOBUTTON
*****************************************************************************/

LPDLGITEMTEMPLATE RADIOBUTTON(LPDLGITEMTEMPLATE item, LPCWSTR text,
                        short id, int x, int y, int cx, int cy, 
                        DWORD style, DWORD exstyle)
    {
     return CONTROL(item, text, id, MAKEINTRESOURCEW(0x0080),
                        style | BS_RADIOBUTTON,
                        x, y, cx, cy, 
                        exstyle);
    }

/*****************************************************************************
                                     RTEXT
*****************************************************************************/
LPDLGITEMTEMPLATE RTEXT(LPDLGITEMTEMPLATE item, LPCWSTR text, short id, 
                        int x, int y, int cx, int cy, DWORD style, 
                        DWORD exstyle)
    {

     return CONTROL(item, text, id, MAKEINTRESOURCEW(0x0082),
                    SS_RIGHT | style, 
                    x, y, cx, cy, 
                    exstyle);

    }

/*****************************************************************************
                                   SCROLLBAR
*****************************************************************************/

LPDLGITEMTEMPLATE SCROLLBAR(LPDLGITEMTEMPLATE item, short id, 
                        int x, int y, int cx, int cy, DWORD style, 
                        DWORD exstyle)
    {

     return CONTROL(item, L"", id, MAKEINTRESOURCEW(0x0084), 
                    style, 
                    x, y, cx, cy, 
                    exstyle);

    }

/*****************************************************************************
                                    STATE3
*****************************************************************************/

LPDLGITEMTEMPLATE STATE3(LPDLGITEMTEMPLATE item, LPCWSTR text,
                        short id, int x, int y, int cx, int cy, 
                        DWORD style, DWORD exstyle)
    {
     return CONTROL(item, text, id, MAKEINTRESOURCEW(0x0080),
                    style | BS_3STATE,
                    x, y, cx, cy, 
                    exstyle);
    }

/****************************************************************************
*                                   DIALOG
* Inputs:
*       LPDLGITEMTEMPLATE * item: On successful return, will be a pointer
*                       to the first place a DLGITEMTEMPLATE can be set
*       UINT size: Buffer size to allocate.  If 0, a default value of
*                       4096 is used.
*       int x: Origin of top left corner
*       int y: Origin of top left corner
*       int cx: Width in dialog units
*       int cy: Height in dialog units
*       DWORD style: Style bits
*       DWORD exstyle: Extended style bits
*       LPCWSTR menu:   NULL if no menu
*                       MAKEINTRESOURCEW(menuid) if by resource id
*                       L"menuname" for string menu
*       LPCWSTR class:  NULL if no class (default dialog box class)
*                       MAKEINTRESOURCEW(atom) if atom of class given
*                       L"classname" if class name of class given
*       LPCWSTR caption:NULL if no caption
*                       L"caption" if present
*       LPCWSTR font:   NULL if no font given (DS_SETFONT will be removed
*                               from styles)
*                       L"font" if font specified (DS_SETFONT will be added)
*       int height:     Font height in points (ignored if font is NULL)
* Result: LPDLGTEMPLATE
*       A dialog template, or NULL if an error occurred
* Effect: 
*       Allocates and initializes a DLGTEMPLATE structure
* Notes:
*       The caller must call free() to release the template
*       The cdit field will be set to 0; it is the caller's responsibility
*       to increment this field each time a control is added
****************************************************************************/

#define DLGTEMPLATE_WORKING_SIZE 4096

LPDLGTEMPLATE DIALOG(LPDLGITEMTEMPLATE * item,
                        UINT size, int x, int y, int cx, int cy,
                        DWORD style, DWORD exstyle, LPCWSTR menu,
                        LPCWSTR class, LPCWSTR caption, LPCWSTR font, 
                        int height)
    {
     LPDLGTEMPLATE dlg;
     LPWORD ptr;

     if(size == 0)
        size = DLGTEMPLATE_WORKING_SIZE;
     dlg = (LPDLGTEMPLATE) malloc(size);
     if(dlg == NULL)
        return NULL;
     
     dlg->x = x;
     dlg->y = y;
     dlg->cx = cx;
     dlg->cy = cy;

     dlg->cdit = 0;  // no dialog items yet

     dlg->style = style;
     if(font == NULL)
        dlg->style &= ~DS_SETFONT;
     else
        dlg->style |= DS_SETFONT;

     dlg->dwExtendedStyle = exstyle;
     dlg->cdit = 0;

     ptr = (LPWORD) &dlg[1];

     if(menu == NULL)
        *ptr++ = 0;     // no menu
     else
     if(HIWORD(menu) == 0)   
        ptr = setResourceID(ptr, LOWORD(menu)); // MAKEINTRESOURCEW
     else
        ptr = appendString(ptr, menu);


     if(class == NULL)
        *ptr++ = 0;
     else
     if(HIWORD(class) == 0)   
        ptr = setResourceID(ptr, LOWORD(class)); // MAKEINTRESOURCEW
     else
        ptr = appendString(ptr, class);
 
     ptr = appendString(ptr, (caption == NULL ? L"" : caption));

     if(font != NULL)
        { /* has font */
         *ptr++ = height;
         ptr = appendString(ptr, font);
        } /* has font */

     if(item != NULL)
        *item = (LPDLGITEMTEMPLATE) ptr;

     return (LPDLGTEMPLATE)dlg;
    }
