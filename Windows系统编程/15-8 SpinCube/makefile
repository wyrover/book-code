!include <ntwin32.mak>

all: spintest.exe spincube.dll

spintest.obj: spintest.c
    $(cc) $(cdebug) $(cflags) $(cvarsdll) spintest.c

spincube.obj: spincube.c spincube.h
    $(cc) $(cdebug) $(cflags) $(cvarsdll) spincube.c

paint.obj: paint.c paint.h spincube.h
    $(cc) $(cdebug) $(cflags) $(cvarsdll) paint.c

spintest.res: spintest.rc spintest.h spintest.dlg
    $(rc) $(rcvars) -r -fo spintest.res spintest.rc

spincube.res: spincube.rc
    $(rc) $(rcvars) -r -fo spincube.res spincube.rc

spincube.lib: spincube.obj paint.obj spincube.def spincube.res
     $(implib) -machine:$(CPU) \
     -def:spincube.def   \
!IF "$(CPU)" == "i386"
     spincube.obj paint.obj \
!ENDIF
     -out:spincube.lib

spincube.dll: spincube.obj paint.obj spincube.def spincube.res
    $(link) $(linkdebug) $(dlllflags) \
    -base:0x1C000000     \
    -out:spincube.dll    \
    spincube.obj spincube.exp spincube.res paint.obj $(guilibsdll)

spintest.exe: spintest.obj spincube.lib spintest.res spintest.def
    $(link) $(linkdebug) $(guiflags) -out:spintest.exe spintest.obj spintest.res spincube.lib $(guilibsdll)
