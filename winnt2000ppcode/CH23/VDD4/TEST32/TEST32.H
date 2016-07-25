/********* TEST32.H - include file for test32.exe *********/

#define IDNULL                 -1
#define DISPLAY_DLG            2000
#define ID_SCSIINQ             2001
#define ID_MODELNUM            2002

/**------- DOS Device Driver Command Codes --------------**/
#define CMD_READ               4
#define CMD_WRITE              8
#define CMD_WRITE_VFY          9
#define CMD_OUT_IOCTL          12

/**------ DOS Device Driver SubCommand Codes -----------**/
#define CMD_IOCTL_READBUFFER   0x09
#define CMD_IOCTL_WRITEBUFFER  0x0A
#define CMD_IOCTL_SCSIINQ      0x0D

BOOL CALLBACK DisplayDlgProc(HWND, UINT, WPARAM, LPARAM);
ULONG APIENTRY VDDScannerCommand(USHORT, PCHAR, ULONG);
