#ifndef XVMRUNTIME_H
#define XVMRUNTIME_H

//命名管道名称
#define XVMRUNTIME_COM_PIPENAME "XVMRuntimePipe"
#define XVMRUNTIME_COM_PIPENAME_FULL "\\\\.\\Pipe\\XVMRuntimePipe"

#define XVMRUNTIME_COM_NOTICEPIPE	_T("\\\\.\\pipe\\xVMNotice%04X")

#define XVMRUNTIME_RAISE_STR	L"XVM_RAISEEXCEPTION"
#define MSG_PACKET_SZINFO_LEN  2
#define MSG_PACKET_SIZE_MAXIMUM 0x10000

#pragma pack(push,1)

struct syscIds{
	int iNtQueryInformationProcess;
	int iNtSetInformationThread;
	int iNtQuerySystemInformation;
	int iNtQueryObject;
	int iNtClose;
	int iNtSetContextThread;
	int iFindowWindow;
	int iNtSetInformationProcess;
};

typedef struct xvmrutime_args
{
    BOOL bEnableHeapHook;
    BOOL bBreakAtOEP;
    BOOL bVirtualDebugMode;
    BOOL bSandbox;
	BOOL bAntiAntiDebug;
	BOOL bBreakOnLdr;
	BOOL bEnableClientPipe;
	BOOL bEnableAlertPipe;
	BOOL bDisableSafeSEH;
	BOOL bEnableVirtualKernel32;
	DWORD hostProcessID;
	syscIds sysc;
	int		sdllmode;
	wchar_t	startupDLL[MAX_PATH];
	int		szscppk;
	char startscppk[1024];
}xvmrutime_args;

#pragma pack(pop)

#define xvm_api_flag_breakpoint 0x1

//从xVMRuntime32模块发送到监视程式xApiSpy的消息定义
enum xvm_msg_reqs{
    xvm_req_init = 0,       //目标进程已初始化，通知xApiSpy并给予暂停机会
    xvm_req_modulefilter,   //模块过滤消息，请求xApiSpy是否监视该模块函数
    xvm_req_apifilter,      //函数过滤消息，请求xApiSpy是否监视该函数
    xvm_req_modapis,        //向xApiSpy发送模块函数信息，即导出表信息
    xvm_req_modapinodes,    //向xApiSpy发送具体的模块函数定义
    xvm_req_apicall,        //模块函数被调用，通知xApiSpy并请求处理
    xvm_req_testflags,       //向xApiSpy请求参数设定
    xvm_req_oepcall,
    xvm_req_vmexecfinished,
    xvm_req_resnode,        //向xApiSpy模块发送资源数据

    xvm_req_memblock,       //向xApiSpy程式发送内存区块信息
    xvm_req_memdata,        //向xApiSpy程式发送内存实际数据
    xvm_req_injectmodule    //向xApiSpy程式发送模块注入结果

    ,xvm_req_syncbreakpoints//向xApiSpy程式发送断点同步信息


};

//从xApiSpy发送到xVMRuntime32模块的消息定义
enum xvm_msg_acks{
    xvm_ack_init = 0,       //回复xVMRuntime32的请求，并继续目标进程的运行
    xvm_ack_modulefilter,   //回复xVMRuntime32的模块过滤请求
    xvm_ack_apifilter,      //回复xVMRuntime32的函数过滤请求
    xvm_ack_apicall,        //回复xVMRuntime32的函数调用通知
    xvm_ack_testflags,
    xvm_ack_continue,
    xvm_ack_break,
    xvm_ack_loopbreak,
    xvm_ack_entervm,
    xvm_ack_doaction,

    xvm_ack_spyrsrc,

    xvm_ack_takememsnapshot,    //请求xVMRuntime模块抓取目标进程内存快照
    xvm_ack_injectmodule,        //请求xVMRuntime模块加载指定模块

    xvm_ack_syncbreakpoints

    ,xvm_ack_takesnapshot       //请求xVMRuntime模块拍摄进程快照
    ,xvm_ack_setapibp
    ,xvm_ack_delapibp
};

