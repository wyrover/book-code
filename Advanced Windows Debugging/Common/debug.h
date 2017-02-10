/*++
Copyright (c) Advanced Windows Debugging (ISBN 0321374460) from Addison-Wesley Professional.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/
#define STOP_ON_DEBUGGER { if (IsDebuggerPresent()) DebugBreak();}
#define TRACE(a) wprintf(a); wprintf  (L"Last error = %d\n", GetLastError());
#define TRACE_ERR(a,err) wprintf(a); wprintf  (L"Last error = %d\n", err);
