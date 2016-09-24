#ifndef ollydbg2_plugin_h__
#define ollydbg2_plugin_h__
#include <string>
struct t_menu;
extern t_menu* appearanceMenu;
#define SHIFTED 0x8000

std::string WideStringToSigle(const std::wstring &singleString);
std::wstring SigleStringToWide(const std::string &singleString);

enum run_state
{
	run_run = 0,
	run_stepover,
	run_stepin
};
int VMDebug_TraceInReal(BOOL singleThread = FALSE,run_state state = run_run);
int VMDebug_DirectStepOver(BOOL singleThread = FALSE,run_state state = run_run);
std::string getxVMDebugFullPathA(const char* lpsub);
std::wstring getxVMDebugFullPathW(const wchar_t* lpsub);
int getPasmFile(TCHAR* lpFilename,HWND hP);

struct t_table;
BOOL isTableActive(t_table* tb);
#endif // ollydbg2_plugin_h__
