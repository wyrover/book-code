#
#  Builds the ENUMPROC DLL and its SHOWPROC client 
#  using Microsoft's NMAKE and command line tools
#

#
#   Set targets and dependencies
#

all: showproc.exe enumproc.dll
showproc.exe: showproc.obj enumproc.lib 
enumproc.lib: enumproc.obj 
enumproc.dll: enumproc.obj

#
#   Define macros for command line options and library modules
#

CFLAGS = -c -W3 -MD -DWIN32 -D_MT -D_DLL -D_X86_ -DWINVER=0x0400
LFLAGS = -incremental:no -pdb:none -nodefaultlib
SUBSYS = -subsystem:console,4
LIBS   = msvcrt.lib user32.lib kernel32.lib advapi32.lib
IMPLIB = enumproc.lib                          # used in building the EXE

!ifdef NODEBUG
CFLAGS = $(CFLAGS) -Ox
LFLAGS = $(LFLAGS) -release
!else
CFLAGS = $(CFLAGS) -Od -Z7
LFLAGS = $(LFLAGS) -debug:full -debugtype:cv
!endif

#
#   Define compiling rules
#

# to make header declare dllexport, define __DLL__
showproc.obj: showproc.c enumproc.h
    cl $(CFLAGS) $*.c

enumproc.obj: enumproc.c enumproc.h 
    cl $(CFLAGS) -D__DLL__ $*.c

.obj.lib:
    lib -machine:$(CPU) $*.obj -out:$*.lib -def:$*.def

.obj.exe:
    link $(LFLAGS) $(SUBSYS) -out:$*.exe $*.obj $(LIBS) $(IMPLIB)

.obj.dll:
    link $(LFLAGS) -DLL -entry:_DllMainCRTStartup@12 \
        -out:$*.dll $*.obj $(LIBS)


