#ifndef XDYNAMIC_OLLYDBG_H
#define XDYNAMIC_OLLYDBG_H

#include <Windows.h>
#include "ollydbg2/plugin.h"
#include "../../xHooklib/xhook_dynamicsym.h"

extern HMODULE ghOllydbg;

typedef HWND (__cdecl* LPCreateframewindow)(t_frame *pf,wchar_t *icon,wchar_t *title);
typedef HWND (__cdecl* LPCreatetabwindow)(t_frame *pf,wchar_t *icon,wchar_t *title);
typedef t_table*	(__cdecl* LPGetactiveframe)(t_frame *pf);
extern xHook::DynamicDllFun<LPGetactiveframe> gLPGetactiveframe;

typedef void (__cdecl* LPDestroysorteddata)(t_sorted *sd);
typedef int (__cdecl* LPCreatesorteddata)(t_sorted *sd,ulong itemsize,int nexp,SORTFUNC *sortfunc,DESTFUNC *destfunc,int mode);
typedef void (__cdecl* LPDeletesorteddata)(t_sorted *sd,ulong addr,ulong subaddr);
typedef int (__cdecl* LPDeletesorteddatarange)(t_sorted *sd,ulong addr0,ulong addr1);
typedef void* (__cdecl* LPAddsorteddata)(t_sorted *sd,void *item);
typedef int (__cdecl* LPReplacesorteddatarange)(t_sorted *sd,void *data,int n,ulong addr0,ulong addr1);
typedef void (__cdecl* LPRenumeratesorteddata)(t_sorted *sd);
typedef int (__cdecl* LPConfirmsorteddata)(t_sorted *sd,int confirm);
typedef int (__cdecl* LPDeletenonconfirmedsorteddata)(t_sorted *sd);
typedef void (__cdecl* LPUnmarknewsorteddata)(t_sorted *sd);
typedef void* (__cdecl* LPFindsorteddata)(t_sorted *sd,ulong addr,ulong subaddr);
typedef void* (__cdecl* LPFindsorteddatarange)(t_sorted *sd,ulong addr0,ulong addr1);
typedef int (__cdecl* LPFindsortedindexrange)(t_sorted *sd,ulong addr0,ulong addr1);
typedef void* (__cdecl* LPGetsortedbyindex)(t_sorted *sd,int index);
typedef int (__cdecl* LPSortsorteddata)(t_sorted *sd,int sort);
typedef void* (__cdecl* LPGetsortedbyselection)(t_sorted *sd,int index);
typedef int (__cdecl* LPIssortedinit)(t_sorted *sd);
typedef ulong (__cdecl* LPGetcputhreadid)(void);
typedef t_thread* (__cdecl* LPFindthread)(ulong threadid);
typedef ulong (__cdecl* LPReadmemory)(void *buf,ulong addr,ulong size,int mode);
typedef ulong (__cdecl* LPDisasm)(uchar *cmd,ulong cmdsize,ulong cmdip,uchar *cmddec,
                   t_disasm *cmdda,int cmdmode,t_reg *cmdreg,
                   t_predict *cmdpredict);

typedef void (__cdecl* LPRemoveanalysis)(ulong base,ulong size,int keephittrace);
typedef int(__cdecl* LPPastememoryhex)(ulong addr,ulong size,
	int ensurebackup,int removeanalysis);
extern xHook::DynamicDllFun<LPPastememoryhex> gLPPastememoryhex;
extern xHook::DynamicDllFun<LPRemoveanalysis>	gLPRemoveanalysis;

typedef void (__cdecl*  LPSetcpu)(ulong threadid,ulong asmaddr,ulong dumpaddr,
                   ulong selsize,ulong stackaddr,int mode);
extern xHook::DynamicDllFun<LPSetcpu> gLPSetcpu;

typedef t_dump *(__cdecl* LPGetcpudisasmdump)(void);
extern xHook::DynamicDllFun<LPGetcpudisasmdump> gLPGetcpudisasmdump;

typedef int(__cdecl* LPListmemory)(void);
extern xHook::DynamicDllFun<LPListmemory> gLPListmemory;
typedef int (__cdecl*  LPFindlabel)(ulong addr,wchar_t *name,int firsttype);

