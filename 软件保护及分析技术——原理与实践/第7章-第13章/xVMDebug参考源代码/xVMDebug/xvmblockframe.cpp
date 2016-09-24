#include <windows.h>
#include "xvmblockframe.h"

#include "xvmdebugframe.h"
#include "../../../nCom/nautolargebuffer.h"
#include "../../../nCom/npacketbase.h"
#include "../../../3rdparty/libudis86/extern.h"
#include "../../../nSRK/nSRK_CC/nasm/i386/nasm_i386.h"
#include "xvmdebug.h"
#include "ollydbg2_plugin.h"

xVMBlockFrame* gXVMBlocks = 0;

long xvm_blocks_block_func(struct t_table *table,HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
	xVMBlockFrame* xvm = (xVMBlockFrame*)table->customdata;
	if(!xvm)
		return 0;
    switch(msg)
    {
    case WM_USER_KEY:
    {
        switch(LOBYTE(lparam))
        {
        case VK_DELETE:
        {

        }break;
        case VK_OEM_1:
        {
                xvm->setSelectedBlockComment();
        }break;
		case 'R':xvm->calcBlockExecReference(); break;
		case 'C':xvm->clearSeqsSelection(table); break;
		case 'M':xvm->moniterSelectedEntries(); break;
        }
    }break;
    case WM_USER_DBLCLK:
    {
        seq_info* seq = (seq_info*)gLPGetsortedbyselection(&table->sorted,table->sorted.selected);
        if (seq)
        {
			seq->selected = !seq->selected;
			gLPUpdatetable(table,0);
            //gLPSetcpu(gLPGetcputhreadid(),seq->entry,0,1,0,CPU_NOCREATE);
        }
    }break;
    case WM_DESTROY:
    {
            xvm->reset();
    }break;

    }
    return 0;
}

long xvm_blocks_bklogs_func(struct t_table *table,HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
	xVMBlockFrame* xf = (xVMBlockFrame*)table->customdata;
	if (!xf) return 0;
    switch(msg)
    {
    case WM_USER_KEY:
    {
        switch(LOBYTE(lparam))
        {
        case VK_DELETE:
        {

        }break;
		case 'H':case 'h':
		{
			xf->hideSelectedLogs();
		}break;
        }
    }break;
    case WM_USER_DBLCLK:
    {
        opexec_info* seq = (opexec_info*)gLPGetsortedbyselection(&table->sorted,table->sorted.selected);
        if (seq)
        {
			if (seq->se->flags & SEQEXEC_FLAG_BREAK)
			{
				seq->se->flags &= ~SEQEXEC_FLAG_BREAK;
				xf->setBreakPoint(seq->se->ip,TRUE);
			}
			else
			{
				seq->se->flags |= SEQEXEC_FLAG_BREAK;
				xf->setBreakPoint(seq->se->ip,FALSE);
			}
				
			gLPUpdatetable(table,0);
        }
    }break;
    }
    return 0;
}


long xvm_template_func(struct t_table *table,HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
	xVMBlockFrame* xf = (xVMBlockFrame*)table->customdata;
	if (!xf) return 0;
	switch (msg)
	{
	case WM_USER_KEY:
	{
						//switch (LOBYTE(lparam))
						//{
						//case VK_DELETE:
						//{

						//}break;
						//case 'H':case 'h':
						//{
						//			 xf->hideSelectedLogs();
						//}break;
						//}
	}break;
	case WM_USER_DBLCLK:
	{
		opexec_tpltb* seq = (opexec_tpltb*)gLPGetsortedbyselection(&table->sorted,table->sorted.selected);
		if (seq)
		{
			if (seq->tpl->flags & SEQEXEC_FLAG_BREAK)
			{
				seq->tpl->flags &= ~SEQEXEC_FLAG_BREAK;
				xf->setBreakPoint(seq->tpl->ip,TRUE);
			} else
			{
				seq->tpl->flags |= SEQEXEC_FLAG_BREAK;
				xf->setBreakPoint(seq->tpl->ip,FALSE);
			}

			gLPUpdatetable(table,0);
		}
	}break;
	}
	return 0;
}
int  xvm_blocks_block_draw_func(wchar_t *result,uchar *mask,int *drawFlags,
                                struct t_table *table,t_sorthdr *sorthdr,int reason,void *item)
{
    xVMBlockFrame* xf = (xVMBlockFrame*)table->customdata;
	if (!xf) return 0;
    seq_info* seq = (seq_info*)sorthdr;
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
		return wsprintfW(result,L"%d",seq->addr);
    }break;
	case 1:
	{
			  int flags = 0;
			  *drawFlags = flags;
			  return wsprintfW(result,L"%d",seq->is->seqID);
	}break;
    case 2:
    {
        mask[0] = 0;
        int flags = DRAW_GRAY;
		if (xf && xf->isHilSeq(seq->is->seqID))
            flags = DRAW_EIP|DRAW_EXTSEL;
        *drawFlags = flags;
        int selected = 0;

		gLPLabeladdress(result,seq->is->rip(),0,-1,0,mask,&selected,ADDR_GRAYHEX | ADDR_HILSYM);
        return wcslen(result);
    }break;
    case 3:
    {
        int flags = DRAW_GRAY;
		if (xf && xf->isHilSeq(seq->is->seqID))
            flags = DRAW_EIP|DRAW_EXTSEL;
        *drawFlags = flags;
		return wsprintfW(result,L"%08X",seq->is->outer());
    }break;
    case 4:
    {
        int flags = 0;
        *drawFlags = flags;
		return wsprintfW(result,L"%d",seq->is->insts.GetCount());
    }break;
    case 5:
    {
        int flags = 0;
        *drawFlags = flags;
		return wsprintfW(result,L"%d",seq->is->refc);
    }break;
    case 6:
    {
		result[0] = 0;
		if (seq->is->flags & INSTSEQ_FLAG_HIDDEN)
			wcscat(result,L"H");
		else
			wcscat(result,L"-");
		if (seq->is->flags & INSTSEQ_FLAG_MONITER)
			wcscat(result,L"M");
		else
			wcscat(result,L"-");
		return wcslen(result);
    }break;
	case 7:
	{
			  return wsprintfW(result,L"%d",seq->is->uuid);
	}break;
	case 8:
	{
		wcscpy(result,seq->is->pattern.c_str());
		return wcslen(result);
	}break;
	case 9:
	{
		wcscpy(result,seq->is->comment.c_str());
		return wcslen(result);
	}break;
    default:return 0;
    }


    return 0;
}


int sevar_to_str(SeqExecVar* var,wchar_t *lpstr)
{
	lpstr[0] = 0;
	switch (var->type)
	{
	case sevar_int:return wsprintfW(lpstr,L"%X",var->sqword);
	case sevar_str:return wsprintfW(lpstr,L"%S",var->byte);
	}
	return wcslen(lpstr);
}

int  xvm_template_draw_func(wchar_t *result,uchar *mask,int *drawFlags, struct t_table *table,t_sorthdr *sorthdr,int reason,void *item)
{
	xVMBlockFrame* xf = (xVMBlockFrame*)table->customdata;
	opexec_tpltb* seq = (opexec_tpltb*)sorthdr;
	switch (reason)
	{
	case DF_CACHESIZE:
	return 0;
	case DF_FILLCACHE:
	case DF_FREECACHE:
	return 0;
	case DF_NEWROW:
	return 0;
	case 0:
	{
		int flags = DRAW_GRAY;
		if (seq->tpl->flags & SEQEXEC_FLAG_BREAK)
		{
			flags = DRAW_BREAK | DRAW_EXTSEL;
		} else if (seq->selected)
			flags = DRAW_COND | DRAW_EXTSEL;
		*drawFlags = flags;
		return wsprintfW(result,L"%d",seq->addr);
	}break;
	case 1:
	{
		if (seq->tpl->ip != 0)
		{
			int flags = DRAW_GRAY;
			if (seq->tpl->ip == xf->hilightOIP())
			{
				flags = DRAW_EIP | DRAW_EXTSEL;
			}
			*drawFlags = flags;
			return wsprintfW(result,L"%08X",seq->tpl->ip);
		}
	}break;
	case 2:
	{
			  int flags = DRAW_GRAY;
			  if (xf && xf->isHilSeq(seq->tpl->seqid))
				  flags = DRAW_EIP | DRAW_EXTSEL;
			  *drawFlags = flags;
			  int selected = 0;
			  gLPLabeladdress(result,seq->tpl->entry,0,-1,0,mask,&selected,ADDR_GRAYHEX | ADDR_HILSYM);
			  return wcslen(result);
	}break;
	case 3:
	{
			  int flags = DRAW_GRAY;
			  *drawFlags = flags;
			  return wsprintfW(result,L"%d",seq->tpl->uuid);
	}break;

	case 4:
	{
			  InstSequence* rs = xf->findSeq(seq->tpl->seqid);
			  if (rs)
			  {
				  wcscpy(result,rs->comment.c_str());
				  return wcslen(result);
			  }
			  return 0;
	}break;
	}
	return 0;
}
int  xvm_blocks_bklogs_draw_func(wchar_t *result,uchar *mask,int *drawFlags,
                                 struct t_table *table,t_sorthdr *sorthdr,int reason,void *item)
{
    xVMBlockFrame* xf = (xVMBlockFrame*)table->customdata;
    opexec_info* seq = (opexec_info*)sorthdr;
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
		int flags = DRAW_GRAY;
		if (seq->se->flags & SEQEXEC_FLAG_BREAK)
		{
			flags = DRAW_BREAK | DRAW_EXTSEL;
		}else if (seq->selected)
			flags = DRAW_COND | DRAW_EXTSEL;
		*drawFlags = flags;
		return wsprintfW(result,L"%d",seq->addr);
	}break;
	case 1:
	{
		 if (seq->se->ip != 0)
		{
			int flags = DRAW_GRAY;
			if (seq->se->ip == xf->hilightOIP())
				flags = DRAW_EIP | DRAW_EXTSEL;
			*drawFlags = flags;
			return wsprintfW(result,L"%08X",seq->se->ip);
		}
	}break;
	case 2:
	{
		int flags = DRAW_GRAY;
		//if (xf && xf->isHilSeq(seq->seqID))
			// flags = DRAW_EIP | DRAW_EXTSEL;
		*drawFlags = flags;
		return wsprintfW(result,L"%d",seq->se->seqid);
	}break;
    case 3:
    {
        int flags = DRAW_GRAY;
		if (xf && xf->isHilSeq(seq->se->seqid))
            flags =  DRAW_EIP|DRAW_EXTSEL;
        *drawFlags = flags;
        int selected = 0;
        gLPLabeladdress(result,seq->entry,0,-1,0,mask,&selected,ADDR_GRAYHEX|ADDR_HILSYM);
        return wcslen(result);
    }break;

	case 4:
	{
		result[0] = 0;
		if (seq->se->flags & SEQEXEC_FLAG_HIDDEN)
			wcscat(result,L"H");
		else
			wcscat(result,L"-");
		return wcslen(result);
	}break;
    case 5:
    {
        *drawFlags = 0;
		InstSequence* rs = xf->findSeq(seq->se->seqid);
		if (rs)
		{
			SequenceExecute* se = seq->se;
			wchar_t	vs[MAX_VAR_NUM][64];
			int narg = 0;
			for (; narg < MAX_VAR_NUM;narg++)
			{
				if (se->vars[narg].type == sevar_none) break;
				sevar_to_str(&se->vars[narg],vs[narg]);
			}

			switch (narg)
			{
			case 1: return wsprintfW(result,rs->pattern.c_str(),vs[0]);
			case 2: return wsprintfW(result,rs->pattern.c_str(),vs[0],vs[1]);
			case 3:return wsprintfW(result,rs->pattern.c_str(),vs[0],vs[1],vs[2]);
			case 4:return wsprintfW(result,rs->pattern.c_str(),vs[0],vs[1],vs[2],vs[3]);
			default:wcscpy(result,rs->pattern.c_str()); break;
			}
			
			return wcslen(result);
		}
        wcscpy(result,L"BAD Sequence");
        return wcslen(result);
    }break;
    case 6:
    {
        *drawFlags = DRAW_GRAY;
		SequenceExecute* se = seq->se;
        return wsprintfW(result,L"EAX=%08X ECX=%08X EDX=%08X EBX=%08X ESP=%08X EBP=%08X ESI=%08X EDI=%08X EFLAG=%08X",
			se->regs[0],
			se->regs[1],
			se->regs[2],
			se->regs[3],
			se->regs[4],
			se->regs[5],
			se->regs[6],
			se->regs[7],
			se->eflags
                         );
    }break;
    default:return 0;
    }


    return 0;
}

