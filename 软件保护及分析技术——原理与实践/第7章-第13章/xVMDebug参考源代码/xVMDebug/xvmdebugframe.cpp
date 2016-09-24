#include <tchar.h>
#include "xvmdebugframe.h"
#include "../../../nCom/nautolargebuffer.h"
#include "../../../3rdparty/libudis86/extern.h"
#include "xvmblockframe.h"
#include "xvmdebug.h"
#include "ollydbg2_plugin.h"
#include "xvmblockframe.h"
xVMDebugFrame* gXVMFrame = 0;

Bit16u xvm_bswap16(Bit16u val16)
{
    return (val16<<8) | (val16>>8);
}

Bit32u xvm_bswap32(Bit32u val32)
{
    val32 = ((val32<<8) & 0xFF00FF00) | ((val32>>8) & 0x00FF00FF);
    return (val32<<16) | (val32>>16);
}


Bit64u xvm_bswap64(Bit64u val64)
{
    Bit32u lo = xvm_bswap32((Bit32u)(val64 >> 32));
    Bit32u hi = xvm_bswap32((Bit32u)(val64 & 0xFFFFFFFF));
    return ((Bit64u)hi << 32) | (Bit64u)lo;
}

int  context_draw_func(wchar_t *,uchar *,int *,struct t_table *,
                       t_sorthdr *,int,void *){
    return 0;
}

long memacc_func(struct t_table *table,HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
	xVMDebugFrame* xvm = (xVMDebugFrame*)table->customdata;
	if (!xvm)
		return 0;
	switch (msg)
	{
		//case WM_USER_DBLCLK:
		//{
		//					   xvm_record* xr = (xvm_record*)gLPGetsortedbyselection(&table->sorted,table->sorted.selected);
		//					   if (xr)
		//					   {
		//						   xr->selected = !xr->selected;
		//						   gLPUpdatetable(table,0);
		//					   }
		//}break;
	case WM_USER_KEY:
	{
		int ctrl,alt,shift;
		ctrl = (GetKeyState(VK_CONTROL) & SHIFTED) ? 1 : 0;
		shift = (GetKeyState(VK_SHIFT) & SHIFTED) ? 1 : 0;
		alt = (GetKeyState(VK_MENU) & SHIFTED) ? 1 : 0;
		switch (LOBYTE(lparam))
		{
		case VK_RETURN:
		{
			xvm_memaccess* ma = (xvm_memaccess*)gLPGetsortedbyselection(&table->sorted,table->sorted.selected);
			if (ma)
			{
				if (ctrl == 1 && alt == 0 && shift == 0)
				{
					if (gLPFindmemory(ma->memvalue))
						gLPSetcpu(0,0,ma->memvalue,0,0,CPU_DUMPHIST);
					else
					{
						if (gLPFindmemory(ma->memptr))
							gLPSetcpu(0,0,ma->memptr,0,0,CPU_DUMPHIST);
					}
				} if (ctrl == 1 && alt == 1 && shift == 0)
				{
					if (gLPFindmemory(ma->memvalue))
						gLPSetcpu(0,0,0,0,ma->memvalue,CPU_DUMPHIST);
					else
					{
						if (gLPFindmemory(ma->memptr))
							gLPSetcpu(0,0,0,0,ma->memptr,CPU_DUMPHIST);
					}
				}else if(ctrl == 0 && alt == 1 && shift == 0)
				{
					if (gLPFindmemory(ma->memptr))
						gLPSetcpu(0,0,0,0,ma->memptr,CPU_DUMPHIST);
				} else
				{
					if (gLPFindmemory(ma->memptr))
						gLPSetcpu(0,0,ma->memptr,0,0,CPU_DUMPHIST);
				}

			}
		}break;
		}
	}break;
	}
	return 0;
}

long disasm_func(struct t_table *table,HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
    xVMDebugFrame* xvm = (xVMDebugFrame*)table->customdata;
    if (!xvm)
        return 0;
    switch(msg)
    {
    case WM_USER_DBLCLK:
    {
        xvm_record* xr = (xvm_record*)gLPGetsortedbyselection(&table->sorted,table->sorted.selected);
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
		case VK_SPACE:
		{
			xvm->assemblerSelectRecord();
		}break;
        case 'T':
        {
            xvm->trace(trace_ateip);
        }break;
         case 'R':
        {
					 xvm->refresh();
            //xvm->traceSequence();
        }break;
		 case 'X':
		 {

		 }break;
        case 'C':xvm->clearSeqFlag();break;
		case 'H':
		{
			xvm->deObfuscateJcc(true);
			for (int i = 0; i < table->sorted.n; i++)
			{
				xvm_record* xrt = (xvm_record*)gLPGetsortedbyindex(&table->sorted,i);
				if (xrt && xrt->selected)
				{
					xvm->setRecordExFlags(xrt->addr - 1,RECORD_FLAG_DELETE);
				}
			}
			xvm->deleteJunkInSeq();
			xvm->flushSeq();
		}break;
        case 'J':
        {
            xvm->deObfuscateJcc(true);
        }break;
        case 'K':xvm->deObfuscateJcc(false);break;
		case 'L':{
			xvm->deObfuscateJcc(false);
			for (int i = 0; i < table->sorted.n; i++)
			{
				xvm_record* xrt = (xvm_record*)gLPGetsortedbyindex(&table->sorted,i);
				if (xrt && xrt->selected)
				{
					xvm->setRecordExFlags(xrt->addr - 1,RECORD_FLAG_DELETE);
				}
			}
			xvm->deleteJunkInSeq();
			xvm->flushSeq();
		}break;
        case 'M': xvm->doMemoryAnalyserCurrentSeq();break;
		case 'P':
		{
					 xvm->doDeObfuscatePattern();
		}break;
        case 'Y':
        {
            xvm->dobCurrentSequenceNormal(true);
        }break;
        case 'N':
        {
            xvm->dobCurrentSequenceNormal(false);
        }break;
		case 'V':
		{
			xvm->dobCurrentSequenceAll();
		}break;
        case 'B':xvm->dobCurrentSequencePushPop();
            break;
        case 'A':
        {
            xvm->DeobfuscateStackOB();
        }break;
		case 'O':
		{
			xvm->doDeObfuscatePatternAll();
		}break;
        case VK_DELETE:
        {
            for (int i=0;i<table->sorted.n;i++)
            {
                xvm_record* xrt = (xvm_record*)gLPGetsortedbyindex(&table->sorted,i);
                if (xrt && xrt->selected)
                {
                    xvm->setRecordExFlags(xrt->addr-1,RECORD_FLAG_DELETE);
                }
            }
            xvm->deleteJunkInSeq();
            xvm->flushSeq();
        }break;
        }
    }break;
    case WM_DESTROY:
    {
        if (xvm)
        {
            xvm->reset();
        }
    }break;
    }
    return 0;
}
void cpuRipChanged(ULONG_PTR rip)
{
    if (gXVMFrame)
        gXVMFrame->syncByRip(rip);
    VMDebug_BlockSyncEip(rip);
}

int  memacc_draw_func(wchar_t *result,uchar *mask,int *drawFlags, struct t_table *table,t_sorthdr *sorthdr,int reason,void *item)
{
	xVMDebugFrame* fm = (xVMDebugFrame*)table->customdata;
	if (!fm) return 0;
	xvm_memaccess* ma = (xvm_memaccess*)sorthdr;
	switch (reason)
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
		int flags = DRAW_GRAY;
		*drawFlags = flags;
		return wsprintfW(result,L"%d",ma->addr);
	}break;
	case 1:
	{
		int flags = DRAW_GRAY;
		*drawFlags = flags;
		return wsprintfW(result,L"%d",ma->instID);
	}break;
	case 2:
	{
		int flags = DRAW_GRAY;
		*drawFlags = flags;
		if (ma->memptr < (16 * 16) && ncasm::x86::is_valid_reg_bytesize(ma->memsize))
		{
			//treate as register
			ChainCPU* cc = fm->getChainCPU();
			if (cc)
			{
				ud_type opt = cc->getRegByOft(ma->memptr);
				if (opt != UD_NONE)
				{
					opt = NASM_I386::udex_get_rel_reg(opt,ma->memsize*8);
					const char* sreg = ud_reg_tab[opt - UD_R_AL];
					return wsprintfW(result,L"%S",sreg);
				}
			}
		}

		return wsprintfW(result,L"%08X",ma->memptr);
	}break;
	case 3:
	{
			  int flags = DRAW_GRAY;
			  *drawFlags = flags;
			  return wsprintfW(result,L"%d",ma->memsize);
	}break;
	case 4:
	{
			  int flags = DRAW_GRAY;
			  *drawFlags = flags;
			  result[0] = 0;
			  if (ma->mode & RECORD_FLAG_MEMREAD)
				  wcscat(result,L"R");
			  else
				  wcscat(result,L"-");

			  if (ma->mode & RECORD_FLAG_MEMWRITE)
				  wcscat(result,L"W");
			  else 
				  wcscat(result,L"-");
			  if (ma->mode & (RECORD_FLAG_SPREAD | RECORD_FLAG_SPWRITE))
				  wcscat(result,L"S");
			  else
				  wcscat(result,L"-");
			  return wcslen(result);
	}break;
	case 5:
	{
			  int flags = DRAW_GRAY;
			  *drawFlags = flags;
			  return wsprintfW(result,L"%08X",ma->memvalue);
	}break;
	default:return 0;
	}
	return 0;
}

int  disasm_draw_func(wchar_t *result,uchar *mask,int *drawFlags,
                      struct t_table *table,t_sorthdr *sorthdr,int reason,void *item)
{
    xVMDebugFrame* fm = (xVMDebugFrame*)table->customdata;
    xvm_record* xr = (xvm_record*)sorthdr;
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
		if (xr->selected)
			flags |= DRAW_COND | DRAW_EXTSEL;
		else if (xr->rc.flags & RECORD_FLAG_EXCEPTION)
			flags |= DRAW_BREAK | DRAW_EXTSEL;
		else if (xr->rc.flags & RECORD_FLAG_SEHFRAME)
			flags |= DRAW_SELECT | DRAW_EXTSEL;
        *drawFlags = flags;
        return wsprintfW(result,L"%d",sorthdr->addr);
    }break;
    case 1:
    {
        int flags = DRAW_GRAY;
        *drawFlags = flags;
        if (fm && xr->rc.ctx1.rip == fm->activeRip())
        {
            *drawFlags |=  DRAW_EIP | DRAW_EXTSEL;
            //gLPSettableselection(table,xr->addr);
        }
        return wsprintfW(result,L"%08X",xr->rc.ctx1.rip);
    }break;
    case 2:
    {
        t_disasm td;
        td.hiliteindex = 1;
        td.hilitereg = 0;
        td.hiregindex = 0;
        gLPDisasm(xr->rc.ctx1.opcode,sizeof(xr->rc.ctx1.opcode),xr->rc.ctx1.rip,0,&td,DA_TEXT|DA_HILITE|DA_MEMORY,0,0);
        wcscpy(result,td.result);
        memcpy(mask,td.mask,sizeof(td.mask));
        int flags = DRAW_MASK | DRAW_HILITE;
        *drawFlags = flags;
        if (xr->rc.flags & RECORD_FLAG_MULTIB)
        {
            wcscat(result,L"(*)");
        }
        return wcslen(result);
    }break;
    case 3:
    {
        return 0;
    }break;
    case 4:
    {
        int flags = DRAW_GRAY;
        *drawFlags = flags;
        return wsprintfW(result,L"EAX=%08X ECX=%08X EDX=%08X EBX=%08X ESP=%08X EBP=%08X ESI=%08X EDI=%08X EFLAG=%08X",
                         xr->rc.ctx1.regs[0],
                xr->rc.ctx1.regs[1],
                xr->rc.ctx1.regs[2],
                xr->rc.ctx1.regs[3],
                xr->rc.ctx1.regs[4],
                xr->rc.ctx1.regs[5],
                xr->rc.ctx1.regs[6],
                xr->rc.ctx1.regs[7],
        #ifndef _M_IX86
                xr->rc.ctx1.regs[8],
                xr->rc.ctx1.regs[9],
                xr->rc.ctx1.regs[10],
                xr->rc.ctx1.regs[11],
                xr->rc.ctx1.regs[12],
                xr->rc.ctx1.regs[13],
                xr->rc.ctx1.regs[14],
                xr->rc.ctx1.regs[15],
        #endif
                xr->rc.ctx1.eflags
                );
    }break;
    default:return 0;
    }


    return 0;
}

void disasm_sel_func(struct t_table *pt,int selected,int reason)
{
    xvm_record* xr = (xvm_record*)gLPGetsortedbyselection(&pt->sorted,selected);
    if (!xr)
        return;
    xVMDebugFrame* fm = (xVMDebugFrame*)pt->customdata;
	if (!fm) return;
    if (HIBYTE(GetKeyState(VK_SHIFT)))
    {
        for (int i=0;i<pt->sorted.n;i++)
        {
            xvm_record* xrt = (xvm_record*)gLPGetsortedbyindex(&pt->sorted,i);
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
                xvm_record* xrt = (xvm_record*)gLPGetsortedbyindex(&pt->sorted,i);
                if (xrt)
                    xrt->selected = TRUE;
            }
        }
    }else if (HIBYTE(GetKeyState(VK_CONTROL)))
    {
        xr->selected = !xr->selected;
	} else if (HIBYTE(GetKeyState('Z')))
	{
		fm->showMemChain(xr);
	}
    fm->setLastsel(selected,xr);
    if (HIBYTE(GetKeyState(VK_MENU)))
        gLPSetcpu(gLPGetcputhreadid(),xr->rc.ctx1.rip,0,1,0,CPU_NOCREATE);
}


void memacc_sel_func(struct t_table *pt,int selected,int reason)
{
	xvm_memaccess* xr = (xvm_memaccess*)gLPGetsortedbyselection(&pt->sorted,selected);
	if (!xr)
		return;
	xVMDebugFrame* fm = (xVMDebugFrame*)pt->customdata;
	if (fm) fm->syncRecordsByIndex(xr->instID);
}
enum xvmdebug_frame_menus{
    xvmdebug_frame_menu_loadrecords = 0
    ,xvmdebug_frame_menu_madeblock
	,xvmdebug_frame_menu_app
	,xvmdebug_frame_menu_detectbk
	,xvmdebug_frame_menu_showstackmac
	,xvm_menu_pattern
	,xvm_menu_pattern1
	,xvm_menu_pattern2
	,xvm_menu_pattern3
	,xvm_menu_pattern4
	,xvm_menu_pattern5
	,xvm_menu_runpattern
	,xvm_menu_saverecords
	,xvm_menu_exporttext
	,xvmdebug_frame_menu_showreadmac
	,xvmdebug_frame_menu_showwritemac
};


std::string getxVMDebugFullPathA(const char* lpsub)
{
	char path[MAX_PATH * 2];
	const char* lpleft = lpsub;
	int len = strlen(lpsub);
	int leftlen = len;
	NStringA::trimLeft(lpleft,leftlen,"\t \r\n");
	if (*lpleft == '.' || *lpleft == '/' || (*lpleft == '\\' && strnicmp(lpleft,"\\??",3)!=0))
	{
		GetModuleFileNameA(ghxVMDebug,path,sizeof(path) / sizeof(wchar_t));
		int nplen = strlen(path);
		for (int i = nplen - 1; i >= 0; i--)
		{
			if (path[i] == '/' || path[i] == '\\')
			{
				path[i + 1] = 0;
				break;
			}
		}
		strcat(path,lpleft);
	} else
		strcpy(path,lpleft);
	char path2[MAX_PATH * 2];
	int nlen = GetFullPathNameA(path,sizeof(path2) / sizeof(char),path2,0);
	return path2;
}


