#ifndef xApiWatch_h__
#define xApiWatch_h__

BOOL xApi_StepOver();
BOOL xApi_RunToReturn();
void restoreAllModuleNoneExec();
BOOL isHookedExecutionAddr(ULONG_PTR exceptaddr);
void resetApiWatch();
#endif // xApiWatch_h__