void xvm_blocks_block_sel_func(struct t_table *pt,int selected,int reason)
{
    seq_info* seq = (seq_info*)gLPGetsortedbyselection(&pt->sorted,selected);
    if (!seq)
        return;
    xVMBlockFrame* xf = (xVMBlockFrame*)pt->customdata;
    if (!xf)
        return;
	if (HIBYTE(GetKeyState(VK_SHIFT)))
	{
		for (int i = 0; i < pt->sorted.n; i++)
		{
			seq_info* xrt = (seq_info*)gLPGetsortedbyindex(&pt->sorted,i);
			if (xrt)
				xrt->selected = FALSE;
		}
		int lastsel = xf->lastsel();
		if (lastsel != -1)
		{
			int minl = min(lastsel,selected);
			int maxl = max(lastsel,selected);

			for (int i = minl; i < maxl + 1; i++)
			{
				seq_info* xrt = (seq_info*)gLPGetsortedbyindex(&pt->sorted,i);
				if (xrt)
					xrt->selected = TRUE;
			}
		}
	} else if (HIBYTE(GetKeyState(VK_CONTROL)))
	{
		seq->selected = !seq->selected;
	}

	xf->setLastsel(selected);
	xf->flushSeq(seq->is->seqID);
	if (HIBYTE(GetKeyState(VK_MENU)))
	{
		gLPSetcpu(gLPGetcputhreadid(),seq->is->rip(),0,1,0,CPU_NOCREATE);
	}
}


void xvm_blocks_bklogs_sel_func(struct t_table *pt,int selected,int reason)
{
	xVMBlockFrame* xf = (xVMBlockFrame*)pt->customdata;
	if (!xf)
		return;
	opexec_info* seq = (opexec_info*)gLPGetsortedbyselection(&pt->sorted,selected);
	if (!seq)
		return;
	if (HIBYTE(GetKeyState(VK_MENU)))
	{
		xf->syncSequenceSeqID(seq->se->seqid);
	}
	if (HIBYTE(GetKeyState(VK_CONTROL)))
	{
		opexec_info* seq = (opexec_info*)gLPGetsortedbyselection(&pt->sorted,pt->sorted.selected);
		if (seq)
		{
			seq->selected = !seq->selected;
			gLPUpdatetable(pt,0);
		}
	}
}


enum xvm_blockexec_menus{
    xvm_blockexec_menu_load,
    xvm_blockexec_menu_save,
    xvm_blockexec_menu_analyserrecords,
    xvm_blockexec_menu_stepover,
    xvm_blockexec_menu_comment,
	xvm_blockexec_menu_app,
	xvm_blockexec_menu_syncbackrcs,
	xvm_blockexec_menu_filter,
	xvm_blockexec_menu_selbklogs,
	xvm_blockexec_menu_exportext,
	xvm_blockexec_menu_pattern,
	xvm_blockexec_menu_dna,
	xvm_blockexec_menu_setuuid,
	xvm_blockexec_menu_clearlogs,
	xvm_blockexec_menu_template,
	xvm_blockexec_menu_setpattern
};


enum xvm_bklogs_fmenu{
	xvm_bklogs_fshowall,
	xvm_bklogs_fshowhidden
};
static int Log_Fileter_MenuProc(t_table *pt,wchar_t *name,ulong index,int mode)
{
	xVMBlockFrame* fm = (xVMBlockFrame*)pt->customdata;
	if (!fm) return 0;
	if (mode == MENU_VERIFY)
	{
		switch (index)
		{
		case xvm_bklogs_fshowhidden:return (gxvmcfg.bkopt.filter & bkopt_flags_showhidden) ? MENU_CHECKED : MENU_NORMAL; break;
		}
	} else if (mode == MENU_EXECUTE)
	{
		switch (index)
		{
		case xvm_bklogs_fshowhidden:{
			if (gxvmcfg.bkopt.filter & bkopt_flags_showhidden)
				gxvmcfg.bkopt.filter &= ~bkopt_flags_showhidden;
			else
				gxvmcfg.bkopt.filter |= bkopt_flags_showhidden;
			fm->flushExecLogs();
		}break;
		}
	}
	return MENU_NORMAL;
}
static int Block_Table_MenuProc(t_table *pt,wchar_t *name,ulong index,int mode) {
    if (mode==MENU_VERIFY)
        return MENU_NORMAL;                // Always available
    if (mode==MENU_EXECUTE)
    {
		xVMBlockFrame* fm = (xVMBlockFrame*)pt->customdata;
		if (!fm) return MENU_ABSENT;
        switch(index)
        {
        case xvm_blockexec_menu_load:
        {
            TCHAR szFilter[] =_T("*.xblock\0*.xblock\0*.*\0*.*\0");
            TCHAR lpFilename[256] ={0};
            OPENFILENAME  lpofn={0};
            DWORD len=0;
            lpofn.lStructSize = sizeof (lpofn);
            lpofn.lpstrFilter = szFilter;
            lpofn.hwndOwner = pt->hparent;
            lpofn.hInstance = GetModuleHandleA(NULL);
            lpofn.lpstrFile = lpFilename;
            lpofn.Flags = OFN_HIDEREADONLY;
            lpofn.lpstrTitle = _T("Select Blocks File to Load");
            lpofn.nMaxFile = 0x256;
            if(!GetOpenFileName(&lpofn) || (len=_tcslen(lpFilename))==0)
                return MENU_NOREDRAW;
            fm->loadBlocks(lpFilename);
        }break;
        case xvm_blockexec_menu_save:
        {
            TCHAR szFilter[] =_T("*.xblock\0*.xblock\0*.*\0*.*\0");
            TCHAR lpFilename[256] ={0};
            OPENFILENAME  lpofn={0};
            DWORD len=0;
            lpofn.lStructSize = sizeof (lpofn);
            lpofn.lpstrFilter = szFilter;
            lpofn.hwndOwner = pt->hparent;
            lpofn.hInstance = GetModuleHandleA(NULL);
            lpofn.lpstrFile = lpFilename;
            lpofn.Flags = OFN_HIDEREADONLY;
            lpofn.lpstrTitle = _T("Select Blocks File to Load");
            lpofn.nMaxFile = 0x256;
            lpofn.lpstrDefExt = _T(".xblock");
            if(!GetSaveFileName(&lpofn) || (len=_tcslen(lpFilename))==0)
                return MENU_NOREDRAW;
            fm->saveBlocks(lpFilename);
        }break;
		case xvm_blockexec_menu_exportext:
		{
            TCHAR szFilter[] =_T("*.txt\0*.txt\0*.*\0*.*\0");
            TCHAR lpFilename[256] ={0};
            OPENFILENAME  lpofn={0};
            DWORD len=0;
            lpofn.lStructSize = sizeof (lpofn);
            lpofn.lpstrFilter = szFilter;
            lpofn.hwndOwner = pt->hparent;
            lpofn.hInstance = GetModuleHandleA(NULL);
            lpofn.lpstrFile = lpFilename;
            lpofn.Flags = OFN_HIDEREADONLY;
            lpofn.lpstrTitle = _T("Export To Text File");
            lpofn.nMaxFile = 0x256;
            lpofn.lpstrDefExt = _T(".txt");
            if(!GetSaveFileName(&lpofn) || (len=_tcslen(lpFilename))==0)
                return MENU_NOREDRAW;
            fm->exportExecLogsToFile(lpFilename);
		}break;
        case xvm_blockexec_menu_analyserrecords:
        {
            TCHAR szFilter[] =_T("*.xtrace\0*.xtrace\0*.*\0*.*\0");
            TCHAR lpFilename[256] ={0};
            OPENFILENAME  lpofn={0};
            DWORD len=0;
            lpofn.lStructSize = sizeof (lpofn);
            lpofn.lpstrFilter = szFilter;
            lpofn.hwndOwner = pt->hparent;
            lpofn.hInstance = GetModuleHandleA(NULL);
            lpofn.lpstrFile = lpFilename;
            lpofn.Flags = OFN_HIDEREADONLY;
            lpofn.lpstrTitle = _T("Select Trace Records File to Analyser");
            lpofn.nMaxFile = 0x256;
            if(!GetOpenFileName(&lpofn) || (len=_tcslen(lpFilename))==0)
                return MENU_NOREDRAW;
            fm->analyserTraceRecords(lpFilename);
        }break;
        case xvm_blockexec_menu_stepover:
        {
            fm->stepOverBlock();
        }break;
        case xvm_blockexec_menu_comment:
        {
            fm->setSelectedBlockComment();
        }break;
		case xvm_blockexec_menu_setpattern:
		{
			fm->setSelectedBlockPattern();
		}break;
		case xvm_blockexec_menu_setuuid:fm->setSelectedBlockUUID(); break;
		case xvm_blockexec_menu_syncbackrcs:
		{
			seq_info* si = (seq_info*)gLPGetsortedbyselection(&pt->sorted,pt->sorted.selected);
			if (si)
			{
				InstSequence*  ir = fm->syncSequenceBack(si->is->seqID);
				if (!ir)
					gLPFlash(L"Sync Sequence Faild!");
				else
				{
					gLPFlash(L"Sequence Synchronized.");
					gLPUpdatetable(pt,0);
				}		
			}
		}break;
		case xvm_blockexec_menu_selbklogs:
		{
			seq_info* si = (seq_info*)gLPGetsortedbyselection(&pt->sorted,pt->sorted.selected);
			if (si)
			{
				fm->selectLogsBySeqID(si->is->seqID);
			}
		}break;
		case xvm_blockexec_menu_clearlogs:
		{
			fm->clearSeqExecLogs();
		}break;
		case xvm_blockexec_menu_pattern:
		{
			TCHAR lpFile[MAX_PATH];
			if (getPasmFile(lpFile,pt->hparent) > 0)
				fm->runPatternAsmOnAllBlocks(lpFile);
		}break;
		case xvm_blockexec_menu_dna:
		{
			TCHAR lpFile[MAX_PATH];
			if (getPasmFile(lpFile,pt->hparent) > 0)
				fm->runCodeDNAOnAllBlocks(lpFile);
		}break;
		case xvm_blockexec_menu_template:fm->exportExecLogsToTemplate(); break;

        }
        return MENU_NOREDRAW;
    };
    return MENU_ABSENT;
}


