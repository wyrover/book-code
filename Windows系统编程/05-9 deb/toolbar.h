
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993-1997 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

#ifndef TOOLBAR_H

 #define TOOLBAR_H

 #define TB_SPACE 0xFFFFFFFF

 //-- typedefs
 typedef struct TEXTBUTTON_STRUCT {
           LPTSTR lpButtonText;   // The string that appears on the button
           INT    idButton;       // This id is sent to the window proc
           HWND   hWndButton;     // The returned window handle of the button
         } TEXTBUTTON, *LPTEXTBUTTON;

 //-- prototypes
 HWND TextButtonBar( HWND, LPTEXTBUTTON, LPINT );

#endif // TOOLBAR_H