enum xvm_msg_notice{
	xvm_notice_init = 0,
	xvm_notice_runscript,
	xvm_notice_scp_error,
	xvm_notice_entervm,
	xvm_notice_outvm,
	xvm_notice_echo,
	xvm_notice_blocks,
	xvm_notice_records,
	xvm_notice_raiseexception,
	xvm_notice_bklogs,
	xvm_notice_mkcodebk,
	xvm_notice_bkexec,
	xvm_notice_dbgmode,
	xvm_notice_dbgdirect,
	xvm_notice_setint3break,
	xvm_notice_sethardbreak,
	xvm_notice_setmembreak,
	xvm_notice_removeint3break,
	xvm_notice_enableint3break,
	xvm_notice_removemembreak,
	xvm_notice_Confirmint3breakpoint,
	xvm_notice_Confirmhardwarebreakpoint,
	xvm_notice_Wipebreakpointrange,
	xvm_notice_Enablemembreakpoint,
	xvm_notice_Removehardbreakpoint,
	xvm_notice_Enablehardbreakpoint,
	xvm_notice_Findfreehardbreakslot
};
enum xvm_vids{
    xvm_vid_ldrcall = 0,
    xvm_vid_oepcall,
    xvm_vid_moduleHook,
    xvm_vid_flags,
    xvm_vid_modname,
    xvm_vid_modpath,
    xvm_vid_state,
    xvm_vid_orgentry,
    xvm_vid_RVAPtr,
    xvm_vid_orgRVA,
    xvm_vid_apiname,
    xvm_vid_apibase,
    xvm_vid_skiped,
    xvm_vid_modulebase,
    xvm_vid_modapinode,
    xvm_vid_modapifinished,
    xvm_vid_redirectapi,
    xvm_vid_dataapi,
    xvm_vid_retaddr,
    xvm_vid_count,
    xvm_vid_threadid,
    xvm_vid_redirname,
    xvm_vid_behook,
    xvm_vid_apiopt,
    xvm_vid_apinamehash,
    xvm_vid_virtualkernel32,
    xvm_vid_hookexport,
    xvm_vid_usevexport,
    xvm_vid_usenullstub,
    xvm_vid_continueopt,
    xvm_vid_address,
    xvm_vid_vmexecinit,
    xvm_vid_path,
    xvm_vid_endaddr,
    xvm_vid_vmrecord,
    xvm_vid_vmprint,
    xvm_vid_rcfile,
    xvm_vid_maxrc,
    xvm_vid_stoprecordcount,
    xvm_vid_getresfrommemsec,
    xvm_vid_getresbyhookapi,
    xvm_vid_getresbybf,
    xvm_vid_respath,
    xvm_vid_resid,
    xvm_vid_resname,
    xvm_vid_resCP,
    xvm_vid_resData,
    xvm_vid_searchmemstr,
    xvm_vid_searchallidtype,
    xvm_vid_bfrestypeid,
    xvm_vid_bfrestypestr,
    xvm_vid_bfstr,
    xvm_vid_mgstart,
    xvm_vid_mgend,
    xvm_vid_rtentry,
    xvm_vid_selected,
    xvm_vid_node,
    xvm_vid_memstate,
    xvm_vid_memprotect,
    xvm_vid_size,
    xvm_vid_begin,
    xvm_vid_finished,
    xvm_vid_binary,
    xvm_vid_name,
    xvm_vid_xvmdebug,
    xvm_vid_enhanceAntiAntiDBG,
    xvm_vid_break,
    xvm_vid_disableSafeSEH,
    xvm_vid_fullpremem,
    xvm_vid_bypasssys,
	xvm_vid_stackptr,
	xvm_vid_scriptopt,
	xvm_vid_errdesc,
	xvm_vid_vmopt,
	xvm_vid_rtouter,
	xvm_vid_recordbypipe,
	xvm_vid_recordmemac,
	xvm_vid_vmexception,
    xvm_vid_vmfollowSEH,
    xvm_vid_processID,
	xvm_vid_vmblockmode,
	xvm_vid_bkfile,
	xvm_vid_subpacket,
	xvm_vid_entrypoint,
	xvm_vid_imgbase,
	xvm_vid_blockentry,
	xvm_vid_onlybkexec,
	xvm_vid_blockexeclog,
	xvm_vid_blockuuid,
	xvm_vid_block,
	xvm_vid_blockid,
	xvm_vid_breakpoints,
	xvm_vid_pattern,
	xvm_vid_comment,
	xvm_vid_options,
	xvm_vid_type,
	xvm_vid_findex,
	xvm_vid_limit,
	xvm_vid_str1,
	xvm_vid_str2,
	xvm_vid_str3,
	xvm_vid_str4,
	xvm_vid_str5,
	xvm_vid_return,
	xvm_vid_index,
	xvm_vid_addr0,
	xvm_vid_addr1
};

enum xvm_opt_vids{
    xvm_opt_vid_hookmodule
};

#endif // XVMRUNTIME_H