std::wstring getxVMDebugFullPathW(const wchar_t* lpsub)
{
	wchar_t path[MAX_PATH * 2];
	const wchar_t* lpleft = lpsub;
	int len = wcslen(lpsub);
	int leftlen = len;
	NStringW::trimLeft(lpleft,leftlen,L"\t \r\n");
	if (*lpleft == '.' || *lpleft == '/' || (*lpleft == '\\' && wcsnicmp(lpleft,L"\\??",3) != 0))
	{
		GetModuleFileNameW(ghxVMDebug,path,sizeof(path) / sizeof(wchar_t));
		int nplen = wcslen(path);
		for (int i = nplen - 1; i >= 0; i--)
		{
			if (path[i] == '/' || path[i] == '\\')
			{
				path[i + 1] = 0;
				break;
			}
		}
		wcscat(path,lpleft);
	} else
		wcscpy(path,lpleft);
	wchar_t path2[MAX_PATH * 2];
	int nlen = GetFullPathNameW(path,sizeof(path2) / sizeof(wchar_t),path2,0);
	return path2;
}


static int disam_Table_MenuProc(t_table *pt,wchar_t *name,ulong index,int mode) {
	xVMDebugFrame* fm = fm = (xVMDebugFrame*)pt->customdata;
	if (mode == MENU_VERIFY)
	{
		switch (index)
		{
		case xvmdebug_frame_menu_showstackmac:return (gxvmcfg.showMemAccFlags & mac_showstack)? MENU_CHECKED : MENU_NORMAL;
		case xvmdebug_frame_menu_showreadmac:return (gxvmcfg.showMemAccFlags & mac_showread)? MENU_CHECKED : MENU_NORMAL;
		case xvmdebug_frame_menu_showwritemac:return (gxvmcfg.showMemAccFlags & mac_showwrite) ? MENU_CHECKED : MENU_NORMAL;
		case xvm_menu_pattern1:
		case xvm_menu_pattern2:
		case xvm_menu_pattern3:
		case xvm_menu_pattern4:
		case xvm_menu_pattern5:{
			int idx = index - xvm_menu_pattern1;
			const wchar_t* lpstr = gxvmcfg.recentPASM[idx].c_str();
			if (lpstr[0] == 0)
				wcscpy(name,L"Empty...");
			else
				wcscpy(name,lpstr);
		}break;
		}
		return MENU_NORMAL;                // Always available
	}    
	else if (mode == MENU_EXECUTE && fm)
    {
        switch(index)
        {
        case xvmdebug_frame_menu_loadrecords:
        {
            xVMDebugFrame* fm = (xVMDebugFrame*)pt->customdata;
			TCHAR szFilter[] = _T("*.xtrace\0*.xtrace\0*.*\0*.*\0");
			TCHAR lpFilename[256] = { 0 };
			OPENFILENAME  lpofn = { 0 };
			DWORD len = 0;
			lpofn.lStructSize = sizeof (lpofn);
			lpofn.lpstrFilter = szFilter;
			lpofn.hwndOwner = pt->hparent;
			lpofn.hInstance = GetModuleHandleA(NULL);
			lpofn.lpstrFile = lpFilename;
			lpofn.Flags = OFN_HIDEREADONLY;
			lpofn.lpstrTitle = _T("Select Records File to Load");
			lpofn.nMaxFile = 0x256;
			if (!GetOpenFileName(&lpofn) || (len = _tcslen(lpFilename)) == 0)
				return MENU_NOREDRAW;
			fm->loadRecords(lpFilename);
        }break;
		case xvm_menu_saverecords:{
			TCHAR szFilter[] = _T("*.xtrace\0*.xtrace\0*.*\0*.*\0");
			TCHAR lpFilename[256] = { 0 };
			OPENFILENAME  lpofn = { 0 };
			DWORD len = 0;
			lpofn.lStructSize = sizeof (lpofn);
			lpofn.lpstrFilter = szFilter;
			lpofn.hwndOwner = pt->hparent;
			lpofn.hInstance = GetModuleHandleA(NULL);
			lpofn.lpstrFile = lpFilename;
			lpofn.Flags = OFN_HIDEREADONLY;
			lpofn.lpstrTitle = _T("Select File To Save Records:");
			lpofn.lpstrDefExt = _T(".xtrace");
			lpofn.nMaxFile = 0x256;
			if (!GetSaveFileName(&lpofn) || (len = _tcslen(lpFilename)) == 0)
				return MENU_NOREDRAW;
			fm->saveRecords(lpFilename);
		}break;
		case xvm_menu_exporttext:
		{
			TCHAR szFilter[] = _T("*.txt\0*.txt\0*.*\0*.*\0");
			TCHAR lpFilename[256] = { 0 };
			OPENFILENAME  lpofn = { 0 };
			DWORD len = 0;
			lpofn.lStructSize = sizeof (lpofn);
			lpofn.lpstrFilter = szFilter;
			lpofn.hwndOwner = pt->hparent;
			lpofn.hInstance = GetModuleHandleA(NULL);
			lpofn.lpstrFile = lpFilename;
			lpofn.lpstrDefExt = _T(".txt");
			lpofn.Flags = OFN_HIDEREADONLY;
			lpofn.lpstrTitle = _T("Export Records To Text:");
			lpofn.nMaxFile = 0x256;
			if (!GetSaveFileName(&lpofn) || (len = _tcslen(lpFilename)) == 0)
				return MENU_NOREDRAW;
			fm->exportToText(lpFilename);
		}break;
        case xvmdebug_frame_menu_madeblock:fm->makeCodeBlock(pt);break;
		case xvmdebug_frame_menu_detectbk: fm->detectCodeBlocks();break;
		case xvmdebug_frame_menu_showstackmac:{
			if (gxvmcfg.showMemAccFlags & mac_showstack)
				gxvmcfg.showMemAccFlags &= ~mac_showstack;
			else
				gxvmcfg.showMemAccFlags |= mac_showstack;
				fm->flushMemAccess();
		}break;
		case xvmdebug_frame_menu_showreadmac:{
			if (gxvmcfg.showMemAccFlags & mac_showread)
				gxvmcfg.showMemAccFlags &= ~mac_showread;
			else
				gxvmcfg.showMemAccFlags |= mac_showread;
			fm->flushMemAccess();
		}break;
		case xvmdebug_frame_menu_showwritemac:{
			if (gxvmcfg.showMemAccFlags & mac_showwrite)
				gxvmcfg.showMemAccFlags &= ~mac_showwrite;
			else
				gxvmcfg.showMemAccFlags |= mac_showwrite;
			fm->flushMemAccess();
		}break;
		case xvm_menu_pattern1:
		case xvm_menu_pattern2:
		case xvm_menu_pattern3:
		case xvm_menu_pattern4:
		case xvm_menu_pattern5:
		{
			xVMDebugFrame* fm = (xVMDebugFrame*)pt->customdata;
			if (fm)
			{
				int pidx = index - xvm_menu_pattern1;
				if (gxvmcfg.recentPASM[pidx].empty() || HIBYTE(GetKeyState(VK_SHIFT)))
				{
					TCHAR lpFilename[256];
					if (!getPasmFile(lpFilename,pt->hparent))
						return MENU_NOREDRAW;
					gxvmcfg.recentPASM[pidx] = lpFilename;
					fm->runPattern(lpFilename,HIBYTE(GetKeyState(VK_CONTROL)) ? TRUE : FALSE);
				}else
					fm->runPattern(gxvmcfg.recentPASM[pidx].c_str(),HIBYTE(GetKeyState(VK_CONTROL)) ? TRUE : FALSE);
				
			}
		}break;
		case xvm_menu_runpattern:
		{
			xVMDebugFrame* fm = (xVMDebugFrame*)pt->customdata;
			if (fm)
			{
				TCHAR lpFilename[256];
				if (!getPasmFile(lpFilename,pt->hparent))
					return MENU_NOREDRAW;
				fm->runPattern(lpFilename,HIBYTE(GetKeyState(VK_CONTROL))?TRUE:FALSE);
			}
		}break;

        }
        return MENU_NOREDRAW;
    };
    return MENU_ABSENT;
}



void VMDebug_debugEventLoop(DEBUG_EVENT* event)
{
    VMDebug_BlockProcessEvent(event);
}




xVMDebugFrame::xVMDebugFrame()
    :m_hFrame(0)
    ,m_activeRip(0)
    ,m_asm(32)
	,m_ccpu(0)
{
	memset(&m_frame,0,sizeof(m_frame));
}

xVMDebugFrame::~xVMDebugFrame()
{
	if (m_ccpu)
	{
		delete m_ccpu;
		m_ccpu = 0;
	}
}
//预执行追踪
void xVMDebugFrame::trace(trace_mode mode)
{
    //我们只允许在被调试程式处于暂停状态的时候才能执行追踪
    if (gRuninfo->status != STAT_PAUSED)
        return;
    ulong tid = gLPGetcputhreadid();
    t_thread* thread = gLPFindthread(tid);
    if (!thread)
        return;
    //取得我们开始追踪的地址，根据我们不同的追踪模式设定
    bx_address addr = 0;
    switch(mode)
    {
    case trace_atsel:   //从所选地址处开始
    {
        addr= gLPGetcpudisasmselection();
    }break;
    default:    //从程式暂停处开始
    {
        addr = thread->reg.ip;
    }break;
    }
    if (addr == 0) return;
    if (gxvmcfg.preExecDetectMultiBranch)
        EnableMBDetect();   //打开多分支探测
    initTrace(&thread->context);
    RIP = addr;
    int nt = gxvmcfg.maxPreExecLines;
    for (int i=0;i<nt;i++)    //最多追踪1000行，这些应该都制定相应的功能设定
    {
        vcpu_return vret = step();  //单步追踪
        if (vret != VCPU_LOOPRET_NORMAL)
            break;
    }
    if (gxvmcfg.preExecDetectMultiBranch)
        DisableMBDetect();  //关闭多分支探测
	//saveBlockRecords(L"Q:\\ToolKit\\OllyDbg2\\plugin\\xVMdebug\\records\\test.xblock");
    flushSeq();
}

void xVMDebugFrame::reset()
{
    m_hFrame = 0;
}

//初始化预执行窗体的时候
void xVMDebugFrame::initFrame(wchar_t* name,BOOL blockOnly,BOOL bShowTitle)
{
    if (m_hFrame)
        return;

	loadPatterns();
    wcscpy(m_frame.name,L"xVMDebug");

    memset(&m_table_disasm,0,sizeof(m_table_disasm));
    wcscpy(m_table_disasm.name,L"xvm_diasm_");
	wcscat(m_table_disasm.name,name);
	m_table_disasm.mode = TABLE_SYNTAX | TABLE_SAVEALL;
	if (!bShowTitle)
		m_table_disasm.mode |= TABLE_NOBAR;
    m_table_disasm.drawfunc = disasm_draw_func;
    m_table_disasm.tableselfunc = disasm_sel_func;
    m_table_disasm.tabfunc = disasm_func;
    m_table_disasm.customdata = this;

    m_table_disasm.bar.nbar = 5;
	m_table_disasm.bar.visible = bShowTitle;

	static t_menu patternmenu[] = {
		{ L"From File...",L"Run a pattern from file.",KK_DIRECT | KK_ALT | VK_OEM_3,disam_Table_MenuProc,NULL,xvm_menu_runpattern },
		{ L"|Empty",L"",KK_DIRECT | KK_ALT | '1',disam_Table_MenuProc,NULL,xvm_menu_pattern1 },
		{ L"Empty",L"",KK_DIRECT | KK_ALT | '2',disam_Table_MenuProc,NULL,xvm_menu_pattern2 },
		{ L"Empty",L"",KK_DIRECT | KK_ALT | '3',disam_Table_MenuProc,NULL,xvm_menu_pattern3 },
		{ L"Empty",L"",KK_DIRECT | KK_ALT | '4',disam_Table_MenuProc,NULL,xvm_menu_pattern4 },
		{ L"Empty",L"",KK_DIRECT | KK_ALT | '5',disam_Table_MenuProc,NULL,xvm_menu_pattern5 },
		{ NULL,NULL,K_NONE,NULL,NULL,0 }
	};

    static t_menu disam_menu[] = {
        { L"Make Code Block",L"Convert Selected To Code Block",K_NONE, disam_Table_MenuProc, NULL, xvmdebug_frame_menu_madeblock },
		{ L"Detect Code Blocks",L"Detect Code Blocks",K_NONE,disam_Table_MenuProc,NULL,xvmdebug_frame_menu_detectbk },
		{ L"|PatternASM",L"Run a Pattern On this records",K_NONE,disam_Table_MenuProc,patternmenu,xvm_menu_pattern },
        { L"|Load Records...",L"Load Records From File",K_NONE, disam_Table_MenuProc, NULL, xvmdebug_frame_menu_loadrecords },
		{ L"Save Records...",L"Save Records To File",K_NONE,disam_Table_MenuProc,NULL,xvm_menu_saverecords },
		{ L"Export To Text...",L"Export Recors To Text File",K_NONE,disam_Table_MenuProc,NULL,xvm_menu_exporttext },
		{ L"|Appearance",L"Appearance",K_NONE,disam_Table_MenuProc,appearanceMenu,xvmdebug_frame_menu_app },
        { NULL, NULL, K_NONE, NULL, NULL, 0 }
    };
    m_table_disasm.menu = disam_menu;


	m_table_disasm.bar.name[0] = L"Index";
	m_table_disasm.bar.expl[0] = L"No.";
	m_table_disasm.bar.defdx[0] = 5;
	//m_table_disasm.bar.mode[0] = BAR_DISABLED;

	m_table_disasm.bar.name[1] = L"address";
	m_table_disasm.bar.expl[1] = L"address";
	m_table_disasm.bar.defdx[1] = 10;

	m_table_disasm.bar.name[2] = L"Command";
	m_table_disasm.bar.expl[2] = L"Command";
	m_table_disasm.bar.defdx[2] = 40;

	m_table_disasm.bar.name[3] = L"Comments";
	m_table_disasm.bar.expl[3] = L"Comments";
	m_table_disasm.bar.defdx[3] = 50;

	m_table_disasm.bar.name[4] = L"Context";
	m_table_disasm.bar.expl[4] = L"Context";
	m_table_disasm.bar.defdx[4] = 100;

    memset(&m_block_disasm,0,sizeof(m_block_disasm));
    m_block_disasm.type = BLK_TABLE;
    m_block_disasm.table = &m_table_disasm;

	memset(&m_table_memacc,0,sizeof(m_table_memacc));
	wcscpy(m_table_memacc.name,L"xvm_memacc_");
	wcscat(m_table_memacc.name,name);
	m_table_memacc.mode = TABLE_SYNTAX | TABLE_SAVEALL;
	if (!bShowTitle)
		m_table_memacc.mode |= TABLE_NOBAR;
	m_table_memacc.drawfunc = memacc_draw_func;
	m_table_memacc.tableselfunc = memacc_sel_func;
	m_table_memacc.tabfunc = memacc_func;
	//m_table_memacc.tabfunc = disasm_func;
	m_table_memacc.customdata = this;

	m_table_memacc.bar.nbar = 6;
	m_table_memacc.bar.visible = bShowTitle;

	m_table_memacc.bar.name[0] = L"Index";
	m_table_memacc.bar.expl[0] = L"No.";
	m_table_memacc.bar.defdx[0] = 5;
	//m_table_disasm.bar.mode[0] = BAR_DISABLED;

	m_table_memacc.bar.name[1] = L"instID";
	m_table_memacc.bar.expl[1] = L"instID";
	m_table_memacc.bar.defdx[1] = 5;

	m_table_memacc.bar.name[2] = L"address";
	m_table_memacc.bar.expl[2] = L"address";
	m_table_memacc.bar.defdx[2] = 10;

	m_table_memacc.bar.name[3] = L"size";
	m_table_memacc.bar.expl[3] = L"size";
	m_table_memacc.bar.defdx[3] = 4;

	m_table_memacc.bar.name[4] = L"mode";
	m_table_memacc.bar.expl[4] = L"mode";
	m_table_memacc.bar.defdx[4] = 4;

	m_table_memacc.bar.name[5] = L"value";
	m_table_memacc.bar.expl[5] = L"value";
	m_table_memacc.bar.defdx[5] = 10;

	static t_menu memacc_menu[] = {
		{ L"Show Stack Access",L"Show Stack Access",K_NONE,disam_Table_MenuProc,NULL,xvmdebug_frame_menu_showstackmac },
		{ L"Show Read Access",L"Show Read Access",K_NONE,disam_Table_MenuProc,NULL,xvmdebug_frame_menu_showreadmac },
		{ L"Show Write Access",L"Show Write Access",K_NONE,disam_Table_MenuProc,NULL,xvmdebug_frame_menu_showwritemac },
		{ L"|Appearance",L"Appearance",K_NONE,disam_Table_MenuProc,appearanceMenu,xvmdebug_frame_menu_app },
		{ NULL,NULL,K_NONE,NULL,NULL,0 }
	};
	m_table_memacc.menu = memacc_menu;


	memset(&m_block_memacc,0,sizeof(m_block_memacc));
	m_block_memacc.type = BLK_TABLE;
	m_block_memacc.table = &m_table_memacc;

	memset(&m_block_host,0,sizeof(m_block_host));
	m_block_host.type = BLK_HDIV;
	m_block_host.blk1 = &m_block_disasm;
	m_block_host.blk2 = &m_block_memacc;
	m_block_host.percent = 70;
	m_block_host.minp2 = 10;

	m_frame.mode = TABLE_SYNTAX | TABLE_SAVEALL;
	if (!blockOnly)
	{
		memset(&m_frame,0,sizeof(m_frame));
		m_frame.block = &m_block_host;
		//m_frame.scheme = 1;

		m_hFrame = glpCreateframewindow(&m_frame,0,L"xVMDebug");

	}
    //gLPSetautoupdate(&m_table_disasm,0);
}

