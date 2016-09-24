#include <Windows.h>
#include <tchar.h>
#include <winternl.h>
#include <vector>
#include <list>
#include <map>
#include "../../xHooklib/xhook_inline.h"
#include "../../xHooklib/xhook_pefuncs.h"
#include "../../xHooklib/xhook_peloader.h"
#include "../../xHooklib/xhook_typedef.h"
#include "../../xHooklib/xhook_psfuncs.h"
#include "../../xHooklib/xhook_crtfuncs.h"
#include "../../../nCom/nlinkedlist.h"
#include "../../../nCom/nsafelocker.h"
#include "../../xHooklib/xhook_vmodule.h"
#include "../../xHooklib/xhook_memory.h"
#include "../../xHooklib/xhook_vmexec.h"
#include "../../xHooklib/xHooklibStub/xHooklibStub.h"
#include "xapihookruntime.h"
#include "xpedumper.h"

#include "../../../nCom/nautolargebuffer.h"
#include "../../../nCom/npacketbase.h"
#include "TinyCScript/tinyc.h"
#include "TinyCScript/crt/internal/internal.h"
#include "../../../nNetLib/npipeclient.h"
#include "VirtualDebugCRT/VirtualDebugApiLayerCRT.h"
#include "xvmruntime.h"

#include "xpedumper.h"
#include "xvmresource.h"
#include "xvmdebug_crt.h"

#include "xsandbox.h"

#define NMALLOC_EXPORT 1
extern "C"{
	NMALLOC_API void*	nc_malloc(size_t size)
	{
		return (void*)malloc(size);
	}
	NMALLOC_API void	nc_free(void* ptr)
	{
		free(ptr);
	}
	NMALLOC_API void*	nc_realloc(void* ptr, size_t size)
	{
		return (void*)realloc(ptr, size);
	}
}

//预先分配内存的总大小，这里设定了32MB对于一般程式启动到入口已经足够
#define MYHEAP_TOTELSIZE        0x2000000
//页大小
#define MYHEAP_PAGESIZE         0x1000
//页数量
#define MYHEAP_TOTELPAGES       (MYHEAP_TOTELSIZE/MYHEAP_PAGESIZE)
//我们自定义的内存管理状态，由于系统的内存状态管理多而复杂我们用自己的
#define MYHEAP_STATE_FREE       0
#define MYHEAP_STATE_RESERVED   0x1
#define MYHEAP_STATE_COMMIT     0x2
//定义一个跨页内存块开始和结束标记
#define MYHEAP_STATE_BKSTART    0x10
#define MYHEAP_STATE_BKEND      0x20

//预先分配的内存地址
static  PVOID   gHeapBase;
//预先分配内存大小
static  SIZE_T  gszHeap;
//预先分配内存的管理状态，我们按页表示状态
static  BYTE    gmemstBC[MYHEAP_TOTELPAGES];
//是否从我们预先分配的内存当中分配内存
static  BOOL    gMyHeapEnable;

static ULONG_PTR gOEPEsp;
static void* gOEPEntry;

void* glpOrgEntry;

static NPipeClient* gpipe = 0;
NSafeLocker gPipeLock;

NPipeClient* gAlertPipe = 0;

static BOOL gHookExport;
static BOOL gUseVExport;
static BOOL gUseNullStub;

OPBreakList	gBlockBPs;
int gDbgRunState = 0;


#define XVM_FLAG_SENDRECORDSBYPIPE	0x1
#define XVM_FLAG_SENDBLOCKBYPIPE	0x2
#define XVM_FLAG_RECORDBLOCKEXEC	0x4

extern "C"{
	void	checkPendingPacketEntry();
	void*	checkPendingPacketOuter(volatile void* lpesp,volatile void* retaddr,volatile void* realEsp);
	void	setPendingPacketExecState(DWORD state);
}

int waitAndDispatchMsg(NPacketBase* pk);
void setVMOptions(NPacketBase* opt,LPVMExecCleanupCallBack lpCleanupFunc);
//发送模块信息
int sendModuleApiInfos(HMODULE hmod,const wchar_t* modname,const wchar_t* modpath)
{

    const char* lpImage = (const char*)hmod;

    PIMAGE_DOS_HEADER	imDH	=	(PIMAGE_DOS_HEADER)lpImage;
    PIMAGE_NT_HEADERS	imNH	=	(PIMAGE_NT_HEADERS)((char*)lpImage+imDH->e_lfanew);
    //构建模块载入事件参数
    NPacketBase pk(xvm_req_modapis);
    pk.putInt64(xvm_vid_modulebase,(ULONG_PTR)hmod);
    pk.putWString(xvm_vid_modname,modname);
    pk.putWString(xvm_vid_modpath,modpath);
    gpipe->fpWrite(&pk);
    //构建模块导出表函数参数
    pk.setempty(false);
    pk.setMsgID(xvm_req_modapinodes);
    pk.putInt64(xvm_vid_modulebase,(ULONG_PTR)hmod);
    DWORD exportRVA = imNH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
    PIMAGE_EXPORT_DIRECTORY	imED	=	(PIMAGE_EXPORT_DIRECTORY)(lpImage+exportRVA);
    long	pExportSize	=	imNH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;
    //如果模块存在导出表，那么我们遍历导出表并收集相关信息
    if(!(pExportSize == 0 || (ULONG_PTR)imED <= (ULONG_PTR)lpImage))
    {
        if (imED->NumberOfFunctions > 0)
        {
            WORD* lpOrdinals = imED->AddressOfNameOrdinals?(WORD*)(lpImage+imED->AddressOfNameOrdinals):0;
            DWORD* lpNames =  imED->AddressOfNames?(DWORD*)(lpImage + imED->AddressOfNames):0;
            DWORD* lpRvas = (DWORD*)(lpImage + imED->AddressOfFunctions);
            PIMAGE_SECTION_HEADER ish = EnterImageFirstSection(lpImage);
            int nsec = imNH->FileHeader.NumberOfSections;
            NPacketBase pknode;
            for (DWORD i=0;i<imED->NumberOfFunctions;i++)
            {
                pknode.setempty(false);
                DWORD rvafunc = lpRvas[i];
                pknode.putInt(xvm_vid_orgRVA,rvafunc);
                pknode.putInt(xvm_vid_apibase,imED->Base+i);
                DWORD oftName = 0;
                //首先我们找出函数所对应的函数名称
                if (lpNames && lpOrdinals)
                {
                    for (DWORD k=0;k<imED->NumberOfNames;k++)
                    {
                        if (lpOrdinals[k] == i)
                        {
                            oftName = lpNames[k];
                            break;
                        }
                    }
                }
                if (oftName)
                    pknode.putString(xvm_vid_apiname,lpImage+oftName);
                //我们在这里判断是否为转向函数导出
                if (!(rvafunc >= exportRVA && rvafunc < (exportRVA + pExportSize)))
                {
                    //如果不是转向函数，那么我们遍历整个区段，初步筛选函数是否为数据导出
                    //这里我们筛选的方法是通过区段属性来筛选的因此不一定准确，但是大多数情况下是正确的
                    BOOL isDataExport = TRUE;
                    PIMAGE_SECTION_HEADER ishcur;
                    for (int j=0;j<nsec;j++)
                    {
                        ishcur = ish + j;
                        if (rvafunc >= ishcur->VirtualAddress && rvafunc < (ishcur->VirtualAddress+ishcur->Misc.VirtualSize))
                        {
                            if (ishcur->Characteristics & IMAGE_SCN_MEM_EXECUTE)
                            {
                                //we only hook that code export
                                isDataExport = FALSE;
                                break;
                            }
                        }
                    }
                    if (isDataExport)
                        pknode.putEmpty(xvm_vid_dataapi);
                }else
                {
                    //如果是转向函数导出，那么我们设定转向信息
                    pknode.putEmpty(xvm_vid_redirectapi);
                    pknode.putString(xvm_vid_redirname,lpImage+rvafunc);
                }
                if (pk.size() + pknode.size() >= MSG_PACKET_SIZE_MAXIMUM)
                {
                    gpipe->fpWrite(&pk);
                    pk.setempty(false);
                    pk.setMsgID(xvm_req_modapinodes);
                    pk.putInt64(xvm_vid_modulebase,(ULONG_PTR)hmod);
                }
                pk.putBinary(xvm_vid_modapinode,pknode.data(),pknode.size());
            }
        }
    }

    pk.putEmpty(xvm_vid_modapifinished);
    gpipe->fpWrite(&pk);
    return 0;
}

//模块导出表HOOK库回调函数,该函数指请求是否HOOK相应模块的导出表
//返回>=0值表示HOOK该模块
//返回<0表示不HOOK该模块导出表
int CALLBACK moduleFilterCallBack(HMODULE hmod,
                                  const wchar_t* moduleName,
                                  const wchar_t* modulePath)
{
    NStackAutoLocker sl(&gPipeLock);
    sl.Lock();
    //在这里我们先发送模块的导出表信息
    sendModuleApiInfos(hmod,moduleName,modulePath);

    //开始发送模块过滤请求事件
    NPacketBase pk(xvm_req_modulefilter);
    pk.putInt64(xvm_vid_modulebase,(ULONG_PTR)hmod);
    pk.putWString(xvm_vid_modname,moduleName);
    pk.putWString(xvm_vid_modpath,modulePath);
    gpipe->fpWrite(&pk);
    //等待xApiSpy的回复并处理相关事务
    int state = waitAndDispatchMsg(&pk);
    if (state > 0)
    {
        if (pk.getMsgID() == xvm_ack_modulefilter)
            return pk.getInt(xvm_vid_state,-1);
    }
    return -1;
}

int CALLBACK moduleHookedCallBack(HMODULE hmod,
                                  int state,
                                  xapi_modinfo* modinfo)
{
    return 0;
}

//hook api:KiFastSystemCallRet
//hook api:KiIntSystemCall
//hook api:KiRaiseUserExceptionDispatcher
//hook api:KiUserApcDispatcher
//hook api:KiUserCallbackDispatcher
//hook api:KiUserExceptionDispatcher
//hook api:LdrAccessResource
//hook api:LdrAddLoadAsDataTable
//hook api:LdrAddRefDll
//导出表HOOK库请求函数过滤回调
int CALLBACK ApiHookFilterCallBack(xapi_modinfo* modinfo,
                                   void* oriEntry,
                                   void* lpRVAPtr,
                                   DWORD orgRVA,
                                   const char* funcname,
                                   int base,
                                   bool bskiped,
                                   long* apiFlags,
                                   void** userdata)
{
    if (!modinfo)
        return 0;
    NPacketBase pk(xvm_req_apifilter);
    pk.putInt64(xvm_vid_modulebase,(ULONG_PTR)modinfo->hmod);
    pk.putInt64(xvm_vid_orgentry,(ULONG_PTR)oriEntry);
    pk.putInt64(xvm_vid_RVAPtr,(ULONG_PTR)lpRVAPtr);
    pk.putInt(xvm_vid_orgRVA,orgRVA);
    if (funcname)
        pk.putString(xvm_vid_apiname,funcname);
    pk.putInt(xvm_vid_apibase,base);
    if (bskiped)
        pk.putEmpty(xvm_vid_skiped);

    NStackAutoLocker sl(&gPipeLock);
    sl.Lock();
    gpipe->fpWrite(&pk);
    //等待xApiSpy的回复并处理相关事务
    int state = waitAndDispatchMsg(&pk);
    if (state > 0)
    {
        if (pk.getMsgID() == xvm_ack_apifilter)
        {
            *apiFlags = pk.getInt(xvm_vid_flags);
            return pk.getInt(xvm_vid_state,0);
        }
    }
    return 0;
}


int CALLBACK testFlagsCallBack(long flags,void* arg1,void* arg2)
{
    if (flags & XAPI_FLAG_HOOKEXPORT)
        return gHookExport;
    if (flags & XAPI_FLAG_USEVEXPORT)
        return gUseVExport;
    if (flags & XAPI_FLAG_NULLSTUB)
        return gUseNullStub;
    return 0;
}

