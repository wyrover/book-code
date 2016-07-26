/***
*wincrt0.c - C runtime Windows EXE start-up routine
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This is the actual startup routine for Windows apps.  It calls the
*       user's main routine WinMain() after performing C Run-Time Library
*       initialization.
*
*******************************************************************************/


#define _WINMAIN_
#include "crt0.c"

