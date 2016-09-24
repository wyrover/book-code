#ifndef XVMDEBUG_H
#define XVMDEBUG_H

#include <windows.h>
#include <string>

#define  vmopt_flags_pausemr		0x1		//pause when max records reached
#define  vmopt_flags_bypasssys		0x2
#define	 vmopt_flags_recordfrompipe	0x4
#define  vmopt_flags_recordtofile	0x8
#define  vmopt_flags_recordmemac	0x10
#define	 vmopt_flags_vmscript		0x20
#define	 vmopt_flags_vmexception	0x40
#define	 vmopt_flags_vmfollowSEH	0x80
#define  vmopt_flags_vmblockmode	0x100
#define  vmopt_flags_vmrecordbktofile	0x200
#define  vmopt_flags_onlyblockexec		0x400
#define  vmopt_flags_default	(vmopt_flags_bypasssys | vmopt_flags_pausemr | vmopt_flags_recordfrompipe)
struct xvmdbg_vmopt{
	int				maxRecords;
	long			flags;
	ULONG_PTR		stopaddr;
	std::wstring	rcfile;
	std::wstring	msfile;
	std::wstring	bkfile;
};

#define bkopt_flags_showhidden	0x1
struct xvmdbg_bkopt{
	long			filter;
};


#define mac_showstack	0x1
#define mac_showread	0x2
#define mac_showwrite	0x4

#define DEFAULT_RECENTPASM_NUM	5

#define sandbox_state_file		0x1
#define sandbox_state_reg		0x2
#define sandbox_state_enable	0x4
#define sandbox_state_script	0x8

typedef struct xvmdbg_config{
    int     maxPreExecLines;
    BOOL    preExecDetectMultiBranch;
    int     maxIATTraceLines;
    BOOL    enableVirtualDebug;
    BOOL    breakOnLdrLoadDll;
    BOOL    bIATTraceCheckCtx;
	BOOL	enhancePreExecUI;//启用增强型预执行界面	包括单步等等
	BOOL	antiAntiDebug;
	BOOL	enablexVMRuntime;
	BOOL	autoRemoveJmp;
	BOOL	embedNotepadPP;
	BOOL	disableSafeSEH;
	long	showMemAccFlags;
	BOOL	bProtectDrx;
	BOOL	bDirectImport;
	BOOL	bEnableApiSpy;
	BOOL	bStartupScript;
	BOOL	bRemoteIDA;
	BOOL	bFreeStkEach;
	BOOL	bReleaseStub;
	int		iStartupDLL;
    std::wstring    vdCRT;
	std::wstring	startupscp;
	std::wstring	startupdll;

	int			scp_stackSize;
	std::string scp_libs;

	long	sbox_state;
	std::wstring	sbox_script;

    unsigned char	int3cmd;			//int3替代指令	0xF4	0xC0000096
    unsigned long	int3ecode;		//int3 exception code
    unsigned char	stepcmd;			//单步替代指令
    unsigned long	stepecode;

    BOOL    singletd;			//single thread mode
	BOOL	loadPatternOnStartup;
	std::wstring	patternDir;
	std::wstring	scriptDir;
	

	std::wstring	recentPASM[DEFAULT_RECENTPASM_NUM];

	xvmdbg_vmopt	vmopt;
	xvmdbg_bkopt	bkopt;

}xvmdbg_config;

enum xvmdbg_cfg_vids{
	xvmdbg_vid_maxpeline = 0,
	xvmdbg_vid_maxiattline,
	xvmdbg_vid_prdemultibranch,
	xvmdbg_vid_enableVD,
	xvmdbg_vid_VDCRT,
	xvmdbg_vid_breakOnSys,
	xvmdbg_vid_int3cmd,
	xvmdbg_vid_int3ecode,
	xvmdbg_vid_stepcmd,
	xvmdbg_vid_stepecode,
	xvmdbg_vid_singletd,
	xvmdbg_vid_checkctx,
	xvmdbg_vid_loadpatternonstartup,
	xvmdbg_vid_patternDir,
	xvmdbg_vid_enhancePreExecUI,
	xvmdbg_vid_antiAntiDebug,
	xvmdbg_vid_enablexVMRuntime,
	xvmdbg_vid_autoremoveJmp,
	xvmdbg_vid_embednpp,
	xvmdbg_vid_scriptdir,
	xvmdbg_vid_scpstacksize,
	xvmdbg_vid_maxrecords,
	xvmdbg_vid_vmoptflags,
	xvmdbg_vid_nosafeseh,
	xvmdbg_vid_rcfile,
	xvmdbg_vid_showstackmac,
	xvmdbg_vid_recentpasm,
	xvmdbg_vid_msfile,
	xvmdbg_vid_scplibs,
	xvmdbg_vid_vmstopaddr,
	xvmdbg_vid_protectdrx,
	xvmdbg_vid_bkflags,
	xvmdbg_vid_diimport,
	xvmdbg_vid_apispy,
	xvmdbg_vid_bkfile,
	xvmdbg_vid_ldstartupscp,
	xvmdbg_vid_startscp,
	xvmdbg_vid_remoteIDA,
	xvmdbg_vid_startupdll,
	xvmdbg_vid_startupdllmode,
	xvmdbg_vid_freestkeach,
	xvmdbg_vid_sandboxstate,
	xvmdbg_vid_sboxscp,
	xvmdbg_vid_releasestub
};

extern xvmdbg_config gxvmcfg;



int loadxvmdbg_config(xvmdbg_config* cfg, const TCHAR* file);
int savexvmdbg_config(xvmdbg_config* cfg, const TCHAR* file);
#endif // XVMDEBUG_H
