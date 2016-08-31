#if !defined (_MAINFRAMETOOLBAR_H_)
#define _MAINFRAMETOOLBAR_H_

HWND mainframe_CreateToolbar (HWND hwnd) ;

BOOL Toolbar_OnNotify (HWND hwnd, int idFrom, LPNMHDR pnmhdr) ;
void Toolbar_OnNeedText (HWND hwnd, int idCtrl, LPTOOLTIPTEXT pttt) ;
void Toolbar_UpdateUI (HWND hwnd) ;

HWND mainframe_CreateStatusLine(HWND hwnd);
#endif                  /* _MAINFRAMETOOLBAR_H_ */
