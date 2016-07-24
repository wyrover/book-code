
#ifndef _SKIN_H
#define _SKIN_H

#include <windows.h>
#include <stdio.h>
#include "..\\build\\res\\resource.h"
#include "voip.h"
/*
 * Defines
 */

#define LOGO_WIDTH  265
#define LOGO_HEIGHT 160

/*
 * ENUMS for ACTIONS
 *
 */

enum {

	ACTION_NONE,
	ACTION_CLOSE,
	ACTION_MINIMIZE,
	ACTION_MENU,
	ACTION_REWIND,
	ACTION_STOP,
	ACTION_PLAY,
	ACTION_PAUSE,
	ACTION_FORWARD,
	ACTION_LOAD,
	ACTION_MOVING,
	ACTION_RESIZE,
	ACTION_VOLUME_BAR,
	ACTION_VOLUME_CURSOR,
	ACTION_PROGRESS_BAR,
	ACTION_PROGRESS_CURSOR,
	ACTION_PLAY_AREA
};

/*
 * Skin Class, load and 
 * display skin into window.
 *
 */

class Skin {

private:

	HWND    hwnd;

	HBITMAP skin;
	HBITMAP logo;
	HBITMAP bottom;
	HBITMAP tmp;

	COLORREF skinColor;
	DWORD    compactMode;

	RECT    menuButton;
	RECT    minimizeButton;
	RECT    closeButton;
	RECT    resizeButton;
	RECT    videoRect;
	HANDLE  dMutex;

	HFONT   font;
		
	BOOL isInsideRect(int x, int y, RECT rect);


public:

	Skin(HINSTANCE hIstance, HWND hwnd);
	~Skin();

	int   GetDefaultWidth();
	int   GetDefaultHeight();

	RECT *GetVideoRect();

	int   LoadDefault(HINSTANCE hInst, HWND hwnd);

	int   Display(HWND hwnd);
	int   UpdateSize(HWND hwnd);

	int   SetCompact(int compact);
	int   SetActionStart(HWND hwnd, int action);
	int   SetActionEnd(HWND hwnd,int action);
	int   GetAction(int x, int y);

};

#endif