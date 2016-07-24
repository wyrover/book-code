
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993-1997 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

#ifndef DEBDEBUG_H

 #define DEBDEBUG_H

 #define EXCEPTION_VDM_EVENT ( (LONG) 0x40000005L )

 //-- debuggee startup information structure
 typedef struct DEB_STARTUP_INFO_STRUCT* PDEB_STARTUP_INFO;
 typedef struct DEB_STARTUP_INFO_STRUCT {
           BOOL                fActive;
           DWORD               dwProcessId;
           LPTSTR              lpstrFileName;
           LPTSTR              lpstrPathName;
           HWND                hWndListBox;
           STARTUPINFO         StartupInfo;
           PROCESS_INFORMATION ProcessInfo;
         } DEB_STARTUP_INFO;

 //-- linked list debug list information structures
 typedef struct DEB_PROCESS_LIST_INFO_STRUCT* PDEB_PROCESS_LIST_INFO;
 typedef struct DEB_PROCESS_LIST_INFO_STRUCT {
           DWORD dwActiveProcesses;
         } DEB_PROCESS_LIST_INFO;

 typedef struct DEB_THREAD_LIST_INFO_STRUCT* PDEB_THREAD_LIST_INFO;
 typedef struct DEB_THREAD_LIST_INFO_STRUCT {
           DWORD dwActiveThreads;
         } DEB_THREAD_LIST_INFO;

 typedef struct DEB_DLL_LIST_INFO_STRUCT* PDEB_DLL_LIST_INFO;
 typedef struct DEB_DLL_LIST_INFO_STRUCT {
           DWORD dwActiveDlls;
         } DEB_DLL_LIST_INFO;

 //-- linked list debug node information structures
 typedef struct DEB_PROCESS_NODE_INFO_STRUCT* PDEB_PROCESS_NODE_INFO;
 typedef struct DEB_PROCESS_NODE_INFO_STRUCT {
           DWORD                     dwProcessId;  // sort key
           DWORD                     dwThreadId;   // first thread
           LPTSTR                    lpstrFileName;
           LPTSTR                    lpstrPathName;
           PLIST                     pThreadList;
           PLIST                     pDllList;
           CREATE_PROCESS_DEBUG_INFO ProcessDebugInfo;
         } DEB_PROCESS_NODE_INFO;

 typedef struct DEB_THREAD_NODE_INFO_STRUCT* PDEB_THREAD_NODE_INFO;
 typedef struct DEB_THREAD_NODE_INFO_STRUCT {
           DWORD                    dwProcessId;
           DWORD                    dwThreadId;  // sort key
           CREATE_THREAD_DEBUG_INFO ThreadDebugInfo;
         } DEB_THREAD_NODE_INFO;

 typedef struct DEB_DLL_NODE_INFO_STRUCT* PDEB_DLL_NODE_INFO;
 typedef struct DEB_DLL_NODE_INFO_STRUCT {
           LPTSTR              lpstrFileName;
           LPTSTR              lpstrPathName;
           LOAD_DLL_DEBUG_INFO DllDebugInfo; // sort key =  DllDebugInfo.lpBaseOfDll
         } DEB_DLL_NODE_INFO;

 //-- public function prototypes
 DWORD WINAPI DebugEventThread( PDEB_STARTUP_INFO );

#endif // DEBDEBUG_H
