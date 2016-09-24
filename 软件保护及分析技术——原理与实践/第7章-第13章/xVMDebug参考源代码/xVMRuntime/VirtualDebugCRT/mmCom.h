#pragma once
#pragma pack(1)

#define HANDLE_MASK_BASE		0x30000000
#define HANDLE_PROCESS_MASK		0x10000000
#define HANDLE_THREAD_MASK		0x20000000

struct s_mm_scpenv
{
	union{
		char*	m_envs[7];
		struct 
		{
			char* m_logfile;
			char* m_crtpath;
			char* m_syspath;
			char* m_incpath;
			char* m_libpath;
			char* m_libfiles;
			char* m_scpfiles;
		};
	};
	
};

struct s_mm_config 
{
	size_t			m_szsection;		//本配置块总大小
	bool			m_virtualdll;		//是否虚拟化自身
	void*			m_ptselfcp;			//自身PE文件拷贝，用于向子进程注入
	size_t			m_szselfcp;			//自身PE文件拷贝大小
	s_mm_scpenv		m_scpenv;			//脚本运行环境
	char*			m_selfdll;			//自身模块位置，如果有
	bool			m_nowait;			//not save to config file
	s_mm_config()
		:m_virtualdll(false)
		,m_ptselfcp(0)
		,m_szselfcp(0)
		,m_selfdll(0)
		,m_nowait(false)
	{
		for (int i=0;i<sizeof(m_scpenv.m_envs)/sizeof(char*);i++)
			m_scpenv.m_envs[i] = 0;
	}
	~s_mm_config()
	{
		for (int i=0;i<sizeof(m_scpenv.m_envs)/sizeof(char*);i++)
			if (m_scpenv.m_envs[i])
				free(m_scpenv.m_envs[i]);
		if (m_selfdll)
			free(m_selfdll);
		if (m_ptselfcp)
			free(m_ptselfcp);
	}


};

struct s_mm_wrapper{
	int		m_oftexe;
	int		m_oftcmd;
	int		m_oftdir;
	void*	m_bufarg;
	int		m_szbuf;
	char	m_data[1];
};

struct s_mm_wrapret{
	int		m_szbuf;
	DWORD	m_processID;
	DWORD	m_ThreadID;
	BOOL	m_RetCode;
};


enum e_exception_result //异常处理结果
{
    XVM_VD_DBG_CONTINUE //异常已经被处理
    ,XVM_VD_DBG_EXCEPTION_NOT_HANDLED   //异常没有处理
};
enum e_cmd_type{
    XVM_VD_DBG_CONNECT, //调试器建立连接消息
    XVM_VD_DBG_ATTACH,  //调试器附加进程调试消息
    XVM_VD_DBG_EXCEPTION,   //异常消息
    XVM_VD_DBG_PRINTLOG,    //打印日志
    XVM_VD_CMD_READMEMORY,  //读取内存消息
    XVM_VD_CMD_GETTHREADCONTEXT,    //获取线程上下文消息
    XVM_VD_CMD_VIRTUALPROTECT,      //修改内存状态
    XVM_VD_CMD_VIRTUALQUERY,        //查询内存状态
    XVM_VD_CMD_QUERYINFOPROCESS,    //查询进程状态
    XVM_VD_CMD_QUERYINFOTHREAD,     //查询线程状态
    XVM_VD_CMD_WRITEMEMORY,         //写入内存数据
    XVM_VD_CMD_SETTHREADCONTEXT,    //设置线程上下文
    XVM_VD_CMD_SUSPENDTHREAD,       //暂停线程
    XVM_VD_CMD_RESUMTHREAD,         //恢复线程
    XVM_VD_CMD_CONTINUEG,           //异常继续
    XVM_VD_CMD_SETINFOTHREAD,       //设置线程信息
    XVM_VD_CMD_TERMINATETHREAD     //终止线程
};

struct s_mm_hdr{    //协议封包头
    s_mm_hdr(e_cmd_type pcmdid = XVM_VD_DBG_CONNECT){m_cmdid = pcmdid;}
	e_cmd_type	m_cmdid;
};

struct s_mm_dbg_connect : public s_mm_hdr{//协议封包具体结构
    s_mm_dbg_connect():s_mm_hdr(XVM_VD_DBG_CONNECT){}
    bool			m_RetCode;          //返回代码
    unsigned char	m_int3cmd;			//int3替代指令	0xF4
    unsigned long	m_int3ecode;		//int3替代指令异常编号0xC0000096
	unsigned char	m_stepcmd;			//单步替代指令
    unsigned long	m_stepecode;		//单步替代指令异常编号
    TCHAR			m_dbgPipe[32];      //通信管道名称
};

struct s_mm_dbg_event : public s_mm_hdr{
    s_mm_dbg_event():s_mm_hdr(XVM_VD_DBG_EXCEPTION),m_Invoked(false){
		memset(&m_dbgEvent,0,sizeof(m_dbgEvent));
	}
    bool				m_Invoked;  //事件是否已经触发过
    e_exception_result	m_result;   //异常处理结果
    DEBUG_EVENT			m_dbgEvent; //调试事件结构
};

struct s_mm_dbg_printlog : public s_mm_hdr{
    s_mm_dbg_printlog():s_mm_hdr(XVM_VD_DBG_PRINTLOG){}
    unsigned long	m_addr;
	int				m_color;
	unsigned int	m_strlen;
	char			m_strbuf[1];
};

