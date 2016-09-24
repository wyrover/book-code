#include "xdynamic_ollydbg.h"

HMODULE ghOllydbg = GetModuleHandle(0);

xHook::DynamicDllFun<LPCreateframewindow> glpCreateframewindow(ghOllydbg,"Createframewindow");
xHook::DynamicDllFun<LPCreatetabwindow> glpCreatetabwindow(ghOllydbg,"Createtabwindow");
xHook::DynamicDllFun<LPCreatesorteddata> gLPCreatesorteddata(ghOllydbg,"Createsorteddata");
xHook::DynamicDllFun<LPAddsorteddata> gLPAddsorteddata(ghOllydbg,"Addsorteddata");
xHook::DynamicDllFun<LPGetcputhreadid> gLPGetcputhreadid(ghOllydbg,"Getcputhreadid");
xHook::DynamicDllFun<LPFindthread> gLPFindthread(ghOllydbg,"Findthread");
xHook::DynamicDllFun<LPReadmemory> gLPReadmemory(ghOllydbg,"Readmemory");
xHook::DynamicDllFun<LPDisasm> gLPDisasm(ghOllydbg,"Disasm");
xHook::DynamicDllFun<LPMovetableselection> gLPMovetableselection(ghOllydbg,"Movetableselection");
xHook::DynamicDllFun<LPSettableselection> gLPSettableselection(ghOllydbg,"Settableselection");
xHook::DynamicDllFun<LPSetautoupdate> gLPSetautoupdate(ghOllydbg,"Setautoupdate");
xHook::DynamicDllFun<LPCopytableselection> gLPCopytableselection(ghOllydbg,"Copytableselection");
xHook::DynamicDllFun<LPCopywholetable> gLPCopywholetable(ghOllydbg,"Copywholetable");
xHook::DynamicDllFun<LPGetsortedbyselection> gLPGetsortedbyselection(ghOllydbg,"Getsortedbyselection");
xHook::DynamicDllFun<LPDestroysorteddata> gLPDestroysorteddata(ghOllydbg,"Destroysorteddata");
xHook::DynamicDllFun<LPUpdatetable> gLPUpdatetable(ghOllydbg,"Updatetable");
xHook::DynamicDllFun<LPGetsortedbyindex> gLPGetsortedbyindex(ghOllydbg,"Getsortedbyindex");
xHook::DynamicDllFun<LPDeletesorteddata> gLPDeletesorteddata(ghOllydbg,"Deletesorteddata");
xHook::DynamicDllFun<LPActivatetablewindow> gLPActivatetablewindow(ghOllydbg,"Activatetablewindow");
xHook::DynamicDllFun<LPGetcpudisasmselection> gLPGetcpudisasmselection(ghOllydbg,"Getcpudisasmselection");
xHook::DynamicDllFun<LPGetactiveframe>	gLPGetactiveframe(ghOllydbg,"Getactiveframe");
xHook::DynamicDllFun<LPMessage> gLPMessage(ghOllydbg,"Message");
xHook::DynamicDllFun<LPSetstatus> gLPSetstatus(ghOllydbg,"Setstatus");
xHook::DynamicDllFun<LPRun> gLPRun(ghOllydbg,"Run");
xHook::DynamicDllFun<LPPastememoryhex> gLPPastememoryhex(ghOllydbg,"Pastememoryhex");
xHook::DynamicDllFun<LPRedrawcpureg> gLPRedrawcpureg(ghOllydbg,"Redrawcpureg");
xHook::DynamicDllFun<LPGetcpudisasmdump> gLPGetcpudisasmdump(ghOllydbg,"Getcpudisasmdump");
xHook::DynamicDllFun<LPRemoveanalysis>	gLPRemoveanalysis(ghOllydbg,"Removeanalysis");
xHook::DynamicDllFun<LPEnsurememorybackup> gLPEnsurememorybackup(ghOllydbg,"Ensurememorybackup");
xHook::DynamicDllFun<LPInfo> gLPInfo(ghOllydbg,"Info");
xHook::DynamicDllFun<LPAssemble> gLPAssemble(ghOllydbg,"Assemble");
xHook::DynamicDllFun<LPGetstring> gLPGetstring(ghOllydbg,"Getstring");
xHook::DynamicDllFun<LPBinaryedit> gLPBinaryedit(ghOllydbg,"Binaryedit");
xHook::DynamicDllFun<LPListmemory> gLPListmemory(ghOllydbg,"Listmemory");
xHook::DynamicDllFun<LPRemoveint3breakpoint> gLPRemoveint3breakpoint(ghOllydbg,"Removeint3breakpoint");
xHook::DynamicDllFun<LPSetint3breakpoint> gLPSetint3breakpoint(ghOllydbg,"Setint3breakpoint");
xHook::DynamicDllFun<LPEnableint3breakpoint> gLPEnableint3breakpoint(ghOllydbg,"Enableint3breakpoint");
xHook::DynamicDllFun<LPFlash> gLPFlash(ghOllydbg,"Flash");

