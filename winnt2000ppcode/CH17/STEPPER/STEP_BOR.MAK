#
#   Borland makefile for STEPPER
#

stepper.exe: stepper.obj bpnode.obj debug.obj stepper.res
stepper.obj: stepper.c stepper.h rsrc.h
bpnode.obj: bpnode.c stepper.h
debug.obj: debug.c stepper.h
stepper.res: stepper.rc stepper.ico rsrc.h

!if !$d(BCROOT)
 BCROOT = $(MAKEDIR)\..
!endif
LIBDIR = $(BCROOT)\lib
INCDIR = $(BCROOT)\include

LIBS   = cw32mti import32 
OBJS   = stepper bpnode debug

CFLAGS = -c -a2 -H -WM -I$(INCDIR) -D_RTLDLL
LFLAGS = -aa -L$(LIBDIR)

!ifdef NODEBUG
 CFLAGS = $(CFLAGS) -O2
!else
 CFLAGS = $(CFLAGS) -Od -v
 LFLAGS = $(LFLAGS) -v
!endif

#
#   Define compiling rules
#

.c.obj:
	bcc32 -c $(CFLAGS) $<

.rc.res:
	brcc32 -i$(INCDIR) $&

.obj.exe:
	tlink32 -Tpe $(LFLAGS) c0w32 $(OBJS), $*,, $(LIBS),, $*

