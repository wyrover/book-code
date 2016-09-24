#ifndef XVMBLOCKFRAME_H
#define XVMBLOCKFRAME_H

#include <windows.h>
#include <vector>

#include "xvmdebugcpu.h"
#include "xdynamic_ollydbg.h"
#include "xvmdebugframe.h"

typedef index_multimap<ULONG_PTR,int>     SequencesIndex;


enum bk_dbgmode{
	dbg_byvm = 0,
	dbg_direct
};
typedef struct seq_info :public t_sorthdr{
    BOOL			selected;
	InstSequence*	is;
}seq_info;


typedef struct opexec_info :public t_sorthdr{
    BOOL        selected;
    ULONG_PTR   entry;
	SequenceExecute* se;
}opexec_info;


struct opexec_tpl{
	ULONG_PTR		ip;
	int				seqid;
	ULONG_PTR		entry;
	int				uuid;
	long			flags;
};

typedef struct opexec_tpltb :public t_sorthdr{
	BOOL        selected;
	opexec_tpl* tpl;
}opexec_tpltb;

typedef NAvlList<ULONG_PTR,opexec_tpl>	ExecTemplateList;
class xVMBlockFrame
{
public:
    xVMBlockFrame();
    void initFrame();
    void reset();
	HWND hFrame();
    int saveBlocks(wchar_t* lpPath);
    int loadBlocks(wchar_t* lpPath);
	int loadBlocks(NPacketBase* pk);
    int analyserTraceRecords(wchar_t* lpPath);
	int analyserTraceRecords(NTraceRecords* rcs);
	
    void	syncByRip(ULONG_PTR rip);
	void	setHilightOIP(ULONG_PTR ip);
	ULONG_PTR hilightOIP();
	void	syncSequenceSeqID(ULONG_PTR seqID);
	InstSequence* 		syncSequenceBack(int seqID);
	void	runPatternAsmOnAllBlocks(wchar_t* lpscript);
	void	runCodeDNAOnAllBlocks(const wchar_t* lpscript);
    void    flushSeqs();
    void    flushSeq(int seqID);
	void	flushExecLogs(NInstSequenceExecLogs* sseq);
	void	flushExecLog(int key,SequenceExecute* se);
	void	flushExecLogs();
	void	hiLastExecLog();
	void	flushExecTemplate();
	void	removeTempExecLogs();
	void	removeLastTempExecLogs();
	void	setDbgMode(int mode);
    BOOL isHilSeq(int seqID);

	BOOL	isFrameActive();

	InstSequence*	ensureSequence(int seqID,ULONG_PTR entry);

	int		emptySeqID(int oldSeqID = -1);
	int		emptyExecLogID(int oldLogID = -1);

	void bringToFront();

	void exportExecLogsToFile(TCHAR* path);

	void hideSelectedLogs();

    void processDebugEvent(DEBUG_EVENT* ev);
    void processExceptionEvent(DWORD dwThreadId,LPEXCEPTION_DEBUG_INFO ex);
    void stepOverBlock();

    void setSelectedBlockComment();
	void setSelectedBlockUUID();
	void setSelectedBlockPattern();
	void exportExecLogsToTemplate();

	void selectLogsBySeqID(int seqid);
	void	clearSeqExecLogs();

    void blockPreExecute(DWORD dwThreadId, ULONG_PTR rip);

    void loadSequence(InstSequence* seq);
    ULONG_PTR lastRip() const;
    void setLastRip(ULONG_PTR lastRip);

	InstSequence* findSeq(int seqID);
    void setBranchingRip(ULONG_PTR rip);
    void detectOPBranch(ULONG_PTR rip);
    BOOL traceOPBranch(NInstSequencesMap *seqs, xVMDebugCPU* cpu, ULONG_PTR branchJmp, ULONG_PTR opEntry);

	void	addBlockEntries(ULONG_PTR addr,int size);
	int		addBlockExecLog(SequenceExecute* se,int startID);
    ULONG_PTR getSequenceCommAddr(InstSequence* seq1,InstSequence* seq2);
	void resetBlocks();

	int lastsel() const;
	void setLastsel(int lastsel);
	void clearSeqsSelection(t_table* pt);
	void moniterSelectedEntries();

	void calcBlockExecReference();
	void resetBlocksReference();

	NInstSequencesMap*	getSequences();
	OPBreakList*	getBreakPoints();
	void setBreakPoint(ULONG_PTR ip,BOOL bRemove = FALSE);
	BOOL blockStepIn();
	BOOL blockStepOver();
	BOOL blockRun(BOOL singleThread);

	int conciseSequences(NInstSequencesMap* seqs);
	int conciseSequences1(NInstSequencesMap* seqs);
private:
    t_frame m_frame;
    t_block m_block_main;
    t_block m_block_blocks;
    t_block m_block_bklogs;
    t_block m_block_left;
	t_block m_block_right;
	t_block	m_block_template;

    t_table m_table_blocks;
    t_table m_table_bklogs;
	t_table m_table_template;
    HWND    m_hFrame;

    int     m_unqID;
    NInstSequencesMap		m_seqs;	//用户管理代码块信息
    SequencesIndex			m_instIndex;
	NInstSequenceExecLogs	m_sseq;	//用于保存块执行记录
	ExecTemplateList		m_setpl;//块执行参考

    std::vector<int>    m_hilSeqs;
    ULONG_PTR           m_lastRip;
    std::vector<ULONG_PTR>  m_shotBPs;

    std::vector<ULONG_PTR>  m_brIPs;

	ULONG_PTR	m_hiOIP;

	//用于维护OPCODE断点
	OPBreakList	m_bps;

	xVMDebugFrame m_vcpu_rcs;
    BOOL    m_skipNext;
	int		m_lastsel;
	int		m_dbgmode;
	int		m_uniqueID;
};


void    VMDebug_BlockProcessEvent(DEBUG_EVENT* ev);
extern void VMDebug_BlockExec();
void    VMDebug_BlockAddSeq(struct InstSequence* seq);
void    VMDebug_BlockSyncEip(ULONG_PTR rip);
void    VMDebug_BlockSetBranching();
xVMBlockFrame* VMDebug_BlockFrame(BOOL bEnsureShow = FALSE);
void    VMDebug_DetectOPBranch();

void VMDebug_AddtoBlockEntries(t_table *pt);

BOOL	VMDebug_IsBlockWndActive();
BOOL	VMDebug_BlockStepIn();
BOOL	VMDebug_BlockStepOver();
BOOL	VMDebug_BlockRun(BOOL singleThread);

#endif // XVMBLOCKFRAME_H
