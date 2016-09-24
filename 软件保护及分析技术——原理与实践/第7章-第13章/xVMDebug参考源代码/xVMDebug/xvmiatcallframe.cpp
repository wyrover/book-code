#include <windows.h>
#include "../../../nCom/npacketbase.h"
#include "../../../nCom/nautolargebuffer.h"
#include "../../xHooklib/xhook_pefuncs.h"
#include "xvmdebug.h"
#include "xvmdebugframe.h"
#include "xvmiatcallframe.h"
#include "ollydbg2_plugin.h"

BOOL EnableWow64FsRedirection (BOOL enable)
{
    typedef BOOLEAN (__stdcall *Wow64EnableWow64FsRedirection_t) (BOOL enable);
    Wow64EnableWow64FsRedirection_t wow64EnableWow64FsRedirection = (Wow64EnableWow64FsRedirection_t) GetProcAddress (GetModuleHandle (_T("kernel32")),"Wow64EnableWow64FsRedirection");

    if (!wow64EnableWow64FsRedirection)
        return FALSE;

    return wow64EnableWow64FsRedirection (enable);
}

xVMIATCallFrame* gIATCallsFrame = 0;

const wchar_t* findModName(const wchar_t* mpath)
{
    int nlen = wcslen(mpath);
    if (nlen > 0)
    {
        int pos=0;
        for (pos=nlen-1;pos>0;pos--)
        {
            if (mpath[pos] == '\\' || mpath[pos] == '/')
            {
                pos++;
                break;
            }
        }
        return mpath+pos;
    }
    return 0;
}

std::wstring SingleStringToWide (const std::string &singleString)
{
    if (singleString.empty())
        return std::wstring();

    WCHAR wbuf[65536];
    int wideLen = MultiByteToWideChar (CP_ACP, 0, singleString.c_str(), -1, wbuf, sizeof(wbuf)/sizeof(WCHAR) - 1);
    wbuf[wideLen] = 0;
    return wbuf;
}



long xvm_iat_calls_func(struct t_table *table,HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
    switch(msg)
    {
    case WM_USER_DBLCLK:
    {
        iatcall_info* xr = (iatcall_info*)gLPGetsortedbyselection(&table->sorted,lparam);
        if (xr)
        {
            xr->selected = !xr->selected;
            gLPUpdatetable(table,0);
        }
    }break;
    case WM_USER_KEY:
    {
        switch(LOBYTE(lparam))
        {
        case VK_DELETE:
        {

        }break;
        case 'C':
        case 'c':
        {
            for (int i=0;i<table->sorted.n;i++)
            {
                iatcall_info* xrt = (iatcall_info*)gLPGetsortedbyindex(&table->sorted,i);
                if (xrt)
                    xrt->selected = FALSE;
            }
            gLPUpdatetable(table,1);
        }break;
        }
    }break;
    case WM_DESTROY:
    {
        xVMIATCallFrame* iatframe = static_cast<xVMIATCallFrame*>(table->customdata);
        if (iatframe)
        {
            iatframe->frameDestory();
        }
    }break;

    }
    return 0;
}

int  xvm_iat_calls_draw_func(wchar_t *result,uchar *mask,int *drawFlags,
                             struct t_table *table,t_sorthdr *sorthdr,int reason,void *item)
{
    if (result) result[0] = 0;
    iatcall_info* seq = reinterpret_cast<iatcall_info*>(sorthdr);
    switch(reason)
    {
    case DF_CACHESIZE:
        return 0;
    case DF_FILLCACHE:
    case DF_FREECACHE:
        return 0;
    case DF_NEWROW:
    {
        return 0;
    }break;
    case 0:
    {
        int flags = 0;
        if (seq->selected)
            flags |= DRAW_COND | DRAW_EXTSEL;
        *drawFlags = flags;
        return wsprintfW(result,L"%d",sorthdr->addr);
    }break;
    case 1:
    {
        int flags = DRAW_GRAY;
        *drawFlags = flags;
        return wsprintfW(result,L"%08X",seq->cmdaddr);
    }break;
    case 2:
    {
        int flags = DRAW_GRAY;
        *drawFlags = flags;
        return wsprintfW(result,L"%08X",seq->callto);
    }break;
    case 3:
    {
        if (seq->calltype == iatcall_memaddr && seq->memoft == 0)
        {
            ULONG_PTR tmpaddr;
			int nlen = gLPReadmemory(&tmpaddr,seq->callto,sizeof(tmpaddr),MM_REMOVEINT3 | MM_SILENT);
            if (nlen != sizeof(tmpaddr))
            {
                *drawFlags = DRAW_GRAY;
                wcscpy(result,L"Access memory faild!");
            }else
            {
                int flags = DRAW_MASK;
                *drawFlags = flags;
                int selected = 0;
                gLPLabeladdress(result,tmpaddr,0,-1,0,mask,&selected,ADDR_GRAYHEX|ADDR_HILSYM);
            }
        }else
        {
            t_disasm td;
            td.hiliteindex = 1;
            td.hilitereg = 0;
            td.hiregindex = 0;
            gLPDisasm(seq->opcode,seq->szop,seq->cmdaddr,0,&td,DA_TEXT|DA_HILITE|DA_MEMORY,0,0);
            wcscpy(result,td.result);
            memcpy(mask,td.mask,sizeof(td.mask));
            int flags = DRAW_MASK | DRAW_HILITE;
            *drawFlags = flags;

        }
        return wcslen(result);
    }break;
    case 4:
    {
        int flags = 0;
        *drawFlags = flags;
        if (seq->rtflags & IATCALL_FLAG_TRACEFAILD)
        {
            *drawFlags |= DRAW_SUSPECT;
            wcscpy(result,L"trace Faild!");
        }else if (seq->rtflags & IATCALL_FLAG_REACHLIMIT)
        {
            *drawFlags |= DRAW_HILITE;
            wcscpy(result,L"Reach Limit!");
        }else if (seq->rtflags & IATCALL_FLAG_MARKINVAILD)
        {
            *drawFlags |= DRAW_SUSPECT;
            wcscpy(result,L"marked as invaild!");
        }
        else
        {
            if (seq->result_addr != 0)
            {
                *drawFlags |= DRAW_MASK;
                int selected = 0;
                int oft = 0;
                if (seq->hasHead)
                {
                    oft = 3;
                    wcscpy(result,L"(H)");
                    strcpy((char*)mask,"   ");
                }
                xVMIATCallFrame* fm = (xVMIATCallFrame*)table->customdata;
                if (fm)
                {
                    iatcall_api_cache* api = fm->getExport(seq->result_addr);
                    if (api)
                    {
                        *drawFlags = DRAW_GRAY;
                        iatcall_mod_cache* imod = fm->cacheModuleExport(api->modBase);
                        if (imod)
                        {
                            return oft+wsprintfW(result+oft,L"%s->%s",imod->name.c_str(),api->name.c_str());
                        }
                    }
                }
                gLPLabeladdress(result+oft,seq->result_addr,0,-1,0,mask+oft,&selected,ADDR_GRAYHEX|ADDR_HILSYM);
            }else
            {
                if (seq->rtflags & IATCALL_FLAG_MARKINVAILD)
                    *drawFlags |= DRAW_HILITE;
                xVMIATCallFrame* fm = (xVMIATCallFrame*)table->customdata;
                if (fm && (seq->rtflags & IATCALL_FLAG_TRACEDOUT))
                {
                    iat_func* fc = fm->getFuncInfo(seq->modhash,seq->funchash);
                    if (fc)
                    {
                        if (fc->module)
                            return wsprintfW(result,L"%s->%s",fc->module->name.c_str(),fc->name.c_str());
                        else
                            return wsprintfW(result,L"%s",fc->name.c_str());
                    }
                }
                return 0;
            }
        }

        return wcslen(result);
    }break;
    case 5:
    {
        if (seq->retaddr != 0)
        {
            int flags = DRAW_GRAY;
            *drawFlags = flags;
            return wsprintfW(result,L"%08X",seq->retaddr);
        }
    }break;
    case 6:
    {
        xVMIATCallFrame* fm = (xVMIATCallFrame*)table->customdata;
        if (fm && seq->modhash && seq->funchash)
        {
            iat_func* fc = fm->getFuncInfo(seq->modhash,seq->funchash);
            if (fc && fc->IATRVA != 0)
            {
                int flags = DRAW_GRAY;
                *drawFlags = flags;
                return wsprintfW(result,L"%08X",fc->IATRVA);
            }
        }
    }break;
    case 7:
    {
        switch(seq->fixedState)
        {
        case fixed_faild:wcscpy(result,L"FAILD!");break;
        case fixed_realmode:wcscpy(result,L"REALMODE");break;
        case fixed_testmode:wcscpy(result,L"TESTMODE");break;
        default:return 0;
        }
        return wcslen(result);
    }break;
    default:return 0;
    }


    return 0;
}

void xvm_iat_calls_sel_func(struct t_table *pt,int selected,int reason)
{
    iatcall_info* xr = (iatcall_info*)gLPGetsortedbyselection(&pt->sorted,selected);
    if (!xr)
        return;
    xVMIATCallFrame* fm = (xVMIATCallFrame*)pt->customdata;
    if (HIBYTE(GetKeyState(VK_SHIFT)))
    {
        if (fm)
        {
            for (int i=0;i<pt->sorted.n;i++)
            {
                iatcall_info* xrt = (iatcall_info*)gLPGetsortedbyindex(&pt->sorted,i);
                if (xrt)
                    xrt->selected = FALSE;
            }
            int lastsel = fm->lastsel();
            if (lastsel != -1)
            {
                int minl = min(lastsel,selected);
                int maxl = max(lastsel,selected);

                for (int i=minl;i<maxl+1;i++)
                {
                    iatcall_info* xrt = (iatcall_info*)gLPGetsortedbyindex(&pt->sorted,i);
                    if (xrt)
                        xrt->selected = TRUE;
                }
            }
        }
    }else if (HIBYTE(GetKeyState(VK_CONTROL)))
    {
        xr->selected = !xr->selected;
    }
    if (fm) fm->setLastsel(selected);
    if (HIBYTE(GetKeyState(VK_MENU)))
        gLPSetcpu(gLPGetcputhreadid(),xr->cmdaddr,0,1,0,CPU_NOCREATE);

}


