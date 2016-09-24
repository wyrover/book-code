#include <windows.h>
#include <tchar.h>
#include <ShellAPI.h>
#include <Shlwapi.h>
#include <ShlObj.h>
#include <Psapi.h>
#include <ShTypes.h>
#include "ollydbg2_plugin.h"
#include "xvmdebugframe.h"
#include "xvmiatcallframe.h"
#include "xvmblockframe.h"
#include "resource.h"
#include "xdynamic_ollydbg.h"
#include "xvmdebug.h"
#include "VirtualDebug/VirtualDebug.h"
#include "../../xHooklib/xhook_inline.h"
#include "../../xHooklib/xhook_peloader.h"
#include "../xVMRuntime/xvmruntime.h"
#include "../../../nNetLib/npipeclient.h"
#include "../xVMRuntime/TinyCScript/tinyc.h"
#include "../../xHooklib/xhook_pefuncs.h"
#include "../xVMRuntime/TinyCScript/crt/internal/internal.h"
#include "../../xHooklib/xhook_typedef.h"
#include "xRemoteIDA/xRemoteIDA.h"
#include "NativeSysCallID.h"
#include "bypassanti.h"
//#include "CreateAsUser.h"
#include "xApiWatch.h"
#ifndef _DEBUG
#define  VMP_PROTECT
#endif

#define NMALLOC_EXPORT 1
extern "C"{
	NMALLOC_API void*	nc_malloc(size_t size)
	{
		return (void*)malloc(size);
	}
	NMALLOC_API void	nc_free(void* ptr)
	{
		free(ptr);
	}
	NMALLOC_API void*	nc_realloc(void* ptr, size_t size)
	{
		return (void*)realloc(ptr, size);
	}
}

NPipeClient	gNoticePipe;
NPipeClient	gIDANotice;
void*	glpNoticeStack = 0;
void*	glpCheckPacketEntry = 0;
BOOL	gPacketPending = FALSE;
ULONG_PTR glpCheckPacketOuter = 0;
PVOID glpRemoteBuf = 0;
ULONG_PTR glpRemoteAddr = 0;
HANDLE ghDbgProcess = NULL;

DWORD  gScriptState = 0;
BOOL	gHasPacket = FALSE;
struct vmodule_info{
	HMODULE hModule;
	ULONG_PTR	imageBase;
	ULONG_PTR	entryPoint;
	ULONG_PTR	sizeofImage;
	wchar_t		modpath[MAX_PATH];
};
NLinkedList<vmodule_info>	gvmodList;



int CopyToClipboard(t_dump* dump,int mode)
{
	DWORD RetLen = 0;
	DWORD ReadLen = 0;
	PVOID pBuff = NULL;
	PVOID pBuff1 = NULL;
	PCHAR lpstr = NULL;

	int i;

	PBYTE p,q;

	OpenClipboard(dump->table.hw);
	EmptyClipboard();
	ReadLen = (dump->sel1 - dump->sel0) * 8 + 8;
	RetLen = dump->sel1 - dump->sel0;
	if (RetLen > 0)
	{
		pBuff1 = GlobalAlloc(GMEM_ZEROINIT,RetLen);
		pBuff = GlobalAlloc(GMEM_MOVEABLE,ReadLen);

		RetLen = gLPReadmemory(pBuff1,dump->sel0,dump->sel1 - dump->sel0,MM_REMOVEINT3 | MM_SILENT);

		p = (PBYTE)pBuff1;
		lpstr = (PCHAR)GlobalLock(pBuff);
		if (lpstr)
		{
			q = (PBYTE)lpstr;
			for (i = 0; i < (int)RetLen; i++)
			{
				if (mode == 2)
				{
					if (i == 0)
					{
						sprintf((char*)q,"0x");
						q += 2;
					} else
					{
						sprintf((char*)q,",0x");
						q += 3;
					}
				}
				sprintf((char *)q,"%02X",*p);
				q += 2;
				if (mode == 1)
					sprintf((char*)q++," ");
				p++;
			}
			*q++ = 0x0;
			GlobalUnlock(pBuff);
			SetClipboardData(CF_TEXT,(HANDLE)pBuff);
		}
		GlobalFree(pBuff1);
	}
	CloseClipboard();
	if (RetLen > 0) return 1;
	else return 0;
}

int ParseFromClipboard(t_dump* dump,int mode)
{
	DWORD RetLen = 0;
	DWORD ReadLen = 0;
	PVOID pBuff = NULL;

	PCHAR lpstr = NULL;
	PCHAR lpstr1 = NULL;
	int len = 0;
	int i = 0,j = 0;

	PBYTE p,q;

	BYTE a,b;
	int nc = 0;
	if (dump->sel0 != 0 && dump->sel1 != 0 && dump->sel1 > dump->sel0)
	{
		OpenClipboard(dump->table.hw);
		pBuff = GetClipboardData(CF_TEXT);
		if (pBuff != NULL)
		{
			lpstr = (PCHAR)GlobalLock(pBuff);
			if (lpstr != NULL)
			{
				len = strlen(lpstr);
				lpstr1 = (PCHAR)LocalAlloc(LMEM_ZEROINIT,len + 3);
				p = (PBYTE)lpstr;
				q = (PBYTE)lpstr1;

				i = 0;
				do
				{
					a = p[i];
					b = p[i + 1];

					if (a <= 'F' && a >= 'A')
						a = a - 'A' + 10;
					else if (a <= 'f' && a >= 'a')
						a = a - 'a' + 10;
					else if (a <= '9' && a >= '0')
						a = a - '0';

					if (b <= 'F' && b >= 'A')
						b = b - 'A' + 10;
					else if (b <= 'f' && b >= 'a')
						b = b - 'a' + 10;
					else if (b <= '9' && b >= '0')
						b = b - '0';

					if (a != p[i] && b != p[i + 1] && a < 16 && b < 16)
					{
						q[j++] = (a << 4) + b;
						i += 2;
					} else
						i++;
				} while (i<len);


				if ((dump->sel0 + j) >(dump->base + dump->size))
				{
					j = dump->base + dump->size - dump->sel0;
				}

				nc = j;

				t_memory* tm = gLPFindmemory(dump->sel0);
				if (tm)
					gLPEnsurememorybackup(tm,0);
				gLPWritememory(pBuff,dump->sel0,j,MM_ADJUSTINT3);
				gLPRemoveanalysis(dump->sel0,j,0);


				LocalFree(lpstr1);
				GlobalUnlock(pBuff);
			}
		}
		CloseClipboard();
	}
	return 0;
}

int CopyInfoToClipboard(t_dump* cpudump,int mode)
{
	PCHAR lpstr = NULL;
	PVOID pBuff = NULL;
	OpenClipboard(cpudump->table.hw);
	EmptyClipboard();
	pBuff = GlobalAlloc(GMEM_MOVEABLE,12);
	lpstr = (PCHAR)GlobalLock(pBuff);
	if (lpstr != NULL)
	{
		ULONG_PTR ptr;
		if (mode == 1)
		{
			if (gLPReadmemory(&ptr,cpudump->sel0,sizeof(ptr),MM_REMOVEINT3 | MM_SILENT) > 0)
				sprintf((char *)lpstr,"%08X ",ptr);
		} else
		{
			sprintf((char *)lpstr,"%08X ",cpudump->sel0);
		}

		GlobalUnlock(pBuff);
		SetClipboardData(CF_TEXT,(HANDLE)pBuff);
	}
	CloseClipboard();
	return 0;
}

int SendPacketByCurrentThread(NPacketBase* pk,BOOL singleThread = FALSE)
{
	if (!glpCheckPacketEntry)	return -1;
	ulong tid = gLPGetcputhreadid();	//取得当前cpu窗口信息
	t_thread* td = gLPFindthread(tid);
	if (!td) return -2;
	if (gPacketPending) return -4;
	int stksz = (((gxvmcfg.scp_stackSize < 8) ? 8 : gxvmcfg.scp_stackSize) - 4) * 1024;
	//在被调试进程中申请一个空间当做栈来使用
	if (!glpNoticeStack)
		glpNoticeStack = VirtualAllocEx(ghDbgProcess,0,stksz + pk->size(),MEM_COMMIT,PAGE_READWRITE);
	if (!glpNoticeStack) return -3;
	gPacketPending = TRUE;
	pk->eraseAll(xvm_vid_stackptr);
	pk->putInt64(xvm_vid_stackptr,(long long)glpNoticeStack);
	ULONG stkBottom = (ULONG)glpNoticeStack + stksz;
	stkBottom -= 60;
	//write esp first	//向我们申请的栈上写入参数
	ULONG lpesp = td->reg.r[4];
	int dwtd = gLPWritememory(pk->data(),stkBottom,pk->size(),MM_SILENT);
	if (dwtd != pk->size()) return -3;
	stkBottom -= sizeof(ULONG);
	ULONG_PTR pksize = pk->size();
	dwtd = gLPWritememory(&pksize,stkBottom,sizeof(pksize),MM_SILENT);
	if (dwtd != sizeof(pksize)) return -6;
	stkBottom -= sizeof(ULONG);
	dwtd = gLPWritememory(&lpesp,stkBottom,sizeof(ULONG),MM_SILENT);
	if (dwtd != sizeof(ULONG)) return -5;
	stkBottom -= sizeof(ULONG);
	//write return eip	写入返回地址
	dwtd = gLPWritememory(&td->reg.ip,stkBottom,sizeof(td->reg.ip),MM_SILENT);
	if (dwtd != sizeof(ULONG)) return -4;
	td->reg.r[4] = stkBottom;//修改被调试线程的esp到我们新申请的栈
	td->reg.ip = (ULONG)glpCheckPacketEntry;
	gLPRegistermodifiedbyuser(td);	
	BOOL delaySetPending = FALSE;
	if (glpCheckPacketOuter)
	{	//这里设定一个临时断点，好让我们有机会释放申请的栈空间
		if (gxvmcfg.bFreeStkEach)
		{
			gLPSetint3breakpoint(glpCheckPacketOuter,BP_TEMP | BP_EXEC | BP_NOBREAK,0,0,0,0,L"",L"",L"");
			delaySetPending = TRUE;
		}
		if (!HIBYTE(GetKeyState(VK_SHIFT)))
			glpRunOrg(singleThread?STAT_RUNTHR:STAT_RUNNING,1);
	}
	if (!delaySetPending)
		gPacketPending = FALSE;
	return 0;
}


void enableVD()
{
    EnableVirtualDebug();
    gxvmcfg.enableVirtualDebug = TRUE;
}

void disableVD()
{
    DisableVirtualDebug();
    gxvmcfg.enableVirtualDebug = FALSE;
}

int char2wchar_t(const char* str,int slen,wchar_t* dest,int dstlen)
{
	if (!dest)
		return 0;
	int wideLen = MultiByteToWideChar(CP_ACP,0,str,slen,dest,dstlen - 1);
	dest[wideLen] = 0;
	return (wideLen > 0) ? (wideLen - 1) : wideLen;
}


int getPasmFile(TCHAR* lpFilename,HWND hP)
{
	lpFilename[0] = 0;
	TCHAR szFilter[] = _T("*.pasm\0*.pasm\0*.*\0*.*\0");
	OPENFILENAME  lpofn = { 0 };
	DWORD len = 0;
	lpofn.lStructSize = sizeof (lpofn);
	lpofn.lpstrFilter = szFilter;
	lpofn.hwndOwner = hP;
	lpofn.hInstance = GetModuleHandleA(NULL);
	lpofn.lpstrFile = lpFilename;
	lpofn.Flags = OFN_HIDEREADONLY;
	lpofn.lpstrTitle = _T("Select Pattern ASM File to Run");
	std::wstring sid = getxVMDebugFullPathW(gxvmcfg.patternDir.c_str());
	lpofn.lpstrInitialDir = sid.c_str();
	lpofn.nMaxFile = 0x256;
	if (!GetOpenFileName(&lpofn) || (len = _tcslen(lpFilename)) == 0)
		return 0;
	return wcslen(lpFilename);
}


std::string WideStringToSigle(const std::wstring &singleString)
{
	if (singleString.empty())
		return std::string();

	char wbuf[65536];
	int wideLen = WideCharToMultiByte(CP_ACP,0,singleString.c_str(),-1,wbuf,sizeof(wbuf)-1,0,0);
	wbuf[wideLen] = 0;
	return wbuf;
}

std::wstring SigleStringToWide(const std::string &singleString)
{
	if (singleString.empty())
		return std::wstring();

	wchar_t wbuf[65536];
	int wideLen = MultiByteToWideChar(CP_ACP,0,singleString.c_str(),-1,wbuf,sizeof(wbuf)-1);
	wbuf[wideLen] = 0;
	return wbuf;
}
//脚本参数打包
void genRunScriptPacket(const char* filename,NPacketBase* pkopt)
{
	pkopt->putString(tinyc_vid_cfile,filename);//将脚本路径打包
	//取得相对目录
	char path[MAX_PATH * 2];
	GetModuleFileNameA(ghxVMDebug,path,sizeof(path)-1);
	int nplen = strlen(path);
	for (int i = nplen - 1; i >= 0; i--)
	{
		if (path[i] == '/' || path[i] == '\\')
		{
			path[i + 1] = 0;
			break;
		}
	}
	std::string shed = WideStringToSigle(gxvmcfg.scriptDir);
	strcat(path,shed.c_str());
	char pathHead[MAX_PATH * 2];
	int nlen = GetFullPathNameA(path,sizeof(pathHead)-1,pathHead,0);
	if (nlen < 1) return;
	if (pathHead[nlen - 1] != '\\')
		pathHead[nlen++] = '\\';
	strcpy(path,pathHead);
	//这些环境路径都是相对于设定的脚本目录写死的
	//系统包含目录
	strcat(path,"crt\\include\\");
	pkopt->putString(tinyc_vid_syspath,path);
	strcpy(path,pathHead);
	strcat(path,"crt\\include\\sec_api\\");
	pkopt->putString(tinyc_vid_syspath,path);
	strcpy(path,pathHead);
	strcat(path,"crt\\include\\sys\\");
	pkopt->putString(tinyc_vid_syspath,path);
	strcpy(path,pathHead);
	strcat(path,"crt\\include\\winapi\\");
	pkopt->putString(tinyc_vid_syspath,path);
	//一般包含目录
	strcpy(path,pathHead);
	strcat(path,"crt\\include\\common\\");
	pkopt->putString(tinyc_vid_incpath,path);
#ifdef _DEBUG
	pkopt->putString(tinyc_vid_incpath,"Q:\\Projects\\nSafeSuite\\xDark\\xTools\\xVMRuntime\\TinyCScript\\crt\\internal\\");
#else
	strcpy(path,pathHead);
	strcat(path,"crt\\include\\internal\\");
	pkopt->putString(tinyc_vid_incpath,path);
#endif // DEBUG
	//库目录
	strcpy(path,pathHead);
	strcat(path,"crt\\lib\\");
	pkopt->putString(tinyc_vid_libpath,path);
	pkopt->putString(tinyc_vid_crtpath,path);
	//这里解析设定的外部符号库
	const char* lpstr = gxvmcfg.scp_libs.c_str();
	const char* lpstrorg = lpstr;
	int szstr = gxvmcfg.scp_libs.length();
	int szlenorg = szstr;
	int wordlen = 0;
	while (const char* lpword = NStringA::getword(lpstr,szstr,wordlen,";","\t ;"))
	{
		if (wordlen < 1) break;
		std::string so;
		so.append((char*)lpword,wordlen);
		pkopt->putString(tinyc_vid_libfile,so.c_str());

		lpstr = lpword + wordlen;
		szstr = szlenorg - (lpstr - lpstrorg);
	}
}
void runScript(const char* filename)
{	//包装封包
	NPacketBase pk(xvm_notice_runscript);
	NPacketBase pkopt;
	//生成脚本运行的各种环境参数以传递给xVMRuntime模块
	genRunScriptPacket(filename,&pkopt);
	pk.putBinary(xvm_vid_scriptopt,pkopt.data(),pkopt.size());
	//发送封包
	SendPacketByCurrentThread(&pk);
}

int getOpenScriptFileName(HWND hp,char* lpfilename)
{
	char szFilter[] = "C Script(*.c)\0*.c\0*.*\0*.*\0";
	lpfilename[0] = 0;
	OPENFILENAMEA  lpofn = { 0 };
	lpofn.lStructSize = sizeof (lpofn);
	lpofn.lpstrFilter = szFilter;
	lpofn.hwndOwner = hp;
	lpofn.hInstance = GetModuleHandleA(NULL);
	lpofn.lpstrFile = lpfilename;
	lpofn.Flags = OFN_HIDEREADONLY;
	lpofn.lpstrTitle = "Select C Script To Run in Target Process";
	lpofn.nMaxFile = 0x256;
	std::string sidir = getxVMDebugFullPathA(WideStringToSigle(gxvmcfg.scriptDir).c_str());
	lpofn.lpstrInitialDir = sidir.c_str();
	if (GetOpenFileNameA(&lpofn))
		return strlen(lpfilename);
	return 0;
}

int getDllFileName(HWND hp,wchar_t* lpfilename)
{
	wchar_t szFilter[] = L"*.dll\0*.dll\0*.*\0*.*\0";
	lpfilename[0] = 0;
	OPENFILENAMEW  lpofn = { 0 };
	lpofn.lStructSize = sizeof (lpofn);
	lpofn.lpstrFilter = szFilter;
	lpofn.hwndOwner = hp;
	lpofn.hInstance = GetModuleHandleA(NULL);
	lpofn.lpstrFile = lpfilename;
	lpofn.Flags = OFN_HIDEREADONLY;
	lpofn.lpstrTitle = L"Select C Script To Run in Target Process";
	lpofn.nMaxFile = 0x256;
	if (GetOpenFileNameW(&lpofn))
		return wcslen(lpfilename);
	return 0;
}
void runScript(HWND hParent)
{
	char lpFilename[256] = { 0 };
	if (getOpenScriptFileName(hParent,lpFilename) > 0)
	{
		runScript(lpFilename);
	}
}


LRESULT WINAPI MySendMessageW(__in HWND hWnd,__in UINT Msg,__in WPARAM wParam,__in LPARAM lParam)
{
	switch (Msg)
	{
	case WM_MDIACTIVATE:
	{
		if (VMDebug_DebugMDIWindowIsActive()
			|| VMDebug_IsBlockWndActive())
			return TRUE;
	}break;
	}
	return SendMessageW(hWnd,Msg,wParam,lParam);
}

LPCreateframewindow gLPCreateFrame = 0;
t_frame*	glpCpuFrame = 0;

static xVMDebugFrame* gECpuFrame = 0;

BOOL isTableActive(t_table* tb)
{
	return (glpCpuFrame && (gLPGetactiveframe(glpCpuFrame) == tb));
}
BOOL isTraceWndActive()
{
	if ((gECpuFrame && glpCpuFrame && gLPGetactiveframe(glpCpuFrame) == gECpuFrame->table()))
		return TRUE;
	return FALSE;
}
void __cdecl  EnhanceSetcpu(ulong threadid,ulong asmaddr,ulong dumpaddr,
	ulong selsize,ulong stackaddr,int mode)
{
	if (mode & CPU_ASMFOCUS)
	{
		if (isTraceWndActive())
		{
			mode &= ~CPU_ASMFOCUS;
		}
	}
	return glpSetcpuOrg(threadid,asmaddr,dumpaddr,selsize,stackaddr,mode);
}
//原始调试函数Run的Hook函数
int __cdecl EnhanceRun(t_status status,int pass)
{	//在该函数当中我们接管了调试器原本的Run函数
	int ret = 0;
	switch (status)
	{
	case STAT_STEPIN:
	{
	}break;
	case STAT_STEPOVER:	//如果发现调试器试图触发步过操作，我们首先转入我们的块步过函数
	{
		if (VMDebug_BlockStepOver())
			return 0;
	}break;
	case STAT_RUNNING:	//同样如果发现运行操作也需要进入块执行调试函数检查
	case STAT_RUNTHR:
	{
		if (VMDebug_BlockRun(status == STAT_RUNTHR))
			return 0;
	}break;
	}
	ret = glpRunOrg(status,pass);
	return ret;
}


void VMDebug_Trace(int mode)
{
	if (!gECpuFrame)
	{
		xVMDebugFrame* fm = VMDebug_EnsureTraceFrame();
		if (!fm)
			return;
		fm->trace((trace_mode)mode);
		fm->bringToFront();
	} else
	{
		gECpuFrame->trace((trace_mode)mode);
	}


}

void setRFCtrlEnable(HWND hwndDlg,BOOL bEnable)
{
	EnableWindow(GetDlgItem(hwndDlg,IDC_EDTRF),bEnable);
	EnableWindow(GetDlgItem(hwndDlg,IDC_BTNSELRF),bEnable);
	EnableWindow(GetDlgItem(hwndDlg,IDC_STRF),bEnable);
}