void xVMDebugFrame::flushSeq()
{

	if (gxvmcfg.autoRemoveJmp)
	{
		DeobfuscateJxx(&m_seq1,TRUE,TRUE);
	}
    gLPDestroysorteddata(&m_table_disasm.sorted);
    gLPCreatesorteddata(&m_table_disasm.sorted,sizeof(xvm_record),1000,0,0,SDM_EXTADDR);
    int uid=1;
    for (TraceRecordEx* tr = m_seq1.insts.first();tr;tr = m_seq1.insts.next(tr))
    {
        xvm_record xvmr;
        xvmr.addr = uid++;
        xvmr.size = 1;
        xvmr.type = 0;
        xvmr.rc = *tr;
        xvmr.selected = (tr->flags & RECORD_FLAG_JUNK)?TRUE:FALSE;
        gLPAddsorteddata(&m_table_disasm.sorted,&xvmr);
    }
    gLPUpdatetable(&m_table_disasm,0);
	flushMemAccess();
}

void xVMDebugFrame::syncByRip(ULONG_PTR rip)
{
    if (!m_table_disasm.hw)
        return;
    m_activeRip = rip;
    for (int i=0;i<m_table_disasm.sorted.n;i++)
    {
        xvm_record* xrt = (xvm_record*)gLPGetsortedbyindex(&m_table_disasm.sorted,i);
        if (xrt)
        {
            if (xrt->rc.ctx1.rip == rip)
            {
                gLPSettableselection(&m_table_disasm,i);
                break;
            }
        }
    }

}

void xVMDebugFrame::traceSequence(InstSequence* seq)
{
    if (seq->insts.size() < 1) return;
    initRecordStep(seq->insts.first());
    for (TraceRecordEx* tr = seq->insts.first();tr;tr = seq->insts.next(tr))
    {
        setRip(tr->ctx1.rip);
        saveContextToRecord(&tr->ctx1);
        vcpu_return vret = step(tr->ctx1.rip,(const char*)tr->ctx1.opcode,tr->ctx1.szOP);
        saveContextToRecord(&tr->ctx2);
    }
}

void xVMDebugFrame::traceSequence()
{
    traceSequence(&m_seq1);
    flushSeq();
}

void xVMDebugFrame::clearSeqFlag()
{
    for (TraceRecordEx* tr = m_seq1.insts.first();tr;tr = m_seq1.insts.next(tr))
    {
        tr->flags &= ~RECORD_FLAG_JUNK;
    }
    flushSeq();
}

void xVMDebugFrame::deObfuscateJcc(bool onlyjmp)
{
    int nr = DeobfuscateJxx(&m_seq1,FALSE,onlyjmp);
    gLPFlash(L"Detected %d JXX",nr);
    flushSeq();
}
void xVMDebugFrame::DeobfuscateStackOB()
{
    int nr = DeobfuscateStackJunk(&m_seq1,TRUE);
    gLPFlash(L"Detected %d stack junk",nr);
    flushSeq();
}

void xVMDebugFrame::loadRecords(TCHAR *path)
{
    NAutoLargeBufferA nla;
    nla.load(path);
    const char* lpdata = 0;
    int szdata = 0;
    lpdata = nla.data(szdata,0,false);
    if (!lpdata || szdata < 1)
        return;

    m_seq1.insts.clear();
    TraceRecordEx* tr = (TraceRecordEx*)lpdata;
    while(((ULONG_PTR)tr - (ULONG_PTR)lpdata) < szdata)
    {
        m_seq1.insts.push_back(*tr);
        tr++;
    }
    flushSeq();
}
//转换已选指令为代码块
void xVMDebugFrame::makeCodeBlock(t_table *pt)
{
    //创建一个代码块实例，考虑到效率问题，我们用直接创建方式
    //然后将实例交由块执行管理类去管理
    InstSequence* seq = new InstSequence;
    for (int i=0;i<pt->sorted.n;i++)
    {
        xvm_record* tr = (xvm_record*)gLPGetsortedbyindex(&pt->sorted,i);
        if (tr && tr->selected)
        {
            //将所有已选择指令添加到块中
            seq->refc = 0;
            seq->insts.push_back(tr->rc);
        }
    }
    if (seq->insts.size() < 1)
    {   //无效的块
        delete seq;
        return;
    }
    //直接传递给块执行管理类
    VMDebug_BlockAddSeq(seq);
	delete seq;
}

bool xVMDebugFrame::IsEffectESP(TraceRecordEx *tr)
{
    initRecordStep(tr);
    bx_phy_address oldesp = get_reg32(BX_32BIT_REG_ESP);
    vcpu_return iret = step(tr->ctx1.rip,(const char*)tr->ctx1.opcode,tr->ctx1.szOP);
    if (iret == VCPU_LOOPRET_NORMAL)
        return (get_reg32(BX_32BIT_REG_ESP) != oldesp);
    return true;
}

void xVMDebugFrame::setRecordExFlags(int no,long flags)
{
    TraceRecordEx* tr = m_seq1.insts.At(no);
    if (tr)
    {
        tr->flags = flags;
    }
}


ULONG_PTR xVMDebugFrame::activeRip() const
{
    return m_activeRip;
}

void xVMDebugFrame::setActiveRip(const ULONG_PTR &activeRip)
{
    m_activeRip = activeRip;
}
int xVMDebugFrame::lastsel() const
{
    return m_lastsel;
}

void xVMDebugFrame::setLastsel(int lastsel,xvm_record* xr)
{
    m_lastsel = lastsel;
	if (xr)
	{
		syncMemAccByIndex(xr->addr);
	}
}
void xVMDebugFrame::doMemoryAnalyserCurrentSeq()
{
    doMemoryStateAnalyser(&m_seq1);
	gLPFlash(L"Memory state analysered!");
    flushSeq();
}

void xVMDebugFrame::dobCurrentSequenceNormal(bool markonly)
{
    int nr = doDeobfuscateNormal(&m_seq1,markonly);
    deleteFlagedInSeq(&m_seq1);
    gLPFlash(L"Detected %d Junk",nr);
    flushSeq();
}

void xVMDebugFrame::dobCurrentSequencePushPop()
{
    int nr = doDeobfuscateTempPushPop(&m_seq1);
    deleteFlagedInSeq(&m_seq1);
    gLPFlash(L"Detected %d Junk",nr);
    flushSeq();
}