enum iatcall_menus{
    iatcall_menu_traceseleted = 1,
    iatcall_menu_traceall,
    iatcall_menu_filters,
    iatcall_menu_save,
    iatcall_menu_load,
    iatcall_menu_showall,
    iatcall_menu_showknown,
    iatcall_menu_showcall,
    iatcall_menu_showjmp,
    iatcall_menu_showcallmem,
    iatcall_menu_showjmpmem,
    iatcall_menu_showpushret,
    iatcall_menu_showmemaddr,
    iatcall_menu_showunknown,
    iatcall_menu_showsuspect,
    iatcall_menu_showfaild,
    iatcall_menu_showrtlimit,
    iatcall_menu_clearall,
    iatcall_menu_tempfix,
    iatcall_menu_fixiatcall,
    iatcall_menu_refine,
	iatcall_menu_app

};

static int IATCalls_Table_MenuProc(t_table *pt,wchar_t *name,ulong index,int mode)
{
    xVMIATCallFrame* fm = (xVMIATCallFrame*)pt->customdata;
    if (mode==MENU_VERIFY)
    {
        switch(index)
        {

        case iatcall_menu_showknown:return (fm && (fm->flushFlags() & IATCF_FLAG_SHOWKNOWN))?MENU_CHECKED:MENU_NORMAL;
        case iatcall_menu_showcall:return (fm && (fm->flushFlags() & IATCF_FLAG_SHOWCALL))?MENU_CHECKED:MENU_NORMAL;
        case iatcall_menu_showjmp:return (fm && (fm->flushFlags() & IATCF_FLAG_SHOWJMP))?MENU_CHECKED:MENU_NORMAL;
        case iatcall_menu_showcallmem:return (fm && (fm->flushFlags() & IATCF_FLAG_SHOWCALLMEM))?MENU_CHECKED:MENU_NORMAL;
        case iatcall_menu_showjmpmem:return (fm && (fm->flushFlags() & IATCF_FLAG_SHOWJMPMEM))?MENU_CHECKED:MENU_NORMAL;
        case iatcall_menu_showpushret:return (fm && (fm->flushFlags() & IATCF_FLAG_SHOWPUSHRET))?MENU_CHECKED:MENU_NORMAL;
        case iatcall_menu_showmemaddr:return (fm && (fm->flushFlags() & IATCF_FLAG_SHOWMEMADDR))?MENU_CHECKED:MENU_NORMAL;
        case iatcall_menu_showunknown:return (fm && (fm->flushFlags() & IATCF_FLAG_SHOWUNKNOWN))?MENU_CHECKED:MENU_NORMAL;
        case iatcall_menu_showfaild:return (fm && (fm->flushFlags() & IATCF_FLAG_SHOWFAILD))?MENU_CHECKED:MENU_NORMAL;
        case iatcall_menu_showrtlimit:return (fm && (fm->flushFlags() & IATCF_FLAG_SHOWRTLIMIT))?MENU_CHECKED:MENU_NORMAL;
        case iatcall_menu_showsuspect:return (fm && (fm->flushFlags() & IATCF_FLAG_SHOWSUSPECT))?MENU_CHECKED:MENU_NORMAL;
        }

        return MENU_NORMAL;                // Always available
    }
    if (mode==MENU_EXECUTE)
    {
        switch(index)
        {
        case iatcall_menu_traceseleted:
        {
            if (fm)
                fm->traceSelected();
        }break;
        case iatcall_menu_traceall:
        {
            if (fm)
                fm->traceAll();
        }break;
        case iatcall_menu_filters:
        {

        }break;
        case iatcall_menu_showall:
        {
            if (fm)
                fm->setFlushFlags(IATCF_FLAG_SHOWALL,true);
        }break;
        case iatcall_menu_clearall:
        {
            if (fm)
                fm->setFlushFlags(IATCF_FLAG_SHOWALL,false);
        }break;
        case iatcall_menu_showknown:{
            if (fm)
                fm->setFlushFlags(IATCF_FLAG_SHOWKNOWN,!(fm->testFlushFlags(IATCF_FLAG_SHOWKNOWN)));
        }break;
        case iatcall_menu_showcall:{
            if (fm)
                fm->setFlushFlags(IATCF_FLAG_SHOWCALL,!(fm->testFlushFlags(IATCF_FLAG_SHOWCALL)));
        }break;
        case iatcall_menu_showjmp:{
            if (fm)
                fm->setFlushFlags(IATCF_FLAG_SHOWJMP,!(fm->testFlushFlags(IATCF_FLAG_SHOWJMP)));
        }break;
        case iatcall_menu_showcallmem:{
            if (fm)
                fm->setFlushFlags(IATCF_FLAG_SHOWCALLMEM,!(fm->testFlushFlags(IATCF_FLAG_SHOWCALLMEM)));
        }break;
        case iatcall_menu_showjmpmem:{
            if (fm)
                fm->setFlushFlags(IATCF_FLAG_SHOWJMPMEM,!(fm->testFlushFlags(IATCF_FLAG_SHOWJMPMEM)));
        }break;
        case iatcall_menu_showpushret:{
            if (fm)
                fm->setFlushFlags(IATCF_FLAG_SHOWPUSHRET,!(fm->testFlushFlags(IATCF_FLAG_SHOWPUSHRET)));
        }break;
        case iatcall_menu_showmemaddr:{
            if (fm)
                fm->setFlushFlags(IATCF_FLAG_SHOWMEMADDR,!(fm->testFlushFlags(IATCF_FLAG_SHOWMEMADDR)));
        }break;
        case iatcall_menu_showsuspect:
        {
            if (fm)
                fm->setFlushFlags(IATCF_FLAG_SHOWSUSPECT,!(fm->testFlushFlags(IATCF_FLAG_SHOWSUSPECT)));
        }break;
        case iatcall_menu_showunknown:{
            if (fm)
                fm->setFlushFlags(IATCF_FLAG_SHOWUNKNOWN,!(fm->testFlushFlags(IATCF_FLAG_SHOWUNKNOWN)));
        }break;
        case iatcall_menu_showfaild:{
            if (fm)
                fm->setFlushFlags(IATCF_FLAG_SHOWFAILD,!(fm->testFlushFlags(IATCF_FLAG_SHOWFAILD)));
        }break;
        case iatcall_menu_showrtlimit:{
            if (fm)
                fm->setFlushFlags(IATCF_FLAG_SHOWRTLIMIT,!(fm->testFlushFlags(IATCF_FLAG_SHOWRTLIMIT)));
        }break;
        case iatcall_menu_save: //save Records
        {
            if (fm)
            {
                TCHAR szFilter[] =_T("*.ximport\0*.ximport\0*.*\0*.*\0");
                TCHAR lpFilename[256] ={0};
                OPENFILENAME  lpofn={0};
                DWORD len=0;
                lpofn.lStructSize = sizeof (lpofn);
                lpofn.lpstrFilter = szFilter;
                lpofn.hwndOwner = pt->hparent;
                lpofn.hInstance = GetModuleHandleA(NULL);
                lpofn.lpstrFile = lpFilename;
                lpofn.Flags = OFN_HIDEREADONLY;
                lpofn.lpstrTitle = _T("Select Records File to Save");
                lpofn.nMaxFile = 0x256;
                lpofn.lpstrDefExt = _T(".ximport");

                if(!GetSaveFileName(&lpofn) || (len=_tcslen(lpFilename))==0)
                    return MENU_NOREDRAW;
                fm->saveRecords(lpFilename);
            }
        }break;
        case iatcall_menu_load: //load Records
        {
            if (fm)
            {
                TCHAR szFilter[] =_T("*.ximport\0*.ximport\0*.*\0*.*\0");
                TCHAR lpFilename[256] ={0};
                OPENFILENAME  lpofn={0};
                DWORD len=0;
                lpofn.lStructSize = sizeof (lpofn);
                lpofn.lpstrFilter = szFilter;
                lpofn.hwndOwner = pt->hparent;
                lpofn.hInstance = GetModuleHandleA(NULL);
                lpofn.lpstrFile = lpFilename;
                lpofn.Flags = OFN_FILEMUSTEXIST;
                lpofn.lpstrTitle = _T("Select Records File to Load");
                lpofn.nMaxFile = 0x256;

                if(!GetOpenFileName(&lpofn) || (len=_tcslen(lpFilename))==0)
                    return MENU_NOREDRAW;
                fm->loadRecords(lpFilename);
            }
        }break;
        case iatcall_menu_tempfix:
        {
            if (fm)
                fm->tempFixupSelected();
        }break;
        case iatcall_menu_fixiatcall:
        {
            if (fm)
                fm->fixupSelectedCall();
        }break;
        case iatcall_menu_refine:
        {
            if (fm)
                fm->refineResults();
        }break;
        }
        return MENU_NOREDRAW;
    };
    return MENU_ABSENT;
}

xVMIATCallFrame::xVMIATCallFrame()
    :m_vcpu(0)
    ,m_hFrame(0)
    ,m_lastsel(-1)
    ,m_flushFlags(IATCF_FLAG_SHOWALL)
    ,m_mmaddrEnd(0)
{
}

