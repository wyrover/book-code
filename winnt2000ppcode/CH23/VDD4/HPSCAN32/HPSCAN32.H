/**--------------------------------------------------**
** HPSCAN32.H:	HP Scanner Application-based VDD.
** Environment: Windows NT.
** (C) Hewlett-Packard Company 1993.
**--------------------------------------------------**/

/**-------- DOS Device Driver Status Codes ---------**/
#define STAT_OK          0x0000   /* SUCCESS */
#define STAT_CE          0x8003   /* invalid command */
#define STAT_GF          0x800C   /* general failure */

/**------- DOS Device Driver Command Codes ---------**/
#define CMD_READ         4    /* read command */ 
#define CMD_WRITE        8    /* write command */
#define CMD_WRITE_VFY    9    /* write with verify */
#define CMD_OUT_IOCTL    12   /* output I/O control */

/**------ DOS Device Driver SubCommand Codes -------**/
#define CMD_IOCTL_READBUFFER   0x09
#define CMD_IOCTL_WRITEBUFFER  0x0A
#define CMD_IOCTL_SCSIINQ      0x0D

/**---------- PASS_THROUGH_STRUCT ------------------**/
typedef struct 
{
    SCSI_PASS_THROUGH sptCmd;
    UCHAR             ucSenseBuf[32];
    UCHAR             ucDataBuf[65536];      /* big! */
} PASS_THROUGH_STRUCT;

/**---- IOCTL Structure from the DOS Stub driver ---**/
typedef struct
{
    USHORT Command;
    USHORT Status;
    USHORT Count;
    USHORT Offset;
    USHORT Segment;	
} HPSCAN_IOCTL;
typedef HPSCAN_IOCTL *PHPSCAN_IOCTL;

/**-------- HPSCAN32.C, private prototypes ---------**/
HANDLE HPScannerOpen(VOID);
BOOL HPScannerClose(HANDLE);
ULONG HPScannerRead(HANDLE, PCHAR, ULONG);
ULONG HPScannerWrite(HANDLE, PCHAR, ULONG);
ULONG HPScannerIOCTL(HANDLE, USHORT, PCHAR, ULONG);

/**-------- HPSCAN32.C, public prototypes ----------**/
BOOL VDDLibMain(HINSTANCE, ULONG, LPVOID);
VOID VDDInit(VOID);
VOID VDDDispatch(VOID);
ULONG APIENTRY VDDScannerCommand(USHORT, PCHAR, ULONG);

