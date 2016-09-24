#include "xvmdebug.h"
#include "../../../nCom/nautolargebuffer.h"
#include "../../../nCom/npacketbase.h"

xvmdbg_config gxvmcfg;


void loadxvmcfg(NPacketBase* pkopt,xvmdbg_config* cfg)
{
    cfg->maxPreExecLines = pkopt->getInt(xvmdbg_vid_maxpeline,1000);
    cfg->maxIATTraceLines = pkopt->getInt(xvmdbg_vid_maxiattline,1000);
	cfg->scp_stackSize = pkopt->getInt(xvmdbg_vid_scpstacksize,64);
    cfg->preExecDetectMultiBranch = pkopt->getInt(xvmdbg_vid_prdemultibranch,TRUE);
    cfg->enableVirtualDebug = pkopt->getInt(xvmdbg_vid_enableVD,FALSE);
    cfg->breakOnLdrLoadDll = pkopt->getInt(xvmdbg_vid_breakOnSys,FALSE);
    cfg->bIATTraceCheckCtx = pkopt->getInt(xvmdbg_vid_checkctx,FALSE);
    cfg->int3cmd = pkopt->getInt(xvmdbg_vid_int3cmd,0);
    cfg->int3ecode = pkopt->getInt(xvmdbg_vid_int3ecode,0);
    cfg->stepcmd = pkopt->getInt(xvmdbg_vid_stepcmd,0);
    cfg->stepecode = pkopt->getInt(xvmdbg_vid_stepecode,0);
    cfg->singletd = pkopt->getInt(xvmdbg_vid_singletd,0);
	cfg->loadPatternOnStartup = pkopt->getInt(xvmdbg_vid_loadpatternonstartup, TRUE);
    cfg->vdCRT = pkopt->getWString(xvmdbg_vid_VDCRT,L"xVMRuntime32.dll");
	cfg->patternDir = pkopt->getWString(xvmdbg_vid_patternDir, L"./xVMDebug/patterns/");
	cfg->enhancePreExecUI = pkopt->getInt(xvmdbg_vid_enhancePreExecUI,TRUE);
	cfg->antiAntiDebug = pkopt->getInt(xvmdbg_vid_antiAntiDebug,FALSE);
	cfg->enablexVMRuntime = pkopt->getInt(xvmdbg_vid_enablexVMRuntime,TRUE);
	cfg->autoRemoveJmp = pkopt->getInt(xvmdbg_vid_autoremoveJmp,TRUE);
	cfg->embedNotepadPP = pkopt->getInt(xvmdbg_vid_embednpp,FALSE);
	cfg->scriptDir = pkopt->getWString(xvmdbg_vid_scriptdir,L"./xVMDebug/scripts/");
	cfg->disableSafeSEH = pkopt->getInt(xvmdbg_vid_nosafeseh,TRUE);
	cfg->showMemAccFlags = pkopt->getInt(xvmdbg_vid_showstackmac,mac_showwrite | mac_showread);
	cfg->scp_libs = pkopt->getString(xvmdbg_vid_scplibs,"user32;gdi32");
	cfg->bProtectDrx = pkopt->getInt(xvmdbg_vid_protectdrx,TRUE);
	cfg->bkopt.filter = pkopt->getInt(xvmdbg_vid_bkflags,0);
	cfg->bDirectImport = pkopt->getInt(xvmdbg_vid_diimport,TRUE);
	cfg->bEnableApiSpy = pkopt->getInt(xvmdbg_vid_apispy,FALSE);
	cfg->bStartupScript = pkopt->getInt(xvmdbg_vid_ldstartupscp,FALSE);

	cfg->startupscp = pkopt->getWString(xvmdbg_vid_startscp);
	cfg->bRemoteIDA = pkopt->getInt(xvmdbg_vid_remoteIDA);
	cfg->iStartupDLL = pkopt->getInt(xvmdbg_vid_startupdllmode);
	cfg->startupdll = pkopt->getWString(xvmdbg_vid_startupdll);

	cfg->vmopt.maxRecords = pkopt->getInt(xvmdbg_vid_maxrecords,1000);
	cfg->vmopt.flags = pkopt->getInt(xvmdbg_vid_vmoptflags,vmopt_flags_default);
	cfg->vmopt.rcfile = pkopt->getWString(xvmdbg_vid_rcfile,L"./xVMDebug/records/temp.xtrace");
	cfg->vmopt.msfile = pkopt->getWString(xvmdbg_vid_msfile,L"./xVMDebug/scripts/vmexec.c");
	cfg->vmopt.bkfile = pkopt->getWString(xvmdbg_vid_bkfile,L"./xVMDebug/records/temp.xblock");
	cfg->vmopt.stopaddr = pkopt->getLongLong(xvmdbg_vid_vmstopaddr,0);
	cfg->bFreeStkEach = pkopt->getInt(xvmdbg_vid_freestkeach,FALSE);
	cfg->bReleaseStub = pkopt->getInt(xvmdbg_vid_releasestub,FALSE);
	int nc = 0;
	for (char* lpd = pkopt->tofirst(xvmdbg_vid_recentpasm); lpd; lpd = pkopt->tonext(xvmdbg_vid_recentpasm))
	{
		if (nc >= DEFAULT_RECENTPASM_NUM)
			break;
		cfg->recentPASM[nc++] = (wchar_t*)lpd;
	}
	//for sand box
	cfg->sbox_state = pkopt->getInt(xvmdbg_vid_sandboxstate,0);
	cfg->sbox_script = pkopt->getWString(xvmdbg_vid_sboxscp);
}



