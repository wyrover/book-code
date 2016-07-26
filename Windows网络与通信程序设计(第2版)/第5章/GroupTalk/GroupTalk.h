/////////////////////////////////////////
// GroupTalk.h


#include "initsock.h"


#define WM_GROUPTALK	WM_USER + 105    

/*

  uMsg:		WM_GROUPTALK
  wParam:	错误代码，0表示没有错误
  lParam:	消息的GT_HDR头指针

*/


#define BUFFER_SIZE 4096
#define GROUP_PORT  4567


const enum
{
	MT_JION = 1,	// 一个用户加入
	MT_LEAVE,		// 一个用户离开
	MT_MESG,		// 一个用户发送消息

	// 内部使用，告诉新加入的用户自己的用户信息
	MT_MINE
};

typedef struct gt_hdr
{
	u_char gt_type;			// 消息类型
	DWORD dwAddr;			// 发送此消息的用户的IP地址
	char szUser[15];		// 发送此消息的用户的用户名

	int nDataLength;		// 后面数据的长度
	char *data() { return (char*)(this + 1); }
} GT_HDR;



class CGroupTalk
{
public:
	// 构造函数，创建工作线程，加入会议组
	CGroupTalk(HWND hNotifyWnd, DWORD dwMultiAddr, DWORD dwLocalAddr = INADDR_ANY, int nTTL = 64);
	// 析构函数，清理资源，离开会议组
	~CGroupTalk();

	// 向其它成员发送消息。dwRemoteAddr为目标成员的地址，如果为0则向所有成员发送
	BOOL SendText(char *szText, int nLen, DWORD dwRemoteAddr = 0);

protected:
		// 帮助函数
	// 加入一个多播组
	BOOL JoinGroup();
	// 离开一个多播组
	BOOL LeaveGroup();
	// 向指定地址发送UDP封包
	BOOL Send(char *szText, int nLen, DWORD dwRemoteAddr);

protected:
		// 具体实现
	// 处理到来的封包
	void DispatchMsg(GT_HDR *pHeader, int nLen);
	// 工作线程
	friend DWORD WINAPI _GroupTalkEntry(LPVOID lpParam); 	CInitSock theSock;

	HWND m_hNotifyWnd;		// 主窗口句柄
	DWORD m_dwMultiAddr;	// 此组使用的多播地址
	DWORD m_dwLocalAddr;	// 用户要使用的本地接口
	int m_nTTL;				// 多播封包的TTL值
	HANDLE m_hThread;		// 工作线程句柄
	HANDLE m_hEvent;		// 事件句柄，用来使用重叠I/O接收数据

	SOCKET m_sRead;			// 接收数据的套节字，它必须加入多播组
	SOCKET m_sSend;			// 发送数据的套节字，不必加入多播组

	BOOL m_bQuit;			// 用来通知工作线程退出
	
	char m_szUser[256];		// 用户名
};




/*

  可以使用下面的结构
#define MAX_EXPIRED 1000*60*5

struct USER_INFO
{
	DWORD dwAddr;
	char szUser[15];
	DWORD dwLastActiveTime;
};

*/