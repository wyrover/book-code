# Nmake macros for building Windows 32-Bit apps

!include <ntwin32.mak>

all: monkey.exe

# Update the resource if necessary

monkey.res: monkey.rc monkey.h
    rc $(rcflags) $(rcvars) -r -fo monkey.res monkey.rc

# Update the object file if necessary

monkey.obj: monkey.c monkey.h
    $(cc) $(cdebug) $(cflags) $(cvars) monkey.c

# Update the executable file if necessary, and if so, add the resource back in.

monkey.exe: monkey.obj monkey.res
    $(link) $(linkdebug) $(guiflags) -out:monkey.exe monkey.obj monkey.res $(guilibs) advapi32.lib