void* CALLBACK VMLoopHookProc(xhook_stackregs* stregs,void* orgEntry,void* userdata)
{
    char* lpOp = *(char**)userdata;
    //    gConsole->PrintfA("IP:%p opcode1:%04X opcode2:%04X arg:%08X\n",
    //                      lpOp,
    //                      *(WORD*)(lpOp),
    //                      *(WORD*)(lpOp+2),
    //                      *(DWORD*)(lpOp+4));


    return orgEntry;
}


NTSTATUS disableProcessSafeSEH()
{
	LPZwQueryInformationProcess lpZwQueryInformationProcess = (LPZwQueryInformationProcess)MiniGetFunctionAddress((ULONG_PTR)GetModuleHandleA("NTDLL"),"ZwQueryInformationProcess");
	LPZwSetInformationProcess lpZwSetInformationProcess = (LPZwSetInformationProcess)MiniGetFunctionAddress((ULONG_PTR)GetModuleHandleA("NTDLL"),"ZwSetInformationProcess");
	DWORD flags = 0;
	lpZwQueryInformationProcess((HANDLE)-1,ProcessExecuteFlags,&flags,sizeof(flags),0);
	flags |= 0x30;
	return lpZwSetInformationProcess((HANDLE)-1,ProcessExecuteFlags,&flags,sizeof(flags));
}


void patch(void* addr,const BYTE* bin,int sz)
{
    DWORD oldpg = 0;
    VirtualProtect(addr,sz,PAGE_EXECUTE_READWRITE,&oldpg);
    memcpy(addr,bin,sz);
    if (oldpg != 0)
        VirtualProtect(addr,sz,oldpg,&oldpg);
}

void* myHeapAllocate(int npage)
{
    int szPage = npage;
    for (int i=0;i<MYHEAP_TOTELPAGES;i++)
    {
        if (gmemstBC[i] == MYHEAP_STATE_FREE)
        {
            for (int j=0;j<szPage;j++)
            {
                if (gmemstBC[i+j] != MYHEAP_STATE_FREE)
                {
                    i+=j;
                    break;
                }
            }
            //found
            if (gmemstBC[i] == MYHEAP_STATE_FREE)
            {
                for (int j=0;j<szPage;j++)
                {
                    gmemstBC[i+j] &= MYHEAP_STATE_COMMIT;
                }
                return (BYTE*)gHeapBase+(i*MYHEAP_PAGESIZE);
            }
        }
    }
    return 0;
}

struct xhook_thread_redir{
    void* orgEntry;
    void* lpArgs;
};


void replaceTIB()
{
    void* teb = getThreadTEB();
    void* lpteb = (void*)myHeapAllocate(0x1000);
    lpteb = (void*)((ULONG_PTR)lpteb & ~0xFFF);
    memcpy(lpteb,teb,0x1000);
    *(ULONG_PTR*)((ULONG_PTR)teb+0x18) = (ULONG_PTR)lpteb;
}

DWORD CALLBACK ThreadCallBackCheate(VOID* lpArg)
{
    // DebugBreak();
    if (!lpArg)
        return -1;
    replaceTIB();
    xhook_thread_redir* xt = (xhook_thread_redir*)lpArg;
    LPTHREAD_START_ROUTINE lpThreadCall = (LPTHREAD_START_ROUTINE)xt->orgEntry;
    return lpThreadCall(xt->lpArgs);
}


void* CALLBACK CreateThreadHookProc(xhook_stackregs* stregs,void* orgEntry,void* userdata)
{

    xhook_thread_redir* xt = (xhook_thread_redir*)malloc(sizeof(xhook_thread_redir));
    xt->orgEntry = (void*)*(ULONG_PTR*)(stregs->rsp+0x18);
    xt->lpArgs = (void*)*(ULONG_PTR*)(stregs->rsp+0x1C);
    *(ULONG_PTR*)(stregs->rsp+0x18) = (ULONG_PTR)ThreadCallBackCheate;
    *(ULONG_PTR*)(stregs->rsp+0x1C) = (ULONG_PTR)xt;
    return orgEntry;
}

//进程快照拍摄函数
void takeProcessSnapshot(void* entry,void* lpesp,const wchar_t* file,BOOL fullDump)
{
    //首先我们计算出相对于OEP入口时栈空间地址到程式暂停时的栈顶
    //我们认为这个区域内的栈数据是重要的
    DWORD delta = (ULONG_PTR)gOEPEsp - (ULONG_PTR)lpesp;
    //保存这段栈地址上的数据，这段数据里面包含了程式暂停时线程
    //上下文数据，因为我们使用pushfd,pushad来作为暂停程式入口
    xapi_saveContext(entry,(void*)((ULONG_PTR)lpesp-40),40,(void*)(gOEPEsp-delta),delta);
    //关闭堆空间HOOK,因为我们在后面的代码会申请内存，而此时我们
    //接管的内存中的数据已经足够使得快照运行因此关闭
    gMyHeapEnable = FALSE;
    //初始化Dump类
    xpedumper xpe;
    HMODULE hBase = GetModuleHandle(0);
    //先dump标准的PE区段，也就是原始程式区段以及到我们预分配的
    //内存地址之间的所有内存数据

    if (fullDump)
    {
        xpe.InitDumpFromMemory(hBase,(PVOID)((ULONG_PTR)gHeapBase+gszHeap));
        //        xpe.AppendSection((BYTE*)gHeapBase,MYHEAP_TOTELSIZE,
        //                          (ULONG_PTR)gHeapBase - (ULONG_PTR)hBase,
        //                          MYHEAP_TOTELSIZE,FALSE);

    }else
    {
        xpe.InitDumpFromMemory(hBase,gHeapBase);
        //开始遍历我们预分配的内存空间并且找出已经分配的空间
        for (int i=0;i<MYHEAP_TOTELPAGES;i++)
        {
            if (gmemstBC[i] & MYHEAP_STATE_COMMIT)
            {
                int j=i;
                for (;j<MYHEAP_TOTELPAGES;j++)
                {
                    if (!(gmemstBC[j] & MYHEAP_STATE_COMMIT))
                        break;
                }
                int szpgbk = j-i;
                int pgs = i;
                i=j;
                BYTE* lpMem = (BYTE*)gHeapBase+(pgs*MYHEAP_PAGESIZE);
                int szMem = szpgbk*MYHEAP_PAGESIZE;

                int szraw = szMem;
                for (signed int pos = szMem-1;pos >= 0;pos--)
                {
                    if (lpMem[pos] != 0)
                    {
                        szraw = pos+1;
                        break;
                    }
                }
                //将每一个已经使用的空间以区段形式转储到快照PE当中
                xpe.AppendSection(lpMem,szraw,
                                  (ULONG_PTR)lpMem - (ULONG_PTR)hBase,
                                  szMem,FALSE);

            }
        }
    }
    //保存快照PE文件，进程快照拍摄完毕
    xpe.SaveToFile(file);
}

int VMExecCleanupCallBack(void** lpEsp,void** lpRet,xHook_VMExec* xcpu)
{
	if (xcpu) return 0;
    gPipeLock.Lock();
    NPacketBase pk(xvm_req_vmexecfinished);
    pk.putInt64(xvm_vid_address,(ULONG_PTR)*lpRet);
    gpipe->fpWrite(&pk);
    int state = waitAndDispatchMsg(&pk);
    gPipeLock.Unlock();
    if (state > 0)
    {
        switch(pk.getMsgID())
        {
        case xvm_ack_entervm:
        {
            char* lpopt;
            int szopt;
            if (pk.getBinary(xvm_vid_continueopt,lpopt,szopt))
            {
                NPacketBase pkopt(lpopt,szopt,false);
                ULONG_PTR addr = pkopt.getLongLong(xvm_vid_address);
                if (addr > 0)
                {
                    //DebugBreak();
					setVMOptions(&pkopt,VMExecCleanupCallBack);
                    int szbuf = 40;
                    void* esptr = *lpEsp;
                    void* newEsp = (void*)((ULONG_PTR)esptr-sizeof(ULONG_PTR));
                    memmove(newEsp,(void*)esptr,szbuf);
                    *(ULONG_PTR*)((ULONG_PTR)newEsp + szbuf) = addr;
                    apiStubEnd((void*)newEsp,VMExecEntry);
                }
            }
        }break;
        }
    }
	return 0;
}

std::wstring	gblockfile;

void setVMOptions(NPacketBase* opt,LPVMExecCleanupCallBack lpCleanupFunc)
{
    //DebugBreak();
	if (opt->contains(xvm_vid_vmexception))
	{
		VMExecEnableException();
	}
    xHook_VMExec* xcpu = VMExecCPU(TRUE);
	xcpu->cpuid->setUseHardware(TRUE);
	xcpu->clearRecords();
	xcpu->resetRecordState();
    if (opt->contains(xvm_vid_vmrecord))
        xcpu->setFlags(VMEXEC_FLAG_RECORD);
    if (opt->contains(xvm_vid_vmprint))
        xcpu->setFlags(VMEXEC_FLAG_PRINT);

	xcpu->setRunState(opt->getInt(xvm_vid_state));
	xcpu->icount = 0;
	long uflags = 0;
	if (opt->contains(xvm_vid_vmblockmode))
	{
		xcpu->setFlags(VMEXEC_FLAG_RECORDBLOCK);
		uflags |= XVM_FLAG_SENDBLOCKBYPIPE;
	}
	gblockfile.clear();
	if (opt->contains(xvm_vid_bkfile))
	{
		xcpu->setFlags(VMEXEC_FLAG_RECORDBLOCK);
		gblockfile = opt->getWString(xvm_vid_bkfile,L"");
	}

	if (opt->contains(xvm_vid_onlybkexec))
	{
		xcpu->resetForBlockExecLogs();
		NInstSequencesMap* ism = xcpu->blockSequences();
		xcpu->setFlags(VMEXEC_FLAG_RECORDBLOCKEXEC);
		uflags |= XVM_FLAG_RECORDBLOCKEXEC;
		char* lpdata = opt->tofirst(xvm_vid_block);
		NPacketBase pkseq;
		while (lpdata)
		{
			pkseq.attach(lpdata,opt->szblock(),false);
			bx_phy_address bkentry = opt->toInt64();
			int seqID = pkseq.getInt(xvm_vid_blockid);
			InstSequence* seq = ism->new_back(seqID);
			seq->flags = INSTSEQ_FLAG_MONITER;
			seq->entry = pkseq.getLongLong(xvm_vid_blockentry);
			seq->refc = 0;
			seq->seqID = seqID;
			seq->uuid = pkseq.getInt(xvm_vid_blockuuid);
			xcpu->addMoniterBlock(seq);
			lpdata = opt->tonext(xvm_vid_block);
		}
	}
	char* lpbps;
	int szbps;
	OPBreakList* bps = xcpu->getBreaks();
	bps->cleanup();
	if (opt->getBinary(xvm_vid_breakpoints,lpbps,szbps))
	{
		NPacketBase pkbps(lpbps,szbps,false);
		char* lpdata = pkbps.tofirst(xvm_vid_break);
		while (lpdata)
		{
			ULONG_PTR ip = pkbps.toInt();
			opcode_bp* opbp = bps->new_back(ip);
			opbp->ip = ip;

			lpdata = pkbps.tonext(xvm_vid_break);
		}
	}
	
	if (opt->contains(xvm_vid_vmfollowSEH))
	{
		xcpu->setFlags(VMEXEC_FLAG_RECORDSEH);
	}

	xcpu->setSpecialAddress((bx_phy_address)VMExecEntry);
	xcpu->setSpecialAddress((bx_phy_address)checkPendingPacketEntry);

	if (opt->contains(xvm_vid_bypasssys))
    {
		xcpu->setSpecialAddress((bx_phy_address)apiStubEntry);
        xcpu->setFlags(VMEXEC_FLAG_BYPASSSYS);
    }
	if (opt->contains(xvm_vid_recordbypipe))
	{
		xcpu->setFlags(VMEXEC_FLAG_RECORD);
		uflags |= XVM_FLAG_SENDRECORDSBYPIPE;
		
	}
	if (opt->contains(xvm_vid_recordmemac))
		xcpu->setFlags(VMEXEC_FLAG_RECORDMEMAC);

    //DebugBreak();
    xcpu->setTraceStopAddress(opt->getLongLong(xvm_vid_endaddr));
	if (opt->contains(xvm_vid_rcfile))
	{
		xcpu->setFlags(VMEXEC_FLAG_RECORDTOFILE);
		xcpu->setTraceRecordFile(opt->getWString(xvm_vid_rcfile,0));
	}
	xcpu->setUserData((void*)uflags);
    xcpu->setTraceStopRCCount(opt->getInt(xvm_vid_stoprecordcount,0));
    xcpu->setMaxTraceRecords(opt->getInt(xvm_vid_maxrc,10000));
    xcpu->setCleanupCallBack(lpCleanupFunc);





}


