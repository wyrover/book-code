#if !defined (_MAINFRAME_H_)
#define _MAINFRAME_H_

extern HWND   hMainFrame;
extern HMENU  hMainMenu;
extern HACCEL hMainAccel;
extern HACCEL haccel; // current accelerator table

LRESULT CALLBACK mainFrameWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) ;

#endif                  /* _MAINFRAME_H_ */