typedef void (__cdecl*  LPRegistermodifiedbyuser)(t_thread *pthr);
extern xHook::DynamicDllFun<LPRegistermodifiedbyuser>   gLPRegistermodifiedbyuser;

typedef ulong (__cdecl* LPGetcputhreadid)(void);
extern xHook::DynamicDllFun<LPGetcputhreadid> gLPGetcputhreadid;

typedef void(__cdecl* LPRedrawcpureg)(void);
extern xHook::DynamicDllFun<LPRedrawcpureg> gLPRedrawcpureg;

typedef int     (__cdecl* LPMovetableselection)(t_table *pt,int n);
typedef int     (__cdecl* LPSettableselection)(t_table *pt,int selected);
extern xHook::DynamicDllFun<LPSettableselection> gLPSettableselection;

typedef t_table* (__cdecl* LPGetcpudisasmtable)(void);
extern xHook::DynamicDllFun<LPGetcpudisasmtable> gLPGetcpudisasmtable;

typedef void    (__cdecl* LPSetautoupdate)(t_table *pt,int autoupdate);
typedef HGLOBAL (__cdecl* LPCopytableselection)(t_table *pt,int column);
typedef HGLOBAL (__cdecl* LPCopywholetable)(t_table *pt,int compatible);
typedef void   (__cdecl* LPDestroysorteddata)(t_sorted *sd);
typedef void (__cdecl* LPUpdatetable)(t_table *pt,int force);
typedef HWND (__cdecl* LPActivatetablewindow)(t_table *pt);

typedef ulong (__cdecl* LPGetcpudisasmselection)(void);

typedef int(__cdecl* LPDecodearglocal)(ulong ip,ulong offs,ulong datasize,
	wchar_t *name,int len);

typedef int (__cdecl* LPRemoveint3breakpoint)(ulong addr,ulong type);
extern xHook::DynamicDllFun<LPRemoveint3breakpoint> gLPRemoveint3breakpoint;

typedef int (__cdecl* LPSetint3breakpoint)(ulong addr,ulong type,int fnindex,
                   int limit,int count,int unk1,wchar_t *condition,
                   wchar_t *expression,wchar_t *exprtype);
extern xHook::DynamicDllFun<LPSetint3breakpoint> gLPSetint3breakpoint;

typedef int (__cdecl* LPEnableint3breakpoint)(ulong addr,int enable);
extern xHook::DynamicDllFun<LPEnableint3breakpoint> gLPEnableint3breakpoint;


typedef int     (__cdecl* LPLabeladdress)(wchar_t *text,ulong addr,ulong reladdr,int relreg,
                   int index,uchar *mask,int *select,ulong mode);
extern xHook::DynamicDllFun<LPLabeladdress> gLPLabeladdress;

typedef int(__cdecl* LPFindnextnamelistW)(ulong *addr,int *type,wchar_t *name,int nname);

typedef int(__cdecl* LPCexpression)(wchar_t *expression,uchar *cexpr,int nexpr,
	int *explen,wchar_t *err,ulong mode);
typedef int     (__cdecl* LPGetstring)(HWND hparent,wchar_t *title,wchar_t *s,int length,
                                   int savetype,int letter,int x,int y,int fi,int mode);
extern xHook::DynamicDllFun<LPGetstring> gLPGetstring;
typedef int(__cdecl* LPConfirmint3breakpoint)(ulong addr);
typedef int(__cdecl* LPConfirmhardwarebreakpoint)(ulong addr);
typedef int(__cdecl* LPConfirmint3breakpointlist)(ulong *addr,int naddr);
typedef void(__cdecl* LPWipebreakpointrange)(ulong addr0,ulong addr1);
typedef int(__cdecl* LPRemovemembreakpoint)(ulong addr);
extern xHook::DynamicDllFun<LPRemovemembreakpoint> gLPRemovemembreakpoint;

typedef int(__cdecl* LPSetmembreakpoint)(ulong addr,ulong size,ulong type,
                   int limit,int count,wchar_t *condition,
                   wchar_t *expression,wchar_t *exprtype);
