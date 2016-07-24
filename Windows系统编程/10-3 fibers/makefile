# Fibers are supported on Windows NT 3.51 service pack 3 and beyond.

TARGETOS=WINNT
APPVER=4.0
!include <win32.mak>

#detect build time environment
!IF "$(OS)" == "Windows_NT"
all: fibers.exe
!else
all: WARN_MSG
!endif

WARN_MSG:
    @echo "This sample will work only on Windows NT"



fibers.obj: fibers.c
    $(cc) $(cflags) $(cvars) fibers.c

fibers.exe: fibers.obj
    $(link) $(conflags) -out:fibers.exe fibers.obj $(conlibs)
