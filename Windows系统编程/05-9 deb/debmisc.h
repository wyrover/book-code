
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993-1997 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/


#ifndef DEBMISC_H

 #define DEBMISC_H

 #ifndef DEBDEBUG_H
   #include "DEBDebug.H"
 #endif

 #ifndef DEBMAIN_H
   #include "DEBMain.H"
 #endif

 //-- function prototypes
 BOOL          StartDebuggee( LPTSTR, HWND );
 BOOL          AttachToDebuggee( DWORD, HWND );
 BOOL CALLBACK EnumProcessListFunc( HWND, LPARAM );
 BOOL          GetDebuggeeFileName( LPTSTR, HWND );
 BOOL          ChooseNewFont( HWND );
 BOOL          ChooseNewBackColor( HWND );
 BOOL          MakeCommonDebugEventString( LPTSTR, LPDEBUG_EVENT);
 HWND          CreateTextButtonBar( HWND, LPINT );
 HWND          CreateIconWindow( HWND, LPCTSTR );
 BOOL          GetPrivateProfileSettings( LPCTSTR, LPCTSTR, PPROFILE );
 BOOL          WritePrivateProfileSettings( LPCTSTR, LPCTSTR, PPROFILE );
 BOOL          WritePrivateProfileInt( LPCTSTR, LPCTSTR, INT, LPCTSTR );
 BOOL          UpdateMenuSettings( HWND );
 BOOL          OutOfMemoryMessageBox( HWND );
 BOOL          MaxDebuggeesMessageBox( HWND );
 BOOL          ErrorMessageBox( LPCTSTR, LPCTSTR, LPCTSTR, INT );
 WNDPROC       SubclassWindow( HWND, WNDPROC );
 BOOL          SendWmSizeMessage( HWND );
 UINT          GetPathFromFullPathName( LPCTSTR, LPTSTR, UINT );
 BOOL          CopyListBoxToClipboard( HWND, LONG );
 LONG          ListBoxInsert( HWND, LPLONG, LPCTSTR );
 LONG          ListBoxPrintF( HWND, LPCTSTR, ... );
 LPTSTR        StringPrintF( LPTSTR, LPCTSTR, ...  );
 BOOL          StringAppendF( LPTSTR, LPCTSTR, ... );

#endif // DEBMISC_H