xVMBlockFrame::xVMBlockFrame()
    :m_hFrame(0)
    ,m_unqID(0)
    ,m_skipNext(FALSE)
	,m_lastsel(-1)
	,m_dbgmode(dbg_byvm)
	,m_uniqueID(0)
{
    m_vcpu_rcs.init_r3();
}

void xVMBlockFrame::initFrame()
{
    if (m_hFrame)
        return;
	m_vcpu_rcs.initFrame(L"BLOCK",TRUE,TRUE);

    wcscpy(m_frame.name,L"xVM_Blocks");

    memset(&m_table_blocks,0,sizeof(m_table_blocks));
    wcscpy(m_table_blocks.name,L"xvm_block_exec");
    m_table_blocks.mode = TABLE_SYNTAX  | TABLE_SAVEALL;
    m_table_blocks.drawfunc = xvm_blocks_block_draw_func;
    m_table_blocks.tableselfunc = xvm_blocks_block_sel_func;
    m_table_blocks.tabfunc = xvm_blocks_block_func;
    m_table_blocks.customdata = this;
    m_table_blocks.bar.nbar = 10;
    m_table_blocks.bar.visible = 1;

    m_table_blocks.bar.name[0] = L"NO.";
    m_table_blocks.bar.expl[0] = L"No.";
    m_table_blocks.bar.defdx[0] = 5;
    //m_table_blocks.bar.mode[0] = BAR_DISABLED;

	m_table_blocks.bar.name[1] = L"SeqID";
	m_table_blocks.bar.expl[1] = L"SeqID";
	m_table_blocks.bar.defdx[1] = 5;

	m_table_blocks.bar.name[2] = L"entry";
	m_table_blocks.bar.expl[2] = L"entry";
	m_table_blocks.bar.defdx[2] = 10;

	m_table_blocks.bar.name[3] = L"outer";
	m_table_blocks.bar.expl[3] = L"outer";

    m_table_blocks.bar.name[4] = L"size";
    m_table_blocks.bar.expl[4] = L"size";
    m_table_blocks.bar.defdx[4] = 5;

    m_table_blocks.bar.name[5] = L"refc.";
    m_table_blocks.bar.expl[5] = L"refc.";
    m_table_blocks.bar.defdx[5] = 5;

	m_table_blocks.bar.name[6] = L"attrib.";
	m_table_blocks.bar.expl[6] = L"attrib.";
	m_table_blocks.bar.defdx[6] = 8;

	m_table_blocks.bar.name[7] = L"UUID";
	m_table_blocks.bar.expl[7] = L"UUID";
	m_table_blocks.bar.defdx[7] = 5;

    m_table_blocks.bar.name[8] = L"Pattern";
    m_table_blocks.bar.expl[8] = L"Pattern";
    m_table_blocks.bar.defdx[8] = 30;

	m_table_blocks.bar.name[9] = L"Comment";
	m_table_blocks.bar.expl[9] = L"Comment";
	m_table_blocks.bar.defdx[9] = 30;

    static t_menu blocks_menu[] = {
		{ L"Select This Block's Logs",L"Select This Block's Logs",K_NONE,Block_Table_MenuProc,NULL,xvm_blockexec_menu_selbklogs },
        { L"Comment This Block...",L"Comment This Block...",K_NONE, Block_Table_MenuProc, NULL, xvm_blockexec_menu_comment},
		{ L"Set UUID...",L"Set UUID of this block",K_NONE,Block_Table_MenuProc,NULL,xvm_blockexec_menu_setuuid },
		{ L"Set Pattern...",L"Set Pattern...",K_NONE,Block_Table_MenuProc,NULL,xvm_blockexec_menu_setpattern },
		{ L"Sync Back Records",L"Sync Records From Records Table",K_NONE,Block_Table_MenuProc,NULL,xvm_blockexec_menu_syncbackrcs },

        { L"|Load Blocks...",L"Load Blocks", K_NONE, Block_Table_MenuProc, NULL, xvm_blockexec_menu_load},
        { L"Save Blocks...",L"Save Blocks", K_NONE, Block_Table_MenuProc, NULL, xvm_blockexec_menu_save},
		{ L"|Run PatternASM Script",L"Run PatternASM Script On All Blocks",K_NONE,Block_Table_MenuProc,NULL,xvm_blockexec_menu_pattern },
		{ L"Run codeDNA Script",L"Run codeDNA Script On All Blocks",K_NONE,Block_Table_MenuProc,NULL,xvm_blockexec_menu_dna },
        { L"Analyser Trace Records",L"Analyser Trace Records", K_NONE, Block_Table_MenuProc, NULL, xvm_blockexec_menu_analyserrecords},
		{ L"|Appearance",L"Appearance",K_NONE,Block_Table_MenuProc,appearanceMenu,xvm_blockexec_menu_app },

        { NULL, NULL, K_NONE, NULL, NULL, 0 }
    };

    m_table_blocks.menu = blocks_menu;
    m_table_blocks.customdata = this;

    memset(&m_table_bklogs,0,sizeof(m_table_bklogs));
    wcscpy(m_table_bklogs.name,L"xvm_block_logs");
	m_table_bklogs.mode = TABLE_SYNTAX | TABLE_SAVEALL ;
    m_table_bklogs.drawfunc = xvm_blocks_bklogs_draw_func;
    m_table_bklogs.tableselfunc = xvm_blocks_bklogs_sel_func;
    m_table_bklogs.tabfunc = xvm_blocks_bklogs_func;
    m_table_bklogs.customdata = this;
    m_table_bklogs.bar.nbar = 7;
    m_table_bklogs.bar.visible = 1;

	m_table_bklogs.bar.name[0] = L"index";
	m_table_bklogs.bar.expl[0] = L"index";
	m_table_bklogs.bar.defdx[0] = 6;

	m_table_bklogs.bar.name[1] = L"OIP";
	m_table_bklogs.bar.expl[1] = L"OIP";
	m_table_bklogs.bar.defdx[1] = 10;

	m_table_bklogs.bar.name[2] = L"seqID";
	m_table_bklogs.bar.expl[2] = L"seqID";
	m_table_bklogs.bar.defdx[2] = 6;

    m_table_bklogs.bar.name[3] = L"entry";
    m_table_bklogs.bar.expl[3] = L"entry";
    m_table_bklogs.bar.defdx[3] = 10;


	m_table_bklogs.bar.name[4] = L"attrib.";
	m_table_bklogs.bar.expl[4] = L"attrib.";
	m_table_bklogs.bar.defdx[4] = 8;


    m_table_bklogs.bar.name[5] = L"Command";
    m_table_bklogs.bar.expl[5] = L"Command";
    m_table_bklogs.bar.defdx[5] = 30;

    m_table_bklogs.bar.name[6] = L"Context";
    m_table_bklogs.bar.expl[6] = L"Context";
    m_table_bklogs.bar.defdx[6] = 150;

	static t_menu bklogs_filtermenu[] = {
		{ L"Show All",L"Show All",K_NONE,Log_Fileter_MenuProc,NULL,xvm_bklogs_fshowall },
		{ L"|Show Hidden",L"Show Hidden",K_NONE,Log_Fileter_MenuProc,NULL,xvm_bklogs_fshowhidden },
		{ NULL,NULL,K_NONE,NULL,NULL,0 }
	};
    static t_menu bklogs_menu[] = {
		{ L"Clear Logs...",L"Clear Logs...",K_NONE,Block_Table_MenuProc,NULL,xvm_blockexec_menu_clearlogs },

		{ L"|Filter",L"Filter Records",K_NONE,Block_Table_MenuProc,bklogs_filtermenu,xvm_blockexec_menu_filter },
		{ L"|Export To Text...",L"Export To Text File",K_NONE,Block_Table_MenuProc,NULL,xvm_blockexec_menu_exportext },
		{ L"Export To Template",L"Export To Template",K_NONE,Block_Table_MenuProc,NULL,xvm_blockexec_menu_template },
		{ L"|Appearance",L"Appearance",K_NONE,Block_Table_MenuProc,appearanceMenu,xvm_blockexec_menu_app },

        { NULL, NULL, K_NONE, NULL, NULL, 0 }
    };

    m_table_bklogs.menu = bklogs_menu;
    m_table_bklogs.customdata = this;

    gLPDestroysorteddata(&m_table_bklogs.sorted);
    gLPCreatesorteddata(&m_table_bklogs.sorted,sizeof(opexec_info),300,0,0,0);



	memset(&m_table_template,0,sizeof(m_table_template));
	wcscpy(m_table_template.name,L"xvm_block_simple");
	m_table_template.mode = TABLE_SYNTAX | TABLE_SAVEALL;
	m_table_template.drawfunc = xvm_template_draw_func;
	//m_table_simple.tableselfunc = xvm_blocks_bklogs_sel_func;
	m_table_template.tabfunc = xvm_template_func;
	m_table_template.customdata = this;
	m_table_template.bar.nbar = 5;
	m_table_template.bar.visible = 1;

	m_table_template.bar.name[0] = L"index";
	m_table_template.bar.expl[0] = L"index";
	m_table_template.bar.defdx[0] = 6;

	m_table_template.bar.name[1] = L"OIP";
	m_table_template.bar.expl[1] = L"OIP";
	m_table_template.bar.defdx[1] = 10;

	m_table_template.bar.name[2] = L"entry";
	m_table_template.bar.expl[2] = L"entry";
	m_table_template.bar.defdx[2] = 10;

	m_table_template.bar.name[3] = L"UUID";
	m_table_template.bar.expl[3] = L"UUID";
	m_table_template.bar.defdx[3] = 6;

	m_table_template.bar.name[4] = L"Comments";
	m_table_template.bar.expl[4] = L"Comments";
	m_table_template.bar.defdx[4] = 30;

	static t_menu bktpl_menu[] = {
		{ L"Appearance",L"Appearance",K_NONE,Block_Table_MenuProc,appearanceMenu,xvm_blockexec_menu_app },

		{ NULL,NULL,K_NONE,NULL,NULL,0 }
	};
	m_table_template.menu = bktpl_menu;

    memset(&m_block_bklogs,0,sizeof(m_block_bklogs));
    m_block_bklogs.type = BLK_TABLE;
    m_block_bklogs.table = &m_table_bklogs;

    memset(&m_block_blocks,0,sizeof(m_block_blocks));
    m_block_blocks.type = BLK_TABLE;
    m_block_blocks.table = &m_table_blocks;


    memset(&m_block_left,0,sizeof(m_block_left));
    m_block_left.type = BLK_HDIV;
    m_block_left.blk1 = &m_block_bklogs;
    m_block_left.blk2 = &m_block_blocks;
    m_block_left.percent = 70;


	memset(&m_block_template,0,sizeof(m_block_template));
	m_block_template.type = BLK_TABLE;
	m_block_template.table = &m_table_template;

	memset(&m_block_right,0,sizeof(m_block_right));
	m_block_right.type = BLK_HDIV;
	m_block_right.blk1 = &m_block_template;
	m_block_right.blk2 = m_vcpu_rcs.block();
	m_block_right.percent = 40;

    memset(&m_block_main,0,sizeof(m_block_main));
    m_block_main.type = BLK_VDIV;
    m_block_main.blk1 = &m_block_left;
	m_block_main.blk2 = &m_block_right;
    m_block_main.percent = 70;

    memset(&m_frame,0,sizeof(m_frame));
    m_frame.block = &m_block_main;
    m_frame.mode = TABLE_SYNTAX  | TABLE_SAVEALL;

    //m_frame.scheme = 1;
    m_hFrame = glpCreateframewindow(&m_frame,0,L"XVM_Block_Exec");

}

