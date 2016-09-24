
struct TCCState;
extern TCCState*	gscpInst;
void	FuckmmAntiHook();
void	OEPEntry(void* pcfgbuf);
BOOL	SetBreakPoint(void* pbpAddr);
BOOL	DeleteBreakPoint(void* pbpAddr);
BOOL	StartDebugListing();
int		fn_run_script(const char* pscpfiles,const char* pcrtpath,const char* psyspath,const char* pincpath,const char* plibpath,const char* plibfiles,const char* plogfile,TCCState** ppInst);
