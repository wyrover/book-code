#include "xvmdebugcpu.h"

#include "xvmdebug.h"
xVMDebugCPU::xVMDebugCPU()
    :m_vcpu1(0)
    ,m_vcpu2(0)
    ,m_multiBrunch(FALSE)
{

    init_r3();
}

xVMDebugCPU::~xVMDebugCPU()
{
    if (m_vcpu1)
        delete m_vcpu1;
    if (m_vcpu2)
        delete m_vcpu2;
}


void xVMDebugCPU::access_write_physical(bx_phy_address addr, unsigned len, void *data)
{
    switch(state())
    {
    //如果CPU状态处于追踪模式，那么我们不使用原本的内存管理，而使用虚拟内存层
    case vmexec_emu_r3_deobfuscate:
    {
        switch(dobMode())
        {
        case vmexec_dob_oftcheate:
        {
            addr += m_offsetDelta;
        }
        case vmexec_dob_lastmemac:
        {
            m_lastAccessAddr = addr;
            m_lastAccessSize = len;
        }
        case vmexec_dob_puretrace:
        case vmexec_dob_iatrace:
        case vmexec_dob_trace:
        {
			int flags = RECORD_FLAG_MEMWRITE;
			if (addr >= (RSP - (sizeof(ULONG_PTR)* BX_GENERAL_REGISTERS)) && addr <= (RSP + (sizeof(ULONG_PTR)* BX_GENERAL_REGISTERS)))
				flags |= RECORD_FLAG_SPWRITE;

			ULONG_PTR memv = 0;
			memcpy(&memv,data,min(sizeof(ULONG_PTR),len));

			bx_phy_address bxa;
			int szmem;
			ULONG_PTR ulptv;
			int mode = lastMemWriteAccess(bxa,szmem,ulptv);
			if ((mode & RECORD_FLAG_MEMWRITE) && ((addr + len) >= bxa && addr <= (bxa+szmem)))
				setLastMemWriteAccess(flags,min(addr,bxa),max(addr + len,bxa + szmem) - min(addr,bxa),memv);
			else
				setLastMemWriteAccess(flags,addr,len,memv);
            m_vmem.writemem(addr,(unsigned char*)data,len);
            return;
        }break;

        }
    }
    }
    xHook_VMExec::access_write_physical(addr,len,data);
}

void xVMDebugCPU::access_read_physical(bx_phy_address addr, unsigned len, void *data)
{

    switch(state())
    {
    case vmexec_emu_r3_deobfuscate:
    {
        switch(dobMode())
        {
        case vmexec_dob_oftcheate:
        {
            addr += m_offsetDelta;
        }
        case vmexec_dob_lastmemac:
        {
            m_lastAccessAddr = addr;
            m_lastAccessSize = len;
        }
        case vmexec_dob_puretrace:
        case vmexec_dob_iatrace:
        case vmexec_dob_trace:
        {
            //我们首先尝试从虚拟内存层中读取数据，如果失败我们再从真实内存空间中读取数据
            int state = m_vmem.readmem(addr,(unsigned char*)data,len);
            if (state != len)
            {
				int nlen = gLPReadmemory(data,addr,len,MM_REMOVEINT3 | MM_PARTIAL | MM_SILENT);
                if (nlen <= 0)
                {
                    //exception here
					int p = 0;
					p++;
                }
            }
			int flags = RECORD_FLAG_MEMREAD;
			if (addr >= (RSP - (sizeof(ULONG_PTR)* BX_GENERAL_REGISTERS)) && addr <= (RSP + (sizeof(ULONG_PTR)* BX_GENERAL_REGISTERS)))
				flags |= RECORD_FLAG_SPREAD;
			ULONG_PTR memv = 0;
			memcpy(&memv,data,min(sizeof(ULONG_PTR),len));
			bx_phy_address bxa;
			int szmem;
			ULONG_PTR ulptv;
			int mode = lastMemReadAccess(bxa,szmem,ulptv);
			if ((mode & RECORD_FLAG_MEMREAD) && ((addr + len) >= bxa && addr <= (bxa + szmem)))
				setLastMemReadAccess(flags,min(addr,bxa),max(addr + len,bxa + szmem) - min(addr,bxa),memv);
			else
				setLastMemReadAccess(flags,addr,len,memv);

			return;
        }break;
        }
    }
    }
    xHook_VMExec::access_read_physical(addr,len,data);
}
const char *xVMDebugCPU::vcpu_fetch_op(bx_phy_address addr)
{
    switch(state())
    {
    case vmexec_emu_r3_deobfuscate:
    {
        switch(dobMode())
        {
        case vmexec_dob_oftcheate:
        case vmexec_dob_lastmemac:
        case vmexec_dob_puretrace:
        case vmexec_dob_multibrunch:
        case vmexec_dob_trace:
        case vmexec_dob_iatrace:
        {   //我们仍然首先从虚拟内存层获取字节码，这样遇到自修改指令仍然能够正确模拟
            int state = m_vmem.readmem(addr,(unsigned char*)m_tempop,16);
            if (state != 16)
            {
				int nlen = gLPReadmemory(m_tempop,addr,16,MM_REMOVEINT3|MM_PARTIAL| MM_SILENT);
                if (nlen <= 0)
                {
                    return 0;
                }
            }
            return m_tempop;
        }break;
        }
    }
    }
    access_read_physical(addr,16,m_tempop);
    return m_tempop;
}

