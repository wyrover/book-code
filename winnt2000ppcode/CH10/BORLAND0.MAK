#
#  Builds the ENUMPROC DLL and its SHOWPROC client 
#  using Borland's MAKE and command line tools
#

all: enumproc.dll showproc.exe

#
#   Define macros for command line options and library modules
#

!if !$d(BCROOT)
  BCROOT = $(MAKEDIR)\..
!endif
LIBDIR = $(BCROOT)\lib
INCDIR = $(BCROOT)\include

LIBS = cw32mti import32 
CFLAGS = -c -WM -I$(INCDIR) -D_RTLDLL
LFLAGS = -L$(LIBDIR)

!IFDEF NODEBUG
CFLAGS = $(CFLAGS) -O2
!ELSE
CFLAGS = $(CFLAGS) -Od -v
LFLAGS = $(LFLAGS) -v
!ENDIF

#
#   rules for building these programs
#

enumproc.obj: enumproc.c enumproc.h
    bcc32 $(CFLAGS) -WD $*.c

showproc.obj: showproc.c enumproc.h
    bcc32 $(CFLAGS) -W $*.c

showproc.exe: showproc.obj enumproc.lib 
    tlink32 -Tpe -ap $(LFLAGS) c0x32 $*, $*.exe, , \
        $(LIBS) enumproc

enumproc.dll: enumproc.obj 
    tlink32 -Tpd $(LFLAGS) c0d32 $*, $*.dll, , $(LIBS) 
    implib $*.lib $<



