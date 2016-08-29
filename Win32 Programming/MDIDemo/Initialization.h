#if !defined (_INITIALIZATION_H_)
#define _INITIALIZATION_H_

/* Function prototypes for initialization routines */

BOOL initInstance (HINSTANCE hInstance, UINT resPoolID, int nCmdShow) ;
int  exitInstance (MSG*) ;
ATOM internalRegisterClass (const LPWNDCLASSEX lpwcex);
ATOM registerMDIChild(HINSTANCE hinst, UINT resPoolID, WNDPROC wproc, UINT extra);
#endif  /* _INITIALIZATION_H_ */
