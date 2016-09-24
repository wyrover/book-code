#include <windows.h>
#include <winternl.h>
#include <TlHelp32.h>
#include <tchar.h>

#include "xapihookruntime.h"

#include "../../xHooklib/xhook_inline.h"
#include "../../xHooklib/xhook_dynamicsym.h"
#include "../../xHooklib/xhook_vmodule.h"
#include "../../xHooklib/xhook_pefuncs.h"
#include "../../xHooklib/xhook_crtfuncs.h"
#include "../../xHooklib/xhook_memory.h"
#include "../../xHooklib/xHooklibStub/xHooklibStub.h"
#include "../../../nCom/nlinkedlist.h"
#include "../../../nCom/npacketbase.h"
#include "../../../nCom/nsafelocker.h"
#include "../../xHooklib/xhook_typedef.h"

#if __cplusplus
extern "C"{
#endif
void  apiStubEntryNull(void* lpEntry);
void* glpSysEnter = 0;
#if __cplusplus
}
#endif


LPLdrGetProcedureAddressEx glpLdrGetProcedureAddressEx;
LPRtlImageDirectoryEntryToData glpRtlImageDirectoryEntryToData;
LPLdrLoadDll    glpLdrLoadDll;
#define MODINFO_BYPASS_FLAG_CALLERBYPASS    0x1

NSafeLocker gLock;
NAvlList<ULONG_PTR,xapi_modinfo_ptr>   gmods;
NAvlList<ULONG_PTR,xapi_apiinfo_ptr>   gapis;

xApiCallbacks                   gcbs = {0};

//中间层代码建立函数
void*  addHookRoutine(xapi_modinfo* modinfo,void* oriEntry,void* lpRVAPtr,DWORD orgRVA,const char* funcname,int base,long flags,void* userdt)
{
    //首先，根据原始函数入库，如果已经建立的函数，直接返回
    xapi_apiinfo_ptr* api = gapis.find((ULONG_PTR)oriEntry);
    if (api)
        return api->info->hookedEntry;
    //分配中转代码需要的内存空间
    int szxapi = sizeof(xapi_apiInfo)+(funcname?strlen(funcname):0)+1;
    int szUse = szxapi;
    char* lpPtr = 0;
    if (gcbs.amw4)
        lpPtr = (char*)gcbs.amw4(szUse,PAGE_EXECUTE_READWRITE);

    if (!lpPtr)
    {
        lpPtr = modinfo->lpPatch + modinfo->szPatchUsed;
        modinfo->szPatchUsed += szUse;
    }
    xapi_apiInfo* apinfo = (xapi_apiInfo*)lpPtr;
    memset(apinfo,0,szxapi);
    api = gapis.new_back((ULONG_PTR)oriEntry);
    api->info = apinfo;
    //直接写入硬编码，注意这里的硬编码只适用于32位程式
    *lpPtr = 0x68;
    *(ULONG_PTR*)(lpPtr+1) = (ULONG_PTR)lpPtr+11;
    *(lpPtr + 5) = 0x68;
    if (gcbs.tfcb && gcbs.tfcb(XAPI_FLAG_NULLSTUB,0,0))
        *(ULONG_PTR*)(lpPtr+6) = (ULONG_PTR)apiStubEntryNull;
    else
        *(ULONG_PTR*)(lpPtr+6) = (ULONG_PTR)apiStubEntry;
    *(lpPtr + 10) = 0xC3;
    *(lpPtr + 11) = 0x68;
    *(ULONG_PTR*)(lpPtr+12) = (ULONG_PTR)oriEntry;
    *(lpPtr + 16) = 0xC3;
    //保存中间层相关信息
    apinfo->oriEntry = oriEntry;
    apinfo->hookedEntry = lpPtr;
#ifdef _DEBUG
    apinfo->lpRVAPtr = lpRVAPtr;
#endif
    apinfo->flags = flags;
    apinfo->userdata = userdt;
    apinfo->orgRVA = orgRVA;
    apinfo->module = modinfo;
    apinfo->AllCallCount = 0;
    apinfo->name[0] = 0;
    apinfo->base = base;
    if (funcname)
        strcpy(apinfo->name,funcname);
    return lpPtr;
}