void* CALLBACK apiExecuteCallBack(xapi_apiInfo* apiinfo,void* retaddr,void* lpRet,void* lpesp,void** esptop)
{
    if (apiinfo)
    {
        //先锁定，防止多线程访问冲突
        gPipeLock.Lock();
        //整理打包参数传送给xApiSpy程式进行记录
        NPacketBase pk(xvm_req_apicall);
        pk.putInt64(xvm_vid_modulebase,(ULONG_PTR)apiinfo->module->hmod);
        pk.putInt64(xvm_vid_retaddr,(ULONG_PTR)retaddr);
        pk.putInt64(xvm_vid_address,(ULONG_PTR)apiinfo->oriEntry);
        pk.putInt(xvm_vid_orgRVA,apiinfo->orgRVA);
        pk.putInt(xvm_vid_count,apiinfo->AllCallCount);
        pk.putInt(xvm_vid_threadid,GetCurrentThreadId());
        if (apiinfo->flags & xvm_api_flag_breakpoint)
            pk.putEmpty(xvm_vid_break);
        gpipe->fpWrite(&pk);
        int state = waitAndDispatchMsg(&pk);
        gPipeLock.Unlock();
        if (state > 0)
        {
            switch(pk.getMsgID())
            {
            case xvm_ack_entervm:
            {
                char* lpopt;
                int szopt;
                if (pk.getBinary(xvm_vid_continueopt,lpopt,szopt))
                {
                    NPacketBase pkopt(lpopt,szopt,false);
                    ULONG_PTR addr = pkopt.getLongLong(xvm_vid_address);
                    if (addr > 0)
                    {   //设定我们的虚拟机选项，也就是界面中的各种参数
						setVMOptions(&pkopt,VMExecCleanupCallBack);
                        ULONG_PTR cesp = (ULONG_PTR)*esptop;
                        int szbuf = (ULONG_PTR)lpesp - cesp;
                        cesp -= sizeof(ULONG_PTR);
                        memmove((void*)cesp,*esptop,szbuf);
                        *(ULONG_PTR*)((ULONG_PTR)lpesp - sizeof(ULONG_PTR)) = addr;
                        //直接通过我们xVMRuntime模块的出口函数恢复线程上下文到
                        //我们的虚拟机入口函数VMExecEntry执行
                        apiStubEnd((void*)cesp,VMExecEntry);
                    }
                }
            }break;
            case xvm_ack_takesnapshot://接收到消息
            {
                char* lpopt;
                int szopt;
                if (pk.getBinary(xvm_vid_continueopt,lpopt,szopt))
                {
                    NPacketBase pkopt(lpopt,szopt,false);
                    //提取参数并传递给takeProcessSnapshot函数
                    const wchar_t* lpfile = pkopt.getWString(xvm_vid_path,0);
                    if (lpfile)
                    {
                        BOOL fullMem = pkopt.contains(xvm_vid_fullpremem)?TRUE:FALSE;
                        //执行快照拍摄动作，并且传递必要的参数，例如我们的快照再次启动后载入地址
                        //栈顶地址
                        takeProcessSnapshot(apiinfo->hookedEntry,lpesp,lpfile,fullMem);
                    }
                }
            }break;
            }
        }
    }
    return lpRet;
}


LPNtSetInformationThread glpNtSetInformationThread;
NTSTATUS NTAPI Proxy_NtSetInformationThread(
        IN HANDLE hThread,
        IN THREADINFOCLASS ThreadInfoClass,
        IN PVOID ThreadInfoBuffer,
        IN ULONG ThreadInfoBufferLength
        )
{
    if (ThreadInfoClass==ThreadHideFromDebugger)
    {
        return 0;
    }
    return glpNtSetInformationThread(hThread,ThreadInfoClass,ThreadInfoBuffer,ThreadInfoBufferLength);
}

void EnableModuleHook(NPacketBase* options)
{

    //sendModuleApiInfos(GetModuleHandle(0));
    xApiCallbacks xcb = {0};
    xcb.mfcb = moduleFilterCallBack;
    xcb.mhedcb = moduleHookedCallBack;
    xcb.sacb = ApiHookFilterCallBack;
    xcb.tfcb = testFlagsCallBack;
    xcb.aecb = apiExecuteCallBack;
    //xcb.amw4 = MyAllocMemoryWithin4GB;
    gHookExport = options->getInt(xvm_vid_hookexport,1);
    gUseVExport = options->getInt(xvm_vid_usevexport,0);
    gUseNullStub = options->getInt(xvm_vid_usenullstub,0);

    xapi_initApiHook(options->getInt(xvm_vid_flags,XAPI_INIT_FLAG_DEFAULT),&xcb);

    //   xapi_initApiHook(XAPI_INIT_FLAG_HOOKEXISTSMODULES|XAPI_INIT_FLAG_CHEATLDR|XAPI_INIT_FLAG_HOOKLOADDLL,&xcb);
}



LPNtProtectVirtualMemory glpNtProtectVirtualMemory = 0;
LPNtAllocateVirtualMemory glpNtAllocateVirtualMemory = 0;
NTSTATUS NTAPI HOOK_NtAllocateVirtualMemory(
        IN HANDLE hProces,
        IN OUT PVOID *PreferredBaseAddress,
        IN ULONG nLowerZeroBits,
        IN OUT PULONG SizeRequestedAllocated,
        IN ULONG AllocationType,
        IN ULONG ProtectionAttributes
        )
{
    //取得请求分配空间大小
    SIZE_T szMem = *SizeRequestedAllocated;
    //如果我们的预分配空间已经申请到了说明内存管理接管已经初始化，执行接管
    if (gHeapBase != 0 && szMem > 0)
    {
        //如果遇到指定地址申请内存的
        BYTE* addr = (BYTE*)*PreferredBaseAddress;
        //计算页大小
        int szPage = (szMem / MYHEAP_PAGESIZE) + ((szMem % MYHEAP_PAGESIZE)?1:0);
        //转换成我们自己的状态管理
        int state = (AllocationType & MEM_COMMIT)?MYHEAP_STATE_COMMIT:MYHEAP_STATE_RESERVED;
        if (gMyHeapEnable && addr == 0)
        {
            //找出一段足够大小的空白区域
            for (int i=0;i<MYHEAP_TOTELPAGES;i++)
            {
                if ((i%16) != 0)
                    continue;
                if (gmemstBC[i] == MYHEAP_STATE_FREE)
                {
                    for (int j=0;j<szPage;j++)
                    {
                        if (gmemstBC[i+j] != MYHEAP_STATE_FREE)
                        {
                            i+=j;
                            break;
                        }
                    }
                    //如果找到了计算出地址
                    if (gmemstBC[i] == MYHEAP_STATE_FREE)
                    {
                        addr = (BYTE*)gHeapBase+(i*MYHEAP_PAGESIZE);
                        break;
                    }
                }
            }
        }
        //这里我们已经完成了地址的预测，即便是对于指定了地址空间的，如果地址空间
        //在我们的管理空间内我们就处理，否则就超出了我们的能力范围了
        if (addr >= (BYTE*)gHeapBase && addr < ((BYTE*)gHeapBase+MYHEAP_TOTELSIZE))
        {
            addr =(BYTE*)((ULONG_PTR)addr & ~(MYHEAP_PAGESIZE-1));
            int pgpos = (addr - (BYTE*)gHeapBase) / MYHEAP_PAGESIZE;
            szPage = min(szPage,MYHEAP_TOTELPAGES-pgpos);
            //我们再次检查预测范围内是否已经存在分配了的内存
            BOOL bFaild = FALSE;
            for (int i=0;i<szPage;i++)
            {
                if (i < szPage-1)
                {
                    if (gmemstBC[pgpos+i] & MYHEAP_STATE_BKEND)
                    {
                        bFaild = TRUE;
                        break;
                    }
                }
                if (gmemstBC[pgpos+i] & MYHEAP_STATE_COMMIT)
                {
                    bFaild = TRUE;
                    break;
                }
            }
            //如果不存在那么能够正常分配，我们设定相关标记
            if (!bFaild)
            {
                for (int i=0;i<szPage;i++)
                {
                    if (i==0)
                    {
                        if (gmemstBC[pgpos+i]  == MYHEAP_STATE_FREE)
                            gmemstBC[pgpos+i] = MYHEAP_STATE_BKSTART;
                    }
                    else if (i==szPage-1)
                    {
                        if (gmemstBC[pgpos+i]  == MYHEAP_STATE_FREE)
                            gmemstBC[pgpos+i] = MYHEAP_STATE_BKEND;
                    }

                    gmemstBC[pgpos+i] &= 0xF0;
                    gmemstBC[pgpos+i] |= state;
                }
                *PreferredBaseAddress = (PVOID)addr;
                *SizeRequestedAllocated = szPage * MYHEAP_PAGESIZE;
                ULONG odPG = 0;
                //注意，这里我们修改内存属性到申请的内存属性
                glpNtProtectVirtualMemory(hProces,PreferredBaseAddress,SizeRequestedAllocated,ProtectionAttributes,&odPG);
                return 0;
            }else
            {
                //指定了地址申请但是我们无法分配到内存，返回错误代码
                if (*PreferredBaseAddress)
                    return 0xC0000018;
                //对于没有指定地址的分配，即使我们接管失败我们调用系统函数来申请
            }

        }
    }

    NTSTATUS state = glpNtAllocateVirtualMemory(hProces
                                                ,PreferredBaseAddress
                                                ,nLowerZeroBits
                                                ,SizeRequestedAllocated
                                                ,AllocationType
                                                ,ProtectionAttributes);
    return state;
}