void xVMIATCallFrame::initFrame()
{
    if (m_hFrame != 0)
        return;
    wcscpy(m_frame.name,L"xVM_IATCalls");//名称
    memset(&m_table_iat,0,sizeof(m_table_iat));
    wcscpy(m_table_iat.name,L"xvm_IAT_calls");//初始化并设定表格
    m_table_iat.mode = TABLE_SYNTAX  | TABLE_SAVEALL;
    m_table_iat.drawfunc = xvm_iat_calls_draw_func;//指定表格绘制回调函数
    m_table_iat.tableselfunc = xvm_iat_calls_sel_func;//指定选择回调函数
    m_table_iat.tabfunc = xvm_iat_calls_func;
    m_table_iat.customdata = this;//初始化各列名称
    m_table_iat.bar.nbar = 8;
    m_table_iat.bar.visible = 1;
    m_table_iat.bar.name[0] = L"NO.";
    m_table_iat.bar.expl[0] = L"No.";
    m_table_iat.bar.defdx[0] = 5;
    m_table_iat.bar.name[1] = L"Address";
    m_table_iat.bar.expl[1] = L"Address";
    m_table_iat.bar.defdx[1] = 10;
    m_table_iat.bar.name[2] = L"CallTo";
    m_table_iat.bar.expl[2] = L"CallTo";
    m_table_iat.bar.defdx[2] = 10;
    m_table_iat.bar.name[3] = L"Command";
    m_table_iat.bar.expl[3] = L"Command";
    m_table_iat.bar.defdx[3] = 5;
    m_table_iat.bar.name[4] = L"Result";
    m_table_iat.bar.expl[4] = L"Result";
    m_table_iat.bar.defdx[4] = 50;
    m_table_iat.bar.name[5] = L"RetAddr.";
    m_table_iat.bar.expl[5] = L"RetAddr.";
    m_table_iat.bar.defdx[5] = 10;
    m_table_iat.bar.name[6] = L"ThunkRVA";
    m_table_iat.bar.expl[6] = L"ThunkRVA";
    m_table_iat.bar.defdx[6] = 10;
    m_table_iat.bar.name[7] = L"FixState";
    m_table_iat.bar.expl[7] = L"FixState";
    m_table_iat.bar.defdx[7] = 10;
    //这里我们添加一个右键菜单来触发追踪操作

    static t_menu IATCall_Filter_Menu[] = {
        { L"Show Calls",L"Show Calls", K_NONE, IATCalls_Table_MenuProc, NULL, iatcall_menu_showcall },
        { L"Show Jmps",L"Show Jmps", K_NONE, IATCalls_Table_MenuProc, NULL, iatcall_menu_showjmp },
        { L"Show Call Mem",L"Show Call Mem", K_NONE, IATCalls_Table_MenuProc, NULL, iatcall_menu_showcallmem },
        { L"Show Jmp Mem",L"Show Jmp Mem", K_NONE, IATCalls_Table_MenuProc, NULL, iatcall_menu_showjmpmem },
        { L"Show Push xxx,Ret",L"Show Push xxx,Ret", K_NONE, IATCalls_Table_MenuProc, NULL, iatcall_menu_showpushret },
        { L"Show Mem Reference",L"Show Memory Reference", K_NONE, IATCalls_Table_MenuProc, NULL, iatcall_menu_showmemaddr },
        { L"|Show Known",L"Show Known", K_NONE, IATCalls_Table_MenuProc, NULL, iatcall_menu_showknown },
        { L"Show Suspect",L"Show Suspect", K_NONE, IATCalls_Table_MenuProc, NULL, iatcall_menu_showsuspect},

        { L"Show Unknown",L"Show Unknown", K_NONE, IATCalls_Table_MenuProc, NULL, iatcall_menu_showunknown},
        { L"|Show Trace Faild",L"Show Trace Faild Item", K_NONE, IATCalls_Table_MenuProc, NULL, iatcall_menu_showfaild },
        { L"Show Reach limited",L"Show Reach limited", K_NONE, IATCalls_Table_MenuProc, NULL, iatcall_menu_showrtlimit },
        { L"|Show ALL",L"Show ALL", K_NONE, IATCalls_Table_MenuProc, NULL, iatcall_menu_showall },
        { L"Clear ALL",L"Clear ALL", K_NONE, IATCalls_Table_MenuProc, NULL, iatcall_menu_clearall },
        { NULL, NULL, K_NONE, NULL, NULL, 0 }
    };

    static t_menu IATCall_menu[] = {
        { L"Trace Selected",L"Trace Selected", K_NONE, IATCalls_Table_MenuProc, NULL, iatcall_menu_traceseleted },
        { L"Trace All",L"Trace All", K_NONE, IATCalls_Table_MenuProc, NULL, iatcall_menu_traceall },
        { L"Refine Results",L"Refine Results", K_NONE, IATCalls_Table_MenuProc, NULL, iatcall_menu_refine },
        { L"|Made Immediate Call",L"Made Immediate Call", K_NONE, IATCalls_Table_MenuProc, NULL, iatcall_menu_tempfix },
        { L"Fix IAT Call",L"Fix IAT Call", K_NONE, IATCalls_Table_MenuProc, NULL, iatcall_menu_fixiatcall },
        { L"|Filter",L"Filter Show By Type", K_NONE, IATCalls_Table_MenuProc, IATCall_Filter_Menu, iatcall_menu_filters },
        { L"|Save Records...",L"Save Records", K_NONE, IATCalls_Table_MenuProc, 0, iatcall_menu_save },
        { L"Load Records...",L"Load Records", K_NONE, IATCalls_Table_MenuProc, 0, iatcall_menu_load },
		{ L"|Appearance",L"Appearance",K_NONE,IATCalls_Table_MenuProc,appearanceMenu,iatcall_menu_app },
        { NULL, NULL, K_NONE, NULL, NULL, 0 }
    };
    m_table_iat.menu = IATCall_menu;
    m_table_iat.customdata = this;  //设定具体的窗体样式
    memset(&m_block_main,0,sizeof(m_block_main));
    m_block_main.type = BLK_TABLE;
    m_block_main.table = &m_table_iat;
    memset(&m_frame,0,sizeof(m_frame));
    m_frame.block = &m_block_main;
    m_frame.mode = TABLE_SYNTAX  | TABLE_SAVEALL;
    m_hFrame = glpCreateframewindow(&m_frame,0,L"XVM_IAT_Calls");//执行创建操作
    m_table_iat.hilite = 1; //高亮配置

}

void xVMIATCallFrame::findCalls(bool beMerge)
{
    //取得当前选取的内存块地址
    ULONG_PTR addr = gMemoryTable->sorted.seladdr;
    if (addr == 0)  return;
    //取得内存块信息
    t_memory* mem = gLPFindmemory(addr);
    if (!mem) return;
    if (mem->size < 1)   return;
    BYTE* buf = (BYTE*)malloc(mem->size);
    if (!buf) return;
    //我们先试图确定映像大小
    ulong imgend = 0;
    ulong imgbase = 0;
    if (mem->owner != 0)
    {
        ulong szImage = 0;
        IMAGE_DOS_HEADER imdh = {0};
        IMAGE_NT_HEADERS imnh = {0};
        //先通过PE头获取映像大小，但是后面我们需要效验
		gLPReadmemory(&imdh,mem->owner,sizeof(imdh),MM_REMOVEINT3 | MM_SILENT | MM_PARTIAL);
        if (imdh.e_magic == IMAGE_DOS_SIGNATURE)
        {
			gLPReadmemory(&imnh,mem->owner + imdh.e_lfanew,sizeof(imnh),MM_REMOVEINT3 | MM_SILENT | MM_PARTIAL);
            if (imnh.Signature == IMAGE_NT_SIGNATURE)
                szImage = imnh.OptionalHeader.SizeOfImage;
        }
        ulong appdr = mem->owner;
        ulong append = 0;
        t_memory* mm = gLPFindmemory(appdr);
        while(mm)
        {
            if (mm->owner != mem->owner)
                break;
            append = mm->base + mm->size;
            mm = gLPFindmemory(mm->base+mm->size);
        }
        //我们容忍一定范围内的差异
        if (abs((int)(append - (mem->owner + szImage))) < max(imnh.OptionalHeader.SectionAlignment,0x1000))
        {
            imgend = append;
            imgbase = mem->owner;
        }
    }
    //清除之前记录
    if (!beMerge)
        m_calls.clear();
	int szRed = gLPReadmemory(buf,mem->base,mem->size,MM_REMOVEINT3 | MM_SILENT | MM_PARTIAL);
    if (szRed >= 6)
    {//执行实际的查找工作
        searchCallsInMemory(mem->base,buf,szRed,imgbase,imgend);
    }
    free(buf);
    //刷新到界面
    flushCalls();
}

void xVMIATCallFrame::setEndOfImageForMemaddr(BOOL bReset)
{
    if (bReset)
    {
        m_mmaddrEnd = 0;
        return;
    }
    //取得当前选取的内存块地址
    ULONG_PTR addr = gMemoryTable->sorted.seladdr;
    if (addr == 0)  return;
    //取得内存块信息
    t_memory* mem = gLPFindmemory(addr);
    if (!mem) return;
    if (mem->size < 1)   return;

    //我们先试图确定映像大小
    ulong imgend = 0;
    ulong imgbase = 0;
    if (mem->owner != 0)
    {
        ulong szImage = 0;
        IMAGE_DOS_HEADER imdh = {0};
        IMAGE_NT_HEADERS imnh = {0};
        //先通过PE头获取映像大小，但是后面我们需要效验
		gLPReadmemory(&imdh,mem->owner,sizeof(imdh),MM_REMOVEINT3 | MM_SILENT | MM_PARTIAL);
        if (imdh.e_magic == IMAGE_DOS_SIGNATURE)
        {
			gLPReadmemory(&imnh,mem->owner + imdh.e_lfanew,sizeof(imnh),MM_REMOVEINT3 | MM_SILENT | MM_PARTIAL);
            if (imnh.Signature == IMAGE_NT_SIGNATURE)
                szImage = imnh.OptionalHeader.SizeOfImage;
        }
        ulong appdr = mem->owner;
        ulong append = 0;
        t_memory* mm = gLPFindmemory(appdr);
        while(mm)
        {
            if (mm->owner != mem->owner)
                break;
            append = mm->base + mm->size;
            mm = gLPFindmemory(mm->base+mm->size);
        }
        //我们容忍一定范围内的差异
        if (abs((int)(append - (mem->owner + szImage))) < max(imnh.OptionalHeader.SectionAlignment,0x1000))
        {
            imgend = append;
            imgbase = mem->owner;
        }
    }
    if (addr >= imgbase && addr < (imgend))
        m_mmaddrEnd = mem->base+mem->size;
}