void setMSCtrlEnable(HWND hwndDlg,BOOL bEnable)
{
	EnableWindow(GetDlgItem(hwndDlg,IDC_EDTMS),bEnable);
	EnableWindow(GetDlgItem(hwndDlg,IDC_BTNSELMS),bEnable);
	EnableWindow(GetDlgItem(hwndDlg,IDC_STMS),bEnable);
}


void setBKCtrlEnable(HWND hwndDlg,BOOL bEnable)
{
	EnableWindow(GetDlgItem(hwndDlg,IDC_EDTBK),bEnable);
	EnableWindow(GetDlgItem(hwndDlg,IDC_BTNSELBK),bEnable);
	EnableWindow(GetDlgItem(hwndDlg,IDC_STBK),bEnable);
}



struct IDAPipeInfo{
	BOOL		beUsing;
	BOOL		beLoaded;
	ULONG_PTR	base;
	ULONG_PTR	endaddr;
	NPipeClient	pipe;

	wchar_t	modname[SHORTNAME];
	char	peHdr[0x1000];

	void reset()
	{
		beUsing = FALSE;
		beLoaded = FALSE;
		base = 0;
		peHdr[0] = 0;
		modname[0] = 0;
		endaddr = 0;
		pipe.fpClose();
	}
};
IDAPipeInfo	gIDAPipes[MAX_IDA_INST];

int getSymbolFromIDA(long addr,wchar_t* name,int nlen,long type = sym_normal|sym_locallab)
{
	for (int i = 0; i < MAX_IDA_INST; i++)
	{
		IDAPipeInfo* pi = &gIDAPipes[i];
		if (pi->beLoaded)
		{
			if (addr >= pi->base && addr < pi->endaddr)
			{
				xida_msg_symbol retsym;
				xida_msg_syminfo getsym;
				getsym.rva = addr - pi->base;
				getsym.flags = type;
				if (pi->pipe.fpWrite(&getsym,sizeof(getsym)))
				{
					if (pi->pipe.fpRead(&retsym,sizeof(retsym)))
					{
						if (retsym.symname[0] != 0)
							return char2wchar_t(retsym.symname,-1,name,256);
					}
				}
			}
		}
	}
	return 0;
}

BOOL findSymbolFromIDA(IDAPipeInfo* pi,const char* symname,xida_msg_syminfo* retsym)
{
	xida_msg_findsym findsym;
	strncpy(findsym.symname,symname,sizeof(findsym.symname));
	if (pi->pipe.fpWrite(&findsym,sizeof(findsym)))
	{
		if (pi->pipe.fpRead(retsym,sizeof(xida_msg_syminfo)))
		{
			if (retsym->rva != -1)
				return TRUE;
		}
	}
	return FALSE;
}


static LPCexpression glpCexpression = 0;
int   Hooked_Cexpression(wchar_t *expression,uchar *cexpr,int nexpr,int *explen,wchar_t *err,ulong mode)
{
	int nlen = glpCexpression(expression,cexpr,nexpr,explen,err,mode);
	if (err && err[0] != 0 && cexpr && nexpr >= 0x10)
	{
		err[0] = 0;

		int hasEmpty = -1;
		for (int i = 0; i < MAX_IDA_INST; i++)
		{
			IDAPipeInfo* pi = &gIDAPipes[i];
			if (pi->beLoaded)
			{
				xida_msg_syminfo sym;
				char buf[256];
				for (int i = 0; i<sizeof(buf) && expression[i]; i++)
				{
					buf[i] = expression[i];
					buf[i + 1] = 0;
				}
				if (findSymbolFromIDA(pi,buf,&sym))
				{
					*cexpr++ = 0xf;
					*(long*)cexpr = sym.rva+pi->base;
					cexpr += 4;
					*(long*)cexpr = 0x35B;
					return 6;
				}
			}
		}
	}
	return nlen;
}
static LPFindlabel glpFindlabel = 0;
int	Hooked_Findlabel(ulong addr,wchar_t *name,int firsttype)
{
	int nlen = glpFindlabel(addr,name,firsttype);
	if (nlen == 0 && name)
	{
		nlen = getSymbolFromIDA(addr,name,256);
	}
	return nlen;
}

static LPDecodearglocal glpDecodearglocal = 0;
int Hooked_Decodearglocal(ulong ip,ulong offs,ulong datasize,wchar_t *name,int len)
{
	int nlen = getSymbolFromIDA(ip,name,len,sym_stack);
	if (nlen == 0)
	{
		nlen = glpDecodearglocal(ip,offs,datasize,name,len);
	}
	return nlen;
}


int getCommentFromIDA(ULONG_PTR addr,wchar_t* buf,int szbuf)
{

	for (int i = 0; i < MAX_IDA_INST; i++)
	{
		IDAPipeInfo* pi = &gIDAPipes[i];
		if (pi->beLoaded)
		{
			if (addr >= pi->base && addr < pi->endaddr)
			{
				xida_msg_getcmt getcmt;
				getcmt.rva = addr - pi->base;
				if (pi->pipe.fpWrite(&getcmt,sizeof(getcmt)))
				{
					xida_msg_comment cmt;
					if (pi->pipe.fpRead(&cmt,sizeof(cmt)))
					{
						if (cmt.comment[0] != 0)
							return char2wchar_t(cmt.comment,-1,buf,szbuf);
					}
				}
			}
		}
	}
	return 0;
}


static LPFindnameW glpFindnameW = 0;
int Hooked_FindnameW(ulong addr,int type,wchar_t *name,int nname)
{
	int nlen = glpFindnameW(addr,type,name,nname);
	if (nlen == 0)
	{


		switch (type)
		{
		case NM_COMMENT:return getCommentFromIDA(addr,name,nname);
		}
	}
	return nlen;
}


void refreshRemoteIDA()
{

	if (gxvmcfg.bRemoteIDA)
	{
		if (!glpCexpression)
		{

			void* lpFunc = GetProcAddress(ghOllydbg,"Cexpression");
			if (lpFunc)
				glpCexpression = (LPCexpression)HookCode(lpFunc,Hooked_Cexpression,HOOKTYPE_PUSH,0);
		}

		if (!glpFindlabel)
		{

			void* lpFunc = GetProcAddress(ghOllydbg,"Findlabel");
			if (lpFunc)
				glpFindlabel = (LPFindlabel)HookCode(lpFunc,Hooked_Findlabel,HOOKTYPE_PUSH,0);
		}
		if (!glpFindnameW)
		{

			void* lpFunc = GetProcAddress(ghOllydbg,"FindnameW");
			if (lpFunc)
				glpFindnameW = (LPFindnameW)HookCode(lpFunc,Hooked_FindnameW,HOOKTYPE_PUSH,0);
		}
		if (!glpDecodearglocal)
		{
			void* lpFunc = GetProcAddress(ghOllydbg,"Decodearglocal");
			if (lpFunc)
				glpDecodearglocal = (LPDecodearglocal)HookCode(lpFunc,Hooked_Decodearglocal,HOOKTYPE_PUSH,0);
		}

	} else
	{

		if (glpCexpression)
		{
			if (UnHookCode(glpCexpression))
				glpCexpression = 0;
		}

		if (glpFindlabel)
		{
			if (UnHookCode(glpFindlabel))
				glpFindlabel = 0;
		}
		if (glpFindnameW)
		{
			if (UnHookCode(glpFindnameW))
				glpFindnameW = 0;
		}
		if (glpDecodearglocal)
		{
			if (UnHookCode(glpDecodearglocal))
				glpDecodearglocal = 0;
		}
	}
}


HWND ghVMOptWnd = 0;
INT_PTR CALLBACK VMOptionsDlgProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HWND hw = GetDlgItem(hwndDlg,IDC_CMBMR);
		SendMessage(hw,CB_ADDSTRING,0,(LPARAM)_T("1000"));
		SendMessage(hw,CB_ADDSTRING,0,(LPARAM)_T("5000"));
		SendMessage(hw,CB_ADDSTRING,0,(LPARAM)_T("10000"));

		SetDlgItemInt(hwndDlg,IDC_CMBMR,gxvmcfg.vmopt.maxRecords,FALSE);
		char tmps[16];
		sprintf_s(tmps,"%08X",gxvmcfg.vmopt.stopaddr);
		SetDlgItemTextA(hwndDlg,IDC_EDTVMADDR,tmps);
		CheckDlgButton(hwndDlg,IDC_PAUSEMR,(gxvmcfg.vmopt.flags & vmopt_flags_pausemr));
		CheckDlgButton(hwndDlg,IDC_BYPASSSYS,(gxvmcfg.vmopt.flags & vmopt_flags_bypasssys));
		CheckDlgButton(hwndDlg,IDC_CHKRECORD,(gxvmcfg.vmopt.flags & vmopt_flags_recordfrompipe));
		CheckDlgButton(hwndDlg,IDC_CHKRECORDTOFILE,(gxvmcfg.vmopt.flags & vmopt_flags_recordtofile));
		CheckDlgButton(hwndDlg,IDC_RCMEM,(gxvmcfg.vmopt.flags & vmopt_flags_recordmemac));
		CheckDlgButton(hwndDlg,IDC_CHKVMSCRIPT,(gxvmcfg.vmopt.flags & vmopt_flags_vmscript));
		CheckDlgButton(hwndDlg,IDC_VMEXCEPT,(gxvmcfg.vmopt.flags & vmopt_flags_vmexception));
		CheckDlgButton(hwndDlg,IDC_VMFLSEHFRM,(gxvmcfg.vmopt.flags & vmopt_flags_vmfollowSEH));
		setRFCtrlEnable(hwndDlg,(gxvmcfg.vmopt.flags & vmopt_flags_recordtofile));
		setMSCtrlEnable(hwndDlg,(gxvmcfg.vmopt.flags & vmopt_flags_vmscript));
		CheckDlgButton(hwndDlg,IDC_VMBLOCKMODE,(gxvmcfg.vmopt.flags & vmopt_flags_vmblockmode));
		CheckDlgButton(hwndDlg,IDC_CHKRECORDBKTOFILE,(gxvmcfg.vmopt.flags & vmopt_flags_vmrecordbktofile));
		setBKCtrlEnable(hwndDlg,(gxvmcfg.vmopt.flags & vmopt_flags_vmrecordbktofile));
		CheckDlgButton(hwndDlg,IDC_VMBLOCKEXEC,(gxvmcfg.vmopt.flags & vmopt_flags_onlyblockexec));

		SetDlgItemTextW(hwndDlg,IDC_EDTRF,gxvmcfg.vmopt.rcfile.c_str());
		SetDlgItemTextW(hwndDlg,IDC_EDTMS,gxvmcfg.vmopt.msfile.c_str());
		SetDlgItemTextW(hwndDlg,IDC_EDTBK,gxvmcfg.vmopt.bkfile.c_str());
	}return TRUE;
	case WM_PAINT:
	{
		PAINTSTRUCT pt;
		HDC hc = BeginPaint(hwndDlg,&pt);
		RECT rc;
		GetClientRect(hwndDlg,&rc);
		FillRect(hc,&rc,gScheme[0].bkbrush);
		SetBkMode(hc,TRANSPARENT);
		EndPaint(hwndDlg,&pt);
	}break;
	//case WM_CTLCOLORMSGBOX:
	//case WM_CTLCOLOREDIT:
	//case WM_CTLCOLORLISTBOX:
	//case WM_CTLCOLORBTN:
	//case WM_CTLCOLORDLG:
	//case WM_CTLCOLORSCROLLBAR:
	case WM_CTLCOLORSTATIC:
	{
		SetBkMode((HDC)wParam,TRANSPARENT);
		return (INT_PTR)gScheme[0].bkbrush;
	}break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_CMBMR:
		{
			gxvmcfg.vmopt.maxRecords = GetDlgItemInt(hwndDlg,IDC_CMBMR,FALSE,FALSE);
		}break;
		case IDC_PAUSEMR:
		{
			if (IsDlgButtonChecked(hwndDlg,IDC_PAUSEMR))
				gxvmcfg.vmopt.flags |= vmopt_flags_pausemr;
			else 
				gxvmcfg.vmopt.flags &= ~vmopt_flags_pausemr;
		}break;
		case IDC_BYPASSSYS:
		{
			if (IsDlgButtonChecked(hwndDlg,IDC_BYPASSSYS))
				gxvmcfg.vmopt.flags |= vmopt_flags_bypasssys;
			else
				gxvmcfg.vmopt.flags &= ~vmopt_flags_bypasssys;
		}break;
		case IDC_CHKRECORD:
		{
			if (IsDlgButtonChecked(hwndDlg,IDC_CHKRECORD))
				gxvmcfg.vmopt.flags |= vmopt_flags_recordfrompipe;
			else
				gxvmcfg.vmopt.flags &= ~vmopt_flags_recordfrompipe;
		}break;
		case IDC_VMFLSEHFRM:{
			if (IsDlgButtonChecked(hwndDlg,IDC_VMFLSEHFRM))
				gxvmcfg.vmopt.flags |= vmopt_flags_vmfollowSEH;
			else
				gxvmcfg.vmopt.flags &= ~vmopt_flags_vmfollowSEH;
								
		}break;
		case IDC_VMBLOCKEXEC:{
			if (IsDlgButtonChecked(hwndDlg,IDC_VMBLOCKEXEC))
				gxvmcfg.vmopt.flags |= vmopt_flags_onlyblockexec;
			else
				gxvmcfg.vmopt.flags &= ~vmopt_flags_onlyblockexec;
		}break;
		case IDC_CHKRECORDTOFILE:
		{
			if (IsDlgButtonChecked(hwndDlg,IDC_CHKRECORDTOFILE))
			{
				gxvmcfg.vmopt.flags |= vmopt_flags_recordtofile;
				setRFCtrlEnable(hwndDlg,TRUE);
			}					
			else
			{
				setRFCtrlEnable(hwndDlg,FALSE);
				gxvmcfg.vmopt.flags &= ~vmopt_flags_recordtofile;
			}
				
		}break;
		case IDC_CHKRECORDBKTOFILE:
		{
			if (IsDlgButtonChecked(hwndDlg,IDC_CHKRECORDBKTOFILE))
			{
				gxvmcfg.vmopt.flags |= vmopt_flags_vmrecordbktofile;
				setBKCtrlEnable(hwndDlg,TRUE);
			} else
			{
				setBKCtrlEnable(hwndDlg,FALSE);
				gxvmcfg.vmopt.flags &= ~vmopt_flags_vmrecordbktofile;
			}
		}break;
		case IDC_VMBLOCKMODE:{
			if (IsDlgButtonChecked(hwndDlg,IDC_VMBLOCKMODE))
			{
				gxvmcfg.vmopt.flags |= vmopt_flags_vmblockmode;
			} else
			{
				gxvmcfg.vmopt.flags &= ~vmopt_flags_vmblockmode;
			}
		}break;
		case IDC_CHKVMSCRIPT:{
			if (IsDlgButtonChecked(hwndDlg,IDC_CHKVMSCRIPT))
			{
				gxvmcfg.vmopt.flags |= vmopt_flags_vmscript;
				setMSCtrlEnable(hwndDlg,TRUE);
			} else
			{
				setMSCtrlEnable(hwndDlg,FALSE);
				gxvmcfg.vmopt.flags &= ~vmopt_flags_vmscript;
			}
		}break;
		case IDC_EDTRF:
		{
			wchar_t ws[MAX_PATH * 2];
			GetDlgItemTextW(hwndDlg,IDC_EDTRF,ws,MAX_PATH);
			gxvmcfg.vmopt.rcfile = ws;
		}break;
		case IDC_EDTMS:{
			wchar_t ws[MAX_PATH * 2];
			GetDlgItemTextW(hwndDlg,IDC_EDTMS,ws,MAX_PATH);
			gxvmcfg.vmopt.msfile = ws;
		}break;
		case IDC_EDTBK:{
			wchar_t ws[MAX_PATH * 2];
			GetDlgItemTextW(hwndDlg,IDC_EDTBK,ws,MAX_PATH);
			gxvmcfg.vmopt.bkfile = ws;
		}break;
		case IDC_EDTVMADDR:{
			char tmpc[32];
			GetDlgItemTextA(hwndDlg,IDC_EDTVMADDR,tmpc,sizeof(tmpc));
			gxvmcfg.vmopt.stopaddr = strtol(tmpc,0,16);
		}break;
		case IDC_BTNSELRF:
		{
			wchar_t szFilter[] = L"Records File(*.xtrace)\0*.xtrace\0*.*\0*.*\0";
			wchar_t lpFilename[256] = { 0 };
			OPENFILENAMEW  lpofn = { 0 };
			lpofn.lStructSize = sizeof (lpofn);
			lpofn.lpstrFilter = szFilter;
			lpofn.hwndOwner = *ghwollymain;
			lpofn.hInstance = GetModuleHandle(NULL);
			lpofn.lpstrFile = lpFilename;
			lpofn.Flags = OFN_HIDEREADONLY;
			lpofn.lpstrTitle = L"Select Records File To Save";
			lpofn.lpstrDefExt = L"xtrace";
			lpofn.nMaxFile = 0x256;
			if (GetSaveFileNameW(&lpofn))
			{
				SetDlgItemTextW(hwndDlg,IDC_EDTRF,lpFilename);
				gxvmcfg.vmopt.rcfile = lpFilename;
			}
		}break;
		case IDC_BTNSELBK:{
			wchar_t szFilter[] = L"Blocks File(*.xblock)\0*.xblock\0*.*\0*.*\0";
			wchar_t lpFilename[256] = { 0 };
			OPENFILENAMEW  lpofn = { 0 };
			lpofn.lStructSize = sizeof (lpofn);
			lpofn.lpstrFilter = szFilter;
			lpofn.hwndOwner = *ghwollymain;
			lpofn.hInstance = GetModuleHandle(NULL);
			lpofn.lpstrFile = lpFilename;
			lpofn.Flags = OFN_HIDEREADONLY;
			lpofn.lpstrTitle = L"Select File To Save Blocks:";
			lpofn.lpstrDefExt = L"xblock";
			lpofn.nMaxFile = 0x256;
			if (GetSaveFileNameW(&lpofn))
			{
				SetDlgItemTextW(hwndDlg,IDC_EDTBK,lpFilename);
				gxvmcfg.vmopt.bkfile = lpFilename;
			}
		}break;
		case IDC_BTNSELMS:{
			wchar_t szFilter[] = L"C Script(*.c)\0*.c\0*.*\0*.*\0";
			wchar_t lpFilename[256] = { 0 };
			OPENFILENAMEW  lpofn = { 0 };
			lpofn.lStructSize = sizeof (lpofn);
			lpofn.lpstrFilter = szFilter;
			lpofn.hwndOwner = *ghwollymain;
			lpofn.hInstance = GetModuleHandle(NULL);
			lpofn.lpstrFile = lpFilename;
			lpofn.Flags = OFN_HIDEREADONLY;
			lpofn.lpstrTitle = L"Select Script File To Load";
			lpofn.lpstrDefExt = L"c";
			lpofn.nMaxFile = 0x256;
			if (GetOpenFileName(&lpofn) && _tcslen(lpFilename) > 0)
			{
				SetDlgItemTextW(hwndDlg,IDC_EDTMS,lpFilename);
				gxvmcfg.vmopt.msfile = lpFilename;
			}
		}break;
		case IDC_RCMEM:{
			if (IsDlgButtonChecked(hwndDlg,IDC_RCMEM))
				gxvmcfg.vmopt.flags |= vmopt_flags_recordmemac;
			else
				gxvmcfg.vmopt.flags &= ~vmopt_flags_recordmemac;
		}break;
		case IDC_VMEXCEPT:{
			if (IsDlgButtonChecked(hwndDlg,IDC_VMEXCEPT))
				gxvmcfg.vmopt.flags |= vmopt_flags_vmexception;
			else
				gxvmcfg.vmopt.flags &= ~vmopt_flags_vmexception;
		}break;
		}
	}

	}
	return DefWindowProc(hwndDlg,uMsg,wParam,lParam);
}


LRESULT CALLBACK VMOptTableProc(HWND hWnd,UINT msg,WPARAM wparm,LPARAM lparam)
{
	switch (msg)
	{
	case WM_SIZE:
	{
		if (ghVMOptWnd)
			MoveWindow(ghVMOptWnd,0,0,LOWORD(lparam),HIWORD(lparam),FALSE);
	}break;
	}
	return DefWindowProc(hWnd,msg,wparm,lparam);
}