LPZwFreeVirtualMemory glpZwFreeVirtualMemory = 0;
NTSTATUS NTAPI Hook_ZwFreeVirtualMemory(__in HANDLE ProcessHandle,__inout PVOID *BaseAddress,
                                        __inout PSIZE_T RegionSize,__in ULONG FreeType)
{
    //计算地址，如果是我们管理申请的内存才处理
    SIZE_T szMem = *RegionSize;
    BYTE* addr = (BYTE*)*BaseAddress;
    if (addr >= (BYTE*)gHeapBase && addr < ((BYTE*)gHeapBase+MYHEAP_TOTELSIZE))
    {
        addr =(BYTE*)((ULONG_PTR)addr & ~(MYHEAP_PAGESIZE-1));
        int szPage = (szMem / MYHEAP_PAGESIZE) + ((szMem % MYHEAP_PAGESIZE)?1:0);
        int pgpos = (addr - (BYTE*)gHeapBase) / MYHEAP_PAGESIZE;
        if (FreeType & MEM_RELEASE)
        {
            if (szMem != 0)
                return (NTSTATUS)0xC00000A0;
            if ((ULONG_PTR)addr & 0xFFFF)
                return (NTSTATUS)0xC00000A0;
            if (!(gmemstBC[pgpos] & MYHEAP_STATE_BKSTART))
                return (NTSTATUS)0xC00000A0;
            BOOL bvaild = FALSE;
            //找到跨页内存区块的边界
            for (int i=pgpos;i<MYHEAP_TOTELPAGES;i++)
            {
                szPage++;
                if (gmemstBC[i] & MYHEAP_STATE_BKEND)
                {
                    bvaild = TRUE;
                    break;
                }
                if (gmemstBC[i] & MYHEAP_STATE_BKSTART)
                    break;
            }
            if (!bvaild)
                return (NTSTATUS)0xC00000A0;
            //将状态修改为释放
            *BaseAddress = addr;
            *RegionSize = szPage*MYHEAP_PAGESIZE;
            for (int i=0;i<szPage;i++)
            {
                gmemstBC[pgpos+i] = MYHEAP_STATE_FREE;
                memset((BYTE*)gHeapBase+((pgpos+i)*MYHEAP_PAGESIZE),0,MYHEAP_PAGESIZE);
            }
            ULONG odPG = 0;
            //这里我们使用修改内存属性到PAGE_NOACCESS来模拟无效内存的状态，这样可以模仿
            //内存访问异常的效果
            glpNtProtectVirtualMemory(ProcessHandle,BaseAddress,RegionSize,PAGE_NOACCESS,&odPG);
            return 0;
        }else if (FreeType & MEM_DECOMMIT)
        {
            //针对MEM_DECOMMIT我们不能将内存标记为FREE而是应当标记为RESERVED
            if (szMem == 0)
                return (NTSTATUS)0xC00000A0;
            int bkpg = 0;
            BOOL bvaild = FALSE;
            for (int i=pgpos;i<MYHEAP_TOTELPAGES;i++)
            {
                bkpg++;
                if (gmemstBC[i] & MYHEAP_STATE_BKEND)
                {
                    bvaild = TRUE;
                    break;
                }
            }
            if (szPage > bkpg)
                return (NTSTATUS)0xC00000A0;
            for (int i=0;i<szPage;i++)
            {

                gmemstBC[pgpos+i] &= 0xF0;
                gmemstBC[pgpos+i] |= MYHEAP_STATE_RESERVED;
                memset((BYTE*)gHeapBase+((pgpos+i)*MYHEAP_PAGESIZE),0,MYHEAP_PAGESIZE);
            }
            *BaseAddress = addr;
            *RegionSize = szPage*MYHEAP_PAGESIZE;
            ULONG odPG = 0;
            glpNtProtectVirtualMemory(ProcessHandle,BaseAddress,RegionSize,PAGE_NOACCESS,&odPG);
            return 0;
        }
        return (NTSTATUS)0xC00000A0;
    }
    return glpZwFreeVirtualMemory(ProcessHandle,BaseAddress,RegionSize,FreeType);
}

void EnableHeapHook(HMODULE hNtdll)
{
    //设定标记为开启内存接管
    gMyHeapEnable = TRUE;
    //初始化内存状态标记
    sys_memset(gmemstBC,0,sizeof(gmemstBC));
    HMODULE hmod = getMainModuleHandle();
    SIZE_T szHeap = MYHEAP_TOTELSIZE;
    //在紧接主模块之后分配一块大内存
    char* lpHeap = (char*)AllocateMemoryWithin4GB_Dynamic(hNtdll,(const char*)hmod,szHeap,PAGE_EXECUTE_READWRITE);
    if (lpHeap)
    {
        //如果分配成功我们就保存相关信息
        gHeapBase = lpHeap;
        gszHeap = szHeap;
        PVOID baseAddr = 0;
        //HOOK掉NtAllocateVirtualMemory与NtFreeVirtualMemory这两个底层函数
        //通过接管这两个函数我们等于接管之后进程所有的内存分配操作
        //因为堆空间的分配也使用这两个函数，因此我们等于就接管了堆空间的创建
        glpNtProtectVirtualMemory = (LPNtProtectVirtualMemory)GetFunctionAddress((ULONG_PTR)hNtdll,"NtProtectVirtualMemory");
        if (glpNtProtectVirtualMemory)
        {
            void* lpFunc = (void*)GetFunctionAddress((ULONG_PTR)hNtdll,"NtAllocateVirtualMemory");
            if (lpFunc)
            {
                baseAddr = lpFunc;
                ULONG szrg = 16;
                ULONG odpg = 0;
                if (glpNtProtectVirtualMemory((HANDLE)-1,&baseAddr,&szrg,PAGE_EXECUTE_READWRITE,&odpg) == 0)
                {
                    glpNtAllocateVirtualMemory = (LPNtAllocateVirtualMemory)HookCodeDirect(hNtdll,lpFunc,HOOK_NtAllocateVirtualMemory,HOOKTYPE_PUSH,0);
                    glpNtProtectVirtualMemory((HANDLE)-1,&baseAddr,&szrg,odpg,&odpg);
                }
            }
            lpFunc = (void*)GetFunctionAddress((ULONG_PTR)hNtdll,"NtFreeVirtualMemory");
            if (lpFunc)
            {
                baseAddr = lpFunc;
                ULONG szrg = 16;
                ULONG odpg = 0;
                if (glpNtProtectVirtualMemory((HANDLE)-1,&baseAddr,&szrg,PAGE_EXECUTE_READWRITE,&odpg) == 0)
                {
                    glpZwFreeVirtualMemory = (LPZwFreeVirtualMemory)HookCodeDirect(hNtdll,lpFunc,Hook_ZwFreeVirtualMemory,HOOKTYPE_PUSH,0);
                    glpNtProtectVirtualMemory((HANDLE)-1,&baseAddr,&szrg,odpg,&odpg);
                }
            }
        }
    }
}

void initVirtualKernel32()
{
	static BOOL bVKer32Inited = FALSE;
	if (bVKer32Inited) return;
	bVKer32Inited = TRUE;
    VirtualOneDLL(GetModuleHandleA("NTDLL"));
    VirtualOneDLL(GetModuleHandleA("KERNELBASE"));
    VirtualOneDLL(GetModuleHandleA("KERNEL32"));
    AfterVirtualDLL();
    VirtualImport((char*)GetBaseByFuncUseMagic((ULONG_PTR)initVirtualKernel32));
}


void sendResourceLeaf(ResourceLeaf* leaf)
{
    NPacketBase pk(xvm_req_resnode);
    //按顺序将资源路径封包
    for (ResourcePath::iterator itr = leaf->m_ResPath.begin();itr != leaf->m_ResPath.end();itr++)
    {
        NPacketBase pkpath;
        ResourceNodeName* rn = &*itr;
        if (!rn->m_NameIsString)
            pkpath.putInt(xvm_vid_resid,rn->m_id);
        else
            pkpath.putWString(xvm_vid_resname,rn->m_sName.c_str());
        pkpath.putEmpty(xvm_vid_selected);
        pk.putBinary(xvm_vid_respath,pkpath.data(),pkpath.size());
    }
    //封包其它数据
    pk.putInt(xvm_vid_resCP,leaf->m_CodePage);
    pk.putBinary(xvm_vid_resData,&leaf->m_data[0],leaf->m_data.size());
    //发送给xApiSpy
    gPipeLock.Lock();
    gpipe->fpWrite(&pk);
    gPipeLock.Unlock();
}

void sendModuleResource(ULONG_PTR hmod)
{
    PIMAGE_OPTIONAL_HEADER imoh = EnterImageOpHeader((const char*)hmod);
    DWORD rsrcRVA = imoh->DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress;
    DWORD szrsrc = imoh->DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].Size;
    if (rsrcRVA == 0)
        return;	//没有资源数据
    const char* lpImage = (const char*)hmod;
    //定位到资源目录
    IMAGE_RESOURCE_DIRECTORY* pResHdr =(IMAGE_RESOURCE_DIRECTORY*)(lpImage + rsrcRVA);
    //定义变量用于存储资源目录项数据
    ResourcePath    ResPath;
    ResourceNodes   ResNodes;
    //收集具体的资源数据
    FollowResourceLeaf(ResPath,ResNodes,lpImage,(const char*)pResHdr,pResHdr);
    //发送资源数据到xApiSpy程式
    for (ResourceNodes::iterator itr = ResNodes.begin();itr != ResNodes.end();itr++)
        sendResourceLeaf(&*itr);
}



typedef struct _RESOURCEINFO{
    LPCWSTR  lpType;
    LPCWSTR  lpName;
    DWORD   wLanguage;

}RESOURCEINFO,*PRESOURCEINFO;


void buildResourceLeaf(ResourceLeaf& leaf,void* lpRes,DWORD szRes,PRESOURCEINFO resInfo)
{
    //DebugBreak();
    leaf.m_data.resize(szRes);
    memcpy(&leaf.m_data[0],lpRes,szRes);
    leaf.m_CodePage = resInfo->wLanguage;

    //由于缺失资源目录信息，因此我们手动构建标准3层资源目录
    //第一层资源类别
    //ID小于65536,字串大于65536
    ResourceNodeName resType;
    resType.m_NameIsString = ((DWORD)resInfo->lpType >= 0x10000);
    if (!resType.m_NameIsString)
    {
        resType.m_id = (int)resInfo->lpType;
    }else
    {
        resType.m_sName = resInfo->lpType;
    }
    leaf.m_ResPath.push_back(resType);
    //第二层资源名称/ID
    ResourceNodeName resName;
    resName.m_NameIsString = ((DWORD)resInfo->lpName >= 0x10000);
    if (!resName.m_NameIsString)
    {
        resName.m_id = (int)resInfo->lpName;
    }else
    {
        resName.m_sName = resInfo->lpName;
    }
    leaf.m_ResPath.push_back(resName);
    //第三层资源本地信息
    ResourceNodeName resCP;
    resCP.m_NameIsString = false;
    resCP.m_id = (int)resInfo->wLanguage;
    leaf.m_ResPath.push_back(resCP);

}

typedef NTSTATUS (NTAPI* LPLdrFindResource_U)(HMODULE hmod,
                                              PRESOURCEINFO resInfo,
                                              DWORD type,
                                              HRSRC* hrsrc);


LPLdrFindResource_U glpLdrFindResource_U = 0;
HMODULE gMonModBase = 0;
NTSTATUS NTAPI Hook_LPLdrFindResource_U(HMODULE hmod,
                                        PRESOURCEINFO resInfo,
                                        DWORD type,
                                        HRSRC* hrsrc)
{
    NTSTATUS st = glpLdrFindResource_U(hmod,resInfo,type,hrsrc);
    if (st == 0 && gMonModBase == hmod && *hrsrc)
    {
        HRSRC hres = *hrsrc;
        //直接调用系统函数获取资源信息
        DWORD szRes = SizeofResource(hmod,hres);
        HGLOBAL hg = LoadResource(hmod,hres);
        LPVOID lpRes = LockResource(hg);
        if (lpRes && szRes > 0)
        {
            ResourceLeaf leaf;
            //重建资源数据
            buildResourceLeaf(leaf,lpRes,szRes,resInfo);
            //发送资源项到xApiSpy
            sendResourceLeaf(&leaf);
            UnlockResource(hg);
            FreeResource(hg);
        }
    }
    return st;
}

BOOL isAsciiString(const BYTE* lpstr,int szmax)
{
    for (int i=0;i<szmax;i++,*lpstr++)
    {
        BYTE chr = *lpstr;
        if (chr == 0)
            break;
        if (chr < 0x20 || chr >= 0x7F)
            return FALSE;
    }
    return TRUE;
}

