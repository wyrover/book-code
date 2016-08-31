.AUTODEPEND
BCC32   = bcc32
ILINK32 = ilink32
BRC32   = brc32

CFLAGS  = -c -tWM- -w -w-par -w-inl -W -a1 -Od
LFLAGS  = -aa -V4.0 -c -x -Gn
RFLAGS  = -X -R

STDOBJS = c0w32.obj
STDLIBS = import32.lib cw32.lib

$(EXEFILE) : $(OBJFILES) $(RESFILES)
   $(ILINK32) $(LFLAGS) $(OBJFILES) $(STDOBJS),$(EXEFILE),,$(LIBFILES) $(STDLIBS),$(DEFFILE),$(RESFILES)

clean:
   del *.obj *.res *.tds *.map