int  npp_draw_func(wchar_t *result,uchar *mask,int *drawFlags, struct t_table *table,t_sorthdr *sorthdr,int reason,void *item)
{
	
	return 0;
}
HWND ghNppWnd = 0;
LRESULT CALLBACK nppTableProc(HWND hWnd,UINT msg,WPARAM wparm,LPARAM lparam)
{
	switch (msg)
	{
	case WM_SIZE:
	{
		if (ghNppWnd)
			MoveWindow(ghNppWnd,0,0,LOWORD(lparam),HIWORD(lparam),FALSE);
	}break;
	case WM_DESTROY:{
		if (ghNppWnd)
		{
			long style = GetWindowLong(ghNppWnd,GWL_STYLE);
			style |= (WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
			SetWindowLong(ghNppWnd,GWL_STYLE,style);

			style = GetWindowLong(ghNppWnd,GWL_EXSTYLE);
			style &= ~(WS_EX_WINDOWEDGE);
			SetWindowLong(ghNppWnd,GWL_EXSTYLE,style);
			SetParent(ghNppWnd,0);
			ghNppWnd = 0;
		}

	}break;
	}
	return DefWindowProc(hWnd,msg,wparm,lparam);
}
HWND glastDepWnd = 0;
LPBinaryedit glpBinaryeditOrg = 0;
int Hooked_Binaryedit(HWND hparent,wchar_t *title,t_hexstr *hstr,
	int letter,int x,int y,int fi,int mode)
{
	HWND hp = glastDepWnd;
	glastDepWnd = 0;
	if (hp == hparent)
		return 0;
	return glpBinaryeditOrg(hparent,title,hstr,letter,x,y,fi,mode);
}

void doFastNumberSwitch(int no,int ctrl,int alt,int shift)
{
	if (no < 0 || no >= 8) return;
	t_reg* reg = 0;
	t_thread* pcputhread = gLPFindthread(gLPGetcputhreadid());
	if (pcputhread != NULL)
		reg = (t_reg*)&pcputhread->reg;
	if (!reg) return;

	if (ctrl == 1 && alt == 0 && shift == 0)
	{
		if (gLPFindmemory(reg->r[no]))
			gLPSetcpu(0,reg->r[no],0,0,0,CPU_NOFOCUS + CPU_ASMHIST);
	} else if (ctrl == 0 && alt == 1 && shift == 0)
	{
		if (gLPFindmemory(reg->r[no]))
			gLPSetcpu(0,0,0,0,reg->r[no],CPU_NOFOCUS);
	} else if (ctrl == 0 && alt == 0 && shift == 1)
	{
		if (gLPFindmemory(reg->r[no]))
			gLPSetcpu(0,0,reg->r[no],0,0,CPU_NOFOCUS + CPU_DUMPHIST);
	}
}

TABFUNC* glpcpuasm_func = 0;
long hook_cpuasm_func(struct t_table *table,HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
	BOOL IsRet = TRUE;
	int ctrl,alt,shift;
	ctrl = (GetKeyState(VK_CONTROL) & SHIFTED) ? 1 : 0;
	shift = (GetKeyState(VK_SHIFT) & SHIFTED) ? 1 : 0;
	alt = (GetKeyState(VK_MENU) & SHIFTED) ? 1 : 0;

	t_dump *cpuasmdump = gLPGetcpudisasmdump();
	if (!cpuasmdump)
		return glpcpuasm_func(table,hwnd,msg,wparam,lparam);
	int key = (int)lparam;
	switch (msg)
	{
	case WM_USER_DBLCLK:
	{
		if (ctrl == 1 && alt == 0 && shift == 0)
		{
			// CTRL + 鼠标双击
			// 跳到下面去显示地址
			ULONG_PTR addr = gLPGetcpudisasmselection();
			gLPSetcpu(0,0,addr,0,0,CPU_DUMPHIST);
			IsRet = FALSE;
		}
	}break;
	case WM_USER_KEY:
	{
		switch(key)
		{
		case VK_RETURN:
		{
			if (ctrl == 1 && shift == 0 && alt == 0)	
			{	//在ASM窗口按“CTRL + 鼠标双击”或者“CTRL + ENTER”，表示在DUMP窗口显示选中的第一个字节开始地址的代码
				ULONG_PTR addr = gLPGetcpudisasmselection();
				gLPSetcpu(0,0,addr,0,0,CPU_DUMPHIST);
				IsRet = FALSE;
			} else
			{
				ULONG_PTR addr = gLPGetcpudisasmselection();
				t_disasm td = {0};
				uchar buf[16];
				int nlen = gLPReadmemory(buf,addr,sizeof(buf),MM_REMOVEINT3 | MM_SILENT);
				if (nlen > 0)
				{
					nlen = gLPDisasm(buf,nlen,addr,0,&td,0,0,0);
					if (nlen > 0)
					{

						for (int i = 0; i<3; i++)
						{
							if (td.op[i].features & OP_CONST)
							{
								ULONG_PTR cstaddr = td.op[i].opconst;
								if (gLPReadmemory(buf,cstaddr,sizeof(buf),MM_REMOVEINT3 | MM_SILENT) > 0)
								{
									if (!(td.jmpaddr != 0 && alt == 0))
									{
										if (shift == 0)
											gLPSetcpu(0,0,cstaddr,0,0,CPU_DUMPHIST);
										else
											gLPSetcpu(0,cstaddr,0,0,0,CPU_ASMHIST);
										IsRet = FALSE;
									}
									break;
								}
							}
						}
						if (IsRet)
						{
							if (td.memconst != 0)
							{
								if (gLPReadmemory(buf,td.memconst,sizeof(buf),MM_REMOVEINT3 | MM_SILENT) > 0)
								{
									if (!(td.jmpaddr != 0 && alt == 0))
									{
										if (shift == 0)
											gLPSetcpu(0,0,td.memconst,0,0,CPU_DUMPHIST);
										else
											gLPSetcpu(0,td.memconst,0,0,0,CPU_ASMHIST);
										IsRet = FALSE;
									}
								}
							}
						}

					}
				}
			}
		}break;
		case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
		{
			if (ctrl == 0 && alt == 0 && shift == 0)
			{
				int len = key - '0';
				void* pBuff = malloc(len);
				if (pBuff != NULL)
				{
					memset(pBuff,0x90,len);
					t_memory* tm = gLPFindmemory(cpuasmdump->sel0);
					if (tm)
						gLPEnsurememorybackup(tm,0);
					gLPWritememory(pBuff,cpuasmdump->sel0,len,MM_ADJUSTINT3);
					gLPRemoveanalysis(cpuasmdump->sel0,len,0);
					gLPUpdatetable(table,0);
					free(pBuff);
				}
				IsRet = FALSE;
			} else
				doFastNumberSwitch(key - '1',ctrl,alt,shift);
		}break;
		}
	}break;
	}
	if (IsRet)
		return glpcpuasm_func(table,hwnd,msg,wparam,lparam);
	return 0;
}

TABFUNC* glpcpudump_func = 0;
long hook_cpudump_func(struct t_table *table,HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
	BOOL IsRet = TRUE;
	int ctrl,alt,shift;
	ctrl = (GetKeyState(VK_CONTROL) & SHIFTED) ? 1 : 0;
	shift = (GetKeyState(VK_SHIFT) & SHIFTED) ? 1 : 0;
	alt = (GetKeyState(VK_MENU) & SHIFTED) ? 1 : 0;

	t_dump *cpudump = (t_dump*)table->customdata;
	if (&cpudump->table != table)
		return glpcpudump_func(table,hwnd,msg,wparam,lparam);
	int key = (int)lparam;
	switch (msg)
	{
	case WM_USER_DBLCLK:
	{
		if (ctrl == 1 && alt == 0 && shift == 0)
		{
			// CTRL + 鼠标双击
			// 跳到下面去显示地址
			gLPSetcpu(0,cpudump->sel0,0,0,0,CPU_ASMHIST);
			IsRet = FALSE;
		} else if (ctrl == 0 && alt == 1 && shift == 0)
		{
			gLPSetcpu(0,0,0,0,cpudump->sel0,CPU_ASMHIST);
			IsRet = FALSE;
		}
	}break;
	case WM_USER_MOVS:
	case WM_USER_SETS:
	case WM_USER_CNTS:
	case WM_USER_MMOV:
	{
		long iret = glpcpudump_func(table,hwnd,msg,wparam,lparam);
		if (cpudump->sel0 <= (cpudump->base + cpudump->size) && cpudump->sel0 >= cpudump->base && cpudump->sel0 <= cpudump->sel1)
		{
			DWORD RetLen = 0;
			gLPReadmemory(&RetLen,cpudump->sel0,4,MM_REMOVEINT3|MM_SILENT);
			gLPInfo(L"Start：0x%X  End：0x%X  Size：0x%X Value：0x%X",cpudump->sel0,cpudump->sel1 - 1,cpudump->sel1 - cpudump->sel0,RetLen);
		}
		return iret;
	}break;
	}
	if (IsRet)
		return glpcpudump_func(table,hwnd,msg,wparam,lparam);
	return 0;
}

//增强ollydbg调试界面函数，
HWND enhanceCpuFrame(t_frame* tfcpu,BOOL bSeted)
{
	static t_table table_npp;	//notepad++区域的t_table实例
	static t_block block_npp;
	static t_block tbkmid;
	static t_block tbvmopt;		
	static t_table table_vmopt;//实时最终记录区域实例
	static t_block block_vmpot;
	if (!bSeted)
	{
		if (!gECpuFrame)
		{	//我们的最终记录管理窗体是一个C++对象，因为其可以作为单独的窗体创建
			gECpuFrame = new xVMDebugFrame;
			//这里的意思是以非独立窗体的方式创建一个实例
			gECpuFrame->initFrame(L"ECPU",TRUE,FALSE);
		}

		//hook asm proc
		t_table* tbasm = tfcpu->block->blk1->blk1->blk1->table;
		if (tbasm && glpcpuasm_func == 0)
		{
			glpcpuasm_func = tbasm->tabfunc;
			tbasm->tabfunc = hook_cpuasm_func;
		}
		//dump table hook
		t_table* tbdump = tfcpu->block->blk2->blk1->table;
		if (tbdump && glpcpudump_func == 0)
		{
			glpcpudump_func = tbdump->tabfunc;
			tbdump->tabfunc = hook_cpudump_func;
		}


		//这里我们开始动手术了，由于t_frame结构已知，因此我们调整即可
		static t_block tbk;
		memset(&tbk,0,sizeof(tbk));
		tbk.type = BLK_VDIV;
		tbk.blk1 = tfcpu->block->blk1->blk1;
		tbk.blk2 = gECpuFrame->block();//这里调整出来一个新的垂直划分的区域我们设定为记录窗口
		tbk.percent = 60;
		tfcpu->block->blk1->blk1 = &tbk;

		memset(&tbvmopt,0,sizeof(tbvmopt));
		tbvmopt.type = BLK_HDIV;	//继续调整实时记录选项区域
		tbvmopt.blk1 = tfcpu->block->blk1->blk2;
		tbvmopt.blk2 = &block_vmpot;
		tbvmopt.percent = 70;
		tbvmopt.maxc2 = 12;
		tfcpu->block->blk1->blk2 = &tbvmopt;
		tbvmopt.blk1->table->mode |= TABLE_NOHSCR | TABLE_NOVSCR;

		memset(&block_vmpot,0,sizeof(block_vmpot));
		block_vmpot.type = BLK_TABLE;
		block_vmpot.table = &table_vmopt;

		memset(&table_vmopt,0,sizeof(table_vmopt));

		wcscpy(table_vmopt.name,L"xvm_opts");
		table_vmopt.mode = TABLE_SYNTAX | TABLE_SAVEALL | TABLE_NOBAR | TABLE_NOHSCR|TABLE_NOVSCR;
		table_vmopt.drawfunc = npp_draw_func;

	}
	ghNppWnd = FindWindow(_T("Notepad++"),0);
	if (ghNppWnd && gxvmcfg.embedNotepadPP)	//如果找到了notepad++窗体那么我们调整区域
	{
		if (tfcpu->block->blk2->blk1 != &tbkmid)
		{
			memset(&table_npp,0,sizeof(table_npp));
			wcscpy(table_npp.name,L"npp");
			table_npp.mode = TABLE_USERDEF | TABLE_NOHSCR | TABLE_NOVSCR | TABLE_NOBAR | TABLE_SAVEAPP | TABLE_SAVEPOS;
			table_npp.drawfunc = npp_draw_func;

			memset(&block_npp,0,sizeof(block_npp));
			block_npp.type = BLK_TABLE;
			block_npp.table = &table_npp;

			memset(&tbkmid,0,sizeof(tbkmid));
			tbkmid.type = BLK_VDIV;
			tbkmid.blk1 = tfcpu->block->blk2->blk1;
			tbkmid.blk2 = &block_npp;
			tbkmid.percent = 60;
			tfcpu->block->blk2->blk1 = &tbkmid;

		}
		table_npp.hw = 0;
		table_npp.drawfunc = npp_draw_func;
	} else
	{	//如果没有找到打开的notepad++那么我们以老样子呈现
		if (tfcpu->block->blk2->blk1 == &tbkmid)
			tfcpu->block->blk2->blk1 = tbkmid.blk1;
	}//调整完毕后，我们直接调用ollydbg提供的CreateFrame函数来创建出窗体
	table_vmopt.drawfunc = npp_draw_func;
	HWND hFrame = gLPCreateFrame(tfcpu,L"ICO_C",tfcpu->name);
	table_vmopt.drawfunc = 0;
	//由于我们的实时记录窗体是一个windows SDK窗体，因此我们这里创建并设定到相关区域当中
	ghVMOptWnd = CreateDialog(ghxVMDebug,(LPCWSTR)IDD_VMOPTIONS,table_vmopt.hw,VMOptionsDlgProc);
	ShowWindow(ghVMOptWnd,SW_SHOWMAXIMIZED);
	SetWindowLongPtr(table_vmopt.hw,GWL_WNDPROC,(LONG_PTR)VMOptTableProc);

	if (ghNppWnd && gxvmcfg.embedNotepadPP)
	{	//将notepad++窗体设定到相关区域当中去
		table_npp.drawfunc = 0;
		SetWindowLongPtr(table_npp.hw,GWL_WNDPROC,(LONG_PTR)nppTableProc);
		long style = GetWindowLong(ghNppWnd,GWL_STYLE);
		style &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
		SetWindowLong(ghNppWnd,GWL_STYLE,style);

		style = GetWindowLong(ghNppWnd,GWL_EXSTYLE);
		style &= ~(WS_EX_WINDOWEDGE);
		SetWindowLong(ghNppWnd,GWL_EXSTYLE,style);

		SetParent(ghNppWnd,table_npp.hw);
		ShowWindow(ghNppWnd,SW_SHOWMAXIMIZED);
	} else
		ghNppWnd = 0;
	return hFrame;
}

HWND checkAndGetCpuFrame(t_frame *pf)
{
	//如果t_frame实例里面的数据呈现未增强时的验资，那么我们增强
	if (pf && pf->block && pf->block->blk1 && pf->block->blk1->blk1 && pf->block->blk1->blk1->blk1)
	{
		t_table* ttcpu = gLPGetcpudisasmtable();
		if (pf->block->blk1->blk1->blk1->table == ttcpu)
		{
			glpCpuFrame = pf;
			return enhanceCpuFrame(glpCpuFrame,FALSE);
		} else if (pf->block->blk1->blk1->blk1->blk1 && pf->block->blk1->blk1->blk1->blk1->table == ttcpu)
		{	//如果已经增强过了，我们仅仅只是创建窗体
			glpCpuFrame = pf;
			return enhanceCpuFrame(glpCpuFrame,TRUE);
		}
	}
	return 0;
}
HWND __cdecl   MyCreateframewindow(t_frame *pf,wchar_t *icon,wchar_t *title)
{	//首先判断是否为cpu，并且是否已经创建
	HWND hWnd = checkAndGetCpuFrame(pf);
	if (hWnd) return hWnd;
	return gLPCreateFrame(pf,icon,title);
}


void enableEnhanceDebugUI()	
{
	//首先为了实现一些同步和切换功能，我们先挂掉SendMessageW函数
	//我们直接从ollydbg2的导入表上挂接
	HookOneImport((ULONG_PTR)ghOllydbg,"USER32.DLL","SendMessageW",MySendMessageW);
	//这里我们挂接掉ollydbg2的Run,Setcpu,CreateFrame 几个函数，从而控制原始的界面的某些行为
	glpRunOrg = (LPRun)HookCode(*gLPRun,EnhanceRun,HOOKTYPE_PUSH,0);
	glpSetcpuOrg = (LPSetcpu)HookCode(*gLPSetcpu,EnhanceSetcpu,HOOKTYPE_PUSH,0);
	gLPCreateFrame = (LPCreateframewindow)HookOneApi(NULL,"Createframewindow",MyCreateframewindow,HOOKTYPE_PUSH);
	glpBinaryeditOrg = (LPBinaryedit)HookOneApi(NULL,"Binaryedit",Hooked_Binaryedit,HOOKTYPE_PUSH);
	//这里取出ollydbg2导出的MDI容器窗口句柄
	HWND hMdi = *ghwmdiclient;
	IMAGE_OPTIONAL_HEADER* imoh = EnterImageOpHeader((const char*)ghOllydbg);
	DWORD szImg = imoh->SizeOfImage;
	HWND hxc = 0;
	//我们根据MDI容器查找子窗口，目的是看是否已经创建了CPU窗口，因为插件总是在ollydbg的窗口初始化完毕后才加载的
	while (hxc = FindWindowEx(hMdi,hxc,0,0))
	{	//ollydbg将窗口的t_frame类指针放到了windowslongptr里面
		void* lpPtr = (void*)GetWindowLongPtr(hxc,0);
		if (!lpPtr) continue;
		if ((ULONG_PTR)lpPtr < (ULONG_PTR)ghOllydbg || (ULONG_PTR)lpPtr >((ULONG_PTR)ghOllydbg + szImg))
			continue;	//因为ollydbg2没有导出cpu窗口的t_frame指针，因此我们需要自己匹配识别
		t_frame* tf = reinterpret_cast<t_frame*>(lpPtr);
		if ((ULONG_PTR)tf->block < (ULONG_PTR)ghOllydbg || (ULONG_PTR)tf->block >((ULONG_PTR)ghOllydbg + szImg))
			continue;
		t_block* tb1 = tf->block;
		if ((ULONG_PTR)tb1->blk1 < (ULONG_PTR)ghOllydbg || (ULONG_PTR)tb1->blk1 >((ULONG_PTR)ghOllydbg + szImg))
			continue;
		if ((ULONG_PTR)tb1->blk2 < (ULONG_PTR)ghOllydbg || (ULONG_PTR)tb1->blk2 >((ULONG_PTR)ghOllydbg + szImg))
			continue;
		//如果都通过上面的一些测试，我们基本可以判断找到的t_frame实例就是cpu的实例，因此发送消息直接将其关闭掉
		SendMessage(hxc,WM_CLOSE,0,0);
		//接着我们直接又调用Createframewindow函数重新创建其窗体，因为我们的增强代码在这个函数中实现
		MyCreateframewindow(tf,L"ICO_C",tf->name);
		break;
	}
}


syscIds gscIds;
BOOL WINAPI MyCreateProcessW(
	__in_opt    LPCWSTR lpApplicationName,
	__inout_opt LPWSTR lpCommandLine,
	__in_opt    LPSECURITY_ATTRIBUTES lpProcessAttributes,
	__in_opt    LPSECURITY_ATTRIBUTES lpThreadAttributes,
	__in        BOOL bInheritHandles,
	__in        DWORD dwCreationFlags,
	__in_opt    LPVOID lpEnvironment,
	__in_opt    LPCWSTR lpCurrentDirectory,
	__in        LPSTARTUPINFOW lpStartupInfo,
	__out       LPPROCESS_INFORMATION lpProcessInformation
)
{
	//DisableRandomAppBase(lpCommandLine);
	//CreateProcessByNativeApi(lpCommandLine,lpCurrentDirectory);
	if (gxvmcfg.enablexVMRuntime && !gxvmcfg.enableVirtualDebug)
	{
		NAutoLargeBufferA nla;//我们的伪调试必须要xVMRuntime模块注入到目标进程当中配合工作
		nla.load(gxvmcfg.vdCRT.c_str());    //我们先加载设定当中的对端模块即xVMRuntime32
		int szdata = 0;
		const char* lpdata = nla.data(szdata,0,false);
		if (!lpdata || szdata < 1)    return FALSE;
		//创建进程
		BOOL retCode = CreateProcessW(lpApplicationName,lpCommandLine,lpProcessAttributes,lpThreadAttributes,bInheritHandles,dwCreationFlags,lpEnvironment,lpCurrentDirectory,lpStartupInfo,lpProcessInformation);
		if (retCode)
		{   //如果进程创建成功，我们继续注入
			wchar_t path[MAX_PATH * 2];
			GetModuleFileNameW(0,path,sizeof(path)-1);
			wchar_t* curDir = (wchar_t*)malloc((wcslen(path) + 50)*sizeof(wchar_t));
			if (curDir)
			{
				wcscpy(curDir,path);

				int slen = wcslen(curDir);
				for (int i = slen - 1; i >= 0; i--)
				{
					if (curDir[i] == '\\' || curDir[i] == '/')
					{
						curDir[i + 1] = 0;
						break;
					}
				}

			}
			xvmrutime_args args = { 0 };
			//传递一些基本的参数，由于我们的xVMRuntime32模块也具有函数监视功能，因此我们这里要配置
			args.bEnableHeapHook = FALSE;
			args.bBreakAtOEP = FALSE;
			args.bVirtualDebugMode = FALSE;
			args.bSandbox = TRUE;
			args.bAntiAntiDebug = gxvmcfg.antiAntiDebug;
			args.bBreakOnLdr = gxvmcfg.breakOnLdrLoadDll;
			args.bEnableClientPipe = gxvmcfg.bEnableApiSpy;
			args.bEnableAlertPipe = TRUE;
			args.hostProcessID = GetCurrentProcessId();
			args.sysc = gscIds;
			args.bDisableSafeSEH = gxvmcfg.disableSafeSEH;
			args.bEnableVirtualKernel32 = gxvmcfg.bDirectImport;
			if (gxvmcfg.bStartupScript && !gxvmcfg.startupscp.empty())
			{
				NPacketBase pk;
				genRunScriptPacket(WideStringToSigle(gxvmcfg.startupscp).c_str(),&pk);
				memcpy(args.startscppk,pk.data(),pk.size());
				args.szscppk = pk.size();
			} else
				args.szscppk = 0;
			args.sdllmode = 0;
			if (gxvmcfg.iStartupDLL != 0 && !gxvmcfg.startupdll.empty())
			{
				args.sdllmode = gxvmcfg.iStartupDLL;
				wcscpy(args.startupDLL,gxvmcfg.startupdll.c_str());
			}

			long flags = XHOOK_PELOADER_FLAG_DIRECTEXEC | XHOOK_PELOADER_FLAG_NORESUMETHREAD;
			flags |= XHOOK_PELOADER_FLAG_LDRLOADDLLCALL;
			if (gxvmcfg.bReleaseStub)
				flags |= XHOOK_PELOADER_FLAG_RELEASESELF;
			HANDLE hPs = StartInProcess(lpProcessInformation->hProcess,
				lpProcessInformation->hThread,
				lpProcessInformation->dwThreadId,
				lpdata,szdata,(const char*)&args,sizeof(args),
				flags);
			if (hPs == INVALID_HANDLE_VALUE)
				retCode = FALSE;
			curDir ? free(curDir) : 0;
			if (!retCode)
			{
				TerminateProcess(lpProcessInformation->hProcess,0);
				CloseHandle(lpProcessInformation->hProcess);
				CloseHandle(lpProcessInformation->hThread);
			}
		}
		return retCode;
	}
	return CreateProcessW(lpApplicationName,lpCommandLine,lpProcessAttributes,lpThreadAttributes,bInheritHandles,dwCreationFlags,lpEnvironment,lpCurrentDirectory,lpStartupInfo,lpProcessInformation);
}


void ProcessDbgFuncCall(NPacketBase* pk)
{
	switch (pk->getMsgID())
	{
	case xvm_notice_setint3break:
	{
		int iRet = gLPSetint3breakpoint(pk->getLongLong(xvm_vid_address),pk->getInt(xvm_vid_type),pk->getInt(xvm_vid_findex),pk->getInt(xvm_vid_limit)
			,pk->getInt(xvm_vid_count),0,(wchar_t*)pk->getWString(xvm_vid_str1),(wchar_t*)pk->getWString(xvm_vid_str2),(wchar_t*)pk->getWString(xvm_vid_str3));
		NPacketBase pkret;
		pkret.putInt(xvm_vid_return,iRet);
		gNoticePipe.fpWrite(&pkret);
	}break;
	case xvm_notice_sethardbreak:
	{
		int iRet = gLPSethardbreakpoint(pk->getInt(xvm_vid_index),
			pk->getInt(xvm_vid_size),
			pk->getInt(xvm_vid_type),
			pk->getInt(xvm_vid_findex),
			pk->getLongLong(xvm_vid_address),
			pk->getInt(xvm_vid_limit),pk->getInt(xvm_vid_count),(wchar_t*)pk->getWString(xvm_vid_str1),(wchar_t*)pk->getWString(xvm_vid_str2),(wchar_t*)pk->getWString(xvm_vid_str3));
		NPacketBase pkret;
		pkret.putInt(xvm_vid_return,iRet);
		gNoticePipe.fpWrite(&pkret);
	}break;
	case xvm_notice_setmembreak:
	{
		int iRet = gLPSetmembreakpoint(pk->getLongLong(xvm_vid_address),
			pk->getInt(xvm_vid_size),
			pk->getInt(xvm_vid_type),
			pk->getInt(xvm_vid_limit),
			pk->getInt(xvm_vid_count),(wchar_t*)pk->getWString(xvm_vid_str1),(wchar_t*)pk->getWString(xvm_vid_str2),(wchar_t*)pk->getWString(xvm_vid_str3));
		NPacketBase pkret;
		pkret.putInt(xvm_vid_return,iRet);
		gNoticePipe.fpWrite(&pkret);					   
	}break;
	case xvm_notice_removeint3break:
	{
		int iRet = gLPRemoveint3breakpoint(pk->getLongLong(xvm_vid_address),
			pk->getInt(xvm_vid_type));
		NPacketBase pkret;
		pkret.putInt(xvm_vid_return,iRet);
		gNoticePipe.fpWrite(&pkret);
	}break;
	case xvm_notice_enableint3break:
	{
		int iRet = gLPEnableint3breakpoint(pk->getLongLong(xvm_vid_address),
			pk->getInt(xvm_vid_type));
		NPacketBase pkret;
		pkret.putInt(xvm_vid_return,iRet);
		gNoticePipe.fpWrite(&pkret);
	}break;
	case xvm_notice_removemembreak:
	{
		int iRet = gLPRemovemembreakpoint(pk->getLongLong(xvm_vid_address));
		NPacketBase pkret;
		pkret.putInt(xvm_vid_return,iRet);
		gNoticePipe.fpWrite(&pkret);
	}break;
	case xvm_notice_Confirmint3breakpoint:
	{
		int iRet = gLPConfirmint3breakpoint(pk->getLongLong(xvm_vid_address));
		NPacketBase pkret;
		pkret.putInt(xvm_vid_return,iRet);
		gNoticePipe.fpWrite(&pkret);
	}break;
	case xvm_notice_Confirmhardwarebreakpoint:
	{
		int iRet = gLPConfirmhardwarebreakpoint(pk->getLongLong(xvm_vid_address));
		NPacketBase pkret;
		pkret.putInt(xvm_vid_return,iRet);
		gNoticePipe.fpWrite(&pkret);
	}break;
	case xvm_notice_Wipebreakpointrange:
	{
		gLPWipebreakpointrange(pk->getLongLong(xvm_vid_addr0),pk->getLongLong(xvm_vid_addr1));
	}break;
	case xvm_notice_Enablemembreakpoint:
	{
		int iRet = gLPEnablemembreakpoint(pk->getLongLong(xvm_vid_address),pk->getInt(xvm_vid_flags));
		NPacketBase pkret;
		pkret.putInt(xvm_vid_return,iRet);
		gNoticePipe.fpWrite(&pkret);
	}break;
	case xvm_notice_Removehardbreakpoint:
	{
		int iRet = gLPRemovehardbreakpoint(pk->getInt(xvm_vid_index));
		NPacketBase pkret;
		pkret.putInt(xvm_vid_return,iRet);
		gNoticePipe.fpWrite(&pkret);
	}break;
	case xvm_notice_Enablehardbreakpoint:
	{
		int iRet = gLPEnablehardbreakpoint(pk->getInt(xvm_vid_index),pk->getInt(xvm_vid_flags));
		NPacketBase pkret;
		pkret.putInt(xvm_vid_return,iRet);
		gNoticePipe.fpWrite(&pkret);
	}break;
	case xvm_notice_Findfreehardbreakslot:
	{
		int iRet = gLPFindfreehardbreakslot(pk->getInt(xvm_vid_type));
		NPacketBase pkret;
		pkret.putInt(xvm_vid_return,iRet);
		gNoticePipe.fpWrite(&pkret);
	}break;
	}
}

NPacketBase gLastPK;
void ProcessPendingPacket()
{
	NPacketBase pk;
	DWORD dwByte = 0;
	while (gNoticePipe.fpPeek(NULL,0,NULL,0,&dwByte) && dwByte > 0)
	{
		gHasPacket = TRUE;
		pk.setHasHeader(true);
		if (!gNoticePipe.fpRead(&pk,FALSE))
			break;
		switch (pk.getMsgID())
		{
		case xvm_notice_init:{
			glpCheckPacketEntry = (void*)pk.getLongLong(xvm_vid_rtentry,0);
			glpCheckPacketOuter = (ULONG_PTR)pk.getLongLong(xvm_vid_rtouter,0);
		}break;
		case xvm_notice_scp_error:{
			ULONG_PTR addrp = pk.getLongLong(xvm_vid_address);
			const char* lpmsg = pk.getString(xvm_vid_errdesc,0);
			if (lpmsg)
			{
				int slen = strlen(lpmsg);
				wchar_t tmpbuf[1024];
				int pos = 0;
				for (int i = 0; i < slen; i++)
				{
					if (lpmsg[i] == '\n' || i==(slen -1))
					{
						if (i == (slen - 1))
							tmpbuf[pos++] = lpmsg[i];
						tmpbuf[pos] = 0;
						pos = 0;
						gLPMessage(addrp,L"%s",tmpbuf);
					} else
						tmpbuf[pos++] = lpmsg[i];
				}				
			}
		}break;
		case xvm_notice_raiseexception:
		{
			char* lpdata = 0;
			int szdata = 0;
			if (pk.getBinary(xvm_vid_subpacket,lpdata,szdata))
				gLastPK.copyfrom(lpdata,szdata,true);
		}break;
		case xvm_notice_outvm:{
			ULONG_PTR outip = pk.getLongLong(xvm_vid_rtouter);
			if (outip != 0)
			{
				int ibp = gLPSetint3breakpoint(outip,BP_ONESHOT | BP_EXEC | BP_BREAK,0,0,0,0,L"",L"",L"");
			}
			unsigned long long icount = pk.getLongLong(xvm_vid_count);
			gLPMessage(0,L"Executed %lld instructions.",icount);
		}break;
		case xvm_notice_blocks:{
			char* lpdata = 0;
			int szdata = 0;
			if (pk.getBinary(xvm_vid_bkfile,lpdata,szdata))
			{
				xVMBlockFrame* xf = VMDebug_BlockFrame();
				NPacketBase pkbody(lpdata,szdata,false);
				xf->loadBlocks(&pkbody);
			}
		}break;
		case xvm_notice_records:
		{
			if (gECpuFrame)
			{
				InstSequence* seq = gECpuFrame->Sequence();
				for (char* lprc = pk.tofirst(); lprc; lprc = pk.tonext())
				{
					switch (pk.vid())
					{
					case xvm_vid_begin:gECpuFrame->clearSeq();break;
					case xvm_vid_finished:gECpuFrame->flushSeq(); break;
					case xvm_vid_vmrecord:{
						seq->insts.push_back(*(TraceRecordEx*)lprc);
					}break;
					}
				}
			}

		}break;
		case xvm_notice_bklogs:
		{
			xVMBlockFrame* xbf = VMDebug_BlockFrame();
			if (xbf) xbf->removeLastTempExecLogs();
			int sid = 0;
			for (char* lprc = pk.tofirst(); lprc; lprc = pk.tonext())
			{
				switch (pk.vid())
				{
				case xvm_vid_begin: break;
				case xvm_vid_finished: break;
				case xvm_vid_blockexeclog:{
					if (xbf)
						sid = xbf->addBlockExecLog((SequenceExecute*)lprc,sid+1);
				}break;
				}
			}
			xbf->hiLastExecLog();
			//if (xbf) xbf->flushExecLogs();

		}break;
		case xvm_notice_mkcodebk:{
			xVMBlockFrame* xbf = VMDebug_BlockFrame();
			int seqID = pk.getInt(xvm_vid_blockid,-1);
			if (xbf)
			{
				ULONG_PTR entryaddr = (ULONG_PTR)pk.getLongLong(xvm_vid_address);
				if (entryaddr != 0)
				{
					InstSequence* seq = xbf->ensureSequence(seqID,entryaddr);
					if (seq)
					{
						seqID = seq->seqID;
						seq->uuid = pk.getInt(xvm_vid_blockuuid,-1);
						seq->flags = pk.getInt(xvm_vid_flags);
						seq->pattern = pk.getWString(xvm_vid_pattern);
						seq->comment = pk.getWString(xvm_vid_comment);
						xbf->flushSeqs();
					}
				}
			}
			NPacketBase pkret;
			pkret.putInt(xvm_vid_blockid,seqID);
			//SendPacketByCurrentThread()
			gNoticePipe.fpWrite(&pkret);
			//gNoticePipe.fpWriteEx(pkret.data(),pkret.size());
		}break;
		case xvm_notice_bkexec:
		{
			xVMBlockFrame* xbf = VMDebug_BlockFrame();
			if (xbf) xbf->removeLastTempExecLogs();
			char* lplog;
			int szbklog;
			if (pk.getBinary(xvm_vid_blockexeclog,lplog,szbklog))
			{
				if (szbklog == sizeof(SequenceExecute))
				{
					xVMBlockFrame* xbf = VMDebug_BlockFrame();
					if (xbf)
					{
						xbf->addBlockExecLog((SequenceExecute*)lplog,-1);
						xbf->hiLastExecLog();
						//xbf->flushExecLogs();
					}
				}
			}
		}break;
		case xvm_notice_dbgmode:
		{
			xVMBlockFrame* xbf = VMDebug_BlockFrame();
			if (xbf)
				xbf->setDbgMode(pk.getInt(xvm_vid_flags));
		}break;
		case xvm_notice_setmembreak:
		case xvm_notice_sethardbreak:
		case xvm_notice_setint3break:
		{
			ProcessDbgFuncCall(&pk);
		}break;
		case xvm_notice_echo:break;
		}
	}
}

void ProcessIDAPacket()
{
	NPacketBase pk;
	DWORD dwByte = 0;
	while (gIDANotice.fpPeek(NULL,0,NULL,0,&dwByte) && dwByte > 0)
	{
		gHasPacket = TRUE;
		pk.setHasHeader(true);
		if (!gIDANotice.fpRead(&pk))
			break;
		switch (pk.getMsgID())
		{
		case xida_vid_syncip:{
			ULONG_PTR edr = pk.getInt(xida_vid_address);
			int pipid = pk.getInt(xida_vid_pipeid,-1);
			if (edr != 0 && pipid != -1 && pipid < MAX_IDA_INST)
			{
				ULONG_PTR fiaddr = gIDAPipes[pipid].base + edr;
				gLPSetcpu(gLPGetcputhreadid(),fiaddr,0,1,0,CPU_NOCREATE);
				SetForegroundWindow(*ghwollymain);
				//SetActiveWindow(*ghwollymain);
				//SetFocus(*ghwollymain);

			}
		}break;
		}
	}
}

void CheckRemoteIDAConnect()
{
	if (!gxvmcfg.bRemoteIDA)
		return;
	ProcessIDAPacket();
	static DWORD lastCount = GetTickCount();
	if (GetTickCount() - lastCount < 5000)
		return;
	lastCount = GetTickCount();
	if (gRuninfo->status == STAT_IDLE)
		return;

	for (int i = 0; i < MAX_IDA_INST; i++)
	{
		IDAPipeInfo* pi = &gIDAPipes[i];
		if (!pi->beUsing)
		{
			TCHAR buf[MAX_PATH];
			wsprintfW(buf,REMOTEIDA_PIPE_TEMP,i);
			pi->pipe.SetPipeName(buf);
			if (pi->pipe.fnOpenInst())
			{
				xida_msg_getinfo msgi;
				msgi.pid = GetCurrentProcessId();
				if (pi->pipe.fpWrite(&msgi,sizeof(msgi)))
				{
					xida_msg_info	info;
					if (pi->pipe.fpRead(&info,sizeof(info)))
					{
						if (info.msgid == xida_msgid_info)
						{
							std::wstring wls = SingleStringToWide(info.modname);
							const wchar_t* lpname = wls.c_str();
							wcsncpy(pi->modname,lpname,sizeof(pi->modname) - 1);
							int clen = wcslen(pi->modname);
							int ndot = 0;
							for (int i = clen - 1; i >= 0;i--)
							{
								if (pi->modname[i] == '.')
									pi->modname[i] = (ndot++ == 0) ? 0 : '_';
							}
							memcpy(pi->peHdr,info.peHdr,sizeof(pi->peHdr));
							if (((PIMAGE_DOS_HEADER)pi->peHdr)->e_magic == IMAGE_DOS_SIGNATURE)
							{
								PIMAGE_NT_HEADERS imnh = EnterImageNtHeader(pi->peHdr);
								//pi->endaddr = pi->base + imnh->OptionalHeader.SizeOfImage;
								pi->beUsing = TRUE;
							}
							
						}
					}
				}
			}
		} else
		{
			if (!pi->pipe.CheckPipeConn())
				pi->reset();
		}
		if (pi->beUsing && !pi->beLoaded)
		{
			t_module* tm = gLPFindmodulebyname(pi->modname);
			if (tm)
			{
				pi->base = tm->base;
				pi->endaddr = tm->base + tm->size;
				pi->beLoaded = TRUE;
			}
		}
	}
}

BOOL WINAPI MyPeekMessageW(LPMSG lpMsg,HWND hWnd,UINT wMsgFilterMin,UINT wMsgFilterMax,UINT wRemoveMsg)
{
	ProcessPendingPacket();
	CheckRemoteIDAConnect();
	BOOL bRet = PeekMessageW(lpMsg,hWnd,wMsgFilterMin,wMsgFilterMax,wRemoveMsg);
	if (bRet)
	{
		//switch (lpMsg->message)
		//{
		//case WM_CHAR:
		//{

		//}break;
		//case WM_KEYDOWN:
		//{
		//	switch (LOBYTE(lpMsg->wParam))
		//	{
		//	case 0x1B:		//esc
		//	{
		//		t_table* tb = gLPGetcpudisasmtable();
		//		if (tb && tb->hw)
		//		{
		//			SendMessageW(tb->hw,WM_KEYDOWN,(WPARAM)VK_OEM_PLUS,0);
		//			SendMessageW(tb->hw,WM_KEYUP,(WPARAM)VK_OEM_PLUS,0);
		//		}
		//	}break;
		//	case 0x60:	//~
		//	{
		//		SendMessageW(*ghwollymain,WM_KEYDOWN,(WPARAM)0x2B,0);
		//		SendMessageW(*ghwollymain,WM_KEYUP,(WPARAM)0x2B,0);
		//	}break;

		//	}

		//}break;
		//}
	}
	return bRet;
}

typedef BOOL (WINAPI* LPEnumProcessModules)(
__in  HANDLE hProcess,
__out_bcount(cb) HMODULE *lphModule,
__in  DWORD cb,
__out LPDWORD lpcbNeeded
);
typedef BOOL(WINAPI* LPGetModuleInformation)(
	__in HANDLE hProcess,
	__in HMODULE hModule,
	__out LPMODULEINFO lpmodinfo,
	__in DWORD cb
	);

typedef DWORD(WINAPI* LPGetModuleFileNameExW)(
	__in HANDLE hProcess,
	__in_opt HMODULE hModule,
	__out_ecount(nSize) LPWSTR lpFilename,
	__in DWORD nSize
	);

LPEnumProcessModules glpEnumProcessModules = 0;
BOOL WINAPI MyEnumProcessModules( HANDLE hProcess, HMODULE *lphModule, DWORD cb, LPDWORD lpcbNeeded	)
{	//首先执行原始的函数
	BOOL st = glpEnumProcessModules(hProcess,lphModule,cb,lpcbNeeded);
	if (lpcbNeeded)
	{	//如果结果有效
		DWORD szHas = *lpcbNeeded;	//那么看剩余空间是否足够容纳我们虚拟载入模块
		if ((cb - *lpcbNeeded) < gvmodList.size() * sizeof(HMODULE))
		{
			*lpcbNeeded += gvmodList.size() * sizeof(HMODULE);
			return FALSE;
		}
		if (st)
		{	//如果可以，我们将虚拟模块插入结果
			HMODULE* hmv = (HMODULE*)((BYTE*)lphModule + szHas);
			for (vmodule_info* vi = gvmodList.first(); vi; vi = gvmodList.next(vi))
			{
				*hmv = vi->hModule;
				hmv++;
				*lpcbNeeded += sizeof(HMODULE);
			}
		}
	}
	return st;
}


LPGetModuleInformation glpGetModuleInformation = 0;
BOOL WINAPI MyGetModuleInformation(
	__in HANDLE hProcess,
	__in HMODULE hModule,
	__out LPMODULEINFO lpmodinfo,
	__in DWORD cb
	)
{	//遍历虚拟模块，看请求的信息是否匹配
	for (vmodule_info* vi = gvmodList.first(); vi; vi = gvmodList.next(vi))
	{
		if (vi->hModule == hModule)
		{	//如果匹配，我们传递回虚拟模块的数据，这些数据由被调试程式脚本传过来
			lpmodinfo->lpBaseOfDll = hModule;
			lpmodinfo->SizeOfImage = vi->sizeofImage;
			lpmodinfo->EntryPoint = (LPVOID)((BYTE*)hModule+vi->entryPoint);
			return TRUE;
		}
	}
	return glpGetModuleInformation(hProcess,hModule,lpmodinfo,cb);
}


LPGetModuleFileNameExW glpGetModuleFileNameExW = 0;
DWORD WINAPI MyGetModuleFileNameExW(
	__in HANDLE hProcess,
	__in_opt HMODULE hModule,
	__out_ecount(nSize) LPWSTR lpFilename,
	__in DWORD nSize
	)
{
	for (vmodule_info* vi = gvmodList.first(); vi; vi = gvmodList.next(vi))
	{	//同样如果是虚拟模块，返回一个设定的路径
		if (vi->hModule == hModule)
		{
			wcsncpy(lpFilename,vi->modpath,nSize);
			return wcslen(lpFilename);
		}
	}
	return glpGetModuleFileNameExW(hProcess,hModule,lpFilename,nSize);
}
void enableRemoteRuntime()
{
	//开一个管道随时准备接收目标进程的通知
	TCHAR name[64];
	_stprintf(name,XVMRUNTIME_COM_NOTICEPIPE,GetCurrentProcessId());
	gNoticePipe.fpCreateInst(name,FALSE);	//这里我们接管PeekMessageW给以我们处理通知封包的机会
	HookOneImport((ULONG_PTR)ghOllydbg,"USER32.DLL","PeekMessageW",MyPeekMessageW);
	LoadLibrary(_T("PSAPI.DLL"));
	glpEnumProcessModules = (LPEnumProcessModules)HookOneApi(_T("PSAPI.DLL"),"EnumProcessModules",MyEnumProcessModules,HOOKTYPE_PUSH);
	glpGetModuleInformation = (LPGetModuleInformation)HookOneApi(_T("PSAPI.DLL"),"GetModuleInformation",MyGetModuleInformation,HOOKTYPE_PUSH);
	glpGetModuleFileNameExW = (LPGetModuleFileNameExW)HookOneApi(_T("PSAPI.DLL"),"GetModuleFileNameExW",MyGetModuleFileNameExW,HOOKTYPE_PUSH);
}

int VMDebug_TraceInReal(BOOL singleThread,run_state state)
{
	NPacketBase pk(xvm_notice_entervm);
	NPacketBase pkopt;

	if (!glpCheckPacketEntry)
		return -1;
	ulong tid = gLPGetcputhreadid();
	t_thread* td = gLPFindthread(tid);
	if (!td) return -2;

	pkopt.putInt64(xvm_vid_address,td->reg.ip);
	pkopt.putInt(xvm_vid_state,state);
	if (gxvmcfg.vmopt.flags & vmopt_flags_bypasssys)
		pkopt.putEmpty(xvm_vid_bypasssys);

	if (gxvmcfg.vmopt.flags & vmopt_flags_pausemr)
		pkopt.putInt(xvm_vid_stoprecordcount,gxvmcfg.vmopt.maxRecords);

	if (gxvmcfg.vmopt.flags & vmopt_flags_recordfrompipe)
		pkopt.putEmpty(xvm_vid_recordbypipe);

	pkopt.putInt(xvm_vid_maxrc,gxvmcfg.vmopt.maxRecords);

	if (gxvmcfg.vmopt.flags & vmopt_flags_recordmemac)
		pkopt.putEmpty(xvm_vid_recordmemac);

	if (gxvmcfg.vmopt.flags & vmopt_flags_vmexception)
		pkopt.putEmpty(xvm_vid_vmexception);

	if (gxvmcfg.vmopt.flags & vmopt_flags_vmfollowSEH)
		pkopt.putEmpty(xvm_vid_vmfollowSEH);
	if (gxvmcfg.vmopt.flags & vmopt_flags_vmblockmode)
		pkopt.putEmpty(xvm_vid_vmblockmode);
	if (gxvmcfg.vmopt.stopaddr != 0)
	{
		pkopt.putInt64(xvm_vid_endaddr,gxvmcfg.vmopt.stopaddr);
	}
	if (gxvmcfg.vmopt.flags & vmopt_flags_recordtofile)
	{
		wchar_t path[MAX_PATH * 2];
		if (gxvmcfg.vmopt.rcfile[0] == '.')
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
			wcscat(path,gxvmcfg.vmopt.rcfile.c_str());
		} else
			wcscpy(path,gxvmcfg.vmopt.rcfile.c_str());
		wchar_t path2[MAX_PATH * 2];
		int nlen = GetFullPathNameW(path,sizeof(path2) / sizeof(wchar_t),path2,0);
		pkopt.putWString(xvm_vid_rcfile,path2);
	}

	if (gxvmcfg.vmopt.flags & vmopt_flags_vmrecordbktofile)
	{
		wchar_t path[MAX_PATH * 2];
		if (gxvmcfg.vmopt.bkfile[0] == '.')
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
			wcscat(path,gxvmcfg.vmopt.bkfile.c_str());
		} else
			wcscpy(path,gxvmcfg.vmopt.bkfile.c_str());
		wchar_t path2[MAX_PATH * 2];
		int nlen = GetFullPathNameW(path,sizeof(path2) / sizeof(wchar_t),path2,0);
		pkopt.putWString(xvm_vid_bkfile,path2);
	}

	if (gxvmcfg.vmopt.flags & vmopt_flags_vmscript)
	{
		if (gScriptState != main_ret_global)
		{
			wchar_t path[MAX_PATH * 2];
			if (gxvmcfg.vmopt.msfile[0] == '.')
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
				wcscat(path,gxvmcfg.vmopt.msfile.c_str());
			} else
				wcscpy(path,gxvmcfg.vmopt.msfile.c_str());
			wchar_t path2[MAX_PATH * 2];
			int nlen = GetFullPathNameW(path,sizeof(path2) / sizeof(wchar_t),path2,0);
			std::wstring sf = path2;
			std::string ss = WideStringToSigle(sf);
			NPacketBase pkscp;
			genRunScriptPacket(ss.c_str(),&pkscp);
			pkopt.putBinary(xvm_vid_scriptopt,pkscp.data(),pkscp.size());
		}
		xVMBlockFrame* xbf = VMDebug_BlockFrame();
		if (xbf)
		{
			NPacketBase pkscparg;
			OPBreakList* bps = xbf->getBreakPoints();
			for (opcode_bp* bp = bps->first(); bp;bp = bps->next(bp))
			{
				pkscparg.putInt(xvm_vid_break,bp->ip);
			}
			if (pkscparg.size() > 0)
				pkopt.putBinary(xvm_vid_breakpoints,pkscparg.data(),pkscparg.size());
		}
	}
	if (gxvmcfg.vmopt.flags & vmopt_flags_onlyblockexec)
	{
		pkopt.putEmpty(xvm_vid_onlybkexec);
		xVMBlockFrame* xbf = VMDebug_BlockFrame();
		if (xbf)
		{
			NInstSequencesMap* ism = xbf->getSequences();
			NPacketBase pkseq;
			for (InstSequence* seq = ism->first(); seq; seq = ism->next(seq))
			{
				if (seq->flags & INSTSEQ_FLAG_MONITER)
				{
					pkseq.setempty(false);
					pkseq.putInt64(xvm_vid_blockentry,seq->insts.first()->ctx1.rip);
					pkseq.putInt(xvm_vid_blockuuid,seq->uuid);
					pkseq.putInt(xvm_vid_blockid,seq->seqID);
					pkopt.putBinary(xvm_vid_block,pkseq.data(),pkseq.size());
				}
			}
		}
	}

	pk.putBinary(xvm_vid_vmopt,pkopt.data(),pkopt.size());
	return SendPacketByCurrentThread(&pk,singleThread);
}