void bruteforceResource(NPacketBase* opt)
{
    HMODULE hmod = (HMODULE)opt->getLongLong(xvm_vid_modulebase);
    if (hmod == 0)
        hmod = GetModuleHandle(0);
    int nf = 24;
    //如果设置遍历所有资源ID类型，这样将导致42亿的穷举量，
    //不过对于现代计算机来说是可以接受的
    if (opt->contains(xvm_vid_searchallidtype))
        nf = 65535;
    for (int i=0;i<nf;i++)
    {
        for (int j=0;j<0x10000;j++)
        {
            FindResourceW(hmod,(LPCWSTR)j,(LPCWSTR)i);
        }
    }
    //穷举常用的字串资源类
    for (int j=0;j<0x10000;j++)
        FindResourceW(hmod,(LPCWSTR)j,(LPCWSTR)L"MUI");
    for (int j=0;j<0x10000;j++)
        FindResourceW(hmod,(LPCWSTR)j,(LPCWSTR)L"AVI");

    LPCWSTR bfType = (LPCWSTR)-1;
    if (opt->contains(xvm_vid_bfrestypestr))
        bfType = opt->getWString(xvm_vid_bfrestypestr);
    else
        bfType = (LPCWSTR)opt->getInt(xvm_vid_bfrestypeid,-1);
    if ((int)bfType != -1)
    {
        //穷举设定的字串
        char* lpnode = opt->tofirst();
        while(lpnode)
        {
            if (opt->vid() == xvm_vid_bfstr)
            {
                FindResourceW(hmod,(LPCWSTR)opt->block(),bfType);
            }
            lpnode = opt->tonext();
        }
        //穷举模块内所有字串资源，实际上单个资源类型穷举量等于映像大小
        if (opt->contains(xvm_vid_searchmemstr))
        {
            ULONG_PTR memstart = opt->getLongLong(xvm_vid_mgstart);
            ULONG_PTR memend = opt->getLongLong(xvm_vid_mgend);
            if (memstart == 0)
            {
                memstart = (ULONG_PTR)hmod;
                memend = memstart + EnterImageNtHeader((const char*)hmod)->OptionalHeader.SizeOfImage;
            }
            MEMORY_BASIC_INFORMATION mbi = {0};
            SIZE_T szt = VirtualQuery((BYTE*)memstart,&mbi,sizeof(mbi));
            if (szt > 0)
                if (memend == 0)
                    memend = (ULONG_PTR)mbi.BaseAddress+mbi.RegionSize;
            while(szt)
            {
                if (mbi.BaseAddress >= (BYTE*)memend)
                    break;
                if (mbi.State != MEM_RESERVE && mbi.State != MEM_FREE)
                {
                    if (!IsBadReadPtr(mbi.BaseAddress,1))
                    {
                        const BYTE* lpStr = (const BYTE*)mbi.BaseAddress;
                        int szSec = mbi.RegionSize-1;
                        for (int j=0;j<szSec;j++,lpStr++)
                        {
                            if (*lpStr == 0)
                                continue;
                            //搜索UNICODE
                            if (*(lpStr+1) == 0)
                                FindResourceW(hmod,(LPCWSTR)lpStr,bfType);
                            else if (isAsciiString(lpStr,szSec-j))
                            {
                                //搜索ASCII字串
                                FindResourceA(hmod,(LPCSTR)lpStr,(LPCSTR)bfType);
                            }
                        }
                    }
                }
                szt=VirtualQuery((BYTE*)mbi.BaseAddress+mbi.RegionSize,&mbi,sizeof(mbi));
            }
        }
    }
}

void dispatchResourceBuild(NPacketBase* opt)
{
    ULONG_PTR hmod = opt->getLongLong(xvm_vid_modulebase);
    if (hmod == 0)
        hmod = (ULONG_PTR)GetModuleHandle(0);
    if (opt->contains(xvm_vid_getresfrommemsec))
        sendModuleResource(hmod);
    BOOL bBF = opt->contains(xvm_vid_getresbybf);
    if (opt->contains(xvm_vid_getresbyhookapi) || bBF)
    {
        gMonModBase = (HMODULE)hmod;
        if (!glpLdrFindResource_U)
        {
            glpLdrFindResource_U = (LPLdrFindResource_U)HookOneApi(_T("NTDLL"),"LdrFindResource_U",
                                                                   Hook_LPLdrFindResource_U,HOOKTYPE_JMP);
        }
    }
    if (bBF)
        bruteforceResource(opt);
}
//内存块发送
void sendMemoryBlock(const BYTE* lpMemBK,int szMemBK,DWORD state,DWORD protect,const char* name)
{
    NStackAutoLocker sl(&gPipeLock);
    sl.Lock();
    //构建onMemoryData封包
    NPacketBase pk(xvm_req_memdata);
    pk.putEmpty(xvm_vid_begin);
    //将内存数据按照最大的封包大小切片并发送
    for (int i=0;i<szMemBK;)
    {
        int szonce = min(szMemBK-i,MSG_PACKET_SIZE_MAXIMUM-1000);
        pk.putInt64(xvm_vid_address,(ULONG_PTR)lpMemBK+i);
        pk.putInt(xvm_vid_size,szonce);
        pk.putBinary(xvm_vid_binary,(const char*)lpMemBK+i,szonce);
        i+=szonce;
        gpipe->fpWrite(&pk);
        pk.setempty(false);
        pk.setMsgID(xvm_req_memdata);
    }
    //发送一个内存块结束标记
    pk.putEmpty(xvm_vid_finished);
    gpipe->fpWrite(&pk);
    //接着构建onMemoryBlock事件通知xApiSpy一个内存块已经发送完毕了
    pk.setempty(false);
    pk.setMsgID(xvm_req_memblock);
    pk.putInt64(xvm_vid_mgstart,(ULONG_PTR)lpMemBK);
    pk.putInt64(xvm_vid_mgend,szMemBK);
    pk.putInt(xvm_vid_memstate,state);
    pk.putInt(xvm_vid_memprotect,protect);
    if (name)
        pk.putString(xvm_vid_name,name);
    gpipe->fpWrite(&pk);
}

//处理内存数据抓取请求
void dispatchTakeMemorySnapshot(NPacketBase* pk)
{
    //整理抓取请求中的参数信息
    ULONG_PTR memstart = pk->getLongLong(xvm_vid_mgstart);
    ULONG_PTR memend = pk->getLongLong(xvm_vid_mgend);
    if (memstart == 0)  //如果并未指定开始地址，假设为主模块地址
        memstart = (ULONG_PTR)GetModuleHandle(0);

    MEMORY_BASIC_INFORMATION mbi = {0};
    SIZE_T szt = VirtualQuery((BYTE*)memstart,&mbi,sizeof(mbi));
    if (szt > 0)
    {   //如果没有指定结束地址，那么尝试使用模块大小
        if (memend == 0)
        {
            if (mbi.State != MEM_RESERVE && mbi.State != MEM_FREE)
            {
                memend = (ULONG_PTR)mbi.BaseAddress+mbi.RegionSize;
                if (!IsBadReadPtr((BYTE*)mbi.BaseAddress,1))
                {
                    if (((ULONG_PTR)mbi.BaseAddress & 0xFFFF) == 0)
                    {
                        if (IsValidPE((const char*)mbi.BaseAddress))
                            memend = memstart + EnterImageNtHeader((const char*)memstart)->OptionalHeader.SizeOfImage;
                    }
                }

            }
        }
    }
    //定义区段名称容器，我们会试图获取内存块对应的区段名称
    std::map<ULONG_PTR,std::string> secNames;
    while(szt)
    {
        if (mbi.BaseAddress >= (BYTE*)memend)
            break;
        if (mbi.State != MEM_RESERVE && mbi.State != MEM_FREE)
        {
            const BYTE* lpMemBK = (const BYTE*)mbi.BaseAddress;
            int szMemBK = mbi.RegionSize;
            if (!IsBadReadPtr(lpMemBK,1))
            {
                //如果我们侦测到有效的PE头信息，那么收集区段名称
                if (IsValidPE((const char*)lpMemBK))
                {
                    PIMAGE_SECTION_HEADER imsh = EnterImageFirstSection((const char*)lpMemBK);
                    int nsec = EnterImageNtHeader((const char*)lpMemBK)->FileHeader.NumberOfSections;
                    for (int i=0;i<nsec;i++,imsh++)
                    {
                        std::string secn;
                        secn.append((const char*)imsh->Name,sizeof(imsh->Name));
                        secNames.insert(std::make_pair((ULONG_PTR)lpMemBK+imsh->VirtualAddress,secn));
                    }
                }
                const char* sname = 0;
                std::map<ULONG_PTR,std::string>::iterator itr = secNames.find((ULONG_PTR)lpMemBK);
                if (itr != secNames.end())
                    sname = itr->second.c_str();
                //直接发送内存块数据到xApiSpy
                sendMemoryBlock(lpMemBK,szMemBK,mbi.State,mbi.Protect,sname);
            }
        }
        szt=VirtualQuery((BYTE*)mbi.BaseAddress+mbi.RegionSize,&mbi,sizeof(mbi));
    }
}

int dispatchContinueOpt(NPacketBase* pk)
{
    int nact = 0;
    char* lpopt = 0;
    lpopt = pk->tofirst();
    while(lpopt)
    {
        if (pk->vid() == xvm_vid_continueopt)
        {
            NPacketBase pkopt(lpopt,pk->szblock(),true);
            switch(pkopt.getMsgID())
            {
            case xvm_ack_spyrsrc:
            {
                dispatchResourceBuild(&pkopt);
                nact++;
            }break;
            case xvm_ack_takememsnapshot:
            {
                dispatchTakeMemorySnapshot(&pkopt);
                nact++;
            }break;
            case xvm_ack_injectmodule:
            {
                const wchar_t* wpath = pkopt.getWString(xvm_vid_path);
                HMODULE hmod = LoadLibraryW(wpath);
                NPacketBase pk(xvm_req_injectmodule);
                pk.putInt64(xvm_vid_modulebase,(ULONG_PTR)hmod);
                gpipe->fpWrite(&pk);
                nact++;
            }break;
            case xvm_ack_setapibp:
            {
                nact++;
            }break;
            }
        }

        lpopt = pk->tonext();
    }
    return nact;
}


void dispatchInit(NPacketBase* pk)
{
    if (pk->contains(xvm_vid_disableSafeSEH))
        disableProcessSafeSEH();
    if (pk->contains(xvm_vid_enhanceAntiAntiDBG))
    {
        glpNtSetInformationThread = (LPNtSetInformationThread)HookOneApi(_T("NTDLL"),"NtSetInformationThread",Proxy_NtSetInformationThread,HOOKTYPE_PUSH);
    }
    if (pk->contains(xvm_vid_xvmdebug))
    {
        NativeApiInit();
        EnableXVMDebug();
        //xvm_setbreakpoint(0x468B9A,1,xvm_bp_execute);
    }
    if (pk->contains(xvm_vid_vmexecinit))
    {
        VMExecEnableException();
    }
    if (pk->contains(xvm_vid_virtualkernel32))
    {
        initVirtualKernel32();
    }
    if (pk->contains(xvm_vid_moduleHook))
    {
        char* lpOptions = 0;
        int szOptions = 0;
        if (pk->getBinary(xvm_vid_moduleHook,lpOptions,szOptions))
        {
            NPacketBase pkOptions(lpOptions,szOptions,false);
            EnableModuleHook(&pkOptions);
        }
    }

}

int waitAndDispatchMsg(NPacketBase* pk)
{
    int state = -1;
    while(gpipe->fpRead(pk))
    {
        state = 0;
        BOOL dispatched = TRUE;
        switch(pk->getMsgID())
        {
        case xvm_ack_init:
        {
            dispatchInit(pk);
            if (dispatchContinueOpt(pk) > 0)
                continue;
            return 0;
        }break;
        case xvm_ack_doaction:
        {
            dispatchContinueOpt(pk);
            continue;
        }break;
        case xvm_ack_continue:
        {
            dispatchContinueOpt(pk);
            return 0;
        }break;
        case xvm_ack_break:
        {
            dispatchContinueOpt(pk);
            DebugBreak();
            return 0;
        }break;
        case xvm_ack_loopbreak:
        {
            dispatchContinueOpt(pk);
            foreverLoop();
            return 0;
        }break;
        default:dispatched = FALSE;break;
        }
        if (!dispatched)
        {
            state = 1;
            break;
        }
    }
    return state;
}

DWORD CALLBACK remoteActionEntry(void* lparg)
{
    if (!lparg)
        return -1;

    MEMORY_BASIC_INFORMATION mbi = {0};
    VirtualQuery(lparg,&mbi,sizeof(mbi));
    NPacketBase opt((const char*)lparg,mbi.RegionSize?mbi.RegionSize:0x1000,true);
    switch(opt.getMsgID())
    {
    case xvm_ack_spyrsrc:
    {
        dispatchResourceBuild(&opt);
    }break;
    case xvm_ack_takememsnapshot:
    {
        dispatchTakeMemorySnapshot(&opt);
    }break;
    }
    VirtualFree(lparg,0,MEM_RELEASE);
    return 0;
}