void xVMBlockFrame::reset()
{
    m_hFrame = 0;
	m_uniqueID = 0;
}

int xVMBlockFrame::saveBlocks(wchar_t *lpPath)
{
    NAutoLargeBufferA nla;
    nla.setHostFile(lpPath);
    nla.setMaxmemsize(0x100000);
    nla.ensureEmptySpace(0x100000);
    if (!nla.open(false,true))
        return -1;
    nla.resize(0);
    NPacketBase pk;
    NPacketBase pkseq;
	for (InstSequence* iseq = m_seqs.first(); iseq; iseq = m_seqs.next(iseq))
	{
		pkseq.setempty(false);
		pkseq.putInt(block_vid_refc,iseq->refc);
		pkseq.putInt(block_vid_seqid,iseq->seqID);
		pkseq.putInt(block_vid_flags,iseq->flags);
		pkseq.putInt(block_vid_uuid,iseq->uuid);
		pkseq.putInt64(block_vid_entry,iseq->entry);
		pkseq.putWString(block_vid_comment,iseq->comment.c_str());
		pkseq.putWString(block_vid_pattern,iseq->pattern.c_str());

		for (TraceRecordEx* tre = iseq->insts.first(); tre; tre = iseq->insts.next(tre))
		{
			pkseq.putBinary(block_vid_record,(const char*)tre,sizeof(TraceRecordEx));
		}
		pk.putBinary(block_vid_sequence,pkseq.data(),pkseq.size());
	}
	for (SequenceExecute* se = m_sseq.first(); se; se = m_sseq.next(se))
	{
		pk.putBinary(block_vid_seqexec,(const char*)se,sizeof(SequenceExecute));
	}


	for (opexec_tpl* tpl = m_setpl.first(); tpl;tpl = m_setpl.next(tpl))
	{
		pk.putBinary(block_vid_tpl,(const char*)tpl,sizeof(opexec_tpl));
	}

    nla.append(pk.data(),pk.size());
    nla.flush(true);
    nla.close();
    return 0;
}
int xVMBlockFrame::loadBlocks(wchar_t *lpPath)
{
    NAutoLargeBufferA nla;
    nla.load(lpPath);
    int szData = 0;
    const char* lpData = nla.data(szData,0,false);
    if (!lpData || szData < 1)
        return -1;
    NPacketBase pk(lpData,szData,false);
	return loadBlocks(&pk);
}

int xVMBlockFrame::loadBlocks(NPacketBase* pk)
{
	m_seqs.clear();
	m_instIndex.clear();
	m_sseq.clear();
	m_setpl.cleanup();
	m_bps.cleanup();
	char* lpnode = pk->tofirst();
	while (lpnode)
	{
		switch (pk->vid())
		{
		case block_vid_sequence:
		{
			NPacketBase pkseq(lpnode,pk->szblock(),false);
			InstSequence* seq = new InstSequence;
			seq->seqID = pkseq.getInt(block_vid_seqid,-1);
			seq->refc = pkseq.getInt(block_vid_refc,0);
			seq->flags = pkseq.getInt(block_vid_flags,0);
			seq->entry = pkseq.getLongLong(block_vid_entry);
			seq->pattern = pkseq.getWString(block_vid_pattern);
			seq->comment = pkseq.getWString(block_vid_comment);
			seq->uuid = pkseq.getInt(block_vid_uuid,-1);
			char* lptr = pkseq.tofirst();
			while (lptr)
			{
				if (pkseq.vid() == block_vid_record)
				{
					if (pkseq.szblock() == sizeof(TraceRecordEx))
						seq->insts.push_back(*((TraceRecordEx*)lptr));
				}
				lptr = pkseq.tonext();
			}
			loadSequence(seq);
		}break;
		case block_vid_seqexec:
		{
			m_sseq.push_back(m_sseq.count(),*(SequenceExecute*)pk->block());
		}break;
		case block_vid_tpl:
		{
			opexec_tpl* tpl =(opexec_tpl*)pk->block();
			opexec_tpl* tpnew = m_setpl.new_back(tpl->ip);
			*tpnew = *tpl;
		}break;
		}
		lpnode = pk->tonext();
	}
	flushSeqs();
	flushExecLogs(&m_sseq);
	flushExecTemplate();
	return 0;
}

int xVMBlockFrame::analyserTraceRecords(wchar_t *lpPath)
{
	m_sseq.cleanup();
	m_seqs.cleanup();
	m_instIndex.clear();
	NInstSequences seqs;
	xHook_VMExec::DetectCodeBlockFromFile(lpPath,&seqs,&m_sseq);
	for (InstSequence* seq =  seqs.first(); seq; seq = seqs.next(seq))
		loadSequence(seq);
	flushSeqs();
	flushExecLogs(&m_sseq);
	return 0;
}

int xVMBlockFrame::analyserTraceRecords(NTraceRecords* rcs)
{
	if (rcs->GetCount() > 0)
	{
		NInstSequences seqs;
		xHook_VMExec::DetectCodeBlock(rcs,&seqs,&m_sseq);
		for (InstSequence* seq =  seqs.first(); seq; seq = seqs.next(seq))
			loadSequence(seq);
		flushSeqs();
		flushExecLogs(&m_sseq);
	}
	return 0;
}

void xVMBlockFrame::syncByRip(ULONG_PTR rip)
{
    if (!m_hFrame)
        return;
    if (m_skipNext)
    {
        m_skipNext = FALSE;
        return;
    }
    m_hilSeqs.clear();
    m_lastRip = rip;
    ULONG_PTR lastRip = 0;
    int firsteq = -1;
    SequencesIndex::iterator itr = m_instIndex.find(rip);
    if (itr != m_instIndex.end())
    {
        lastRip = itr->first;
        for (;itr != m_instIndex.end();itr++)
        {
            if (itr->first != lastRip)
                break;
            int seqID = itr->second;
            if (firsteq == -1)
                firsteq = seqID;
            m_hilSeqs.push_back(seqID);
        }
    }

    if (firsteq != -1)
    {
        for (int i=0;i<m_table_blocks.sorted.n;i++)
        {
            seq_info* xrt = (seq_info*)gLPGetsortedbyindex(&m_table_blocks.sorted,i);
            if (xrt)
            {
				if (xrt->is->seqID == firsteq)
                {
                    gLPSettableselection(&m_table_blocks,i);
                    break;
                }
            }
        }
    }
	m_vcpu_rcs.syncByRip(rip);
}

void xVMBlockFrame::flushSeqs()
{
    gLPDestroysorteddata(&m_table_blocks.sorted);
    gLPCreatesorteddata(&m_table_blocks.sorted,sizeof(seq_info),1000,0,0,SDM_EXTADDR);
	int idx = 1;

	for (InstSequence* rseq = m_seqs.first(); rseq; rseq = m_seqs.next(rseq))
	{
		seq_info seq;
		seq.addr = idx++;
		seq.size = 1;
		seq.type = 0;
		seq.selected = FALSE;
		seq.is = rseq;
		gLPAddsorteddata(&m_table_blocks.sorted,&seq);
	}

    gLPUpdatetable(&m_table_blocks,1);
}

void xVMBlockFrame::flushSeq(int seqID)
{
	InstSequence* seqT = m_vcpu_rcs.Sequence();
	InstSequence* seq = m_seqs.find(seqID);
	if (seq)
	{

		*seqT = *seq;
		m_vcpu_rcs.flushSeq();
	}
}

BOOL xVMBlockFrame::isHilSeq(int seqID)
{
    int nc = m_hilSeqs.size();
    for (int i=0;i<nc;i++)
    {
        if (m_hilSeqs[i] == seqID)
            return TRUE;
    }
    return FALSE;
}

void xVMBlockFrame::processDebugEvent(DEBUG_EVENT *ev)
{
    switch(ev->dwDebugEventCode)
    {
    case EXCEPTION_DEBUG_EVENT:
    {
        LPEXCEPTION_DEBUG_INFO except = &ev->u.Exception;
        processExceptionEvent(ev->dwThreadId,except);
    }break;
    }
}
//这里我们具体的处理异常
void xVMBlockFrame::processExceptionEvent(DWORD dwThreadId, LPEXCEPTION_DEBUG_INFO ex)
{
    if (!ex->dwFirstChance)
        return;
    //我们只关注断点异常
    if (ex->ExceptionRecord.ExceptionCode == 0x80000004
            || ex->ExceptionRecord.ExceptionCode == 0x80000003)
    {
        int nc = m_brIPs.size();
        for (int i=0;i<nc;i++)
        {
            //如果异常地址在我们的监控内，那么我们交由blockPreExecute函数处理
            if (m_brIPs[i] == (ULONG_PTR)ex->ExceptionRecord.ExceptionAddress)
            {
                blockPreExecute(dwThreadId,(ULONG_PTR)ex->ExceptionRecord.ExceptionAddress);
                break;
            }
        }
    }
}

