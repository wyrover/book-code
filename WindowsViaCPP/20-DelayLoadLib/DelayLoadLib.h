/******************************************************************************
Module:  DelayLoadLib.h
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/


#ifndef DELAYLOADLIBAPI 
#define DELAYLOADLIBAPI extern "C" __declspec(dllimport)
#endif


///////////////////////////////////////////////////////////////////////////////


DELAYLOADLIBAPI int fnLib();
DELAYLOADLIBAPI int fnLib2();


//////////////////////////////// End of File //////////////////////////////////
