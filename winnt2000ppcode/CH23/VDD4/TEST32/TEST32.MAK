# Nmake macros for building Windows 32-Bit apps
!include <ntwin32.mak>

all: test32.exe

test32.rbj: test32.rc test32.h
    rc -r test32.rc
    cvtres -$(CPU) test32.res -o test32.rbj

test32.obj: test32.c test32.h
    $(cc) $(cflags) $(cvars) $(cdebug) test32.c

test32.exe: test32.obj test32.rbj test32.def hpscan32.lib
    $(cvtobj) $(cvtdebug) *.obj
    $(link) $(linkdebug) $(guiflags) -out:test32.exe\
        test32.obj test32.rbj hpscan32.lib $(guilibs)