//步过代码块
void xVMBlockFrame::stepOverBlock()
{
    //首先我们取出所有包含当前EIP的代码块
    //这个列表是在我们同步更新时生成的
    int nc = m_hilSeqs.size();
    if (nc <1) return;
    int nb = m_shotBPs.size();
    //删除所有上一次块步过设定的断点
    for (int i=0;i<nb;i++)
        gLPRemoveint3breakpoint(m_shotBPs[i],BP_ONESHOT|BP_EXEC|BP_BREAK);
    m_shotBPs.clear();  //清楚断点列表先
    for (int i=0;i < nc;i++)
    {
        int seqID = m_hilSeqs[i];
		InstSequence* seq = m_seqs.find(seqID);
		if (seq && seq->insts.size() > 0)
		{
			ULONG_PTR bpaddr = seq->insts.back()->ctx1.rip;
			//我们在每一个包涵EIP的代码块出口上设定断点，我们这里设定一次性执行断点
			//我们应当保存这些断点的记录，在程式下次中断时全部删除
			wchar_t nullstr[1];
			nullstr[0] = 0;
			int ibp = gLPSetint3breakpoint(bpaddr,BP_ONESHOT | BP_EXEC | BP_BREAK,0,0,0,0,nullstr,nullstr,nullstr);
			if (ibp == 0)
			{
				gLPEnableint3breakpoint(bpaddr,TRUE);
				m_shotBPs.push_back(bpaddr);
			}
		}
      
    }
    if (m_shotBPs.size() > 0)
		glpRunOrg(STAT_RUNNING,1);
}

void xVMBlockFrame::setSelectedBlockComment()
{
    seq_info* seq = (seq_info*)gLPGetsortedbyselection(&m_table_blocks.sorted,m_table_blocks.sorted.selected);
    if (seq)
    {
        POINT pt = {0};
        GetCursorPos(&pt);
        //        stdapi (int)     Getstring(HWND hparent,wchar_t *title,wchar_t *s,int length,
        //                           int savetype,int letter,int x,int y,int fi,int mode);
        wchar_t cmt[100];
		wsprintf(cmt,L"Enter Comment For Block %08X",seq->is->rip());
        wchar_t comment[TEXTLEN];
        comment[0] = 0;
        wcscpy(comment,seq->is->comment.c_str());
        int nret = gLPGetstring(m_table_blocks.hparent,cmt,comment,sizeof(comment)/sizeof(wchar_t)-1,0,0,pt.x,pt.y,0,0);
        if (nret >= 0)
        {
			seq->is->comment = comment;
            gLPUpdatetable(&m_table_blocks,0);
			gLPUpdatetable(&m_table_bklogs,0);
			gLPUpdatetable(&m_table_template,0);
        }
    }

}


void xVMBlockFrame::setSelectedBlockUUID()
{
	seq_info* seq = (seq_info*)gLPGetsortedbyselection(&m_table_blocks.sorted,m_table_blocks.sorted.selected);
	if (seq)
	{
		POINT pt = { 0 };
		GetCursorPos(&pt);
		//        stdapi (int)     Getstring(HWND hparent,wchar_t *title,wchar_t *s,int length,
		//                           int savetype,int letter,int x,int y,int fi,int mode);
		wchar_t cmt[100];
		wsprintf(cmt,L"Enter UUID For Block %08X",seq->is->rip());
		wchar_t suuid[TEXTLEN];
		suuid[0] = 0;
		wsprintf(suuid,L"%d",seq->is->uuid);
		int nret = gLPGetstring(m_table_blocks.hparent,cmt,suuid,sizeof(suuid) / sizeof(wchar_t)-1,0,0,pt.x,pt.y,0,0);
		if (nret >= 0)
		{
			InstSequence* seq2 = m_seqs.find(seq->is->seqID);
			if (seq2)
				seq2->uuid = wcstol(suuid,0,10);
			flushSeqs();
			gLPUpdatetable(&m_table_bklogs,0);
		}
	}
}

void xVMBlockFrame::setSelectedBlockPattern()
{
	seq_info* seq = (seq_info*)gLPGetsortedbyselection(&m_table_blocks.sorted,m_table_blocks.sorted.selected);
	if (seq)
	{
		POINT pt = { 0 };
		GetCursorPos(&pt);
		//        stdapi (int)     Getstring(HWND hparent,wchar_t *title,wchar_t *s,int length,
		//                           int savetype,int letter,int x,int y,int fi,int mode);
		wchar_t cmt[100];
		wsprintf(cmt,L"Enter Pattern For Block %08X",seq->is->rip());
		wchar_t pattern[TEXTLEN];
		pattern[0] = 0;
		wcscpy(pattern,seq->is->pattern.c_str());
		int nret = gLPGetstring(m_table_blocks.hparent,cmt,pattern,sizeof(pattern) / sizeof(wchar_t)-1,0,0,pt.x,pt.y,0,0);
		if (nret >= 0)
		{
			seq->is->pattern = pattern;
			gLPUpdatetable(&m_table_blocks,0);
			gLPUpdatetable(&m_table_bklogs,0);
		}
	}
}



void xVMBlockFrame::blockPreExecute(DWORD dwThreadId,ULONG_PTR rip)
{
    CONTEXT ctx;
    ctx.ContextFlags = CONTEXT_INTEGER | CONTEXT_CONTROL;
    t_thread* thread = gLPFindthread(dwThreadId);
    BOOL ctxGot = FALSE;
    if (thread) //由于ollydbg调用插件处理调试事件先于ollydbg自身的处理因此我们需要自己获取上下文
        ctxGot = GetThreadContext(thread->thread,&ctx);
    if (!thread || !ctxGot) //如果失败了我们尝试直接自己获取
    {
        //说明ollydbg的函数不靠谱，我们自己尝试一下
        HANDLE ht = OpenThread(MAXIMUM_ALLOWED,FALSE,dwThreadId);
        if (ht == 0)
            return;
        ctxGot = GetThreadContext(thread->thread,&ctx);
        CloseHandle(ht);
    }
    if (!ctxGot)  return;//自己也无法获取，那么无法继续了
    m_vcpu_rcs.initTrace(&ctx);//初始化分层式虚拟机
    m_vcpu_rcs.setRip(rip);
    if (m_vcpu_rcs.step() != VCPU_LOOPRET_NORMAL) return;//步过一行指令即可
    ULONG_PTR opEntry = m_vcpu_rcs.getRip();//取出下一行指令，我们认为是OP分支入口
    //到这里我们就获取到了OP分支的入口，有了入口我们就可以从我们已经有的
    //代码块中查找相关块信息，也可以通过配置直接再次使用代码追踪来动态追踪
    //这个代码块，当虚拟执行到分支跳转我们认为OP块结束。这里我们只实现从代码块中查找
    //从我们的索引当中找代码块编号
    opexec_info opexec;
    opexec.addr = m_unqID++;
    opexec.size = 1;
    opexec.type = 0;
    opexec.selected = FALSE;
    opexec.entry = opEntry;

  //  SequencesIndex::iterator itr = m_instIndex.find(opEntry);
  //  if (itr != m_instIndex.end())
  //  {   //这里我们只使用第一个代码块的信息
  //      int seqID = itr->second;
		//InstSequence* seq = m_seqs.find(seqID);
		//opexec.se = seq;
  //  }
    gLPAddsorteddata(&m_table_bklogs.sorted,&opexec);
    gLPMovetableselection(&m_table_bklogs,MOVEBOTTOM);
    syncByRip(opEntry);
    m_skipNext =TRUE;
}

void xVMBlockFrame::loadSequence(InstSequence *seq)
{
	InstSequence* seqn = m_seqs.new_back(seq->seqID);
	*seqn = *seq;
    for (TraceRecordEx* tr = seq->insts.first();tr;tr = seq->insts.next(tr))
    {
        m_instIndex.insert(std::make_pair(tr->ctx1.rip,seq->seqID));
    }
}
ULONG_PTR xVMBlockFrame::lastRip() const
{
    return m_lastRip;
}

void xVMBlockFrame::setLastRip(ULONG_PTR lastRip)
{
    m_lastRip = lastRip;
}

//设置分支跳转代码监控
void xVMBlockFrame::setBranchingRip(ULONG_PTR rip)
{
    //先判断是否已经设置
    int nc = m_brIPs.size();
    for (int i=0;i<nc;i++)
    {
        if (m_brIPs[i] == rip)
            return;
    }
    //通过添加一个断点来实现监控，具体监控过程,和块执行不同这里的断点是永久性断点
    //我们在插件的ODBG2_Pluginmainloop函数当中完成
    wchar_t nullstr[1];
    nullstr[0] = 0;
    int ibp = gLPSetint3breakpoint(rip,BP_MANUAL|BP_EXEC|BP_BREAK,0,0,0,0,nullstr,nullstr,nullstr);
    if (ibp == 0)
        m_brIPs.push_back(rip);
}