int VMDebug_DirectStepOver(BOOL singleThread /*= FALSE*/,run_state state /*= run_run*/)
{
	NPacketBase pk(xvm_notice_dbgdirect);

	NPacketBase pkopt;

	if (!glpCheckPacketEntry)
		return -1;
	ulong tid = gLPGetcputhreadid();
	t_thread* td = gLPFindthread(tid);
	if (!td) return -2;

	pkopt.putInt64(xvm_vid_address,td->reg.ip);
	pkopt.putInt(xvm_vid_state,state);


	xVMBlockFrame* xbf = VMDebug_BlockFrame();
	if (xbf)
	{
		NPacketBase pkscparg;
		OPBreakList* bps = xbf->getBreakPoints();
		for (opcode_bp* bp = bps->first(); bp; bp = bps->next(bp))
		{
			pkscparg.putInt(xvm_vid_break,bp->ip);
		}
		if (pkscparg.size() > 0)
			pkopt.putBinary(xvm_vid_breakpoints,pkscparg.data(),pkscparg.size());
	}

	pk.putBinary(xvm_vid_options,pkopt.data(),pkopt.size());
	return SendPacketByCurrentThread(&pk,singleThread);
}


void loadNativeSysApiID()
{
	NativeSysCallID sc;
	TCHAR path[MAX_PATH];
	GetSystemDirectory(path,sizeof(path) / sizeof(TCHAR)-1);
	_tcscat(path,_T("\\NTDLL.DLL"));
	sc.setTargetFile(path);
	gscIds.iNtQueryInformationProcess = sc.getNativeSysCallID("NtQueryInformationProcess");
	gscIds.iNtSetInformationThread = sc.getNativeSysCallID("NtSetInformationThread");
	gscIds.iNtQuerySystemInformation = sc.getNativeSysCallID("NtQuerySystemInformation");
	gscIds.iNtQueryObject = sc.getNativeSysCallID("NtQueryObject");
	gscIds.iNtClose = sc.getNativeSysCallID("NtClose");
	gscIds.iNtSetContextThread = sc.getNativeSysCallID("NtSetContextThread");
	gscIds.iNtSetInformationProcess = sc.getNativeSysCallID("NtSetInformationProcess");
	sc.reset();
	gscIds.iFindowWindow = sc.getFindowWindowSysCallID();
}
LPNtSetContextThread glpNtSetContextThread = 0;

