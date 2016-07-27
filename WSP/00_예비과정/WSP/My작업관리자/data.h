// data.h

// 사용자 정의 메시지 
#define WM_TRAYMESSAGE WM_USER + 100

#define IDC_BTNREFRESH			1
#define IDC_BTNTERMINATEPROCESS	2
#define IDC_BTNSPY				3
#define IDC_LOG					4

struct MEMBER
{
	char name[20];
	char uni[20];
	char maj[20];
	char num[20];
	char phone[20];

	MEMBER() {}
	MEMBER(char *n, char*u, char*ma, char*nu, char*ph)
	{
		strcpy(name, n);		strcpy(uni, u);
		strcpy(maj, ma);		strcpy(num, nu);
		strcpy(phone, ph);
	}
};
	   
extern HWND hMainList;
extern HWND hBtnRefresh, hBtnTerminateProcess, hBtnSpy;
extern HWND hStaticLog;


typedef struct _TDATA
{
	HWND	hDlg;			// 주 스레드가 만든 Dialog핸들
	char	exename[256];	// process 이름
	DWORD	pid;			// process ID
	int		idx;			// 선택한 리스트뷰의 idx
}TDATA;


extern HANDLE	hThreadList[20];	// 생성된 thread의 핸들
extern int		ThreadCount;
extern HANDLE	hExitEvent;


