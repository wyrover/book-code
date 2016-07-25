// eventapp.h

typedef struct _DISK {
   DWORD  dwSPC;
   DWORD  dwBPS;
   DWORD  dwFC;
   DWORD  dwTC;
   DWORD  dwFree;
} DISK;
  
#define MAXSTR             128
#define IDC_CUSTOM         3000
#define IDC_LOGCUSTOM      3001
#define IDC_ERR            3002
#define IDC_WARN           3003
#define IDC_INFO           3004
#define IDC_LOGERR         3005
#define IDC_LOGWARN        3006
#define IDC_LOGINFO        3007
#define IDC_READ           3008
#define IDC_BACKUP         3009
#define IDC_CLEAR          3010