extern xHook::DynamicDllFun<LPSetmembreakpoint> gLPSetmembreakpoint;
typedef int(__cdecl* LPEnablemembreakpoint)(ulong addr,int enable);
typedef int(__cdecl* LPRemovehardbreakpoint)(int index);
typedef int(__cdecl* LPSethardbreakpoint)(int index,ulong size,ulong type,int fnindex,
	ulong addr,int limit,int count,wchar_t *condition,wchar_t *expression,wchar_t *exprtype);
extern xHook::DynamicDllFun<LPSethardbreakpoint> gLPSethardbreakpoint;
typedef int(__cdecl* LPEnablehardbreakpoint)(int index,int enable);
typedef int(__cdecl* LPFindfreehardbreakslot)(ulong type);

extern xHook::DynamicDllFun<LPConfirmint3breakpoint> gLPConfirmint3breakpoint;
extern xHook::DynamicDllFun<LPConfirmhardwarebreakpoint> gLPConfirmhardwarebreakpoint;
extern xHook::DynamicDllFun<LPConfirmint3breakpointlist> gLPConfirmint3breakpointlist;
extern xHook::DynamicDllFun<LPWipebreakpointrange> gLPWipebreakpointrange;
extern xHook::DynamicDllFun<LPEnablemembreakpoint> gLPEnablemembreakpoint;
extern xHook::DynamicDllFun<LPRemovehardbreakpoint> gLPRemovehardbreakpoint;
extern xHook::DynamicDllFun<LPEnablehardbreakpoint> gLPEnablehardbreakpoint;
extern xHook::DynamicDllFun<LPFindfreehardbreakslot> gLPFindfreehardbreakslot;

typedef int (__cdecl* LPFindnameW)(ulong addr,int type,wchar_t *name,int nname);

typedef ulong (__cdecl*  LPAssemble)(wchar_t *src,ulong ip,uchar *buf,ulong nbuf,int mode, wchar_t *errtxt);
extern xHook::DynamicDllFun<LPAssemble>	gLPAssemble;
typedef void(__cdecl* LPInfo)(wchar_t *format,...);
extern xHook::DynamicDllFun<LPInfo> gLPInfo;
//varapi (void)    Info(wchar_t *format,...);
typedef void (__cdecl* LPMessage)(ulong addr,wchar_t *format,...);
extern xHook::DynamicDllFun<LPMessage> gLPMessage;

typedef int(__cdecl* LPBinaryedit)(HWND hparent,wchar_t *title,t_hexstr *hstr,
	int letter,int x,int y,int fi,int mode);
extern xHook::DynamicDllFun<LPBinaryedit> gLPBinaryedit;

//varapi (void)    Tempinfo(wchar_t *format,...);
typedef void (__cdecl* LPFlash)(wchar_t *format,...);
//varapi (void)    Progress(int promille,wchar_t *format,...);
//stdapi (void)    Moveprogress(int promille);
typedef void (__cdecl* LPSetstatus)(t_status newstatus);
extern xHook::DynamicDllFun<LPSetstatus> gLPSetstatus;

typedef t_module* (__cdecl* LPFindmodule)(ulong addr);
extern xHook::DynamicDllFun<LPFindmodule> gLPFindmodule;
typedef t_module* (__cdecl* LPFindmodulebyname)(wchar_t *shortname);
extern xHook::DynamicDllFun<LPFindmodulebyname> gLPFindmodulebyname;
//stdapi (t_module *) Findmainmodule(void);
//stdapi (int)     Issystem(ulong addr);
//stdapi (ulong *) Findfixup(t_module *pmod,ulong addr);
//stdapi (ulong)   Findfileoffset(t_module *pmod,ulong addr);
//stdapi (int)     Decoderange(wchar_t *s,ulong addr,ulong size);
//stdapi (int)     Getexeversion(wchar_t *path,wchar_t *version);
typedef int     (__cdecl* LPGetexportfrommemory)(ulong addr,wchar_t *s);
extern xHook::DynamicDllFun<LPGetexportfrommemory> gLPGetexportfrommemory;
typedef int  (__cdecl* LPRun)(t_status status,int pass);
extern xHook::DynamicDllFun<LPRun> gLPRun;
extern LPRun	glpRunOrg;
extern LPSetcpu	glpSetcpuOrg;
//stdapi (void)    Flushmemorycache(void);
//stdapi (ulong)   Readmemory(void *buf,ulong addr,ulong size,int mode);
//stdapi (ulong)   Readmemoryex(void *buf,ulong addr,ulong size,int mode,
//                   ulong threadid);
typedef ulong  (__cdecl* LPWritememory)(const void *buf,ulong addr,ulong size,int mode);
extern xHook::DynamicDllFun<LPWritememory> gLPWritememory;
typedef t_memory* (__cdecl* LPFindmemory)(ulong addr);
extern xHook::DynamicDllFun<LPFindmemory> gLPFindmemory;

