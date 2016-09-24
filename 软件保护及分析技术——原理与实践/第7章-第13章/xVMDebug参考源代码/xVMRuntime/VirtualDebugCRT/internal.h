
extern BOOL		checkmutx();
//buf中0xE6强制搜索紧跟字节，0xE7模糊字节
extern char*	memsearch(HINSTANCE phinst,char* plpText,int pszText);
extern char*	memsearchall(char* pAddress,char* plpText,int pszText,int pType);
extern char*	hookcodemem(void* paddr,void* proc);
extern BOOL		unhookcodemem(void* pproc);
extern char*	vcall(char* plpFunc,int pnArg,...);
extern BOOL		dbglistening();
extern void		virusedmodule(BOOL pwipehdr);
extern void		injectclient();
extern int		runscript(const char* pscpfiles,const char* pcrtpath,const char* psyspath,const char* pincpath,const char* plibpath,const char* plibfiles,const char* plogfile = 0,void** ppinst = 0);
extern void		print(int paddr,int pcolor,char* pformat,...);

#define _V_EFG_	0
#define _V_EDI_	1
#define _V_ESI_	2
#define _V_EBP_	3
#define _V_ESP_	4
#define _V_EBX_	5
#define _V_EDX_	6
#define _V_ECX_	7
#define _V_EAX_	8



#define DEFILE_FUNC_ENTRY(name,func)	void* gfunc_core_##name = func;\
										void* gfunc_retp_##name = 0;\
										void func_entry_##name()\
										{\
											__asm__ __volatile__(	\
											"leave\n\t"	\
											"pusha\n\t"	\
											"pushf\n\t"	\
											"pushl %esp\n\t"	\
											"call *gfunc_core_"#name"\n\t"	\
											"popf\n\t"	\
											"popa\n\t"	\
											"jmp *gfunc_retp_"#name"\n\t"	\
											);	\
										}



#define DEFILE_FUNC_RETP(name,ptr)	gfunc_retp_##name = ptr;


// void __stdcall func_call_back(char** pvars)