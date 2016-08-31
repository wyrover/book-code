#if !defined (_TOWERS_OF_HANOI_H_)
#define _TOWERS_OF_HANOI_H_

/* Function prototypes for application routines */

#define NUMDISKS                   5

#define POSTWIDTH                  64
#define POSTHEIGHT                 ((NUMDISKS + 1) * 2 * DISKHEIGHT)

#define DISKWIDTHUNIT              (POSTWIDTH/2)
#define DISKHEIGHT                 64

#define LEFTPOSTPOS                -640
#define CENTERPOSTPOS              0
#define RIGHTPOSTPOS               640

#define FIRST                      0
#define SECOND                     1
#define THIRD                      2

// Terminate application application-defined exception code
#define STATUS_TERMINATION_REQUEST    0x60000001

extern  HWND            WhosOn [THIRD + 1][NUMDISKS] ;
extern  WORD            HowMany [THIRD + 1] ;

/* Window extra area field(s) */

#define UGWW_POSTNUM                 0
#define UGW_DWMAXUSED                (UGWW_POSTNUM + sizeof (WORD))

#define disk_GetDiskSize(w)   (GetWindowLong (w, GWL_ID))

#define disk_GetPostNum(w)    (GetWindowWord (w, UGWW_POSTNUM))
#define disk_SetPostNum(w,p)  (SetWindowWord (w, UGWW_POSTNUM, (WORD)(p)))

#endif          /* _TOWERS_OF_HANOI_H_ */
