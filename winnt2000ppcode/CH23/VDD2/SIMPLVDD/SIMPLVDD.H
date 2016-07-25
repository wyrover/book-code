// SIMPLVDD.H - Include file for SIMPLVDD.C

// DOS Device Driver status codes
#define STAT_OK          0x0000   // SUCCESS 
#define STAT_CE          0x8003   // invalid command 
#define STAT_GF          0x800C   // general failure 

// DOS Device Driver command codes
#define CMD_IN_IOCTL     3    // input I/O control 
#define CMD_READ         4    // read command  
#define CMD_WRITE        8    // write command 
#define CMD_WRITE_VFY    9    // write with verify 

// SimplDrv information
#define FILE_DEVICE_SIMPLDRV     0x00008300
#define SIMPLDRV_IOCTL_INDEX     0x830
#define IOCTL_SIMPLDRV_HELLO    CTL_CODE(FILE_DEVICE_SIMPLDRV, \
   SIMPLDRV_IOCTL_INDEX, METHOD_BUFFERED, FILE_ANY_ACCESS)

// end of file
