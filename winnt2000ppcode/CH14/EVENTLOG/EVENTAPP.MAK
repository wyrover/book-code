
# Nmake macros for building Windows 32-Bit apps
!include <win32.mak>

PROJ = EVENTAPP

all: $(PROJ).exe

.cpp.obj:
  $(cc) $(cdebug) $(cflags) $(cvars) $*.cpp

$(PROJ).res: $(PROJ).rc $(PROJ).h
    $(rc) $(rcflags) $(rcvars) /fo $(PROJ).res $(PROJ).rc

$(PROJ).EXE: $(PROJ).obj eventlog.obj $(PROJ).res
    $(link) $(linkdebug) $(guilflags) \
    $(PROJ).obj eventlog.obj $(PROJ).res $(guilibs) version.lib \
    -out:$(PROJ).exe $(MAPFILE)
