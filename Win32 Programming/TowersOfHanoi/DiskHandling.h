#if !defined (_DISKHANDLING_H_)
#define _DISKHANDLING_H_

//
// Function prototypes for callback functions
//

LRESULT CALLBACK diskWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) ;

//
// Function prototypes for global functions
//

void disk_PositionDiskWindow (HWND hwndDisk) ;
void disk_MoveTower (UINT cDisks, UINT nFromPeg, UINT nAuxPeg, UINT nToPeg) ;

#endif                  /* _DISKHANDLING_H_ */
