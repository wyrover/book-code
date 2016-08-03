/******************************************************************************
Module:  DIPSLib.h
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/


#if !defined(DIPSLIBAPI)
#define DIPSLIBAPI __declspec(dllimport)
#endif


///////////////////////////////////////////////////////////////////////////////


// External function prototypes
DIPSLIBAPI BOOL WINAPI SetDIPSHook(DWORD dwThreadId);


//////////////////////////////// End of File //////////////////////////////////
