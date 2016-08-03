/******************************************************************************
Module:  LastMsgBoxInfoLib.h
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/


#ifndef LASTMSGBOXINFOLIBAPI 
#define LASTMSGBOXINFOLIBAPI extern "C" __declspec(dllimport)
#endif


///////////////////////////////////////////////////////////////////////////////


LASTMSGBOXINFOLIBAPI BOOL WINAPI LastMsgBoxInfo_HookAllApps(BOOL bInstall, 
   DWORD dwThreadId);


//////////////////////////////// End of File //////////////////////////////////
