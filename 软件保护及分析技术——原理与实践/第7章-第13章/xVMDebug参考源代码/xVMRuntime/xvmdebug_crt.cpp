#include <tchar.h>
#include "xvmdebug_crt.h"
#include "../../xHooklib/xhook_typedef.h"
#include "../../xHooklib/xhook_inline.h"
#include "../../xHooklib/xhook_vmexec.h"
#include "../../../nCom/nsafelocker.h"
#include "../../../nCom/naddrstate.h"

LPZwContinue glpZwContinue = NULL;
LPZwProtectVirtualMemory glpZwProtectVirtualMemory = 0;

typedef struct xvm_breakpoint
{
    long flags;
}xvm_breakpoint;

typedef struct xvm_membkstate
{
    memaddr addr;
    long size;
    DWORD oldProtect;
    DWORD newProtect;
}xvm_membkstate;

#ifdef _DEBUG
typedef std::map<memaddr,xvm_membkstate>       memstate;
#else
typedef index_map<memaddr,xvm_membkstate>       memstate;
#endif
typedef naddrstate<xvm_bp_type,xvm_breakpoint>  bpstate;
NSafeLocker gBPLock;
memstate  gmemst;
bpstate   gbpst;


void setProtectByRange(xvm_membkstate* mmst)
{
    DWORD assumePG = 0;
    bpstate::nstate* bprange = gbpst.hit(mmst->addr,mmst->size);
    while(bprange)
    {
        switch(bprange->state)
        {
        case xvm_bp_execute:
        {
            assumePG |= xvm_bp_execute;
        }break;
        case xvm_bp_write:
        {
            assumePG |= xvm_bp_write;
        }break;
        case xvm_bp_access:
        {
            assumePG |= xvm_bp_access;
        }break;
        }
        bprange = bprange->next;
    }
    if ((assumePG & PAGE_READWRITE) && (assumePG & PAGE_READONLY))
        assumePG &= ~PAGE_READONLY;

    if (assumePG & xvm_bp_access)
        mmst->newProtect = PAGE_NOACCESS;
    else if ((assumePG & xvm_bp_write) && (assumePG & xvm_bp_execute))
    {
        mmst->newProtect = PAGE_READONLY;
    }else if (assumePG & xvm_bp_write)
    {
        if (mmst->oldProtect & (PAGE_EXECUTE|PAGE_EXECUTE_READ|PAGE_EXECUTE_READWRITE|PAGE_EXECUTE_WRITECOPY))
            mmst->newProtect = PAGE_EXECUTE_READ;
        else
            mmst->newProtect = PAGE_READONLY;
    }else
    {
        if (mmst->oldProtect & (PAGE_READWRITE|PAGE_WRITECOPY|PAGE_EXECUTE_READWRITE|PAGE_EXECUTE_WRITECOPY|PAGE_WRITECOMBINE))
            mmst->newProtect = PAGE_READWRITE;
        else
            mmst->newProtect = PAGE_READONLY;
    }

    PVOID bbaddr = (PVOID)mmst->addr;
    ULONG rgsz = mmst->size;
    glpZwProtectVirtualMemory(GetCurrentProcess(),&bbaddr,&rgsz,mmst->newProtect,&mmst->oldProtect);
}



NTSTATUS NTAPI Hook_ZwProtectVirtualMemory(
        IN HANDLE hProcess,
        IN OUT PVOID *BaseAddress,
        IN OUT PULONG RegionSize,
        IN ULONG Protect,
        OUT PULONG OldProtect
        )
{
    NTSTATUS state = glpZwProtectVirtualMemory(hProcess,BaseAddress,RegionSize,Protect,OldProtect);
    if (state != 0)
        return state;
    memaddr memstart = (memaddr)(*BaseAddress) & ~0xFFF;
    memaddr memend = memstart + *RegionSize;
    if (memend % 0x1000)
        memend = (memend & ~0xFFF) + 0x1000;
    gBPLock.Lock();
    for (memaddr addr =memstart;addr < memend;addr += 0x1000)
    {
        memstate::iterator itr;
        if ((itr = gmemst.find(addr)) != gmemst.end())
        {
            xvm_membkstate* mmst = &itr->second;
            mmst->oldProtect = Protect;
            setProtectByRange(mmst);
        }
    }
    gBPLock.Unlock();
    return state;
}

int EnableXVMDebug()
{


    HMODULE hNtdll = GetModuleHandle(_T("NTDLL"));

    if (!glpZwContinue)
        glpZwContinue = (LPZwContinue)GetProcAddress(hNtdll,"ZwContinue");
    //Hook ZwProtectVirtualMemory Would be careful
    if (!glpZwProtectVirtualMemory)
    {
        void* lpPVM = GetProcAddress(hNtdll,"ZwProtectVirtualMemory");
        if (lpPVM)
        {
            DWORD oldpg = 0;
            VirtualProtect(lpPVM,16,PAGE_EXECUTE_READWRITE,&oldpg);
            glpZwProtectVirtualMemory = (LPZwProtectVirtualMemory)HookCodeDirect(hNtdll,lpPVM,Hook_ZwProtectVirtualMemory,HOOKTYPE_PUSH,0);
            VirtualProtect(lpPVM,16,oldpg,&oldpg);
        }
    }
    return 0;
}


int xvm_setbreakpoint(ULONG_PTR addr, int size, xvm_bp_type type)
{
    NStackAutoLocker sl(&gBPLock);
    sl.Lock();

    gbpst.set(addr,size,type);
    bpstate::nstate* bpst = gbpst.hit(addr,size);
    memaddr memstart = bpst->addr & ~0xFFF;
    memaddr memend = bpst->addr + bpst->size;
    if (memend % 0x1000)
        memend = (memend & ~0xFFF) + 0x1000;

    //ensure we had the memory state in manager
    MEMORY_BASIC_INFORMATION mbi = {0};
    for (memaddr addr =memstart;addr < memend;addr += 0x1000)
    {
        memstate::iterator itr;
        if ((itr = gmemst.find(addr)) == gmemst.end())
        {
            SIZE_T szt = VirtualQuery((BYTE*)addr,&mbi,sizeof(mbi));
            if (szt && mbi.State != MEM_RESERVE && mbi.State != MEM_FREE)
            {
                xvm_membkstate mms = {0};
                mms.oldProtect = mbi.Protect;
                mms.addr = addr;
                mms.size = 0x1000;
                gmemst.insert(std::make_pair(addr,mms));
                itr = gmemst.find(addr);
            }
        }
        xvm_membkstate* mmst = &itr->second;
        setProtectByRange(mmst);
    }

    return 0;
}