NTSTATUS NTAPI Hooked_NtSetContextThread(
	IN HANDLE ThreadHandle,
	IN PCONTEXT Context)
{
	if (gxvmcfg.bProtectDrx)
	{
		ULONG reflags = Context->ContextFlags;
		Context->ContextFlags &= ~0x10;
		NTSTATUS ist = glpNtSetContextThread(ThreadHandle,Context);
		Context->ContextFlags = reflags;
		return ist;
	}
	return glpNtSetContextThread(ThreadHandle,Context);
}


BOOL WINAPI Hooked_WaitForDebugEvent(__in LPDEBUG_EVENT lpDebugEvent,__in DWORD dwMilliseconds)
{
	static int cc = 0;
	if (gHasPacket)
	{
		gHasPacket = FALSE;
		cc = 10;
	}
	if (cc > 0)
	{
		if (dwMilliseconds == 1) dwMilliseconds = 0;
		cc--;
	}
	return WaitForDebugEvent(lpDebugEvent,dwMilliseconds);
}
void hookNeedApis()
{
	if (!glpNtSetContextThread)
	{
		glpNtSetContextThread = (LPNtSetContextThread)HookOneApi(_T("NTDLL"),"NtSetContextThread",Hooked_NtSetContextThread,HOOKTYPE_PUSH);
	}

	EnableAllProcessPrivileges();
	HookOneImport((ULONG_PTR)ghOllydbg,"KERNEL32.DLL","CreateProcessW",MyCreateProcessW);
	HookOneImport((ULONG_PTR)ghOllydbg,"KERNEL32.DLL","WaitForDebugEvent",Hooked_WaitForDebugEvent);
	//CloseProcessHandle(GetCurrentProcessId());
	//FuckParent();
	
}
#ifdef VMP_PROTECT
#pragma comment(lib,"Q:/ToolKit/VMProtect/Lib/COFF/VMProtectSDK32.lib")
#include "Q:/ToolKit/VMProtect/Include/C/VMProtectSDK.h"
#endif

int cdecl ODBG2_Pluginquery(int ollydbgversion,wchar_t unk1[SHORTNAME],
                            wchar_t pluginname[SHORTNAME],wchar_t pluginversion[SHORTNAME]) {
    if (ollydbgversion<201)
        return 0;

#ifdef VMP_PROTECT
	VMProtectBeginUltra("Startup");
#endif
    // Report name and version to OllyDbg.
    wcscpy(unk1,L"xVMDebug");
    wcscpy(pluginname,L"xVMDebug");       // Name of plugin
    wcscpy(pluginversion,L"0.2");       // Version of plugin

    TCHAR path[MAX_PATH*2];
    GetModuleFileName(0,path,sizeof(path)-1);
    TCHAR* ppfile = (TCHAR*)malloc((_tcslen(path)+50)*sizeof(TCHAR));
    if (ppfile)
    {
        _tcscpy(ppfile,path);
        _tcscpy(ppfile+_tcslen(ppfile)-4,_T(".xvmcfg"));
        loadxvmdbg_config(&gxvmcfg,ppfile);
        free(ppfile);
    }
#ifdef VMP_PROTECT
    MessageBox(GetForegroundWindow(),_T("本插件为\r\n\t《软件保护与分析技术》 \r\n\t《代码虚拟与自动化分析》\r\n随书演示程式.\n请勿用于任何除配合理解书中内容的任何其它用途。"),_T("About xVMDebug"),MB_OK|MB_ICONINFORMATION);
#endif
	hookNeedApis();
	loadNativeSysApiID();
    if (gxvmcfg.enableVirtualDebug)
        enableVD();
	if (gxvmcfg.enhancePreExecUI)
		enableEnhanceDebugUI();
	if (gxvmcfg.enablexVMRuntime)
		enableRemoteRuntime();

	refreshRemoteIDA();
#ifdef VMP_PROTECT
	VMProtectEnd();
#endif
    return 0x2010001;
}

void saveGlobalConfig(){
    TCHAR path[MAX_PATH*2];
    GetModuleFileName(0,path,sizeof(path)-1);
    TCHAR* ppfile = (TCHAR*)malloc((_tcslen(path)+50)*sizeof(TCHAR));
    if (ppfile)
    {
        _tcscpy(ppfile,path);
        _tcscpy(ppfile+_tcslen(ppfile)-4,_T(".xvmcfg"));
        savexvmdbg_config(&gxvmcfg,ppfile);
        free(ppfile);
    }
}

int cdecl ODBG2_Pluginclose(void)
{
    saveGlobalConfig();
    return 0;
}

void cdecl ODBG2_Plugindestroy(void)
{
    disableVD();
}