int xVMDebugFrame::doMemoryStateAnalyser(InstSequence *seq)
{
    if (seq->insts.size() < 1)
        return 0;
	ensureChainCPU();	//这里确保ChainCPU的实例化
    m_ccpu->reset();
	int uid = 1;
    for (TraceRecordEx* tr = seq->insts.first();tr;tr = seq->insts.next(tr))
    {	//遍历所有追踪记录
		//注意在我们的ChainCPU当中需要追踪记录的某些信息，尤其是内存访问信息
		instRuntimeInfo ir;
		ir.flags = 0;
		if (tr->flags & RECORD_FLAG_MEMREAD)
			ir.flags |= IRI_MEM_ACCESS_READ;
		if (tr->flags & RECORD_FLAG_MEMWRITE)
			ir.flags |= IRI_MEM_ACCESS_WRITE;
		ir.mraddr = tr->mraddr;
		ir.mrsize = tr->mrsize;
		ir.mwaddr = tr->mwaddr;
		ir.mwsize = tr->mwsize;
		ir.instID = uid++;
		//整理好参数以后就调用step来分析该条指令
        m_ccpu->step(tr->ctx1.rip,(const char*)tr->ctx1.opcode,tr->ctx1.szOP,&ir);
    }
    return 0;
}
//执行一般性的代码简化分析
int xVMDebugFrame::doDeobfuscateNormal(InstSequence *seq, bool flagonly)
{
	//我们一次备用的最大指令反汇编记录，这个数量决定我们花型匹配的基础范围
#define MAX_N_INST  6		
    ud_t udis[MAX_N_INST];
    ud_t udgen;
    TraceRecordEx* trs[MAX_N_INST];	//相应的执行记录
    int nrc = 0;
    for (int i=0;i<MAX_N_INST;i++)
    {
        ud_init(&udis[i]);
        ud_set_mode(&udis[i],32);
    }
    int mark = flagonly?RECORD_FLAG_JUNK:RECORD_FLAG_DELETE;
	//我们通过循环遍历所有的执行记录来开始我们的匹配，这决定了我们的匹配方式是至上而下的
    for (TraceRecordEx* tr1 = seq->insts.first();tr1;tr1 = seq->insts.next(tr1))
    {
        TraceRecordEx* tr = tr1;
        BOOL reLoop = FALSE;
        int nhas = 0;	//这里我们反汇编好一定数量的指令
        for (nhas = 0;nhas < MAX_N_INST;nhas++)
        {
            if (!tr) break;
            if (tr->flags & mark)
            {
                reLoop = TRUE;
                break;
            }
            ud_set_input_buffer(&udis[nhas],(const uint8_t*)tr->ctx1.opcode,tr->ctx1.szOP);
            ud_set_pc(&udis[nhas],tr->ctx1.rip);
            int instlen = ud_decode(&udis[nhas]);
            if (instlen < 1) break;
            trs[nhas] = tr;
            tr = seq->insts.next(tr);
        }
        if (reLoop) continue;
        if (nhas < 2) continue;
        int state = 0;	//取出第一条与第二条指令的操作数
        const ud_operand* op01 = ud_insn_opr(&udis[0],0);
        const ud_operand* op02 = ud_insn_opr(&udis[0],1);
        const ud_operand* op11 = ud_insn_opr(&udis[1],0);
        const ud_operand* op12 = ud_insn_opr(&udis[1],1);
        switch(udis[0].mnemonic)	//从第一条指令的指令类型开始我们的匹配
        {
        case UD_Ibswap:
        {
            if (udis[1].mnemonic == UD_Ibswap)
            {
                if (NASM_I386::IsSameOperand(op01,op11))
                {
                    trs[0]->flags |= mark;
                    trs[1]->flags |= mark;
                    state = 1;
                }
            }
        }break;
		case UD_Irdtsc:
		{
			trs[0]->flags |= mark;
			state = 1;
		}break;
        case UD_Ixchg:
        {
            if (NASM_I386::IsSameOperand(op01,op02))
            {
                trs[0]->flags |= mark;
            }else if (udis[1].mnemonic == UD_Ixchg)
            {
                if (NASM_I386::IsSameOperand(op01,op11) || NASM_I386::IsSameOperand(op01,op12))
                {
                    if (NASM_I386::IsSameOperand(op01,op12))
                        std::swap(op11,op12);
                    if (NASM_I386::IsSameOperand(op02,op12))
                    {
                        trs[0]->flags |= mark;
                        trs[1]->flags |= mark;
                        state = 1;
                    }
                }
                if (state <= 0)
                {
                    if ( nhas >= 3 && udis[2].mnemonic == UD_Ixchg)
                    {//xchg<>xchg<>xchg
                        const ud_operand* op21 = ud_insn_opr(&udis[2],0);
                        const ud_operand* op22 = ud_insn_opr(&udis[2],1);
                        if (NASM_I386::IsSameOperand(op01,op21) || NASM_I386::IsSameOperand(op01,op22)
                                || NASM_I386::IsSameOperand(op02,op21) || NASM_I386::IsSameOperand(op02,op22))
                        {
                            if (NASM_I386::IsSameOperand(op02,op21) || NASM_I386::IsSameOperand(op02,op22))
                                std::swap(op01,op02);
                            if (NASM_I386::IsSameOperand(op01,op22))
                                std::swap(op21,op22);
                            if (NASM_I386::IsSameOperand(op22,op11) || NASM_I386::IsSameOperand(op22,op12))
                            {
                                if (NASM_I386::IsSameOperand(op22,op12))
                                    std::swap(op11,op12);
                                if (NASM_I386::IsSameOperand(op12,op02))
                                {
                                    trs[1]->flags |= mark;
                                    trs[2]->flags |= mark;
                                    m_asm.udex_init(&udgen,UD_Ixchg,trs[0]->ctx1.rip);
                                    udgen.operand[0] = *op11;
                                    udgen.operand[1] = *op12;
                                    trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[0]->ctx1.opcode,sizeof(trs[0]->ctx1.opcode));
                                    state = 1;
                                }

                            }
                        }
                    }
                }
            }else if (nhas >= 3 && udis[2].mnemonic == UD_Ixchg)
            {//xchg<>?<>xchg
                const ud_operand* op21 = ud_insn_opr(&udis[2],0);
                const ud_operand* op22 = ud_insn_opr(&udis[2],1);
                if (NASM_I386::IsSameOperand(op01,op21) || NASM_I386::IsSameOperand(op01,op22))
                {
                    if (NASM_I386::IsSameOperand(op01,op22))
                        std::swap(op21,op22);
                    if (NASM_I386::IsSameOperand(op02,op22))
                    {
                        if (NASM_I386::IsSameOperand(op11,op01) || NASM_I386::IsSameOperand(op11,op02))
                        {
                            if (NASM_I386::IsSameOperand(op11,op01))
                                std::swap(op01,op02);
                            switch(udis[1].mnemonic)
                            {
                            case UD_Inot:case UD_Idec:case UD_Iinc:case UD_Ineg:case UD_Ibswap:
                            {
                                udgen = udis[1];
                                ud_set_pc(&udgen,trs[0]->ctx1.rip);
                                udgen.operand[0] = *op01;
                                trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[0]->ctx1.opcode,sizeof(trs[0]->ctx1.opcode));
                                trs[1]->flags |= mark;
                                trs[2]->flags |= mark;
                                state = 1;
                            }break;
                            }
                        }

                    }
                }
            }
        }break;
        case UD_Ineg:
        {
            if (udis[1].mnemonic == UD_Ineg)
            {
                if (NASM_I386::IsSameOperand(op01,op11))
                {
                    trs[0]->flags |= mark;
                    trs[1]->flags |= mark;
                    state = 1;
                }
            }
        }break;
        case UD_Inot:
        {
            if (udis[1].mnemonic == UD_Inot)
            {
                if (NASM_I386::IsSameOperand(op01,op11))
                {
                    trs[0]->flags |= mark;
                    trs[1]->flags |= mark;
                    state = 1;
                }
            }
        }break;
		case UD_Ipush:
		{
			switch (udis[1].mnemonic)
			{
			case UD_Imov:
			{
				if (NASM_I386::IsRegister(op01,NASM_I386::udex_get_rel_reg(UD_R_RSP,op01->size))
					&& NASM_I386::IsSameOperand(op01,op11)
					&& NASM_I386::IsEspMemOperand(op12))
				{
					trs[0]->flags |= mark;
					trs[1]->flags |= mark;
					state = 1;
				} else if ((op11->type == UD_OP_MEM)//判断第二条即mov指令的寄存器是否为内存寻址操作数
					&& udis[1].opr_mode == op11->size	//判断内存寻址大小是否与push指令大小相同
					&& (op11->base == UD_R_ESP && op11->index == UD_NONE && op11->scale == 0))//判断是否为[esp]内存寻址
				{   //push xxx,mov [esp],xxx
					m_asm.udex_init(&udgen,UD_Ipush,trs[0]->ctx1.rip);
					udgen.operand[0] = *op12;
					trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[0]->ctx1.opcode,sizeof(trs[0]->ctx1.opcode));
					if (NASM_I386::IsRegister(op12,UD_R_RSP,true))
					{
						m_asm.udex_init(&udgen,UD_Isub,trs[1]->ctx1.rip);
						m_asm.udex_set_op(&udgen,0,UD_OP_MEM,0,op11->size,UD_R_RSP);
						m_asm.udex_set_op(&udgen,1,UD_OP_IMM,4,op11->size);
						trs[1]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[1]->ctx1.opcode,sizeof(trs[1]->ctx1.opcode));
					} else
						trs[1]->flags |= mark;
					state = 1;
				} else if (nhas >= 3)
                {
                    const ud_operand* op21 = ud_insn_opr(&udis[2],0);
                    const ud_operand* op22 = ud_insn_opr(&udis[2],1);
                    const ud_operand* op31 = ud_insn_opr(&udis[3],0);
                    const ud_operand* op32 = ud_insn_opr(&udis[3],1);
                    if (op11->type == UD_OP_REG && NASM_I386::IsSameOperand(op01,op11) && op12->type == UD_OP_REG && NASM_I386::udex_get_rel_reg(op12->base,64) == UD_R_RSP)
                    {//push reg1|mov reg1,esp
                        ud_type resp = NASM_I386::udex_get_rel_reg(UD_R_RSP,op01->size);
                        if (udis[2].mnemonic == UD_Ixchg
                                && NASM_I386::IsEspMemOperand(op21)
                                && NASM_I386::IsSameOperand(op01,op22)
                                )
                        {   //xchg [esp],reg1

                            m_asm.udex_init(&udgen,UD_Ipush,trs[0]->ctx1.rip);
                            m_asm.udex_set_op(&udgen,0,UD_OP_REG,0,0,resp);
                            trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[0]->ctx1.opcode,sizeof(trs[0]->ctx1.opcode));
                            m_asm.udex_init(&udgen,UD_Isub,trs[1]->ctx1.rip);
                            m_asm.udex_set_op(&udgen,0,UD_OP_MEM,0,op11->size,resp);
                            m_asm.udex_set_op(&udgen,1,UD_OP_IMM,4,op11->size);
                            trs[1]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[1]->ctx1.opcode,sizeof(trs[1]->ctx1.opcode));
                            trs[2]->flags |= mark;
                            state = 1;
                        }else if (nhas >= 5 && udis[2].mnemonic == UD_Iadd
                                  && NASM_I386::IsSameOperand(op01,op21) && op22->type == UD_OP_IMM
                                  && udis[3].mnemonic == UD_Ixchg
                                  && NASM_I386::IsEspMemOperand(op31)
                                  && NASM_I386::IsSameOperand(op01,op32)
                                  )
                        {   //add reg1,imm|xchg [esp],reg1| (pop esp or mov esp,[esp])
                            if ((udis[4].mnemonic == UD_Ipop && NASM_I386::IsRegister(ud_insn_opr(&udis[4],0),resp))
                                    || ((udis[4].mnemonic == UD_Imov)
                                        && NASM_I386::IsRegister(ud_insn_opr(&udis[4],0),resp)
                                        && NASM_I386::IsEspMemOperand(ud_insn_opr(&udis[4],1))))
                            {
                                m_asm.udex_init(&udgen,UD_Iadd,trs[0]->ctx1.rip);
                                m_asm.udex_set_op(&udgen,0,UD_OP_REG,0,0,resp);
                                m_asm.udex_set_op(&udgen,1,UD_OP_IMM,op22->lval.sdword-4,op11->size);
                                trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[0]->ctx1.opcode,sizeof(trs[0]->ctx1.opcode));
                                trs[1]->flags |= mark;
                                trs[2]->flags |= mark;
                                trs[3]->flags |= mark;
                                trs[4]->flags |= mark;
                                state = 1;
                            }
                        }
                    }else if (op11->type == UD_OP_REG && NASM_I386::IsSameOperand(op01,op11) && NASM_I386::IsEspMemOperand(op12,4))
                    {   //push reg1|mov reg1,[esp+4]
                        const ud_operand* op21 = ud_insn_opr(&udis[2],0);
                        if (udis[2].mnemonic == UD_Ipop && NASM_I386::IsEspMemOperand(op21) && op21->size == op01->size)
                        {//pop [esp]
                            m_asm.udex_init(&udgen,UD_Ixchg,trs[0]->ctx1.rip);
                            m_asm.udex_set_op(&udgen,0,UD_OP_MEM,0,0,UD_R_ESP);
                            m_asm.udex_set_op(&udgen,1,UD_OP_REG,0,0,op01->base);
                            trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[0]->ctx1.opcode,sizeof(trs[0]->ctx1.opcode));
                            trs[1]->flags |= mark;
                            trs[2]->flags |= mark;
                            state = 1;
                        }
                    }else if (NASM_I386::IsEspMemOperand(op01)
                              && NASM_I386::IsEspMemOperand(op12)
                              && udis[2].mnemonic == UD_Iadd
                              && NASM_I386::IsRegister(op21,UD_R_RSP,true)
                              && op22->type == UD_OP_IMM && op22->lval.sqword == 8)
                    {   //(push [esp])(mov op1,[esp])(add esp,8)->pop op1
                        m_asm.udex_init(&udgen,UD_Ipop,trs[0]->ctx1.rip);
                        udgen.operand[0] = *op11;
                        trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[0]->ctx1.opcode,sizeof(trs[0]->ctx1.opcode));
                        trs[1]->flags |= mark;
                        trs[2]->flags |= mark;
                        state = 1;
                    }else if (NASM_I386::IsNoneEspRelateOP(op01)
                              && NASM_I386::IsNoneEspRelateOP(op11)
                              && NASM_I386::IsSameOperand(op01,op11)
                              && NASM_I386::IsNoneEspRelateOP(op12)
                              && udis[2].mnemonic == UD_Ipop
                              && NASM_I386::IsSameOperand(op12,op21))
                    {   //(push op1)(mov op1,op2)(pop op2)->xchg op1,op2 ->none vsp relate
                        m_asm.udex_init(&udgen,UD_Ixchg,trs[0]->ctx1.rip);
                        udgen.operand[0] = *op01;
                        udgen.operand[1] = *op21;
                        trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[0]->ctx1.opcode,sizeof(trs[0]->ctx1.opcode));
                        trs[1]->flags |= mark;
                        trs[2]->flags |= mark;
                        state = 1;
                    }

                    if (state <= 0 && nhas >= 4)
                    {
                        if (udis[2].mnemonic == UD_Imov
                                && udis[3].mnemonic == UD_Ipop
                                )
                        {//(push op1)(mov op1,op2)
							if (NASM_I386::IsNoneEspRelateOP(op01)
								&& NASM_I386::IsNoneEspRelateOP(op11)
								&& NASM_I386::IsNoneEspRelateOP(op12)
								&& NASM_I386::IsNoneEspRelateOP(op21)
								&& NASM_I386::IsNoneEspRelateOP(op22)
								&& NASM_I386::IsNoneEspRelateOP(op31))
							{//(push op1)(mov op1,op2)(mov ??)(pop op1)->mov op3,op2
								if (NASM_I386::IsSameOperand(op01, op11)
									&& NASM_I386::IsSameOperand(op01, op31))
								{//(push op1)(mov op1,op2)(mov op3,op1)(pop op1)->mov op3,op2
									if (NASM_I386::IsSameOperand(op01, op22))
									{
										m_asm.udex_init(&udgen, UD_Imov, trs[0]->ctx1.rip);
										udgen.operand[0] = *op21;
										udgen.operand[1] = *op12;
										trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen, (char*)trs[0]->ctx1.opcode, sizeof(trs[0]->ctx1.opcode));
										trs[1]->flags |= mark;
										trs[2]->flags |= mark;
										trs[3]->flags |= mark;
										state = 1;
									}
									else if (op21 && op21->type == UD_OP_MEM && op21->index == 0 && op21->scale == 0 && op21->offset == 0
										&& op21->base == op01->base && op01->type == UD_OP_REG)
									{//(push op1)(mov op1,op2)(mov [op1],op3)(pop op1)->mov [op2],op3
										m_asm.udex_init(&udgen, UD_Imov, trs[0]->ctx1.rip);

										m_asm.udex_set_op(&udgen, 0, UD_OP_MEM, 0, 0, op12->base);
										udgen.operand[1] = *op22;
										trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen, (char*)trs[0]->ctx1.opcode, sizeof(trs[0]->ctx1.opcode));
										trs[1]->flags |= mark;
										trs[2]->flags |= mark;
										trs[3]->flags |= mark;
										state = 1;
									}
									else if (op22 && op22->type == UD_OP_MEM && op22->index == 0 && op22->scale == 0 && op22->offset == 0
										&& op22->base == op01->base && op01->type == UD_OP_REG)
									{//(push op1)(mov op1,op2)(mov op3,[op1])(pop op1)->mov op3,[op2]
										m_asm.udex_init(&udgen, UD_Imov, trs[0]->ctx1.rip);
										udgen.operand[0] = *op21;
										m_asm.udex_set_op(&udgen, 1, UD_OP_MEM, 0, 0, op12->base);

										trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen, (char*)trs[0]->ctx1.opcode, sizeof(trs[0]->ctx1.opcode));
										trs[1]->flags |= mark;
										trs[2]->flags |= mark;
										trs[3]->flags |= mark;
										state = 1;
									}
								}


							}

						}

                    }

					if (state <= 0 && nhas >= 5)
					{
						const ud_operand* op41 = ud_insn_opr(&udis[4], 0);
						const ud_operand* op42 = ud_insn_opr(&udis[4], 1);
						if (udis[2].mnemonic == UD_Isub && udis[4].mnemonic == UD_Ipop
							&& op12->type == UD_OP_IMM && op12->lval.sqword == 0
							&& NASM_I386::IsSameOperand(op01,op11)
							&& NASM_I386::IsSameOperand(op01,op21)
							&& NASM_I386::IsSameOperand(op01,op41))
						{

							if (udis[3].mnemonic == UD_Ixchg)
							{
								if (NASM_I386::IsSameOperand(op21, op31) || NASM_I386::IsSameOperand(op21, op32))
								{
									if (NASM_I386::IsSameOperand(op21, op32))
										std::swap(op31, op32);
									if (NASM_I386::IsSameOperand(op22, op32))
									{//(push op1)(mov op1,0)(sub op1,op2)(xchg op1,op2)(pop op1)->neg op2
										m_asm.udex_init(&udgen, UD_Ineg, trs[0]->ctx1.rip);
										udgen.operand[0] = *op22;
										trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen, (char*)trs[0]->ctx1.opcode, sizeof(trs[0]->ctx1.opcode));
										trs[1]->flags |= mark;
										trs[2]->flags |= mark;
										trs[3]->flags |= mark;
										trs[4]->flags |= mark;
										state = 1;
									}
								}
							}else if (udis[3].mnemonic == UD_Imov)
							{
								if (NASM_I386::IsSameOperand(op21, op32) && NASM_I386::IsSameOperand(op22, op31))
								{//(push op1)(mov op1,0)(sub op1,op2)(xchg op1,op2)(pop op1)->neg op2
									m_asm.udex_init(&udgen, UD_Ineg, trs[0]->ctx1.rip);
									udgen.operand[0] = *op22;
									trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen, (char*)trs[0]->ctx1.opcode, sizeof(trs[0]->ctx1.opcode));
									trs[1]->flags |= mark;
									trs[2]->flags |= mark;
									trs[3]->flags |= mark;
									trs[4]->flags |= mark;
									state = 1;
								}
							}

						}
					}

                }
            }break;
            case UD_Ipop:
            {
                if (!((op01->type == UD_OP_MEM) && (op11->type == UD_OP_MEM)))
                {   //push xx1x,pop xxx2->mov xx2,xx1
                    //trs[1]->ctx1.opcode
                    if (NASM_I386::IsSameOperand(op01,op11))
                    {
                        trs[0]->flags |= mark;
                        trs[1]->flags |= mark;
                        state = 1;
                    }else
                    {
                        m_asm.udex_init(&udgen,UD_Imov,trs[0]->ctx1.rip);
                        udgen.operand[0] = *op11;
                        udgen.operand[1] = *op01;
                        trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[0]->ctx1.opcode,sizeof(trs[0]->ctx1.opcode));
                        trs[1]->flags |= mark;
                        state = 1;
                    }

                }
            }break;
            case UD_Iadd:
            {
                if (nhas >= 3)
                {
                    const ud_operand* op21 = ud_insn_opr(&udis[2],0);
                    //const ud_operand* op22 = ud_insn_opr(&udis[2],1);
                    if (NASM_I386::IsRegister(op01,NASM_I386::udex_get_rel_reg(UD_R_RSP,op01->size)))
                    {//<push esp> <add x>

                        if (NASM_I386::IsEspMemOperand(op11,4)
                                && udis[2].mnemonic == UD_Ipop && NASM_I386::IsEspMemOperand(ud_insn_opr(&udis[2],0)))
                        {//<push esp><add [esp+imm],xxx><pop [esp]>--> mov [esp],esp
                            m_asm.udex_init(&udgen,UD_Imov,trs[0]->ctx1.rip);
                            m_asm.udex_set_op(&udgen,0,UD_OP_MEM,0,op01->size,UD_R_RSP);
                            m_asm.udex_set_op(&udgen,1,UD_OP_REG,0,op01->size,UD_R_RSP);
                            trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[0]->ctx1.opcode,sizeof(trs[0]->ctx1.opcode));
                            trs[1]->flags |= mark;
                            trs[2]->flags |= mark;
                            state = 1;
                        }else if ( NASM_I386::IsEspMemOperand(op11)
                                   && (op12->type == UD_OP_IMM)
                                   && udis[2].mnemonic == UD_Ipop
                                   && op21->type == UD_OP_REG)
                        {//<push esp><add [esp],imm1><pop reg1>---->lea reg1,[esp+imm1]
                            m_asm.udex_init(&udgen,UD_Ilea,trs[0]->ctx1.rip);
                            udgen.operand[0] = *op21;
                            m_asm.udex_set_op(&udgen,1,UD_OP_MEM,op12->lval.uqword,op01->size,UD_R_RSP);
                            trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[0]->ctx1.opcode,sizeof(trs[0]->ctx1.opcode));
                            trs[1]->flags |= mark;
                            trs[2]->flags |= mark;
                            state = 1;
                        }
                    }else if (op11 && op12 && !ud_insn_opr(&udis[1],2) && NASM_I386::IsEspMemOperand(op11) && udis[2].mnemonic == UD_Ipop)
                    {
                        m_asm.udex_init(&udgen,UD_Imov,trs[0]->ctx1.rip);
                        udgen.operand[0] = *op21;
                        udgen.operand[1] = *op01;
                        trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[0]->ctx1.opcode,sizeof(trs[0]->ctx1.opcode));
                        udgen = udis[1];
                        udgen.operand[0] = *op21;
                        trs[1]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[1]->ctx1.opcode,sizeof(trs[1]->ctx1.opcode));
                        trs[2]->flags |= mark;
                        state = 1;
                    }
                }
            }break;
            case UD_Ilea:
            {
                if (nhas >= 3)
                {
                    const ud_operand* op21 = ud_insn_opr(&udis[2],0);
                    const ud_operand* op22 = ud_insn_opr(&udis[2],1);
                    const ud_operand* op31 = ud_insn_opr(&udis[3],0);
                    const ud_operand* op32 = ud_insn_opr(&udis[3],1);
                    if (op01->type == UD_OP_REG && NASM_I386::IsSameOperand(op01,op11))
                    {//<push reg><lea reg,?>

                        if (udis[2].mnemonic == UD_Ixchg
                                && NASM_I386::IsEspMemOperand(op21)
                                && NASM_I386::IsSameOperand(op01,op22))
                        {   //xchg [esp],reg
                            if (NASM_I386::IsEspMemOperand(op12,4))
                            {//<push reg><lea reg,[esp+4]>
                                m_asm.udex_init(&udgen,UD_Ipush,trs[0]->ctx1.rip);
                                m_asm.udex_set_op(&udgen,0,UD_OP_REG,0,op01->size,UD_R_RSP);
                                trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[0]->ctx1.opcode,sizeof(trs[0]->ctx1.opcode));
                                trs[1]->flags |= mark;
                                trs[2]->flags |= mark;
                                state = 1;
                            }else if (nhas >= 4 && NASM_I386::IsEspMemOperand(op12,0,true)
                                      && udis[3].mnemonic == UD_Imov
                                      && NASM_I386::IsRegister(op31,UD_R_ESP)
                                      && NASM_I386::IsEspMemOperand(op32))
                            {//mov esp,[esp]
                                m_asm.udex_init(&udgen,UD_Ilea,trs[0]->ctx1.rip);
                                m_asm.udex_set_op(&udgen,0,UD_OP_REG,0,op01->size,UD_R_RSP);
                                m_asm.udex_set_op(&udgen,1,UD_OP_MEM,op12->lval.sqword-4,op01->size,UD_R_RSP);
                                trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[0]->ctx1.opcode,sizeof(trs[0]->ctx1.opcode));
                                trs[1]->flags |= mark;
                                trs[2]->flags |= mark;
                                trs[3]->flags |= mark;
                                state = 1;
                            }
                        }

                    }
                }
            }break;
            case UD_Isub:
            {
                if (nhas >= 3)
                {   //(push
                    const ud_operand* op21 = ud_insn_opr(&udis[2],0);
                    const ud_operand* op22 = ud_insn_opr(&udis[2],1);
                    if (op01->type == UD_OP_IMM && op01->lval.uqword == 0
                            && NASM_I386::IsEspMemOperand(op11)
                            && udis[2].mnemonic == UD_Ipop
                            && NASM_I386::IsSameOperand(op12,op21))
                    {   //(push 0)(sub [esp],op)(pop op)->neg op
                        m_asm.udex_init(&udgen,UD_Ineg,trs[0]->ctx1.rip);
                        udgen.operand[0] = *op12;
                        trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[0]->ctx1.opcode,sizeof(trs[0]->ctx1.opcode));
                        trs[1]->flags |= mark;
                        trs[2]->flags |= mark;
                        state = 1;
                    }else if (NASM_I386::IsRegister(op01,UD_R_RSP,true)
                              && NASM_I386::IsEspMemOperand(op11)
                              && op12->type == UD_OP_IMM)
                    {   //(push esp)(sub [esp],imm)
                        if ((udis[2].mnemonic == UD_Imov
                             && NASM_I386::IsRegister(op21,UD_R_RSP,true)
                             && NASM_I386::IsEspMemOperand(op22))
                                || (udis[2].mnemonic == UD_Ipop && NASM_I386::IsRegister(op21,UD_R_RSP,true)))
                        {//(mov esp,[esp] / pop esp)->sub esp,imm-4
                            m_asm.udex_init(&udgen,UD_Isub,trs[0]->ctx1.rip);
                            m_asm.udex_set_op(&udgen,0,UD_OP_REG,0,0,UD_R_RSP);
                            m_asm.udex_set_op(&udgen,1,UD_OP_IMM,NASM_I386::udex_get_opimm(op12),op12->size);
                            trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[0]->ctx1.opcode,sizeof(trs[0]->ctx1.opcode));
                            trs[1]->flags |= mark;
                            trs[2]->flags |= mark;
                        }
                    }
                }
            }break;
            case UD_Iret:
            {
                if (op01->type == UD_OP_IMM && !op11
                        && udis[1].opr_mode == udis[0].opr_mode)
                {   //push imm,ret->jmp
                    BYTE* lpop = trs[0]->ctx1.opcode;
                    lpop[0] = 0xE9;
                    *(ULONG_PTR*)(lpop+1) = NASM_I386::udex_get_opimm(op01)-trs[0]->ctx1.rip-5;
                    trs[0]->ctx1.szOP = 5;
                    trs[1]->flags |= mark;
                    state = 1;
                }
            }break;
            }
            if (state <= 0)
            {
                if (op11 && op12 && (NASM_I386::IsEspMemOperand(op11,4)
                                     && (NASM_I386::udex_get_rel_reg(op12->base,64) != UD_R_RSP)
                                     && (NASM_I386::udex_get_rel_reg(op12->index,64) != UD_R_RSP)))
                {
                    memcpy(trs[1]->ctx1.opcode,trs[0]->ctx1.opcode,trs[0]->ctx1.szOP);
                    trs[1]->ctx1.szOP = trs[0]->ctx1.szOP;
                    udgen = udis[1];
                    udgen.operand[0].lval.uqword = 0;
                    trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[0]->ctx1.opcode,sizeof(trs[0]->ctx1.opcode));
                    state = 1;
                }
            }
            if (state <= 0)
            {
                if (nhas >= 3)
                {
                    switch(udis[2].mnemonic)
                    {
                    case UD_Ipop:
                    {   //<>push op1<>??<>pop op2<>
                        const ud_operand* op21 = ud_insn_opr(&udis[2],0);
                        if (NASM_I386::IsSameOperand(op01,op21))
                        {
                            if (op11 && op12 && !ud_insn_opr(&udis[1],2))
                            {
                                if (NASM_I386::IsEspMemOperand(op11))
                                {
                                    m_asm.udex_init(&udgen,udis[1].mnemonic,trs[0]->ctx1.rip);
                                    udgen.operand[0] = *op01;
                                    udgen.operand[1] = *op12;
                                    trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[0]->ctx1.opcode,sizeof(trs[0]->ctx1.opcode));
                                    trs[1]->flags |= mark;
                                    trs[2]->flags |= mark;
                                    state = 1;
                                }else if (NASM_I386::IsEspMemOperand(op12))
                                {
                                    m_asm.udex_init(&udgen,udis[1].mnemonic,trs[0]->ctx1.rip);
                                    udgen.operand[0] = *op11;
                                    udgen.operand[1] = *op01;
                                    trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[0]->ctx1.opcode,sizeof(trs[0]->ctx1.opcode));
                                    trs[1]->flags |= mark;
                                    trs[2]->flags |= mark;
                                    state = 1;
                                }
                            }else if (op11 && !op12)
                            {
                                if (NASM_I386::IsEspMemOperand(op11))
                                {
                                    m_asm.udex_init(&udgen,udis[1].mnemonic,trs[0]->ctx1.rip);
                                    udgen.operand[0] = *op01;
                                    trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[0]->ctx1.opcode,sizeof(trs[0]->ctx1.opcode));
                                    trs[1]->flags |= mark;
                                    trs[2]->flags |= mark;
                                    state = 1;
                                }
                            }
                        }
                        if (state <= 0)
                        {
                            if (op11 && op12 && NASM_I386::IsEspMemOperand(op11)
                                    && NASM_I386::udex_get_rel_reg(op12->base,64) != UD_R_RSP
                                    && NASM_I386::udex_get_rel_reg(op12->index,64) != UD_R_RSP
                                    && !IsEffectESP(trs[1]))
                            { //(push op1)(?? [esp],op3)(pop op2)->(mov op2,op1)(?? op2,op3)
                                m_asm.udex_init(&udgen,UD_Imov,trs[0]->ctx1.rip);
                                udgen.operand[0] = *op21;
                                udgen.operand[1] = *op01;
                                trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[0]->ctx1.opcode,sizeof(trs[0]->ctx1.opcode));
                                m_asm.udex_init(&udgen,udis[1].mnemonic,trs[1]->ctx1.rip);
                                udgen.operand[0] = *op21;
                                udgen.operand[1] = *op12;
                                trs[1]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[1]->ctx1.opcode,sizeof(trs[1]->ctx1.opcode));
                                trs[2]->flags |= mark;
                                state = 1;
                            }
                        }
                    }break;
                    }
                }
            }
        }break;
        case UD_Iadd:
        {
            if (op02->type == UD_OP_IMM && op02->lval.uqword == 0)
            {
                trs[0]->flags |= mark;
                state = 1;
            }else if (NASM_I386::IsRegister(op01,UD_R_RSP,true)
                      && op02->type == UD_OP_IMM && NASM_I386::udex_get_opimm(op02) == 4
                      && udis[1].mnemonic == UD_Imov
                      && udis[0].opr_mode == udis[1].opr_mode
                      && NASM_I386::IsEspMemOperand(op12,-4))
            {   //add esp,4)mov op1,[esp-4]->pop op1
                m_asm.udex_init(&udgen,UD_Ipop,trs[0]->ctx1.rip);
                udgen.operand[0] = *op11;
                trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[0]->ctx1.opcode,sizeof(trs[0]->ctx1.opcode));
                trs[1]->flags |= mark;
            }
            else if (nhas >= 3)
            {
                const ud_operand* op21 = ud_insn_opr(&udis[2],0);
                const ud_operand* op22 = ud_insn_opr(&udis[2],1);
                if (op11 && op12 && op21 && op22 && NASM_I386::IsSameOperand(op01,op11) && NASM_I386::IsSameOperand(op01,op21)
                        && udis[2].mnemonic == UD_Isub
                        && NASM_I386::IsSameOperand(op02,op22)
                        )
                {
                    switch(udis[1].mnemonic)
                    {
                    case UD_Isub: case UD_Iadd:
                    {
                        trs[0]->flags |= mark;
                        trs[2]->flags |= mark;
                        state = 1;
                    }break;
                    }
                }
            }
        }break;
        case UD_Isub:
        {
            if (op02->type == UD_OP_IMM && op02->lval.uqword == 0)
            {
                trs[0]->flags |= mark;
                state = 1;
            }else
            {
                switch(udis[1].mnemonic)
                {
                case UD_Imov:
                {
                    if (op01->type == UD_OP_REG
                            && op01->base == UD_R_ESP
                            && (op02->type == UD_OP_CONST || op02->type == UD_OP_IMM)
                            && op02->lval.uword == 4 &&
                            op11->type == UD_OP_MEM
                            && op11->base == UD_R_ESP && op11->index == 0 && op11->scale == 0)
                    {   //sub esp,4 mov [esp],xxx
                        m_asm.udex_init(&udgen,UD_Ipush,trs[0]->ctx1.rip);
                        udgen.operand[0] = *op12;
                        trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[0]->ctx1.opcode,sizeof(trs[0]->ctx1.opcode));
                        if (NASM_I386::IsRegister(op12,UD_R_RSP,true))
                        {
                            m_asm.udex_init(&udgen,UD_Isub,trs[1]->ctx1.rip);
                            m_asm.udex_set_op(&udgen,0,UD_OP_MEM,0,op11->size,UD_R_RSP);
                            m_asm.udex_set_op(&udgen,1,UD_OP_IMM,4,op11->size);
                            trs[1]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[1]->ctx1.opcode,sizeof(trs[1]->ctx1.opcode));
                        }else
                            trs[1]->flags |= mark;
                        state = 1;
                    }
                }break;
                }
                if (state <= 0 && nhas >= 3)
                {
                    const ud_operand* op21 = ud_insn_opr(&udis[2],0);
                    const ud_operand* op22 = ud_insn_opr(&udis[2],1);
                    if (op11 && op21 && op22 && NASM_I386::IsSameOperand(op01,op11) && NASM_I386::IsSameOperand(op01,op21)
                            && udis[2].mnemonic == UD_Iadd
                            && NASM_I386::IsSameOperand(op02,op22)
                            )
                    {
                        switch(udis[1].mnemonic)
                        {
                        case UD_Isub: case UD_Iadd:
                        {
                            trs[0]->flags |= mark;
                            trs[2]->flags |= mark;
                            state = 1;
                        }break;
                        }
                    }
                }
            }
        }break;
        case UD_Imov:
        {
            switch(udis[1].mnemonic)
            {
            case UD_Iadd:
            {
							if (NASM_I386::IsSameOperand(op01, op11)
								&& NASM_I386::IsImmOperand(op02)
								&& NASM_I386::IsImmOperand(op12))
							{//(mov op1,imm1)(add op1,imm2)->(mov op1<-imm1 + imm2)
								m_asm.udex_init(&udgen, UD_Imov, trs[0]->ctx1.rip);
								udgen.operand[0] = *op01;
								m_asm.udex_set_op(&udgen, 1, UD_OP_IMM, NASM_I386::udex_get_opimm(op02) + NASM_I386::udex_get_opimm(op12));
								trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen, (char*)trs[0]->ctx1.opcode, sizeof(trs[0]->ctx1.opcode));
								trs[1]->flags |= mark;
								state = 1;
							}else if ((op02->type == UD_OP_MEM && op02->base == UD_R_ESP && op02->index == 0 && op02->scale == 0 && op02->lval.uqword == 0)
                        && (op11->type == UD_OP_REG && op11->base == UD_R_ESP && op12->type == UD_OP_IMM && op12->lval.uqword == 4))
                {   //mov xxx,[esp] add esp,4 ->pop xxx
                    m_asm.udex_init(&udgen,UD_Ipop,trs[0]->ctx1.rip);
                    udgen.operand[0] = *op01;
                    trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[0]->ctx1.opcode,sizeof(trs[0]->ctx1.opcode));
                    trs[1]->flags |= mark;
                    state = 1;
							}
							else if (NASM_I386::IsImmOperand(op02) && NASM_I386::udex_get_opimm(op02) == 0
								&& NASM_I386::IsSameOperand(op01,op11))
							{
								m_asm.udex_init(&udgen, UD_Imov, trs[0]->ctx1.rip);
								udgen.operand[0] = *op01;
								udgen.operand[1] = *op12;
								trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen, (char*)trs[0]->ctx1.opcode, sizeof(trs[0]->ctx1.opcode));
								trs[1]->flags |= mark;
								state = 1;
							}
            }break;
            case UD_Imov:
            {   //mov p1,xxx1 & mov p1,xxx2 ->mov p1,xxx2
                if (NASM_I386::IsSameOperand(op01,op11))
                {
                    trs[0]->flags |= mark;
                    state = 1;
                }
            }break;
			case UD_Ixor:
			{	//(mov op1,imm1)(xor op1,imm2)->(mov op1<-imm1 xor imm2)
							if (NASM_I386::IsSameOperand(op01, op11)
								&& NASM_I386::IsImmOperand(op02)
								&& NASM_I386::IsImmOperand(op12))
							{
								m_asm.udex_init(&udgen, UD_Imov, trs[0]->ctx1.rip);
								udgen.operand[0] = *op01;
								m_asm.udex_set_op(&udgen, 1, UD_OP_IMM, NASM_I386::udex_get_opimm(op02) ^ NASM_I386::udex_get_opimm(op12));
								trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen, (char*)trs[0]->ctx1.opcode, sizeof(trs[0]->ctx1.opcode));
								trs[1]->flags |= mark;
								state = 1;
							}

			}break;
			case UD_Isub:
			{//(mov op1,imm1)(sub op1,imm2)->(mov op1<-imm1 sub imm2)
							if (NASM_I386::IsSameOperand(op01, op11)
								&& NASM_I386::IsImmOperand(op02)
								&& NASM_I386::IsImmOperand(op12))
							{
								m_asm.udex_init(&udgen, UD_Imov, trs[0]->ctx1.rip);
								udgen.operand[0] = *op01;
								m_asm.udex_set_op(&udgen, 1, UD_OP_IMM, NASM_I386::udex_get_opimm(op02) - NASM_I386::udex_get_opimm(op12));
								trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen, (char*)trs[0]->ctx1.opcode, sizeof(trs[0]->ctx1.opcode));
								trs[1]->flags |= mark;
								state = 1;
							}
			}break;
			case UD_Iand:
			{//(mov op1,imm1)(and op1,imm2)->(mov op1<-imm1 and imm2)
							if (NASM_I386::IsSameOperand(op01, op11)
								&& NASM_I386::IsImmOperand(op02)
								&& NASM_I386::IsImmOperand(op12))
							{
								m_asm.udex_init(&udgen, UD_Imov, trs[0]->ctx1.rip);
								udgen.operand[0] = *op01;
								m_asm.udex_set_op(&udgen, 1, UD_OP_IMM, NASM_I386::udex_get_opimm(op02) & NASM_I386::udex_get_opimm(op12));
								trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen, (char*)trs[0]->ctx1.opcode, sizeof(trs[0]->ctx1.opcode));
								trs[1]->flags |= mark;
								state = 1;
							}
			}break;
			case UD_Ior:
			{//(mov op1,imm1)(or op1,imm2)->(mov op1<-imm1 or imm2)
							if (NASM_I386::IsSameOperand(op01, op11)
								&& NASM_I386::IsImmOperand(op02)
								&& NASM_I386::IsImmOperand(op12))
							{
								m_asm.udex_init(&udgen, UD_Imov, trs[0]->ctx1.rip);
								udgen.operand[0] = *op01;
								m_asm.udex_set_op(&udgen, 1, UD_OP_IMM, NASM_I386::udex_get_opimm(op02) | NASM_I386::udex_get_opimm(op12));
								trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen, (char*)trs[0]->ctx1.opcode, sizeof(trs[0]->ctx1.opcode));
								trs[1]->flags |= mark;
								state = 1;
							}
			}break;
			case UD_Iinc:
			{//(mov op1,imm1)(inc op1)->(mov op1<-imm1++)
							if (NASM_I386::IsSameOperand(op01, op11)
								&& NASM_I386::IsImmOperand(op02))
							{
								m_asm.udex_init(&udgen, UD_Imov, trs[0]->ctx1.rip);
								udgen.operand[0] = *op01;
								m_asm.udex_set_op(&udgen, 1, UD_OP_IMM, NASM_I386::udex_get_opimm(op02)+1);
								trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen, (char*)trs[0]->ctx1.opcode, sizeof(trs[0]->ctx1.opcode));
								trs[1]->flags |= mark;
								state = 1;
							}
			}break;
			case UD_Idec:
			{//(mov op1,imm1)(dec op1)->(mov op1<-imm1--)
							if (NASM_I386::IsSameOperand(op01, op11)
								&& NASM_I386::IsImmOperand(op02))
							{
								m_asm.udex_init(&udgen, UD_Imov, trs[0]->ctx1.rip);
								udgen.operand[0] = *op01;
								m_asm.udex_set_op(&udgen, 1, UD_OP_IMM, NASM_I386::udex_get_opimm(op02) -1);
								trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen, (char*)trs[0]->ctx1.opcode, sizeof(trs[0]->ctx1.opcode));
								trs[1]->flags |= mark;
								state = 1;
							}
			}break;
			case UD_Inot:
			{//(mov op1,imm1)(not op1)->(mov op1<- not imm1)
							if (NASM_I386::IsSameOperand(op01, op11)
								&& NASM_I386::IsImmOperand(op02))
							{
								m_asm.udex_init(&udgen, UD_Imov, trs[0]->ctx1.rip);
								udgen.operand[0] = *op01;
								m_asm.udex_set_op(&udgen, 1, UD_OP_IMM, ~NASM_I386::udex_get_opimm(op02));
								trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen, (char*)trs[0]->ctx1.opcode, sizeof(trs[0]->ctx1.opcode));
								trs[1]->flags |= mark;
								state = 1;
							}
			}break;
			case UD_Ineg:
			{//(mov op1,imm1)(neg op1)->(mov op1<- neg imm1)
							if (NASM_I386::IsSameOperand(op01, op11)
								&& NASM_I386::IsImmOperand(op02))
							{
								m_asm.udex_init(&udgen, UD_Imov, trs[0]->ctx1.rip);
								udgen.operand[0] = *op01;
								m_asm.udex_set_op(&udgen, 1, UD_OP_IMM, -NASM_I386::udex_get_opimm(op02));
								trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen, (char*)trs[0]->ctx1.opcode, sizeof(trs[0]->ctx1.opcode));
								trs[1]->flags |= mark;
								state = 1;
							}
			}break;
			case UD_Ishl:
			{//(mov op1,imm1)(shl op1)->(mov op1<- shl imm1)
							if (NASM_I386::IsSameOperand(op01, op11)
								&& NASM_I386::IsImmOperand(op02)
								&& NASM_I386::IsImmOperand(op12))
							{
								m_asm.udex_init(&udgen, UD_Imov, trs[0]->ctx1.rip);
								udgen.operand[0] = *op01;
								m_asm.udex_set_op(&udgen, 1, UD_OP_IMM, NASM_I386::udex_get_opimm(op02) << NASM_I386::udex_get_opimm(op12));
								trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen, (char*)trs[0]->ctx1.opcode, sizeof(trs[0]->ctx1.opcode));
								trs[1]->flags |= mark;
								state = 1;
							}
			}break;
			case UD_Ishr:
			{//(mov op1,imm1)(shr op1)->(mov op1<- shr imm1)
							if (NASM_I386::IsSameOperand(op01, op11)
								&& NASM_I386::IsImmOperand(op02)
								&& NASM_I386::IsImmOperand(op12))
							{
								m_asm.udex_init(&udgen, UD_Imov, trs[0]->ctx1.rip);
								udgen.operand[0] = *op01;
								m_asm.udex_set_op(&udgen, 1, UD_OP_IMM, NASM_I386::udex_get_opimm(op02) >> NASM_I386::udex_get_opimm(op12));
								trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen, (char*)trs[0]->ctx1.opcode, sizeof(trs[0]->ctx1.opcode));
								trs[1]->flags |= mark;
								state = 1;
							}
			}break;
            }
			if (state <= 0 && nhas >= 3)
			{
				const ud_operand* op21 = ud_insn_opr(&udis[2], 0);
				const ud_operand* op22 = ud_insn_opr(&udis[2], 1);
				if (udis[2].mnemonic == UD_Isub
					&& NASM_I386::IsImmOperand(op02) && NASM_I386::IsSameOperand(op02, op22)
					&& NASM_I386::IsSameOperand(op01, op21))
				{//(mov op1,imm1)(??)(sub op1,imm1) -> mov op1,0 (??)
					switch (udis[1].mnemonic)
					{
					case UD_Isub:
					case UD_Iadd:
					{
									if (!NASM_I386::IsSameOperand(op01, op12))
									{
										m_asm.udex_init(&udgen, UD_Imov, trs[0]->ctx1.rip);
										udgen.operand[0] = *op01;
										m_asm.udex_set_op(&udgen, 1, UD_OP_IMM,0);
										trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen, (char*)trs[0]->ctx1.opcode, sizeof(trs[0]->ctx1.opcode));
										trs[2]->flags |= mark;
										state = 1;
									}
					}break;
					}
				}
				else if (udis[2].mnemonic == UD_Iadd
					&& NASM_I386::IsImmOperand(op02) && NASM_I386::IsImmOperand(op22)
					&& NASM_I386::IsSameOperand(op01, op21))
				{//(mov op1,imm1)(??)(add op1,imm1+imm2 == 0) -> mov op1,0 (??)
					if (NASM_I386::udex_get_sizeimm(NASM_I386::udex_get_opimm(op02) + NASM_I386::udex_get_opimm(op22), op02->size) == 0)
					{
						switch (udis[1].mnemonic)
						{
						case UD_Isub:
						case UD_Iadd:
						{
										if (!NASM_I386::IsSameOperand(op01, op12))
										{
											m_asm.udex_init(&udgen, UD_Imov, trs[0]->ctx1.rip);
											udgen.operand[0] = *op01;
											m_asm.udex_set_op(&udgen, 1, UD_OP_IMM, 0);
											trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen, (char*)trs[0]->ctx1.opcode, sizeof(trs[0]->ctx1.opcode));
											trs[2]->flags |= mark;
											state = 1;
										}
						}break;
						}
					}
				}
			}
        }break;
        case UD_Ixor:
        {
            switch(udis[1].mnemonic)
            {
            case UD_Ixor:
            {
                if (NASM_I386::IsSameOperand(op01,op11) && NASM_I386::IsSameOperand(op02,op12))
                {
                    trs[0]->flags |= mark;
                    trs[1]->flags |= mark;
                    state = 1;
                }else if (nhas >= 3 && udis[2].mnemonic == UD_Ixor)
                {   //xor<>xor<>xor
                    const ud_operand* op21 = ud_insn_opr(&udis[2],0);
                    const ud_operand* op22 = ud_insn_opr(&udis[2],1);
                    if (NASM_I386::IsSameOperand(op01,op12)
                            && NASM_I386::IsSameOperand(op01,op21)
                            && NASM_I386::IsSameOperand(op02,op11)
                            && NASM_I386::IsSameOperand(op02,op22))
                    {
                        m_asm.udex_init(&udgen,UD_Ixchg,trs[0]->ctx1.rip);
                        udgen.operand[0] = *op01;
                        udgen.operand[1] = *op02;
                        trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[0]->ctx1.opcode,sizeof(trs[0]->ctx1.opcode));
                        trs[1]->flags |= mark;
                        trs[2]->flags |= mark;
                        state = 1;
                    }
                }
            }break;
            }


        }break;
        case UD_Ilea:
        {
            if (udis[1].mnemonic == UD_Ijmp
                    && NASM_I386::IsRegister(op01,UD_R_RSP,true)
                    && NASM_I386::IsEspMemOperand(op02,4)
                    && NASM_I386::IsEspMemOperand(op11,-4))
            {
                m_asm.udex_init(&udgen,UD_Iret,trs[0]->ctx1.rip);
                trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[0]->ctx1.opcode,sizeof(trs[0]->ctx1.opcode));
                trs[1]->flags |= mark;
                state = 1;
            }else if (udis[1].mnemonic == UD_Ilea
                      && NASM_I386::IsSameOperand(op01,op11))
            {
                trs[0]->flags |= mark;
                state = 1;
            }else if (op02->index == UD_NONE && op02->scale == 0
                      && op11 && NASM_I386::IsSameOperand(op01,op11))
            {
                int64_t lva;
                switch(udis[1].mnemonic)
                {
                case UD_Isub:case UD_Iadd:
                {
                    if (op12->type != UD_OP_IMM)
                        break;
                    lva = NASM_I386::udex_get_opimm(op12);
                }
                case UD_Iinc:case UD_Idec:
                {
                    m_asm.udex_init(&udgen,UD_Ilea,trs[0]->ctx1.rip);
                    udgen.operand[0] = *op01;
                    int64_t ulv = op02->lval.sqword;
                    switch(udis[1].mnemonic)
                    {
                    case UD_Isub:ulv -= lva;break;
                    case UD_Iadd:ulv += lva;break;
                    case UD_Iinc:ulv++;break;
                    case UD_Idec:ulv--;break;
                    }
                    m_asm.udex_set_op(&udgen,1,UD_OP_MEM,ulv,0,op02->base);
                    trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[0]->ctx1.opcode,sizeof(trs[0]->ctx1.opcode));
                    trs[1]->flags |= mark;
                    state = 1;
                }break;
                }
            }else if (NASM_I386::IsRegister(op01,UD_R_RSP,true)
                      && op02->type == UD_OP_MEM && op02->scale == 0 && op02->index == UD_NONE
                      && op02->base == op01->base)
            {//lea esp,[esp-4]->sub esp,--4
                int64_t lv = NASM_I386::udex_get_opimm(op02);
                if (lv == 0)
                {
                    trs[0]->flags |= mark;
                    state = 1;
                }else if (lv > 0)
                {
                    m_asm.udex_init(&udgen,UD_Iadd,trs[0]->ctx1.rip);
                    m_asm.udex_set_op(&udgen,0,UD_OP_REG,0,0,op01->base);
                    m_asm.udex_set_op(&udgen,1,UD_OP_IMM,lv,op02->size);
                    trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[0]->ctx1.opcode,sizeof(trs[0]->ctx1.opcode));
                    state = 1;
                }else   //lv < 0
                {
                    lv = abs(lv);
                    m_asm.udex_init(&udgen,UD_Isub,trs[0]->ctx1.rip);
                    m_asm.udex_set_op(&udgen,0,UD_OP_REG,0,0,op01->base);
                    m_asm.udex_set_op(&udgen,1,UD_OP_IMM,lv,op02->size);
                    trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[0]->ctx1.opcode,sizeof(trs[0]->ctx1.opcode));
                    state = 1;
                }

            }
        }break;
        }
        if (state <= 0)
        {
            if ((udis[0].mnemonic == UD_Isub && (udis[1].mnemonic == UD_Isub || udis[1].mnemonic == UD_Iadd))
                    || (udis[0].mnemonic == UD_Iadd && (udis[1].mnemonic == UD_Isub || udis[1].mnemonic == UD_Iadd)))
            {
                if (NASM_I386::IsSameOperand(op01,op11))
                {
                    if ((op02->type == UD_OP_IMM || op02->type == UD_OP_CONST) && op02->type == op12->type)
                    {
                        int64_t delta = 0;
                        switch(udis[0].mnemonic)
                        {
                        case UD_Isub:
                        {
                            if (udis[1].mnemonic == UD_Isub)
                                delta = -(op02->lval.sqword + op12->lval.sqword);
                            else
                                delta = op12->lval.sqword - op02->lval.sqword;
                        }break;
                        case UD_Iadd:
                        {
                            if (udis[1].mnemonic == UD_Isub)
                                delta = op02->lval.sqword - op12->lval.sqword;
                            else
                                delta = op12->lval.sqword + op02->lval.sqword;
                        }break;
                        }
                        if (delta == 0)
                        {
                            trs[0]->flags |= mark;
                            trs[1]->flags |= mark;
                        }else
                        {
                            if (delta > 0)
                            {
                                m_asm.udex_init(&udgen,UD_Iadd,trs[0]->ctx1.rip);
                                udgen.operand[0] = *op01;
                                m_asm.udex_set_op(&udgen,1,UD_OP_IMM,delta,op01->size);
                                trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[0]->ctx1.opcode,sizeof(trs[0]->ctx1.opcode));
                            }else
                            {
                                m_asm.udex_init(&udgen,UD_Isub,trs[0]->ctx1.rip);
                                udgen.operand[0] = *op01;
                                m_asm.udex_set_op(&udgen,1,UD_OP_IMM,abs(delta),op01->size);
                                trs[0]->ctx1.szOP = m_asm.udex_to_bc(&udgen,(char*)trs[0]->ctx1.opcode,sizeof(trs[0]->ctx1.opcode));
                            }
                            trs[1]->flags |= mark;
                        }
                        state = 1;
                    }
                }
            }
        }

        if (state <= 0)
        {
            if (op01 && op02 && udis[1].mnemonic == UD_Imov
                    && NASM_I386::IsEspMemOperand(op01)
                    && NASM_I386::IsEspMemOperand(op11)
                    && op01->size == op11->size
                    && !IsEffectESP(trs[0]))
            {
                trs[0]->flags |= mark;
                state = 1;
            }
        }
        if (state > 0)
            nrc += state;
    }
    return nrc;
}

