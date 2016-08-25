/******************************************************************************
Module:  DelayLoadLib.h
Notices: Copyright (c) 2000 Jeffrey Richter
******************************************************************************/


#ifndef DELAYLOADLIBAPI 
#define DELAYLOADLIBAPI extern "C" __declspec(dllimport)
#endif


///////////////////////////////////////////////////////////////////////////////


DELAYLOADLIBAPI int fnLib();
DELAYLOADLIBAPI int fnLib2();


//////////////////////////////// End of File //////////////////////////////////
