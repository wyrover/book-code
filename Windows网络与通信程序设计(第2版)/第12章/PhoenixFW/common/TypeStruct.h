/////////////////////////////////////////////
// TypeStruct.h文件



#include "PMacRes.h"
#include "iocommon.h"

#ifndef __TYPESTRUCT_H__
#define __TYPESTRUCT_H__

////////////////////////////////////////////

// 用户层过滤规则
struct RULE_ITEM 
{
	TCHAR szApplication[MAX_PATH];	// 应用程序名称
	UCHAR ucAction;					// 采取的动作
	UCHAR ucDirection;				// 方向
	UCHAR ucServiceType;			// 服务类型
	USHORT usServicePort;			// 服务端口
	TCHAR sDemo[51];				// 描述
};

// 文件头结构
struct RULE_FILE_HEADER
{
	TCHAR szSignature[12];	// 文件签名
	ULONG ulHeaderLength;	// 头长度

	UCHAR ucMajorVer;		// 主版本号
	UCHAR ucMinorVer;		// 次版本号
	DWORD dwVersion;		// 文件头版本

	UCHAR ucLspWorkMode;	// 工作模式
	UCHAR ucKerWorkMode;
	UCHAR bAutoStart;		// 是否开机自动启动

	TCHAR szWebURL[MAX_PATH];	// 制造商Web页面
	TCHAR szEmail[MAX_PATH];	// 制造商Email地址

	ULONG ulLspRuleCount;		// 用户层过滤规则的个数
	ULONG ulKerRuleCount;		// 核心层过滤规则的个数
};

// 文件结构
struct RULE_FILE
{
	RULE_FILE_HEADER header;
	RULE_ITEM LspRules[MAX_RULE_COUNT];
	PassthruFilter KerRules[MAX_RULE_COUNT];
};



///////////////////////////////////
// 会话

struct SESSION 
{
	SOCKET s;
	int nProtocol;				// 使用的协议，如TCP、UDP、FTP等
	UCHAR ucDirection;			// 方向
	ULONG ulLocalIP;			// 本地IP地址
	ULONG ulRemoteIP;			// 远程IP地址
	USHORT usLocalPort;			// 本地端口号
	USHORT usRemotePort;		// 远程端口号
	TCHAR szPathName[MAX_PATH];	// 应用程序
	UCHAR ucAction;				// 对此Session的反应
	// others   如 启动时间、发送的字节数等
};


struct QUERY_SESSION  
{
	UCHAR bUsed;				// 指示此结构是否使用
	TCHAR szPathName[MAX_PATH];	// 应用程序名
	int nReturnValue;			// 主模块的返回值
};

// DLL IO控制结构
struct LSP_IO_CONTROL     // IO控制函数的参数类型
{
	UCHAR ucWorkMode;				// 工作模式
	RULE_FILE *pRuleFile;			// 规则文件

	HWND hPhoenixWnd;				// 接收LSP消息的窗口

	TCHAR szPath[MAX_PATH];			// 主程序路径

	SESSION *pSession;				// 用于取得一个Session
	int nSessionIndex;
};

typedef int  (__stdcall * PFNLSPIoControl)(LSP_IO_CONTROL *pIoControl, int nType);



















#endif // __TYPESTRUCT_H__