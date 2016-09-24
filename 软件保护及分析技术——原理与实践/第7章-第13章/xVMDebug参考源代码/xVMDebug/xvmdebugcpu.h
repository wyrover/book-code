#ifndef XVMDEBUGCPU_H
#define XVMDEBUGCPU_H

#include "../../xHooklib/xhook_vmexec.h"
#include "xdynamic_ollydbg.h"
#include "../../../nCom/nstate.h"
#include "../../../nSRK/nSRK_CC/nasm/i386/nasm_i386.h"
#include "patternAsm.h"
struct memstate{
    int type;
};

//从虚拟机继承一个子类用作分层虚拟机
class xVMDebugCPU : public xHook_VMExec
{
    friend class xVMDebugFrame;
public:
    xVMDebugCPU();
    ~xVMDebugCPU();
public:
	virtual bx_hostpageaddr_t getHostMemAddr(bx_phy_address ppf,unsigned rw);
    //虚拟机执行中需要向物理内存写入数据
    virtual void access_write_physical(bx_phy_address addr, unsigned len, void *data);
    //虚拟机执行中需要从物理内存读取数据
    virtual void access_read_physical(bx_phy_address  addr, unsigned len, void *data);
    //取得即将执行指令的字节码
    virtual const char* vcpu_fetch_op(bx_phy_address addr);
    //指令执行前的事件回调
    virtual vcpu_return vcpu_before_exec(bxInstruction_c* inst);
    //指令执行后的事件回调
    virtual vcpu_return vcpu_after_exec(bxInstruction_c* inst);


    void initTrace(CONTEXT *context);
    void initPureTrace(CONTEXT *context);
    void initIATrace(bx_phy_address addr);
    void initRecordStep(TraceRecordEx* tr);
    void saveContextToRecord(TraceRecord *ctx, bxInstruction_c *inst = 0);
    void loadContextFromRecord(TraceRecord* ctx);
    void init();
    void initColorTrace();

    void resetColorContext();

    void madeJmpInst(bxInstruction_c* inst,bx_phy_address addr,bx_phy_address target);

    void EnableMBDetect();
    void DisableMBDetect();

    void deleteJunkInSeq();

    int isValidIATCall();

    const InstSequence& Sequence();

    BOOL checkESPSpace(const char* lpOP,int szOP,int szStk);
    ULONG_PTR lastAccessAddr() const;
    ULONG_PTR lastAccessSize() const;

    int offsetDelta() const;
    void setOffsetDelta(int offsetDelta);

	static int  deleteFlagedInSeq(InstSequence* seq,int flags = RECORD_FLAG_DELETE);
	static int	doDeObfuscatePattern(patternAsm* pasm,InstSequence* seq,bool reserve);
	static int	matchCodeDNA(patternAsm* pasm,InstSequence* seq,bool reserve);
private:
    vmem    m_vmem;

    char    m_tempop[16];
    ulong   m_uqid;
    InstSequence    m_seq1;
    TraceRecordEx   m_lastTr;

    //for multibrunch
    BOOL                m_multiBrunch;
    xVMDebugCPU*        m_vcpu1;
    xVMDebugCPU*        m_vcpu2;

    //for iatrace
    ULONG_PTR           m_lastmem;
    ULONG_PTR           m_szlstmem;
    ULONG_PTR           m_traceaddr;

    ULONG_PTR           m_lastAccessAddr;
    ULONG_PTR           m_lastAccessSize;
    int                 m_offsetDelta;

    //for advance deobfuscate
    typedef     nstate<ULONG_PTR,memstate> mmstate;
    mmstate     m_state;
};

#endif // XVMDEBUGCPU_H