void cdecl ODBG2_Pluginmainloop(DEBUG_EVENT *debugevent)
{
    if (!debugevent)
        return;
	switch (debugevent->dwDebugEventCode)
	{
	case CREATE_PROCESS_DEBUG_EVENT:
	{
		ghDbgProcess = debugevent->u.CreateProcessInfo.hProcess;
	}break;
	case EXCEPTION_DEBUG_EVENT:
	{
		ULONG_PTR eaddr = (ULONG_PTR)debugevent->u.Exception.ExceptionRecord.ExceptionAddress;
		if (isHookedExecutionAddr(eaddr))
		{
			restoreAllModuleNoneExec();
			gLPListmemory();
			int tp = gLPSetint3breakpoint(eaddr,BP_ONESHOT | BP_EXEC | BP_BREAK,0,0,0,0,L"",L"",L"");
			(ULONG_PTR)debugevent->u.Exception.ExceptionRecord.ExceptionCode = 0x80000003;
		}
		if (glpCheckPacketOuter && (eaddr == glpCheckPacketOuter))
		{
			if (glpNoticeStack && gxvmcfg.bFreeStkEach)
			{
				t_thread* thd = gLPFindthread(debugevent->dwThreadId);
				if (thd)
				{
					DWORD state = 0;
					int nred = gLPReadmemory(&state,thd->tib + 0x118,sizeof(state),MM_SILENT);
					if (nred == sizeof(state))
					{
						gScriptState = state;
						VirtualFreeEx(ghDbgProcess,glpNoticeStack,0,MEM_RELEASE);
					}
					state = 0;
					gLPWritememory(&state,thd->tib + 0x118,sizeof(state),MM_SILENT | MM_WRITETHRU);
					//ULONG_PTR ptr = 0;
					//gLPWritememory(&ptr,thd->tib + 0x110,sizeof(ULONG_PTR),MM_SILENT | MM_WRITETHRU);
				}
				glpNoticeStack = 0;
				gPacketPending = FALSE;
			}
		}
		if (glpRemoteBuf && ((ULONG_PTR)debugevent->u.Exception.ExceptionRecord.ExceptionAddress == glpRemoteAddr))
		{
			VirtualFreeEx(ghDbgProcess,glpRemoteBuf,0,MEM_RELEASE);
			glpRemoteBuf = 0;
			glpRemoteAddr = 0;
		}
	}break;
	//case LOAD_DLL_DEBUG_EVENT:
	//{
	//							 int p = 0;
	//							 p++;
	//}break;
	case OUTPUT_DEBUG_STRING_EVENT:
	{	//我们只关注1个直接长度的output字串事件
		if (debugevent->u.DebugString.nDebugStringLength == 1)
		{
			if (!gLastPK.isEmpty())	//这里记录的最后一个通知封包
			{
				switch (gLastPK.getMsgID())
				{
				case LOAD_DLL_DEBUG_EVENT:
				{
					BOOL bLoaded = FALSE;	//首先我们查找这个模块是否已经被模拟载入过了
					HMODULE hmod = (HMODULE)gLastPK.getLongLong(xvm_vid_modulebase);
					for (vmodule_info* vi = gvmodList.first(); vi; vi = gvmodList.next(vi))
					{
						if (vi->hModule == hmod)
						{
							bLoaded = TRUE;
							break;
						}
					}
					if (!bLoaded)
					{		//如果没有载入，那么我们将output_debug 事件伪装成一个Load_Dll_Debug事件给调试器
						const wchar_t* lppath = gLastPK.getWString(xvm_vid_modpath);
						vmodule_info* vmodinfo = gvmodList.new_back();
						vmodinfo->hModule = hmod;
						vmodinfo->entryPoint = gLastPK.getLongLong(xvm_vid_entrypoint);
						vmodinfo->imageBase = gLastPK.getLongLong(xvm_vid_imgbase);
						vmodinfo->sizeofImage = gLastPK.getInt(xvm_vid_size);
						wcscpy(vmodinfo->modpath,lppath);
						debugevent->dwDebugEventCode = LOAD_DLL_DEBUG_EVENT;
						debugevent->u.LoadDll.lpBaseOfDll = (LPVOID)hmod;
						debugevent->u.LoadDll.dwDebugInfoFileOffset = 0;
						debugevent->u.LoadDll.nDebugInfoSize = 0;
						debugevent->u.LoadDll.lpImageName = (LPVOID)gLastPK.getLongLong(xvm_vid_path);
						debugevent->u.LoadDll.fUnicode = 1;
						debugevent->u.LoadDll.hFile = CreateFileW(lppath,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
					}
				}break;
				case EXCEPTION_DEBUG_EVENT:
				{
					debugevent->dwDebugEventCode = EXCEPTION_DEBUG_EVENT;
					debugevent->u.Exception.dwFirstChance = 0;
					debugevent->u.Exception.ExceptionRecord.ExceptionCode = 0x80000003;
					debugevent->u.Exception.ExceptionRecord.ExceptionAddress = OutputDebugStringA;
					//debugevent->u.Exception.ExceptionRecord.ExceptionFlags
				}break;
				}
				gLastPK.setempty(false);
			}
		}
	}break;
	}

    VMDebug_debugEventLoop(debugevent);
}

void resetNoticePipe()
{
	gNoticePipe.fpClose();
	TCHAR name[64];
	_stprintf(name,XVMRUNTIME_COM_NOTICEPIPE,GetCurrentProcessId());
	gNoticePipe.fpCreateInst(name,FALSE);

}

void updateECpuFrame(ULONG_PTR rip)
{
	if (!gECpuFrame)
		return;
	gECpuFrame->autoTrace(rip);
	gECpuFrame->syncByRip(rip);
}
DRAWFUNC* gcpuDrawFunc = 0;
ULONG_PTR lastEIP = 0;
int  hooked_cpudraw_func(wchar_t *result,uchar *mask,int *drawFlags,
struct t_table *table,t_sorthdr *sorthdr,int reason,void *item)
{
	int ret;
	ulong tid;
	t_thread* thread;
	if (!gcpuDrawFunc)
		return 0;
	ret = gcpuDrawFunc(result,mask,drawFlags,table,sorthdr,reason,item);
	if (reason == 0) //只关注地址栏
	{
		//当发现有绘制EIP的操作我们就进行对比
		if (drawFlags && (*drawFlags & DRAW_EIP))
		{
			tid = gLPGetcputhreadid();
			thread = gLPFindthread(tid);
			if (thread)
			{   //如果最后记录的EIP已经和当前的EIP不一样，说明有调试操作
				//触发我们的cpuRipChanged函数，在该函数中我们实现界面同步
				if (thread->context.Eip != lastEIP)
				{
					lastEIP = thread->context.Eip;
					updateECpuFrame(lastEIP);
					cpuRipChanged(lastEIP);
				}
			}
		}
	}
	return ret;
}


void VMDebug_SetupHook()
{
	t_table* tcpu = gLPGetcpudisasmtable();
	//安装我们的监控函数
	if (tcpu && tcpu->drawfunc != hooked_cpudraw_func)
	{
		if (tcpu->drawfunc != hooked_cpudraw_func)
		{
			gcpuDrawFunc = tcpu->drawfunc;
			tcpu->drawfunc = hooked_cpudraw_func;
		}
	}
}

void resetIDAPipe()
{
	if (gxvmcfg.bRemoteIDA)
	{
		gIDANotice.fpClose();
		TCHAR name1[64];
		_stprintf(name1,REMOTEIDA_PIPE_NOTICE,GetCurrentProcessId());
		gIDANotice.fpCreateInst(name1,TRUE);
	}
	for (int i = 0; i < MAX_IDA_INST;i++)
	{
		IDAPipeInfo* pi = &gIDAPipes[i];
		pi->reset();
	}

	
}
void cdecl ODBG2_Pluginreset(void)
{
	glpNoticeStack = 0;
	glpCheckPacketOuter = 0;
	gScriptState = 0;
	glpCheckPacketEntry = 0;
	gvmodList.clear();
	resetApiWatch();
	resetNoticePipe();
	resetIDAPipe();
    VMDebug_SetEndOfImage(TRUE);
    VMDebug_SetupHook();
    VMDebug_IATReset();

}


void saveOptions(HWND hwndDlg)
{
    gxvmcfg.maxPreExecLines = GetDlgItemInt(hwndDlg,IDC_MAXLINES,0,FALSE);
    gxvmcfg.maxIATTraceLines = GetDlgItemInt(hwndDlg,IDC_MAXTRACELINES,0,FALSE);
	gxvmcfg.scp_stackSize = GetDlgItemInt(hwndDlg,IDC_EDTSTACKSIZE,0,FALSE);
    gxvmcfg.preExecDetectMultiBranch = IsDlgButtonChecked(hwndDlg,IDC_CHKDETECTMB);
    gxvmcfg.breakOnLdrLoadDll = IsDlgButtonChecked(hwndDlg,IDC_CHKBREAKSYS);
    gxvmcfg.singletd = IsDlgButtonChecked(hwndDlg,IDB_CHKSINGLETD);
    gxvmcfg.bIATTraceCheckCtx = IsDlgButtonChecked(hwndDlg,IDC_CHKCONTEXT);
	gxvmcfg.loadPatternOnStartup = IsDlgButtonChecked(hwndDlg, IDC_CHKLOADPATTERN);
	gxvmcfg.enhancePreExecUI = IsDlgButtonChecked(hwndDlg,IDC_CHKENHANCEUI);
	gxvmcfg.antiAntiDebug = IsDlgButtonChecked(hwndDlg,IDC_CHKANTIANTIDBG);
	gxvmcfg.enablexVMRuntime = IsDlgButtonChecked(hwndDlg,IDC_CHKENABLEXVM);
	gxvmcfg.autoRemoveJmp = IsDlgButtonChecked(hwndDlg,IDC_AUTODELJMP);
	gxvmcfg.embedNotepadPP = IsDlgButtonChecked(hwndDlg,IDC_CHKEMBNPP);
	gxvmcfg.disableSafeSEH = IsDlgButtonChecked(hwndDlg,IDC_CHKNOSAFESEH);
	gxvmcfg.bProtectDrx = IsDlgButtonChecked(hwndDlg,IDC_CHKPTDRX);
	gxvmcfg.bDirectImport = IsDlgButtonChecked(hwndDlg,IDC_CHKDRIMPORT);
	gxvmcfg.bEnableApiSpy = IsDlgButtonChecked(hwndDlg,IDC_CHKAPISPY);
	gxvmcfg.bRemoteIDA = IsDlgButtonChecked(hwndDlg,IDC_REMOTEIDA);
	gxvmcfg.bFreeStkEach = IsDlgButtonChecked(hwndDlg,IDC_AUTOFREESTK);
	gxvmcfg.bReleaseStub = IsDlgButtonChecked(hwndDlg,IDC_RELEASESTUB);
	refreshRemoteIDA();
    wchar_t ws[MAX_PATH*2];
    GetDlgItemTextW(hwndDlg,IDC_EDTVDCRT,ws,MAX_PATH);
    gxvmcfg.vdCRT = ws;

	GetDlgItemTextW(hwndDlg, IDC_EDTPATTERNDIR, ws, MAX_PATH);
	gxvmcfg.patternDir = ws;

	GetDlgItemTextW(hwndDlg,IDC_EDTSCPCRT,ws,MAX_PATH);
	gxvmcfg.scriptDir = ws;

	char	wp[MAX_PATH * 2];
	GetDlgItemTextA(hwndDlg,IDC_EDTSCPLIBS,wp,MAX_PATH);
	gxvmcfg.scp_libs = wp;

    if (IsDlgButtonChecked(hwndDlg,IDB_CHKNOINT3))
    {
        gxvmcfg.int3cmd = 0xF4;
        gxvmcfg.int3ecode = 0xC0000096;
    }else
    {
        gxvmcfg.int3cmd = 0;
        gxvmcfg.int3ecode = 0;
    }
    if (IsDlgButtonChecked(hwndDlg,IDB_CHKNOSTEP))
    {
        gxvmcfg.stepcmd = 0xF4;
        gxvmcfg.stepecode = 0xC0000096;
    }else
    {
        gxvmcfg.stepcmd = 0;
        gxvmcfg.stepecode = 0;
    }

    saveGlobalConfig();
}

INT_PTR CALLBACK OptionsDlgProc(
        __in  HWND hwndDlg,
        __in  UINT uMsg,
        __in  WPARAM wParam,
        __in  LPARAM lParam
        )
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
    {
        SetDlgItemInt(hwndDlg,IDC_MAXLINES,gxvmcfg.maxPreExecLines,FALSE);
        SetDlgItemInt(hwndDlg,IDC_MAXTRACELINES,gxvmcfg.maxIATTraceLines,FALSE);
		SetDlgItemInt(hwndDlg,IDC_EDTSTACKSIZE,gxvmcfg.scp_stackSize,FALSE);
		CheckDlgButton(hwndDlg,IDC_CHKENHANCEUI,gxvmcfg.enhancePreExecUI);
        CheckDlgButton(hwndDlg,IDC_CHKCONTEXT,gxvmcfg.bIATTraceCheckCtx);
        CheckDlgButton(hwndDlg,IDC_CHKDETECTMB,gxvmcfg.preExecDetectMultiBranch);
        CheckDlgButton(hwndDlg,IDC_CHKBREAKSYS,gxvmcfg.breakOnLdrLoadDll);
		CheckDlgButton(hwndDlg,IDC_CHKANTIANTIDBG,gxvmcfg.antiAntiDebug);
        SetDlgItemTextW(hwndDlg,IDC_EDTVDCRT,gxvmcfg.vdCRT.c_str());
		SetDlgItemTextA(hwndDlg,IDC_EDTSCPLIBS,gxvmcfg.scp_libs.c_str());
        CheckDlgButton(hwndDlg,IDB_CHKNOINT3,!(gxvmcfg.int3cmd == 0));
        CheckDlgButton(hwndDlg,IDB_CHKNOSTEP,!(gxvmcfg.stepcmd == 0));
        CheckDlgButton(hwndDlg,IDB_CHKSINGLETD,gxvmcfg.singletd);
		CheckDlgButton(hwndDlg, IDC_CHKLOADPATTERN, gxvmcfg.loadPatternOnStartup);
		CheckDlgButton(hwndDlg,IDC_CHKENABLEXVM,gxvmcfg.enablexVMRuntime);
		CheckDlgButton(hwndDlg,IDC_AUTODELJMP,gxvmcfg.autoRemoveJmp);
		CheckDlgButton(hwndDlg,IDC_CHKEMBNPP,gxvmcfg.embedNotepadPP);
		SetDlgItemTextW(hwndDlg, IDC_EDTPATTERNDIR, gxvmcfg.patternDir.c_str());
		SetDlgItemTextW(hwndDlg,IDC_EDTSCPCRT,gxvmcfg.scriptDir.c_str());
		CheckDlgButton(hwndDlg,IDC_CHKNOSAFESEH,gxvmcfg.disableSafeSEH);
		CheckDlgButton(hwndDlg,IDC_CHKPTDRX,gxvmcfg.bProtectDrx);
		CheckDlgButton(hwndDlg,IDC_CHKDRIMPORT,gxvmcfg.bDirectImport);
		CheckDlgButton(hwndDlg,IDC_CHKAPISPY,gxvmcfg.bEnableApiSpy);
		CheckDlgButton(hwndDlg,IDC_REMOTEIDA,gxvmcfg.bRemoteIDA);
		CheckDlgButton(hwndDlg,IDC_AUTOFREESTK,gxvmcfg.bFreeStkEach);
		CheckDlgButton(hwndDlg,IDC_RELEASESTUB,gxvmcfg.bReleaseStub);
    }return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_SELCRT:
        {
            TCHAR szFilter[] =_T("*.dll\0*.dll\0*.*\0*.*\0");
            TCHAR lpFilename[256] ={0};
            OPENFILENAME  lpofn={0};
            lpofn.lStructSize = sizeof (lpofn);
            lpofn.lpstrFilter = szFilter;
            lpofn.hwndOwner = hwndDlg;
            lpofn.hInstance = GetModuleHandleA(NULL);
            lpofn.lpstrFile = lpFilename;
            lpofn.Flags = OFN_HIDEREADONLY;
            lpofn.lpstrTitle = _T("Select Virtual Debug CRT Module");
            lpofn.nMaxFile = 0x256;
            if(GetOpenFileName(&lpofn))
            {
                SetDlgItemText(hwndDlg,IDC_EDTVDCRT,lpFilename);
            }

        }break;
		case IDC_SELSCPCRT:
		{
			BROWSEINFOW brwi = { 0 };
			brwi.hwndOwner = hwndDlg;
			brwi.lpszTitle = _T("Select Script CRT Directory");
			PIDLIST_ABSOLUTE cid = SHBrowseForFolderW(&brwi);
			if (cid)
			{
				TCHAR lpFilename[256] = { 0 };
				if (SHGetPathFromIDListW(cid,lpFilename))
				{
					SetDlgItemText(hwndDlg,IDC_EDTSCPCRT,lpFilename);
				}
			}
		}break;
		case IDC_SELPATTERNDIR:
		{
			BROWSEINFOW brwi = { 0 };
			brwi.hwndOwner = hwndDlg;
			brwi.lpszTitle = _T("Select Pattern Directory");
			PIDLIST_ABSOLUTE cid = SHBrowseForFolderW(&brwi);
			if (cid)
			{
				TCHAR lpFilename[256] = { 0 };
				if (SHGetPathFromIDListW(cid, lpFilename))
				{
					SetDlgItemText(hwndDlg, IDC_EDTPATTERNDIR, lpFilename);
				}
			}
								
		}break;

        case IDOK:
        {
            saveOptions(hwndDlg);
            EndDialog(hwndDlg,IDOK);
        }break;
        case IDCANCEL:
            EndDialog(hwndDlg,IDCANCEL);
            break;
        }
    }
    return FALSE;
}