int xVMDebugFrame::doDeobfuscateTempPushPop(InstSequence *seq)
{
    ud_t udis;
    ud_init(&udis);
    ud_set_mode(&udis,32);
    TraceRecordEx* tr1 = 0;
    TraceRecordEx* tr2 = 0;
    ud_type lastReg = UD_NONE;
    int nc = 0;
    for (TraceRecordEx* tr = seq->insts.first();tr;tr = seq->insts.next(tr))
    {
        if (tr->flags & RECORD_FLAG_DELETE)
            continue;
        ud_set_input_buffer(&udis,(const uint8_t*)tr->ctx1.opcode,tr->ctx1.szOP);
        ud_set_pc(&udis,tr->ctx1.rip);
        int instlen = ud_decode(&udis);
        if (instlen < 1) break;
        nc++;

        if (udis.mnemonic == UD_Ipush)
        {
            const ud_operand* oper1 = ud_insn_opr(&udis,0);
            if (oper1 && oper1->type == UD_OP_REG)
            {
                tr1 = tr;
                lastReg = oper1->base;
            }else
            {
                tr1 = 0;
                tr2 = 0;
                nc = 0;
                continue;
            }
        }else
        {
            BOOL isValid = FALSE;
            if (udis.mnemonic == UD_Ipop)
            {
                const ud_operand* oper1 = ud_insn_opr(&udis,0);
                if (oper1 && oper1->type == UD_OP_REG && oper1->base == lastReg)
                {
                    tr2 = tr;
                    isValid = TRUE;
                }
            }
            if (!isValid && IsEffectESP(tr))   //stack changed
            {
                tr1 = 0;
                tr2 = 0;
                nc = 0;
                continue;
            }
        }

        if (tr1 && tr2)
        {
            tr1->flags |= RECORD_FLAG_TEMPFLAG;
            tr2->flags |= RECORD_FLAG_TEMPFLAG;
            ud_lval lv;
            lv.uqword = 0;
            int ts = 0;
            ud_type lr64 = NASM_I386::udex_get_rel_reg(lastReg,64);
            ud_type lruse = UD_NONE;
            for (TraceRecordEx* tr = seq->insts.next(tr1);tr != tr2;tr = seq->insts.next(tr))
            {
                if (!tr) break;
                ud_set_input_buffer(&udis,(const uint8_t*)tr->ctx1.opcode,tr->ctx1.szOP);
                ud_set_pc(&udis,tr->ctx1.rip);
                int instlen = ud_decode(&udis);
                if (instlen < 1) break;
                BOOL vaildStep = FALSE;
                const ud_operand* op1 = ud_insn_opr(&udis,0);
                const ud_operand* op2 = ud_insn_opr(&udis,1);
                switch(udis.mnemonic)
                {
                case UD_Imov:
                {
                    if ((ts < 2) && op1->type == UD_OP_REG &&
                            ((NASM_I386::udex_get_rel_reg(op1->base,64) == lr64))
                            && (op2->type == UD_OP_IMM || op2->type == UD_OP_CONST))
                    {
                        ts = 1;
                        lruse = op1->base;
                        vaildStep = TRUE;
                        lv = op2->lval;
                        tr->flags |= RECORD_FLAG_TEMPFLAG;
                    }else if (ts < 3)
                    {
                        if (NASM_I386::udex_isref_reg(op1,lastReg) != UD_NONE)
                        {  //x? lastReg,imm
                            if ((op2->type == UD_OP_IMM || op2->type == UD_OP_CONST))
                            {
                                lv.uqword = op2->lval.uqword;
                                vaildStep = TRUE;
                                ts = 2;
                                tr->flags |= RECORD_FLAG_TEMPFLAG;
                            }
                        }else if (op2->type == UD_OP_REG && op2->base == lruse)
                        {
                            vaildStep = TRUE;
                            ts = 3;
                            tr->flags |= RECORD_FLAG_OPERFLAG;
                        }
                    }
                }break;
                case UD_Ibswap: case UD_Inot: case UD_Ineg:case UD_Idec:case UD_Iinc:
                {
                    if ((ts < 3) && op1->type == UD_OP_REG && op1->base == lruse)
                    {
                        switch(udis.mnemonic)
                        {
                        case UD_Inot:
                        {
                            switch(op1->size)
                            {
                            case 8: lv.sbyte = ~lv.sbyte;break;
                            case 16: lv.sword = ~lv.sword;break;
                            case 32: lv.sdword = ~lv.sdword;break;
                            case 48: case 64: lv.sqword = ~lv.sqword;break;
                            }
                        }break;
                        case UD_Iinc:
                        {
                            switch(op1->size)
                            {
                            case 8: lv.sbyte++;break;
                            case 16: lv.sword++;break;
                            case 32: lv.sdword++;break;
                            case 48: case 64: lv.sqword++;break;
                            }
                        }break;
                        case UD_Idec:
                        {
                            switch(op1->size)
                            {
                            case 8: lv.sbyte--;break;
                            case 16: lv.sword--;break;
                            case 32: lv.sdword--;break;
                            case 48: case 64: lv.sqword--;break;
                            }
                        }break;
                        case UD_Ineg:
                        {
                            switch(op1->size)
                            {
                            case 8: lv.sbyte = -lv.sbyte;break;
                            case 16: lv.sword = -lv.sword;break;
                            case 32: lv.sdword = -lv.sdword;break;
                            case 48:
                            case 64: lv.sqword = -lv.sqword;break;
                            }
                        }break;
                        case UD_Ibswap:
                        {
                            switch(op1->size)
                            {
                            case 8:
                            case 16: lv.uword = xvm_bswap16(lv.uword);break;
                            case 32: lv.udword = xvm_bswap32(lv.udword);break;
                            case 48:
                            case 64: lv.uqword = xvm_bswap64(lv.uqword);break;
                            }
                        }break;
                        }
                        vaildStep = TRUE;
                        ts = 2;
                        tr->flags |= RECORD_FLAG_TEMPFLAG;
                    }
                }break;
                case UD_Isub: case UD_Iadd: case UD_Ixor:case UD_Iand:case UD_Ior:case UD_Ishr:case UD_Ishl:
                {
                    if (ts < 3)
                    {
                        if (NASM_I386::udex_isref_reg(op1,lastReg) != UD_NONE)
                        {  //x? lastReg,imm
                            if ((op2->type == UD_OP_IMM || op2->type == UD_OP_CONST))
                            {
                                int64_t i64vl = NASM_I386::udex_get_opimm(op2);
                                switch(udis.mnemonic)
                                {
                                case UD_Isub:
                                {
                                    switch(op1->size)
                                    {
                                    case 8: lv.sbyte -= i64vl;break;
                                    case 16: lv.sword -= i64vl;break;
                                    case 32: lv.sdword -= i64vl;break;
                                    case 48: case 64: lv.sqword -= i64vl;break;
                                    }
                                }break;
                                case UD_Iadd:
                                {
                                    switch(op1->size)
                                    {
                                    case 8: lv.sbyte += i64vl;break;
                                    case 16: lv.sword += i64vl;break;
                                    case 32: lv.sdword += i64vl;break;
                                    case 48: case 64: lv.sqword += i64vl;break;
                                    }
                                }break;
                                case UD_Ixor:
                                {
                                    switch(op1->size)
                                    {
                                    case 8: lv.sbyte ^= i64vl;break;
                                    case 16: lv.sword ^= i64vl;break;
                                    case 32: lv.sdword ^= i64vl;break;
                                    case 48: case 64: lv.sqword ^= i64vl;break;
                                    }
                                }break;
                                case UD_Iand:
                                {
                                    switch(op1->size)
                                    {
                                    case 8: lv.sbyte &= i64vl;break;
                                    case 16: lv.sword &= i64vl;break;
                                    case 32: lv.sdword &= i64vl;break;
                                    case 48: case 64: lv.sqword &= i64vl;break;
                                    }
                                }break;
                                case UD_Ior:
                                {
                                    switch(op1->size)
                                    {
                                    case 8: lv.sbyte |= i64vl;break;
                                    case 16: lv.sword |= i64vl;break;
                                    case 32: lv.sdword |= i64vl;break;
                                    case 48: case 64: lv.sqword |= i64vl;break;
                                    }
                                }break;
                                case UD_Ishr:
                                {
                                    switch(op1->size)
                                    {
                                    case 8: lv.ubyte >>= i64vl;break;
                                    case 16: lv.uword >>= i64vl;break;
                                    case 32: lv.udword >>= i64vl;break;
                                    case 48: case 64: lv.uqword >>= i64vl;break;
                                    }
                                }break;
                                case UD_Ishl:
                                {
                                    switch(op1->size)
                                    {
                                    case 8: lv.ubyte <<= i64vl;break;
                                    case 16: lv.uword <<= i64vl;break;
                                    case 32: lv.udword <<= i64vl;break;
                                    case 48: case 64: lv.uqword <<= i64vl;break;
                                    }
                                }break;
                                }
                                vaildStep = TRUE;
                                ts = 2;
                                tr->flags |= RECORD_FLAG_TEMPFLAG;
                            }
                        }else if (op2->type == UD_OP_REG && op2->base == lruse)
                        {
                            vaildStep = TRUE;
                            ts = 3;
                            tr->flags |= RECORD_FLAG_OPERFLAG;
                        }
                    }
                }break;
                }

				if (!vaildStep)
				{
					//here we check push op1,?? [esp] pop op1->?? op1 push op1 ... pop op1
					switch (udis.mnemonic)
					{
					case UD_Ibswap: case UD_Inot: case UD_Ineg:case UD_Idec:case UD_Iinc:
					{
										if (op1 && NASM_I386::IsEspMemOperand(op1, 0))
										{
											vaildStep = TRUE;
											ts = 4;
											tr->flags |= RECORD_FLAG_MOVETOTOP;
										}
											
					}break;
					}
				}
                if (!vaildStep)
                {
                    for (int i=0;i<3;i++)
                    {
                        const ud_operand* op = ud_insn_opr(&udis,i);
                        if (op && op->type != UD_NONE)
                        {
                            if ( ((op->base != UD_NONE) && ((NASM_I386::udex_get_rel_reg(op->base,64) == lr64)))
                                 ||   ((op->index != UD_NONE) && ((NASM_I386::udex_get_rel_reg(op->index,64) == lr64)))
                                 || NASM_I386::udex_get_rel_reg(op->base,64) == UD_R_RSP
                                 || NASM_I386::udex_get_rel_reg(op->index,64) == UD_R_RSP)

                            {
                                ts = -1;
                                break;
                            }
                        }
                    }
                }
                if (ts < 0) break;
            }
            tr2 = seq->insts.next(tr2);
            BOOL validSeq = FALSE;
			switch (ts)
			{
			case 3:
			{
					  for (TraceRecordEx* tr = tr1; tr && tr != tr2; tr = seq->insts.next(tr))
					  {
						  if (tr->flags & RECORD_FLAG_OPERFLAG)
						  {
							  validSeq = TRUE;
							  break;
						  }
					  }
					  if (validSeq)
					  {
						  for (TraceRecordEx* tr = tr1; tr && tr != tr2; tr = seq->insts.next(tr))
						  {							  //RECORD_FLAG_DELETE;
							  if (tr->flags & RECORD_FLAG_OPERFLAG)
							  {
								  //tr->flags |= RECORD_FLAG_JUNK;
								  ud_set_input_buffer(&udis, (const uint8_t*)tr->ctx1.opcode, tr->ctx1.szOP);
								  ud_set_pc(&udis, tr->ctx1.rip);
								  int instlen = ud_decode(&udis);
								  if (instlen >= 0)
								  {
									  m_asm.udex_set_op(&udis, 1, UD_OP_IMM, lv.uqword, udis.operand[1].size);
									  tr->ctx1.szOP = m_asm.udex_to_bc(&udis, (char*)tr->ctx1.opcode, sizeof(tr->ctx1.opcode));
								  }

							  }
						  }
					  }
			}break;
			case 4:
			{
				validSeq = FALSE;
			}break;
			case 0:
			{
				validSeq = TRUE;
			}break;
			default:break;
			}
            for (TraceRecordEx* tr = tr1;tr != tr2;tr = seq->insts.next(tr))
            {
                if (!tr) break;
                if (tr->flags & RECORD_FLAG_TEMPFLAG)
                {
                    tr->flags &= ~RECORD_FLAG_TEMPFLAG;
                    if (validSeq)
                    {
                        tr->flags |= RECORD_FLAG_JUNK | RECORD_FLAG_DELETE;
                    }
                }
				if (tr->flags & RECORD_FLAG_MOVETOTOP)
				{
					TraceRecordEx* ct = tr;
					while (ct != tr1)
					{
						if (!seq->insts.up(tr)) break;
						ct = seq->insts.next(tr);
					}
					ud_set_input_buffer(&udis, (const uint8_t*)tr->ctx1.opcode, tr->ctx1.szOP);
					ud_set_pc(&udis, tr->ctx1.rip);
					int instlen = ud_decode(&udis);
					if (instlen > 0)
					{
						m_asm.udex_set_op(&udis, 0, UD_OP_REG, 0, udis.opr_mode, NASM_I386::udex_get_rel_reg(lastReg, udis.opr_mode));
						tr->ctx1.szOP = m_asm.udex_to_bc(&udis, (char*)tr->ctx1.opcode, sizeof(tr->ctx1.opcode));
					}
					tr->flags &= ~RECORD_FLAG_MOVETOTOP;
				}
                if (tr->flags & RECORD_FLAG_OPERFLAG)
                    tr->flags &= ~RECORD_FLAG_OPERFLAG;
                if (validSeq && !(tr->flags & RECORD_FLAG_DELETE))
                {
                    ud_set_input_buffer(&udis,(const uint8_t*)tr->ctx1.opcode,tr->ctx1.szOP);
                    ud_set_pc(&udis,tr->ctx1.rip);
                    int instlen = ud_decode(&udis);
                    if (instlen > 0)
                    {
                        BOOL reGen = FALSE;
                        for (int i=0;i<3;i++)
                        {
                            ud_operand* op = (ud_operand*)ud_insn_opr(&udis,i);
                            if (op && op->type == UD_OP_MEM && NASM_I386::udex_get_rel_reg(op->base,64) == UD_R_RSP)

                            {
                                switch(op->size)
                                {
                                case 8: {
                                    op->lval.sbyte -= 4;
                                }break;
                                case 16: op->lval.sword -= 4;break;
                                case 32: op->lval.sdword -= 4;break;
                                case 48: case 64: op->lval.sqword -= 4;break;
                                }
                                reGen = TRUE;
                            }
                        }
                        if (reGen)
                        {
                            tr->ctx1.szOP = m_asm.udex_to_bc(&udis,(char*)tr->ctx1.opcode,sizeof(tr->ctx1.opcode));
                        }
                    }
                }
            }


            tr1 = 0;
            tr2 = 0;
        }
    }
    return 0;
}

