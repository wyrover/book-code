#
#   Microsoft NMake file for STEPPER
#
#   NOTE:  The DEBUG.C file assumes that FOO.EXE was compiled
#   with Borland's tools.  To compile and run it with Microsoft
#   tools, you must make some simple modifications as described
# 	in the article.

all: stepper.exe
stepper.exe: stepper.obj bpnode.obj debug.obj stepper.rbj
stepper.obj: stepper.c stepper.h rsrc.h
bpnode.obj: bpnode.c stepper.h
debug.obj: debug.c stepper.h
stepper.rbj: stepper.rc stepper.ico rsrc.h

#
#   Define macros for command line options and library modules
#

CFLAGS = -c -W3 -MD -DWIN32 -D_MT -D_X86_ -DWINVER=0x0400
LFLAGS = -incremental:no -pdb:none -nodefaultlib
SUBSYS = -subsystem:windows,4
LIBS = libcmt.lib kernel32.lib user32.lib gdi32.lib shell32.lib
OBJS = debug.obj bpnode.obj

!ifdef NODEBUG
CFLAGS = $(CFLAGS) -Os
LFLAGS = $(LFLAGS) -release
!else
CFLAGS = $(CFLAGS) -Od -Z7
LFLAGS = $(LFLAGS) -debug:full -debugtype:cv
!endif

#
#   Define compiling rules
#

.c.obj:
     cl $(CFLAGS) $*.c

.rc.rbj:
   rc $*.rc

.obj.exe:
    link $(LFLAGS) $(SUBSYS) -out:$*.exe $*.obj $(OBJS) $*.res $(LIBS)
