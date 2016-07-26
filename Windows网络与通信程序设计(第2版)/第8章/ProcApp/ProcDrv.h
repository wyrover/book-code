////////////////////////////////////////////////
// ProcDrv.h文件


#define IOCTL_NTPROCDRV_GET_PROCINFO    CTL_CODE(FILE_DEVICE_UNKNOWN, \
			0x0800, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)



//	用户与内核交互的缓冲区格式，这个结构向用户程序返回进程信息
typedef struct _CallbackInfo
{
    HANDLE  hParentId;
    HANDLE  hProcessId;
    BOOLEAN bCreate;
}CALLBACK_INFO, *PCALLBACK_INFO;