//模块导出表函数HOOK
int hookModuleExportTable(xapi_modinfo* modinfo,HMODULE hmod)
{
    if (modinfo == 0)
        return -1;
    const char* lpImage = (const char*)hmod;

    //首先判断是否具备导出表
    PIMAGE_DOS_HEADER	pimDH	=	(PIMAGE_DOS_HEADER)lpImage;
    PIMAGE_NT_HEADERS	pimNH	=	(PIMAGE_NT_HEADERS)((char*)lpImage+pimDH->e_lfanew);
    DWORD exportRVA = pimNH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
    PIMAGE_EXPORT_DIRECTORY	imED	=	(PIMAGE_EXPORT_DIRECTORY)(lpImage+exportRVA);
    long	pExportSize	=	pimNH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;
    if(pExportSize== 0 || (ULONG_PTR)imED <= (ULONG_PTR)lpImage)
        return -2;

    if (imED->NumberOfFunctions < 1)
        return -2;

    //计算我们中间层所需要的空间，并随后分配内存空间
    //we build a virtual module PE Header
    DWORD szCalc = (imED->NumberOfFunctions * sizeof(xapi_apiInfo)) + sizeof(IMAGE_EXPORT_DIRECTORY) + pExportSize + 0x1000;
#ifdef _M_IX86
    char* lpPatch = (char*)VirtualAlloc(0,szCalc,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
#else
    char* lpPatch = AllocateMemoryWithin4GB((const char*)hmod,szCalc,PAGE_EXECUTE_READWRITE);
#endif
    if (!lpPatch)
        return -3;
    modinfo->lpPatch = lpPatch;
    modinfo->szPatch = szCalc;
    modinfo->szPatchUsed = 0;
    modinfo->szofImage = pimNH->OptionalHeader.SizeOfImage;
    //我们总是建立一个虚拟PE头
    char* lpVPE = modinfo->lpPatch+modinfo->szPatchUsed;
    memcpy(lpVPE,(void*)hmod,0x1000);
    modinfo->szPatchUsed += 0x1000;
    modinfo->lpVExport = lpPatch + modinfo->szPatchUsed;

    PIMAGE_NT_HEADERS imvnh = EnterImageNtHeader(lpVPE);
    //PIMAGE_NT_HEADERS imnhorg = EnterImageNtHeader((char*)hmod);
    imvnh->OptionalHeader.AddressOfEntryPoint = (ULONG_PTR)hmod - (ULONG_PTR)lpVPE +  imvnh->OptionalHeader.AddressOfEntryPoint;
    imvnh->OptionalHeader.SizeOfImage = modinfo->szPatch;
    imvnh->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress = modinfo->szPatchUsed;
    imvnh->Signature = IMAGE_NT_SIGNATURE;
    //我们总是建立一个虚拟的导出表
    memcpy(modinfo->lpVExport,imED,sizeof(IMAGE_EXPORT_DIRECTORY));
    PIMAGE_EXPORT_DIRECTORY imEDNew = (PIMAGE_EXPORT_DIRECTORY)modinfo->lpVExport;

    DWORD delta = (ULONG_PTR)(lpPatch + modinfo->szPatchUsed) - (ULONG_PTR)hmod;
    imEDNew->AddressOfFunctions = delta + sizeof(IMAGE_EXPORT_DIRECTORY);
    DWORD* lpVFunc = (DWORD*)(lpImage + imEDNew->AddressOfFunctions);

    modinfo->szPatchUsed += sizeof(IMAGE_EXPORT_DIRECTORY) + (imED->NumberOfFunctions*sizeof(DWORD));
    DWORD* lpRvas = (DWORD*)(lpImage + imED->AddressOfFunctions);

    //这里我们拷贝中转函数的信息，因为系统判断中转函数信息是根据导出表范围来判断的
    //因此我们需要拷贝
    for (int i=0;i<imED->NumberOfFunctions;i++)
    {
        DWORD rvafunc = lpRvas[i];
        lpVFunc[i] = rvafunc;
        if (rvafunc >= exportRVA && rvafunc < (exportRVA + pExportSize))
        {
            const char* lpstr = lpImage + rvafunc;
            if ((modinfo->szPatchUsed % sizeof(ULONG_PTR)) != 0)
                modinfo->szPatchUsed += sizeof(ULONG_PTR) - (modinfo->szPatchUsed % sizeof(ULONG_PTR));
            strcpy(lpPatch+modinfo->szPatchUsed,lpstr);
            lpVFunc[i] = delta + modinfo->szPatchUsed;
            modinfo->szPatchUsed += strlen(lpstr)+1;
        }
    }

    modinfo->szVExport = modinfo->szPatchUsed-imvnh->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
    imvnh->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size = modinfo->szVExport;


    //进入模块导入表的HOOK
    PIMAGE_SECTION_HEADER ish = EnterImageFirstSection(lpImage);
    int nsec = pimNH->FileHeader.NumberOfSections;
    //这里我们测试全局配置，也就是触发onTestFlags事件来判断是否HOOK导出表
    BOOL hookExport = gcbs.tfcb?(gcbs.tfcb(XAPI_FLAG_HOOKEXPORT,modinfo,0) != 0):FALSE;

    DWORD oldPG = 0;
    DWORD szVir = imED->NumberOfFunctions*sizeof(DWORD);
    if (hookExport)
        VirtualProtect(lpRvas,szVir,PAGE_READWRITE,&oldPG);
    WORD* lpOrdinals = imED->AddressOfNameOrdinals?(WORD*)(lpImage+imED->AddressOfNameOrdinals):0;
    DWORD* lpNames =  imED->AddressOfNames?(DWORD*)(lpImage + imED->AddressOfNames):0;

    for (int i=0;i<imED->NumberOfFunctions;i++)
    {
        DWORD rvafunc = lpRvas[i];

        if (!(rvafunc >= exportRVA && rvafunc < (exportRVA + pExportSize)))
        {
            DWORD oftName = 0;
            if (lpNames && lpOrdinals)
            {
                for (int k=0;k<imED->NumberOfNames;k++)
                {
                    if (lpOrdinals[k] == i)
                    {
                        oftName = lpNames[k];
                        break;
                    }
                }
            }

            PIMAGE_SECTION_HEADER ishcur;
            BOOL hooked = FALSE;
            for (int j=0;j<nsec;j++)
            {
                ishcur = ish + j;
                if (rvafunc >= ishcur->VirtualAddress && rvafunc < (ishcur->VirtualAddress+ishcur->Misc.VirtualSize))
                {
                    if (ishcur->Characteristics & IMAGE_SCN_MEM_EXECUTE)
                    {
                        //这里我们就调用函数过滤过程，也就是触发onApiFilter，请求是否HOOK函数
                        if (gcbs.sacb)
                        {
                            long apiFlags = 0;
                            void* userdt = 0;
                            int iret = gcbs.sacb(modinfo,(void*)(lpImage+rvafunc),&lpRvas[i],rvafunc
                                                 ,oftName?(lpImage+oftName):0,imED->Base+i,false,&apiFlags,&userdt);
                            if (iret > 0)
                            {
                                //如果HOOK那么我们调用addHookRoutine来生成实际的中间层指令代码
                                void* vfret = addHookRoutine(modinfo,(void*)(lpImage+rvafunc),&lpRvas[i],rvafunc
                                                             ,oftName?(lpImage+oftName):0,imED->Base+i,apiFlags,userdt);
                                if (vfret)
                                {
                                    DWORD newrva = (ULONG_PTR)vfret - (ULONG_PTR)lpImage;

                                    if (hookExport && (iret & XAPI_FLAG_HOOKEXPORT))
                                        lpRvas[i] = newrva;
                                    if (iret & XAPI_FLAG_USEVEXPORT)
                                        lpVFunc[i] = newrva;
                                    hooked = TRUE;
                                }
                            }
                        }
                        break;

                    }
                }
            }

            if (!hooked)
            {
                if (gcbs.sacb)
                {
                    long apiFlags = 0;
                    void* userdt = 0;
                    int iret = gcbs.sacb(modinfo,(void*)(lpImage+rvafunc),&lpRvas[i],rvafunc
                                         ,oftName?(lpImage+oftName):0,imED->Base+i,true,&apiFlags,&userdt);
                    if (iret > 0)
                    {
                        void* vfret = addHookRoutine(modinfo,(void*)(lpImage+rvafunc),&lpRvas[i],rvafunc
                                                     ,oftName?(lpImage+oftName):0,imED->Base+i,apiFlags,userdt);
                        if (vfret)
                        {
                            DWORD newrva = (ULONG_PTR)vfret - (ULONG_PTR)lpImage;

                            if (hookExport && (iret & XAPI_FLAG_HOOKEXPORT))
                                lpRvas[i] = newrva;
                            if (iret & XAPI_FLAG_USEVEXPORT)
                                lpVFunc[i] = newrva;
                            hooked = TRUE;
                        }
                    }
                }
            }

        }
    }
    if (hookExport && oldPG != 0)
        VirtualProtect(lpRvas,imED->NumberOfFunctions*sizeof(DWORD),oldPG,&oldPG);
    return 0;
}

int Hook_ImportTable(HMODULE hmod)
{
    return 0;
}

//在该函数当中我们完成模块导出表HOOK的过滤工作
int Hook_ExportTable(HMODULE hmod,void** lpVExport,DWORD* szVExport)
{
    gLock.Lock();
    xapi_modinfo_ptr* infoptr = gmods.find((ULONG_PTR)hmod);
    gLock.Unlock();
    xapi_modinfo* modinfo = 0;
    if (!infoptr)
    {
        //filter here

        wchar_t modpath[MAX_PATH*4];
        modpath[0] = 0;
        wchar_t* modname = 0;
        DWORD nlen = GetModuleFileNameW(hmod,modpath,sizeof(modpath)/sizeof(wchar_t));
        if (nlen > 0)
        {
            int i=0;
            for (i=nlen-1;i>0;i--)
            {
                if (modpath[i] == '\\' || modpath[i] == '/')
                {
                    i++;
                    break;
                }
            }
            modname = modpath+i;
        }else
            return 0;
        //这里我们请求模块HOOK函数，其原型为
        //typedef int (CALLBACK* LPModuleFilterCallBack)(HMODULE hmod,
        //   const wchar_t* moduleName,
        //   const wchar_t* modulePath);

        if (gcbs.mfcb && gcbs.mfcb(hmod,modname,modpath) < 0)
            return 1;
        gLock.Lock();
        infoptr = gmods.find((ULONG_PTR)hmod);
        if (!infoptr)
        {
            int szmod = sizeof(xapi_modinfo)+((wcslen(modname)+1)*sizeof(wchar_t));
            modinfo = (xapi_modinfo*)malloc(szmod);
            int state = modinfo?0:-1;
            if (modinfo)
            {
                memset(modinfo,0,szmod);
                modinfo->modName[0] = 0;
                if (modname)
                    wcscpy(modinfo->modName,modname);
                modinfo->hmod = hmod;
                //在hookModuleExportTable函数中，我们完成模块导出表的具体HOOK工作
                state = hookModuleExportTable(modinfo,hmod);
                if (state < 0)
                {
                    free(modinfo);
                    modinfo = 0;
                }else
                {
                    infoptr = gmods.new_back((ULONG_PTR)hmod);
                    infoptr->info = modinfo;
                }
            }
            if (gcbs.mhedcb)
                gcbs.mhedcb(hmod,state,modinfo);
        }else
            modinfo = infoptr->info;
        gLock.Unlock();
    }else
        modinfo = infoptr->info;
    if (modinfo)
    {
        //这里说明模块导出表已经被HOOK了，那么我们可以执行一些相关的额外工作
        //例如我们根据设定可以使用虚拟导出表HOOK技术
        //gcbs.tfcb函数就是请求设定值
        BOOL useVExport = gcbs.tfcb?(gcbs.tfcb(XAPI_FLAG_USEVEXPORT,modinfo,0) != 0):TRUE;
        if (lpVExport)
        {
            *lpVExport = useVExport?modinfo->lpVExport:0;
        }
        if (szVExport)
            *szVExport =useVExport?modinfo->szVExport:0;
    }
    return 0;
}

void* WINAPI Hook_LPRtlImageDirectoryEntryToData(HMODULE hmod,void* a2,DWORD type,LPDWORD lpSize)
{
    void* lpRet = glpRtlImageDirectoryEntryToData(hmod,a2,type,lpSize);
    if (lpRet == 0)
        return lpRet;
    switch(type)
    {
    //we disable all bound import
    case IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT:
    case IMAGE_DIRECTORY_ENTRY_IAT:
    {
        lpRet = 0;
    }break;
    case IMAGE_DIRECTORY_ENTRY_EXPORT:
    {
        //__asm int 3
        void* lpVExport = 0;
        DWORD szVExport = 0;
        Hook_ExportTable(hmod,&lpVExport,&szVExport);
        if (lpVExport && szVExport > 0)
        {
            lpRet = lpVExport;
            *lpSize = szVExport;
        }

    }break;
    }


    return lpRet;
}

NTSTATUS NTAPI Hook_LPLdrLoadDll(wchar_t* EnvStr,void* unk1,PUNICODE_STRING path,HMODULE* hmod)
{
    NTSTATUS state = glpLdrLoadDll(EnvStr,unk1,path,hmod);
    if (hmod && *hmod != 0)
    {
        void* lpVExport = 0;
        DWORD szVExport = 0;
        Hook_ExportTable(*hmod,&lpVExport,&szVExport);
    }
    return state;
}

extern "C" void* WINAPI apiHookProcNull(void* lpCallRet,void* lpRet,void* lpEsp)
{
    apiStubEnd(lpEsp,lpRet);
    return 0;
}
static BOOL gUseDynamic = FALSE;
static xstub_dyFuncs gfuncs;
void* apiHookProcDynamic(xapi_apiInfo* api,void* lpCallRet,void* lpRet,void* lpEsp)
{
    if (!api->dyEntry)
    {
        if (!api->module->hmod)
        {
            PENTRY_LoadLibraryW lpLoadLibraryW = (PENTRY_LoadLibraryW)gfuncs.lpGetProcAddress(gfuncs.hker32,"LoadLibraryW");
            api->module->hmod = lpLoadLibraryW(api->module->modName);
        }
        if (api->name[0] != 0)
            api->dyEntry = gfuncs.lpGetProcAddress(api->module->hmod,api->name);
        else
            api->dyEntry = gfuncs.lpGetProcAddress(api->module->hmod,(LPCSTR)api->base);
    }
    apiStubEnd(lpEsp,api->dyEntry);
    return 0;
}

extern "C" void* WINAPI apiHookProc(void* lpCallRet,void* lpRet,void* lpEsp)
{
    xapi_apiInfo* api = (xapi_apiInfo*)((const char*)lpRet-11);
    //如果启用动态载入，转入载入函数
    if (gUseDynamic)
    {
        return apiHookProcDynamic(api,lpCallRet,lpRet,lpEsp);
    }
    api->AllCallCount++;
    if (gcbs.aecb)
    {
        void* lpEspReal = (void*)((char*)lpEsp+40);
#ifdef _M_IX64
        lpEspReal = (void*)((char*)lpEsp+136);
#endif
        lpRet = gcbs.aecb(api,lpCallRet,lpRet,lpEspReal,&lpEsp);
    }
    apiStubEnd(lpEsp,lpRet);
    return 0;
}

static xapi_entrycontext* xentry = 0;


void WINAPI snapshotOutProc(void* lpEsp)
{
    char stackhold[0x200];  //首先我们先抬高栈顶以保证有足够空间

    ULONG_PTR dwESP = (ULONG_PTR)lpEsp+40;
    ULONG_PTR* lpVal = (ULONG_PTR*)((BYTE*)xentry->stackData);
    ULONG_PTR* lpMask = (ULONG_PTR*)((BYTE*)xentry->stackMask);
    for (int i=0;i<xentry->stackDataSize/sizeof(ULONG_PTR);i++)
    {
        ULONG_PTR dwval = lpMask[i];
        if (dwval != 0)
        {
            lpVal[i] = (int)(dwESP-dwval);
        }
    }


    stackhold[0x1FF] = 1;   //防止被编译器优化
    //这里我们根据入口栈地址来计算新的栈顶
    BYTE* newStack = (BYTE*)dwESP - xentry->stackDataSize;
    memcpy(stackhold,xentry->stackData,1);
    xentry->newstack = newStack;
    //这里我们设定返回OEP
    if (xentry->restorEBP)
    {
        *(ULONG_PTR*)((BYTE*)xentry->stackData+8) = dwESP-xentry->ebpDelta;
    }
    //恢复转储时栈空间上的代码
    memcpy(newStack,xentry->stackData,xentry->stackDataSize);
    //执行跳转
    apiStubEnd(xentry->newstack,xentry->orgEntry);
}

extern "C" void* WINAPI snapshotEntryProc(void* lpRet,void* lpEsp)
{
    //我们打开动态载入标记，这样在调用函数中间层的时候会动态载入函数
    gUseDynamic = TRUE;
    //我们通过PEB来取得NTDLL模块基址，因为我们的快照PE实际上不需要导入表
    HMODULE hmod = (HMODULE)GetNtdllBasePEB();
    sys_memset(&gfuncs,0,sizeof(gfuncs));
    gfuncs.hNtdll = hmod;
    //做简单的初始化，载入kernel32并获取我们要用到的函数地址
    initKernel32Base(&gfuncs);
    initDynamicApis(&gfuncs);

    PENTRY_LoadLibraryW lpLoadLibraryW = (PENTRY_LoadLibraryW)gfuncs.lpGetProcAddress(gfuncs.hker32,"LoadLibraryW");
    PENTRY_IsDebuggerPresent lpIsDebuggerPresent = (PENTRY_IsDebuggerPresent)gfuncs.lpGetProcAddress(gfuncs.hker32,"IsDebuggerPresent");
    //这里是一段调试代码，在这里我们可以选择一次性载入所有函数的地址
    BOOL isDebug = lpIsDebuggerPresent();
    xapi_modinfo_ptr* modinfo = gmods.tofirst();
    while(modinfo)
    {
        xapi_modinfo* modone = modinfo->info;
        modone->hmod = isDebug?lpLoadLibraryW(modone->modName):0;
        modinfo = gmods.tonext();
    }
    if (isDebug)
    {

        xapi_apiinfo_ptr* apiptr = gapis.tofirst();
        while(apiptr)
        {
            xapi_apiInfo* api = apiptr->info;
            if (!api->dyEntry)
            {
                if (!api->module->hmod)
                {
                    api->module->hmod = lpLoadLibraryW(api->module->modName);
                }
                if (api->name[0] != 0)
                    api->dyEntry = gfuncs.lpGetProcAddress(api->module->hmod,api->name);
                else
                    api->dyEntry = gfuncs.lpGetProcAddress(api->module->hmod,(LPCSTR)api->base);
            }
            apiptr= gapis.tonext();
        }
    }
    //出口，跳转到转储时的代码地址，即crt初始化完毕
    snapshotOutProc(lpEsp);
    return 0;
}


int xapi_saveContext(void *entry, void *lpContext, int szContext, void *lpesp, DWORD stackDataSize)
{
    //因为此时我们的内存接管还没有关闭，因此实际上我们
    //只要分配内存就会被转存到快照PE当中
    xentry = (xapi_entrycontext*)malloc(sizeof(xapi_entrycontext));
    //保存入口
    xentry->orgEntry = entry;
    //保存栈空间上的重要数据
    xentry->stackDataSize = stackDataSize+szContext;
    xentry->stackData = (BYTE*)malloc(xentry->stackDataSize);
    xentry->stackMask = (BYTE*)malloc(xentry->stackDataSize);
    memset(xentry->stackMask,0,xentry->stackDataSize);
    memcpy(xentry->stackData,lpContext,szContext);
    memcpy((BYTE*)xentry->stackData+szContext,lpesp,stackDataSize);

    //注意对于EBP的保存，我们是根据其相对于ESP来计算的
    //如果超出范围我们才直接保存数值
    ULONG_PTR dwEBP = *(ULONG_PTR*)((BYTE*)lpContext + 8);
    ULONG_PTR dwESP = (ULONG_PTR)lpesp+stackDataSize;
    //这里我们遍历数据并将一些数据按照ESP修正
    ULONG_PTR* lpVal = (ULONG_PTR*)((BYTE*)xentry->stackData);
    ULONG_PTR* lpMask = (ULONG_PTR*)((BYTE*)xentry->stackMask);
    for (int i=0;i<xentry->stackDataSize/sizeof(ULONG_PTR);i++)
    {
        ULONG_PTR dwval = lpVal[i];
        if ((abs((int)(dwESP-dwval))<0x1000))
        {
            lpMask[i] = (int)(dwESP-dwval);
        }
    }

    xentry->ebpDelta = (int)(dwESP-dwEBP);
    xentry->restorEBP = (abs((int)(dwESP-dwEBP))<0x1000)?TRUE:FALSE;
    return 0;
}


//该函数初始化导出表HOOK库
int xapi_initApiHook(long flags, xApiCallbacks *xCallBacks)
{
    if (xCallBacks)
        gcbs = *xCallBacks;
    //如果初始化设置设定了接管LdrLoadDll那么我们可以在模块载入时HOOK其导出表
    if (flags & XAPI_INIT_FLAG_HOOKLOADDLL)
    {
        glpLdrLoadDll = (LPLdrLoadDll)HookOneApi(_T("NTDLL"),"LdrLoadDll",Hook_LPLdrLoadDll,HOOKTYPE_PUSH);
    }
    //如果需要我们可以在初始化导出表库时直接HOOK掉所有已经载入的模块的导出表
    if (flags & XAPI_INIT_FLAG_HOOKEXISTSMODULES)
    {
        HANDLE hsp = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,GetCurrentProcessId());
        MODULEENTRY32 me = {0};
        me.dwSize = sizeof(me);
        if (hsp != INVALID_HANDLE_VALUE && Module32First(hsp,&me))
        {
            do
            {
                Hook_ExportTable(me.hModule,0,0);
                break;
            }while(Module32Next(hsp,&me));
            CloseHandle(hsp);
        }
    }
    //在系统函数中试图定位导出表时，我们先检查我们的导出表HOOK状态
    if (flags & XAPI_INIT_FLAG_CHEATLDR)
    {
        glpRtlImageDirectoryEntryToData = (LPRtlImageDirectoryEntryToData)HookOneApi(_T("NTDLL"),"RtlImageDirectoryEntryToData",Hook_LPRtlImageDirectoryEntryToData,HOOKTYPE_PUSH);
    }

    return 0;
}


xapi_modinfo *xapi_lookupInModule(void *addr)
{

    xapi_modinfo_ptr* modptr = gmods.tofirst();
    while(modptr)
    {
        xapi_modinfo* mdinfo = modptr->info;
        if ((ULONG_PTR)addr >= (ULONG_PTR)mdinfo->hmod && (ULONG_PTR)addr < ((ULONG_PTR)mdinfo->hmod + mdinfo->szofImage))
        {
            return mdinfo;
        }
        modptr = gmods.tonext();
    }
    return 0;
}

