#if !defined (_INITIALIZATION_H_)
#define _INITIALIZATION_H_

/* Function prototypes for initialization routines */

BOOL initInstance (HINSTANCE hInstance, UINT resPoolID, int nCmdShow) ;
int  exitInstance (MSG*) ;
ATOM internalRegisterClass (const LPWNDCLASSEX lpwcex);

#endif  /* _INITIALIZATION_H_ */