void initPipeClient(BOOL ldrcall)
{
    if (gpipe)
        return;

    gpipe = new NPipeClient;
    gpipe->SetPipeName(_T(XVMRUNTIME_COM_PIPENAME_FULL));
    //启用同步通信模式
    gpipe->OpenSyncInst();
    //向xApiSpy发送进程启动通知
    NPacketBase pk(xvm_req_init);
    pk.putEmpty(ldrcall?xvm_vid_ldrcall:xvm_vid_oepcall);

    //这里我们将进程的主文件信息收集并传递给xApiSpy监视器
    wchar_t modpath[MAX_PATH*2];
    modpath[0] = 0;
    wchar_t* modname = 0;
    HMODULE hmod = GetModuleHandle(0);
    DWORD nlen = GetModuleFileNameW(hmod,modpath,sizeof(modpath)/sizeof(wchar_t));
    if (nlen > 0)
    {
        int pos=0;
        for (pos=nlen-1;pos>0;pos--)
        {
            if (modpath[pos] == '\\' || modpath[pos] == '/')
            {
                pos++;
                break;
            }
        }
        modname = modpath+pos;
        pk.putWString(xvm_vid_modname,modname);
        pk.putWString(xvm_vid_modpath,modpath);
    }
    pk.putInt64(xvm_vid_modulebase,(ULONG_PTR)GetModuleHandle(0));
    pk.putInt64(xvm_vid_rtentry,(ULONG_PTR)remoteActionEntry);
	pk.putInt(xvm_vid_processID,GetCurrentProcessId());
    gpipe->fpWrite(&pk);
    sendModuleApiInfos(hmod,modname,modpath);
    //等待xApiSpy回复
    waitAndDispatchMsg(&pk);
}


void fn_script_error_func(void *lparg,const char *msg)
{
	if (!msg) return;
	NPacketBase pk(xvm_notice_scp_error);
	pk.putString(xvm_vid_errdesc,msg);
	gAlertPipe->fpWrite(&pk);
}

#include <stdarg.h>
int fn_script_printf(const char * _Format,...)
{
	va_list arglist;
	va_start(arglist,_Format);
	char sbuf[1024];
	int nlen = _vsprintf_s_l(sbuf,sizeof(sbuf),_Format,NULL,arglist);
	if (nlen > 0)
	{
		sbuf[nlen] = 0;
		NPacketBase pk(xvm_notice_scp_error);
		pk.putString(xvm_vid_errdesc,sbuf);
		gAlertPipe->fpWrite(&pk);

	}
	return nlen;
}


int fn_script_addrprintf(long addr,const char * _Format,...)
{
	va_list arglist;
	va_start(arglist,_Format);
	char sbuf[1024];
	int nlen = _vsprintf_s_l(sbuf,sizeof(sbuf),_Format,NULL,arglist);
	if (nlen > 0)
	{
		sbuf[nlen] = 0;
		NPacketBase pk(xvm_notice_scp_error);
		pk.putInt64(xvm_vid_address,addr);
		pk.putString(xvm_vid_errdesc,sbuf);
		gAlertPipe->fpWrite(&pk);

	}
	return nlen;
}
void fn_raise_loaddllexception(void* hModule,const wchar_t* lpModPath)
{
	NPacketBase pk(xvm_notice_raiseexception);
	NPacketBase pkopt(LOAD_DLL_DEBUG_EVENT);	//将各种信息打包
	pkopt.putInt64(xvm_vid_modulebase,(long long)hModule);
	pkopt.putInt64(xvm_vid_path,(long long)&lpModPath);
		//取出模块的入口，大小，基址等信息一并打包
	PIMAGE_NT_HEADERS imnh = EnterImageNtHeader((const char*)hModule);
	pkopt.putInt64(xvm_vid_imgbase,imnh->OptionalHeader.ImageBase);
	pkopt.putInt64(xvm_vid_entrypoint,imnh->OptionalHeader.AddressOfEntryPoint);
	pkopt.putInt(xvm_vid_size,imnh->OptionalHeader.SizeOfImage);
	pkopt.putWString(xvm_vid_modpath,lpModPath);
	pk.putBinary(xvm_vid_subpacket,pkopt.data(),pkopt.size());
	gAlertPipe->fpWrite(&pk);	//通过通知管道通知调试器
	OutputDebugStringA("");	//利用outputDebugString来触发一个异常
}


void dbg_raisePauseException()
{
	NPacketBase pk(xvm_notice_raiseexception);
	NPacketBase pkopt(EXCEPTION_DEBUG_EVENT);	//将各种信息打包
	//取出模块的入口，大小，基址等信息一并打包
	pk.putBinary(xvm_vid_subpacket,pkopt.data(),pkopt.size());
	gAlertPipe->fpWrite(&pk);	//通过通知管道通知调试器
	OutputDebugStringA("");	//利用outputDebugString来触发一个异常
}

xHook_VMExec* globalCPU = 0;
int VMExecUnderDBGCleanupCallBack(void** lpEsp,void** lpRet,xHook_VMExec* xcpu)
{
	if (!xcpu) return 0;
	NPacketBase pk(xvm_notice_outvm);
	pk.putInt64(xvm_vid_rtouter,(ULONG_PTR)*lpRet);
	pk.putInt64(xvm_vid_count,xcpu->icount);
	gAlertPipe->fpWrite(&pk);
	
	long flags = (long)xcpu->userData();
	if (flags & XVM_FLAG_SENDRECORDSBYPIPE)
	{

		pk.setempty(false);
		pk.alloc(sizeof(TraceRecordEx)* 1100);
		pk.setMsgID(xvm_notice_records);
		NTraceRecords* rcs = xcpu->Records();
		int nc = 0;
		pk.putEmpty(xvm_vid_begin);
		for (TraceRecordEx* tr = rcs->first(); tr; tr = rcs->next(tr))
		{
			pk.putBinary(xvm_vid_vmrecord,(const char*)tr,sizeof(TraceRecordEx));
			if (nc++ >= 1000)
			{
				gAlertPipe->fpWrite(&pk);
				pk.setempty(false);
				pk.setMsgID(xvm_notice_records);
				nc = 0;
			}
		}
		pk.putEmpty(xvm_vid_finished);
		gAlertPipe->fpWrite(&pk);
	}

	if ((flags & XVM_FLAG_SENDBLOCKBYPIPE) || !gblockfile.empty())
	{
		NPacketBase pkbody;
		xcpu->saveBlockRecords(&pkbody);
		pk.setempty(false);
		pk.setMsgID(xvm_notice_blocks);
		pk.putBinary(xvm_vid_bkfile,pkbody.data(),pkbody.size());
		if ((flags & XVM_FLAG_SENDBLOCKBYPIPE))
		{
			gAlertPipe->fpWrite(&pk);
		}
		if (!gblockfile.empty())
		{
			HANDLE hFile = CreateFileW(gblockfile.c_str(),GENERIC_READ | GENERIC_WRITE,0,0,OPEN_ALWAYS,0,0);
			if (hFile != INVALID_HANDLE_VALUE)
			{
				SetFilePointer(hFile,0,0,FILE_BEGIN);
				SetEndOfFile(hFile);
				DWORD dwWted = 0;
				WriteFile(hFile,pkbody.data(),pkbody.size(),&dwWted,0);
				SetEndOfFile(hFile);
				CloseHandle(hFile);
			}
		}
	}
	if (flags & XVM_FLAG_RECORDBLOCKEXEC)
	{
		pk.setempty(false);
		pk.alloc(sizeof(SequenceExecute)* 1000);
		pk.setMsgID(xvm_notice_bklogs);

		NInstSequenceExecLogs* rcs = xcpu->seqExecLogs();
		int nc = 0;
		pk.putEmpty(xvm_vid_begin);
		for (SequenceExecute* tr = rcs->first(); tr; tr = rcs->next(tr))
		{
			pk.putBinary(xvm_vid_blockexeclog,(const char*)tr,sizeof(SequenceExecute));
			if (nc++ >= 900)
			{
				gAlertPipe->fpWrite(&pk);
				pk.setempty(false);
				pk.setMsgID(xvm_notice_bklogs);
				nc = 0;
			}
		}
		pk.putEmpty(xvm_vid_finished);
		gAlertPipe->fpWrite(&pk);
	}
	pk.setempty(false);
	pk.setMsgID(xvm_notice_echo);
	gAlertPipe->fpWrite(&pk);
	if (globalCPU) return 1;
	return 0;
}


void processPendingPacket(void** realEsp,void** lpRet,void** lpEsp)
{

	const char* lppk = (const char*)((BYTE*)*lpEsp + (sizeof(ULONG_PTR)* 12));
	int szpk = *(ULONG_PTR*)((BYTE*)*lpEsp + (sizeof(ULONG_PTR)* 11));
	NPacketBase pk(lppk,szpk);

	switch (pk.getMsgID())
	{
	case xvm_notice_runscript:	//接收到运行脚本封包
	{
		char* lpopt = 0;
		int szopt = 0;
		if (pk.getBinary(xvm_vid_scriptopt,lpopt,szopt))
		{	//取出脚本参数
			NPacketBase pkopt(lpopt,szopt,false);
			//编译并运行脚本
			int state = xvm_run_script(&pkopt,0,fn_script_error_func);
			setPendingPacketExecState(state);
		}	
	}break;
	case xvm_notice_entervm:
	{
		char* lpopt;
		int szopt;
		if (pk.getBinary(xvm_vid_vmopt,lpopt,szopt))
		{
			NPacketBase pkopt(lpopt,szopt,false);
			ULONG_PTR addr = pkopt.getLongLong(xvm_vid_address);
			if (addr > 0)
			{	//首先我们设定一般的虚拟执行参数
				setVMOptions(&pkopt,VMExecUnderDBGCleanupCallBack);
				char* lpopt = 0;
				int szopt = 0;
				if (pkopt.getBinary(xvm_vid_scriptopt,lpopt,szopt))
				{	//这里我们检测是否有实时监控脚本
					if (!globalCPU)
					{
						NPacketBase pkscp(lpopt,szopt,false);
						TCCState* tcInst = 0;	//首先我们先运行脚本
						int state = xvm_run_script(&pkscp,&tcInst,fn_script_error_func);
						if (state == main_ret_keep || state == main_ret_global)	//如果脚本成功编译并运行，那么会返回脚本实例
						{	//这里我们测试并设定回调符号
							xHook_VMExec* xcpu = VMExecCPU(FALSE);
							if (xcpu)
							{	//如果虚拟机也成功实例化了，那么我们获取相应的回调符号位置
								void* lpbs = tcc_get_symbol(tcInst,"vmexec_before_step");
								void* lpas = tcc_get_symbol(tcInst,"vmexec_after_step");
								void* lpbl = tcc_get_symbol(tcInst,"vmexec_before_loop");
								void* lpal = tcc_get_symbol(tcInst,"vmexec_after_loop");
								//这里我们调用函数设定
								xcpu->setScriptCallbacks((scp_cb_vmexec_before_step)lpbs,
									(scp_cb_vmexec_after_step)lpas,
									(scp_cb_vmexec_before_loop)lpbl,
									(scp_cb_vmexec_after_loop)lpal);
								if (state == main_ret_global)
								{
									globalCPU = xcpu;
								}
							}
						}
						//将执行状态通知调试器
						setPendingPacketExecState(state);
					}
				}

				ULONG_PTR lpREsp = (ULONG_PTR)*realEsp;
				lpREsp -= sizeof(ULONG_PTR);
				*(ULONG_PTR*)(lpREsp) = addr;
				*lpRet = VMExecEntry;
				*realEsp = (void*)lpREsp;
			}
		}
	}break;
	case xvm_notice_dbgdirect:
	{
		char* lpopt = 0;
		int szopt = 0;
		if (pk.getBinary(xvm_vid_options,lpopt,szopt))
		{	
			NPacketBase pkopt(lpopt,szopt,false);
			gBlockBPs.cleanup();
			gDbgRunState = pkopt.getInt(xvm_vid_state);
			char* lpbps;
			int szbps;
			if (pkopt.getBinary(xvm_vid_breakpoints,lpbps,szbps))
			{
				NPacketBase pkbps(lpbps,szbps,false);
				char* lpdata = pkbps.tofirst(xvm_vid_break);
				while (lpdata)
				{
					ULONG_PTR ip = pkbps.toInt();
					opcode_bp* opbp = gBlockBPs.new_back(ip);
					opbp->ip = ip;
					lpdata = pkbps.tonext(xvm_vid_break);
				}
			}
		}
	}break;
	}

}


