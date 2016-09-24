#ifndef XVMDEBUG_CRT_H
#define XVMDEBUG_CRT_H
#include <windows.h>

enum xvm_bp_type{
    xvm_bp_execute = 0x1,
    xvm_bp_access = 0x2,
    xvm_bp_write = 0x4
};


int EnableXVMDebug();
int xvm_setbreakpoint(ULONG_PTR addr,int size,xvm_bp_type type);
#endif // XVMDEBUG_CRT_H