void xVMBlockFrame::detectOPBranch(ULONG_PTR rip)
{
    BYTE buf[16];	//先取当前查表指令的字节码
	int nlen = gLPReadmemory(buf,rip,sizeof(buf),MM_REMOVEINT3|MM_SILENT);
    if (nlen != sizeof(buf))
        return;
    ud_t udis;
    ud_init(&udis);
    ud_set_input_buffer(&udis,(const uint8_t*)buf,sizeof(buf));
    ud_set_mode(&udis,(sizeof(ULONG_PTR) == sizeof(DWORD))?32:64);
    ud_set_pc(&udis,rip);
    int instlen = ud_decode(&udis);	//解码查表指令

    //now trying to get branch jmp	//这里我们开两个分层式虚拟CPU
    ulong tid = gLPGetcputhreadid();
    t_thread* td = gLPFindthread(tid);
    if (!td) return;
    xVMDebugCPU* cpu1 = new xVMDebugCPU;
    if (!cpu1) return;
    xVMDebugCPU* cpu2 = new xVMDebugCPU;
    if (!cpu2)
    {
        delete cpu1;
        return;
    }
    cpu1->init_r3();	//首先我们初始化虚拟CPU环境
    cpu1->initPureTrace(&td->context);	//这里我们根据暂停时实际的线程上下文来初始化虚拟CPU
    cpu1->setDobMode(vmexec_dob_lastmemac);	//设定虚拟执行模式为内存访问模式
    cpu1->setRip(rip);	//设定虚拟cpu的开始地址
    ULONG_PTR lastIP = cpu1->getRip();
    vcpu_return ret1 = cpu1->step();	//这里我们开始单步模拟执行指令
    vcpu_return ret2;	//实际上我们并不关系查表指令的具体结构，因为是查表因此必定引发内存寻址
	//我们只关系表的位置，因此最后一次内存访问位置必定是OP代码分支入口表的位置
    ULONG_PTR opTablePtr = cpu1->lastAccessAddr();
    if (opTablePtr == 0 || cpu1->lastAccessSize() != sizeof(ULONG_PTR))
    {	//如果无法定位表，那么无法继续
        delete cpu1;
        delete cpu2;
        gLPFlash(L"Oh My God!!!This IP is not Fetch instruction.");
        return;
    }
    ULONG_PTR branchJmpIP = 0;	//这里我们检测是否直接使用类似jmp [xxx]形式来查表
    if (cpu1->getRip() != lastIP + instlen)
        branchJmpIP = lastIP;
	//这里我们开始遍历表的范围以尝试OP分支入口表的潜在项
	//这里我们开始真正的对比追踪，以查找分支跳转
	//实际上有些保护系统在跳转上还会使用多分支来扰乱视听，
	// 这种情况只需要少许修改此处代码即可处理，不过在本书中就不给演示了
    int oftdop = 0;
    for (oftdop=1;oftdop<100;oftdop++)	//表的大小上可以根据实际情况设定
    {
        cpu1->initPureTrace(&td->context);
        cpu1->setRip(rip);
        cpu2->initPureTrace(&td->context);
        cpu2->setRip(rip);	//分别初始化两个cpu，
        cpu2->setDobMode(vmexec_dob_oftcheate);
        cpu2->setOffsetDelta(oftdop*sizeof(ULONG_PTR));
		//这里我们让一个cpu模拟原本查表的正常执行流程
		//另外一个cpu我们通过修改cpu访问表时的内存偏移数据来遍历表中的其它项
        ret1 = cpu1->step();	
        ret2 = cpu2->step();
        if (cpu2->dobMode() == vmexec_dob_oftcheate)
        {
            cpu2->setOffsetDelta(0);
            cpu2->setDobMode(vmexec_dob_puretrace);
        }	//如果执行出现无法追踪的异常那么出错
        if (ret1 != ret2)
        {
            branchJmpIP = -1;
            break;
        }	//如果单步后的指令位置发生变化，那么表示我们找到了分支跳转
        if (cpu1->getRip() != cpu2->getRip())
        {
            branchJmpIP = rip;
            break;
        }
		//实际上如果我们发现寄存器发生变化我们也认为对比失败
        BOOL hasDif =FALSE;
        for (int i=0;i<BX_GENERAL_REGISTERS;i++)
        {
            if (cpu1->get_reg32(i) != cpu2->get_reg32(i))
            {
                hasDif = TRUE;
                break;
            }
        }
        if (hasDif) break;
    }
	//如果我们无法找到，那么我们尝试不对比寄存器的情况
    if (branchJmpIP == 0)
    {
        cpu1->initPureTrace(&td->context);
        cpu2->initPureTrace(&td->context);
        cpu1->setRip(rip);
        cpu2->setRip(rip);//再次初始化两个cpu
        cpu2->setDobMode(vmexec_dob_oftcheate);
        cpu2->setOffsetDelta(oftdop*sizeof(ULONG_PTR));
        ret1 = cpu1->step();
        ret2 = cpu2->step();
        cpu2->setOffsetDelta(0);
        cpu2->setDobMode(vmexec_dob_puretrace);
        for (int i=0;i<gxvmcfg.maxPreExecLines;i++)
        {
            lastIP = cpu1->getRip();
            cpu2->setEFlags(cpu1->force_flags());
            ret1 = cpu1->step();
            ret2 = cpu2->step();
            if (ret1 != ret2)
                break;
            if (cpu1->getRip() != cpu2->getRip())
            {
                branchJmpIP = lastIP;
                break;
            }
        }
    }
	//如果我们得到了分支跳转，我们这里具体的来穷举OP代码分支入口表
    NInstSequencesMap seqs;
    if (branchJmpIP == -1) branchJmpIP = 0;
    if (branchJmpIP != 0)   //trying to get each op
    {
        int minidx = 0;
        int maxidx = 1;
        for (int i=0;i<600;i++)	//假设最大600个分支项目
        {
            int index = 0;
            if ((i%2) == 0)
                index = minidx--;
            else
                index = maxidx++;//我们一前一后的遍历
            cpu2->init_r3(FALSE);
            cpu2->initPureTrace(&td->context);
            cpu2->setRip(rip);
            cpu2->setDobMode(vmexec_dob_oftcheate);
            cpu2->setOffsetDelta(index*sizeof(ULONG_PTR));
            lastIP = cpu2->getRip();
            ret2 = cpu2->step();
            opTablePtr = cpu2->lastAccessAddr();
            cpu2->setOffsetDelta(0);
            cpu2->setDobMode(vmexec_dob_puretrace);

            ULONG_PTR opEntry = 0;
            if (lastIP == branchJmpIP)
                opEntry = cpu2->getRip();
            if (opEntry == 0)
            {
                for (int j=0;j<gxvmcfg.maxPreExecLines;j++)
                {
                    lastIP = cpu2->getRip();
                    ret2 = cpu2->step();
                    if (lastIP == branchJmpIP)
                    {
                        opEntry = cpu2->getRip();
                        break;
                    }
                    if (ret2 != VCPU_LOOPRET_NORMAL)
                        break;
                }
            }
			//如果不处于明显的无效内存我们尝试实行代码追踪，以追踪处OP分支的代码
            if (opEntry != 0 && (signed long)opEntry >= 0x10000)
            {	//注意我们的代码分支追踪不是精确的，因为我们以第二次查OPCODE为结束判断
                if (!traceOPBranch(&seqs,cpu2,branchJmpIP,opEntry))
                {
                    if (gLPFindmemory(opEntry))
                    {
                        gLPMessage(opEntry,L"OP Entry Detected,But trace faild,Maybe mostly is not a OP!");
                    }
                }
            }
        }
    }
    delete cpu1;
    delete cpu2;
    if (branchJmpIP == 0)
    {
        gLPFlash(L"Can't detected branch jmp!!!");
        return;
    }
	//这里我们已经成功的找到分支跳转了和一个穷举出来的OP分支入口集合了
	//接着我们就需要对代码进行整理
    gLPMessage(branchJmpIP,L"Detected OP Branch Jmp [%08X]",branchJmpIP);
	conciseSequences(&seqs);
    flushSeqs();
    gLPMessage(seqs.size(),L"Detected [%d] OP Branchs",seqs.size());
}

BOOL xVMBlockFrame::traceOPBranch(NInstSequencesMap* seqs,xVMDebugCPU *cpu,ULONG_PTR branchJmp, ULONG_PTR opEntry)
{
    if (seqs->find(opEntry))
        return TRUE;
    cpu->setDobMode(vmexec_dob_trace);
    cpu->setRip(opEntry);
    vcpu_return ret;
    int nLine=0;
    for (nLine=0;nLine<gxvmcfg.maxPreExecLines;nLine++)
    {
        if (cpu->getRip() == branchJmp)
        {
            cpu->step();
			InstSequence* seq = seqs->new_back(opEntry);
            *seq = cpu->Sequence();
            return TRUE;
        }
        ret = cpu->step();
        if (ret != VCPU_LOOPRET_NORMAL)
            break;
    }
    if (nLine > 10)
    {
        t_memory* mem1 = gLPFindmemory(opEntry);
        t_memory* mem2 = gLPFindmemory(branchJmp);
        if (mem1 == mem2)
        {
			InstSequence* seq = seqs->new_back(opEntry);
            *seq = cpu->Sequence();
            return TRUE;
        }
    }
    return FALSE;
}

ULONG_PTR xVMBlockFrame::getSequenceCommAddr(InstSequence *seq1, InstSequence *seq2)
{
    for (TraceRecordEx* tr = seq1->insts.first();tr;tr= seq1->insts.next(tr))
    {
        for (TraceRecordEx* tr2 = seq2->insts.first();tr2;tr2= seq2->insts.next(tr2))
        {
            if (tr->ctx1.rip == tr2->ctx1.rip)
            {
                return tr->ctx1.rip;
            }
        }
    }
    return 0;
}

void xVMBlockFrame::flushExecLogs(NInstSequenceExecLogs* sseq)
{
	gLPDestroysorteddata(&m_table_bklogs.sorted);
	gLPCreatesorteddata(&m_table_bklogs.sorted,sizeof(opexec_info),300,0,0,0);
	int i = 0;
	for (SequenceExecute* se = sseq->first(); se; se = sseq->next(se))
	{
		flushExecLog(*sseq->key(se),se);
		i++;
	}
	gLPSettableselection(&m_table_bklogs,MOVEBOTTOM);
	gLPUpdatetable(&m_table_bklogs,0);
}

void xVMBlockFrame::flushExecLogs()
{
	flushExecLogs(&m_sseq);
	hiLastExecLog();
}

InstSequence* xVMBlockFrame::findSeq(int seqID)
{
	return m_seqs.find(seqID);
}

void xVMBlockFrame::resetBlocks()
{
	m_seqs.clear();
	m_instIndex.clear();
	m_sseq.clear();
	m_uniqueID = 0;
}

InstSequence* xVMBlockFrame::syncSequenceBack(int seqID)
{
	InstSequence* seq = findSeq(seqID);
	if (seq)
	{
		InstSequence* seqr = m_vcpu_rcs.Sequence();
		if (seqr)
		{
			seq->insts = seqr->insts;
			return seq;
		}
	}
	return 0;
}

void xVMBlockFrame::syncSequenceSeqID(ULONG_PTR seqID)
{
	for (int i = 0; i < m_table_blocks.sorted.n; i++)
	{
		seq_info* xrt = (seq_info*)gLPGetsortedbyindex(&m_table_blocks.sorted,i);
		if (xrt)
		{
			if (xrt->is->seqID == seqID)
			{
				gLPSettableselection(&m_table_blocks,i);
				gLPUpdatetable(&m_table_blocks,0);
				break;
			}
		}
	}
}

void xVMBlockFrame::selectLogsBySeqID(int seqid)
{
	for (int i = 0; i < m_table_bklogs.sorted.n; i++)
	{
		opexec_info* xrt = (opexec_info*)gLPGetsortedbyindex(&m_table_bklogs.sorted,i);
		if (xrt)
		{
			if (xrt->se->seqid == seqid)
			{
				xrt->selected = TRUE;
			} else
				xrt->selected = FALSE;
		}
	}
	gLPUpdatetable(&m_table_bklogs,0);
}

void xVMBlockFrame::hideSelectedLogs()
{
	for (int i = 0; i < m_table_bklogs.sorted.n; i++)
	{
		opexec_info* xrt = (opexec_info*)gLPGetsortedbyindex(&m_table_bklogs.sorted,i);
		if (xrt && xrt->selected)
		{
			SequenceExecute* se = m_sseq.find(xrt->addr);
			if (se)
			{
				if (se->flags & SEQEXEC_FLAG_HIDDEN)
					se->flags &= ~SEQEXEC_FLAG_HIDDEN;
				else
					se->flags |= SEQEXEC_FLAG_HIDDEN;
			}
		}
	}
	flushExecLogs(&m_sseq);
}

void xVMBlockFrame::bringToFront()
{
	HWND hmdi = GetParent(m_hFrame);
	SendMessage(hmdi,WM_MDIACTIVATE,(WPARAM)m_hFrame,0);
}

