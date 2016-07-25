#
# Builds the FOO program for Stepper to step through
# using Borland's MAKE and command line tools

all: foo.exe

!if !$d(BCROOT)
 BCROOT = $(MAKEDIR)\..
!endif
LIBDIR = $(BCROOT)\lib
INCDIR = $(BCROOT)\include

LIBS   = cw32 import32 
OBJS   = stepper bpnode debug

CFLAGS = -c -WC -H -I$(INCDIR)
LFLAGS = -Tpe -ap -L$(LIBDIR)

!ifdef NODEBUG
 CFLAGS = $(CFLAGS) -O2
!else
 CFLAGS = $(CFLAGS) -Od -v
 LFLAGS = $(LFLAGS) -v -s
!endif

.obj.exe:
    tlink32 $(LFLAGS) c0x32 $&, $&,, $(LIBS),,

.c.obj:
    bcc32 $(CFLAGS) $&.c