typedef int(__cdecl* LPEnsurememorybackup)(t_memory *pmem,int makebackup);
extern xHook::DynamicDllFun<LPEnsurememorybackup> gLPEnsurememorybackup;

//stdapi (uchar *) Finddecode(ulong addr,ulong *psize);
//stdapi (int)     Listmemory(void);
//stdapi (HGLOBAL) Copymemoryhex(ulong addr,ulong size);
//stdapi (int)     Pastememoryhex(ulong addr,ulong size,
//                   int ensurebackup,int removeanalysis);
//stdapi (int)     Editmemory(HWND hparent,ulong addr,ulong size,
//                   int ensurebackup,int removeanalysis,int x,int y,int font);

extern xHook::DynamicDllFun<LPCreateframewindow> glpCreateframewindow;
extern xHook::DynamicDllFun<LPCreatetabwindow> glpCreatetabwindow;
extern xHook::DynamicDllFun<LPCreatesorteddata> gLPCreatesorteddata;
extern xHook::DynamicDllFun<LPAddsorteddata> gLPAddsorteddata;
//extern xHook::DynamicDllFun<LPGetcputhreadid> gLPGetcputhreadid;
extern xHook::DynamicDllFun<LPFindthread> gLPFindthread;
extern xHook::DynamicDllFun<LPReadmemory> gLPReadmemory;
extern xHook::DynamicDllFun<LPDisasm> gLPDisasm;
extern xHook::DynamicDllFun<LPMovetableselection> gLPMovetableselection;
extern xHook::DynamicDllFun<LPSetautoupdate> gLPSetautoupdate;
extern xHook::DynamicDllFun<LPCopytableselection> gLPCopytableselection;
extern xHook::DynamicDllFun<LPCopywholetable> gLPCopywholetable;
extern xHook::DynamicDllFun<LPGetsortedbyselection> gLPGetsortedbyselection;
extern xHook::DynamicDllFun<LPGetsortedbyindex> gLPGetsortedbyindex;
extern xHook::DynamicDllFun<LPDestroysorteddata> gLPDestroysorteddata;
extern xHook::DynamicDllFun<LPUpdatetable> gLPUpdatetable;
extern xHook::DynamicDllFun<LPDeletesorteddata> gLPDeletesorteddata;
extern xHook::DynamicDllFun<LPActivatetablewindow> gLPActivatetablewindow;
extern xHook::DynamicDllFun<LPGetcpudisasmselection> gLPGetcpudisasmselection;
extern xHook::DynamicDllFun<LPFlash> gLPFlash;


extern t_run*		gRuninfo;
extern t_table*		gMemoryTable;
extern t_table*		gbpoint;
extern HMODULE		ghOllydbg;
extern HWND*		ghwollymain;
extern HWND*		ghwmdiclient;
extern HMODULE		ghxVMDebug;
extern COLORREF*	gColor;
extern t_scheme*	gScheme;
extern t_scheme*	gHilite;
extern t_font*		gFont;

void	fn_odbg_setint3(void* paddr,unsigned char* pbuf,int pszbuf,unsigned char pint3cmd);
void	fn_odbg_getint3(void* paddr,unsigned char* pbuf,int pszbuf,unsigned char pint3cmd);
bool	fn_odbg_checkint3(void* paddr,unsigned long pexceptcode);

#endif // XDYNAMIC_OLLYDBG_H