vcpu_return xVMDebugCPU::vcpu_before_exec(bxInstruction_c *inst)
{
    switch(dobMode())
    {
    case vmexec_dob_multibrunch:
    case vmexec_dob_trace:
    {
        m_lastTr.flags = 0;
        if (m_multiBrunch)
        {
            ULONG_PTR addr1 = 0;
            ULONG_PTR addr2 = 0;
            BYTE op1 = inst->opcode_bytes[0];
            BOOL isJMC = FALSE;
            switch(op1)
            {
            case 0x70:case 0x71:case 0x72:case 0x73:case 0x74:case 0x75:case 0x76:case 0x77:
            case 0x78:case 0x79:case 0x7A:case 0x7B:case 0x7C:case 0x7D:case 0x7E:case 0x7F:
            {
                isJMC = TRUE;
                addr1 = RIP+2;
                addr2 = RIP + (signed int)inst->opcode_bytes[1] + 2;
            }break;
            case 0x0F:
            {
                if (inst->opcode_bytes[1] >= 0x80 && inst->opcode_bytes[1] <= 0x8F)
                {
                    addr1 = RIP+6;
                    addr2 = RIP + *(signed long*)(inst->opcode_bytes+2) + 6;
                    isJMC = TRUE;
                }
            }break;
            }
            if (isJMC)
            {
                m_vcpu1->initDeobfuscate();
                m_vcpu1->m_seq1.insts.clear();
                m_vcpu1->setDobMode(vmexec_dob_multibrunch);
                m_vcpu1->setRip(addr1);
                for (int i=0;i<256;i++)
                {
                    vcpu_return vret = m_vcpu1->step();
                    if (vret != VCPU_LOOPRET_NORMAL)
                        break;
                }

                m_vcpu2->initDeobfuscate();
                m_vcpu2->m_seq1.insts.clear();
                m_vcpu2->setDobMode(vmexec_dob_multibrunch);
                m_vcpu2->setRip(addr2);
                for (int i=0;i<256;i++)
                {
                    vcpu_return vret = m_vcpu2->step();
                    if (vret != VCPU_LOOPRET_NORMAL)
                        break;
                }

                InstSequence* seq1 = &m_vcpu1->m_seq1;
                InstSequence* seq2 = &m_vcpu2->m_seq1;
                BOOL bMultiBrunch = FALSE;
                for (TraceRecordEx* tr1 = seq1->insts.first();tr1 && !bMultiBrunch;tr1 = seq1->insts.next(tr1))
                {
                    for (TraceRecordEx* tr2 = seq2->insts.first();tr2;tr2 = seq2->insts.next(tr2))
                    {
                        if (tr1->ctx1.rip == tr2->ctx1.rip)
                        {
                            //multi-brunch found
                            if (tr1->ctx1.regs[0] == tr2->ctx1.regs[0]
                                    && tr1->ctx1.regs[1] == tr2->ctx1.regs[1]
                                    && tr1->ctx1.regs[2] == tr2->ctx1.regs[2]
                                    && tr1->ctx1.regs[3] == tr2->ctx1.regs[3]
                                    && tr1->ctx1.regs[4] == tr2->ctx1.regs[4]
                                    && tr1->ctx1.regs[5] == tr2->ctx1.regs[5]
                                    && tr1->ctx1.regs[6] == tr2->ctx1.regs[6]
                                    && tr1->ctx1.regs[7] == tr2->ctx1.regs[7]
                                    )
                            {
                                bMultiBrunch = TRUE;
                                break;
                            }else
                            {
                                //???
                            }


                        }
                    }
                }
                if (bMultiBrunch)
                {
                    madeJmpInst(inst,RIP,addr2);
                    m_lastTr.flags |= RECORD_FLAG_MULTIB;
                }

            }
        }
        m_lastTr.ctx1.unqueID = m_uqid;
        m_uqid++;
        saveContextToRecord(&m_lastTr.ctx1,inst);
    }break;
    case vmexec_dob_oftcheate:
    case vmexec_dob_puretrace:
    case vmexec_dob_stackob:
    {

    }break;
    }
    return xHook_VMExec::vcpu_before_exec(inst);
}


