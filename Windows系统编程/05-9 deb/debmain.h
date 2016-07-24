
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993-1997 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

#ifndef DEBMAIN_H

 #define DEBMAIN_H

 #ifndef DEB_H
  #include "DEB.H"    // resource and message ids
 #endif

 // ********
 // typedefs
 // ********
 typedef struct PROFILE_STRUCT {
   INT       xPos;               // application window's horizontal position
   INT       yPos;               // application window's vertical position
   INT       nWidth;             // application window's width
   INT       nHeight;            // application window's height
   BOOL      fMaximized;         // flag: maximized state?
   BOOL      fMinimized;         // flag: minimized state?
   BOOL      fToolBar;           // flag: display the ToolBar?
   BOOL      fSavedDirectory;    // flag: use saved directory for opening files?
   TCHAR     szInitialDir[MAX_PATH];  // initial directory for debuggees
   LOGFONT   LogFont;            // current logical font
   COLORREF  rgbForeColor;       // current listbox text foreground color
   COLORREF  rgbBackColor;       // current listbox text foreground color
   BOOL      fClearOnNew;        // flag: clears the event listbox on a new debuggee
   BOOL      fVerbose;           // flag: verbose event listings
   BOOL      fShowSymbols;       // flag: show symbols (limited)
   LONG      DebugMode;          // debug mode flag CreateProcess
   LONG      DebuggeePriority;   // debuggee priority CreateProcess
   DWORD     DebugErrorLevel;    // debugger error level
   BOOL      fSavePreferences;   // flag: save the preferences settings
   BOOL      fSaveOnExit;        // flag: save settings upon exit?
 } PROFILE, *PPROFILE;

 typedef struct GLOBAL_STRUCT {
   HINSTANCE hInstance;          // current instance
   HWND      hWndMain;           // Main window handle
   HWND      hWndListBox;        // debug event listbox handle
   HWND      hWndToolBar;        // windows handle to the text toolbar
   WNDPROC   OldListBoxWndProc;  // original listbox procedure
   TCHAR     szAppName[64];      // name of the application
   TCHAR     szShortAppName[32]; // short name of the application
   TCHAR     szApiFailed[64]; // "A Windows API Failed" message
   LONG      MaxStrLen;          // maximum string length in Debug Event listbox
   DWORD     dwActiveDebuggees;  // count of active debuggee processes
   SIZE      ClientSize;         // client windows size
   SIZE      ListBoxSize;        // debug event listbox size
   INT       ToolBarHeight;      // text button bar height
   INT       xPosOld;            // application window's old horizontal pos
   INT       yPosOld;            // application window's old vertical pos
   BOOL      fHelpUsed;          // boolean denoting online help invoked
 } GLOBAL, *PGLOBAL;

 // ***********
 // global data
 // ***********
 extern GLOBAL  Global;          // various global data
 extern PROFILE Profile;         // various profile data

#endif // DEBMAIN_H
