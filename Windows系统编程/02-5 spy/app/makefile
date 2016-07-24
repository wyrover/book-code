# Some nmake macros for Win32 apps development

!include <ntwin32.mak>


# This line allows NMAKE to work as well

all: spy.exe

# Update the object file if necessary

spy.obj: spy.c spy.h spyfuncs.h 
    $(cc) $(cflags) $(cvars) $(cdebug) spy.c

hook.obj: hook.c spy.h spyfuncs.h 
    $(cc) $(cflags) $(cvars) $(cdebug) hook.c

dialogs.obj: dialogs.c spy.h spyfuncs.h
    $(cc) $(cflags) $(cvars) $(cdebug) dialogs.c

misc.obj: misc.c spy.h spyfuncs.h
    $(cc) $(cflags) $(cvars) $(cdebug) misc.c

wm.obj: wm.c spy.h spyfuncs.h 
    $(cc) $(cflags) $(cvars) $(cdebug) wm.c

wprintf.obj: wprintf.c spy.h spyfuncs.h 
    $(cc) $(cflags) $(cvars) $(cdebug) wprintf.c

# Update the resources if necessary

spy.res: spy.rc spy.h spyfuncs.h
    $(rc) $(rcflags) $(rcvars)  spy.rc

# Update the executable file if necessary, and if so, add the resource back in.

spy.exe: spy.obj hook.obj dialogs.obj misc.obj wm.obj wprintf.obj spy.res
    $(link) $(linkdebug) $(guiflags) -out:spy.exe   \
    spy.obj hook.obj dialogs.obj misc.obj wm.obj \
    wprintf.obj spy.res ..\dll\hook.lib $(guilibs) advapi32.lib
!IF ("$(TARGETLANG)" == "LANG_JAPANESE") && ("$(OS)" == "Windows_NT")
    rlman -p 932 -n 17 1 -a $*.exe $*.tok $*.exe
!ENDIF
