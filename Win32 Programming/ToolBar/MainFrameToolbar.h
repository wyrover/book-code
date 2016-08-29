#if !defined (_MAINFRAMETOOLBAR_H_)
#define _MAINFRAMETOOLBAR_H_

HWND mainFrame_CreateToolbar (HWND hwnd) ;

BOOL toolbar_OnNotify (HWND hwnd, int idFrom, LPNMHDR pnmhdr) ;
void toolbar_OnNeedText (HWND hwnd, int idCtrl, LPTOOLTIPTEXT pttt) ;
void toolbar_UpdateUI (HWND hwnd) ;

#endif                  /* _MAINFRAMETOOLBAR_H_ */