void xVMIATCallFrame::flushCalls()
{
    gLPDestroysorteddata(&m_table_iat.sorted);
    gLPCreatesorteddata(&m_table_iat.sorted,sizeof(iatcall_info),1000,0,0,SDM_EXTADDR);
    int index = 1;
    for(iatcall_info* info = m_calls.tofirst();info;info = m_calls.tonext())
    {
        info->addr = index++;
        if ((info->rtflags & IATCALL_FLAG_TRACEFAILD) && !testFlushFlags(IATCF_FLAG_SHOWFAILD))
            continue;
        if ((info->rtflags & IATCALL_FLAG_REACHLIMIT) && !testFlushFlags(IATCF_FLAG_SHOWRTLIMIT))
            continue;

        if ((info->rtflags & IATCALL_FLAG_SUSPECT))
        {
            if (!testFlushFlags(IATCF_FLAG_SHOWSUSPECT))
                continue;
        }
        else
        {
            if ((info->rtflags & IATCALL_FLAG_TRACEDOUT) && (!(info->rtflags & IATCALL_FLAG_MARKINVAILD)))
            {
                if (!testFlushFlags(IATCF_FLAG_SHOWKNOWN))
                    continue;
            }else
            {
                if (!testFlushFlags(IATCF_FLAG_SHOWUNKNOWN))
                    continue;
            }
        }

        switch(info->calltype)
        {
        case iatcall_call:
        {
            if (!testFlushFlags(IATCF_FLAG_SHOWCALL))
                continue;
        }break;
        case iatcall_jmp:
        {
            if (!testFlushFlags(IATCF_FLAG_SHOWJMP))
                continue;
        }break;
        case iatcall_callmem:
        {
            if (!testFlushFlags(IATCF_FLAG_SHOWCALLMEM))
                continue;
        }break;
        case iatcall_jmpmem:
        {
            if (!testFlushFlags(IATCF_FLAG_SHOWJMPMEM))
                continue;
        }break;
        case iatcall_pushret:
        {
            if (!testFlushFlags(IATCF_FLAG_SHOWPUSHRET))
                continue;
        }break;
        case iatcall_memaddr:
        {
            if (!testFlushFlags(IATCF_FLAG_SHOWMEMADDR))
                continue;
        }break;
        }
        gLPAddsorteddata(&m_table_iat.sorted,info);
    }

    gLPFlash(L"Flush %d IAT Call Records.",m_table_iat.sorted.n);
    gLPUpdatetable(&m_table_iat,1);
}

void xVMIATCallFrame::traceSelected()
{
    openCPU();
    for (int i=0;i<m_table_iat.sorted.n;i++)
    {
        iatcall_info* iat = (iatcall_info*)gLPGetsortedbyindex(&m_table_iat.sorted,i);
        if (iat && iat->selected)
        {
            iat = findIATCall(iat->cmdaddr);
            if (iat)
                traceIAT(iat);

        }
    }
    flushCalls();
    closeCPU();
}

void xVMIATCallFrame::reset()
{
    closeCPU();
    m_cache.clear();
}



iatcall_info *xVMIATCallFrame::findIATCall(ULONG_PTR addr)
{
    iatcall_info* iatcall = m_calls.find(addr);
    return iatcall;
}
//实际执行追踪的函数
BOOL xVMIATCallFrame::traceIAT(iatcall_info *info)
{
    if (info->calltype == iatcall_memaddr)
    {
        ULONG_PTR dwPtr = 0;
		if (gLPReadmemory(&dwPtr,info->callto,sizeof(dwPtr),MM_REMOVEINT3 | MM_SILENT) == sizeof(dwPtr))
        {
            iatcall_api_cache* api = getExport(dwPtr);
            if (api)
            {
                info->retaddr = info->cmdaddr+info->szop;
                info->result_addr = dwPtr;
                hitIATCallModule(info);
                return (info->result_addr != 0);
            }
            //我们初始化虚拟化CPU
            m_vcpu->init_r3(FALSE);
            //将CPU初始化到函数追踪状态，包括初始化环境和地址
            m_vcpu->initIATrace(dwPtr);
        }else
        {
            info->rtflags |= IATCALL_FLAG_TRACEFAILD;
            return FALSE;
        }
    }else
    {
        //我们初始化虚拟化CPU
        m_vcpu->init_r3(FALSE);
        //将CPU初始化到函数追踪状态，包括初始化环境和地址
        m_vcpu->initIATrace(info->cmdaddr);
    }
    int nt = gxvmcfg.maxIATTraceLines;
    //这里我们最大允许虚拟化执行1000行指令，这里应当可以通过选项设定
    info->rtflags &= IATCALL_FLAG_KEEPMARK_MASK;
    info->rtflags |= IATCALL_FLAG_REACHLIMIT;
    for (int i=0;i<nt;i++)
    {
        bx_phy_address rsp = m_vcpu->get_reg32(BX_32BIT_REG_ESP);
        if ((rsp > DEOBFUSCATE_ESP) && ((rsp - DEOBFUSCATE_ESP) > (sizeof(ULONG_PTR)*2)))
        {
            info->rtflags |= IATCALL_FLAG_TRACEFAILD;
            break;
        }
        //我们使用cpu的单步执行
        vcpu_return iret = m_vcpu->step();
        if (iret == VCPU_LOOPRET_FINISHED)
        {   //当追踪结束以后，我们检查结果，将栈空间大小放到vi变量
            int vi = m_vcpu->isValidIATCall();
            info->rtflags |= IATCALL_FLAG_TRACEFAILD;
            if (vi != -1)
            {
                //取得cpu执行结束时的地址和栈地址
                bx_phy_address addrip = m_vcpu->getRip();
                rsp = m_vcpu->get_reg32(BX_32BIT_REG_ESP);
                ULONG_PTR retaddr = 0;
                //如果是一个函数调用那么应当会有返回地址
                m_vcpu->access_read_physical(rsp,sizeof(ULONG_PTR),&retaddr);
                info->hasHead = FALSE;
                info->retaddr = 0;
                //分类处理
                switch(info->calltype)
                {
                case iatcall_memaddr:
                case iatcall_jmpmem:
                case iatcall_pushret:
                {
                    if (vi == 0)    //如果是此两种类型，如果是正确的函数调用，那么栈应当平衡
                    {
                        if (retaddr == 0 || (retaddr - info->cmdaddr <= 6))
                        {
                            info->retaddr = retaddr;
                            info->result_addr = addrip;
                            info->rtflags &= IATCALL_FLAG_KEEPMARK_MASK;
                        }
                    }
                }break;
                case iatcall_callmem:
                {
                    if (vi == sizeof(ULONG_PTR))    //如果是该类型栈应当多出一个返回地址
                    {
                        if (retaddr == info->cmdaddr + 6)
                        {
                            info->retaddr = retaddr;
                            info->result_addr = addrip;
                            info->rtflags &= IATCALL_FLAG_KEEPMARK_MASK;
                        }
                    }
                }break;
                case iatcall_call:
                {
                    if (vi == sizeof(ULONG_PTR))
                    {
                        if (retaddr - info->cmdaddr <= 8)
                        {
                            info->retaddr = retaddr;
                            info->result_addr = addrip;
                            info->rtflags &= IATCALL_FLAG_KEEPMARK_MASK;
                        }
                    }
                    else if (vi <= 0)   //如果栈没有平衡，那么我们还要检测是否为特定加密调用
                    {
                        int stsize = abs(vi)+sizeof(ULONG_PTR);
                        if (m_vcpu->checkESPSpace((const char*)&info->ophead,1,stsize))
                        {
                            if (retaddr - info->cmdaddr <= 8)
                            {
                                info->hasHead = TRUE;
                                info->retaddr = retaddr;
                                info->result_addr = addrip;
                                info->rtflags &= IATCALL_FLAG_KEEPMARK_MASK;
                            }
                        }else
                        {
                            iatcall_info iatcp = *info;
                            iatcp.calltype = iatcall_jmp;
                            if (traceIAT(&iatcp))
                            {
                                iatcp.rtflags |= IATCALL_FLAG_SUSPECT;
                                *info = iatcp;
                            }
                        }
                    }
                    if (info->result_addr && !info->hasHead)
                    {
                        if (info->ophead == 0x90)
                            info->hasHead = TRUE;
                    }
                }break;
                case iatcall_jmp:
                {
                    if (vi == 0)
                    {
                        if (retaddr == 0 || (retaddr - info->cmdaddr <= 6))
                        {
                            info->retaddr = retaddr;
                            info->result_addr = addrip;
                            info->rtflags &= IATCALL_FLAG_KEEPMARK_MASK;
                        }
                    }
                    else if (vi < 0)
                    {
                        int stsize = abs(vi);
                        if (m_vcpu->checkESPSpace((const char*)&info->ophead,1,stsize))
                        {
                            if (retaddr == 0 || (retaddr - info->cmdaddr <= 6))
                            {
                                info->hasHead = TRUE;
                                info->result_addr = addrip;
                                info->retaddr = retaddr;
                                info->rtflags &= IATCALL_FLAG_KEEPMARK_MASK;
                                info->rtflags |= IATCALL_FLAG_SUSPECT;
                            }
                        }
                    }
                    if (info->result_addr && !info->hasHead)
                    {
                        if (info->ophead == 0x90)
                            info->hasHead = TRUE;
                    }
                }break;
                }
            }
            break;
        }else if (iret != VCPU_LOOPRET_NORMAL)
        {
            info->rtflags &= IATCALL_FLAG_KEEPMARK_MASK;
            info->rtflags |= IATCALL_FLAG_TRACEFAILD;
            break;
        }
    }
    if (info->result_addr != 0)
        hitIATCallModule(info);
    return (info->result_addr != 0);
}

void xVMIATCallFrame::tempFixupSelected()
{
    for (int i=0;i<m_table_iat.sorted.n;i++)
    {
        iatcall_info* iat = (iatcall_info*)gLPGetsortedbyindex(&m_table_iat.sorted,i);
        if (iat && iat->selected)
        {
            iat = findIATCall(iat->cmdaddr);
            if (iat)
                fixIATCall(iat,TRUE);
        }
    }
    gLPUpdatetable(&m_table_iat,1);
}