vcpu_return xVMDebugCPU::vcpu_after_exec(bxInstruction_c *inst)
{
    switch(dobMode())
    {
    case vmexec_dob_puretrace:
    {
        if (prev_rip == RIP)
            RIP += inst->ilen();
        return VCPU_LOOPRET_NORMAL;
    }
    case vmexec_dob_multibrunch:
    case vmexec_dob_trace:  //多分支或者预执行模式
    {   //保存指令现场
        m_lastTr.ctx2.unqueID = m_uqid;
		m_lastTr.flags |= lastMemReadAccess(m_lastTr.mraddr,m_lastTr.mrsize,m_lastTr.mrvalue); 
		m_lastTr.flags |= lastMemWriteAccess(m_lastTr.mwaddr,m_lastTr.mwsize,m_lastTr.mwvalue);
        saveContextToRecord(&m_lastTr.ctx2,inst);
        m_seq1.insts.push_back(m_lastTr);    //将指令保存到结果集
        //在预执行当中我们关闭了虚拟机的传输指令加速因此rep会被解释成多个指令重复执行
        if (prev_rip == RIP)
        {
            // printf("(*)");  //如果遇到传输指令我们只执行一次就强制性步过了
            RIP += inst->ilen();
        }
    }break;
    case vmexec_dob_iatrace:
    {
        bx_phy_address addr = RIP;
        //我们缓冲最后一个执行代码的内存区域，判断是否出现夸内存段
        if (addr < m_lastmem || addr > (m_lastmem+m_szlstmem))
        {
            //如果代码的执行已经超出了最后一个内存块那么我们查看具体跳转到了什么内存块
            t_memory* tmem = gLPFindmemory(addr);
            if (tmem)
            {
                m_lastmem = tmem->base;
                m_szlstmem = tmem->size;
                t_module* mod = gLPFindmodule(m_lastmem);
                if (mod)
                {
                    //如果目标是一个模块，那么我们认为到达入口
                    t_module* tcmod = gLPFindmodule(m_traceaddr);
                    if (mod != tcmod)
                    {
                        return VCPU_LOOPRET_FINISHED;
                    }
                }
            }
        }
    }break;
    }
    return xHook_VMExec::vcpu_after_exec(inst);
}
//初始化分层式代码虚拟机环境，我们从线程上下文拷贝初始数据
void xVMDebugCPU::initTrace(CONTEXT *context)
{
    setState(vmexec_emu_r3_deobfuscate);
    setDobMode(vmexec_dob_trace);
    eflags = context->EFlags;

    EDI = context->Edi;
    ESI = context->Esi;
    EBP = context->Ebp;
    ESP = context->Esp;
    EBX = context->Ebx;
    EDX = context->Edx;
    ECX = context->Ecx;
    EAX = context->Eax;

    init();
	//setTraceRecordFile(_T("d:\\test.bin"));
	//setFlags(VMEXEC_FLAG_RECORD);
	//setFlags(VMEXEC_FLAG_RECORDBLOCK);
}