int xVMDebugFrame::doDeObfuscatePattern()
{
	//doDeobfuscateNormal(&m_se1);
	int nr = xVMDebugCPU::doDeObfuscatePattern(&m_paAsm,&m_seq1,false);
	nr += xVMDebugCPU::doDeObfuscatePattern(&m_paAsm,&m_seq1,true);
	gLPFlash(L"Detected %d Junk", nr);
	flushSeq();
	return nr;
}


int xVMDebugFrame::loadPatterns()
{
	m_paAsm.clearPatterns();
	wchar_t path[MAX_PATH*2];
	GetModuleFileNameW(ghxVMDebug, path, sizeof(path) / sizeof(wchar_t));
	int nplen = wcslen(path);
	for (int i = nplen - 1; i >= 0; i--) {
		if (path[i] == '/' || path[i] == '\\') {
			path[i+1] = 0;
			break;
		}
	}
	wcscat(path, gxvmcfg.patternDir.c_str());
	wchar_t pathHead[MAX_PATH*2];
	wcscpy(pathHead, path);
	wcscat(pathHead, L"\\");
	wcscat(path, L"\\*.pasm");
	WIN32_FIND_DATAW wf32 = { 0 };
	int pn = 0;
	if (HANDLE hFind = FindFirstFileW(path, &wf32))  {
			do 
			{
				if(!(wf32.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))  {
					wchar_t pathOne[MAX_PATH * 2];
					wcscpy(pathOne, pathHead);
					wcscat(pathOne, wf32.cFileName);
					pn += m_paAsm.loadPatternFromFile(pathOne);
				}
			} while (FindNextFileW(hFind,&wf32));
			FindClose(hFind);
	}
	return pn;
}