extern "C" void __stdcall checkForPendingPacket(void* realEsp,void* lpRet,void* lpEsp)
{
	processPendingPacket(&realEsp,&lpRet,&lpEsp);
	checkPendingPacketOuter(lpEsp,lpRet,realEsp);
}


void initAlertPipe(BOOL ldrcall,xvmrutime_args* args)
{
	if (gAlertPipe) return;
	gAlertPipe = new NPipeClient;
	TCHAR name[64];
	_stprintf(name,XVMRUNTIME_COM_NOTICEPIPE,args->hostProcessID);
	gAlertPipe->SetPipeName(name);
	//启用同步通信模式
	gAlertPipe->OpenSyncInst(0,FALSE);
	NPacketBase pk(xvm_notice_init);
	pk.putInt64(xvm_vid_rtentry,(long long)checkPendingPacketEntry);
	void* lpOuter = checkPendingPacketOuter(0,0,0);
	pk.putInt64(xvm_vid_rtouter,(long long)lpOuter);
	gAlertPipe->fpWrite(&pk);
}
extern "C" void* WINAPI OEPEntryCallBack(void* lpEsp)
{
    //先锁定，防止多线程访问冲突
    gPipeLock.Lock();
    //整理打包参数传送给xApiSpy程式进行记录
    NPacketBase pk(xvm_req_oepcall);
    pk.putInt64(xvm_vid_address,(ULONG_PTR)gOEPEntry);
    gpipe->fpWrite(&pk);
    int state = waitAndDispatchMsg(&pk);
    gPipeLock.Unlock();
    if (state > 0)
    {
        switch(pk.getMsgID())
        {
        case xvm_ack_entervm:
        {
            char* lpopt;
            int szopt;
            if (pk.getBinary(xvm_vid_continueopt,lpopt,szopt))
            {
                NPacketBase pkopt(lpopt,szopt,false);
                ULONG_PTR addr = pkopt.getLongLong(xvm_vid_address);
                if (addr > 0)
                {   //设定我们的虚拟机选项，也就是界面中的各种参数
					setVMOptions(&pkopt,VMExecCleanupCallBack);
                    int szbuf = 36;
                    *(ULONG_PTR*)((ULONG_PTR)lpEsp + szbuf + sizeof(ULONG_PTR)) = (ULONG_PTR)addr;
                    //直接通过我们xVMRuntime模块的出口函数恢复线程上下文到
                    //我们的虚拟机入口函数VMExecEntry执行
                    apiStubEnd((void*)lpEsp,VMExecEntry);
                }
            }
        }break;
        }
    }

    apiStubEnd(lpEsp,gOEPEntry);
    return 0;
}
//初始化伪调试CRT
void InitVirtualDebugCRT(BOOL ldrcall,void** lpEsp,void** lpRet)
{
    if (lpRet) glpOrgEntry = *lpRet;
    //由于我们的模块式虚拟载入的，而且我们使用到了异常，因此无条件关闭SafeSEH
    disableProcessSafeSEH();
    initVirtualKernel32();
    NativeApiInit();    //获取使用到的本地函数地址信息
    BOOL bListen = StartDebugListing(); //连接调试器并启动调试
    if (bListen)
    {
        //将返回地址设定到sysBreakBeforeEntry函数上
        if (lpRet)
            *lpRet = sysBreakBeforeEntry;//该函数实际上是在转入原始入口地址前模拟一个INT3以达到中断目的
    }
}
extern "C"{
	void* getPEB();
}

static LPNtQueryObject glpNtQueryObject = 0;

BOOL isVistaAbove()
{
	OSVERSIONINFO osv = { 0 };
	osv.dwOSVersionInfoSize = sizeof(osv);
	GetVersionEx(&osv);
	return (osv.dwMajorVersion > 5);
}

void clearPEBAndHeapDebugFlag()
{
	BYTE* lpPEB = (BYTE*)getPEB();
	*(lpPEB + 2) = 0;	//DebugPresent
	*(DWORD*)(lpPEB + 0x68) &= 0xFFFFFF8F;	//0x68
	BOOL isWow64 = FALSE;
	static LPIsWow64Process lpIsWow64Process = 0;
	if (!lpIsWow64Process)
		lpIsWow64Process = (LPIsWow64Process)GetProcAddress(GetModuleHandle(_T("KERNEL32")),"IsWow64Process");
	if (lpIsWow64Process)
	{
		BOOL osCalled = lpIsWow64Process(GetCurrentProcess(),&isWow64);
		if (osCalled && isWow64)
		{
			BYTE* lpVar = lpPEB + 0x1002;
			DWORD oldPG = 0;
			if (VirtualProtect(lpVar,0x100,PAGE_EXECUTE_READWRITE,&oldPG))
			{
				*lpVar = 0;
				*(DWORD*)(lpPEB + 0x10BC) &= 0xFFFFFF8F;
				VirtualProtect(lpVar,0x100,oldPG,&oldPG);
			}
		}
	}
	BYTE* lpHeap = *(BYTE**)(lpPEB + 0x18);
	if (isVistaAbove())
	{
		*(DWORD*)(lpHeap + 0x40) &= 2;
		*(DWORD*)(lpHeap + 0x44) = 0;
	} else
	{
		*(DWORD*)(lpHeap + 0xC) &= 2;
		*(DWORD*)(lpHeap + 0x10) = 0;
	}
}


syscall_hook_ret NativeFindWindowHookProc(xhook_stackregs* stregs,void** orgEntry,void* userdata,void** uargs,BOOL afterCall)
{
	struct nativeFindWindowArgs : public syscall_stack_hdr
	{
		void* arg1;
		void* arg2;
		PUNICODE_STRING strWndClass;
		PUNICODE_STRING strWndName;
		void* arg5;
	};
	nativeFindWindowArgs* lparg = (nativeFindWindowArgs*)stregs->rsp;
	if (lparg->strWndClass && lparg->strWndClass->Buffer && _wcsnicmp(lparg->strWndClass->Buffer,L"OLLYDBG",7) == 0)
	{
		stregs->rax = 0;
		return syscall_ret_skip;
	}
	return syscall_ret_call;
}

syscall_hook_ret NativeNtQueryInformationProcessHookProc(xhook_stackregs* stregs,void** orgEntry,void* userdata,void** uargs,BOOL afterCall)
{
	struct nativeArgs : public syscall_stack_hdr
	{
		HANDLE ProcessHandle;
		PROCESSINFOCLASS ProcessInformationClass;
		PVOID ProcessInformation;
		ULONG ProcessInformationLength;
		PULONG ReturnLength;
	};
	nativeArgs* lparg = (nativeArgs*)stregs->rsp;
	switch (lparg->ProcessInformationClass)
	{
	case 0x7:// ProcessDebugPort
	{
		if (lparg->ProcessInformation && lparg->ProcessInformationLength == sizeof(DWORD_PTR))
		{
			*(DWORD_PTR*)lparg->ProcessInformation = FALSE;
			stregs->rax = 0;
			return syscall_ret_skip;
		}
	}break;
	case 0x1f://ProcessDebugFlags
	{
		if (lparg->ProcessInformation && lparg->ProcessInformationLength == sizeof(DWORD))
		{
			*(DWORD*)lparg->ProcessInformation = TRUE;
			stregs->rax = 0;
			return syscall_ret_skip;
		}
	}break;
	case 0x1E:	//ProcessDebugHandle
	{
		stregs->rax = 0xC0000353;
		return syscall_ret_skip;
	}break;
	//case 0x22:	//we check here if just pass the system breakpoint
	//{

	//}break;
	}
	return syscall_ret_call;
}

int	gTLSFixState = 0;

syscall_hook_ret NativeNtSetInformationProcessHookProc(xhook_stackregs* stregs,void** orgEntry,void* userdata,void** uargs,BOOL afterCall)
{
	struct nativeArgs : public syscall_stack_hdr
	{
		IN HANDLE hProcess;
		IN PROCESSINFOCLASS ProcessInfoClass;
		IN PVOID ProcessInfoBuffer;
		IN ULONG ProcessInfoBufferLength;
	};
	nativeArgs* lparg = (nativeArgs*)stregs->rsp;
	switch (lparg->ProcessInfoClass)
	{
	case 0x22:// ProcessExecutionFlags
	{
		static BOOL gbCleared = FALSE;
		if (!gbCleared)
		{
			clearPEBAndHeapDebugFlag();
			gbCleared = TRUE;
		}
		if (gTLSFixState == 1)
		{
			gTLSFixState++;
			//这里我们修复因为过早初始化kernel32引起的TLS不会被回调的问题
			resetModuleLoaderFlags(GetModuleHandle(_T("KERNELBASE")));
		}
	}break;
	}
	return syscall_ret_call;
}


syscall_hook_ret NativeNtSetInformationThreadHookProc(xhook_stackregs* stregs,void** orgEntry,void* userdata,void** uargs,BOOL afterCall)
{
	struct nativeArgs : public syscall_stack_hdr
	{
		HANDLE hThread;
		THREADINFOCLASS ThreadInfoClass;
		PVOID ThreadInfoBuffer;
		ULONG ThreadInfoBufferLength;
	};
	nativeArgs* lparg = (nativeArgs*)stregs->rsp;
	if (lparg->ThreadInfoClass == ThreadHideFromDebugger && !lparg->ThreadInfoBuffer && !lparg->ThreadInfoBufferLength)
	{
		if (!glpNtQueryObject)
			glpNtQueryObject = (LPNtQueryObject)GetProcAddress(GetModuleHandle(_T("NTDLL")),"NtQueryObject");

		PUBLIC_OBJECT_BASIC_INFORMATION obi;
		ULONG rlen = 0;
		NTSTATUS iret = glpNtQueryObject(lparg->hThread,ObjectBasicInformation,&obi,sizeof(PUBLIC_OBJECT_BASIC_INFORMATION),&rlen);
		stregs->rax = iret;
		return syscall_ret_skip;
	}
	return syscall_ret_call;
}
syscall_hook_ret NativeNtQuerySystemInformationHookProc(xhook_stackregs* stregs,void** orgEntry,void* userdata,void** uargs,BOOL afterCall)
{
	struct nativeArgs : public syscall_stack_hdr
	{
		SYSTEM_INFORMATION_CLASS SystemInformationClass;
		PVOID SystemInformation;
		ULONG SystemInformationLength;
		PULONG ReturnLength;
	};
	nativeArgs* lparg = (nativeArgs*)stregs->rsp;
	typedef struct _SYSTEM_KERNEL_DEBUGGER_INFORMATION
	{
		BOOLEAN DebuggerEnabled;
		BOOLEAN DebuggerNotPresent;
	} SYSTEM_KERNEL_DEBUGGER_INFORMATION,*PSYSTEM_KERNEL_DEBUGGER_INFORMATION;
	if (lparg->SystemInformationClass == 35 /*SystemKernelDebuggerInformation*/ && lparg->SystemInformation && lparg->SystemInformationLength >= sizeof(SYSTEM_KERNEL_DEBUGGER_INFORMATION))
	{
		stregs->rax = 0;

		SYSTEM_KERNEL_DEBUGGER_INFORMATION* DebuggerInfo = (SYSTEM_KERNEL_DEBUGGER_INFORMATION*)lparg->SystemInformation;
		DebuggerInfo->DebuggerEnabled = FALSE;
		DebuggerInfo->DebuggerNotPresent = TRUE;
		return syscall_ret_skip;
	}
	return syscall_ret_call;
}