void xVMDebugCPU::initPureTrace(CONTEXT *context)
{
    initTrace(context);
    setDobMode(vmexec_dob_puretrace);
}

void xVMDebugCPU::initIATrace(bx_phy_address addr)
{
    initDeobfuscate();
    setDobMode(vmexec_dob_iatrace);
    m_vmem.init();
    m_lastmem = 0;
    m_szlstmem = 0;
    m_traceaddr = addr;
    setRip(addr);
}

void xVMDebugCPU::initRecordStep(TraceRecordEx *tr)
{

    setState(vmexec_emu_r3_deobfuscate);
    setDobMode(vmexec_dob_puretrace);
    loadContextFromRecord(&tr->ctx1);
}

void xVMDebugCPU::saveContextToRecord(TraceRecord *ctx, bxInstruction_c *inst)
{
    ctx->rip = RIP;
    if (inst)
    {
        ctx->nip = RIP+inst->ilen();
        ctx->szOP = inst->ilen();
    }
    ctx->eflags = force_flags();
    ctx->regs[0] = RAX;
    ctx->regs[1] = RCX;
    ctx->regs[2] = RDX;
    ctx->regs[3] = RBX;
    ctx->regs[4] = RSP;
    ctx->regs[5] = RBP;
    ctx->regs[6] = RSI;
    ctx->regs[7] = RDI;
    if (inst)
        memcpy(ctx->opcode,inst->opcode_bytes,ctx->szOP);
}

void xVMDebugCPU::loadContextFromRecord(TraceRecord *ctx)
{
    eflags = ctx->eflags;
    EDI = ctx->regs[7];
    ESI = ctx->regs[6];
    EBP = ctx->regs[5];
    ESP = ctx->regs[4];
    EBX = ctx->regs[3];
    EDX = ctx->regs[2];
    ECX = ctx->regs[1];
    EAX = ctx->regs[0];
}

void xVMDebugCPU::init()
{
    m_seq1.insts.clear();
    m_vmem.init();
    m_uqid = 0;
    m_lastAccessAddr = 0;
    m_lastAccessSize = 0;
    m_offsetDelta = 0;
	resetRecordState();
}

void xVMDebugCPU::initColorTrace()
{
    init();
    setState(vmexec_emu_r3_deobfuscate);
    setDobMode(vmexec_dob_mmstrace);
    prev_rip = 0;
    prev_rsp = 0;
}

void xVMDebugCPU::resetColorContext()
{
    setEFlags(0x202);
    EAX = DEOBFUSCATE_EAX;
    ECX = DEOBFUSCATE_ECX;
    EDX = DEOBFUSCATE_EDX;
    EBX = DEOBFUSCATE_EBX;
    ESP = DEOBFUSCATE_ESP;
    EBP = DEOBFUSCATE_EBP;
    ESI = DEOBFUSCATE_ESI;
    EDI = DEOBFUSCATE_EDI;
}

void xVMDebugCPU::madeJmpInst(bxInstruction_c *inst, bx_phy_address addr, bx_phy_address target)
{

    char op[16];
    op[0] = 0xE9;
    int ret;
    *(DWORD*)(op+1) = target-addr-5;
#if BX_SUPPORT_X86_64
    if (BX_CPU_THIS_PTR cpu_mode == BX_MODE_LONG_64)
        ret = fetchDecode64((const Bit8u*)op, inst, 5);
    else
#endif
        ret = fetchDecode32((const Bit8u*)op, inst, 5);


#ifdef BX_INSTR_STORE_OPCODE_BYTES
    if (ret >= 0)
        inst->set_opcode_bytes((const Bit8u*)op);
#endif
}

void xVMDebugCPU::EnableMBDetect()
{
    if (!m_vcpu1)
    {
        m_vcpu1 = new xVMDebugCPU;
        m_vcpu1->init_r3();
        //m_vcpu1->initDeobfuscate();
    }
    if (!m_vcpu2)
    {
        m_vcpu2 = new xVMDebugCPU;
        m_vcpu2->init_r3();
    }
    m_multiBrunch = TRUE;
}