int xVMDebugFrame::doDeObfuscatePatternAll()
{
	int nr = 0;
	int nall = 0;
	int ncount = 0;
	do{
		nr = xVMDebugCPU::doDeObfuscatePattern(&m_paAsm,&m_seq1,false);
		nr += xVMDebugCPU::doDeObfuscatePattern(&m_paAsm,&m_seq1,true);
		nall += nr;
		if (ncount++ > 200) break;
	}while (nr > 0);
	gLPFlash(L"Deobfuscate %d Instructions",nall);
	flushSeq();
	return 0;
}

HWND xVMDebugFrame::hFrame()
{
	return m_hFrame;
}

void xVMDebugFrame::dobCurrentSequenceAll()
{

	int nr = 0;
	while (true)
	{
		int nonce = doDeobfuscateNormal(&m_seq1,false);
		deleteFlagedInSeq(&m_seq1);
		nonce += doDeobfuscateTempPushPop(&m_seq1);
		deleteFlagedInSeq(&m_seq1);
		if (nonce < 1) break;
		nr += nonce;
	}
	
	gLPFlash(L"Detected %d Junk",nr);
	flushSeq();
}

void xVMDebugFrame::bringToFront()
{
	HWND hmdi = GetParent(m_hFrame);
	SendMessage(hmdi,WM_MDIACTIVATE,(WPARAM)m_hFrame,0);
}

