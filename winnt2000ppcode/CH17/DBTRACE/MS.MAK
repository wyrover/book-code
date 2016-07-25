#
#   Microsoft NMake file for DEBUG TRACE
#

all: dbtrace.exe
dbtrace.exe: dbtrace.obj dbtrace.rbj 

#
#   Define macros for command line options and library modules
#

CFLAGS = -c -W3 -MD -DWIN32 -D_MT -D_DLL -D_X86_ -DWINVER=0x0400
LFLAGS = -incremental:no -pdb:none -nodefaultlib
SUBSYS = -subsystem:windows,4
LIBS   = msvcrt.lib user32.lib kernel32.lib comdlg32.lib gdi32.lib shell32.lib
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

.c.obj:
     cl $(CFLAGS) $*.c

.rc.rbj:
   rc $*.rc

.obj.exe:
    link $(LFLAGS) $(SUBSYS) -out:$*.exe $*.obj $*.res $(LIBS)
