//data.h

//1.생성하고자 하는 서비스 이름
#define SERVICE_NAME "MemStat"

#define SERVICE_CONTROL_NEWFILE 128

extern SERVICE_STATUS_HANDLE	g_hSrv;
extern DWORD	g_NowState;
extern BOOL		g_bPause;
extern HANDLE	g_ExitEvent;