xHook::DynamicDllFun<LPSethardbreakpoint> gLPSethardbreakpoint(ghOllydbg,"Sethardbreakpoint");
xHook::DynamicDllFun<LPSetmembreakpoint> gLPSetmembreakpoint(ghOllydbg,"Setmembreakpoint");
xHook::DynamicDllFun<LPRegistermodifiedbyuser>   gLPRegistermodifiedbyuser(ghOllydbg,"Registermodifiedbyuser");
xHook::DynamicDllFun<LPRemovemembreakpoint> gLPRemovemembreakpoint(ghOllydbg,"Removemembreakpoint");

xHook::DynamicDllFun<LPFindmodule> gLPFindmodule(ghOllydbg,"Findmodule");
xHook::DynamicDllFun<LPFindmodulebyname> gLPFindmodulebyname(ghOllydbg,"Findmodulebyname");

xHook::DynamicDllFun<LPWritememory> gLPWritememory(ghOllydbg,"Writememory");
xHook::DynamicDllFun<LPFindmemory> gLPFindmemory(ghOllydbg,"Findmemory");
xHook::DynamicDllFun<LPLabeladdress> gLPLabeladdress(ghOllydbg,"Labeladdress");

extern xHook::DynamicDllFun<LPConfirmint3breakpoint> gLPConfirmint3breakpoint(ghOllydbg,"Confirmint3breakpoint");
extern xHook::DynamicDllFun<LPConfirmhardwarebreakpoint> gLPConfirmhardwarebreakpoint(ghOllydbg,"Confirmhardwarebreakpoint");
extern xHook::DynamicDllFun<LPConfirmint3breakpointlist> gLPConfirmint3breakpointlist(ghOllydbg,"Confirmint3breakpointlist");
extern xHook::DynamicDllFun<LPWipebreakpointrange> gLPWipebreakpointrange(ghOllydbg,"Wipebreakpointrange");
extern xHook::DynamicDllFun<LPEnablemembreakpoint> gLPEnablemembreakpoint(ghOllydbg,"Enablemembreakpoint");
extern xHook::DynamicDllFun<LPRemovehardbreakpoint> gLPRemovehardbreakpoint(ghOllydbg,"Removehardbreakpoint");
extern xHook::DynamicDllFun<LPEnablehardbreakpoint> gLPEnablehardbreakpoint(ghOllydbg,"Enablehardbreakpoint");
extern xHook::DynamicDllFun<LPFindfreehardbreakslot> gLPFindfreehardbreakslot(ghOllydbg,"Findfreehardbreakslot");


xHook::DynamicDllFun<LPGetcpudisasmtable> gLPGetcpudisasmtable(ghOllydbg,"Getcpudisasmtable");
xHook::DynamicDllFun<LPSetcpu> gLPSetcpu(ghOllydbg,"Setcpu");
xHook::DynamicDllFun<LPGetexportfrommemory> gLPGetexportfrommemory(ghOllydbg,"Getexportfrommemory");

t_run*   gRuninfo = (t_run*)xHook::loadSymbol(ghOllydbg,"_run");
t_table* gMemoryTable = (t_table*)xHook::loadSymbol(ghOllydbg,"_memory");
t_table* gbpoint = (t_table*)xHook::loadSymbol(ghOllydbg,"_bpoint");
HWND* ghwollymain = (HWND*)xHook::loadSymbol(ghOllydbg,"_hwollymain");
HWND* ghwmdiclient = (HWND*)xHook::loadSymbol(ghOllydbg,"_hwclient");
COLORREF* gColor = (COLORREF*)xHook::loadSymbol(ghOllydbg,"color");
t_scheme* gScheme = (t_scheme*)xHook::loadSymbol(ghOllydbg,"scheme");

t_scheme*	gHilite = (t_scheme*)xHook::loadSymbol(ghOllydbg,"hilite");
t_font*		gFont = (t_font*)xHook::loadSymbol(ghOllydbg,"font");

LPRun		glpRunOrg = *gLPRun;
LPSetcpu	glpSetcpuOrg = *gLPSetcpu;