void xVMDebugCPU::DisableMBDetect()
{
    m_multiBrunch = FALSE;
    if (m_vcpu1)
    {
        delete m_vcpu1;
        m_vcpu1 = 0;
    }
    if (m_vcpu2)
    {
        delete m_vcpu2;
        m_vcpu2 = 0;
    }
}

void xVMDebugCPU::deleteJunkInSeq()
{
    deleteFlagedInSeq(&m_seq1);
}

int xVMDebugCPU::deleteFlagedInSeq(InstSequence *seq, int flags)
{
	int nc = 0;
    for (TraceRecordEx* tr = seq->insts.first();tr;)
    {
		if (tr->flags & flags)
		{
			tr = seq->insts.erase(tr);
			nc++;
		}else
            tr = seq->insts.next(tr);
    }
	return nc;
}

int xVMDebugCPU::isValidIATCall()
{

    //    EDI = 0x77777777;
    //    ESI = 0x66666666;
    //    EBP = 0x55555555;
    //    ESP = DEOBFUSCATE_ESP_BASE;
    //    EBX = 0x44444444;
    //    EDX = 0x33333333;
    //    ECX = 0x22222222;
    //    EAX = 0x11111111;

    int isvalid = -1;
    if (gxvmcfg.bIATTraceCheckCtx)
    {
        if (RCX == 0x22222222 && ESI == 0x66666666 && EDI == 0x77777777
                && EBP == 0x55555555)
        {
            isvalid = 0;
        }
    }else
    {
        isvalid = 0;
    }

    //    int isvalid = -1;
    //    if (RCX == 0x22222222 && ESI == 0x66666666 && EDI == 0x77777777
    //            && EBP == 0x55555555)
    //    {
    //        isvalid = 0;
    //    }
    if (RSP != DEOBFUSCATE_ESP)
    {
        if (isvalid == 0)
            isvalid = DEOBFUSCATE_ESP-RSP;
    }
    return isvalid;
}

const InstSequence &xVMDebugCPU::Sequence()
{
    return m_seq1;
}

BOOL xVMDebugCPU::checkESPSpace(const char *lpOP, int szOP, int szStk)
{
    setState(vmexec_emu_r3_deobfuscate);
    setDobMode(vmexec_dob_trace);
    RSP = DEOBFUSCATE_ESP;
    ULONG_PTR oldRSP = RSP;
    step(0x400000,lpOP,szOP);
    ULONG_PTR newRSP = RSP;
    if ((oldRSP - newRSP) == szStk)
        return TRUE;
    return FALSE;
}
ULONG_PTR xVMDebugCPU::lastAccessAddr() const
{
    return m_lastAccessAddr;
}
ULONG_PTR xVMDebugCPU::lastAccessSize() const
{
    return m_lastAccessSize;
}
int xVMDebugCPU::offsetDelta() const
{
    return m_offsetDelta;
}

void xVMDebugCPU::setOffsetDelta(int offsetDelta)
{
    m_offsetDelta = offsetDelta;
}

