#if !defined (_DISPLAYMESSAGE_H_)
#define _DISPLAYMESSAGE_H_

VOID mainFrame_DisplayMessage (HWND hwnd, UINT wId, WPARAM wParam, LPARAM lParam) ;
void mainFrame_ComputeMessageWidths(HWND hwnd);

extern int mainFrame_cyPixels ;      /* Height (device units)  */
extern int mainFrame_cyChar ;        /* Height (logical units) */

#endif                  /* _DISPLAYMESSAGE_H_ */