void savexvmcfg(NPacketBase* pkopt,xvmdbg_config* cfg)
{
    pkopt->putInt(xvmdbg_vid_maxpeline,cfg->maxPreExecLines);
    pkopt->putInt(xvmdbg_vid_maxiattline,cfg->maxIATTraceLines);
	pkopt->putInt(xvmdbg_vid_scpstacksize,cfg->scp_stackSize);
    pkopt->putInt(xvmdbg_vid_prdemultibranch,cfg->preExecDetectMultiBranch);
    pkopt->putInt(xvmdbg_vid_enableVD,cfg->enableVirtualDebug);
    pkopt->putInt(xvmdbg_vid_breakOnSys,cfg->breakOnLdrLoadDll);
    pkopt->putInt(xvmdbg_vid_checkctx,cfg->bIATTraceCheckCtx);
    pkopt->putInt(xvmdbg_vid_int3cmd,cfg->int3cmd);
    pkopt->putInt(xvmdbg_vid_int3ecode,cfg->int3ecode);
    pkopt->putInt(xvmdbg_vid_stepcmd,cfg->stepcmd);
    pkopt->putInt(xvmdbg_vid_stepecode,cfg->stepecode);
    pkopt->putInt(xvmdbg_vid_singletd,cfg->singletd);
	pkopt->putInt(xvmdbg_vid_enhancePreExecUI,cfg->enhancePreExecUI);
	pkopt->putInt(xvmdbg_vid_enablexVMRuntime,cfg->enablexVMRuntime);
	pkopt->putInt(xvmdbg_vid_autoremoveJmp,cfg->autoRemoveJmp);
	pkopt->putInt(xvmdbg_vid_loadpatternonstartup, cfg->loadPatternOnStartup);
	pkopt->putInt(xvmdbg_vid_embednpp,cfg->embedNotepadPP);
    pkopt->putWString(xvmdbg_vid_VDCRT,cfg->vdCRT.c_str());
	pkopt->putWString(xvmdbg_vid_patternDir, cfg->patternDir.c_str());
	pkopt->putWString(xvmdbg_vid_scriptdir,cfg->scriptDir.c_str());
	pkopt->putString(xvmdbg_vid_scplibs,cfg->scp_libs.c_str());
	pkopt->putInt(xvmdbg_vid_antiAntiDebug,cfg->antiAntiDebug);
	pkopt->putInt(xvmdbg_vid_nosafeseh,cfg->disableSafeSEH);
	pkopt->putInt(xvmdbg_vid_showstackmac,cfg->showMemAccFlags);
	pkopt->putInt(xvmdbg_vid_protectdrx,cfg->bProtectDrx);
	pkopt->putInt(xvmdbg_vid_bkflags,cfg->bkopt.filter);
	pkopt->putInt(xvmdbg_vid_diimport,cfg->bDirectImport);
	pkopt->putInt(xvmdbg_vid_apispy,cfg->bEnableApiSpy);
	pkopt->putInt(xvmdbg_vid_ldstartupscp,cfg->bStartupScript);
	pkopt->putWString(xvmdbg_vid_startscp,cfg->startupscp.c_str());
	pkopt->putInt(xvmdbg_vid_remoteIDA,cfg->bRemoteIDA);
	pkopt->putInt(xvmdbg_vid_startupdllmode,cfg->iStartupDLL);
	pkopt->putWString(xvmdbg_vid_startupdll,cfg->startupdll.c_str());
	pkopt->putInt(xvmdbg_vid_freestkeach,cfg->bFreeStkEach);
	pkopt->putInt(xvmdbg_vid_maxrecords,cfg->vmopt.maxRecords);
	pkopt->putInt(xvmdbg_vid_vmoptflags,cfg->vmopt.flags);
	pkopt->putInt64(xvmdbg_vid_vmstopaddr,cfg->vmopt.stopaddr);
	pkopt->putWString(xvmdbg_vid_rcfile,cfg->vmopt.rcfile.c_str());
	pkopt->putWString(xvmdbg_vid_msfile,cfg->vmopt.msfile.c_str());
	pkopt->putWString(xvmdbg_vid_bkfile,cfg->vmopt.bkfile.c_str());
	pkopt->putInt(xvmdbg_vid_releasestub,cfg->bReleaseStub);
	for (int i = 0; i < DEFAULT_RECENTPASM_NUM;i++)
	{
		pkopt->putWString(xvmdbg_vid_recentpasm,cfg->recentPASM[i].c_str());
	}
	//for sandbox
	pkopt->putInt(xvmdbg_vid_sandboxstate,cfg->sbox_state);
	pkopt->putWString(xvmdbg_vid_sboxscp,cfg->sbox_script.c_str());


}

int loadxvmdbg_config(xvmdbg_config* cfg, const TCHAR *file)
{
    if (!cfg) return -1;
    NAutoLargeBufferA nla;
    nla.load(file);
    const char* lpdata = 0;
    int szdata = 0;
    lpdata = nla.data(szdata,0,false);
    if (!lpdata || szdata < 1)
    {
        NPacketBase pkopt;
        loadxvmcfg(&pkopt,cfg);
    }else
    {
        NPacketBase pkopt(lpdata,szdata,false);
        loadxvmcfg(&pkopt,cfg);
    }
    return 0;
}


int savexvmdbg_config(xvmdbg_config *cfg, const TCHAR* file)
{
    NPacketBase pkopt;
    savexvmcfg(&pkopt,cfg);
    HANDLE hf = CreateFile(file,GENERIC_READ|GENERIC_WRITE,0,0,OPEN_ALWAYS,0,0);
    if (hf == INVALID_HANDLE_VALUE)
        return -1;
    SetFilePointer(hf,0,0,FILE_BEGIN);
    SetEndOfFile(hf);
    DWORD dwted = 0;
    WriteFile(hf,pkopt.data(),pkopt.size(),&dwted,0);
    SetEndOfFile(hf);
    CloseHandle(hf);
    return 0;
}