int xVMDebugCPU::doDeObfuscatePattern(patternAsm* pasm,InstSequence* seq,bool reserve)
{
	TraceRecordEx* tr1 = 0;
	TraceRecordEx* tr2 = 0;
	for (TraceRecordEx* tr = reserve ? seq->insts.last() : seq->insts.first(); tr; tr = reserve ? seq->insts.prev(tr) : seq->insts.next(tr))
	{
		//初始化匹配工作
		int nseq = pasm->initForTest(reserve);
		if (nseq < 1) return 0;
		//这里我们遍历整个代码记录列表
		for (TraceRecordEx* subtr = tr; subtr; subtr = reserve ? seq->insts.prev(subtr) : seq->insts.next(subtr))
		{	//由于我们的代码结果是依据实际执行结果的，因此我们可以很容易整理出栈变化和eflags的变化
			bool isEspChanged = (subtr->ctx1.regs[4] != subtr->ctx2.regs[4]);
			bool isEflagsChanged = (subtr->ctx1.eflags != subtr->ctx2.eflags);
			patternSequence* sr = 0;
			int srt = pasm->testInstruction((const char*)subtr->ctx1.opcode,subtr->ctx1.szOP,
				subtr->ctx1.rip,isEspChanged,isEflagsChanged,subtr,sr);
			if (srt < 0)	//匹配失败，已经没有剩余花型可匹配，可以开始下一轮匹配
			{
				break;
			} else if (srt == 1 && sr)	//匹配成功，至少有一个花型成功匹配
			{	//这里我们处理花型的替换操作
				//无论替换成功与否，我们先将指针设定到下一行指令，避免重复匹配刚刚替换的指令
				subtr = reserve ? seq->insts.prev(subtr) : seq->insts.next(subtr);
				tr = subtr;
				//我们需要遍历每一个patternSequence结构中的语句，因为每一行语句都有可能发生替换
				for (patternInst* pi = sr->insts.first(); pi; pi = sr->insts.next(pi))
				{	//我们取出匹配指令所对应的最终记录
					TraceRecordEx* trp = reinterpret_cast<TraceRecordEx*>(pi->userdata);
					if (trp)
					{
						TraceRecordEx* trprev = trp;
						TraceRecordEx* trpnew = 0;
						bool lessone = false;
						//遍历所有的替换列表
						for (pattern_inst_body* rpi = pi->rps.first(); rpi; rpi = pi->rps.next(rpi))
						{
							trpnew = seq->insts.Allocate();
								//如果是->nop我们不生成指令
							if (rpi->ins.opcode != I_NOP)
							{
								//这里我们根据记录生成替换指令
								if (pi->userdata) 
									*trpnew = *reinterpret_cast<TraceRecordEx*>(pi->userdata);
								else
								{
									trpnew->flags = 0;
									trpnew->ctx1.eflags = 0;
									trpnew->ctx1.rip = trp->ctx1.rip;
									trpnew->ctx1.regs[4] = 0;
									trpnew->ctx2.eflags = 0;
									trpnew->ctx2.rip = trp->ctx1.rip;
									trpnew->ctx2.regs[4] = 0;
								}
								int len = pasm->genReplaceOpcode(sr,rpi,(char*)trpnew->ctx1.opcode,trpnew->ctx1.szOP,trpnew->ctx1.rip);
								if (len > 0)
								{
									trpnew->ctx1.szOP = len;
									trprev = seq->insts.AddAfter(trprev,trpnew);
									trpnew = 0;
									lessone = true;
								}
							} else lessone = true;
						}
						if (trpnew)	//部分或者全部替换未成功
						{
							seq->insts.FreeOnly(trpnew);
							trpnew = 0;
						}
						if (lessone)
						{
							trp->flags |= RECORD_FLAG_DELETE;
						}
					}
				}
				break;
			}
		}
	}
	return deleteFlagedInSeq(seq);
}

int xVMDebugCPU::matchCodeDNA(patternAsm* pasm,InstSequence* seq,bool reserve)
{
	TraceRecordEx* tr1 = 0;
	TraceRecordEx* tr2 = 0;
	//初始化匹配工作
	int nseq = pasm->initForTest(reserve);
	if (nseq < 1) return -2;
	//这里我们遍历整个代码记录列表
	for (TraceRecordEx* tr = reserve ? seq->insts.last() : seq->insts.first(); 
		tr; tr = reserve ? seq->insts.prev(tr) : seq->insts.next(tr))
	{	//由于我们的代码结果是依据实际执行结果的，因此我们可以很容易整理出栈变化和eflags的变化
		bool isEspChanged = (tr->ctx1.regs[4] != tr->ctx2.regs[4]);
		bool isEflagsChanged = (tr->ctx1.eflags != tr->ctx2.eflags);
		patternSequence* sr = 0;
		int srt = pasm->testInstruction((const char*)tr->ctx1.opcode,tr->ctx1.szOP,
			tr->ctx1.rip,isEspChanged,isEflagsChanged,tr,sr);
		if (srt < 0)	//匹配失败，已经没有剩余花型可匹配，可以开始下一轮匹配
		{
			break;
		} else if (srt == 1 && sr)	//匹配成功，至少有一个花型成功匹配
		{	//这里至少有一个花型匹配了，因此我们认为匹配成功，返回花型的ID
			return sr->id;
		}
	}
	return -1;
}

bx_hostpageaddr_t xVMDebugCPU::getHostMemAddr(bx_phy_address ppf,unsigned rw)
{
	return 0;
}