//structures
typedef struct _OBJECT_TYPE_INFORMATION
{
	UNICODE_STRING TypeName;
	ULONG TotalNumberOfHandles;
	ULONG TotalNumberOfObjects;
} OBJECT_TYPE_INFORMATION,*POBJECT_TYPE_INFORMATION;

typedef struct _OBJECT_ALL_INFORMATION
{
	ULONG NumberOfObjects;
	OBJECT_TYPE_INFORMATION ObjectTypeInformation[1];
} OBJECT_ALL_INFORMATION,*POBJECT_ALL_INFORMATION;

//enums
enum _OBJECT_INFORMATION_CLASS_
{
	ObjectAllInformation = 3
};

syscall_hook_ret NativeNtQueryObjectHookProc(xhook_stackregs* stregs,void** orgEntry,void* userdata,void** uargs,BOOL afterCall)
{
	struct nativeArgs : public syscall_stack_hdr
	{
		HANDLE Handle;
		OBJECT_INFORMATION_CLASS ObjectInformationClass;
		PVOID ObjectInformation;
		ULONG ObjectInformationLength;
		PULONG ReturnLength;
	};
	if (!afterCall) return syscall_ret_call;

	nativeArgs* lparg = (nativeArgs*)stregs->rsp;
	if (NT_SUCCESS(stregs->rax) && lparg->ObjectInformation)
	{
		if (lparg->ObjectInformationClass == ObjectTypeInformation)
		{
			OBJECT_TYPE_INFORMATION* pObjectTypeInfo = (OBJECT_TYPE_INFORMATION*)lparg->ObjectInformation;
			if (pObjectTypeInfo->TypeName.Length > 0 && pObjectTypeInfo->TypeName.Buffer && wcsnicmp(pObjectTypeInfo->TypeName.Buffer,L"DebugObject",11) == 0)
			{
				pObjectTypeInfo->TotalNumberOfObjects = 0;
				pObjectTypeInfo->TotalNumberOfHandles = 0;
			}
		} else if (lparg->ObjectInformationClass == ObjectAllInformation)
		{
			OBJECT_ALL_INFORMATION* pObjectAllInfo = (OBJECT_ALL_INFORMATION*)lparg->ObjectInformation;
			unsigned char* pObjInfoLocation = (unsigned char*)pObjectAllInfo->ObjectTypeInformation;
			unsigned int TotalObjects = pObjectAllInfo->NumberOfObjects;
			for (unsigned int i = 0; i < TotalObjects; i++)
			{
				OBJECT_TYPE_INFORMATION* pObjectTypeInfo = (OBJECT_TYPE_INFORMATION*)pObjInfoLocation;
				if (pObjectTypeInfo->TypeName.Length > 0 && pObjectTypeInfo->TypeName.Buffer && wcsnicmp(pObjectTypeInfo->TypeName.Buffer,L"DebugObject",11) == 0)
				{
						pObjectTypeInfo->TotalNumberOfObjects = 0;
						//Bug found by Aguila, thanks!
						pObjectTypeInfo->TotalNumberOfHandles = 0;
				}
				pObjInfoLocation = (unsigned char*)pObjectTypeInfo->TypeName.Buffer;
				pObjInfoLocation += pObjectTypeInfo->TypeName.MaximumLength;
				ULONG_PTR tmp = ((ULONG_PTR)pObjInfoLocation) & -(LONG_PTR)sizeof(void*);
				if ((ULONG_PTR)tmp != (ULONG_PTR)pObjInfoLocation)
					tmp += sizeof(void*);
				pObjInfoLocation = ((unsigned char*)tmp);
			}
		}
	}

	//if (lparg->SystemInformationClass == 35 /*SystemKernelDebuggerInformation*/ && lparg->SystemInformation && lparg->SystemInformationLength >= sizeof(SYSTEM_KERNEL_DEBUGGER_INFORMATION))
	//{
	//	stregs->rax = 0;

	//	SYSTEM_KERNEL_DEBUGGER_INFORMATION* DebuggerInfo = (SYSTEM_KERNEL_DEBUGGER_INFORMATION*)lparg->SystemInformation;
	//	DebuggerInfo->DebuggerEnabled = FALSE;
	//	DebuggerInfo->DebuggerNotPresent = TRUE;
	//	return syscall_ret_skip;
	//}
	return syscall_ret_call;
}

syscall_hook_ret NativeNtCloseHookProc(xhook_stackregs* stregs,void** orgEntry,void* userdata,void** uargs,BOOL afterCall)
{
	struct nativeArgs : public syscall_stack_hdr
	{
		HANDLE Handle;
	};
	nativeArgs* lparg = (nativeArgs*)stregs->rsp;


	if (!glpNtQueryObject)
		glpNtQueryObject = (LPNtQueryObject)GetProcAddress(GetModuleHandle(_T("NTDLL")),"NtQueryObject");

	PUBLIC_OBJECT_BASIC_INFORMATION obi;
	ULONG rlen = 0;
	NTSTATUS iret = glpNtQueryObject(lparg->Handle,ObjectBasicInformation,&obi,sizeof(PUBLIC_OBJECT_BASIC_INFORMATION),&rlen);
	if (!NT_SUCCESS(iret))
	{
		stregs->rax = 0xC0000008;
		return syscall_ret_skip;
	}
	return syscall_ret_call;
}


syscall_hook_ret NativeNtSetContextThreadProc(xhook_stackregs* stregs,void** orgEntry,void* userdata,void** uargs,BOOL afterCall)
{
	struct nativeArgs : public syscall_stack_hdr
	{
		HANDLE ThreadHandle;
		PCONTEXT Context;
	};
	nativeArgs* lparg = (nativeArgs*)stregs->rsp;

	if (!afterCall)
	{
		*uargs = (void*)lparg->Context->ContextFlags;
		lparg->Context->ContextFlags &= ~0x10; //CONTEXT_DEBUG_REGISTERS ^ CONTEXT_AMD64/CONTEXT_i386
		return syscall_ret_call;
	} else
	{
		lparg->Context->ContextFlags = (ULONG)*uargs;
		return syscall_ret_skip;
	}
	return syscall_ret_call;
}


void setupAntiAntiDebug(xvmrutime_args* args)
{
	//findwindowex
	if (args->sysc.iFindowWindow >= 0)
		setupMoniterNativeApi(args->sysc.iFindowWindow,NativeFindWindowHookProc);
	//NtQueryInformationProcess
	if (args->sysc.iNtQueryInformationProcess >= 0)
		setupMoniterNativeApi(args->sysc.iNtQueryInformationProcess,NativeNtQueryInformationProcessHookProc,MONITER_FLAG_CALLAFTER);
	//NtSetInformationThread
	if (args->sysc.iNtSetInformationThread >= 0)
		setupMoniterNativeApi(args->sysc.iNtSetInformationThread,NativeNtSetInformationThreadHookProc);
	//NtQuerySystemInformation
	if (args->sysc.iNtQuerySystemInformation >= 0)
		setupMoniterNativeApi(args->sysc.iNtQuerySystemInformation,NativeNtQuerySystemInformationHookProc,MONITER_FLAG_CALLAFTER);
	//NtQueryObject
	if (args->sysc.iNtQueryObject >= 0)
		setupMoniterNativeApi(args->sysc.iNtQueryObject,NativeNtQueryObjectHookProc,MONITER_FLAG_CALLAFTER);
	//NtClose
	if (args->sysc.iNtClose >= 0)
		setupMoniterNativeApi(args->sysc.iNtClose,NativeNtCloseHookProc);
	//NtSetContextThread
	//if (args->sysc.iNtSetContextThread >= 0)
		//setupMoniterNativeApi(args->sysc.iNtSetContextThread,NativeNtSetContextThreadProc,MONITER_FLAG_CALLALL);
	if (args->sysc.iNtSetInformationProcess >= 0)
		setupMoniterNativeApi(args->sysc.iNtSetInformationProcess,NativeNtSetInformationProcessHookProc);
}

void initAtCall(HMODULE hNtdll,HMODULE hKrl32,xvmrutime_args* args,BOOL ldrcall,void** lpEsp,void** lpRet)
{
	if (args->bEnableVirtualKernel32)
		initVirtualKernel32();

	if (args->bDisableSafeSEH)
		disableProcessSafeSEH();
	if (args->bEnableAlertPipe)
		initAlertPipe(TRUE,args);

	if (args->bAntiAntiDebug)
		args->bSandbox = TRUE;
	if (args->bSandbox)
	{
		InitSandbox(hNtdll);
		EnableSandboxMoniter(hNtdll);

	}
	if (args->bAntiAntiDebug)
	{
		setupAntiAntiDebug(args);
	}
	if (args->bVirtualDebugMode)
	{
		InitVirtualDebugCRT(ldrcall,lpEsp,lpRet);
	}
	if (args->bEnableClientPipe)
		initPipeClient(ldrcall);
	//__asm int 3
	if (args->sdllmode != 0)
	{
		if (args->sdllmode == 2)
		{
			NAutoLargeBufferA nla;
			nla.setHostFile(args->startupDLL);
			if (nla.open(true,true))
			{
				int size = 0;
				const char* lpdata = nla.data(size,0,false);
				if (lpdata && size > 0)
				{
					LoadModuleInMemory(lpdata,size,0,0,0);
				}
			}
		} else
		{
			LoadLibraryW(args->startupDLL);
		}
	}

	if (args->szscppk > 0)
	{
		NPacketBase pkopt(args->startscppk,args->szscppk,false);
		args->szscppk = 0;
		int state = xvm_run_script(&pkopt,0,fn_script_error_func);
		
	}
	gTLSFixState = 1;
}
void* CALLBACK xhook_PlainInitThunk(HMODULE hNtdll,HMODULE hKrl32,int resion,void** lpEsp
                                    ,void** lpRet,void* lpargs,int szargs)
{
    xvmrutime_args* args = (xvmrutime_args*)lpargs;
    switch(resion)
    {
    case XHOOK_INITHUNK_RESION_OEPCALL:
    {
		gOEPEsp = (ULONG_PTR)*lpEsp + 40;
		gOEPEntry = *lpRet;
		if (args->bBreakAtOEP)
			*lpRet = apiOEPEntry;
		initAtCall(hNtdll,hKrl32,args,FALSE,lpEsp,lpRet);
    }break;
    case XHOOK_INITHUNK_RESION_LDRLOADDLLCALL:
    {
        if (!hKrl32)
        {
			if (args->bBreakOnLdr)
				sysInt3BreakPoint();
            if (args->bVirtualDebugMode)
                return  (void*)(XHOOK_INITHUNK_RETURN_LOADFULLIMPORT | XHOOK_INITHUNK_RETURN_INITCRT);
            return (void*)(XHOOK_INITHUNK_RETURN_IMPORTKERNEL32 | XHOOK_INITHUNK_RETURN_INITCRT);
        }
		initAtCall(hNtdll,hKrl32,args,TRUE,lpEsp,lpRet);
    }break;
    case XHOOK_INITHUNK_RESION_LDRTHUNKINITCALL:
    {
        if (args->bEnableHeapHook)
            EnableHeapHook(hNtdll);
    }break;
    }
    return (void*)XHOOK_INITHUNK_RETURN_NORMAL;
}

BOOL APIENTRY DllMain( HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        //AllocConsole();
        //gConsole = new CConsole();
        //gConsole->CreateConsole(_T("Recorder"));

        //        DebugBreak();
        //        EnableXVMDebug();
        //        HMODULE hm = LoadLibrary(_T("shell32.dll"));
        //        xvm_setbreakpoint((ULONG_PTR)hm+0x2000,1,xvm_bp_access);
        //        DWORD old = 0;
        //        VirtualProtect((LPVOID)((memaddr)hm+0x2000),0x2000,PAGE_EXECUTE_READWRITE,&old);

    }
    return TRUE;
}

