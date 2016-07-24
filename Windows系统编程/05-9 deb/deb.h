
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993-1997 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

#ifndef DEB_H

#define DEB_H

 //-- menu ids
 #define IDM_FILE_OPEN               1101
 #define IDM_FILE_ATTACH             1102
 #define IDM_FILE_EXIT               1109

 #define IDM_EDIT_CUT                1201
 #define IDM_EDIT_COPY               1202
 #define IDM_EDIT_DELETE             1203

 #define IDM_OPTIONS_FONT            1701
 #define IDM_OPTIONS_PREFERENCES     1702
 #define IDM_OPTIONS_TOOLBAR         1703
 #define IDM_OPTIONS_SAVEDDIR        1704
 #define IDM_OPTIONS_COLOR           1705
 #define IDM_OPTIONS_SAVEONEXIT      1798
 #define IDM_OPTIONS_SAVENOW         1799

 #define IDM_HELP_CONTENTS           1901
 #define IDM_HELP_SEARCH             1902
 #define IDM_HELP_HOWTOUSE           1903
 #define IDM_HELP_ABOUT              1909

 //-- string ids
 #define IDS_APPNAME                 2000
 #define IDS_SHORT_APPNAME           2001

 #define IDS_OFN_TITLE               2010
 #define IDS_OFN_FILTERNAME          2011
 #define IDS_OFN_FILTER              2012
 #define IDS_OFN_DEBUGGEE_TITLE      2013

 #define IDS_BUTTON_OPEN             2020
 #define IDS_BUTTON_ATTACH           2021
 #define IDS_BUTTON_CUT              2022
 #define IDS_BUTTON_COPY             2023
 #define IDS_BUTTON_DELETE           2024
 #define IDS_BUTTON_HELP             2025

 #define IDS_EXIT_BOX_TITLE          2030
 #define IDS_EXIT_BOX_TEXT           2031

 #define IDS_DEBUG_EVENTS            2040

 #define IDS_API_FAILED_MSG             9001
 #define IDS_WINDOWS_NT_REQUIRED_TITLE  9998
 #define IDS_WINDOWS_NT_REQUIRED        9999

 //-- control ids
 #define IDC_INITIALDIR              9001
 #define IDC_PROCESSLIST             9002
 #define IDC_DEBUG_ONLY_THIS_PROCESS 9003
 #define IDC_DEBUG_PROCESS           9004
 #define IDC_IDLE_PRIORITY_CLASS     9005
 #define IDC_NORMAL_PRIORITY_CLASS   9006
 #define IDC_HIGH_PRIORITY_CLASS     9007
 #define IDC_REALTIME_PRIORITY_CLASS 9008
 #define IDC_CLEAR_ON_NEW            9009
 #define IDC_VERBOSE                 9010
 #define IDC_SHOW_SYMBOLS            9011
 #define IDC_DEBUG_MODE_NONE         9020
 #define IDC_DEBUG_MODE_ERROR        9021
 #define IDC_DEBUG_MODE_MINORERROR   9022
 #define IDC_DEBUG_MODE_WARNING      9023
 #define IDC_SAVE_PREFERENCES        9999

// now defined in winuser.h ... #define IDHELP  9

 //-- user messages
 #ifndef WM_APP
  #define WM_APP 0x8000
 #endif
 #define UM_CREATE_LISTBOX (WM_APP+0)

#endif // DEB_H
