
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993-1997 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/*****************************************************************************\
*
* Module: spy.h
*
*   Main include file for the Windows debugging Spy SDK applet.
*
\*****************************************************************************/

#include <windows.h>

#ifdef  FE_IME
#include <ime.h>
#endif

#include "dialogs.h"
#include "..\hook.h"


#define MAXSTRING                   MAX_PATH

#define LINES_MAX                   250         // Max lines allowed in window.


//
// Message Table Flags
//
#define MTF_SELECTED                0x00000001

#define MTF_TYPE_MOUSE              0x00010000
#define MTF_TYPE_DDE                0x00020000
#define MTF_TYPE_CLIP               0x00040000
#define MTF_TYPE_NC                 0x00080000
#define MTF_TYPE_KEYBD              0x00100000
#define MTF_TYPE_LB                 0x00200000
#define MTF_TYPE_BM                 0x00400000
#define MTF_TYPE_STM                0x00800000
#define MTF_TYPE_CB                 0x01000000
#define MTF_TYPE_EM                 0x02000000
#ifdef FE_IME
#define MTF_TYPE_IME                0x04000000
#endif


//
// Menu ids.
//

#define MENU_SPY_SELECTWINDOW       1000
#define MENU_SPY_ABOUT              1001
#define MENU_SPY_EXIT               1002

#define MENU_EDIT_CUT               1100
#define MENU_EDIT_COPY              1101
#define MENU_EDIT_CLEAR             1102

#define MENU_OPTIONS_MESSAGES       1200
#define MENU_OPTIONS_FONT           1201
#define MENU_OPTIONS_OUTPUT         1202

#define MENU_START                  1300
#define MENU_STOP                   1301

//
// Resource file constants
//
#define IDS_APPLICATION_NAME        2000
#define IDS_MENU_START              2001
#define IDS_MENU_STOP               2002
#define IDS_PARENT                  2003
#define IDS_NOPARENT                2004
#define IDS_UNDEFINED               2005
#define IDS_ERROR_WND_LINE          2006
#define IDS_ERROR_CANT_OPEN_FILE    2007
#define IDS_ERROR_CANT_OPEN_COM1    2008
#define IDS_ERROR_CANT_LOAD_DLL     2009

//
// Position of the top level Start/Stop menu item.
//
#define MENUPOS_STARTSTOP           3


//
// Prototype for the Message Decode functions.
//
typedef VOID (*LPFNDECODE)(HWND, UINT, PSPYMSGDATA);


//
// The Message Description Table structure that describes each message
// that Spy understands.
//
typedef struct
{
    LPTSTR pszMsg;
    UINT msg;
    UINT Flags;
    LPFNDECODE pfnDecode;
} MSGDESC, *PMSGDESC;


//
// The message group structure.  A table of these structures describes
// each different group of messages that can be selected/deselected
// for spying on using the Messages dialog.
//
typedef struct
{
    INT idCheckBox;
    DWORD flMask;
    INT cMsgs;
    INT cUseCount;
} MSGGROUP, *PMSGGROUP;


extern HANDLE ghInst;
extern HWND ghwndSpyApp;
extern HWND ghwndPrintf;
extern HANDLE ghHookThread;
extern HWND ghwndSpyHook;
extern HWND ghwndSpyingOn;              // The window we are spying on.
extern HFONT ghfontPrintf;
extern INT gnLines;
extern BOOL gfSpyOn;
extern BOOL gfSpyAll;
extern BOOL gfOutputWin;
extern BOOL gfOutputCom1;
extern BOOL gfOutputFile;
extern HFILE gfhFile;
extern HANDLE gfhCom1;
extern CHAR gszFile[];
extern INT gcxBorder;
extern INT gcyBorder;
extern BOOL gfMsgsUser;
extern BOOL gfMsgsUnknown;
extern CHAR gszAppName[];
extern UCHAR gszWindowName[];
extern WINDOWPLACEMENT gwndpl;

extern MSGDESC gaMsgs[];
extern INT gcMessages;
extern MSGGROUP gaMsgGroup[];
extern INT gcMsgGroups;
extern INT gaiSelected[];
extern INT gaiSelected2[];


#include "spyfuncs.h"
#include "wprintf.h"

#ifdef FE_IME
#include <ime.h>               //  For WM_IME_REPORT
#include <winuser.h>           //  For WM_IMEKEYDOWN, WM_IMEKEYUP
#endif