void xVMBlockFrame::exportExecLogsToFile(TCHAR* path)
{
	NAutoLargeBufferA nla;
	nla.setHostFile(path);
	nla.setMaxmemsize(1024 * 1024 * 4);
	nla.resize(0);
	char tmpstr[1024];
	for (SequenceExecute* se = m_sseq.first(); se; se = m_sseq.next(se))
	{
		ULONG_PTR entry = 0;
		ULONG_PTR opptr = 0;
		InstSequence* seq = m_seqs.find(se->seqid);
		if (seq && seq->insts.size() > 0)
		{
			entry = seq->insts.first()->ctx1.rip;
		}

		int slen = snprintf(tmpstr,sizeof(tmpstr)-1,"SeqID:%08d OP:%08X->ENTRY:%08X %-48S EAX=%08X ECX=%08X EDX=%08X EBX=%08X ESP=%08X EBP=%08X ESI=%08X EDI=%08X EFLAG=%08X\r\n",se->seqid,se->ip,entry,(seq ? seq->pattern.c_str() : (L"")),
			se->regs[0],
			se->regs[1],
			se->regs[2],
			se->regs[3],
			se->regs[4],
			se->regs[5],
			se->regs[6],
			se->regs[7],
#ifndef _M_IX86
			se->regs[8],
			se->regs[9],
			se->regs[10],
			se->regs[11],
			se->regs[12],
			se->regs[13],
			se->regs[14],
			se->regs[15],
#endif
			se->eflags
			);
		nla.append(tmpstr,slen);
	}
	nla.close();
}

void xVMBlockFrame::runPatternAsmOnAllBlocks(wchar_t* lpscript)
{
	patternAsm	pa;
	pa.loadPatternFromFile(lpscript);
	int nc = 0;
	for (InstSequence* rseq = m_seqs.first(); rseq; rseq = m_seqs.next(rseq))
	{
		int ncount = 0;
		int nr;
		do
		{
			nr = xVMDebugCPU::doDeObfuscatePattern(&pa,rseq,false);
			nr += xVMDebugCPU::doDeObfuscatePattern(&pa,rseq,true);
			nc += nr;
			if (ncount++ > 200) break;
		} while (nr > 0);

	}
	gLPFlash(L"Detected %d Junk",nc);
	flushSeqs();
}

void xVMBlockFrame::runCodeDNAOnAllBlocks(const wchar_t* lpscript)
{
	patternAsm	pa;	//载入选定的代码DNA脚本
	pa.loadPatternFromFile(lpscript);
	//这里我们遍历所有的代码块
	for (InstSequence* rseq = m_seqs.first(); rseq; rseq = m_seqs.next(rseq))
	{	//首先对代码块进行顺序匹配
		int pid = xVMDebugCPU::matchCodeDNA(&pa,rseq,false);
		if (pid < 0)	//如果没有匹配花型，那么逆序匹配
			pid = xVMDebugCPU::matchCodeDNA(&pa,rseq,true);
		if (pid >= 0)	
		{	//如果有成功匹配，那么我们需要根据代码DNA脚本的定义更新代码块数据
			patternSequence* pattern = pa.getPattern(pid);
			if (pattern)
			{	//这里我们只根据花型中的DNA注释更新代码块的注释
				//实际上我们可以定义更加高级的信息，不过出于本书的目的已经够用
				rseq->comment = SigleStringToWide(pattern->comment);
				rseq->pattern = SigleStringToWide(pattern->pattern);
				rseq->uuid = pattern->uuid;
			}
		}
	}
	//刷新界面
	flushSeqs();
}

void xVMBlockFrame::addBlockEntries(ULONG_PTR addr,int size)
{
	ulong tid = gLPGetcputhreadid();
	t_thread* td = gLPFindthread(tid);
	if (!td) return;

	//计算并读取出IAT表范围数据
	int naddr = size / sizeof(ULONG_PTR);
	ULONG_PTR* memaddrs = new ULONG_PTR[naddr];
	int nred = gLPReadmemory(memaddrs,addr,naddr*sizeof(ULONG_PTR),MM_REMOVEINT3|MM_SILENT);
	if (nred == naddr*sizeof(ULONG_PTR))
	{
		index_map<ULONG_PTR,ULONG_PTR>	addrs;
		int seqID = 0;
		xVMDebugCPU* cpu1 = new xVMDebugCPU;
		if (!cpu1) return;
		//遍历每一个可能的地址
		for (int i = 0; i < naddr; i++)
		{
			ULONG_PTR addrMem = memaddrs[i];
			addrs.insert(std::make_pair(addrMem,addrMem));
		}
		for (index_map<ULONG_PTR,ULONG_PTR>::iterator itr = addrs.begin(); itr != addrs.end();itr++)
		{
			cpu1->init_r3();	//首先我们初始化虚拟CPU环境
			cpu1->initTrace(&td->context);	//这里我们根据暂停时实际的线程上下文来初始化虚拟CPU
			cpu1->setDobMode(vmexec_dob_trace);	//设定虚拟执行模式为分层式跟踪模式
			cpu1->setRip(itr->first);	//设定虚拟cpu的开始地址

			for (int j = 0; j < gxvmcfg.maxPreExecLines; j++)
			{
				vcpu_return ret2 = cpu1->step();
				if (ret2 != VCPU_LOOPRET_NORMAL)
					break;
			}
			seqID = emptySeqID(seqID);
			InstSequence* seqn = m_seqs.new_back(seqID);
			*seqn = cpu1->Sequence();
			seqn->seqID = seqID;
			seqn->entry = itr->first;
		}
		delete cpu1;
		//这里我们追踪出来的代码块是不准确的，因为我们只按照设定预执行了一段代码
		//那么在conciseSequences1函数当中我们将剔除里面的垃圾指令
		conciseSequences1(&m_seqs);
	}
	delete[] memaddrs;
	flushSeqs();
}

int xVMBlockFrame::emptySeqID(int oldSeqID /*= -1*/)
{
	int i = (oldSeqID <0)?0:oldSeqID;
	while(m_seqs.find(i))
		i++;
	return i;
}

void xVMBlockFrame::setLastsel(int lastsel)
{
	m_lastsel = lastsel;
}

int xVMBlockFrame::lastsel() const
{
	return m_lastsel;
}

void xVMBlockFrame::clearSeqsSelection(t_table* pt)
{
	for (int i = 0; i < pt->sorted.n; i++)
	{
		seq_info* xrt = (seq_info*)gLPGetsortedbyindex(&pt->sorted,i);
		if (xrt)
			xrt->selected = FALSE;
	}
	gLPUpdatetable(pt,0);
}

void xVMBlockFrame::moniterSelectedEntries()
{
	for (int i = 0; i < m_table_blocks.sorted.n; i++)
	{
		seq_info* xrt = (seq_info*)gLPGetsortedbyindex(&m_table_blocks.sorted,i);
		if (xrt && xrt->selected)
		{

			if (xrt->is->flags & INSTSEQ_FLAG_MONITER)
				xrt->is->flags &= ~INSTSEQ_FLAG_MONITER;
			else
				xrt->is->flags |= INSTSEQ_FLAG_MONITER;
		}
	}
	flushSeqs();
}

NInstSequencesMap* xVMBlockFrame::getSequences()
{
	return &m_seqs;
}


int xVMBlockFrame::addBlockExecLog(SequenceExecute* se,int startID)
{
	for (int i = 0; i < MAX_VAR_NUM;i++)
	{
		switch (se->vars[i].type)
		{
		case sevar_setuuid:
		{
			se->vars[i].type = sevar_none;
			InstSequence* is = m_seqs.find(se->seqid);
			if (is)
			{
				is->uuid = se->vars[0].sqword;
			}
		}break;
		case sevar_setcomment : {
			se->vars[i].type = sevar_none;
			InstSequence* is = m_seqs.find(se->seqid);
			if (is)
			{
				is->comment = SigleStringToWide(se->vars[0].byte);
			}
		}break;
		case sevar_setpattern: {
			se->vars[i].type = sevar_none;
			InstSequence* is = m_seqs.find(se->seqid);
			if (is)
			{
				is->pattern = SigleStringToWide(se->vars[0].byte);
			}
		}break;
		}
	}
	int sid = emptyExecLogID(startID);
	SequenceExecute* senew = m_sseq.push_back(sid,se);
	flushExecLog(sid,senew);
	return sid;
}

int xVMBlockFrame::emptyExecLogID(int oldLogID /*= -1*/)
{
	if (oldLogID == -1)
	{
		if (!m_sseq.find(m_uniqueID++))
			return m_uniqueID - 1;
		else
			m_uniqueID = m_sseq.count();
	}
	int i = (oldLogID < 0) ? 0 : oldLogID;
	while (m_sseq.find(i))
		i++;
	return i;
}

int xVMBlockFrame::conciseSequences(NInstSequencesMap* seqs)
{
	ULONG_PTR blockEndAddr = 0;
	index_map<ULONG_PTR,int> refc;
	int forc = 0;
	for (InstSequence* seq = seqs->first(); seq; seq = seqs->next(seq),forc++)
	{	//这里我们统计代码块的引用信息
		InstSequence* seq2 = seqs->next(seq);
		if (!seq2)  break;
		for (; seq2; seq2 = seqs->next(seq2))
		{
			ULONG_PTR comAddr = getSequenceCommAddr(seq,seq2);
			if (comAddr != 0)
			{
				index_map<ULONG_PTR,int>::iterator itrc = refc.find(comAddr);
				if (itrc == refc.end())
					itrc = refc.insert(std::make_pair(comAddr,0)).first;
				itrc->second++;
			}
		}
		if (forc > 5) break;

	}
	if (refc.size() > 0)
		blockEndAddr = refc.back().first;
	for (InstSequence* seq = seqs->first(); seq; seq = seqs->next(seq))
	{//在上面的过程当中因为我们使用了第二次分支查表来结束，因此追踪出来的代码分支
		//包含很多重复的指令，我们在这里尝试剔除这些指令
		seq->refc = 0;
		seq->seqID = m_unqID++;
		if (blockEndAddr != 0)
		{
			for (TraceRecordEx* tr = seq->insts.first(); tr; tr = seq->insts.next(tr))
			{
				if (tr->ctx1.rip == blockEndAddr)
				{
					while (tr)
					{
						tr = seq->insts.erase(tr);
					}
					break;
				}
			}
		}
		loadSequence(seq);
	}
	return 0;
}

