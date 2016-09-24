#ifndef XAPIHOOKRUNTIME_H
#define XAPIHOOKRUNTIME_H

#if __cplusplus
extern "C"{
#endif
void setProcessHeap(void* lpHeap);
HMODULE getMainModuleHandle();
void snapshotEntry();
void foreverLoop();
void  apiStubEnd(volatile void* lpesp,volatile void* retaddr);
void  apiStubEntry(void* lpEntry);
void apiOEPEntry();

void* getTIBSystemCall();

void Hooked_TIBSystemCall();
void Hooked_FastSystemCall();
void Hooked_IntSystemCall();
void*  getThreadTEB();
void nullTIBSyscallRet();
void nullTIBSyscallRetDelta4();
void nullSyscallRet();

#if __cplusplus
}
#endif

#pragma pack(push,1)

struct xapi_modinfo{
    HMODULE     hmod;
    DWORD       szofImage;
    char*       lpPatch;
    char*       lpVExport;
    DWORD       szVExport;
    SIZE_T      szPatch;
    SIZE_T      szPatchUsed;
    wchar_t     modName[1];
};

struct xapi_modinfo_ptr{
    xapi_modinfo*   info;
};
struct xapi_apiInfo
{
    BYTE    jmpCode[17];
    xapi_modinfo* module;
    void*   oriEntry;
    void*   hookedEntry;
    void*   dyEntry;
    long    flags;
    void*   userdata;
#ifdef _DEBUG
    void*   lpRVAPtr;
#endif
    DWORD   orgRVA;
    DWORD   AllCallCount;
    DWORD   base;
    char    name[1];
};

struct xapi_apiinfo_ptr{
    xapi_apiInfo*   info;
};


struct xapi_entrycontext{
    volatile void* orgEntry;
    volatile void* newstack;
    void*   stackData;
    int     stackDataSize;
    void*   stackMask;
    int     restorEBP;
    int     ebpDelta;

};
#pragma pack(pop)

//return >= 0 to indicate hook

typedef int (CALLBACK* LPModuleFilterCallBack)(HMODULE hmod,
                                               const wchar_t* moduleName,
                                               const wchar_t* modulePath);
typedef int (CALLBACK* LPModuleHookedCallBack)(HMODULE hmod,
                                               int state,
                                               xapi_modinfo* modinfo);

typedef int (CALLBACK* LApiHookFilterCallBack)(xapi_modinfo* modinfo,
                                          void* oriEntry,
                                          void* lpRVAPtr,
                                          DWORD orgRVA,
                                          const char* funcname,
                                          int base,
                                          bool bskiped,
                                               long*  apiFlags,
                                               void** userdata);

#define XAPI_FLAG_HOOKEXPORT    0x1
#define XAPI_FLAG_USEVEXPORT    0x2
#define XAPI_FLAG_NULLSTUB      0x4

typedef int (CALLBACK* LPTestFlagsCallBack)(long flags,void* arg1,void* arg2);

typedef void* (CALLBACK* LPApiExecuteCallBack)(xapi_apiInfo* apiinfo,void* retaddr,void* lpRet,void* lpesp,void** esptop);

typedef void* (CALLBACK* LPAllocMemoryWithin4GB)(SIZE_T szAlloc,int lProtect);
typedef struct _xApiCallbacks{
    LPTestFlagsCallBack     tfcb;
    LPModuleFilterCallBack  mfcb;
    LPModuleHookedCallBack  mhedcb;
    LApiHookFilterCallBack  sacb;
    LPApiExecuteCallBack    aecb;
    LPAllocMemoryWithin4GB  amw4;
}xApiCallbacks;


#define XAPI_INIT_FLAG_HOOKEXISTSMODULES    0x1
#define XAPI_INIT_FLAG_CHEATLDR             0x2
#define XAPI_INIT_FLAG_HOOKLOADDLL          0x4

#define XAPI_INIT_FLAG_DEFAULT  (XAPI_INIT_FLAG_HOOKEXISTSMODULES|XAPI_INIT_FLAG_CHEATLDR|XAPI_INIT_FLAG_HOOKLOADDLL)

int xapi_initApiHook(long flags = XAPI_INIT_FLAG_DEFAULT,xApiCallbacks* xCallBacks = 0);
xapi_modinfo* xapi_lookupInModule(void* addr);

int xapi_saveContext(void* entry,void* lpContext,int szContext,void* lpesp, DWORD stackDataSize);
#endif // XAPIHOOKRUNTIME_H
