#ifndef XVMIATCALLFRAME_H
#define XVMIATCALLFRAME_H

#include <map>
#include "xdynamic_ollydbg.h"
#include "xvmdebugcpu.h"
#include "../xVMRuntime/xvmiatcall.h"
#include "../../../nCom/nstring.h"

enum fixed_state{
    fixed_none,
    fixed_testmode,
    fixed_realmode,
    fixed_faild
};

std::wstring SingleStringToWide(const std::string &singleString);

typedef struct iatcall_info : public t_sorthdr,public iatcall_data{
    //追踪结果描述
    ULONG_PTR       result_addr;//追踪结果地址
    //GUI用
    BOOL            selected;   //GUI用，是否已选
    int             fixedState;
}iatcall_info;

typedef NAvlList<ULONG_PTR,iatcall_info> iatcall_map;
//typedef std::map<ULONG_PTR,iatcall_info> iatcall_map;

struct iatcall_mod_cache;
struct iatcall_api_cache{
    ULONG_PTR   addr;
    ULONG_PTR   modBase;
    long    base;
    BOOL    hasName;
    std::wstring name;
};

typedef index_map<ULONG_PTR,iatcall_api_cache>   iatcall_apicache;
struct iatcall_mod_cache{
    ULONG_PTR           addrbase;
    ULONG_PTR           addrend;
    iatcall_apicache    apis;
    std::wstring        name;
};


typedef index_map<ULONG_PTR,iatcall_mod_cache>   iatcall_cache;

#define IATCF_FLAG_SHOWKNOWN    0x1
#define IATCF_FLAG_SHOWCALL     0x2
#define IATCF_FLAG_SHOWJMP      0x4
#define IATCF_FLAG_SHOWCALLMEM  0x8
#define IATCF_FLAG_SHOWJMPMEM   0x10
#define IATCF_FLAG_SHOWPUSHRET  0x20
#define IATCF_FLAG_SHOWMEMADDR  0x40
#define IATCF_FLAG_SHOWUNKNOWN  0x80
#define IATCF_FLAG_SHOWFAILD    0x100
#define IATCF_FLAG_SHOWRTLIMIT  0x200
#define IATCF_FLAG_SHOWSUSPECT  0x400

#define IATCF_FLAG_SHOWALL   (IATCF_FLAG_SHOWKNOWN | IATCF_FLAG_SHOWCALL \
    | IATCF_FLAG_SHOWJMP | IATCF_FLAG_SHOWCALLMEM \
    | IATCF_FLAG_SHOWJMPMEM | IATCF_FLAG_SHOWPUSHRET \
    | IATCF_FLAG_SHOWMEMADDR | IATCF_FLAG_SHOWUNKNOWN \
    | IATCF_FLAG_SHOWFAILD | IATCF_FLAG_SHOWRTLIMIT | IATCF_FLAG_SHOWSUSPECT)
class xVMIATCallFrame
{
public:
    xVMIATCallFrame();
    void initFrame();
    void findCalls(bool beMerge);
    void setEndOfImageForMemaddr(BOOL bReset);
    void flushCalls();
    void traceSelected();
    void reset();

    iatcall_info* findIATCall(ULONG_PTR addr);
    BOOL traceIAT(iatcall_info* info);
    void tempFixupSelected();
    void fixupSelectedCall();
    BOOL fixIATCall(iatcall_info* info,BOOL direct);
    void addArrayToIATCall(ULONG_PTR addr,int size);
    void setIATArea(ULONG_PTR addr,int size,BOOL onlyFix);
    void traceAll();
    void openCPU();
    void closeCPU();

    void    refineResults();
    iatcall_mod_cache*  cacheModuleExport(ULONG_PTR addr);
    iatcall_mod_cache*  cacheModuleExportFromFile(ULONG_PTR addr);
    iatcall_mod_cache*  cacheModuleExportFromMemory(ULONG_PTR addr);
    iatcall_api_cache*  getExport(ULONG_PTR addr);



    iat_func *hitIATCallModule(iatcall_info* info);
    void saveRecords(TCHAR* filename);
    void loadRecords(TCHAR* filename);
    void searchCallsInMemory(ULONG_PTR base,BYTE* lpMem,int szmem,ULONG_PTR imgbase,DWORD imgend);
    void frameDestory();
    int lastsel() const;
    void setLastsel(int lastsel);
    iat_func* getFuncInfo(long modhash,long funchash);
    void setFlushFlags(long flags, bool on = true, bool replace = false);
    bool testFlushFlags(long flags);
    long    flushFlags() const ;
private:
    t_frame m_frame;
    t_block m_block_main;

    t_table m_table_iat;
    HWND    m_hFrame;

    iatcall_map     m_calls;
    iat_modules     m_mods;
    iatcall_cache   m_cache;

    xVMDebugCPU*    m_vcpu;

    long    m_flushFlags;

    ULONG_PTR   m_mmaddrEnd;

    int m_lastsel;
};


extern void VMDebug_FindIATCalls(BOOL beMerge);
extern void VMDebug_AddtoIATCalls(t_table *pt);
extern void VMDebug_SetIAT(t_table *pt,BOOL withFix);
extern void VMDebug_IATReset();
extern void VMDebug_SetEndOfImage(BOOL bReset);
#endif // XVMIATCALLFRAME_H