void _stdcall InjectDll(int Mode)
{
	HANDLE* hpp = (HANDLE*)GetProcAddress(ghOllydbg,"process");
	if (!hpp || *hpp == 0) return;
	wchar_t szFilter[] = L"*.dll\0*.dll\0*.*\0*.*\0";
	wchar_t lpFilename[256] = { 0 };
	OPENFILENAME  lpofn = { 0 };
	PVOID buf;
	DWORD len = 0;
	DWORD num;
	HANDLE hThread;

	lpofn.lStructSize = sizeof (lpofn);
	lpofn.lpstrFilter = szFilter;
	lpofn.hwndOwner = NULL;
	lpofn.hInstance = GetModuleHandleA(NULL);
	lpofn.lpstrFile = lpFilename;
	lpofn.Flags = OFN_HIDEREADONLY;
	lpofn.lpstrTitle = L"Select Dll to Inject";
	lpofn.nMaxFile = 0x256;

	if (!GetOpenFileNameW(&lpofn) || (len = wcslen(lpFilename)) == 0)
		return;
	static BYTE shellCodeTemp[] = {
		0x6A,0x00,0x60,0x9C,0xE8,0x00,0x00,0x00,0x00,0x5E,0x81,0xE6,0x00,0x00,0xFF,0xFF,0xAD,0x89,0x44,0x24,0x24,0xAD,0x56,0xFF,0xD0,0x9D,0x61,0xC3
	};
	PVOID pbuf;
	HANDLE hProc = *hpp;
	//Mode 1，RemoteThread
	switch (Mode)
	{
	case 1:
	{	//remote thread mode
		HANDLE hThread = RemoteInjectDLL(hProc,lpFilename,XHOOK_PELOADER_FLAG_EXITTHREAD | XHOOK_PELOADER_FLAG_RELEASESELF,0);
		CloseHandle(hThread);
	}break;
	case 2:
	{	//current dll mode
		t_thread *pt;
		if (!(gRuninfo->status == STAT_PAUSED))
		{
			MessageBox(*ghwollymain,L"Suspend Process First!",L"Error",MB_ICONERROR);
			return;
		}
		DWORD tid = gLPGetcputhreadid();
		if (tid && (pt = gLPFindthread(tid)))
		{
			if (glpRemoteBuf == NULL)
				glpRemoteBuf = VirtualAllocEx(hProc,NULL,0x1000,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
			if (glpRemoteBuf == NULL)return;
			pbuf = (PVOID)GlobalAlloc(GMEM_ZEROINIT,0x1000);
			//Sethardwarebreakpoint(pt->reg.ip,1,HB_ONESHOT);
			int lsn = gLPSetint3breakpoint(pt->reg.ip,BP_TEMP | BP_EXEC | BP_NOBREAK,0,0,0,0,L"",L"",L"");
			*(ULONG_PTR*)pbuf = pt->reg.ip;
			glpRemoteAddr = pt->reg.ip;
			*(ULONG_PTR*)((ULONG_PTR)pbuf + 4) = (ULONG_PTR)GetProcAddress(GetModuleHandle(_T("kernel32.dll")),"LoadLibraryW");
			wcscpy((wchar_t*)((ULONG_PTR)pbuf + 8),lpFilename);
			memcpy((void*)((ULONG_PTR)pbuf + len * 2 + 8 + 2),(void*)shellCodeTemp,sizeof(shellCodeTemp));
			pt->reg.ip = (ulong)((ULONG_PTR)glpRemoteBuf + len * 2 + 8 + 2);
			WriteProcessMemory(hProc,glpRemoteBuf,pbuf,0x1000,&num);
			gLPListmemory();
			gLPSetcpu(tid,pt->reg.ip,0,NULL,NULL,CPU_REDRAW);
			GlobalFree((HGLOBAL)pbuf);
		}
	}break;
	case 3:
	{//remote virtual mode
		NAutoLargeBufferA nla;
		nla.load(lpFilename);  
		int szdata = 0;
		const char* lpdata = nla.data(szdata,0,false);
		if (lpdata && szdata > 0)
		{
			HANDLE hThread = RemoteInjectVDLL(hProc,lpdata,szdata,0,0,XHOOK_PELOADER_FLAG_EXITTHREAD | XHOOK_PELOADER_FLAG_RELEASESELF,0);
			CloseHandle(hThread);
		}
	}break;
	}

}


enum xvm_menus{
    xvm_menu_about = 0
    ,xvm_menu_vmtrace
    ,xvm_menu_vmtracesel
    ,xvm_menu_blockexec
    ,xvm_menu_findiatcalls
    ,xvm_menu_findiatcallsmerge
    ,xvm_menu_options
    ,xvm_menu_iatmemaddr
    ,xvm_menu_setIAT
    ,xvm_menu_traceFixIAT
    ,xvm_menu_opbranching
    ,xvm_menu_enablevirtualdebug
    ,xvm_menu_analyserop
    ,xvm_menu_setendofiatmemaddr
    ,xvm_menu_colorcontext
	,xvm_menu_script
	,xvm_menu_runscript
	,xvm_menu_pattern
	,xvm_menu_runpattern
	,xvm_menu_xas
	,xvm_menu_script1
	,xvm_menu_script2
	,xvm_menu_script3
	,xvm_menu_script4
	,xvm_menu_script5
	,xvm_menu_xas1
	,xvm_menu_xas2
	,xvm_menu_xas3
	,xvm_menu_xas4
	,xvm_menu_xas5
	,xvm_menu_vmtracereal
	,xvm_menu_font
	,xvm_menu_font1
	,xvm_menu_font2
	,xvm_menu_font3
	,xvm_menu_font4
	,xvm_menu_font5
	,xvm_menu_font6
	,xvm_menu_font7
	,xvm_menu_font8
	,xvm_menu_showscroll
	,xvm_menu_colours
	,xvm_menu_colour1
	,xvm_menu_colour2
	,xvm_menu_colour3
	,xvm_menu_colour4
	,xvm_menu_colour5
	,xvm_menu_colour6
	,xvm_menu_colour7
	,xvm_menu_colour8
	,xvm_menu_highlithing
	,xvm_menu_highlithing1
	,xvm_menu_highlithing2
	,xvm_menu_highlithing3
	,xvm_menu_highlithing4
	,xvm_menu_highlithing5
	,xvm_menu_highlithing6
	,xvm_menu_highlithing7
	,xvm_menu_highlithing8
	,xvm_menu_startupscript
	,xvm_menu_blockentrys
	,xvm_menu_remoteida
	,xvm_menu_injectdll
	,xvm_menu_ijdllcthread
	,xvm_menu_ijdllrthread
	,xvm_menu_ijdllstartup
	,xvm_menu_ijdllvc
	,xvm_menu_ijdllvr
	,xvm_menu_byte
	,xvm_menu_gotoesp
	,xvm_menu_gotoebp
	,xvm_menu_return
	,xvm_menu_shiftreturn
	,xvm_menu_ctrlreturn
	,xvm_menu_altreturn
	,xvm_menu_sandbox
	,xvm_menu_misc
	,xvm_menu_apientry
	,xvm_menu_apiret
};



static int MenuProc(t_table *pt,wchar_t *name,ulong index,int mode) {
    if (mode==MENU_VERIFY)
    {
		switch (index)
		{
		case xvm_menu_enablevirtualdebug:return gxvmcfg.enableVirtualDebug ? MENU_CHECKED : MENU_NORMAL;
		case xvm_menu_startupscript:
		{
			if (gxvmcfg.startupscp.empty())
			{
				wcscpy(name,L"Startup Script:Empty...");
			} else
			{
				wsprintf(name,L"Startup Script:%s",gxvmcfg.startupscp.c_str());
			}
			return gxvmcfg.bStartupScript ? MENU_CHECKED : MENU_NORMAL;
		}break;
		case xvm_menu_ijdllstartup:
		{
			if (gxvmcfg.startupdll.empty())
			{
				wcscpy(name,L"Startup:Empty...");
			} else
			{
				if (gxvmcfg.iStartupDLL == 2)
					wsprintf(name,L"[V]Startup:%s",gxvmcfg.startupdll.c_str());
				else
					wsprintf(name,L"Startup:%s",gxvmcfg.startupdll.c_str());
			}
			return (gxvmcfg.iStartupDLL != 0) ? MENU_CHECKED : MENU_NORMAL;
		}break;
		}

        return MENU_NORMAL;                // Always available
    }
    else if (mode==MENU_EXECUTE)
    {
        switch(index)
        {
        case xvm_menu_vmtrace:VMDebug_Trace(trace_ateip);break;
        case xvm_menu_vmtracesel:VMDebug_Trace(trace_atsel);break;
        case xvm_menu_blockexec:VMDebug_BlockExec();break;
        case xvm_menu_findiatcallsmerge:VMDebug_FindIATCalls(TRUE);break;
        case xvm_menu_findiatcalls:VMDebug_FindIATCalls(FALSE);break;
        case xvm_menu_setendofiatmemaddr:VMDebug_SetEndOfImage(FALSE);break;
        case xvm_menu_iatmemaddr:VMDebug_AddtoIATCalls(pt);break;
        case xvm_menu_setIAT:VMDebug_SetIAT(pt,FALSE);break;
        case xvm_menu_traceFixIAT:VMDebug_SetIAT(pt,TRUE);break;
        case xvm_menu_opbranching:VMDebug_BlockSetBranching();break;
		case xvm_menu_vmtracereal:VMDebug_TraceInReal(); break;
		case xvm_menu_startupscript:
		{
			if (HIBYTE(GetKeyState(VK_SHIFT)) ||	gxvmcfg.startupscp.empty())
			{
				char lpf[MAX_PATH];
				if (getOpenScriptFileName(*ghwollymain,lpf) > 0)
				{
					std::string sf = lpf;
					gxvmcfg.startupscp = SingleStringToWide(sf);
				} else
				{
					gxvmcfg.startupscp.clear();
				}
			} else
			{
				gxvmcfg.bStartupScript = !gxvmcfg.bStartupScript;
			}
		}break;
		case xvm_menu_ijdllstartup:
		{
			if (HIBYTE(GetKeyState(VK_SHIFT)) || gxvmcfg.startupdll.empty())
			{
				wchar_t lpf[MAX_PATH];
				if (getDllFileName(*ghwollymain,lpf) > 0)
				{
					gxvmcfg.startupdll = lpf;
				} else
				{
					gxvmcfg.startupdll.clear();
				}
			} else
			{
				if (gxvmcfg.iStartupDLL == 0)
				{
					if (HIBYTE(GetKeyState(VK_CONTROL)))
						gxvmcfg.iStartupDLL = 2;
					else
						gxvmcfg.iStartupDLL = 1;
				} else
					gxvmcfg.iStartupDLL = 0;
			}
		}break;
        case xvm_menu_options:
        {
            if (ghwollymain)
                DialogBox((HINSTANCE)ghxVMDebug,MAKEINTRESOURCE(IDD_OPTIONS),*ghwollymain,OptionsDlgProc);
        }break;
        case xvm_menu_enablevirtualdebug:
        {
            if (!gxvmcfg.enableVirtualDebug)
                enableVD();
            else
                disableVD();
        }break;
        case xvm_menu_colorcontext:
        {
            t_thread* thd = gLPFindthread(gLPGetcputhreadid());
            if (thd)
            {
                thd->reg.r[0] = 0x11111111;
                thd->reg.r[1] = 0x22222222;
                thd->reg.r[2] = 0x33333333;
                thd->reg.r[3] = 0x44444444;
                //thd->reg.r[0] = 0x11111111;
                thd->reg.r[5] = 0x55555555;
                thd->reg.r[6] = 0x66666666;
                thd->reg.r[7] = 0x77777777;
                gLPRegistermodifiedbyuser(thd);
				gLPRedrawcpureg();
            }
        }break;
        case xvm_menu_analyserop:VMDebug_DetectOPBranch();break;
		case xvm_menu_runscript:runScript(*ghwollymain); break;
		case xvm_menu_blockentrys: VMDebug_AddtoBlockEntries(pt); break;
		case xvm_menu_ijdllcthread:InjectDll(2); break;
		case xvm_menu_ijdllrthread:InjectDll(1); break;
		case xvm_menu_ijdllvr:InjectDll(3); break;
		case xvm_menu_apientry:xApi_StepOver(); break;
		case xvm_menu_apiret:xApi_RunToReturn(); break;
        default:
        {
				   MessageBox(GetForegroundWindow(),_T("本插件为\r\n\t《软件保护与分析技术》\r\n\t《代码虚拟与自动化分析》\r\n随书演示程式.\n请勿用于任何除配合理解书中内容的任何其它用途。"),_T("About xVMDebug"),MB_OK | MB_ICONINFORMATION);
		}break;
        }
        return MENU_NOREDRAW;
    };
    return MENU_ABSENT;
}


void syncIDAIp()
{
	ULONG_PTR addr = gLPGetcpudisasmselection();
	t_module* tm = gLPFindmodule(addr);
	if (!tm) return;
	for (int i = 0; i < MAX_IDA_INST; i++)
	{
		IDAPipeInfo* pi = &gIDAPipes[i];
		if (pi->beLoaded)
		{
			if (pi->base == tm->base)
			{
				xida_msg_setip setip;
				setip.rva = addr - pi->base;
				pi->pipe.fpWrite(&setip,sizeof(setip));
			}
		}
	}
}

enum xvm_idamenus{
	xvm_idamenu_syncip = 0
};

static int IDAMenuProc(t_table *pt,wchar_t *name,ulong index,int mode)
{
	if (mode == MENU_VERIFY)
	{

		return MENU_NORMAL;                // Always available
	} else if (mode == MENU_EXECUTE)
	{
		switch (index)
		{
		case xvm_idamenu_syncip: syncIDAIp();break;
		}
	}
	return MENU_ABSENT;
}

static t_menu idamenu[] = {
	{ L"Sync IP to IDA",L"Sync IP to IDA",KK_DIRECT | KK_CTRL | KK_ALT | 'S',IDAMenuProc,NULL,xvm_idamenu_syncip },
	{ NULL,NULL,K_NONE,NULL,NULL,0 }
};


enum {
	xvm_frmenu_ctrlnum1
	,xvm_frmenu_ctrlnum2
	,xvm_frmenu_ctrlnum3
	,xvm_frmenu_ctrlnum4
	,xvm_frmenu_ctrlnum5
	,xvm_frmenu_ctrlnum6
	,xvm_frmenu_ctrlnum7
	,xvm_frmenu_ctrlnum8
	,xvm_frmenu_altnum1
	,xvm_frmenu_altnum2
	,xvm_frmenu_altnum3
	,xvm_frmenu_altnum4
	,xvm_frmenu_altnum5
	,xvm_frmenu_altnum6
	,xvm_frmenu_altnum7
	,xvm_frmenu_altnum8
	,xvm_frmenu_shiftnum1
	,xvm_frmenu_shiftnum2
	,xvm_frmenu_shiftnum3
	,xvm_frmenu_shiftnum4
	,xvm_frmenu_shiftnum5
	,xvm_frmenu_shiftnum6
	,xvm_frmenu_shiftnum7
	,xvm_frmenu_shiftnum8
};

static int fastRegMenuProc(t_table *pt,wchar_t *name,ulong index,int mode)
{
	if (mode == MENU_VERIFY)
	{
		return MENU_SHORTCUT;                // Always available
	} else if (mode == MENU_EXECUTE)
	{
		switch (index)
		{
		case xvm_frmenu_ctrlnum1:doFastNumberSwitch(0,1,0,0); break;
		case xvm_frmenu_ctrlnum2:doFastNumberSwitch(1,1,0,0); break;
		case xvm_frmenu_ctrlnum3:doFastNumberSwitch(2,1,0,0); break;
		case xvm_frmenu_ctrlnum4:doFastNumberSwitch(3,1,0,0); break;
		case xvm_frmenu_ctrlnum5:doFastNumberSwitch(4,1,0,0); break;
		case xvm_frmenu_ctrlnum6:doFastNumberSwitch(5,1,0,0); break;
		case xvm_frmenu_ctrlnum7:doFastNumberSwitch(6,1,0,0); break;
		case xvm_frmenu_ctrlnum8:doFastNumberSwitch(7,1,0,0); break;
		case xvm_frmenu_altnum1:doFastNumberSwitch(0,0,1,0); break;
		case xvm_frmenu_altnum2:doFastNumberSwitch(1,0,1,0); break;
		case xvm_frmenu_altnum3:doFastNumberSwitch(2,0,1,0); break;
		case xvm_frmenu_altnum4:doFastNumberSwitch(3,0,1,0); break;
		case xvm_frmenu_altnum5:doFastNumberSwitch(4,0,1,0); break;
		case xvm_frmenu_altnum6:doFastNumberSwitch(5,0,1,0); break;
		case xvm_frmenu_altnum7:doFastNumberSwitch(6,0,1,0); break;
		case xvm_frmenu_altnum8:doFastNumberSwitch(7,0,1,0); break;
		case xvm_frmenu_shiftnum1:doFastNumberSwitch(0,0,0,1); break;
		case xvm_frmenu_shiftnum2:doFastNumberSwitch(1,0,0,1); break;
		case xvm_frmenu_shiftnum3:doFastNumberSwitch(2,0,0,1); break;
		case xvm_frmenu_shiftnum4:doFastNumberSwitch(3,0,0,1); break;
		case xvm_frmenu_shiftnum5:doFastNumberSwitch(4,0,0,1); break;
		case xvm_frmenu_shiftnum6:doFastNumberSwitch(5,0,0,1); break;
		case xvm_frmenu_shiftnum7:doFastNumberSwitch(6,0,0,1); break;
		case xvm_frmenu_shiftnum8:doFastNumberSwitch(7,0,0,1); break;
		}
	}
	return MENU_ABSENT;
}

static t_menu fastRegmenu[] = {
	{ L"NONE",L"NONE",KK_DIRECT | KK_CTRL | '1',fastRegMenuProc,NULL,xvm_frmenu_ctrlnum1 },
	{ L"NONE",L"NONE",KK_DIRECT | KK_CTRL | '2',fastRegMenuProc,NULL,xvm_frmenu_ctrlnum2 },
	{ L"NONE",L"NONE",KK_DIRECT | KK_CTRL | '3',fastRegMenuProc,NULL,xvm_frmenu_ctrlnum3 },
	{ L"NONE",L"NONE",KK_DIRECT | KK_CTRL | '4',fastRegMenuProc,NULL,xvm_frmenu_ctrlnum4 },
	{ L"NONE",L"NONE",KK_DIRECT | KK_CTRL | '5',fastRegMenuProc,NULL,xvm_frmenu_ctrlnum5 },
	{ L"NONE",L"NONE",KK_DIRECT | KK_CTRL | '6',fastRegMenuProc,NULL,xvm_frmenu_ctrlnum6 },
	{ L"NONE",L"NONE",KK_DIRECT | KK_CTRL | '7',fastRegMenuProc,NULL,xvm_frmenu_ctrlnum7 },
	{ L"NONE",L"NONE",KK_DIRECT | KK_CTRL | '8',fastRegMenuProc,NULL,xvm_frmenu_ctrlnum8 },
	{ L"NONE",L"NONE",KK_DIRECT | KK_ALT | '1',fastRegMenuProc,NULL,xvm_frmenu_altnum1 },
	{ L"NONE",L"NONE",KK_DIRECT | KK_ALT | '2',fastRegMenuProc,NULL,xvm_frmenu_altnum2 },
	{ L"NONE",L"NONE",KK_DIRECT | KK_ALT | '3',fastRegMenuProc,NULL,xvm_frmenu_altnum3 },
	{ L"NONE",L"NONE",KK_DIRECT | KK_ALT | '4',fastRegMenuProc,NULL,xvm_frmenu_altnum4 },
	{ L"NONE",L"NONE",KK_DIRECT | KK_ALT | '5',fastRegMenuProc,NULL,xvm_frmenu_altnum5 },
	{ L"NONE",L"NONE",KK_DIRECT | KK_ALT | '6',fastRegMenuProc,NULL,xvm_frmenu_altnum6 },
	{ L"NONE",L"NONE",KK_DIRECT | KK_ALT | '7',fastRegMenuProc,NULL,xvm_frmenu_altnum7 },
	{ L"NONE",L"NONE",KK_DIRECT | KK_ALT | '8',fastRegMenuProc,NULL,xvm_frmenu_altnum8 },
	{ L"NONE",L"NONE",KK_DIRECT | KK_CTRL | KK_ALT | '1',fastRegMenuProc,NULL,xvm_frmenu_shiftnum1 },
	{ L"NONE",L"NONE",KK_DIRECT | KK_CTRL | KK_ALT | '2',fastRegMenuProc,NULL,xvm_frmenu_shiftnum2 },
	{ L"NONE",L"NONE",KK_DIRECT | KK_CTRL | KK_ALT | '3',fastRegMenuProc,NULL,xvm_frmenu_shiftnum3 },
	{ L"NONE",L"NONE",KK_DIRECT | KK_CTRL | KK_ALT | '4',fastRegMenuProc,NULL,xvm_frmenu_shiftnum4 },
	{ L"NONE",L"NONE",KK_DIRECT | KK_CTRL | KK_ALT | '5',fastRegMenuProc,NULL,xvm_frmenu_shiftnum5 },
	{ L"NONE",L"NONE",KK_DIRECT | KK_CTRL | KK_ALT | '6',fastRegMenuProc,NULL,xvm_frmenu_shiftnum6 },
	{ L"NONE",L"NONE",KK_DIRECT | KK_CTRL | KK_ALT | '7',fastRegMenuProc,NULL,xvm_frmenu_shiftnum7 },
	{ L"NONE",L"NONE",KK_DIRECT | KK_CTRL | KK_ALT | '8',fastRegMenuProc,NULL,xvm_frmenu_shiftnum8 },
	{ NULL,NULL,K_NONE,NULL,NULL,0 }
};


enum {
	xvm_bytemenu_copyspace
	,xvm_bytemenu_copycstyle
	,xvm_bytemenu_copyplain
	,xvm_bytemenu_paste
	,xvm_bytemenu_copyaddr
	,xvm_bytemenu_copydword
	,xvm_bytemenu_null
	,xvm_bytemenu_return
	,xvm_bytemenu_altreturn
	,xvm_bytemenu_ctrlreturn
	,xvm_bytemenu_shiftreturn
};
static int BytesMenuProc(t_table *pt,wchar_t *name,ulong index,int mode)
{
	if (mode == MENU_VERIFY)
	{

		return MENU_NORMAL;                // Always available
	} else if (mode == MENU_EXECUTE)
	{
		t_dump *cpudump = (t_dump*)pt->customdata;
		if (&cpudump->table != pt)
			return MENU_ABSENT;

		switch (index)
		{
		case xvm_bytemenu_copyspace:
		{
			CopyToClipboard(cpudump,1);
		}break;
		case xvm_bytemenu_copycstyle:
		{
			CopyToClipboard(cpudump,2);
		}break;
		case xvm_bytemenu_copyplain:
		{
			CopyToClipboard(cpudump,0);
		}break;
		case xvm_bytemenu_paste:
		{
			 ParseFromClipboard(cpudump,0);
		}break;
		case xvm_bytemenu_copyaddr:
		{
			CopyInfoToClipboard(cpudump,0);
		}break;
		case xvm_bytemenu_copydword:
		{
			CopyInfoToClipboard(cpudump,1);
		}break;
		case xvm_bytemenu_return:
		{
			char buff[16];
			gLPReadmemory(buff,cpudump->sel0,0x4,MM_REMOVEINT3 | MM_SILENT);
			DWORD opdata = 0;
			glastDepWnd = pt->hw;
			if (cpudump->sel0 != 0 && (opdata = *(DWORD*)buff) != 0)
			{
				if (gLPReadmemory(buff,opdata,1,MM_SILENT) > 0)
				{
						gLPSetcpu(0,0,opdata,0,0,CPU_NOFOCUS + CPU_DUMPHIST);
				}
			}

		}break;
		case xvm_bytemenu_shiftreturn:{
			char buff[16];
			gLPReadmemory(buff,cpudump->sel0,0x4,MM_REMOVEINT3 | MM_SILENT);
			glastDepWnd = pt->hw;
			DWORD opdata = 0;
			if (cpudump->sel0 != 0 && (opdata = *(DWORD*)buff) != 0)
			{
				if (gLPReadmemory(buff,opdata,1,MM_SILENT) > 0)
				{
					gLPSetcpu(0,opdata,0,0,0,CPU_NOFOCUS + CPU_ASMHIST);
				}
			}
		}break;
		case xvm_bytemenu_altreturn:{
			char buff[16];
			gLPReadmemory(buff,cpudump->sel0,0x4,MM_REMOVEINT3 | MM_SILENT);
			glastDepWnd = pt->hw;
			DWORD opdata = 0;
			if (cpudump->sel0 != 0 && (opdata = *(DWORD*)buff) != 0)
			{
				if (gLPReadmemory(buff,opdata,1,MM_SILENT) > 0)
				{
					gLPSetcpu(0,0,0,0,opdata,CPU_NOFOCUS + CPU_ASMHIST);
				}
			}
		}break;
		}
		
	}
	return MENU_ABSENT;
}


static t_menu bytemenu[] = {
	{ L"Copy Bytes(space)",L"Copy Bytes(space)",KK_DIRECT + KK_CTRL+KK_SHIFT + 'C',BytesMenuProc,NULL,xvm_bytemenu_copyspace },
	{ L"Copy Bytes(C Style)",L"Copy Bytes(C Style)",KK_DIRECT + KK_CTRL + KK_SHIFT + 'S',BytesMenuProc,NULL,xvm_bytemenu_copycstyle },
	{ L"Copy Bytes(plain)",L"Copy Bytes(plain)",KK_DIRECT + KK_CTRL + KK_SHIFT + 'X',BytesMenuProc,NULL,xvm_bytemenu_copyplain },
	{ L"Copy Address",L"Copy Address",KK_DIRECT + KK_CTRL + 'X',BytesMenuProc,NULL,xvm_bytemenu_copyaddr },
	{ L"Copy Dword",L"Copy Dword",KK_DIRECT + KK_CTRL + KK_ALT+ 'X',BytesMenuProc,NULL,xvm_bytemenu_copydword },
	{ L"|Paste From Clipboard",L"Paste From Clipboard",KK_DIRECT + KK_CTRL + KK_SHIFT + 'V',BytesMenuProc,NULL,xvm_bytemenu_paste },
	{ L"NULL",L"NULL",K_NONE,BytesMenuProc,fastRegmenu,xvm_bytemenu_null },
	{ NULL,NULL,K_NONE,NULL,NULL,0 }
};

enum xvm_sandbox_menus{
	xvm_sb_filebox,
	xvm_sb_regbox,
	xvm_sb_enable,
	xvm_sb_script
};

static int SandBoxMenuProc(t_table *pt,wchar_t *name,ulong index,int mode)
{
	if (mode == MENU_VERIFY)
	{
		switch (index)
		{
		case xvm_sb_filebox:return (gxvmcfg.sbox_state & sandbox_state_file) ? MENU_CHECKED : MENU_NORMAL;
		case xvm_sb_regbox:return (gxvmcfg.sbox_state & sandbox_state_reg) ? MENU_CHECKED : MENU_NORMAL;
		case xvm_sb_enable:return (gxvmcfg.sbox_state & sandbox_state_enable) ? MENU_CHECKED : MENU_NORMAL;
		case xvm_sb_script:
		{
			if (gxvmcfg.sbox_script.empty())
			{
				wcscpy(name,L"Script:Empty...");
			} else
			{
				wsprintf(name,L"Script:%s",gxvmcfg.sbox_script.c_str());
			}
			return (gxvmcfg.sbox_state & sandbox_state_script) ? MENU_CHECKED : MENU_NORMAL;
		}break;
		}
		return MENU_NORMAL;                // Always available
	} else if (mode == MENU_EXECUTE)
	{
		switch (index)
		{
		case xvm_sb_filebox:
		{
			if (gxvmcfg.sbox_state & sandbox_state_file)
				gxvmcfg.sbox_state &= ~sandbox_state_file;
			else
				gxvmcfg.sbox_state |= sandbox_state_file;
		}break;

		case xvm_sb_regbox:
		{
			if (gxvmcfg.sbox_state & sandbox_state_reg)
				gxvmcfg.sbox_state &= ~sandbox_state_reg;
			else
				gxvmcfg.sbox_state |= sandbox_state_reg;
		}break;
		case xvm_sb_enable:
		{
			if (gxvmcfg.sbox_state & sandbox_state_enable)
				gxvmcfg.sbox_state &= ~sandbox_state_enable;
			else
				gxvmcfg.sbox_state |= sandbox_state_enable;
		}break;
		case xvm_sb_script:
		{
			if (HIBYTE(GetKeyState(VK_SHIFT)) || gxvmcfg.sbox_script.empty())
			{
				char lpf[MAX_PATH];
				if (getOpenScriptFileName(*ghwollymain,lpf) > 0)
				{
					std::string sf = lpf;
					gxvmcfg.sbox_script = SingleStringToWide(sf);
				} else
				{
					gxvmcfg.sbox_script.clear();
				}
			} else
			{
				if (gxvmcfg.sbox_state & sandbox_state_script)
					gxvmcfg.sbox_state &= ~sandbox_state_script;
				else
					gxvmcfg.sbox_state |= sandbox_state_script;
			}
		}break;
		}
	}
	return MENU_ABSENT;
}

static t_menu sandboxmenu[] = {
	{ L"Enable SandBox",L"Enable SandBox",KK_DIRECT | KK_CTRL| KK_ALT | KK_SHIFT | 'S',SandBoxMenuProc,NULL,xvm_sb_enable },
	{ L"|Enable File Box",L"Enable File Box",KK_DIRECT | KK_ALT | KK_SHIFT | 'F',SandBoxMenuProc,NULL,xvm_sb_filebox },
	{ L"Enable Registry Box",L"Enable Registry Box",KK_DIRECT | KK_ALT | KK_SHIFT | 'R',SandBoxMenuProc,NULL,xvm_sb_regbox },
	{ L"|Script:Empty...",L"Script:Empty...",K_NONE,SandBoxMenuProc,NULL,xvm_sb_script },
	{ NULL,NULL,K_NONE,NULL,NULL,0 }
};


static t_menu ijdllmenu[] = {
	{ L"Current Thread",L"Current Thread",K_NONE,MenuProc,NULL,xvm_menu_ijdllcthread },
	{ L"Remote Thread",L"Remote Thread",K_NONE,MenuProc,NULL,xvm_menu_ijdllrthread },
	{ L"Remote Virtual Mode",L"Current Virtual Mode",K_NONE,MenuProc,NULL,xvm_menu_ijdllvr },
	{ L"|Startup:Empty...",L"Startup",K_NONE,MenuProc,NULL,xvm_menu_ijdllstartup },
	{ NULL,NULL,K_NONE,NULL,NULL,0 }
};
static t_menu scriptmenu[] = {
	{ L"From File...",L"Run a give c script file.",KK_DIRECT | KK_ALT | KK_SHIFT | 'R',MenuProc,NULL,xvm_menu_runscript },
	{ L"|Startup Script:Empty...",L"Loading a script at target process startup.",KK_DIRECT | KK_ALT | KK_SHIFT | 'S',MenuProc,NULL,xvm_menu_startupscript },
	{ NULL,NULL,K_NONE,NULL,NULL,xvm_menu_script1 },
	{ NULL,NULL,K_NONE,NULL,NULL,xvm_menu_script2 },
	{ NULL,NULL,K_NONE,NULL,NULL,xvm_menu_script3 },
	{ NULL,NULL,K_NONE,NULL,NULL,xvm_menu_script4 },
	{ NULL,NULL,K_NONE,NULL,NULL,xvm_menu_script5 },
	{ NULL,NULL,K_NONE,NULL,NULL,0 }
};
static t_menu xasmenu[] = {
	{ L"Select Xas From File...",L"Select Xas From File...",KK_DIRECT | KK_ALT | KK_SHIFT | 'A',MenuProc,NULL,xvm_menu_xas },
	{ NULL,NULL,K_NONE,NULL,NULL,xvm_menu_xas1 },
	{ NULL,NULL,K_NONE,NULL,NULL,xvm_menu_xas2 },
	{ NULL,NULL,K_NONE,NULL,NULL,xvm_menu_xas3 },
	{ NULL,NULL,K_NONE,NULL,NULL,xvm_menu_xas4 },
	{ NULL,NULL,K_NONE,NULL,NULL,xvm_menu_xas5 },
	{ NULL,NULL,K_NONE,NULL,NULL,0 }
};

static t_menu MiscMenu[] = {
	{ L"Block Exec",L"Execute Code By Block.",KK_DIRECT | KK_ALT | KK_SHIFT | 'B',MenuProc,NULL,xvm_menu_blockexec },
	{ L"Find IAT Calls",L"Find IAT Calls.",KK_DIRECT | KK_ALT | KK_SHIFT | 'I',MenuProc,NULL,xvm_menu_findiatcallsmerge },
	{ L"Color Current Context",L"Color Current Context",KK_DIRECT | KK_ALT | KK_SHIFT | 'C',MenuProc,NULL,xvm_menu_colorcontext },
	{ L"Enable Virtual Debug",L"Enable Virtual Debug",K_NONE,MenuProc,NULL,xvm_menu_enablevirtualdebug },
	{ NULL,NULL,K_NONE,NULL,NULL,0 }
};

static t_menu mainmenu[] = {
	{ L"VMTrace",L"Trace Target By VM Tech.",KK_DIRECT | KK_WIN | VK_F7,MenuProc,NULL,xvm_menu_vmtrace },
	{ L"VMTrace At Selection",L"Trace Target By VM Tech Start at Selection.",KK_DIRECT | KK_WIN | VK_F8,MenuProc,NULL,xvm_menu_vmtracesel },
	{ L"VMTrace(real)",L"Trace Target In Real World.",KK_DIRECT | KK_WIN | VK_F9,MenuProc,NULL,xvm_menu_vmtracereal },
	{ L"Run To Api Entry",L"Run To Api Entry.",KK_DIRECT | KK_WIN | VK_F6,MenuProc,NULL,xvm_menu_apientry },
	{ L"Run To Call Ret.",L"Run To Call Ret.",KK_DIRECT | KK_WIN | VK_F5,MenuProc,NULL,xvm_menu_apiret },
	{ L"|Remote IDA",L"Remote IDA",K_NONE,MenuProc,idamenu,xvm_menu_remoteida },
	{ L"Script",L"Script",K_NONE,MenuProc,scriptmenu,xvm_menu_script },
	{ L"InjectDLL",L"InjectDLL",K_NONE,MenuProc,ijdllmenu,xvm_menu_injectdll },
	//{ L"SandBox",L"SandBox",K_NONE,MenuProc,sandboxmenu,xvm_menu_sandbox },
	{ L"|Miscellaneous",L"Miscellaneous",K_NONE,MenuProc,MiscMenu,xvm_menu_misc },
    { L"|Options",L"Options.", KK_DIRECT|KK_ALT |KK_SHIFT |'O', MenuProc, NULL, xvm_menu_options },
    { L"|About...",L"About xVMDebug plugin", K_NONE, MenuProc, NULL, xvm_menu_about },
    { NULL, NULL, K_NONE, NULL, NULL, 0 }
};

static t_menu memorymenu[] = {
    { L"Find IAT Calls",L"Find IAT Calls.", K_NONE, MenuProc, NULL, xvm_menu_findiatcalls },
    { L"Find IAT Calls(Merge)",L"Find IAT Calls(Merge).", K_NONE, MenuProc, NULL, xvm_menu_findiatcallsmerge },
    { L"Set End Of Image(IATTrace)",L"Set End Of Image(IATTrace)", K_NONE, MenuProc, NULL, xvm_menu_setendofiatmemaddr },

    { NULL, NULL, K_NONE, NULL, NULL, 0 }
};


static t_menu dumpmenu[] = {
    { L"Add to IAT",L"Add to IAT.", K_NONE, MenuProc, NULL, xvm_menu_iatmemaddr },
	{ L"Add To Block Entries",L"Made these address as block entries",K_NONE,MenuProc,NULL,xvm_menu_blockentrys },
    { L"Set this area as IAT",L"Set this area as IAT", K_NONE, MenuProc, NULL, xvm_menu_setIAT },
    { L"Trace and Fix this area",L"Trace and Fix this area", K_NONE, MenuProc, NULL, xvm_menu_traceFixIAT },
	{ L"|Clipboard",L"Clipboard",K_NONE,BytesMenuProc,bytemenu,xvm_menu_byte },
	{ L"|Follow In Dump",L"Follow In Dump",KK_DIRECT + VK_RETURN,BytesMenuProc,NULL,xvm_bytemenu_return },
	{ L"Follow In Asm",L"Follow In Asm",KK_DIRECT +KK_SHIFT+ VK_RETURN,BytesMenuProc,NULL,xvm_bytemenu_shiftreturn },
	{ L"Follow In Stack",L"Follow In Stack",KK_DIRECT + KK_ALT + VK_RETURN,BytesMenuProc,NULL,xvm_bytemenu_altreturn },
    { NULL, NULL, K_NONE, NULL, NULL, 0 }
};


static t_menu cpumenu[] = {
    { L"Set This IP As Branching",L"Set This IP As Branching", K_NONE, MenuProc, NULL, xvm_menu_opbranching },
    { L"Analyser OPs At This IP",L"Analyser OPs At This IP", K_NONE, MenuProc, NULL, xvm_menu_analyserop },
    { L"Make Code Block",L"Make Code Block", K_NONE, MenuProc, NULL, xvm_menu_opbranching },
	{ L"|Clipboard",L"Clipboard",K_NONE,BytesMenuProc,bytemenu,xvm_menu_byte },
    { NULL, NULL, K_NONE, NULL, NULL, 0 }
};

static int StackMenuProc(t_table *pt,wchar_t *name,ulong index,int mode)
{
	if (mode == MENU_VERIFY)
	{
		switch (index)
		{
		case xvm_menu_altreturn:
		case xvm_menu_ctrlreturn:
		case xvm_menu_shiftreturn:
		case xvm_menu_return:
		case xvm_menu_gotoebp:
		case xvm_menu_gotoesp:return MENU_SHORTCUT;
		}
		return MENU_NORMAL;                // Always available
	} else if (mode == MENU_EXECUTE)
	{
		t_dump *cpustackdump = (t_dump*)pt->customdata;
		if (&cpustackdump->table == pt)
		{
			t_thread * pthread = gLPFindthread(cpustackdump->threadid);
			switch (index)
			{
			case xvm_menu_gotoesp:{
				if (pthread)
					gLPSetcpu(0,0,0,0,pthread->reg.r[REG_ESP],CPU_STACKFOCUS);
			}break;
			case xvm_menu_gotoebp:{
				if (pthread)
					gLPSetcpu(0,0,0,0,pthread->reg.r[REG_EBP],CPU_STACKFOCUS);
			}break;
			case xvm_menu_return:
			{
				char buff[16];
				DWORD opdata = 0;
				gLPReadmemory(buff,cpustackdump->sel0,0x4,MM_SILENT);
				if (cpustackdump->sel0 != 0 && (opdata = *(DWORD*)buff) != 0)
				{
					if (gLPFindmemory(opdata))
					{
						if (gLPFindmodule(opdata) == 0 && opdata >= cpustackdump->base && opdata <= (cpustackdump->base + cpustackdump->size) && (opdata & 3) == 0)
							gLPSetcpu(0,0,0,0,opdata,CPU_STACKFOCUS);
						else
							gLPSetcpu(0,opdata,0,0,0,CPU_NOFOCUS + CPU_ASMHIST);
					}
				}
			}break;
			case xvm_menu_altreturn:
			{
				char buff[16];
				DWORD opdata = 0;
				gLPReadmemory(buff,cpustackdump->sel0,0x4,MM_SILENT);
				if (cpustackdump->sel0 != 0 && (opdata = *(DWORD*)buff) != 0)
				{
					if (gLPFindmemory(opdata))
						gLPSetcpu(0,0,opdata,0,0,CPU_NOFOCUS + CPU_ASMHIST);
				}
			}break;
			case xvm_menu_ctrlreturn:
			{
				char buff[16];
				DWORD opdata = 0;
				gLPReadmemory(buff,cpustackdump->sel0,0x4,MM_SILENT);
				if (cpustackdump->sel0 != 0 && (opdata = *(DWORD*)buff) != 0)
				{
					if (gLPFindmemory(opdata))
						gLPSetcpu(0,opdata,0,0,0,CPU_NOFOCUS + CPU_ASMHIST);
				}
			}break;
			}
		}

	}
	return MENU_ABSENT;
}

static t_menu stackmenu[] = {
	{ L"Goto ESP",L"Goto ESP",KK_DIRECT | VK_ESCAPE,StackMenuProc,NULL,xvm_menu_gotoesp },
	{ L"Goto EBP",L"Goto EBP",KK_DIRECT | VK_OEM_3,StackMenuProc,NULL,xvm_menu_gotoebp },
	{ L"onReturn",L"onReturn",KK_DIRECT | VK_RETURN,StackMenuProc,NULL,xvm_menu_return },
	{ L"key",L"key",KK_DIRECT | KK_ALT | VK_RETURN,StackMenuProc,NULL,xvm_menu_altreturn },
	{ L"key",L"key",KK_DIRECT | KK_CTRL | VK_RETURN,StackMenuProc,NULL,xvm_menu_ctrlreturn },
	{ L"|Clipboard",L"Clipboard",K_NONE,BytesMenuProc,bytemenu,xvm_menu_byte },
	{ NULL,NULL,K_NONE,NULL,NULL,0 }
};

t_menu* cdecl ODBG2_Pluginmenu(wchar_t *type) {
    if (wcscmp(type,PWM_MAIN)==0)
    {
        return mainmenu;
    }else if (wcscmp(type,PWM_MEMORY) == 0)
    {
        return memorymenu;
    }else if (wcscmp(type,PWM_DUMP) == 0)
    {
        return dumpmenu;
	} else if (wcscmp(type,PWM_DISASM) == 0)
		return cpumenu;
	else if (wcscmp(type,PWM_STACK) == 0)
		return stackmenu;

    return NULL;                         // No menu
}



static int APPMenuProc(t_table *pt,wchar_t *name,ulong index,int mode)
{
	if (mode == MENU_VERIFY)
	{
		if(!pt) return MENU_ABSENT;
		switch (index)
		{
		case xvm_menu_showscroll:
		{
			return pt->hscroll ? MENU_CHECKED : MENU_NORMAL;
		}break;
		case xvm_menu_font1:
		case xvm_menu_font2:
		case xvm_menu_font3:
		case xvm_menu_font4:
		case xvm_menu_font5:
		case xvm_menu_font6:
		case xvm_menu_font7:
		case xvm_menu_font8:
		{
			int im = (index - xvm_menu_font1);
			wcscpy(name,gFont[im].name);
			return (pt->font == im) ? MENU_CHECKED : MENU_NORMAL;
		}break;
		case 	xvm_menu_colour1:
		case 	xvm_menu_colour2:
		case 	xvm_menu_colour3:
		case 	xvm_menu_colour4:
		case 	xvm_menu_colour5:
		case 	xvm_menu_colour6:
		case 	xvm_menu_colour7:
		case 	xvm_menu_colour8:
		{
			int im = (index - xvm_menu_colour1);
			wcscpy(name,gScheme[im].name);
			return (pt->scheme == im) ? MENU_CHECKED : MENU_NORMAL;
		}break;
		case 	xvm_menu_highlithing1:
		case 	xvm_menu_highlithing2:
		case 	xvm_menu_highlithing3:
		case 	xvm_menu_highlithing4:
		case 	xvm_menu_highlithing5:
		case 	xvm_menu_highlithing6:
		case 	xvm_menu_highlithing7:
		case 	xvm_menu_highlithing8:
		{
			int im = (index - xvm_menu_highlithing1);
			wcscpy(name,gHilite[im].name);
			return (pt->hilite == im) ? MENU_CHECKED : MENU_NORMAL;
		}break;
		}
		return MENU_NORMAL;                // Always available
	} else if (mode == MENU_EXECUTE)
	{
		if (!pt) return MENU_ABSENT;
		switch (index)
		{
		case xvm_menu_showscroll:
		{
			pt->hscroll = pt->hscroll ? FALSE : TRUE;
			return MENU_NORMAL;
		}break;
		case xvm_menu_font1:
		case xvm_menu_font2:
		case xvm_menu_font3:
		case xvm_menu_font4:
		case xvm_menu_font5:
		case xvm_menu_font6:
		case xvm_menu_font7:
		case xvm_menu_font8:
		{
			int im = (index - xvm_menu_font1);
			pt->font = im;
			return MENU_CHECKED;
		}break;
		case 	xvm_menu_colour1:
		case 	xvm_menu_colour2:
		case 	xvm_menu_colour3:
		case 	xvm_menu_colour4:
		case 	xvm_menu_colour5:
		case 	xvm_menu_colour6:
		case 	xvm_menu_colour7:
		case 	xvm_menu_colour8:
		{
			int im = (index - xvm_menu_colour1);
			pt->scheme = im;
			return MENU_CHECKED;
		}break;
		case 	xvm_menu_highlithing1:
		case 	xvm_menu_highlithing2:
		case 	xvm_menu_highlithing3:
		case 	xvm_menu_highlithing4:
		case 	xvm_menu_highlithing5:
		case 	xvm_menu_highlithing6:
		case 	xvm_menu_highlithing7:
		case 	xvm_menu_highlithing8:
		{
			int im = (index - xvm_menu_highlithing1);
			pt->hilite = im;
			return MENU_CHECKED;
		}break;
		}
		return MENU_NOREDRAW;
	};
	return MENU_ABSENT;
}


static t_menu fontmenu[] = {
	{ L"NONE",L"",K_NONE,APPMenuProc,NULL,xvm_menu_font1 },
	{ L"NONE",L"",K_NONE,APPMenuProc,NULL,xvm_menu_font2 },
	{ L"NONE",L"",K_NONE,APPMenuProc,NULL,xvm_menu_font3 },
	{ L"NONE",L"",K_NONE,APPMenuProc,NULL,xvm_menu_font4 },
	{ L"NONE",L"",K_NONE,APPMenuProc,NULL,xvm_menu_font5 },
	{ L"NONE",L"",K_NONE,APPMenuProc,NULL,xvm_menu_font6 },
	{ L"NONE",L"",K_NONE,APPMenuProc,NULL,xvm_menu_font7 },
	{ L"NONE",L"",K_NONE,APPMenuProc,NULL,xvm_menu_font8 },
	{ NULL,NULL,K_NONE,NULL,NULL,0 }
};

static t_menu colormenu[] = {
	{ L"NONE",L"",K_NONE,APPMenuProc,NULL,xvm_menu_colour1 },
	{ L"NONE",L"",K_NONE,APPMenuProc,NULL,xvm_menu_colour2 },
	{ L"NONE",L"",K_NONE,APPMenuProc,NULL,xvm_menu_colour3 },
	{ L"NONE",L"",K_NONE,APPMenuProc,NULL,xvm_menu_colour4 },
	{ L"NONE",L"",K_NONE,APPMenuProc,NULL,xvm_menu_colour5 },
	{ L"NONE",L"",K_NONE,APPMenuProc,NULL,xvm_menu_colour6 },
	{ L"NONE",L"",K_NONE,APPMenuProc,NULL,xvm_menu_colour7 },
	{ L"NONE",L"",K_NONE,APPMenuProc,NULL,xvm_menu_colour8 },
	{ NULL,NULL,K_NONE,NULL,NULL,0 }
};

static t_menu hilitemenu[] = {
	{ L"NONE",L"",K_NONE,APPMenuProc,NULL,xvm_menu_highlithing1 },
	{ L"NONE",L"",K_NONE,APPMenuProc,NULL,xvm_menu_highlithing2 },
	{ L"NONE",L"",K_NONE,APPMenuProc,NULL,xvm_menu_highlithing3 },
	{ L"NONE",L"",K_NONE,APPMenuProc,NULL,xvm_menu_highlithing4 },
	{ L"NONE",L"",K_NONE,APPMenuProc,NULL,xvm_menu_highlithing5 },
	{ L"NONE",L"",K_NONE,APPMenuProc,NULL,xvm_menu_highlithing6 },
	{ L"NONE",L"",K_NONE,APPMenuProc,NULL,xvm_menu_highlithing7 },
	{ L"NONE",L"",K_NONE,APPMenuProc,NULL,xvm_menu_highlithing8 },
	{ NULL,NULL,K_NONE,NULL,NULL,0 }
};

static t_menu appMenu[] = {
	{ L"show horizontal scroll",L"show horizontal scroll",K_NONE,APPMenuProc,NULL,xvm_menu_showscroll },
	{ L"|Font",L"Font",K_NONE,APPMenuProc,fontmenu,xvm_menu_font },
	{ L"Colours",L"Colours",K_NONE,APPMenuProc,colormenu,xvm_menu_colours },
	{ L"Highlithing",L"Highlithing",K_NONE,APPMenuProc,hilitemenu,xvm_menu_highlithing },
	{ NULL,NULL,K_NONE,NULL,NULL,0 }
};


t_menu* appearanceMenu = appMenu;


void fn_odbg_setint3( void* paddr,unsigned char* pbuf,int pszbuf,unsigned char pint3cmd)
{
    int pnbps = gbpoint->sorted.n;
    if (pnbps < 1)
        return;
    t_bpoint** pbps = (t_bpoint**)gbpoint->sorted.dataptr;
    for (int i=0;i<pnbps;i++)
    {
        t_bpoint* pbpone = pbps[i];
        if (pbpone->type & BP_SET)
        {
            if ((pbpone->addr >= (ulong)paddr) && (pbpone->addr <= ((ulong)paddr+pszbuf)))
            {
                int poft = pbpone->addr - (ulong)paddr;
                unsigned char porgchar = *(unsigned char*)(pbuf+poft);
                if (porgchar == 0xCC)
                    *(unsigned char*)(pbuf+poft) = pint3cmd;
            }
        }
    }

}

void fn_odbg_getint3( void* paddr,unsigned char* pbuf,int pszbuf,unsigned char pint3cmd )
{
    int pnbps = gbpoint->sorted.n;
    if (pnbps < 1)
        return;
    t_bpoint** pbps = (t_bpoint**)gbpoint->sorted.dataptr;
    for (int i=0;i<pnbps;i++)
    {
        t_bpoint* pbpone = pbps[i];
        if (pbpone->type & BP_SET)
        {
            if ((pbpone->addr >= (ulong)paddr) && (pbpone->addr <= ((ulong)paddr+pszbuf)))
            {
                int poft = pbpone->addr - (ulong)paddr;
                unsigned char porgchar = *(unsigned char*)(pbuf+poft);
                if (porgchar == pint3cmd)
                    *(unsigned char*)(pbuf+poft) = 0xCC;
            }
        }
    }
}

bool fn_odbg_checkint3( void* paddr,unsigned long pexceptcode )
{
    int pnbps = gbpoint->sorted.n;
    if (pnbps < 1)
        return FALSE;
    t_bpoint** pbps = (t_bpoint**)gbpoint->sorted.dataptr;
    for (int i=0;i<pnbps;i++)
    {
        t_bpoint* pbpone = pbps[i];
        if (pbpone->type & BP_SET)
        {
            if (pbpone->addr == (ulong)paddr)
            {
                //                if (pexceptcode == gmmconfig.m_int3ecode)
                //                    return TRUE;
            }
        }
    }
    return FALSE;
}
