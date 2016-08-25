/******************************************************************************
Module:  LastMsgBoxInfoLib.h
Notices: Copyright (c) 2000 Jeffrey Richter
******************************************************************************/


#ifndef LASTMSGBOXINFOLIBAPI 
#define LASTMSGBOXINFOLIBAPI extern "C" __declspec(dllimport)
#endif


///////////////////////////////////////////////////////////////////////////////


LASTMSGBOXINFOLIBAPI BOOL WINAPI LastMsgBoxInfo_HookAllApps(BOOL fInstall, 
   DWORD dwThreadId);


//////////////////////////////// End of File //////////////////////////////////