void xVMIATCallFrame::fixupSelectedCall()
{
    for (int i=0;i<m_table_iat.sorted.n;i++)
    {
        iatcall_info* iat = (iatcall_info*)gLPGetsortedbyindex(&m_table_iat.sorted,i);
        if (iat && iat->selected)
        {
            iat = findIATCall(iat->cmdaddr);
            if (iat)
                fixIATCall(iat,FALSE);
        }
    }
    flushCalls();
}
//修复一个调用代码
BOOL xVMIATCallFrame::fixIATCall(iatcall_info *info, BOOL direct)
{
    BYTE bufjmp[16];
    ULONG_PTR fixaddr = info->cmdaddr;
    //取出调用代码指令地址，转入分类处理
    switch(info->calltype)
    {
    case iatcall_call:
    {
        info->fixedState = fixed_faild;
        if (info->hasHead) fixaddr--;
        int region = info->retaddr - fixaddr;
        if (direct)
        {
            //CALL类型我们需要验证返回地址，如果发现返回地址减去指令地址并
            //不在正常的5-6范围内，那么说明结果可能是不正确的
            if (info->result_addr && info->retaddr)
            {
                if (region == 6 || (region == 5 && info->opsuffix == 0x90))
                {   //先将范围内全部设置为nop
                    memset(bufjmp,0x90,region);
                    bufjmp[0] = 0xE8;
                    *(ULONG_PTR*)(bufjmp+1) = info->result_addr - fixaddr - 5;
                    gLPWritememory(bufjmp,fixaddr,region,MM_SILENT);
                    info->fixedState = fixed_testmode;
                }
            }
        }else
        {
            if (region == 6 || (region == 5 && info->opsuffix == 0x90))
            {
                iat_func* api = getFuncInfo(info->modhash,info->funchash);
                if (api && api->IATRVA != 0)
                {
                    memset(bufjmp,0x90,6);
                    bufjmp[0] = 0xFF;
                    bufjmp[1] = 0x15;
                    *(ULONG_PTR*)(bufjmp+2) = api->IATRVA;
                    gLPWritememory(bufjmp,fixaddr,6,MM_SILENT);
                    info->fixedState = fixed_realmode;
                }
            }
        }
    }break;
    case iatcall_jmp:
    {   //由于jmp类型并没有返回地址，因此我们直接修复
        //但是jmp可以通过外部方式来验证，例如jmp总是会被call调用
        //我们可以搜索这些调用来验证jmp是否为一个函数调用
        info->fixedState = fixed_faild;
        if (info->hasHead) fixaddr--;
        if (direct)
        {
            if (info->result_addr)
            {
                bufjmp[0] = 0xE9;
                *(ULONG_PTR*)(bufjmp+1) = info->result_addr - fixaddr - 5;
                gLPWritememory(bufjmp,fixaddr,5,MM_SILENT);
                info->fixedState =  fixed_testmode;
            }
        }else
        {
            iat_func* api = getFuncInfo(info->modhash,info->funchash);
            if (api && api->IATRVA != 0)
            {
                bufjmp[0] = 0xFF;
                bufjmp[1] = 0x25;
                *(ULONG_PTR*)(bufjmp+2) = api->IATRVA;
                gLPWritememory(bufjmp,fixaddr,6,MM_SILENT);
                info->fixedState = fixed_realmode;
            }
        }
    }break;
    case iatcall_callmem:
    {
        info->fixedState = fixed_faild;
        int region = info->retaddr - info->cmdaddr;
        if (region == 6)
        {
            //实际上测试修复与文件修复都是在同一个函数里面实现的只是在具体的编码上不同
            if (direct)
            {
                if (info->result_addr && info->retaddr)
                {

                    memset(bufjmp,0x90,region);
                    bufjmp[0] = 0xE8;
                    *(ULONG_PTR*)(bufjmp+1) = info->result_addr - info->cmdaddr - 5;
                    gLPWritememory(bufjmp,info->cmdaddr,region,MM_SILENT);
                    info->fixedState = fixed_testmode;
                }
            }else
            {
                //从之前的缓冲当中找出IATRVA地址
                iat_func* api = getFuncInfo(info->modhash,info->funchash);
                if (api && api->IATRVA != 0)
                {
                    memset(bufjmp,0x90,region);
                    bufjmp[0] = 0xFF;
                    bufjmp[1] = 0x15;
                    *(ULONG_PTR*)(bufjmp+2) = api->IATRVA;
                    gLPWritememory(bufjmp,fixaddr,region,MM_SILENT);
                    info->fixedState = fixed_realmode;
                }
            }
        }
    }break;
    case iatcall_pushret:
    case iatcall_jmpmem:
    {
        info->fixedState = fixed_faild;
        if (direct)
        {
            if (info->result_addr)
            {
                bufjmp[0] = 0xE9;
                *(ULONG_PTR*)(bufjmp+1) = info->result_addr - info->cmdaddr - 5;
                gLPWritememory(bufjmp,info->cmdaddr,5,MM_SILENT);
                info->fixedState = fixed_testmode;
            }
        }else
        {
            iat_func* api = getFuncInfo(info->modhash,info->funchash);
            if (api && api->IATRVA != 0)
            {
                bufjmp[0] = 0xFF;
                bufjmp[1] = 0x25;
                *(ULONG_PTR*)(bufjmp+2) = api->IATRVA;
                gLPWritememory(bufjmp,fixaddr,6,MM_SILENT);
                info->fixedState = fixed_realmode;
            }
        }
    }break;
    case iatcall_memaddr:
    {
        info->fixedState = fixed_faild;
        if (direct)
        {
            if (info->result_addr)
            {
                gLPWritememory(&info->result_addr,info->callto,sizeof(info->result_addr),MM_SILENT);
                info->fixedState = fixed_testmode;
            }
        }else
        {
            iat_func* api = getFuncInfo(info->modhash,info->funchash);
            if (api && api->IATRVA != 0)
            {
                gLPWritememory(&api->IATRVA,info->cmdaddr+info->memoft,sizeof(ULONG_PTR),MM_SILENT);
                info->fixedState = fixed_realmode;
            }
        }
    }break;
    }
    return ((info->fixedState == fixed_testmode) || (info->fixedState == fixed_realmode));
}

void xVMIATCallFrame::addArrayToIATCall(ULONG_PTR addr, int size)
{
    int naddr = size / sizeof(ULONG_PTR);
    ULONG_PTR* memaddrs = new ULONG_PTR[naddr];
	int nred = gLPReadmemory(memaddrs,addr,naddr*sizeof(ULONG_PTR),MM_REMOVEINT3 | MM_SILENT);
    if (nred == naddr*sizeof(ULONG_PTR))
    {
        for (int i=0;i<naddr;i++)
        {
            ULONG_PTR addrMem = memaddrs[i];
            BYTE tmp[4];
			nred = gLPReadmemory(tmp,addrMem,sizeof(tmp),MM_REMOVEINT3 | MM_SILENT);
            if (nred != sizeof(tmp))
                continue;
            ULONG_PTR cmdaddr = addr + (i*sizeof(ULONG_PTR));
            if (m_calls.find(cmdaddr) == 0)
            {
                iatcall_info iatinfo;
                memset(&iatinfo,0,sizeof(iatinfo));
                iatinfo.szop = 0;
                iatinfo.cmdaddr = cmdaddr;
                iatinfo.memoft = 0;
                iatinfo.selected = FALSE;
                iatinfo.fixedState = fixed_none;
                iatinfo.callto = cmdaddr;
                iatinfo.size = 1;
                iatinfo.type = 0;
                iatinfo.calltype = iatcall_memaddr;
                m_calls[iatinfo.cmdaddr] = iatinfo;
            }
        }
    }
    delete[] memaddrs;
    flushCalls();
}

void xVMIATCallFrame::setIATArea(ULONG_PTR addr, int size, BOOL onlyFix)
{
    openCPU();
    //计算并读取出IAT表范围数据
    int naddr = size / sizeof(ULONG_PTR);
    ULONG_PTR* memaddrs = new ULONG_PTR[naddr];
	int nred = gLPReadmemory(memaddrs,addr,naddr*sizeof(ULONG_PTR),MM_REMOVEINT3 | MM_SILENT);
    if (nred == naddr*sizeof(ULONG_PTR))
    {
        //遍历每一个可能的地址
        for (int i=0;i<naddr;i++)
        {
            ULONG_PTR addrMem = memaddrs[i];
            BYTE tmp[4];
			nred = gLPReadmemory(tmp,addrMem,sizeof(tmp),MM_REMOVEINT3 | MM_SILENT);
            if (nred != sizeof(tmp))
                continue;
            ULONG_PTR cmdaddr = addr + (i*sizeof(ULONG_PTR));
            iatcall_info iatinfo;
            memset(&iatinfo,0,sizeof(iatinfo));
            iatinfo.szop = 0;
            iatinfo.cmdaddr = cmdaddr;
            iatinfo.memoft = 0;
            iatinfo.selected = FALSE;
            iatinfo.fixedState = fixed_none;
            iatinfo.callto = cmdaddr;
            iatinfo.size = 1;
            iatinfo.type = 0;
            iatinfo.calltype = iatcall_memaddr;
            //追踪每一个地址
            if (traceIAT(&iatinfo))
            {
                if (onlyFix)
                {
                    gLPWritememory(&iatinfo.result_addr,cmdaddr,sizeof(ULONG_PTR),MM_SILENT);
                }else
                {
                    //如果追踪成功了，我们将其对应的ThunkRVA保存到缓冲区
                    iat_func* ifc =hitIATCallModule(&iatinfo);
                    if (ifc) ifc->IATRVA = iatinfo.callto;
                }
            }
        }
    }
    delete[] memaddrs;
    flushCalls();
    closeCPU();
}

void xVMIATCallFrame::traceAll()
{

    openCPU();
    for (int i=0;i<m_table_iat.sorted.n;i++)
    {
        iatcall_info* iat = (iatcall_info*)gLPGetsortedbyindex(&m_table_iat.sorted,i);
        if (iat)
        {
            iat = findIATCall(iat->cmdaddr);
            if (iat)
                traceIAT(iat);
        }
    }
    refineResults();
    gLPUpdatetable(&m_table_iat,1);
    closeCPU();
}

void xVMIATCallFrame::openCPU()
{
    if (!m_vcpu)
    {
        m_vcpu = new xVMDebugCPU;
        m_vcpu->init_r3();
    }
}

void xVMIATCallFrame::closeCPU()
{
    if (m_vcpu)
    {
        delete m_vcpu;
        m_vcpu = 0;
    }
}

