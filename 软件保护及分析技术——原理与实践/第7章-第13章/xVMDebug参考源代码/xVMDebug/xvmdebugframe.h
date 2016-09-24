#ifndef XVMDEBUGFRAME_H
#define XVMDEBUGFRAME_H
#include <windows.h>
#include <tchar.h>
#include "xvmdebugcpu.h"
#include "../../../nCom/nstate.h"
#include "../../xHooklib/vcpu/chaincpu.h"


enum trace_mode{
    trace_ateip,
    trace_atsel
};


struct xvm_record :public t_sorthdr{
    TraceRecordEx   rc;
    BOOL            selected;
};

struct xvm_memaccess : public t_sorthdr{
	ULONG_PTR	memptr;
	ULONG_PTR	memvalue;
	int			memsize;
	int			mode;
	int			instID;
};
class xVMDebugFrame : public xVMDebugCPU
{
public:
    xVMDebugFrame();
    ~xVMDebugFrame();
    void trace(trace_mode mode);

	void assemblerSelectRecord();
    void reset();
	void initFrame(wchar_t* name,BOOL blockOnly = FALSE,BOOL bShowTitle = TRUE);
	HWND hFrame();
	t_frame* frame();
	t_table* table();
	t_block* block();
    void flushSeq();
	void flushMemAccess();
	InstSequence*	Sequence();
	void clearSeq();
	void bringToFront();
    void syncByRip(ULONG_PTR rip);
	void syncMemAccByIndex(int index);
	void syncRecordsByIndex(int index);

	void autoTrace(ULONG_PTR rip);

	void	refresh();

    void traceSequence(InstSequence* seq);
    void traceSequence();
    void clearSeqFlag();
    void deObfuscateJcc(bool onlyjmp);
    void DeobfuscateStackOB();
    void doMemoryAnalyserCurrentSeq();
    void dobCurrentSequenceNormal(bool markonly = false);
    void dobCurrentSequencePushPop();
	void dobCurrentSequenceAll();

    int  doMemoryStateAnalyser(InstSequence* seq);
	int doDeObfuscatePattern();
	int doDeObfuscatePatternAll();
    int  doDeobfuscateNormal(InstSequence* seq,bool flagonly = false);
    int  doDeobfuscateTempPushPop(InstSequence* seq);

    void loadRecords(TCHAR* path);
	void saveRecords(TCHAR* path);
	void runPattern(const TCHAR* filename,BOOL once = FALSE);
    void makeCodeBlock(t_table *pt);
	void detectCodeBlocks();
    bool IsEffectESP(TraceRecordEx* tr);

	ChainCPU*	ensureChainCPU();
	ChainCPU*	getChainCPU();


	void exportToText(TCHAR* path);

    void setRecordExFlags(int no,long flags);


    ULONG_PTR activeRip() const;
    void setActiveRip(const ULONG_PTR &activeRip);

    int lastsel() const;
	void setLastsel(int lastsel,xvm_record* xr);

	int loadPatterns();


	void showMemChain(xvm_record* xr);

private:
    t_frame m_frame;
    t_block m_block_disasm;
    t_table m_table_disasm;
	t_block m_block_memacc;
	t_table m_table_memacc;
	t_block	m_block_host;
    HWND    m_hFrame;
    ULONG_PTR   m_activeRip;
    int m_lastsel;


    NASM_I386   m_asm;

	patternAsm	m_paAsm;
	patternAsm	m_paAsmOnce;


	ChainCPU*   m_ccpu;

};

void VMDebug_debugEventLoop(DEBUG_EVENT* event);
HWND VMDebug_DebugMDIWindow();
BOOL VMDebug_DebugMDIWindowIsActive();
int	 VMDebug_StepOver();
xVMDebugFrame* VMDebug_EnsureTraceFrame();

void cpuRipChanged(ULONG_PTR rip);

#endif // XVMDEBUGFRAME_H
