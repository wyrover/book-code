# Nmake macros for building Windows 32-Bit apps

TARGETOS=WINNT

!include <ntwin32.mak>

all: uconvert.exe uconvert.hlp


# Update the resource if necessary
uconvert.res: uconvert.rc uconvert.h
    rc -r -fo uconvert.res uconvert.rc

# Update the online help file if necessary.
uconvert.hlp: uconvert.hpj uconvert.rtf
    if exist uconvert.PH del uconvert.PH
    $(hc) uconvert.hpj


# Update the object files if necessary

uconvert.obj: uconvert.c uconvert.h
    $(cc) $(cdebug) $(cflags) $(cvars) uconvert.c


install.obj: install.c install.h
    $(cc) $(cdebug) $(cflags) $(cvars) install.c

dialogs.obj: dialogs.c uconvert.h
    $(cc) $(cdebug) $(cflags) $(cvars) dialogs.c


# Update the executable file if necessary

uconvert.exe: uconvert.obj install.obj dialogs.obj uconvert.res
    $(link) $(linkdebug) $(guiflags) -out:uconvert.exe \
          uconvert.obj install.obj dialogs.obj uconvert.res $(guilibs) advapi32.lib shell32.lib