void xVMIATCallFrame::refineResults()
{
    for (int i=0;i<m_table_iat.sorted.n;i++)
    {
        iatcall_info* iat = (iatcall_info*)gLPGetsortedbyindex(&m_table_iat.sorted,i);
        if (iat)
        {
            iat = findIATCall(iat->cmdaddr);
            if (iat)
            {
                if(iat->calltype == iatcall_call)
                {
                    iatcall_info* iatinfo = m_calls.find(iat->callto);

                    if (!iatinfo)
                    {
                        iatinfo = m_calls.find(iat->callto+1);
                        if (iatinfo)
                        {
                            if (!(((iatinfo->calltype == iatcall_call) || (iatinfo->calltype == iatcall_jmp))
                                  && iatinfo->hasHead))
                                iatinfo = 0;
                        }
                    }
                    if (iatinfo)
                    {
                        if (iatinfo->rtflags & IATCALL_FLAG_TRACEDOUT)
                        {
                            iat->rtflags = IATCALL_FLAG_TRACEDOUT | IATCALL_FLAG_MARKINVAILD | IATCALL_FLAG_SUSPECT;
                        }
                    }
                }
            }
        }
    }
    flushCalls();
}



iatcall_mod_cache *xVMIATCallFrame::cacheModuleExport(ULONG_PTR addr)
{
    iatcall_mod_cache* imod = cacheModuleExportFromFile(addr);
    if (imod) return imod;
    return cacheModuleExportFromMemory(addr);
}