t_block* xVMDebugFrame::block()
{
	return &m_block_host;
}

void xVMDebugFrame::autoTrace(ULONG_PTR rip)
{
	if (m_seq1.insts.GetCount() < 1)
		trace(trace_ateip);
}

t_table* xVMDebugFrame::table()
{
	return &m_table_disasm;
}

void xVMDebugFrame::clearSeq()
{
	m_seq1.insts.clear();
}

InstSequence* xVMDebugFrame::Sequence()
{
	return &m_seq1;
}

void xVMDebugFrame::detectCodeBlocks()
{
	xVMBlockFrame* xbf = VMDebug_BlockFrame();
	xbf->resetBlocks();
	xbf->analyserTraceRecords(&m_seq1.insts);
}

void xVMDebugFrame::flushMemAccess()
{
	gLPDestroysorteddata(&m_table_memacc.sorted);
	gLPCreatesorteddata(&m_table_memacc.sorted,sizeof(xvm_memaccess),1000,0,0,SDM_EXTADDR);
	int uid = 1;
	int iid = 1;
	for (TraceRecordEx* tr = m_seq1.insts.first(); tr; tr = m_seq1.insts.next(tr),iid++)
	{
		if (tr->flags & (RECORD_FLAG_MEMREAD | RECORD_FLAG_MEMWRITE))
		{
			BOOL bShow = TRUE;
			if (!(gxvmcfg.showMemAccFlags & mac_showstack) && (tr->flags & (RECORD_FLAG_SPREAD | RECORD_FLAG_SPWRITE)))
				bShow = FALSE;

			if (!(gxvmcfg.showMemAccFlags & mac_showread) && (tr->flags & RECORD_FLAG_MEMREAD))
				bShow = FALSE;
			if (!(gxvmcfg.showMemAccFlags & mac_showwrite) && (tr->flags & RECORD_FLAG_MEMWRITE))
				bShow = FALSE;
			if (bShow)
			{
				xvm_memaccess xvmm;
				xvmm.size = 1;
				xvmm.type = 0;
				xvmm.instID = iid;
				if (tr->flags & RECORD_FLAG_MEMREAD)
				{
					xvmm.addr = uid++;
					xvmm.memptr = tr->mraddr;
					xvmm.memsize = tr->mrsize;
					xvmm.memvalue = tr->mrvalue;
					xvmm.mode = tr->flags & (RECORD_FLAG_MEMREAD | RECORD_FLAG_SPREAD);
					gLPAddsorteddata(&m_table_memacc.sorted,&xvmm);
				} 
				if (tr->flags & RECORD_FLAG_MEMWRITE)
				{
					xvmm.addr = uid++;
					xvmm.memptr = tr->mwaddr;
					xvmm.memsize = tr->mwsize;
					xvmm.memvalue = tr->mwvalue;
					xvmm.mode = tr->flags & (RECORD_FLAG_MEMWRITE| RECORD_FLAG_SPWRITE);
					gLPAddsorteddata(&m_table_memacc.sorted,&xvmm);
				}
				
			}
		}
	}

	gLPUpdatetable(&m_table_memacc,0);
}

void xVMDebugFrame::runPattern(const TCHAR* filename,BOOL once)
{
	m_paAsmOnce.clearPatterns();
	m_paAsmOnce.loadPatternFromFile(filename);
	int nr = 0;
	int nc = 0;
	int ncount = 0;
	do{
		nr = xVMDebugCPU::doDeObfuscatePattern(&m_paAsmOnce,&m_seq1,false);
		nr += xVMDebugCPU::doDeObfuscatePattern(&m_paAsmOnce,&m_seq1,true);
		nc += nr;
		if (ncount++ > 200) break;
	} while (nr > 0 && !once);

	gLPFlash(L"Detected %d Junk",nc);
	flushSeq();
}

void xVMDebugFrame::syncMemAccByIndex(int index)
{
	if (!m_table_memacc.hw)
		return;
	for (int i = 0; i < m_table_memacc.sorted.n; i++)
	{
		xvm_memaccess* xrt = (xvm_memaccess*)gLPGetsortedbyindex(&m_table_memacc.sorted,i);
		if (xrt)
		{
			if (xrt->instID == index)
			{
				gLPSettableselection(&m_table_memacc,i);
				gLPUpdatetable(&m_table_memacc,0);
				break;
			}
		}
	}
}

void xVMDebugFrame::syncRecordsByIndex(int index)
{
	if (!m_table_disasm.hw)
		return;
	for (int i = 0; i < m_table_disasm.sorted.n; i++)
	{
		xvm_record* xrt = (xvm_record*)gLPGetsortedbyindex(&m_table_disasm.sorted,i);
		if (xrt)
		{
			if (xrt->addr == index)
			{
				gLPSettableselection(&m_table_disasm,i);
				gLPUpdatetable(&m_table_disasm,0);
				break;
			}
		}
	}
}

void xVMDebugFrame::assemblerSelectRecord()
{
	xvm_record* xr = (xvm_record*)gLPGetsortedbyselection(&m_table_disasm.sorted,m_table_disasm.sorted.selected);
	if (xr)
	{
		POINT pt = { 0 };
		GetCursorPos(&pt);
		//        stdapi (int)     Getstring(HWND hparent,wchar_t *title,wchar_t *s,int length,
		//                           int savetype,int letter,int x,int y,int fi,int mode);
		wchar_t cmt[100];
		wsprintf(cmt,L"Enter Command For Record %d",xr->addr);
		wchar_t tmpstr[TEXTLEN];
		tmpstr[0] = 0;
		//wcscpy(tmpstr,xr->comment);
		int nret = gLPGetstring(m_table_disasm.hparent,cmt,tmpstr,sizeof(tmpstr) / sizeof(wchar_t)-1,0,0,pt.x,pt.y,0,0);
		if (nret >= 0)
		{
			wchar_t errstr[TEXTLEN];
			int nlen = gLPAssemble(tmpstr,xr->rc.ctx1.rip,xr->rc.ctx1.opcode,sizeof(xr->rc.ctx1.opcode),0,errstr);
			if (nlen > 0)
			{
				TraceRecordEx* tr = m_seq1.insts.At(xr->addr-1);
				if (tr)
					*tr = xr->rc;
				gLPUpdatetable(&m_table_disasm,0);	
			} else
				gLPFlash(L"%s",errstr);
		}
	}
}

void xVMDebugFrame::saveRecords(TCHAR* path)
{
	saveRecordsToFile(&m_seq1.insts,path);
}

void xVMDebugFrame::exportToText(TCHAR* path)
{
	NAutoLargeBufferA nla;
	nla.setHostFile(path);
	nla.setMaxmemsize(1024 * 1024 * 4);
	nla.resize(0);
	char tmpstr[1024];
	ncasm::x86::code ci = {0};
	ncasm::x86::inst ins;
	ncasm::x86::insttext instext;
	ci.features = 0;
	ci.dt = Decode32Bits;
	for (TraceRecordEx* tr = m_seq1.insts.first(); tr; tr = m_seq1.insts.next(tr))
	{
		ci.code = (const uint8_t*)tr->ctx1.opcode;
		ci.codeLen = tr->ctx1.szOP;
		ci.codeOffset = tr->ctx1.rip;
		ncasm::x86::disasm(&ci,&ins,1);
		ncasm::x86::format(&ci,&ins,&instext);
		int slen = snprintf(tmpstr,sizeof(tmpstr)-1,"%08X\t%-8s %-48s EAX=%08X ECX=%08X EDX=%08X EBX=%08X ESP=%08X EBP=%08X ESI=%08X EDI=%08X EFLAG=%08X\r\n",tr->ctx1.rip,instext.mnemonic.p,instext.operands.p,
			tr->ctx1.regs[0],
			tr->ctx1.regs[1],
			tr->ctx1.regs[2],
			tr->ctx1.regs[3],
			tr->ctx1.regs[4],
			tr->ctx1.regs[5],
			tr->ctx1.regs[6],
			tr->ctx1.regs[7],
#ifndef _M_IX86
			tr->ctx1.regs[8],
			tr->ctx1.regs[9],
			tr->ctx1.regs[10],
			tr->ctx1.regs[11],
			tr->ctx1.regs[12],
			tr->ctx1.regs[13],
			tr->ctx1.regs[14],
			tr->ctx1.regs[15],
#endif
			tr->ctx1.eflags
			);
		nla.append(tmpstr,slen);
	}
	nla.close();
}

ChainCPU* xVMDebugFrame::ensureChainCPU()
{
	if (!m_ccpu)
		m_ccpu = new ChainCPU;
	return m_ccpu;
}

void xVMDebugFrame::showMemChain(xvm_record* xr)
{	//如果链式内存分析CPU没有初始化，说明没有进行内存分析
	if (!m_ccpu) return;
	//清除已有的内存访问记录
	gLPDestroysorteddata(&m_table_memacc.sorted);
	gLPCreatesorteddata(&m_table_memacc.sorted,sizeof(xvm_memaccess),1000,0,0,SDM_EXTADDR);
	int instID = xr->addr;
	//先找出指令可能的内存数据修改记录
	instModifiedInfo* imi = m_ccpu->hitModifyInfo(instID);
	if (!imi)
	{
		gLPUpdatetable(&m_table_memacc,0);
		return;
	}
	//根据修改记录分别查找修改内存范围内的内存访问历史记录
	int i = 1;
	for (int i = 0; i < imi->nmem; i++)
	{
		nmemaddr addr = imi->mem[i].addr;
		int		size = imi->mem[i].size;
		chain_link* clink = m_ccpu->hitMemChain(instID,addr,size);
		for (; clink;clink = clink->next)
		{
			//整理并显示到访问窗口当中去
			xvm_memaccess xvmm;
			xvmm.addr = i++;
			xvmm.size = 1;
			xvmm.type = 0;
			xvmm.memptr = clink->addr;
			xvmm.memsize = clink->size;
			xvmm.memvalue = 0;
			xvmm.instID = clink->instID;
			xvmm.mode = (clink->oper & IRI_MEM_ACCESS_READ)?RECORD_FLAG_MEMREAD:RECORD_FLAG_MEMWRITE;
			gLPAddsorteddata(&m_table_memacc.sorted,&xvmm);

		}
	}
	//刷新内存访问窗口
	gLPUpdatetable(&m_table_memacc,0);
}

void xVMDebugFrame::refresh()
{
	flushMemAccess();
}

ChainCPU* xVMDebugFrame::getChainCPU()
{
	return m_ccpu;
}

t_frame* xVMDebugFrame::frame()
{
	return &m_frame;
}


xVMDebugFrame* VMDebug_EnsureTraceFrame()
{
    if (!gXVMFrame)
    {
        gXVMFrame = new xVMDebugFrame();
    }
    gXVMFrame->initFrame(L"alone");
    return gXVMFrame;
}



HWND VMDebug_DebugMDIWindow()
{
	if (gXVMFrame)
		return gXVMFrame->hFrame();
	return 0;
}

int VMDebug_StepOver()
{
	if (VMDebug_DebugMDIWindowIsActive())
	{
		int p = 0;
		p++;
	}

	return -1;
}

BOOL VMDebug_DebugMDIWindowIsActive()
{
	if (!gXVMFrame)
		return FALSE;
	t_table* tcpu = gLPGetcpudisasmtable();
	if (tcpu->hparent)
	{
		HWND hCpuWind = GetParent(tcpu->hparent);
		if (hCpuWind)
		{
			HWND hmdi = (HWND)SendMessageW(hCpuWind,WM_MDIGETACTIVE,0,0);
			if (hmdi)
			{
				if (hmdi == gXVMFrame->hFrame())
					return TRUE;
			}
		}
	}
	return FALSE;
}