int xVMBlockFrame::conciseSequences1(NInstSequencesMap* seqs)
{
	//这里已经追踪完毕，但是代码分支当中显然有大量的重复指令，因此我们需要精简OP代码分支
	//首先我们需要为每一条指令建立指令索引
	struct inst_index{
		int seqid;
		int instid;
	};
	index_multimap<ULONG_PTR,inst_index> idx;
	//这里我们要记录指令所处的代码块以及在代码块中的序号
	for (InstSequence* seq = seqs->first(); seq; seq = seqs->next(seq))
	{
		int i = 0;
		for (TraceRecordEx* tr = seq->insts.first(); tr; tr = seq->insts.next(tr),i++)
		{
			inst_index ii;
			ii.seqid = seq->seqID;
			ii.instid = i;
			idx.insert(std::make_pair(tr->ctx1.rip,ii));
		}
	}
	//索引建立好以后我们需要再次遍历整个记录剔除重复的指令
	for (InstSequence* seq = seqs->first(); seq; seq = seqs->next(seq))
	{
		int i = 0;
		for (TraceRecordEx* tr = seq->insts.first(); tr; tr = seq->insts.next(tr),i++)
		{
			int nc = idx.count(tr->ctx1.rip);
			if (nc > 1)
			{	//如果指令一旦出现在多个OP代码分支中，那么我们认为之后的代码都不是该代码分支的
				BOOL bErase = FALSE;
				int i = 0;
				for (index_multimap<ULONG_PTR,inst_index>::iterator itr = idx.find(tr->ctx1.rip); itr != idx.end() && i < nc; itr++,i++)
				{
					if (itr->second.seqid != seq->seqID)
					{
						if (itr->second.instid <= i)
						{
							bErase = TRUE;
							break;
						}
					}
				}
				if (bErase)
				{
					while (tr)
						tr = seq->insts.erase(tr);
					break;
				}
			}
		}
	}
	return 0;
}

void xVMBlockFrame::calcBlockExecReference()
{
	resetBlocksReference();
	for (SequenceExecute* se = m_sseq.first(); se;se = m_sseq.next(se))
	{
		InstSequence* seq = m_seqs.find(se->seqid);
		if (seq)
			seq->refc++;
	}
	flushSeqs();
}

void xVMBlockFrame::resetBlocksReference()
{
	for (InstSequence* seq = m_seqs.first(); seq;seq = m_seqs.next(seq))
	{
		seq->refc = 0;
	}
}

void xVMBlockFrame::clearSeqExecLogs()
{
	m_sseq.cleanup();
	flushExecLogs(&m_sseq);
}

HWND xVMBlockFrame::hFrame()
{
	return m_hFrame;
}
BOOL xVMBlockFrame::isFrameActive()
{
	HWND hmdi = (HWND)SendMessageW(*ghwmdiclient,WM_MDIGETACTIVE,0,0);
	if (hmdi == m_hFrame)
		return TRUE;
	return FALSE;
}
BOOL xVMBlockFrame::blockStepIn()
{
	if (!isFrameActive())
		return FALSE;
	//VMDebug_TraceInReal(TRUE,run_stepin);
	return TRUE;
}

BOOL xVMBlockFrame::blockStepOver()
{
	if (!isFrameActive())
		return FALSE;
	switch (m_dbgmode)
	{
	case dbg_direct:VMDebug_DirectStepOver(TRUE,run_stepover);break;
	default:VMDebug_TraceInReal(TRUE,run_stepover); break;
	}
	
	return TRUE;
}

BOOL xVMBlockFrame::blockRun(BOOL singleThread)
{	//如果代码块调试窗口处于激活状态，我们就让程式进入虚拟化调试跟踪
	if (!isFrameActive())
		return FALSE;
	switch (m_dbgmode)
	{
	case dbg_direct:VMDebug_DirectStepOver(singleThread,run_run); break;
		//这里我们给定了继续运行的方式，可以指定只运行一个线程，
		//关键在于第二个参数我们传递了一个状态信息给监控脚本，
		// 这样在脚本当中我们就可以检测到我们请求的执行状态，从而给出正确的反应
	default:VMDebug_TraceInReal(singleThread,run_run); break;
	}


	
	return TRUE;
}

OPBreakList* xVMBlockFrame::getBreakPoints()
{
	return &m_bps;
}

void xVMBlockFrame::setBreakPoint(ULONG_PTR ip,BOOL bRemove)
{
	if (bRemove)
	{
		m_bps.remove(ip);
	} else
	{
		opcode_bp* bp = m_bps.new_back(ip);
		bp->ip = ip;
		//m_sseq.find(addrid)
	}

}

void xVMBlockFrame::removeTempExecLogs()
{
	for (SequenceExecute* se = m_sseq.first(); se;)
	{
		if (se->flags & SEQEXEC_FLAG_TEMP)
			se = m_sseq.remove(se);
		else
			se = m_sseq.next(se);
	}
}

void xVMBlockFrame::exportExecLogsToTemplate()
{
	m_setpl.cleanup();
	for (SequenceExecute* se = m_sseq.first(); se;se = m_sseq.next(se))
	{
		if (se->flags & SEQEXEC_FLAG_TEMP) continue;
		opexec_tpl* tpl = m_setpl.new_back(se->ip);
		tpl->ip = se->ip;
		tpl->uuid = se->uuid;
		tpl->entry = 0;
		tpl->flags = 0;
		tpl->seqid = se->seqid;
		InstSequence* is = m_seqs.find(se->seqid);
		if (is)
		{
			tpl->entry = is->rip();
		}
	}
	flushExecTemplate();
}

void xVMBlockFrame::flushExecTemplate()
{
	gLPDestroysorteddata(&m_table_template.sorted);
	gLPCreatesorteddata(&m_table_template.sorted,sizeof(opexec_tpltb),1000,0,0,SDM_EXTADDR);
	int i = 0;
	for (opexec_tpl* tpl = m_setpl.first(); tpl;tpl = m_setpl.next(tpl))
	{
		opexec_tpltb optpl;
		optpl.addr = i++;
		optpl.size = 1;
		optpl.type = 0;
		optpl.selected = FALSE;
		optpl.tpl = tpl;
		if (tpl->flags & SEQEXEC_FLAG_BREAK)
			setBreakPoint(tpl->ip);
		gLPAddsorteddata(&m_table_template.sorted,&optpl);
	}
	gLPUpdatetable(&m_table_template,0);
}

void xVMBlockFrame::setHilightOIP(ULONG_PTR ip)
{
	m_hiOIP = ip;
}

ULONG_PTR xVMBlockFrame::hilightOIP()
{
	return m_hiOIP;
}

InstSequence* xVMBlockFrame::ensureSequence(int seqID,ULONG_PTR entry)
{
	InstSequence* seqn = m_seqs.find(seqID);
	if (!seqn)
	{
		if (seqID == -1)
			seqID = emptySeqID(-1);
		seqn = m_seqs.new_back(seqID);
		seqn->seqID = seqID;
		seqn->entry = entry;
	}
	return seqn;
}

void xVMBlockFrame::setDbgMode(int mode)
{
	m_dbgmode = mode;
}

void xVMBlockFrame::removeLastTempExecLogs()
{
	for (SequenceExecute* se = m_sseq.last(); se;se = m_sseq.prev(se))
	{
		if (se->flags & SEQEXEC_FLAG_TEMP)
			se = m_sseq.remove(se);
		else
			break;
	}
}

void xVMBlockFrame::flushExecLog(int key,SequenceExecute* se)
{
	if ((gxvmcfg.bkopt.filter & bkopt_flags_showhidden) || !(se->flags & SEQEXEC_FLAG_HIDDEN))
	{
		opexec_info opexec;
		opexec.addr = key;
		opexec.size = 1;
		opexec.type = 0;
		opexec.selected = FALSE;
		opexec.se = se;
		if (se->flags & SEQEXEC_FLAG_BREAK)
			setBreakPoint(se->ip);
		InstSequence* seq = m_seqs.find(se->seqid);
		if (seq)
		{
			opexec.entry = seq->rip();
		}
		gLPAddsorteddata(&m_table_bklogs.sorted,&opexec);
	}
}

void xVMBlockFrame::hiLastExecLog()
{
	if (m_sseq.count() > 0)
	{
		for (SequenceExecute* se = m_sseq.last(); se; se = m_sseq.prev(se))
		{
			if (!(se->flags & SEQEXEC_FLAG_TEMP) && se->ip != 0)
			{
				setHilightOIP(se->ip);
				break;
			}
		}
	}
}


xVMBlockFrame* VMDebug_BlockFrame(BOOL bEnsureShow)
{
    if (!gXVMBlocks)
    {
        gXVMBlocks = new xVMBlockFrame();
		gXVMBlocks->initFrame();
    }
	if (gXVMBlocks && bEnsureShow)
		gXVMBlocks->initFrame();
    return gXVMBlocks;
}


void VMDebug_BlockExec()
{
    xVMBlockFrame* fm = VMDebug_BlockFrame(TRUE);
    if (fm)
		fm->bringToFront();
}

void    VMDebug_BlockAddSeq(struct InstSequence* seq)
{
    xVMBlockFrame* fm = VMDebug_BlockFrame();
    if (fm)
    {
        fm->loadSequence(seq);
        fm->flushSeqs();
    }

}

void    VMDebug_BlockSyncEip(ULONG_PTR rip)
{
    if (gXVMBlocks)
        gXVMBlocks->syncByRip(rip);
}
void    VMDebug_BlockSetBranching()
{
    xVMBlockFrame* fm = VMDebug_BlockFrame();
    if (fm)
    {
        fm->setBranchingRip(gLPGetcpudisasmselection());
    }
}

void VMDebug_BlockProcessEvent(DEBUG_EVENT *ev)
{
    if (gXVMBlocks)
    {
        gXVMBlocks->processDebugEvent(ev);
    }
}


void VMDebug_DetectOPBranch()
{
    xVMBlockFrame* fm = VMDebug_BlockFrame();
    if (fm)
    {
        fm->detectOPBranch(gLPGetcpudisasmselection());
    }
}

void VMDebug_AddtoBlockEntries(t_table *pt)
{
	//为了方便，这里直接使用了customdata转换为dump窗体，但是我不确定是否所有
	//ollydbg Dump窗体都是这样，因此我们简单效验一下
	if (((ULONG_PTR)pt - (ULONG_PTR)pt->customdata) != offsetof(t_dump,table))
		return;
	//取出已选择范围信息
	t_dump* dmp = reinterpret_cast<t_dump*>(pt->customdata);
	ULONG_PTR addr1 = dmp->sel0;
	ULONG_PTR addr2 = dmp->sel1;
	int size = addr2 - addr1;
	if (size <= 0)
		return;
	xVMBlockFrame* fm = VMDebug_BlockFrame();
	if (fm)	//这里传递数据范围给实际的处理函数
		fm->addBlockEntries(addr1,size);
}

BOOL VMDebug_IsBlockWndActive()
{
	if (!gXVMBlocks) return FALSE;
	return gXVMBlocks->isFrameActive();
}

BOOL VMDebug_BlockStepIn()
{
	if (!gXVMBlocks) return FALSE;
	return gXVMBlocks->blockStepIn();
}

BOOL VMDebug_BlockStepOver()
{
	if (!gXVMBlocks) return FALSE;
	return gXVMBlocks->blockStepOver();
}

BOOL VMDebug_BlockRun(BOOL singleThread)
{
	if (!gXVMBlocks) return FALSE;
	return gXVMBlocks->blockRun(singleThread);
}