#include <ShellAPI.h>
#include <Shlwapi.h>
#include <ShlObj.h>
iatcall_mod_cache *xVMIATCallFrame::cacheModuleExportFromFile(ULONG_PTR addr)
{

    iatcall_cache::iterator itr = m_cache.find(addr);
    if (itr != m_cache.end())
        return &itr->second;

    t_module* tcmod = gLPFindmodule(addr);
    if (!tcmod) return 0;
    if (tcmod->base != addr)
    {
        itr = m_cache.find(tcmod->base);
        if (itr != m_cache.end())
            return &itr->second;
    }
    wchar_t path1[MAX_PATH];
    wchar_t path2[MAX_PATH];
    wchar_t pp[MAX_PATH];
    wchar_t modname[MAX_PATH];
    wchar_t* lpfn = 0;
    wcscpy(path1,tcmod->path);
    ::GetFullPathNameW(tcmod->path,sizeof(pp),pp,&lpfn);
    if (lpfn)
    {
        wcscpy(modname,lpfn);
        *lpfn = 0;
        int path2len = GetSystemDirectoryW(path2,sizeof(path2)/sizeof(wchar_t));
        if (path2len > 0)
            path2[path2len++] = '\\';
        path2[path2len] = 0;
        if (wcsnicmp(pp,path2,path2len) == 0)
        {
            int path1len = GetSystemWow64DirectoryW(path1,sizeof(path1)/sizeof(wchar_t));
            if (path1len > 0)
                path1[path1len++] = '\\';
            path1[path1len] = 0;
            wcscat(path1,modname);
        }
    }
    //EnableWow64FsRedirection(TRUE);
    HANDLE hf =CreateFileW(path1,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
    //EnableWow64FsRedirection(FALSE);
    if (hf == INVALID_HANDLE_VALUE)
        return 0;
    HANDLE hMap = CreateFileMapping(hf,0,PAGE_READONLY,0,0,0);
    if (hMap == 0)
    {
        CloseHandle(hf);
        return 0;
    }
    iatcall_mod_cache * modret = 0;
    DWORD szData = GetFileSize(hf,0);
    const char* lpData = (const char*)MapViewOfFile(hMap,FILE_MAP_READ,0,0,szData);
    if (lpData && szData)
    {
        PIMAGE_OPTIONAL_HEADER imoh = EnterImageOpHeader(lpData);
        DWORD lpExportRVA =  imoh->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
        DWORD szExport = imoh->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;
        if (lpExportRVA && szExport)
        {
            PIMAGE_EXPORT_DIRECTORY	imED	=	(PIMAGE_EXPORT_DIRECTORY)(lpData+RvaToOffset(lpData,lpExportRVA));
            if (imED->NumberOfFunctions > 0)
            {
                WORD* lpOrdinals = imED->AddressOfNameOrdinals?(WORD*)(lpData+RvaToOffset(lpData,imED->AddressOfNameOrdinals)):0;
                DWORD* lpNames =  imED->AddressOfNames?(DWORD*)(lpData + RvaToOffset(lpData,imED->AddressOfNames)):0;
                DWORD* lpRvas = (DWORD*)(lpData + RvaToOffset(lpData,imED->AddressOfFunctions));
                iatcall_mod_cache imod;
                imod.name = findModName(tcmod->path);
                imod.addrbase = tcmod->base;
                imod.addrend = tcmod->base + tcmod->size;

                for (DWORD i=0;i<imED->NumberOfFunctions;i++)
                {
                    iatcall_api_cache iapi;
                    iapi.base = imED->Base+i;
                    iapi.modBase = imod.addrbase;
                    DWORD rvafunc = lpRvas[i];
                    iapi.addr = rvafunc;
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
                    iapi.hasName = oftName?TRUE:FALSE;
                    if (iapi.hasName)
                    {

                        iapi.name = SingleStringToWide(lpData + RvaToOffset(lpData,oftName));
                        if (iapi.name.size() > 512)
                        {
                            iapi.name.clear();
                            iapi.hasName = FALSE;
                        }
                    }
                    if (!iapi.hasName)
                    {
                        char tmpNameBuf[16];
                        sprintf(tmpNameBuf,"#%04X",iapi.base);
                        iapi.name = SingleStringToWide(tmpNameBuf);
                    }
                    imod.apis.insert(std::make_pair(iapi.addr,iapi));
                }
                modret = &m_cache.insert(std::make_pair(imod.addrbase,imod)).first->second;
            }
        }
        UnmapViewOfFile(lpData);
    }
    CloseHandle(hMap);
    CloseHandle(hf);
    return modret;
}

iatcall_mod_cache *xVMIATCallFrame::cacheModuleExportFromMemory(ULONG_PTR addr)
{
    iatcall_cache::iterator itr = m_cache.find(addr);
    if (itr != m_cache.end())
        return &itr->second;

    t_module* tcmod = gLPFindmodule(addr);
    if (!tcmod || tcmod->edatabase == 0 || tcmod->edatasize == 0)
        return 0;
    if (tcmod->base != addr)
    {
        itr = m_cache.find(tcmod->base);
        if (itr != m_cache.end())
            return &itr->second;
    }
    char* lpExport = (char*)malloc(tcmod->edatasize);
    if (!lpExport)
        return 0;

	int szExport = gLPReadmemory(lpExport,tcmod->edatabase,tcmod->edatasize,MM_REMOVEINT3 | MM_SILENT);
    if (szExport != tcmod->edatasize)
    {
        free(lpExport);
        return 0;
    }
    iatcall_mod_cache * modret = 0;
    int nlen;
    PIMAGE_EXPORT_DIRECTORY	imED	=	(PIMAGE_EXPORT_DIRECTORY)lpExport;
    if (imED->NumberOfFunctions > 0)
    {
        int szOrdinals = imED->NumberOfFunctions* sizeof(WORD);
        WORD* lpOrdinals = (WORD*)malloc(szOrdinals);
        if (lpOrdinals)
        {
			nlen = gLPReadmemory(lpOrdinals,imED->AddressOfNameOrdinals + tcmod->base,szOrdinals,MM_REMOVEINT3 | MM_SILENT);
            if (nlen != szOrdinals)
            {
                free(lpOrdinals);
                lpOrdinals = 0;
            }
        }
        int szNames = imED->NumberOfNames * sizeof(DWORD);
        DWORD* lpNames =  (DWORD*)malloc(szNames);
        if (lpNames)
        {
			nlen = gLPReadmemory(lpNames,imED->AddressOfNames + tcmod->base,szNames,MM_REMOVEINT3 | MM_SILENT);
            if (nlen != szNames)
            {
                free(lpNames);
                lpNames = 0;
            }
        }
        int szRvas = sizeof(DWORD)*imED->NumberOfFunctions;

        DWORD* lpRvas =  (DWORD*)malloc(szRvas);
        if (lpRvas)
        {
			nlen = gLPReadmemory(lpRvas,imED->AddressOfFunctions + tcmod->base,szRvas,MM_REMOVEINT3 | MM_SILENT);
            if (nlen != szRvas)
            {
                free(lpRvas);
                lpRvas = 0;
            }
        }
        iatcall_mod_cache imod;
        imod.name = findModName(tcmod->path);
        imod.addrbase = tcmod->base;
        imod.addrend = tcmod->base + tcmod->size;

        for (DWORD i=0;i<imED->NumberOfFunctions;i++)
        {
            iatcall_api_cache iapi;
            iapi.base = imED->Base+i;
            iapi.modBase = imod.addrbase;
            DWORD rvafunc = lpRvas[i];
            iapi.addr = rvafunc;
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
            iapi.hasName = oftName?TRUE:FALSE;
            char tmpNameBuf[MAX_PATH*2];
            if (iapi.hasName)
            {
                tmpNameBuf[0] = 0;
				nlen = gLPReadmemory(tmpNameBuf,iapi.modBase + oftName,sizeof(tmpNameBuf),MM_REMOVEINT3 | MM_SILENT);
            }else
            {
                sprintf(tmpNameBuf,"#%04X",iapi.base);
            }
            iapi.name = SingleStringToWide(tmpNameBuf);

            imod.apis.insert(std::make_pair(iapi.addr,iapi));
        }
        modret = &m_cache.insert(std::make_pair(imod.addrbase,imod)).first->second;
        if (lpOrdinals) free(lpOrdinals);
        if (lpRvas) free(lpRvas);
        if (lpNames) free(lpNames);
    }
    free(lpExport);
    return modret;
}


iatcall_api_cache* xVMIATCallFrame::getExport(ULONG_PTR addr)
{
    iatcall_mod_cache* imod = 0;
    if (m_cache.size() > 0)
    {
        iatcall_cache::iterator itr = m_cache.upper_bound(addr);
        if (itr != m_cache.begin())
            itr--;
        if ((addr >= itr->second.addrbase) && (addr < itr->second.addrend))
            imod = &itr->second;
    }
    if (!imod)
        imod = cacheModuleExport(addr);
    if (!imod)
        return 0;
    DWORD fRVA = addr - imod->addrbase;
    iatcall_apicache::iterator ifc = imod->apis.find(fRVA);
    if (ifc != imod->apis.end())
        return &ifc->second;
    return 0;
}

iat_func* xVMIATCallFrame::hitIATCallModule(iatcall_info *info)
{
    if (info->result_addr)
    {
        iatcall_api_cache* api = getExport(info->result_addr);
        if (api)
        {
            t_module* tcmod = gLPFindmodule(info->result_addr);
            if (tcmod)
            {
                const wchar_t* modname = findModName(tcmod->path);
                if (modname)
                {
                    long modhash = nhash_lower<wchar_t>(modname);
                    iat_modules::iterator itr = m_mods.find(modhash);
                    if (itr == m_mods.end())
                    {
                        iat_module imod;
                        imod.nameHash = modhash;
                        imod.name = modname;
                        itr = m_mods.insert(std::make_pair(modhash,imod)).first;
                    }
                    info->modhash = modhash;
                    info->funchash = nhash<wchar_t>(api->name.c_str());
                    info->rtflags &= IATCALL_FLAG_KEEPMARK_MASK;
                    info->rtflags |= IATCALL_FLAG_TRACEDOUT;
                    iat_module* imr = &itr->second;
                    iat_modfuncs::iterator itrfc = imr->funcs.find(info->funchash);
                    if (itrfc == imr->funcs.end())
                    {
                        iat_func ifc;
                        ifc.nameHash = info->funchash;
                        ifc.name = api->name;
                        ifc.modHash = modhash;
                        ifc.module = imr;
                        ifc.IATRVA = 0;
                        return &imr->funcs.insert(std::make_pair(info->funchash,ifc)).first->second;
                    }
                    return &itrfc->second;
                }
            }
        }else
        {
            info->rtflags = IATCALL_FLAG_SUSPECT;
            //info->result_addr = 0;
            info->modhash = 0;
            info->funchash = 0;
        }

    }
    return 0;
}


void xVMIATCallFrame::saveRecords(TCHAR *filename)
{

    //build module and api table
    for (iatcall_info* info = m_calls.tofirst();info;info = m_calls.tonext())
    {
        hitIATCallModule(info);
    }
    //save module table first
    NPacketBase pkmods;
    NPacketBase pkmodone;
    NPacketBase pkfunc;
    for (iat_modules::iterator itr = m_mods.begin();itr != m_mods.end();itr++)
    {
        pkmodone.setempty(false);
        iat_module* imr = &itr->second;
        pkmodone.putInt(iatcall_vid_hash,imr->nameHash);
        pkmodone.putWString(iatcall_vid_name,imr->name.c_str());
        for (iat_modfuncs::iterator itrfc = imr->funcs.begin();itrfc != imr->funcs.end();itrfc++)
        {
            pkfunc.setempty(false);
            iat_func* func = &itrfc->second;
            pkfunc.putInt(iatcall_vid_hash,func->nameHash);
            pkfunc.putInt(iatcall_vid_iatrva,func->IATRVA);
            pkfunc.putWString(iatcall_vid_name,func->name.c_str());

            pkmodone.putBinary(iatcall_vid_node,pkfunc.data(),pkfunc.size());
        }
        pkmods.putBinary(iatcall_vid_node,pkmodone.data(),pkmodone.size());
    }
    NPacketBase pk;
    pk.putBinary(iatcall_vid_module,pkmods.data(),pkmods.size());

    //save each records
    NPacketBase pkcall;
    for (iatcall_info* info = m_calls.tofirst();info;info = m_calls.tonext())
    {
        pkcall.setempty(false);
        pkcall.putInt64(iatcall_vid_cmdaddr,info->cmdaddr);
        pkcall.putInt64(iatcall_vid_callto,info->callto);
        pkcall.putByte(iatcall_vid_ophead,info->ophead);
        pkcall.putByte(iatcall_vid_opsuffix,info->opsuffix);
        if (info->szop > 0)
            pkcall.putBinary(iatcall_vid_opcode,(const char*)info->opcode,info->szop);
        pkcall.putInt(iatcall_vid_calltype,info->calltype);
        pkcall.putInt(iatcall_vid_memoft,info->memoft);
        pkcall.putInt64(iatcall_vid_retaddr,info->retaddr);
        pkcall.putInt(iatcall_vid_hashead,info->hasHead);
        pkcall.putInt(iatcall_vid_rtflags,info->rtflags);
        pkcall.putInt(iatcall_vid_modhash,info->modhash);
        pkcall.putInt(iatcall_vid_apihash,info->funchash);
        pk.putBinary(iatcall_vid_record,pkcall.data(),pkcall.size());
    }

    HANDLE hf = CreateFile(filename,GENERIC_READ|GENERIC_WRITE,0,0,OPEN_ALWAYS,0,0);
    if (hf == INVALID_HANDLE_VALUE)
        return;
    SetFilePointer(hf,0,0,FILE_BEGIN);
    SetEndOfFile(hf);
    DWORD dwted = 0;
    WriteFile(hf,pk.data(),pk.size(),&dwted,0);
    SetEndOfFile(hf);
    CloseHandle(hf);

}

void xVMIATCallFrame::loadRecords(TCHAR *filename)
{
    NAutoLargeBufferA   nla;
    nla.load(filename);
    const char* lpdata = 0;
    int szdata = 0;
    lpdata = nla.data(szdata,0,false);
    if (!lpdata || szdata < 1)
        return;
    m_mods.clear();
    m_calls.clear();
    NPacketBase pk(lpdata,szdata,false);
    char* lpnode = pk.tofirst();
    while(lpnode)
    {
        switch(pk.vid())
        {
        case iatcall_vid_module:
        {
            NPacketBase pkmods(pk.block(),pk.szblock(),false);
            char*lpmod = pkmods.tofirst();
            while(lpmod)
            {
                NPacketBase pkmodone(lpmod,pkmods.szblock(),false);

                iat_module imod;
                imod.nameHash = pkmodone.getInt(iatcall_vid_hash);
                imod.name = pkmodone.getWString(iatcall_vid_name);
                iat_modules::iterator itrmod = m_mods.insert(std::make_pair(imod.nameHash,imod)).first;
                iat_module* imodptr = &itrmod->second;
                char* lpnode = pkmodone.tofirst();
                while(lpnode)
                {
                    switch(pkmodone.vid())
                    {
                    case iatcall_vid_node:
                    {
                        NPacketBase pknode(lpnode,pkmodone.szblock(),false);
                        iat_func func;
                        func.nameHash = pknode.getInt(iatcall_vid_hash);
                        func.name = pknode.getWString(iatcall_vid_name);
                        func.IATRVA = pknode.getInt(iatcall_vid_iatrva);
                        iat_modfuncs::iterator ifc = imodptr->funcs.insert(std::make_pair(func.nameHash,func)).first;
                        ifc->second.module =imodptr;
                    }break;
                    }
                    lpnode = pkmodone.tonext();
                }

                lpmod = pkmods.tonext();
            }
        }break;
        }
        lpnode = pk.tonext();
    }

    lpnode = pk.tofirst();
    while(lpnode)
    {
        switch(pk.vid())
        {
        case iatcall_vid_record:
        {
            NPacketBase pkcall(lpnode,pk.szblock(),false);
            iatcall_info info;
            memset(&info,0,sizeof(info));


            info.cmdaddr = pkcall.getLongLong(iatcall_vid_cmdaddr);
            info.callto = pkcall.getLongLong(iatcall_vid_callto);
            info.ophead = pkcall.getByte(iatcall_vid_ophead);
            info.opsuffix = pkcall.getByte(iatcall_vid_opsuffix);
            char* lpval = 0;
            int szval = 0;
            if (pkcall.getBinary(iatcall_vid_opcode,lpval,szval))
            {
                info.szop = min(szval,sizeof(info.opcode));
                memcpy(info.opcode,lpval,info.szop);
            }
            info.calltype = (IATCall_Type)pkcall.getInt(iatcall_vid_calltype);
            info.memoft = pkcall.getInt(iatcall_vid_memoft);
            info.retaddr = pkcall.getLongLong(iatcall_vid_retaddr);
            info.hasHead = pkcall.getInt(iatcall_vid_hashead);
            info.rtflags = pkcall.getInt(iatcall_vid_rtflags);
            info.modhash = pkcall.getInt(iatcall_vid_modhash);
            info.funchash = pkcall.getInt(iatcall_vid_apihash);
            m_calls[info.cmdaddr] = info;

        }break;
        }
        lpnode = pk.tonext();
    }
    flushCalls();
}


void xVMIATCallFrame::searchCallsInMemory(ULONG_PTR base, BYTE *lpMem, int szmem, ULONG_PTR imgbase, DWORD imgend)
{
    //我们防止越界访问
    int szLoop = szmem - 5;
    iatcall_info iatinfo;
    memset(&iatinfo,0,sizeof(iatinfo));
    //直接遍历段空间，使用硬编码搜索
    for (int oft =0;oft<szLoop;oft++)
    {
        BYTE op1 = *(BYTE*)(lpMem+oft);
        BOOL isCall = FALSE;
        switch(op1)
        {
        case 0xE8:  //call xxx
        {
            iatinfo.calltype = iatcall_call;
            iatinfo.callto = base+oft+5+*(DWORD*)(lpMem+oft+1);
            iatinfo.szop = 5;
            memcpy(iatinfo.opcode,lpMem+oft,5);
            iatinfo.ophead = (oft!=0)?(*(lpMem+oft-1)):0x90;
            iatinfo.opsuffix = (oft!=0)?(*(lpMem+oft+5)):0x90;
            //int nlen = gLPGetexportfrommemory(iatinfo.callto,fname);
            isCall = TRUE;
        }break;
        case 0xE9:// jmp xxx
        {
            iatinfo.calltype = iatcall_jmp;
            iatinfo.callto = base+oft+5+*(DWORD*)(lpMem+oft+1);
            iatinfo.szop = 5;
            memcpy(iatinfo.opcode,lpMem+oft,5);
            iatinfo.ophead = (oft!=0)?(*(lpMem+oft-1)):0x90;
            iatinfo.opsuffix = (oft!=0)?(*(lpMem+oft+5)):0x90;
            isCall = TRUE;
        }break;
        case 0xFF: //call [xxx] or jmp [xxx]
        {
            BYTE op2 = *(BYTE*)(lpMem+oft+1);
            if (op2 == 0x15)
            {
                iatinfo.calltype = iatcall_callmem;
                iatinfo.callto = *(DWORD*)(lpMem+oft+2);
                iatinfo.szop = 6;
                memcpy(iatinfo.opcode,lpMem+oft,6);
                isCall = TRUE;
            }else if (op2 == 0x25)
            {
                iatinfo.calltype = iatcall_jmpmem;
                iatinfo.callto = *(DWORD*)(lpMem+oft+2);
                iatinfo.szop = 6;
                memcpy(iatinfo.opcode,lpMem+oft,6);
                isCall = TRUE;
            }
        }break;
        case 0x68: //push xxx ,ret
        {
            BYTE op2 = *(BYTE*)(lpMem+oft+5);
            if (op2 == 0xC3)
            {
                iatinfo.calltype = iatcall_pushret;
                iatinfo.callto = *(DWORD*)(lpMem+oft+1);
                iatinfo.szop = 6;
                memcpy(iatinfo.opcode,lpMem+oft,6);
                isCall = TRUE;
            }
        }break;
        }
        if (isCall)
        {
            iatinfo.cmdaddr = base+oft;
            iatinfo.selected = FALSE;
            iatinfo.fixedState = fixed_none;
            iatinfo.size = 1;
            iatinfo.type = 0;
            m_calls[iatinfo.cmdaddr] = iatinfo;
        }
    }
    if (imgbase > 0 && imgend > imgbase)
    {
        ULONG_PTR band = imgend;
        if (m_mmaddrEnd >= imgbase && m_mmaddrEnd < imgend)
            band = m_mmaddrEnd;

        //查找内存地址引用
        for (int oft =0;oft<szLoop;oft++)
        {
            ULONG_PTR memaddr = *(ULONG_PTR*)(lpMem+oft);//首先假设为一个地址
            if (memaddr >= (imgbase+1) && memaddr < imgend)
            {   //如果这个地址是在映像范围内假设有效
                ULONG_PTR tmpaddr = 0;
				if (gLPReadmemory(&tmpaddr,memaddr,sizeof(memaddr),MM_REMOVEINT3 | MM_SILENT) == sizeof(memaddr))
                {   //读取指针引用的内存中的地址数据
                    //如果目标指针仍然在映像内，我们认为是程序内数据引用，剔除
                    //这里我们应当可以根据设定指定映像大小，因为我们的快照PE会将中间层包含到PE大小内
                    if (tmpaddr < imgbase || tmpaddr >= band)
                    {
                        ULONG_PTR uddr =0;//最后我们再次验证引用的内存地址是否有效
						if (gLPReadmemory(&uddr,tmpaddr,sizeof(tmpaddr),MM_REMOVEINT3 | MM_SILENT) == sizeof(tmpaddr))
                        {   //如果地址有效，我们强制以指令解析
                            for (int i=1;i<(16-sizeof(ULONG_PTR));i++)
                            {
                                t_disasm td;
                                td.hiliteindex = 0;
                                td.hilitereg = 0;
                                td.hiregindex = 0;
                                BYTE* lpOP = lpMem+oft-i;
                                ULONG_PTR baddr = base+oft-i;
                                if (lpOP < lpMem)
                                    break;
                                int clen = gLPDisasm(lpOP,i+sizeof(ULONG_PTR),baddr,0,&td,0,0,0);
                                if (clen > 0 && td.memconst == memaddr)
                                {   //我们只记录那些仅仅以内存常量来寻址的指令
                                    BOOL vaild = FALSE;
                                    for (int j=0;j<NOPERAND;j++)
                                    {
                                        if (td.op[j].features == 0)
                                            break;
                                        if (td.op[j].opconst == memaddr && td.op[j].aregs == 0)
                                        {
                                            vaild = TRUE;
                                            break;
                                        }
                                    }
                                    //我们的前5种指令的优先级大于内存地址引用，因此不覆盖插入
                                    if (vaild && m_calls.find(baddr) == 0)
                                    {
                                        iatinfo.szop = min(clen,sizeof(iatinfo.opcode));
                                        memcpy(iatinfo.opcode,lpOP,iatinfo.szop);
                                        iatinfo.memoft = i;
                                        iatinfo.cmdaddr = baddr;
                                        iatinfo.selected = FALSE;
                                        iatinfo.fixedState = fixed_none;
                                        iatinfo.callto = td.memconst;
                                        iatinfo.size = 1;
                                        iatinfo.type = 0;
                                        iatinfo.calltype = iatcall_memaddr;
                                        m_calls[iatinfo.cmdaddr] = iatinfo;
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    for(iatcall_info* info = m_calls.tofirst();info;)
    {
        //剔除目标地址超过可访问内存区域的记录
        if (info->callto >= 0x80000000 || info->callto <= 0x10000)
        {
            info = m_calls.removeCurrent();
            continue;
        }
        //剔除目标地址内存不可访问的记录
        ULONG_PTR testAddr;
		if (gLPReadmemory(&testAddr,info->callto,sizeof(testAddr),MM_REMOVEINT3 | MM_SILENT) <= 0)
        {
            info = m_calls.removeCurrent();
            continue;
        }
        //如果是内存指针型，我们剔除引用地址不可访问的记录
        if (info->calltype == iatcall_callmem || info->calltype == iatcall_jmpmem)
        {
            info->callto = testAddr;
			if (gLPReadmemory(&testAddr,testAddr,sizeof(testAddr),MM_REMOVEINT3 | MM_SILENT) <= 0)
            {
                info = m_calls.removeCurrent();
                continue;
            }
        }
        info = m_calls.tonext();
    }
    //    //过滤段内子程式调用，首先我们建立一个目标地址调用次数集合
    //    index_map<ULONG_PTR,int>    subProcs;
    //    for(iatcall_info* info = m_calls.tofirst();info;info = m_calls.tonext())
    //    {
    //        index_map<ULONG_PTR,int>::iterator itrsub = subProcs.find(info->callto);
    //        if (itrsub == subProcs.end())
    //        {
    //            subProcs.insert(std::make_pair(info->callto,1));
    //        }else
    //        {
    //            itrsub->second++;
    //        }
    //    }
    //    //如果一个目标地址被多次
    //    for (index_map<ULONG_PTR,int>::iterator itr = subProcs.begin();itr != subProcs.end();)
    //    {
    //        if (itr->second < 10)
    //        {
    //            itr = subProcs.erase(itr);
    //            continue;
    //        }
    //        itr++;
    //    }
    //    for(iatcall_info* info = m_calls.tofirst();info;info = m_calls.tonext())
    //    {
    //        if (subProcs.find(info->callto) != subProcs.end())
    //        {
    //            m_calls.remove(m_calls.currentKey());
    //            continue;
    //        }
    //    }
}

void xVMIATCallFrame::frameDestory()
{
    m_hFrame = 0;
}
int xVMIATCallFrame::lastsel() const
{
    return m_lastsel;
}

void xVMIATCallFrame::setLastsel(int lastsel)
{
    m_lastsel = lastsel;
}

iat_func *xVMIATCallFrame::getFuncInfo(long modhash, long funchash)
{
    iat_modules::iterator itr = m_mods.find(modhash);
    if (itr != m_mods.end())
    {
        iat_module* im = &itr->second;
        iat_modfuncs::iterator itrfc = im->funcs.find(funchash);
        if (itrfc != im->funcs.end())
        {
            return &itrfc->second;
        }
    }
    return 0;
}

void xVMIATCallFrame::setFlushFlags(long flags, bool on, bool replace)
{

    if (replace)
    {
        if (on)
            m_flushFlags = flags;
        else
            m_flushFlags = 0;
    }else
    {
        if (on)
            m_flushFlags |= flags;
        else
            m_flushFlags &= ~flags;
    }
    flushCalls();
}

bool xVMIATCallFrame::testFlushFlags(long flags)
{
    return (m_flushFlags & flags) == flags;
}

long xVMIATCallFrame::flushFlags() const
{
    return m_flushFlags;
}

xVMIATCallFrame* ensureIATCallFrame()
{
    if (!gIATCallsFrame)
    {
        gIATCallsFrame = new xVMIATCallFrame();
    }
    gIATCallsFrame->initFrame();
    return gIATCallsFrame;
}

void VMDebug_FindIATCalls(BOOL beMerge)
{
    xVMIATCallFrame* fm = ensureIATCallFrame();
    fm->findCalls(beMerge);
}
void VMDebug_AddtoIATCalls(t_table *pt)
{
    //为了方便，这里直接使用了customdata转换为dump窗体，但是我不确定是否所有
    //ollydbg Dump窗体都是这样，因此我们简单效验一下
    if (((ULONG_PTR)pt - (ULONG_PTR)pt->customdata) != offsetof(t_dump,table))
        return;
    //取出已选择范围信息
    t_dump* dmp = reinterpret_cast<t_dump*>(pt->customdata);
    ULONG_PTR addr1 = dmp->sel0;
    ULONG_PTR addr2 = dmp->sel1+1;
    int size = addr2-addr1;
    if (size <= 0)
        return;
    xVMIATCallFrame* fm = ensureIATCallFrame();
    //直接传递到addArrayToIATCall函数执行添加
    fm->addArrayToIATCall(addr1,size);
}

void VMDebug_SetIAT(t_table *pt, BOOL withFix)
{
    //为了方便，这里直接使用了customdata转换为dump窗体，但是我不确定是否所有
    //ollydbg Dump窗体都是这样，因此我们简单效验一下
    if (((ULONG_PTR)pt - (ULONG_PTR)pt->customdata) != offsetof(t_dump,table))
        return;
    //取出已选择范围信息
    t_dump* dmp = reinterpret_cast<t_dump*>(pt->customdata);
    ULONG_PTR addr1 = dmp->sel0;
    ULONG_PTR addr2 = dmp->sel1;
    int size = addr2-addr1;
    if (size <= 0)
        return;
    xVMIATCallFrame* fm = ensureIATCallFrame();
    //直接传递到addArrayToIATCall函数执行添加
    fm->setIATArea(addr1,size,withFix);
}

void VMDebug_IATReset()
{
    if (gIATCallsFrame)
        gIATCallsFrame->reset();
}


void VMDebug_SetEndOfImage(BOOL bReset)
{
    if (bReset &&!gIATCallsFrame)
        return;
    xVMIATCallFrame* fm = ensureIATCallFrame();
    fm->setEndOfImageForMemaddr(bReset);
}
