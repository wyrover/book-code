# Nmake macros for building Windows 32-Bit apps

!include <ntwin32.mak>

all: memory.exe


# Update the object file if necessary

memory.obj: memory.c memory.h
    $(cc) $(cflags) $(cvars) $(cdebug) memory.c

nmmemcli.obj: nmmemcli.c
    $(cc) $(cflags) $(cvars) $(cdebug) nmmemcli.c

nmmemsrv.obj: nmmemsrv.c
    $(cc) $(cflags) $(cvars) $(cdebug) nmmemsrv.c


# Update the resources if necessary

memory.res: memory.rc memory.h
    rc $(rcflags) $(rcvars)  memory.rc

# Update the executable file if necessary, and if so, add the resource back in.

memory.exe: memory.obj nmmemcli.obj nmmemsrv.obj memory.res
    $(link) $(linkdebug) $(guiflags) -out:memory.exe     \
    memory.obj nmmemcli.obj nmmemsrv.obj memory.res $(guilibs) shell32.lib

clean:
    del *.obj
    del *.res
    del *.vcp
    del *.bak
    del *.bsc
    del *.sbr