struct s_mm_dbg_readmemory : public s_mm_hdr{   //读取内存
    s_mm_dbg_readmemory():s_mm_hdr(XVM_VD_CMD_READMEMORY){}
    BOOL	m_bRet;     //返回状态
    void*	m_lpAddr;   //读取内存地址
    ULONG	m_szRead;   //请求读取大小
    ULONG	m_szReaded; //最终读取到大小
};

struct s_mm_dbg_getcontext : public s_mm_hdr{
    s_mm_dbg_getcontext():s_mm_hdr(XVM_VD_CMD_GETTHREADCONTEXT){}
    DWORD	m_ThreadID; //线程ID
    BOOL	m_bRet;     //结果
};

struct s_mm_dbg_virprotect : public s_mm_hdr{
    s_mm_dbg_virprotect():s_mm_hdr(XVM_VD_CMD_VIRTUALPROTECT){}
	BOOL	RetCode;
	LPVOID	lpAddress;
	SIZE_T	dwSize;
	DWORD	flNewProtect;
	DWORD	lpflOldProtect;
};

struct s_mm_dbg_virquery : public s_mm_hdr{
    s_mm_dbg_virquery():s_mm_hdr(XVM_VD_CMD_VIRTUALQUERY){}
	SIZE_T	RetCode;
	LPCVOID lpAddress;
	MEMORY_BASIC_INFORMATION lpBuffer;
	SIZE_T	dwLength;
};

struct s_mm_dbg_queryinfops : public s_mm_hdr{
    s_mm_dbg_queryinfops():s_mm_hdr(XVM_VD_CMD_QUERYINFOPROCESS){}
	LONG	RetCode;
	int		ProcessInfoClass;
	PVOID	ProcessInfoBuffer;
	ULONG	ProcessInfoBufferLength;
	PULONG	BytesReturned;
};
struct s_mm_dbg_queryinfotd : public s_mm_hdr{
    s_mm_dbg_queryinfotd():s_mm_hdr(XVM_VD_CMD_QUERYINFOTHREAD){}
	LONG	RetCode;
	HANDLE	hThread;
	int		ThreadInfoClass;
	PVOID	ThreadInfoBuffer;
	ULONG	ThreadInfoBufferLength;
	ULONG	BytesReturned;
};

struct s_mm_dbg_setcontext : public s_mm_hdr{
    s_mm_dbg_setcontext():s_mm_hdr(XVM_VD_CMD_SETTHREADCONTEXT){}
	DWORD	m_ThreadID;
	CONTEXT	m_context;
};

struct s_mm_dbg_suspendtd : public s_mm_hdr{
    s_mm_dbg_suspendtd():s_mm_hdr(XVM_VD_CMD_SUSPENDTHREAD){}
	DWORD	m_ThreadID;
	DWORD	m_RetCode;
};

struct s_mm_dbg_resumtd : public s_mm_hdr{
    s_mm_dbg_resumtd():s_mm_hdr(XVM_VD_CMD_RESUMTHREAD){}
	DWORD	m_ThreadID;
	DWORD	m_RetCode;
};

struct s_mm_dbg_writemem : public s_mm_hdr{
    s_mm_dbg_writemem():s_mm_hdr(XVM_VD_CMD_WRITEMEMORY){}
	BOOL	m_RetCode;
	PVOID	BaseAddress;
	PVOID	Buffer;
	ULONG	BytesToWrite;
	ULONG	BytesWritten;
};

struct s_mm_dbg_setinfotd : public s_mm_hdr{
    s_mm_dbg_setinfotd():s_mm_hdr(XVM_VD_CMD_SETINFOTHREAD){}
	LONG	m_RetCode;
	HANDLE	hThread;
	int		ThreadInfoClass;
	PVOID	ThreadInfoBuffer;
	ULONG	ThreadInfoBufferLength;
};

struct s_mm_dbg_continueg : public s_mm_hdr{
    s_mm_dbg_continueg():s_mm_hdr(XVM_VD_CMD_CONTINUEG){}
};

struct s_mm_dbg_termthread : public s_mm_hdr{
    s_mm_dbg_termthread():s_mm_hdr(XVM_VD_CMD_TERMINATETHREAD){}
	DWORD	m_ThreadID;
	DWORD	m_RetCode;
	DWORD   m_dwExitCode;
};



struct s_e_flags{
	union{
		DWORD	m_eflags;
		struct{
			bool m_f_CF:1;
			bool m_un02:1;
			bool m_f_PF:1;
			bool m_un04:1;
			bool m_f_AF:1;
			bool m_un06:1;
			bool m_f_ZF:1;
			bool m_f_SF:1;
			bool m_f_TF:1;
			bool m_f_IF:1;
			bool m_f_DF:1;
			bool m_f_OF:1;
			bool m_un13:1;
			bool m_un14:1;
			bool m_f_NT:1;
			bool m_un16:1;
			bool m_f_RF:1;
			bool m_f_VM:1;
			bool m_un19:1;
			bool m_un20:1;
			bool m_un21:1;
			bool m_un22:1;
			bool m_un23:1;
			bool m_un24:1;
			bool m_un25:1;
			bool m_un26:1;
			bool m_un27:1;
			bool m_un28:1;
			bool m_un29:1;
			bool m_un30:1;
			bool m_un31:1;
			bool m_un32:1;
		};
	};
};


#pragma pack()


extern int		loadmmconfig(s_mm_config* mmcfg, void* pbuf);
extern char*	bufmmconfig(s_mm_config* mmcfg, int& pszbuf, const char* pcurdir = 0);
extern int		savemmconfigxml(s_mm_config* pmmconfig,const char* pxmlfile);
extern int		loadmmconfigxml(s_mm_config* pmmconfig,const char* pxmlfile);
