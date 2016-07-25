#
#   Borland makefile for DEBUG TRACE
#

dbtrace.exe: dbtrace.obj dbtrace.res
dbtrace.obj: dbtrace.c dbtrace.h
dbtrace.res: dbtrace.rc dbtrace.h

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

.c.obj:
	bcc32 -c $(CFLAGS) $<

.rc.res:
	brcc32 -i$(INCDIR) $&

.obj.exe:
	tlink32 -Tpe $(LFLAGS) c0w32 $*, $*,, $(LIBS),, $*


